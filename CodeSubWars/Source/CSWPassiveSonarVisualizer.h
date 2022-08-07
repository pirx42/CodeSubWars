// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

//#include "boost/weak_ptr.hpp"

#include "OpenGLView.h"
#include "CSWIEquipmentVisualizer.h"

namespace CodeSubWars
{

  class CSWObject;
  class CSWPassiveSonar;
  class CSWCamera;

  class CSWPassiveSonar3DView : public OpenGLView
  {
    public:
      CSWPassiveSonar3DView(QWidget* pParent, std::shared_ptr<CSWPassiveSonar> m_pPassiveSonar);
    
      virtual void draw();
    
    protected:
      virtual void mousePressEvent(QMouseEvent*);

      virtual void wheelEvent(QWheelEvent*);

      virtual void timerEvent(QTimerEvent*);
    
      std::shared_ptr<CSWCamera> m_pMainCamera;
      std::weak_ptr<CSWPassiveSonar> m_pPassiveSonar;
      std::weak_ptr<CSWObject> m_pSolidParent;
      Matrix44D m_matPassiveSonarTCamera;
  };




  class CSWPassiveSonarVisualizer : public QWidget,
                                    public CSWIEquipmentVisualizer
  {
    public:
      CSWPassiveSonarVisualizer(QWidget* pParent, std::shared_ptr<CSWPassiveSonar> pPassiveSonar);
    
      virtual void updateContent();
    
    protected:
      virtual void paintEvent(QPaintEvent* pEvent);

      CSWPassiveSonar3DView* m_pView;
  };

}