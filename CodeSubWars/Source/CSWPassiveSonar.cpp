// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWPassiveSonar.h"
#include "CSWSoundReceiver.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "CSWMap.h"
#include "CSWPassiveSonarVisualizer.h"
#include "glut.h"


namespace CodeSubWars
{

  CSWPassiveSonar::PtrType CSWPassiveSonar::create(const std::string& strName, const Matrix44D& matBaseTObject)
  {
    return PtrType(new CSWPassiveSonar(strName, matBaseTObject));
  }


  CSWPassiveSonar::~CSWPassiveSonar()
  {
  }


  QWidget* CSWPassiveSonar::getInformationWidget(QWidget* pParent)
  {
    if (!m_pVisualizer)
      m_pVisualizer = new CSWPassiveSonarVisualizer(pParent, std::dynamic_pointer_cast<CSWPassiveSonar>(getSharedThis()));
    return m_pVisualizer;
  }


  void CSWPassiveSonar::update()
  {
    CSWSonar::update();
  //  CSWMap::PtrType pMap = getMap();
  //  if (pMap)
  //  {
  //    //insert detected stuff into set map
  //    pMap->insertElement(getTargetPosition(), level);
  //  }
  }


  void CSWPassiveSonar::draw()
  {
    if (CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_PASSIVE_SONAR_SCAN)
    {
      //for debugging only
      glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        glLoadMatrixd(getWorldTransform().pData);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);

        glShadeModel(GL_SMOOTH);

        glDepthMask(false);
        glEnable(GL_DEPTH_TEST);

        double fValue = 0;
        int nNumValues = 0;
        boost::circular_buffer<std::pair<Vector3D, double> >::const_iterator it = m_LastScanValues.begin();
        for (; nNumValues < 10 && it != m_LastScanValues.end(); ++it, ++nNumValues) fValue += it->second;
        if (nNumValues)
          fValue /= nNumValues;
        fValue /= 150;
        if (fValue > 1)
          fValue = 1;
      
        if (getAngleOfBeam() < 85)
        {
          glBegin(GL_TRIANGLE_FAN);
            assert(m_Vertices.size() > 2);
          
            glColor4d(fValue, 0, 0.5, 0.25 + fValue*0.25);
            glVertex3dv(m_Vertices.front().pData);

            glColor4d(fValue, 0, 0.5, 0);
            std::vector<Vector3D>::const_iterator it = boost::next(m_Vertices.begin());
            for (; it != m_Vertices.end(); ++it)
            {
              glVertex3dv(it->pData);
            }

            glVertex3dv(boost::next(m_Vertices.begin())->pData);
          
          glEnd();
        }
        else 
        {
          glColor4d(fValue, 0, 0.5, 0.2 + fValue*0.2);
          GLUquadricObj* pQuad = gluNewQuadric();
          gluQuadricDrawStyle(pQuad, GLU_FILL);
          gluSphere(pQuad, 1000, 10, 10);
          gluDeleteQuadric(pQuad);
        }
      
        glDepthMask(true);
        glPopMatrix();
      glPopAttrib();
    }  
  }


  Vector3D CSWPassiveSonar::getAdditionalDirection(const Vector3D& vecCurrentDir, const Vector3D& vecUp) const
  {
    return vecCurrentDir;
  }


  const Vector3D& CSWPassiveSonar::getDirection() const
  {
    return CSWSonar::getDirection();
  }

  const double& CSWPassiveSonar::getAngleOfBeam() const
  {
    return m_pSoundReceiver->getAngleOfBeam();
  }


  void CSWPassiveSonar::setAngleOfBeam(const double& fAngle)
  {
    m_pSoundReceiver->setAngleOfBeam(fAngle);

    //rebuild displaying mesh
    if (getAngleOfBeam() >= 85)
      return;
    m_Vertices.clear();
    m_Vertices.push_back(Vector3D(0, 0, 0));
    double fLength = 1000;
    int nSides = 20;
    double fRadian = fLength*tan(ARSTD::getInRad(fAngle));
    for (int nCntSide = 0; nCntSide < nSides; ++nCntSide)
    {
      double fA = 2*PI*static_cast<double>(nCntSide)/static_cast<double>(nSides);
      m_Vertices.push_back(Vector3D(sin(fA)*fRadian, cos(fA)*fRadian, fLength));
    }
  }


  void CSWPassiveSonar::addSound(const Vector3D& vecWorldTPosEmitter, const double& fLevel)
  {
    m_pSoundReceiver->addSound(vecWorldTPosEmitter, getWorldTransform().getTranslation(), getDirection(), fLevel);
  }


  const double& CSWPassiveSonar::getLevel() const
  {
    return m_pSoundReceiver->getLevel();
  }


  void CSWPassiveSonar::reset()
  {
    findMaximum();
    m_LastScanValues.push_front(std::make_pair(getDirection(), getLevel()));
    m_pSoundReceiver->reset();
  }


  void CSWPassiveSonar::adjustDirectionToMaximum(const double& fTimeToFind)
  {
    setEnableAutomaticRotation(false);
    setScanDirectionMode(LOCAL_DIRECTION);
    setScanDirection(getTransform().getZAxis());
    m_fAdjustingEndTime = ARSTD::Time::getTime() + fTimeToFind;
    m_fMaximumLevel = -1;
    m_fLastLevel = -1;
    m_vecMaximumDirection = getScanDirection();
    m_bFoundMaximum = false;
    m_bFindingMaximum = true;
    m_fStepSize = ARSTD::getInRad(0.5);
    m_nCurrentDirectionIdx = 0;
    m_nNumberBadDirections = 0;
    m_nNumberRestarts = 0;
    m_nNumberDirectionStepsWithoutNewMaximum = 0;
    m_nDirectionChangeOrientation = 1;
  }


  bool CSWPassiveSonar::isAdjusting() const
  {
    return m_bFindingMaximum;
  }


  bool CSWPassiveSonar::hasAdjusted() const
  {
    return m_bFoundMaximum || m_fAdjustingEndTime < ARSTD::Time::getTime();
  }


  CSWPassiveSonar::CSWPassiveSonar(const std::string& strName, const Matrix44D& matBaseTObject)
  : CSWSonar(strName, matBaseTObject),
    m_pSoundReceiver(CSWSoundReceiver::create()),
    m_LastScanValues(50),
    m_pVisualizer(NULL),
    m_fAdjustingEndTime(0),
    m_bFoundMaximum(false),
    m_bFindingMaximum(false)
  {
    setAngleOfBeam(15);
  
    m_Directions.push_back(std::make_pair( 0,  1));
    m_Directions.push_back(std::make_pair( 1,  1));
    m_Directions.push_back(std::make_pair( 1,  0));
    m_Directions.push_back(std::make_pair( 1, -1));
    m_Directions.push_back(std::make_pair( 0, -1));
    m_Directions.push_back(std::make_pair(-1, -1));
    m_Directions.push_back(std::make_pair(-1,  0));
    m_Directions.push_back(std::make_pair(-1,  1));
  }


  void CSWPassiveSonar::findMaximum()
  {
    if (hasAdjusted())
    { 
      if (m_bFindingMaximum && m_fMaximumLevel > 0)
      {
        //if time to find is over and a local maximum was found: set that maximum direction
        setScanDirection(m_vecMaximumDirection);
        update();
      }
    
      m_bFindingMaximum = false;
      return;
    }
  
    if (m_fMaximumLevel < getLevel())
    {
      m_fMaximumLevel = getLevel();
      m_nNumberDirectionStepsWithoutNewMaximum = 0;
      m_vecMaximumDirection = m_vecMainScanDirection;
      m_nMaximumDirectionIdx = m_nCurrentDirectionIdx;
    }
    else
    {
      ++m_nNumberDirectionStepsWithoutNewMaximum;
    }
  
    if (m_fLastLevel < getLevel())
    {
      m_nNumberBadDirections = 0;
    }
    else
    {
      ++m_nNumberBadDirections;
      m_nCurrentDirectionIdx += m_nDirectionChangeOrientation;
    }
    m_fLastLevel = getLevel();

    if (m_nNumberDirectionStepsWithoutNewMaximum >= 2*(m_Directions.size() + 1))
    {
      //restart from last good direction with new delta direction
      setScanDirection(m_vecMaximumDirection);
      update();
      m_nCurrentDirectionIdx = m_nMaximumDirectionIdx + m_nNumberRestarts + 1;
      ++m_nNumberRestarts;
    }

    if (m_nNumberDirectionStepsWithoutNewMaximum >= 4*(m_Directions.size() + 1) || m_nNumberRestarts >= m_Directions.size() + 1)
    {
      setScanDirection(m_vecMaximumDirection);
      update();
      m_bFoundMaximum = true;
      m_bFindingMaximum = false;
      return;
    }
  
    m_nCurrentDirectionIdx %= m_Directions.size();

    assert(m_nCurrentDirectionIdx >= 0 && m_nCurrentDirectionIdx < m_Directions.size());  
    m_vecMainScanDirection = m_vecMainScanDirection.getRotated(m_vecMainScanUp, -m_Directions[m_nCurrentDirectionIdx].first*m_fStepSize);
    Vector3D vecLeft = m_vecMainScanUp % m_vecMainScanDirection;
    m_vecMainScanDirection = m_vecMainScanDirection.getRotated(vecLeft, -m_Directions[m_nCurrentDirectionIdx].second*m_fStepSize);
    m_vecMainScanUp = m_vecMainScanDirection % vecLeft;
  }

}