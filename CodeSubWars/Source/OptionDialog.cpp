// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "OptionDialog.h"


namespace CodeSubWars
{

  OptionDialog::OptionDialog(QWidget* pParent)
  : QDialog(pParent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    m_nCollidableProperties(CSWWorld::getInstance()->getSettings()->getCollidableProperties()),
    m_nDamageableProperties(CSWWorld::getInstance()->getSettings()->getDamageableProperties()),
    m_nDynamicProperties(CSWWorld::getInstance()->getSettings()->getDynamicProperties()),
    m_nVariousProperties(CSWWorld::getInstance()->getSettings()->getVariousProperties())
  {
    setWindowTitle("Preferences");
    setSizeGripEnabled(false);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(2);
    pMainLayout->setMargin(2);

      QTabWidget* pDetailTabWidget = new QTabWidget(this);
      pMainLayout->addWidget(pDetailTabWidget);

        //collideable widget
        QScrollArea* pSV = new QScrollArea;
        QWidget* pContent = new QWidget;
        QVBoxLayout* pVBoxLayout = new QVBoxLayout;

        m_pCollidableButtonGroup = new QButtonGroup(this);
        m_pCollidableButtonGroup->setExclusive(false);      
        connect(m_pCollidableButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(collidableClicked(QAbstractButton*)));

        pVBoxLayout->addWidget(createCheckBox(m_pCollidableButtonGroup, m_CollidableMap, m_nCollidableProperties, 
                                              CSWSettings::DISPLAY_COLLISION_MESH, "display collision meshes"));
        pVBoxLayout->addWidget(createCheckBox(m_pCollidableButtonGroup, m_CollidableMap, m_nCollidableProperties, 
                                              CSWSettings::DISPLAY_OBJECT_OBJECT_COLLISION, "display object-object collisions"));
        pVBoxLayout->addWidget(createCheckBox(m_pCollidableButtonGroup, m_CollidableMap, m_nCollidableProperties, 
                                              CSWSettings::DISPLAY_OBJECT_TRIGGER_COLLISION, "display object-trigger collisions"));
        pVBoxLayout->addWidget(createCheckBox(m_pCollidableButtonGroup, m_CollidableMap, m_nCollidableProperties, 
                                              CSWSettings::DISPLAY_TRIGGER_TRIGGER_COLLISION, "display trigger-trigger collisions"));

        pContent->setLayout(pVBoxLayout);
        pSV->setWidget(pContent);
        pDetailTabWidget->addTab(pSV, "Collision");
      
        //damageable widget
        pSV = new QScrollArea;
        pContent = new QWidget;
        pVBoxLayout = new QVBoxLayout;

        m_pDamageableButtonGroup = new QButtonGroup(this);
        m_pDamageableButtonGroup->setExclusive(false);      
        connect(m_pDamageableButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(damageableClicked(QAbstractButton*)));
      
        pVBoxLayout->addWidget(createCheckBox(m_pDamageableButtonGroup, m_DamageableMap, m_nDamageableProperties, 
                                              CSWSettings::DISPLAY_HEALTH, "display health"));

        pContent->setLayout(pVBoxLayout);
        pSV->setWidget(pContent);
        pDetailTabWidget->addTab(pSV, "Damageable");

        //dynamic widget
        pSV = new QScrollArea;
        pContent = new QWidget;
        pVBoxLayout = new QVBoxLayout;

        m_pDynamicButtonGroup = new QButtonGroup(this);
        m_pDynamicButtonGroup->setExclusive(false);      
        connect(m_pDynamicButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(dynamicClicked(QAbstractButton*)));
      
        pVBoxLayout->addWidget(createCheckBox(m_pDynamicButtonGroup, m_DynamicMap, m_nDynamicProperties, CSWSettings::DISPLAY_TOTAL_FORCE, "display total forces"));
        pVBoxLayout->addWidget(createCheckBox(m_pDynamicButtonGroup, m_DynamicMap, m_nDynamicProperties, CSWSettings::DISPLAY_DETAIL_FORCE, "display detail forces"));
        pVBoxLayout->addWidget(createCheckBox(m_pDynamicButtonGroup, m_DynamicMap, m_nDynamicProperties, CSWSettings::DISPLAY_TOTAL_IMPULS, "display total impulses"));
        pVBoxLayout->addWidget(createCheckBox(m_pDynamicButtonGroup, m_DynamicMap, m_nDynamicProperties, CSWSettings::DISPLAY_DETAIL_IMPULS, "display detail impulses"));
        pVBoxLayout->addWidget(createCheckBox(m_pDynamicButtonGroup, m_DynamicMap, m_nDynamicProperties, CSWSettings::DISPLAY_MASS_POINTS, "display mass points"));

        pContent->setLayout(pVBoxLayout);
        pSV->setWidget(pContent);
        pDetailTabWidget->addTab(pSV, "Dynamic");

        //various widget
        pSV = new QScrollArea;
        pContent = new QWidget;
        pVBoxLayout = new QVBoxLayout;
      
        m_pVariousButtonGroup = new QButtonGroup(this);
        m_pVariousButtonGroup->setExclusive(false);      
        connect(m_pVariousButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(variousClicked(QAbstractButton*)));
      
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::INITIALZE_SUBS_IN_PLANE, "initialize submarines in a plane"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::DISPLAY_ACTIVE_SONAR_SCAN, "display active sonar scans"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::DISPLAY_PASSIVE_SONAR_SCAN, "display passive sonar scans"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::DISPLAY_SOUND_EMITTING, "display sound emitting"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::INCREASE_VISIBILITY_OF_SMALL_OBJECTS, "increase visibility of small objects"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::STORE_WORLD_PERIODICALLY, "store world periodically"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::DISPLAY_OBJECT_TREE, "display object tree connections"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::DISPLAY_WEAPONS_IN_OBJECT_TREE, "display weapon objects in object list"));
        pVBoxLayout->addWidget(createCheckBox(m_pVariousButtonGroup, m_VariousMap, m_nVariousProperties, 
                                              CSWSettings::AUTOMATIC_BATTLE_STOP, "automatically stop battle if one submarine/team leftover"));

