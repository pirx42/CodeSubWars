// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

//#include "boost/weak_ptr.hpp"

#include "CSWIEquipmentVisualizer.h"

namespace CodeSubWars
{

  class CSWWeaponBattery;

  class CSWWeaponBatteryVisualizer : public QWidget,
                                     public CSWIEquipmentVisualizer
  {
    public:
      CSWWeaponBatteryVisualizer(QWidget* pParent, std::shared_ptr<CSWWeaponBattery> pWeaponBattery);
    
      virtual void updateContent();
    
    protected:
      std::weak_ptr<CSWWeaponBattery> m_pWeaponBattery;
      QLabel* m_pLabel;
      QTextEdit* m_pDetails;
    
      int m_nOldNumWeapons;
  };

}