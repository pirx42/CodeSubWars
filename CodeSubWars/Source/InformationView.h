// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

  #pragma once

//#include "WorldTreeModel.h"

#include "CSWObject.h"

namespace CodeSubWars
{

  //class CSWObject;
  class CSWSettings;


  class InformationView : public QWidget
  {
      Q_OBJECT

    public:
      InformationView(QWidget* pParent = NULL);

      virtual ~InformationView();

      virtual QSize sizeHint() const;

    signals:
      void selectionChanged();

    public slots:
      void updateContent();
      void rebuild();
      void enforceCompleteRebuild(bool bIgnoreDifferentChildNumber);

    protected slots:
      void doSelectionChanged();

    protected:
      struct InformationField
      {
        QTabWidget* pTabWidget;
      
        //CSWObject
        QLabel* pObjectNameLabel;
        QLabel* pObjectPositionLabel;
        QLabel* pObjectOrientationLabel;

        //CSWICommandable
        QLabel* pCurrentCommandLabel;
        QTextEdit* pCurrentCommandTextEdit;
        QLabel* pNumWaitingCommandsLabel;
        QTextEdit* pWaitingCommandsTextEdit;
      
        //CSWIDamageable
        QLabel* pCurrentHealthValueLabel;
        QLabel* pCurrentHealthStateLabel;
      
        //CSWIDynamic
        QLabel* pMassLabel;
        QLabel* pLocalVelocityLabel;
        QLabel* pWorldVelocityLabel;
        QLabel* pLocalAccelerationLabel;
        QLabel* pWorldAccelerationLabel;
        QLabel* pLocalAngularVelocityLabel;
        QLabel* pWorldAngularVelocityLabel;
        QLabel* pLocalAngularAccelerationLabel;
        QLabel* pWorldAngularAccelerationLabel;

        //CSWIEventDealable
        QLabel* pInformationEventLabel;
        QTextEdit* pMessageEventTextEdit;

        //CSWEquipment
        QWidget* pEquipmentInformationWidget;
      
        //CSWIRechargeable
        QLabel* pRechargeInformationLabel;
      
        //CSWIResourceProvider
        QLabel* pConnectedObjectsLabel;
      
        //CSWISolid
        QLabel* pSizeLabel;

        //CSWISoundReceiver
        QLabel* pSoundsLabel;
      };
    
      void ensureDetailContentIsAvailable(std::shared_ptr<CSWObject> pObject);
      void setupDetailContent(std::shared_ptr<CSWObject> pObject, InformationField& infoField, QWidget* pParent);
      QWidget* createTab(QList<QWidget*>& widgets, bool bScrollable);
      void updateDetailContent();

      void buildTree(QTreeWidget* pParent, std::shared_ptr<CSWObject> pRootObject);
      void buildChilds(QTreeWidget* pParent, std::shared_ptr<CSWObject> pObject, const std::map<QString, std::pair<bool, bool> >& properties);
      void buildChilds(QTreeWidgetItem* pItem, std::shared_ptr<CSWObject> pObject, const std::map<QString, std::pair<bool, bool> >& properties);

      QTreeWidget* m_pObjectTree;
      QStackedWidget* m_pDetailStackedWidget;

      std::map<std::weak_ptr<CSWObject>, InformationField, ObjectLess> m_ObjectToDetailWidgetMap; 

      int m_nOldChildNumber;
      InformationField m_CurrentField;
  };

}