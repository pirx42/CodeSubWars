// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class OptionDialog : public QDialog
  {
      Q_OBJECT
    public:
      OptionDialog(QWidget* pParent);
      virtual ~OptionDialog();
    
      int getCollidableProperties() const;
      int getDamageableProperties() const;
      int getDynamicProperties() const;
      int getVariousProperties() const;

    protected slots:
      void collidableClicked(QAbstractButton* pButton);
      void damageableClicked(QAbstractButton* pButton);
      void dynamicClicked(QAbstractButton* pButton);
      void variousClicked(QAbstractButton* pButton);
    
    protected:
      typedef std::map<QWidget*, int> ButtonToValueMap;

      QWidget* createCheckBox(QButtonGroup* pButtonGroup, ButtonToValueMap& buttonValueMap, 
                              int nWholeProperties, int nProperty, const QString& strDescription);
  
      int m_nCollidableProperties;
      int m_nDamageableProperties;
      int m_nDynamicProperties;
      int m_nVariousProperties;

      QButtonGroup* m_pCollidableButtonGroup;
      QButtonGroup* m_pDamageableButtonGroup;
      QButtonGroup* m_pDynamicButtonGroup;
      QButtonGroup* m_pVariousButtonGroup;
    
      ButtonToValueMap m_CollidableMap;
      ButtonToValueMap m_DamageableMap;
      ButtonToValueMap m_DynamicMap;
      ButtonToValueMap m_VariousMap;
    
  };

}