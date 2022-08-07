// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWCamera.h"
#include "OpenGLView.h"
#include "glut.h"


namespace CodeSubWars
{

  OpenGLView::OpenGLView(QWidget* parent, const QGLWidget* pShareWidget, Qt::WindowFlags f)
  : QGLWidget(parent, pShareWidget, f),
    m_bLeftPressed(false),
    m_bMidPressed(false),
    m_bRightPressed(false),
    m_fOldTime(ARSTD::Time::getRealTime()),
    m_FPSBuffer(5)
  {
  }


  OpenGLView::~OpenGLView()
  {
  }


  QSize OpenGLView::sizeHint() const
  {
    return QSize(700, 400);
  }


  QSize OpenGLView::minimumSizeHint() const
  {
    return QSize(100, 100);
  }


  double OpenGLView::getWheelStepSize() const
  {
    return 0.5;
  }


  void OpenGLView::initializeGL()
  {
    QGLWidget::initializeGL();
    // Set up the rendering context, define display lists etc.:
    glClearColor(0, 0, 0, 0);

    glDepthFunc(GL_LESS);

    // enable textoutput
    HDC hdc = wglGetCurrentDC(); 
    SelectObject(hdc, GetStockObject(SYSTEM_FONT)); 
    wglUseFontBitmaps(hdc, 0, 256, 1000); 
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH_HINT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glShadeModel(GL_FLAT);
  }


  void OpenGLView::resizeGL(int nWidth, int nHeight)
  {
    QGLWidget::resizeGL(nWidth, nHeight);
    glViewport(0, 0, static_cast<GLint>(nWidth), static_cast<GLint>(nHeight));
  }


  void OpenGLView::paintGL()
  {
    if (!isVisible())
      return;
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
    {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1, 1, 0, -1000, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glDisable(GL_LIGHTING);
        glShadeModel(GL_FLAT);

        glColor3d(1, 1, 1);
        glBegin(GL_LINES);
          glVertex2d(0, 0);  glVertex2d(1, 1);
          glVertex2d(0, 1);  glVertex2d(1, 0);
        glEnd();
      glPopAttrib();
    }
    else
    {
      draw();
    }

    draw2D();

    glFlush();
  }


  void OpenGLView::mousePressEvent(QMouseEvent* ev)
  { 
    m_MouseStartPoint = ev->pos();
    m_fDeltaX = 0;
    m_fDeltaY = 0;

    switch(ev->button())
    {
      case Qt::LeftButton :
        m_bLeftPressed = true; 
        break;
    
      case Qt::RightButton :
        m_bRightPressed = true; 
        break;
    
      case Qt::MidButton :
        m_bMidPressed = true; 
        break;
    }                               

    m_nTimerID = startTimer(40);
  }


  void OpenGLView::mouseReleaseEvent(QMouseEvent* ev)
  {   
    switch(ev->button())
    {
      case Qt::LeftButton :
        m_bLeftPressed = false; 
        break;
    
      case Qt::RightButton :
        m_bRightPressed = false; 
        break;
    
      case Qt::MidButton :
        m_bMidPressed = false; 
        break;
    }
  
    killTimer(m_nTimerID);
  }


  void OpenGLView::mouseMoveEvent(QMouseEvent* ev)
  {
    if (m_bLeftPressed || m_bRightPressed || m_bMidPressed)
    {  
      m_fDeltaX = (ev->pos().x() - m_MouseStartPoint.x())*15.0 / static_cast<double>(this->width());
      m_fDeltaY = (ev->pos().y() - m_MouseStartPoint.y())*15.0 / static_cast<double>(this->height());
  
      if (m_fDeltaX < 0) 
      {
        m_fDeltaX *= -m_fDeltaX;
      }
      else 
      {
        m_fDeltaX *= m_fDeltaX;
      }
  
      if (m_fDeltaY < 0) 
      {
        m_fDeltaY *= -m_fDeltaY;
      }
      else 
      {
        m_fDeltaY *= m_fDeltaY;
      }
    }
  }


  void OpenGLView::wheelEvent(QWheelEvent* pEvent)
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
    {
      pEvent->ignore();
      QGLWidget::wheelEvent(pEvent);
      return;
    }

    pEvent->accept();
    pCam->getTransform().getTranslation() += pCam->getTransform().getZAxis()*static_cast<double>(pEvent->delta())*getWheelStepSize();

