// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "OpenGLView.h"
#include "CSWIEquipmentVisualizer.h"

namespace CodeSubWars
{

  class CSWMap;
  class CSWCamera;

  class CSWMap3DView : public OpenGLView
  {
    public:
      CSWMap3DView(QWidget* pParent, std::shared_ptr<CSWMap> pMap);
    
      virtual void draw();
    
    protected:
      virtual void mousePressEvent(QMouseEvent*);

      virtual void wheelEvent(QWheelEvent*);

      virtual void timerEvent(QTimerEvent*);
    
      static std::pair<Vector3D, Vector3D> determineColor(unsigned int nValue);
  
      std::shared_ptr<CSWCamera> m_pMainCamera;
      std::weak_ptr<CSWMap> m_pMap;
      Matrix44D m_matMapTCamera;
  };



  class CSWMapVisualizer : public QWidget,
                           public CSWIEquipmentVisualizer
  {
    public:
      CSWMapVisualizer(QWidget* pParent, std::shared_ptr<CSWMap> pMap);
    
      virtual void updateContent();
    
    protected:
      virtual void paintEvent(QPaintEvent* pEvent);

      CSWMap3DView* m_pView;
  };

}