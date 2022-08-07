// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

//#include <boost/weak_ptr.hpp>

namespace CodeSubWars
{

  class CSWCamera;
  class CSWSettings;

  class OpenGLView : public QGLWidget
  {
      Q_OBJECT

    public:
      OpenGLView(QWidget* pParent, const QGLWidget* pShareWidget = 0, Qt::WindowFlags f = 0);

      virtual ~OpenGLView();

      virtual void draw() = 0;

      virtual QSize sizeHint() const;
      virtual QSize minimumSizeHint() const;

    protected:
      virtual double getWheelStepSize() const;
  
      virtual void initializeGL();
    
      virtual void resizeGL(int nWidth, int nHeight);

      virtual void paintGL();

      virtual void mousePressEvent(QMouseEvent*);

      virtual void mouseReleaseEvent(QMouseEvent*);

      virtual void mouseMoveEvent(QMouseEvent*);
    
      virtual void wheelEvent(QWheelEvent*);

      virtual void timerEvent(QTimerEvent*);

      void setupCameraAndLight(std::shared_ptr<CSWCamera> pCamera);
      void drawWorldTripod() const;
      void draw2D();

      std::pair<bool, Vector3D> getWorldPos(double fX, double fY, double fZ);
      std::pair<bool, Ray3D> getWorldPickRay(double fX, double fY);

      double m_fDeltaX;
      double m_fDeltaY;
      bool m_bLeftPressed;
      bool m_bMidPressed;
      bool m_bRightPressed;

      QPoint m_MouseStartPoint;
      int m_nTimerID;

      Matrix44D m_matCurrentProjection;

      std::weak_ptr<CSWCamera> m_pCamera;

      double m_fOldTime;
      boost::circular_buffer<double> m_FPSBuffer;
  };


}
