// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

//#include "boost/weak_ptr.hpp"

#include "CSWIEquipmentVisualizer.h"

namespace CodeSubWars
{

  class CSWEngine;

  class CSWEngineVisualizer : public QWidget,
                              public CSWIEquipmentVisualizer
  {
    public:
      CSWEngineVisualizer(QWidget* pParent, std::shared_ptr<CSWEngine> pEngine);
    
      virtual void updateContent();
    
    protected:
      std::weak_ptr<CSWEngine> m_pEngine;
      QLabel* m_pLabel;
  };

}