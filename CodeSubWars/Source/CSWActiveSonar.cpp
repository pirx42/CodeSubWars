// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWActiveSonar.h"
#include "CSWActiveSonarVisualizer.h"
#include "CSWMap.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "CSWMessageCollisionObjects.h"
#include "CSWISolid.h"
#include "CSWIDynamic.h"
#include "CSWBorder.h"
#include "CSWSubmarine.h"
#include "CSWWeapon.h"
#include "CSWScanRay.h"

namespace CodeSubWars
{

  //maximum visible range is 3 km
  const double CSWActiveSonar::MAX_DISTANCE = 3000;

  //vertical angle is 30 degree
  const double CSWActiveSonar::VERTICAL_SCANNING_ANGLE = getInRad(15);
  //vertical rotation velocity is 1125 degree per second or scanning the whole vertical range with 37.5 Hz
  const double CSWActiveSonar::VERTICAL_ANGULAR_VELOCITY = getInRad(1125);


  CSWActiveSonar::PtrType CSWActiveSonar::create(const std::string& strName, const Matrix44D& matBaseTObject)
  {
    return PtrType(new CSWActiveSonar(strName, matBaseTObject));
  }


  CSWActiveSonar::~CSWActiveSonar()
  {
  }


  QWidget* CSWActiveSonar::getInformationWidget(QWidget* pParent)
  {
    if (!m_pVisualizer)
      m_pVisualizer = new CSWActiveSonarVisualizer(pParent, std::dynamic_pointer_cast<CSWActiveSonar>(getSharedThis()));
    return m_pVisualizer;
  }


  void CSWActiveSonar::update()
  {
    CSWSonar::update();

    validate();

    CSWMap::PtrType pMap = getMap();
    if (pMap && hasTargetDetected() && !isTargetBorder())
    {
      CSWMap::DangerLevel level = CSWMap::UNKNOWN;
      if (isTargetDynamic())
        level = CSWMap::LOW;
      if (isTargetSubmarine())
        level = CSWMap::MEDIUM;
      if (isTargetWeapon())
        level = CSWMap::HIGH;
      pMap->insertElement(getTargetPosition(), level);
    }
  }


  const double& CSWActiveSonar::getAngleOfBeam() const
  {
    return m_fAngleOfBeam;
  }


  void CSWActiveSonar::setAngleOfBeam(const double& fAngle)
  {
    m_fAngleOfBeam = fAngle;
  }


