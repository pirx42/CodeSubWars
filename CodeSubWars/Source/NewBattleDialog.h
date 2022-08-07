// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWUtilities.h"

namespace CodeSubWars
{

  class NewBattleDialog : public QDialog
  {
      Q_OBJECT

    public:
      NewBattleDialog(QString strPath, QWidget* pParent = NULL);
      virtual ~NewBattleDialog();

      CSWUtilities::SubmarineFileContainer getSubmarines() const;
      int getBattleMode() const;
      int getTeamSize() const;

    protected slots:
      void addSelected();
      void addSelected(QListWidgetItem*);
      void removeSelected();
      void removeSelected(QListWidgetItem*);
      void selectBattleMode(int);

    protected:  
      void rebuildAvailableView();
      void rebuildSelectedView();
      
      CSWUtilities::SubmarineFileContainer m_AvailableSubmarineFiles;
      CSWUtilities::SubmarineFileContainer m_SubmarineFiles;
      int m_nBattleMode;
    
      QListWidget* m_pAvailableView;
      QListWidget* m_pSelectedView;
      QPushButton* m_pOkButton;
      QComboBox* m_pTeamSizeBox;
  };

}