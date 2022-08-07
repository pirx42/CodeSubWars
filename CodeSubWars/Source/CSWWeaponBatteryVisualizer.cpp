// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWeaponBatteryVisualizer.h"
#include "CSWWeaponBattery.h"
#include "CSWWeapon.h"


namespace CodeSubWars
{

  CSWWeaponBatteryVisualizer::CSWWeaponBatteryVisualizer(QWidget* pParent, CSWWeaponBattery::PtrType pWeaponBattery)
  : QWidget(pParent),
    m_pWeaponBattery(pWeaponBattery),
    m_nOldNumWeapons(-1)
  {
    QVBoxLayout* pLayout = new QVBoxLayout(this); 
    pLayout->setSpacing(2);
    pLayout->setMargin(1);
    m_pLabel = new QLabel(this);
    m_pLabel->setTextFormat(Qt::PlainText);
    pLayout->addWidget(m_pLabel);

    m_pDetails = new QTextEdit(this);
    m_pDetails->setAutoFormatting(QTextEdit::AutoNone);
    m_pDetails->setLineWrapMode(QTextEdit::NoWrap);
    m_pDetails->setWordWrapMode(QTextOption::NoWrap);
    m_pDetails->setReadOnly(true);
  //  m_pDetails->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pLayout->addWidget(m_pDetails);
  
    setLayout(pLayout);
  }


  void CSWWeaponBatteryVisualizer::updateContent()
  {
    if (CSWWeaponBattery::PtrType pWeaponBattery = m_pWeaponBattery.lock())
    {
      if (m_nOldNumWeapons == pWeaponBattery->getNumInserted())
        return;
      m_nOldNumWeapons = pWeaponBattery->getNumInserted();
    
      std::stringstream ss;
      ss.precision(1);
      ss.setf(std::ios::fixed);
      ss << "Max. Weapons: " << pWeaponBattery->m_nMaxSize << "\n"
         << "Current contains (" << pWeaponBattery->getNumInserted() << "): \n";
      m_pLabel->setText(ss.str().c_str());
    
      std::stringstream ss2;
      CSWWeaponBattery::WeaponContainer::const_iterator it = pWeaponBattery->m_WeaponContainer.begin();
      for (; it != pWeaponBattery->m_WeaponContainer.end(); ++it)
      {
        ss2 << (*it)->getName() << "\n";
      }

      m_pDetails->setText(ss2.str().c_str());
      update();
    }  
  }

}