  void CSWActiveSonar::draw()
  {
    m_pScanRay->draw();
    
    if (CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_ACTIVE_SONAR_SCAN)
    {  
      //for debugging only
      glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        glLoadIdentity();
      
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);

        glShadeModel(GL_SMOOTH);

        glDepthMask(false);
        glEnable(GL_DEPTH_TEST);
      
        if (m_LastScans.size() > 1)
        {
          glBegin(GL_TRIANGLES);
            int nNumValues = 0;
            int nMaxIterations = 10;
            boost::circular_buffer<ScanLine>::const_iterator it = m_LastScans.begin();
            double fCurrentBlend = 0.2;
            int nTmp = std::min(nMaxIterations, static_cast<int>(m_LastScans.size()));
            double fBlendDelta = fCurrentBlend;
            if (nTmp)
              fBlendDelta = fCurrentBlend/nTmp;
            Vector3D vecP1;
            Vector3D vecP2;
            Vector3D vecP3;
            Vector3D vecP4;
            Vector4D col1;
            Vector4D col2;
            for (; nNumValues < nMaxIterations && it != boost::prior(m_LastScans.end()); ++it, ++nNumValues)
            {
              vecP1 = it->segment.getPoint1();
              vecP2 = it->segment.getPoint2();
              vecP3 = boost::next(it)->segment.getPoint1();
              vecP4 = boost::next(it)->segment.getPoint2();
              col1 = it->vecColor;
              col1.a += fCurrentBlend;
              col2 = boost::next(it)->vecColor;
              col2.a += fCurrentBlend;
            
              glColor4dv(col1.pData);
              glVertex3dv(vecP1.pData);
              glVertex3dv(vecP2.pData);
              glColor4dv(col2.pData);
              glVertex3dv(vecP3.pData);

              glVertex3dv(vecP4.pData);
              glVertex3dv(vecP3.pData);
              glColor4dv(col1.pData);
              glVertex3dv(vecP2.pData);
              fCurrentBlend -= fBlendDelta;
            }
          glEnd();
        }
        glPopMatrix();
        glDepthMask(true);
      glPopAttrib();
    }
  }


  Vector3D CSWActiveSonar::getAdditionalDirection(const Vector3D& vecCurrentDir, const Vector3D& vecUp) const
  {
    double fVerticalAngularVelocity = 0;
    double fVerticalScanningAngle = 0;

    if (m_bAutomaticRotationEnabled || (m_ScanDirectionMode != LOCAL_DIRECTION && m_ScanDirectionMode != GLOBAL_POSITION))
    {
      if (m_ScanVelocityMode == FAST)
      {
        fVerticalAngularVelocity = VERTICAL_ANGULAR_VELOCITY;
      }
      else if (m_ScanVelocityMode == SLOW)
      {
        fVerticalAngularVelocity = VERTICAL_ANGULAR_VELOCITY/3;
      }

      if (m_ScanRangeMode == NEAR_RANGE)
      {
        fVerticalScanningAngle = VERTICAL_SCANNING_ANGLE;
      }
      else if (m_ScanRangeMode == FAR_RANGE)
      {
        fVerticalScanningAngle = VERTICAL_SCANNING_ANGLE/4;
      }
    }
  
    double fCurrentTime = m_fActiveAutomaticRotationTimePeriod;
    if (m_ScanDirectionMode == FULL)
    {
      fVerticalAngularVelocity *= 2;
    }
    else if (m_ScanDirectionMode == FRONT)
    {
      fVerticalScanningAngle *= 2;
    }
    else if (m_ScanDirectionMode == BACK)
    {
      fVerticalScanningAngle *= 2;
    }
    else if (m_ScanDirectionMode == LOCAL_DIRECTION)
    {
      fCurrentTime = ARSTD::Time::getTime();
      fVerticalAngularVelocity *= 2;
    }
    else if (m_ScanDirectionMode == GLOBAL_POSITION)
    {
      fCurrentTime = ARSTD::Time::getTime();
      fVerticalAngularVelocity *= 2;
    }
  
    return vecCurrentDir.getRotated(vecUp % vecCurrentDir, fVerticalScanningAngle*sin(fVerticalAngularVelocity*fCurrentTime));
  }


  const double& CSWActiveSonar::getDistance() const
  {
    return m_fDistance;
  }


  const double& CSWActiveSonar::getMaximalDistance() const
  {
    return MAX_DISTANCE;
  }


  bool CSWActiveSonar::hasTargetDetected() const
  {
    return m_bTargetDetected;
  }


  bool CSWActiveSonar::isTargetDynamic() const
  {
    if (CSWObject::PtrType pObj = m_pDetectedObject.lock())
      return std::dynamic_pointer_cast<CSWIDynamic>(pObj) != nullptr;
    return false;
  }


  bool CSWActiveSonar::isTargetBorder() const
  {
    if (CSWObject::PtrType pObj = m_pDetectedObject.lock())
      return std::dynamic_pointer_cast<CSWBorder>(pObj) != nullptr;
    return false;
  }


  bool CSWActiveSonar::isTargetSubmarine() const
  {
    if (CSWObject::PtrType pObj = m_pDetectedObject.lock())
      return std::dynamic_pointer_cast<CSWSubmarine>(pObj) != nullptr;
    return false;
  }


  bool CSWActiveSonar::isTargetWeapon() const
  {
    if (CSWObject::PtrType pObj = m_pDetectedObject.lock())
      return std::dynamic_pointer_cast<CSWWeapon>(pObj) != nullptr;
    return false;
  }


  const Vector3D& CSWActiveSonar::getTargetPosition() const
  {
    return m_vecTargetPosition;
  }


  CSWActiveSonar::CSWActiveSonar(const std::string& strName, const Matrix44D& matBaseTObject)
  : CSWSonar(strName, matBaseTObject),
    m_fAngleOfBeam(45),
    m_fDistance(MAX_DISTANCE),
    m_bTargetDetected(false),
    m_vecTargetPosition(0, 0, 0),
    m_LastScans(50),
    m_pVisualizer(NULL),
    m_pScanRay(CSWScanRay::create(strName + ".ScanRay", Matrix44D(Vector3D(0, 0, 60)), MAX_DISTANCE - 60))
  {
    attach(m_pScanRay);
  }


  void CSWActiveSonar::validate()
  {
    double fVisualizingDistance;  
  
    double fDetectedDistance = m_pScanRay->getDetectedDistance() + 60;
    CSWObject::PtrType pDetectedObject = m_pScanRay->getIntersectingObject();    
    m_bTargetDetected = fDetectedDistance < MAX_DISTANCE && pDetectedObject;
    if (m_bTargetDetected)
    {
      if (std::dynamic_pointer_cast<CSWBorder>(pDetectedObject))
      {
        m_vecTargetPosition = m_pScanRay->getWorldTransform().getTranslation() + 
                              m_pScanRay->getWorldTransform().getZAxis()*fDetectedDistance;
        m_fDistance = fDetectedDistance;
      }
      else
      {
        //for simplification: on object detection return allways the position of the detected object
        m_vecTargetPosition = pDetectedObject->getWorldTransform().getTranslation();
        m_fDistance = (getWorldTransform().getTranslation() - m_vecTargetPosition).getLength();
      }
      fVisualizingDistance = fDetectedDistance;
      m_pDetectedObject = pDetectedObject;
    }
    else
    {
      m_fDistance = MAX_DISTANCE;
      m_vecTargetPosition = Vector3D(0, 0, 0);
      fVisualizingDistance = MAX_DISTANCE;
    }
  
    //store current scanning segment
    Vector3D vecVisualizingTargetPosition = getWorldTransform().getTranslation() + getDirection()*fVisualizingDistance;
    m_LastScans.push_front(ScanLine(Segment3D(getWorldTransform().getTranslation(), vecVisualizingTargetPosition),
                                    m_bTargetDetected && !isTargetBorder() ? Vector4D(1, 1, 0, 0.2) : Vector4D(0, 0, 0.6, 0)));
  }

}