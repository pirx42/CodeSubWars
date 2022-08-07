// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "boost/weak_ptr.hpp"

#include "OpenGLView.h"
#include "CSWIEquipmentVisualizer.h"

namespace CodeSubWars
{

  class CSWObject;
  class CSWActiveSonar;
  class CSWCamera;

  class CSWActiveSonar3DView : public OpenGLView
  {
    public:
      CSWActiveSonar3DView(QWidget* pParent, std::shared_ptr<CSWActiveSonar> m_pActiveSonar);
    
      virtual void draw();

    protected:
      virtual void mousePressEvent(QMouseEvent*);

      virtual void wheelEvent(QWheelEvent*);

      virtual void timerEvent(QTimerEvent*);

      std::shared_ptr<CSWCamera> m_pMainCamera;
      std::weak_ptr<CSWActiveSonar> m_pActiveSonar;
      std::weak_ptr<CSWObject> m_pSolidParent;
      Matrix44D m_matActiveSonarTCamera;
  };




  class CSWActiveSonarVisualizer : public QWidget,
                                   public CSWIEquipmentVisualizer
  {
    public:
      CSWActiveSonarVisualizer(QWidget* pParent, std::shared_ptr<CSWActiveSonar> pActiveSonar);
    
      virtual void updateContent();
    
    protected:
      virtual void paintEvent(QPaintEvent* pEvent);

      CSWActiveSonar3DView* m_pView;
  };

}