// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWPassiveSonarVisualizer.h"
#include "CSWPassiveSonar.h"
#include "CSWCamera.h"
#include "CSWISolid.h"
#include "CSWUtilities.h"
#include "glut.h"


namespace CodeSubWars
{

  CSWPassiveSonar3DView::CSWPassiveSonar3DView(QWidget* pParent, CSWPassiveSonar::PtrType pPassiveSonar)
  : OpenGLView(pParent),
    m_pMainCamera(CSWCamera::create("PassiveSonar", Matrix44D())),
    m_pPassiveSonar(pPassiveSonar),
    m_pSolidParent(pPassiveSonar->findFirstParent<CSWISolid>()),
    m_matPassiveSonarTCamera(Vector3D(1, 0, 0),
                             Vector3D(0, 0, 1),
                             Vector3D(0, -1, 0),
                             Vector3D(0, 400, 0))
  {
    m_pCamera = m_pMainCamera;
  }


  void CSWPassiveSonar3DView::draw()
  {
    CSWPassiveSonar::PtrType pPassiveSonar = m_pPassiveSonar.lock();
    CSWObject::PtrType pSolidParent = m_pSolidParent.lock();
    CSWISoundReceiver::PtrType pSoundReceiver = std::dynamic_pointer_cast<CSWISoundReceiver>(pPassiveSonar);
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pPassiveSonar || !pSoundReceiver || !pSolidParent || !pCam)
      return;
  
    pCam->setTransform(pSolidParent->getWorldTransform()*m_matPassiveSonarTCamera);
    pCam->calcWorldTransforms();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

      setupCameraAndLight(pCam);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //draw scan area
      glPushMatrix();
        Matrix44D mat2(pCam->getWorldTransform());
        mat2.getTranslation() = pPassiveSonar->getWorldTransform().getTranslation();
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
        gluDisk(pDisk, 0, 150, 25, 1);
        glColor3f(0, 0, 0.1);
        gluDisk(pDisk, 0, 100, 25, 1);
        glColor3f(0, 0, 0.3);
        gluDisk(pDisk, 0, 50, 25, 1);
        gluDeleteQuadric(pDisk);        
      glPopMatrix();

      glShadeModel(GL_SMOOTH);
      glDisable(GL_LIGHTING);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glLineWidth(4);
      glBegin(GL_LINES);
        glColor4d(0.3, 0.3, 0.6, 1);      
        glVertex3dv(pPassiveSonar->getWorldTransform().getTranslation().pData);
        glColor4d(0.3, 0.3, 0.6, 0);      
        glVertex3dv((pPassiveSonar->getWorldTransform().getTranslation() + pSoundReceiver->getDirection()*150.0).pData);
      glEnd();

  //    glDisable(GL_BLEND);
      glDisable(GL_DEPTH_TEST);

      //draw detected points
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);
        boost::circular_buffer<std::pair<Vector3D, double> >::const_iterator it = pPassiveSonar->m_LastScanValues.begin();
        for (; it != pPassiveSonar->m_LastScanValues.end(); ++it)
        {
          Vector3D vec = pPassiveSonar->getWorldTransform().getTranslation() + it->first*it->second;
          glColor3dv(CSWUtilities::calcColorR2G2B(it->second/150).pData);      
          glVertex3dv(vec.pData);
        }
      glEnd();

      ARSTD::OpenGLTools::paintTripod(pSolidParent->getWorldTransform(), 10);
  
      //draw world coord tripod in lower left corner
      drawWorldTripod();
  
    glPopAttrib();
  }


  void CSWPassiveSonar3DView::mousePressEvent(QMouseEvent* pEvent)
  {
    OpenGLView::mousePressEvent(pEvent);
    //disable moving with left mouse button
    m_bLeftPressed = false; 
  }


  void CSWPassiveSonar3DView::wheelEvent(QWheelEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
    {
      pEvent->ignore();
      OpenGLView::wheelEvent(pEvent);
      return;
    }
  
    pCam->setTransform(m_matPassiveSonarTCamera);
    OpenGLView::wheelEvent(pEvent);
    m_matPassiveSonarTCamera = pCam->getTransform();
  }


  void CSWPassiveSonar3DView::timerEvent(QTimerEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;
  
    pCam->setTransform(m_matPassiveSonarTCamera);
    OpenGLView::timerEvent(pEvent);
    m_matPassiveSonarTCamera = pCam->getTransform();
  }





  CSWPassiveSonarVisualizer::CSWPassiveSonarVisualizer(QWidget* pParent, CSWPassiveSonar::PtrType pPassiveSonar)
  : QWidget(pParent, NULL)
  {
    QHBoxLayout* pLayout = new QHBoxLayout(this); 
    pLayout->setMargin(1);
    m_pView = new CSWPassiveSonar3DView(this, pPassiveSonar);
    pLayout->addWidget(m_pView);
  }


  void CSWPassiveSonarVisualizer::updateContent()
  {
    update();
  }


  void CSWPassiveSonarVisualizer::paintEvent(QPaintEvent* pEvent)
  {
    m_pView->repaint();
  
    QWidget::paintEvent(pEvent);
  }

}