    QGLWidget::wheelEvent(pEvent);
  }


  void OpenGLView::timerEvent(QTimerEvent* pTimerEvent)
  {   
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam || (m_fDeltaX == 0 && m_fDeltaY == 0))
      return;

    Matrix44D mat(true);
    Matrix44D matPatientTCam = pCam->getTransform();
  
    // translation in view direction
    if (m_bMidPressed)
    {
  //    pCam->moveAlongViewDir(m_fDeltaX);
      matPatientTCam.getTranslation() -= matPatientTCam.getZAxis()*m_fDeltaY;
    }  
    else if (m_bRightPressed) // rotation
    {
      Vector3D vecRotationCenter = pCam->getRotationCenter();
      matPatientTCam.getTranslation() -= vecRotationCenter;

      Vector3D vecRotationAxis(matPatientTCam.getYAxis());
      vecRotationAxis.normalize();
      mat.setRotation(vecRotationAxis, -m_fDeltaX*PI/180);
      matPatientTCam = mat*matPatientTCam;
    
      vecRotationAxis = matPatientTCam.getXAxis();
      vecRotationAxis.normalize();
      mat.setRotation(vecRotationAxis, m_fDeltaY*PI/180);    
      matPatientTCam = mat*matPatientTCam;
    
      matPatientTCam.getTranslation() += vecRotationCenter;
    }    
    else if (m_bLeftPressed)
    {
  //    pCam->moveAlongUpDir(m_fDeltaX);
  //    pCam->moveAlongRightDir(m_fDeltaY);
      // x axis translation
      Vector3D xAxis = matPatientTCam.getXAxis();
      xAxis *= m_fDeltaX;
      // add xAxis to translation 
      mat.getTranslation() += xAxis;
    
      // y axis translation
      Vector3D yAxis = matPatientTCam.getYAxis();
      yAxis *= m_fDeltaY;
		  //set translation 
      mat.getTranslation() += yAxis;
    
      matPatientTCam = mat*matPatientTCam;
    }

    pCam->setTransform(matPatientTCam);
  }


  void OpenGLView::setupCameraAndLight(CSWCamera::PtrType pCamera)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pCamera->getFieldOfView(), static_cast<GLdouble>(width())/static_cast<GLdouble>(height()), 1, 20000);

    Vector3D vecPos(pCamera->getEyePosition());
    Vector3D vecDir(pCamera->getDirection());       
    Vector3D vecUp(pCamera->getUpDirection());
  
    gluLookAt(vecPos.x, vecPos.y, vecPos.z, 
              vecPos.x + vecDir.x, vecPos.y + vecDir.y, vecPos.z + vecDir.z, 
              vecUp.x, vecUp.y, vecUp.z);

    glGetDoublev(GL_PROJECTION_MATRIX, m_matCurrentProjection.pData);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  
    GLfloat lightPosition[] = {static_cast<GLfloat>(vecPos.x), static_cast<GLfloat>(vecPos.y), static_cast<GLfloat>(vecPos.z), 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Vector4F(1, 1, 1, 1).pData);
  }


  void OpenGLView::drawWorldTripod() const
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      double fHToW = static_cast<double>(height())/width();
      glOrtho(0, 1, fHToW, 0, -1000, 1000);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
        glLoadIdentity();

        pCam->setWorldTransform(pCam->calcRootTObject());
        Matrix44D matCameraTWorld(pCam->getInvertWorldTransform());
        matCameraTWorld.getTranslation() = Vector3D(0.1*fHToW, 0.9*fHToW, 0);

        //not the best way :( ... but it works
        matCameraTWorld.getXAxis() *= -1;
        matCameraTWorld.getYAxis() *= -1;
        matCameraTWorld.getZAxis() *= -1;
        ARSTD::OpenGLTools::paintTripod(matCameraTWorld, 0.07*fHToW);
      glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
  }


  void OpenGLView::draw2D()
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 1, 0, -1000, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glShadeModel(GL_FLAT);

      double fCurrentTime = ARSTD::Time::getRealTime();
      m_FPSBuffer.push_back(1.0/(fCurrentTime - m_fOldTime));
      m_fOldTime = fCurrentTime;
    
      double fAvgFPS = std::accumulate(m_FPSBuffer.begin(), m_FPSBuffer.end(), 0.0) / m_FPSBuffer.capacity();

      std::stringstream ss;
      ss.setf(std::ios::fixed);
      ss << std::setprecision(2);
      if (CSWCamera::PtrType pCam = m_pCamera.lock())
        ss << pCam->getName() << ", ";
      else
        ss << "no camera selected, ";
      ss << "FPS:  " << fAvgFPS;
      glColor3d(1, 1, 1);
      glRasterPos2d(0.02, 0.05); 
      glListBase(1000);
      glCallLists(static_cast<GLsizei>(ss.str().size()), GL_UNSIGNED_BYTE, ss.str().c_str()); 
    glPopAttrib();
  }


  std::pair<bool, Vector3D> OpenGLView::getWorldPos(double fX, double fY, double fZ)
  {
    int viewport[4];
    makeCurrent();
    glGetIntegerv(GL_VIEWPORT, viewport);
    Vector3D vecWorldTPosition;
    int result = gluUnProject(fX, viewport[3] - fY, fZ, Matrix44D::IDENTITY.pData, m_matCurrentProjection.pData, viewport, &vecWorldTPosition.x, &vecWorldTPosition.y, &vecWorldTPosition.z);
    return std::make_pair(result == GL_TRUE, vecWorldTPosition);
  }


  std::pair<bool, Ray3D> OpenGLView::getWorldPickRay(double fX, double fY)
  {
    std::pair<bool, Vector3D> resultWorldTPos1 = getWorldPos(fX, fY, 0.0);
    if (!resultWorldTPos1.first)
      return std::make_pair(false, Ray3D(Vector3D(), Vector3D()));
    std::pair<bool, Vector3D> resultWorldTPos2 = getWorldPos(fX, fY, 1.0);
    if (!resultWorldTPos2.first)
      return std::make_pair(false, Ray3D(Vector3D(), Vector3D()));
    return std::make_pair(true, Ray3D(resultWorldTPos1.second, (resultWorldTPos2.second - resultWorldTPos1.second).getNormalized()));
  }

}

#include "OpenGLViewMoc.cpp"