        pContent->setLayout(pVBoxLayout);
        pSV->setWidget(pContent);
        pDetailTabWidget->addTab(pSV, "Various");



      QHBoxLayout* pButtonsLayout = new QHBoxLayout;
      pMainLayout->addLayout(pButtonsLayout);

        pButtonsLayout->addStretch();
    
        QPushButton* pOkButton = new QPushButton(tr("Ok"), this);
        pOkButton->setFocus();
        pButtonsLayout->addWidget(pOkButton);
        connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));

        QPushButton* pCancelButton = new QPushButton(tr("Cancel"), this);
        pButtonsLayout->addWidget(pCancelButton);
        connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    
        pButtonsLayout->addStretch();

  }


  OptionDialog::~OptionDialog()
  {
  }


  int OptionDialog::getCollidableProperties() const
  {
    return m_nCollidableProperties;
  }


  int OptionDialog::getDamageableProperties() const
  {
    return m_nDamageableProperties;
  }


  int OptionDialog::getDynamicProperties() const
  {
    return m_nDynamicProperties;
  }


  int OptionDialog::getVariousProperties() const
  {
    return m_nVariousProperties;
  }


  void OptionDialog::collidableClicked(QAbstractButton* pButton)
  {
    ButtonToValueMap::const_iterator itFound = m_CollidableMap.find(pButton);
    assert(itFound != m_CollidableMap.end());
    int id = itFound->second;
    QCheckBox* pCheckBox = dynamic_cast<QCheckBox*>(pButton);
    assert(pCheckBox);
    if (pCheckBox->isChecked())
      m_nCollidableProperties |= id;
    else
      m_nCollidableProperties &= ~id;
  }


  void OptionDialog::damageableClicked(QAbstractButton* pButton)
  {
    ButtonToValueMap::const_iterator itFound = m_DamageableMap.find(pButton);
    assert(itFound != m_DamageableMap.end());
    int id = itFound->second;
    QCheckBox* pCheckBox = dynamic_cast<QCheckBox*>(pButton);
    assert(pCheckBox);
    if (pCheckBox->isChecked())
      m_nDamageableProperties |= id;
    else
      m_nDamageableProperties &= ~id;
  }


  void OptionDialog::dynamicClicked(QAbstractButton* pButton)
  {
    ButtonToValueMap::const_iterator itFound = m_DynamicMap.find(pButton);
    assert(itFound != m_DynamicMap.end());
    int id = itFound->second;
    QCheckBox* pCheckBox = dynamic_cast<QCheckBox*>(pButton);
    assert(pCheckBox);
    if (pCheckBox->isChecked())
      m_nDynamicProperties |= id;
    else
      m_nDynamicProperties &= ~id;
  }


  void OptionDialog::variousClicked(QAbstractButton* pButton)
  {
    ButtonToValueMap::const_iterator itFound = m_VariousMap.find(pButton);
    assert(itFound != m_VariousMap.end());
    int id = itFound->second;
    QCheckBox* pCheckBox = dynamic_cast<QCheckBox*>(pButton);
    assert(pCheckBox);
    if (pCheckBox->isChecked())
      m_nVariousProperties |= id;
    else
      m_nVariousProperties &= ~id;
  }


  QWidget* OptionDialog::createCheckBox(QButtonGroup* pButtonGroup, ButtonToValueMap& buttonValueMap, 
                                        int nWholeProperties, int nProperty, const QString& strDescription)
  {
    QCheckBox* pCheckBox = new QCheckBox(strDescription);
    pCheckBox->setChecked(nWholeProperties & nProperty);
    pButtonGroup->addButton(pCheckBox);
    buttonValueMap[pCheckBox] = nProperty;
    return pCheckBox;
  }

}

#include "OptionDialogMoc.cpp"
