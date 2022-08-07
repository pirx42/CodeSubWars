// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMapVisualizer.h"
#include "CSWMap.h"
#include "CSWCamera.h"
#include "glut.h"


namespace CodeSubWars
{

  CSWMap3DView::CSWMap3DView(QWidget* pParent, CSWMap::PtrType pMap)
  : OpenGLView(pParent),
    m_pMainCamera(CSWCamera::create("Map", Matrix44D())),
    m_pMap(pMap),
    m_matMapTCamera(Vector3D(1, 0, 0),
                    Vector3D(0, 0, 1),
                    Vector3D(0, -1, 0),
                    Vector3D(0, 3800, 0))
  {
    m_pCamera = m_pMainCamera;
  }


  void CSWMap3DView::draw()
  {
    CSWMap::PtrType pMap = m_pMap.lock();
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pMap || !pCam)
      return;
  
    pCam->setTransform(pMap->getWorldTransform()*m_matMapTCamera);
    pCam->calcWorldTransforms();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

      setupCameraAndLight(pCam);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //draw scan area
      glPushMatrix();
        Matrix44D mat2(pCam->getWorldTransform());
        mat2.getTranslation() = pMap->getWorldTransform().getTranslation();
        glLoadMatrixd(mat2.pData);
      
        glDisable(GL_BLEND);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);

        glShadeModel(GL_FLAT);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
      
        GLUquadricObj* pDisk = gluNewQuadric();
        gluQuadricDrawStyle(pDisk, GLU_FILL);
        glColor3f(0, 0, 0.3);
        gluDisk(pDisk, 0, 2000, 25, 1);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 1500, 25, 1);
        glColor3f(0, 0, 0.3);
        gluDisk(pDisk, 0, 1000, 25, 1);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 500, 25, 1);
        gluDeleteQuadric(pDisk);        
      glPopMatrix();

      //draw detected points with velocities
      glEnable(GL_BLEND);
      CSWMap::MapElementContainer::const_iterator it = pMap->m_Map.begin();
      for (; it != pMap->m_Map.end(); ++it)
      {
        //velocity
        glColor3d(0, 0.7, 0.6);
        glLineWidth(2);
        glBegin(GL_LINES);
          glVertex3dv(it->second.vecWorldTPosition.pData);
          glVertex3dv((it->second.vecWorldTPosition + it->second.vecWorldTVelocity).pData);
        glEnd();

        //position
        std::pair<Vector3D, Vector3D> colors = determineColor(it->second.nLevel);
        glColor3dv(colors.second.pData);      
        glPointSize(14);
        glBegin(GL_POINTS);
          glVertex3dv(it->second.vecWorldTPosition.pData);
        glEnd();

        glColor3d(0, 0, 0);      
        glPointSize(8);
        glBegin(GL_POINTS);
          glVertex3dv(it->second.vecWorldTPosition.pData);
        glEnd();

        glColor3dv(colors.first.pData);      
        glPointSize(6);
        glBegin(GL_POINTS);
          glVertex3dv(it->second.vecWorldTPosition.pData);
        glEnd();
      }

      glColor3f(1.0, 1.0, 1.0); 
      glListBase(1000);
      it = pMap->m_Map.begin();
      for (; it != pMap->m_Map.end(); ++it)
      {
        std::stringstream ss;
        ss.precision(1);
        ss.setf(std::ios::fixed);
        ss << "  " 
           << it->second.nID << "/"
           << (ARSTD::Time::getTime() - it->second.fTime);
        glRasterPos3dv(it->second.vecWorldTPosition.pData);
        glCallLists(static_cast<GLsizei>(ss.str().size()), GL_UNSIGNED_BYTE, ss.str().c_str()); 
      }
    
      ARSTD::OpenGLTools::paintTripod(pMap->getWorldTransform(), 100);
  
      //draw world coord tripod in lower left corner
      drawWorldTripod();
  
    glPopAttrib();
  }


  void CSWMap3DView::mousePressEvent(QMouseEvent* pEvent)
  {
    OpenGLView::mousePressEvent(pEvent);
    //disable moving with left mouse button
    m_bLeftPressed = false; 
  }


  void CSWMap3DView::wheelEvent(QWheelEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
    {
      pEvent->ignore();
      OpenGLView::wheelEvent(pEvent);
      return;
    }
  
    pCam->setTransform(m_matMapTCamera);
    OpenGLView::wheelEvent(pEvent);
    m_matMapTCamera = pCam->getTransform();
  }


  void CSWMap3DView::timerEvent(QTimerEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;
  
    pCam->setTransform(m_matMapTCamera);
    OpenGLView::timerEvent(pEvent);
    m_matMapTCamera = pCam->getTransform();
  }


  std::pair<Vector3D, Vector3D> CSWMap3DView::determineColor(unsigned int nValue)
  {
    if (nValue == CSWMap::UNKNOWN)
      return std::make_pair(Vector3D(1, 0, 0), Vector3D(1, 0, 0));
  
    Vector3D vecDangerLevelColor(0, 0, 0);
    if (nValue & CSWMap::NONE)
      vecDangerLevelColor = Vector3D(0, 1, 0);
    if (nValue & CSWMap::LOW)
      vecDangerLevelColor = Vector3D(1, 1, 0);
    if (nValue & CSWMap::MEDIUM)
      vecDangerLevelColor = Vector3D(1, 0.5, 0);
    if (nValue & CSWMap::HIGH)
      vecDangerLevelColor = Vector3D(1, 0, 0);

    Vector3D vecUserColor(0, 0, 0);
    unsigned int nUserValue = nValue & (~(CSWMap::NONE | CSWMap::LOW | CSWMap::MEDIUM | CSWMap::HIGH));
    static double fMax = ARSTD::log2(static_cast<double>(0xffffffff));
    double fVal = ARSTD::log2(static_cast<double>(nUserValue))/fMax;
    vecUserColor.g = fVal < 0.5 ? 1.0 : -2.0*fVal + 2;
    vecUserColor.b = fVal < 0.5 ? 2.0*fVal : 1.0;
    return std::make_pair(vecDangerLevelColor, vecUserColor);  
  }





  CSWMapVisualizer::CSWMapVisualizer(QWidget* pParent, CSWMap::PtrType pMap)
  : QWidget(pParent, NULL)
  {
    QHBoxLayout* pLayout = new QHBoxLayout(this); 
    pLayout->setMargin(1);
    m_pView = new CSWMap3DView(this, pMap);
    pLayout->addWidget(m_pView);
  }


  void CSWMapVisualizer::updateContent()
  {
    update();
  }


  void CSWMapVisualizer::paintEvent(QPaintEvent* pEvent)
  {
    m_pView->repaint();
  
    QWidget::paintEvent(pEvent);
  }

}