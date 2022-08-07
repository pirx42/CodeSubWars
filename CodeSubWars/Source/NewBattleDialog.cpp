// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "NewBattleDialog.h"
#include "CSWWorld.h"


namespace CodeSubWars
{

  NewBattleDialog::NewBattleDialog(QString strPath, QWidget* pParent)
  : QDialog(pParent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    m_nBattleMode(CSWWorld::SINGLE)
  {
    setWindowTitle("NewBattle");
    setSizeGripEnabled(true);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
 
      QHBoxLayout* pListViewsLayout = new QHBoxLayout;
      pMainLayout->addLayout(pListViewsLayout);

        QVBoxLayout* pAvailableLayout = new QVBoxLayout;
        pListViewsLayout->addLayout(pAvailableLayout);

          QLabel* pAvailableDescription = new QLabel("Available submarines", this);
          pAvailableLayout->addWidget(pAvailableDescription);
             
          m_pAvailableView = new QListWidget(this);
          m_pAvailableView->setSelectionMode(QAbstractItemView::MultiSelection);
          m_pAvailableView->setSelectionBehavior(QAbstractItemView::SelectRows);
          pAvailableLayout->addWidget(m_pAvailableView);
          connect(m_pAvailableView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
                  this, SLOT(addSelected(QListWidgetItem*)));

        QVBoxLayout* pSelectButtonsLayout = new QVBoxLayout;
        pListViewsLayout->addLayout(pSelectButtonsLayout);

          pSelectButtonsLayout->addStretch();
      
          QPushButton* pAddButton = new QPushButton(tr("Add ->"), this);
          pSelectButtonsLayout->addWidget(pAddButton);
          connect(pAddButton, SIGNAL(clicked()), this, SLOT(addSelected()));

          QPushButton* pRemoveButton = new QPushButton(tr("<- Remove"), this);
          pSelectButtonsLayout->addWidget(pRemoveButton);
          connect(pRemoveButton, SIGNAL(clicked()), this, SLOT(removeSelected()));
      
          pSelectButtonsLayout->addStretch();

        QVBoxLayout* pSelectedLayout = new QVBoxLayout;
        pListViewsLayout->addLayout(pSelectedLayout);

          QLabel* pSelectedDescription = new QLabel("Selected submarines", this);
          pSelectedLayout->addWidget(pSelectedDescription);

          m_pSelectedView = new QListWidget(this);
          m_pSelectedView->setSelectionMode(QAbstractItemView::MultiSelection);
          m_pSelectedView->setSelectionBehavior(QAbstractItemView::SelectRows);
          pSelectedLayout->addWidget(m_pSelectedView);
          connect(m_pSelectedView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
                  this, SLOT(removeSelected(QListWidgetItem*)));

      QGroupBox* pGroupBox = new QGroupBox("Battle options", this);
      pMainLayout->addWidget(pGroupBox);
    
      QGridLayout* pOptionGroupLayout = new QGridLayout(pGroupBox);
      pGroupBox->setLayout(pOptionGroupLayout);
    
      QButtonGroup* pOptionGroup = new QButtonGroup;
      connect(pOptionGroup, SIGNAL(buttonClicked(int)), this, SLOT(selectBattleMode(int)));

        QRadioButton* pSelectSingle = new QRadioButton("Single", pGroupBox);
        pSelectSingle->setChecked(true);
        pOptionGroup->addButton(pSelectSingle, CSWWorld::SINGLE);
        pOptionGroupLayout->addWidget(pSelectSingle, 0, 0);

        QRadioButton* pSelectTeam = new QRadioButton("Team", pGroupBox);
        pOptionGroup->addButton(pSelectTeam, CSWWorld::TEAM);
        pOptionGroupLayout->addWidget(pSelectTeam, 1, 0);
      
        pOptionGroupLayout->addWidget(new QLabel("TeamSize:", pGroupBox), 1, 2);

        m_pTeamSizeBox = new QComboBox(pGroupBox);
        m_pTeamSizeBox->addItem("3");
        m_pTeamSizeBox->addItem("5");
        m_pTeamSizeBox->addItem("10");
        m_pTeamSizeBox->setEnabled(false);
        m_pTeamSizeBox->setFixedWidth(60);
        pOptionGroupLayout->addWidget(m_pTeamSizeBox, 1, 3);
        connect(pSelectTeam, SIGNAL(toggled(bool)), m_pTeamSizeBox, SLOT(setEnabled(bool)));

      QHBoxLayout* pButtonsLayout = new QHBoxLayout;
      pMainLayout->addLayout(pButtonsLayout);

        pButtonsLayout->addStretch();
    
        m_pOkButton = new QPushButton(tr("Ok"), this);
        m_pOkButton->setFocus();
        m_pOkButton->setEnabled(false);
        pButtonsLayout->addWidget(m_pOkButton);
        connect(m_pOkButton, SIGNAL(clicked()), this, SLOT(accept()));

        QPushButton* pCancelButton = new QPushButton(tr("Cancel"), this);
        pButtonsLayout->addWidget(pCancelButton);
        connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    
        pButtonsLayout->addStretch();


    m_AvailableSubmarineFiles = CSWUtilities::determineAvailableSubmarines(strPath.toStdString());
    rebuildAvailableView();
  }


  NewBattleDialog::~NewBattleDialog()
  {
  }


  CSWUtilities::SubmarineFileContainer NewBattleDialog::getSubmarines() const
  {
    return m_SubmarineFiles;
  }


  int NewBattleDialog::getBattleMode() const
  {
    return m_nBattleMode;
  }


  int NewBattleDialog::getTeamSize() const
  {
    return m_pTeamSizeBox->currentText().toInt();
  }


  void NewBattleDialog::addSelected()
  {
    QList<QListWidgetItem*> items = m_pAvailableView->selectedItems();
    QList<QListWidgetItem*>::const_iterator it = items.begin();
    for (; it != items.end(); ++it) 
    {
      std::map<std::string, std::string>::const_iterator itFound = m_AvailableSubmarineFiles.find((*it)->text().toStdString());
      assert(itFound != m_AvailableSubmarineFiles.end());
      m_SubmarineFiles.insert(*itFound);
    }
  
    rebuildSelectedView(); 
    m_pAvailableView->clearSelection();
  }


  void NewBattleDialog::addSelected(QListWidgetItem* pItem)
  {
    if (!pItem) return;
    std::map<std::string, std::string>::const_iterator itFound = m_AvailableSubmarineFiles.find(pItem->text().toStdString());
    assert(itFound != m_AvailableSubmarineFiles.end());
    m_SubmarineFiles.insert(*itFound);

    rebuildSelectedView(); 
    m_pAvailableView->clearSelection();
  }


  void NewBattleDialog::removeSelected()
  {
    QList<QListWidgetItem*> items = m_pSelectedView->selectedItems();
    QList<QListWidgetItem*>::const_iterator it = items.begin();
    for (; it != items.end(); ++it) 
    {
      m_SubmarineFiles.erase((*it)->text().toStdString());
    }

    rebuildSelectedView();  
    m_pSelectedView->clearSelection();
  }


  void NewBattleDialog::removeSelected(QListWidgetItem* pItem)
  {
    if (!pItem)
      return;
    m_SubmarineFiles.erase(pItem->text().toStdString());

    rebuildSelectedView();
    m_pSelectedView->clearSelection();
  }


  void NewBattleDialog::selectBattleMode(int id)
  {
    m_nBattleMode = id;
  }


  void NewBattleDialog::rebuildAvailableView()
  {
    m_pAvailableView->clear();
    std::map<std::string, std::string>::const_iterator it = m_AvailableSubmarineFiles.begin();
    for (; it != m_AvailableSubmarineFiles.end(); ++it)
    {
      m_pAvailableView->addItem(it->first.c_str());
    }
  }


  void NewBattleDialog::rebuildSelectedView()
  {
    m_pSelectedView->clear();
    std::map<std::string, std::string>::const_iterator it = m_SubmarineFiles.begin();
    for (; it != m_SubmarineFiles.end(); ++it)
    {
      m_pSelectedView->addItem(it->first.c_str());
    }
    m_pOkButton->setEnabled(!m_SubmarineFiles.empty());
  }

}

#include "NewBattleDialogMoc.cpp"