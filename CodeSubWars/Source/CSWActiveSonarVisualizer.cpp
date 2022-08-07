// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWActiveSonarVisualizer.h"
#include "CSWActiveSonar.h"
#include "CSWCamera.h"
#include "CSWISolid.h"
#include "CSWUtilities.h"
#include "glut.h"

namespace CodeSubWars
{

  CSWActiveSonar3DView::CSWActiveSonar3DView(QWidget* pParent, CSWActiveSonar::PtrType pActiveSonar)
  : OpenGLView(pParent),
    m_pMainCamera(CSWCamera::create("ActiveSonar", Matrix44D())),
    m_pActiveSonar(pActiveSonar),
    m_pSolidParent(pActiveSonar->findFirstParent<CSWISolid>()),
    m_matActiveSonarTCamera(Vector3D(1, 0, 0),
                            Vector3D(0, 0, 1),
                            Vector3D(0, -1, 0),
                            Vector3D(0, 5500, 0))
  {
    m_pCamera = m_pMainCamera;
  }


  void CSWActiveSonar3DView::draw()
  {
    CSWActiveSonar::PtrType pActiveSonar = m_pActiveSonar.lock();
    CSWObject::PtrType pSolidParent = m_pSolidParent.lock();
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pActiveSonar || !pSolidParent || !pCam)
      return;
  
    pCam->setTransform(pSolidParent->getWorldTransform()*m_matActiveSonarTCamera);
    pCam->calcWorldTransforms();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

      setupCameraAndLight(pCam);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //draw scan area
      glPushMatrix();
        Matrix44D mat2(pCam->getWorldTransform());
        mat2.getTranslation() = pActiveSonar->getWorldTransform().getTranslation();
        glLoadMatrixd(mat2.pData);
      
        glDisable(GL_BLEND);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);

        glShadeModel(GL_FLAT);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
      
        GLUquadricObj* pDisk = gluNewQuadric();
        gluQuadricDrawStyle(pDisk, GLU_FILL);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 4500, 25, 1);
        glColor3f(0, 0, 0.3);
        gluDisk(pDisk, 0, 4000, 25, 1);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 3500, 25, 1);
        glColor3f(0, 0, 0.3);
        gluDisk(pDisk, 0, 3000, 25, 1);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 2500, 25, 1);
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

      glShadeModel(GL_SMOOTH);
      glDisable(GL_LIGHTING);
      glEnable(GL_BLEND);

      glColor3d(0.3, 0.3, 0.6);
      glLineWidth(4);
      glBegin(GL_LINES);
        glVertex3dv(pActiveSonar->getWorldTransform().getTranslation().pData);
        glVertex3dv((pActiveSonar->getWorldTransform().getTranslation() + pActiveSonar->getDirection()*pActiveSonar->getMaximalDistance()).pData);
      glEnd();

      glDisable(GL_DEPTH_TEST);

      //draw detected points
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);
        boost::circular_buffer<CSWActiveSonar::ScanLine>::const_iterator it = pActiveSonar->m_LastScans.begin();
        for (; it != pActiveSonar->m_LastScans.end(); ++it)
        {
          glColor3dv(CSWUtilities::calcColorR2G2B(it->segment.getLength()/pActiveSonar->getMaximalDistance()).pData);      
          glVertex3dv(it->segment.getPoint2().pData);
        }
      glEnd();

      ARSTD::OpenGLTools::paintTripod(pSolidParent->getWorldTransform(), 100);
  
      //draw world coord tripod in lower left corner
      drawWorldTripod();
  
    glPopAttrib();
  }


  void CSWActiveSonar3DView::mousePressEvent(QMouseEvent* pEvent)
  {
    OpenGLView::mousePressEvent(pEvent);
    //disable moving with left mouse button
    m_bLeftPressed = false; 
  }


  void CSWActiveSonar3DView::wheelEvent(QWheelEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
    {
      pEvent->ignore();
      OpenGLView::wheelEvent(pEvent);
      return;
    }
  
    pCam->setTransform(m_matActiveSonarTCamera);
    OpenGLView::wheelEvent(pEvent);
    m_matActiveSonarTCamera = pCam->getTransform();
  }


  void CSWActiveSonar3DView::timerEvent(QTimerEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;
  
    pCam->setTransform(m_matActiveSonarTCamera);
    OpenGLView::timerEvent(pEvent);
    m_matActiveSonarTCamera = pCam->getTransform();
  }





  CSWActiveSonarVisualizer::CSWActiveSonarVisualizer(QWidget* pParent, CSWActiveSonar::PtrType pActiveSonar)
  : QWidget(pParent, NULL)
  {
    QHBoxLayout* pLayout = new QHBoxLayout(this); 
    pLayout->setMargin(1);
    m_pView = new CSWActiveSonar3DView(this, pActiveSonar);
    pLayout->addWidget(m_pView);
  }


  void CSWActiveSonarVisualizer::updateContent()
  {
    update();
  }


  void CSWActiveSonarVisualizer::paintEvent(QPaintEvent* pEvent)
  {
    m_pView->repaint();
  
    QWidget::paintEvent(pEvent);
  }

}