// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWUtilities.h"

#include "CSWSettings.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "CSWCamera.h"
#include "InformationView.h"
#include "PythonContext.h"

#include "CSWGyroCompass.h"

#include "CSWICollideable.h"
#include "CSWICommandable.h"
#include "CSWIDamageable.h"
#include "CSWIDynamic.h"
#include "CSWIEventDealable.h"
#include "CSWIImpulsEmitter.h"
#include "CSWIForceEmitter.h"
#include "CSWIUpdateable.h"
#include "CSWIPythonable.h"
#include "CSWIRechargeable.h"
#include "CSWIResourceProvider.h"
#include "CSWISolid.h"
#include "CSWISoundEmitter.h"
#include "CSWISoundReceiver.h"

#include "CSWEquipment.h"
#include "CSWWeapon.h"
#include "CSWWeaponSupply.h"

#include "CSWIEquipmentVisualizer.h"


namespace CodeSubWars
{

  InformationView::InformationView(QWidget* parent)
  : QWidget(parent),
    m_nOldChildNumber(0)
  {
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setSpacing(2);
    pLayout->setMargin(0);
  
    QSplitter* pHorizontalSplitter = new QSplitter(Qt::Horizontal, this);
    pHorizontalSplitter->setOpaqueResize(true);
    pLayout->addWidget(pHorizontalSplitter);
  
    m_pObjectTree = new QTreeWidget(pHorizontalSplitter);
    m_pObjectTree->setHeaderLabel("Objects (0)");
    m_pObjectTree->setSelectionMode(QAbstractItemView::SingleSelection);
  
    m_pDetailStackedWidget = new QStackedWidget(pHorizontalSplitter);
    m_pDetailStackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QList<int> sizes;
    sizes.push_back(width()*0.3);
    sizes.push_back(width()*0.7);
    pHorizontalSplitter->setSizes(sizes);

    connect(m_pObjectTree, SIGNAL(itemSelectionChanged()), this, SLOT(doSelectionChanged()));
  }


  InformationView::~InformationView()
  {
  }


  QSize InformationView::sizeHint() const
  {
    return QSize(200, 200);
  }


  void InformationView::updateContent()
  {
    if (!isVisible())
      return;

    if (CSWWorld::getInstance()->hasChanged())
    {
      enforceCompleteRebuild(false);
    }

    updateDetailContent();

    update();
  }


  void InformationView::rebuild()
  {
    CSWObject::PtrType pObject = CSWWorld::getInstance()->getSettings()->getSelectedObject();
    if (!pObject)
      return;

    QList<QTreeWidgetItem*> selectedItems = m_pObjectTree->selectedItems(); 
    QList<QTreeWidgetItem*>::iterator it = selectedItems.begin();
    for (; it != selectedItems.end(); ++it) (*it)->setSelected(false);
  
    QString str = pObject->getName().c_str();
    QList<QTreeWidgetItem*> items = m_pObjectTree->findItems(str, Qt::MatchExactly, 0);
    if (!items.empty())
    {
      assert(items.size() == 1);
      items.front()->setSelected(true);
      m_pObjectTree->scrollToItem(items.front());
    }

    ensureDetailContentIsAvailable(pObject);
    updateDetailContent();
  }


  void InformationView::enforceCompleteRebuild(bool bIgnoreDifferentChildNumber)
  {
    int nCurrentChildNumber = CSWWorld::getInstance()->getObjectTree()->getTotalChildNumber();
    nCurrentChildNumber -= CSWWorld::getInstance()->getObjectTree()->getChildNumber<CSWCamera>();
    if (!(CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_WEAPONS_IN_OBJECT_TREE))
    {
      nCurrentChildNumber -= CSWWorld::getInstance()->getObjectTree()->getChildNumber<CSWWeapon>();
    }

    if (bIgnoreDifferentChildNumber || m_nOldChildNumber != nCurrentChildNumber)
    {
      m_pObjectTree->setHeaderLabel("Objects (" + QString::number(nCurrentChildNumber) + ")");
      buildTree(m_pObjectTree, CSWWorld::getInstance()->getObjectTree());
      m_nOldChildNumber = nCurrentChildNumber;
    }
  }


  void InformationView::doSelectionChanged()
  {
    QList<QTreeWidgetItem*> selectedItems = m_pObjectTree->selectedItems(); 
    if (selectedItems.empty())
      return;

    QString strObjectName = selectedItems.front()->text(0);
  
    CSWObject::PtrType pObject = std::dynamic_pointer_cast<CSWObject>(CSWObject::findElement(CSWWorld::getInstance()->getObjectTree(), strObjectName.toStdString()));
    if (!pObject || pObject == CSWWorld::getInstance()->getSettings()->getSelectedObject())
      return;

    CSWWorld::getInstance()->getSettings()->setSelectedObject(pObject);

    ensureDetailContentIsAvailable(pObject);
    updateDetailContent();

    emit selectionChanged();
  }


  void InformationView::updateDetailContent()
  {
    CSWObject::PtrType pObject = CSWWorld::getInstance()->getSettings()->getSelectedObject();
    if (!pObject)
      return;

    //PyThreadState* state = NULL;
    PythonContext::PtrType pyContext = PythonContext::createEmpty();
    if (CSWIPythonable::PtrType pPythonObject = std::dynamic_pointer_cast<CSWIPythonable>(pObject))
    {
      //state = pPythonObject->getThreadState();
      //PyEval_AcquireThread(state);
      pyContext = pPythonObject->getPythonContext();
    }
    auto lck = pyContext->makeCurrent();

    m_CurrentField.pObjectNameLabel->setText(QString("Name: ") + pObject->getName().c_str());
    m_CurrentField.pObjectPositionLabel->setText(QString("Position [m]: ") + 
                                                 QString::number(pObject->getWorldTransform().getTranslation().x, 'f', 2) + ", " +
                                                 QString::number(pObject->getWorldTransform().getTranslation().y, 'f', 2) + ", " +
                                                 QString::number(pObject->getWorldTransform().getTranslation().z, 'f', 2));

    Vector3D angles = CSWUtilities::determineAngles(pObject->getWorldTransform());
    m_CurrentField.pObjectOrientationLabel->setText(QString("Inclination (x) [degree]: ") + QString::number(angles.x, 'f', 2) + "\n" +
                                                    QString("Direction (y) [degree]: ") + QString::number(angles.y, 'f', 2) + "\n" +
                                                    QString("AxialInclination (z) [degree]: ") + QString::number(angles.z, 'f', 2));
  
    if (CSWICollideable::PtrType pObj = std::dynamic_pointer_cast<CSWICollideable>(pObject))
    {
    }    
    if (CSWICommandable::PtrType pObj = std::dynamic_pointer_cast<CSWICommandable>(pObject))
    {
      if (m_CurrentField.pCurrentCommandLabel->parentWidget()->isVisible())
      {
        if (pObj->getCommandProcessor()->getCurrentCommandProgress() >= 0)
        {
          m_CurrentField.pCurrentCommandLabel->setText(QString("Stackdepth: ") + QString::number(pObj->getCommandProcessor()->getCurrentStackDepth()) +
                                                      QString("\nHistory (executed): ") + QString::number(pObj->getCommandProcessor()->getNumExecutedCommands()) +
                                                      QString("\nCurrent (") + 
                                                      QString::number(pObj->getCommandProcessor()->getCurrentCommandProgress()*100, 'f', 1) + "%)");
          auto verticalValue = m_CurrentField.pCurrentCommandTextEdit->verticalScrollBar()->value();
          auto horizontalValue = m_CurrentField.pCurrentCommandTextEdit->horizontalScrollBar()->value();
          m_CurrentField.pCurrentCommandTextEdit->setText(QString(pObj->getCommandProcessor()->getCurrentCommandName().c_str()) + 
                                                          "\nDetailed Description: \n" +
                                                          pObj->getCommandProcessor()->getCurrentCommandDetails().c_str());
          m_CurrentField.pCurrentCommandTextEdit->verticalScrollBar()->setValue(verticalValue);
          m_CurrentField.pCurrentCommandTextEdit->horizontalScrollBar()->setValue(horizontalValue);
        }
        else
        {
          m_CurrentField.pCurrentCommandLabel->setText(QString("Stackdepth: ") + QString::number(pObj->getCommandProcessor()->getCurrentStackDepth()) +
                                                      QString("\nHistory (executed): ") + QString::number(pObj->getCommandProcessor()->getNumExecutedCommands()) +
                                                      "\nCurrent (n.a.)");
          m_CurrentField.pCurrentCommandTextEdit->setText("");
        }

        m_CurrentField.pNumWaitingCommandsLabel->setText(QString("Waiting (") + QString::number(pObj->getCommandProcessor()->getNumWaitingCommands()) + "): ");
      
        std::stringstream ss;
        int nCnt = 1;
        ARSTD::CommandProcessor::CommandConstRange range = pObj->getCommandProcessor()->getWaitingCommands();
        for (ARSTD::CommandProcessor::CommandConstIterator itCommand = range.first; itCommand != range.second; ++itCommand, ++nCnt)
        {
          ss << nCnt << "." << (*itCommand)->getName() << "\n";
        }
        m_CurrentField.pWaitingCommandsTextEdit->setText(QString(ss.str().c_str()));
      }
    }    
    if (CSWIDamageable::PtrType pObj = std::dynamic_pointer_cast<CSWIDamageable>(pObject))
    {
      if (m_CurrentField.pCurrentHealthStateLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pCurrentHealthValueLabel->setText(QString("Health [%]: ") + QString::number(pObj->getHealth()*100, 'f', 1)); 
        QString strState = "Unknown";
        if (pObj->isAlive())
          strState = "Alive";
    //    else if (pObj->isDisabled()) strState = "Disabled";
        else if (pObj->isZombie())
          strState = "Zombie";
        else if (pObj->isDead())
          strState = "Dead";
        m_CurrentField.pCurrentHealthStateLabel->setText(QString("Health state: ") + strState); 
      }
    }    
    if (CSWIDynamic::PtrType pObj = std::dynamic_pointer_cast<CSWIDynamic>(pObject))
    {
      if (m_CurrentField.pMassLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pMassLabel->setText(QString("Total Mass [kt]: ") + QString::number(pObj->getTotalMass()/1e+6, 'g', 3));

        Matrix33D matObjectTWorld = pObject->getInvertWorldTransform().getRotationAsMatrix33();
        Vector3D vecWorldTVelocity = pObj->getVelocity();
        Vector3D vecObjectTVelocity = matObjectTWorld*vecWorldTVelocity;
        m_CurrentField.pLocalVelocityLabel->setText(QString("Velocity (local) [m/s]: ") + 
                                                    QString::number(vecObjectTVelocity.x, 'f', 2) + ", " +
                                                    QString::number(vecObjectTVelocity.y, 'f', 2) + ", " +
                                                    QString::number(vecObjectTVelocity.z, 'f', 2) + "   (" +
                                                    QString::number(vecObjectTVelocity.getLength(), 'f', 2) + ")");

        m_CurrentField.pWorldVelocityLabel->setText(QString("Velocity (world) [m/s]: ") + 
                                                    QString::number(vecWorldTVelocity.x, 'f', 2) + ", " +
                                                    QString::number(vecWorldTVelocity.y, 'f', 2) + ", " +
                                                    QString::number(vecWorldTVelocity.z, 'f', 2) + "   (" +
                                                    QString::number(vecWorldTVelocity.getLength(), 'f', 2) + ")");

        Vector3D vecWorldTAcceleration = pObj->getAcceleration();
        Vector3D vecObjectTAcceleration = matObjectTWorld*vecWorldTAcceleration;
        m_CurrentField.pLocalAccelerationLabel->setText(QString("Acceleration (local) [m/s^2]: ") + 
                                                        QString::number(vecObjectTAcceleration.x, 'f', 2) + ", " +
                                                        QString::number(vecObjectTAcceleration.y, 'f', 2) + ", " +
                                                        QString::number(vecObjectTAcceleration.z, 'f', 2) + "   (" +
                                                        QString::number(vecObjectTAcceleration.getLength(), 'f', 2) + ")");

        m_CurrentField.pWorldAccelerationLabel->setText(QString("Acceleration (world) [m/s^2]: ") + 
                                                        QString::number(vecWorldTAcceleration.x, 'f', 2) + ", " +
                                                        QString::number(vecWorldTAcceleration.y, 'f', 2) + ", " +
                                                        QString::number(vecWorldTAcceleration.z, 'f', 2) + "   (" +
                                                        QString::number(vecWorldTAcceleration.getLength(), 'f', 2) + ")");

        Vector3D vecWorldTAngularVelocity = pObj->getAngularVelocity();
        Vector3D vecObjectTAngularVelocity = matObjectTWorld*vecWorldTAngularVelocity;
        m_CurrentField.pLocalAngularVelocityLabel->setText(QString("AngularVelocity (local) [degree/s]: ") + 
                                                          QString::number(180/ARSTD::PI*vecObjectTAngularVelocity.x, 'f', 2) + ", " +
                                                          QString::number(180/ARSTD::PI*vecObjectTAngularVelocity.y, 'f', 2) + ", " +
                                                          QString::number(180/ARSTD::PI*vecObjectTAngularVelocity.z, 'f', 2) + "   (" +
                                                          QString::number(180/ARSTD::PI*vecObjectTAngularVelocity.getLength(), 'f', 2) +")");

        m_CurrentField.pWorldAngularVelocityLabel->setText(QString("AngularVelocity (world) [degree/s]: ") + 
                                                          QString::number(180/ARSTD::PI*vecWorldTAngularVelocity.x, 'f', 2) + ", " +
                                                          QString::number(180/ARSTD::PI*vecWorldTAngularVelocity.y, 'f', 2) + ", " +
                                                          QString::number(180/ARSTD::PI*vecWorldTAngularVelocity.z, 'f', 2) + "   (" +
                                                          QString::number(180/ARSTD::PI*vecWorldTAngularVelocity.getLength(), 'f', 2) +")");

        Vector3D vecWorldTAngularAcceleration = pObj->getAngularAcceleration();
        Vector3D vecObjectTAngularAcceleration = matObjectTWorld*vecWorldTAngularAcceleration;
        m_CurrentField.pLocalAngularAccelerationLabel->setText(QString("AngularAcceleration (local) [degree/s^2]: ") + 
                                                              QString::number(180/ARSTD::PI*vecObjectTAngularAcceleration.x, 'f', 2) + ", " +
                                                              QString::number(180/ARSTD::PI*vecObjectTAngularAcceleration.y, 'f', 2) + ", " +
                                                              QString::number(180/ARSTD::PI*vecObjectTAngularAcceleration.z, 'f', 2) + "   (" +
                                                              QString::number(180/ARSTD::PI*vecObjectTAngularAcceleration.getLength(), 'f', 2) +")");

        m_CurrentField.pWorldAngularAccelerationLabel->setText(QString("AngularAcceleration (world) [degree/s^2]: ") + 
                                                              QString::number(180/ARSTD::PI*vecWorldTAngularAcceleration.x, 'f', 2) + ", " +
                                                              QString::number(180/ARSTD::PI*vecWorldTAngularAcceleration.y, 'f', 2) + ", " +
                                                              QString::number(180/ARSTD::PI*vecWorldTAngularAcceleration.z, 'f', 2) + "   (" +
                                                              QString::number(180/ARSTD::PI*vecWorldTAngularAcceleration.getLength(), 'f', 2) +")");
      }
    }    
    if (CSWIEventDealable::PtrType pObj = std::dynamic_pointer_cast<CSWIEventDealable>(pObject))
    {
      if (m_CurrentField.pInformationEventLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pInformationEventLabel->setText(pObj->getCurrentInformation().c_str());
        m_CurrentField.pMessageEventTextEdit->insertPlainText(pObj->getCurrentMessage().c_str());
      }
    }    
    if (CSWEquipment::PtrType pObj = std::dynamic_pointer_cast<CSWEquipment>(pObject))
    {
      if (m_CurrentField.pEquipmentInformationWidget->parentWidget()->isVisible()) 
      {
        if (CSWIEquipmentVisualizer* pV = dynamic_cast<CSWIEquipmentVisualizer*>(m_CurrentField.pEquipmentInformationWidget))
        {
          pV->updateContent();
        }
  //      m_CurrentField.pEquipmentInformationWidget->repaint();
      }
    }    
    if (CSWIForceEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWIForceEmitter>(pObject))
    {
    }    
    if (CSWIImpulsEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWIImpulsEmitter>(pObject))
    {
    }    
    if (CSWIPythonable::PtrType pObj = std::dynamic_pointer_cast<CSWIPythonable>(pObject))
    {
    }  
    if (CSWIResourceProvider::PtrType pObj = std::dynamic_pointer_cast<CSWIResourceProvider>(pObject))
    {
      if (m_CurrentField.pConnectedObjectsLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pConnectedObjectsLabel->setText(pObj->getCurrentInfo().c_str());
      }
    }    
    if (CSWIRechargeable::PtrType pObj = std::dynamic_pointer_cast<CSWIRechargeable>(pObject))
    {
      if (m_CurrentField.pRechargeInformationLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pRechargeInformationLabel->setText(QString("Accepted resources: ") + CSWWeaponSupply::translate(pObj->getAcceptedResourceIDs()).c_str() + "\n" +
                                                          QString("Currently recharging resources: ") + CSWWeaponSupply::translate(pObj->getRechargingIDs()).c_str() + "\n" +
                                                          QString("Fill level [%]: ") + QString::number(pObj->getFillLevel()*100, 'f', 1));
      }
    }    
    if (CSWISolid::PtrType pObj = std::dynamic_pointer_cast<CSWISolid>(pObject))
    {
      if (m_CurrentField.pSizeLabel->parentWidget()->isVisible())
      {
        m_CurrentField.pSizeLabel->setText("Size (HxWxD) [m]: " + 
                                          QString::number(pObj->getSurfaceBoundingBox().getWidth(), 'f', 2) + ", " +
                                          QString::number(pObj->getSurfaceBoundingBox().getHeight(), 'f', 2) + ", " +
                                          QString::number(pObj->getSurfaceBoundingBox().getDepth(), 'f', 2));
      }
    }    
    if (CSWISoundEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWISoundEmitter>(pObject))
    {
    }    
    if (CSWISoundReceiver::PtrType pObj = std::dynamic_pointer_cast<CSWISoundReceiver>(pObject))
    {
      if (m_CurrentField.pSoundsLabel->parentWidget()->isVisible())
      {
        std::stringstream ss;
        ss.precision(2);
        ss.setf(std::ios::fixed);
        ss << "Angle [degree]: " << pObj->getAngleOfBeam()
          << "\nDirection: " << pObj->getDirection()
          << "\nLevel [dB]: " << pObj->getLevel();
        m_CurrentField.pSoundsLabel->setText(ss.str().c_str());
      }  
    }    
    if (CSWIUpdateable::PtrType pObj = std::dynamic_pointer_cast<CSWIUpdateable>(pObject))
    {
    }      

    //if (state) PyEval_ReleaseThread(state);    
  }


  void InformationView::ensureDetailContentIsAvailable(CSWObject::PtrType pObject)
  {
    std::map<std::weak_ptr<CSWObject>, InformationField>::const_iterator itElement = m_ObjectToDetailWidgetMap.find(pObject);

    if (itElement != m_ObjectToDetailWidgetMap.end())
    {
      m_CurrentField = itElement->second;
    }
    else
    {
  //    m_pDetailStackedWidget->hide();
      InformationField infoField;
      setupDetailContent(pObject, infoField, m_pDetailStackedWidget);
      m_pDetailStackedWidget->addWidget(infoField.pTabWidget);
      m_ObjectToDetailWidgetMap.insert(std::make_pair(pObject, infoField));
      m_CurrentField = infoField;
    }  
    m_pDetailStackedWidget->setCurrentWidget(m_CurrentField.pTabWidget);
  //  m_pDetailStackedWidget->show();
  }


  void InformationView::setupDetailContent(CSWObject::PtrType pObject, InformationField& infoField, QWidget* pParent)
  {
    if (!pObject)
      throw std::runtime_error("object not defined");

    QFont f;
    f.setFamily("Courier");
    f.setPointSize(8);
  
    infoField.pTabWidget = new QTabWidget;

    QList<QWidget*> widgets;

    infoField.pObjectNameLabel = new QLabel;
    infoField.pObjectNameLabel->setTextFormat(Qt::PlainText);
    infoField.pObjectPositionLabel = new QLabel;
    infoField.pObjectPositionLabel->setTextFormat(Qt::PlainText);
    infoField.pObjectOrientationLabel = new QLabel;
    infoField.pObjectOrientationLabel->setTextFormat(Qt::PlainText);
    widgets.push_back(infoField.pObjectNameLabel);
    widgets.push_back(infoField.pObjectPositionLabel);
    widgets.push_back(infoField.pObjectOrientationLabel);
  
    infoField.pTabWidget->addTab(createTab(widgets, true), "CSWObject");

    if (CSWICollideable::PtrType pObj = std::dynamic_pointer_cast<CSWICollideable>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWICollidable");
    }    
    if (CSWICommandable::PtrType pObj = std::dynamic_pointer_cast<CSWICommandable>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pCurrentCommandLabel = new QLabel;
      infoField.pCurrentCommandLabel->setTextFormat(Qt::PlainText);
      infoField.pCurrentCommandTextEdit = new QTextEdit;
      infoField.pCurrentCommandTextEdit->setFont(f);
      infoField.pCurrentCommandTextEdit->setAutoFormatting(QTextEdit::AutoNone);
      infoField.pCurrentCommandTextEdit->setLineWrapMode(QTextEdit::NoWrap);
      infoField.pCurrentCommandTextEdit->setWordWrapMode(QTextOption::NoWrap);
      infoField.pCurrentCommandTextEdit->setReadOnly(true);
      infoField.pCurrentCommandTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      widgets.push_back(infoField.pCurrentCommandLabel);
      widgets.push_back(infoField.pCurrentCommandTextEdit);
    
      infoField.pNumWaitingCommandsLabel = new QLabel;
      infoField.pNumWaitingCommandsLabel->setTextFormat(Qt::PlainText);
      infoField.pWaitingCommandsTextEdit = new QTextEdit;
      infoField.pWaitingCommandsTextEdit->setFont(f);
      infoField.pWaitingCommandsTextEdit->setAutoFormatting(QTextEdit::AutoNone);
      infoField.pWaitingCommandsTextEdit->setLineWrapMode(QTextEdit::NoWrap);
      infoField.pWaitingCommandsTextEdit->setWordWrapMode(QTextOption::NoWrap);
      infoField.pWaitingCommandsTextEdit->setReadOnly(true);
      infoField.pWaitingCommandsTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    
      widgets.push_back(infoField.pNumWaitingCommandsLabel);
      widgets.push_back(infoField.pWaitingCommandsTextEdit);
    
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWICommandable");
    }    
    if (CSWIDamageable::PtrType pObj = std::dynamic_pointer_cast<CSWIDamageable>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pCurrentHealthValueLabel = new QLabel;
      infoField.pCurrentHealthValueLabel->setTextFormat(Qt::PlainText);
      infoField.pCurrentHealthStateLabel = new QLabel;
      infoField.pCurrentHealthStateLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pCurrentHealthValueLabel);
      widgets.push_back(infoField.pCurrentHealthStateLabel);

      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIDamagable");
    }    
    if (CSWIDynamic::PtrType pObj = std::dynamic_pointer_cast<CSWIDynamic>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pMassLabel = new QLabel;
      infoField.pMassLabel->setTextFormat(Qt::PlainText);
      infoField.pLocalVelocityLabel = new QLabel;
      infoField.pLocalVelocityLabel->setTextFormat(Qt::PlainText);
      infoField.pWorldVelocityLabel = new QLabel;
      infoField.pWorldVelocityLabel->setTextFormat(Qt::PlainText);
      infoField.pLocalAngularVelocityLabel = new QLabel;
      infoField.pLocalAngularVelocityLabel->setTextFormat(Qt::PlainText);
      infoField.pWorldAngularVelocityLabel = new QLabel;
      infoField.pWorldAngularVelocityLabel->setTextFormat(Qt::PlainText);
      infoField.pLocalAccelerationLabel = new QLabel;
      infoField.pLocalAccelerationLabel->setTextFormat(Qt::PlainText);
      infoField.pWorldAccelerationLabel = new QLabel;
      infoField.pWorldAccelerationLabel->setTextFormat(Qt::PlainText);
      infoField.pLocalAngularAccelerationLabel = new QLabel;
      infoField.pLocalAngularAccelerationLabel->setTextFormat(Qt::PlainText);
      infoField.pWorldAngularAccelerationLabel = new QLabel;
      infoField.pWorldAngularAccelerationLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pMassLabel);
      widgets.push_back(infoField.pLocalVelocityLabel);
      widgets.push_back(infoField.pWorldVelocityLabel);
      widgets.push_back(infoField.pLocalAngularVelocityLabel);
      widgets.push_back(infoField.pWorldAngularVelocityLabel);
      widgets.push_back(infoField.pLocalAccelerationLabel);
      widgets.push_back(infoField.pWorldAccelerationLabel);
      widgets.push_back(infoField.pLocalAngularAccelerationLabel);
      widgets.push_back(infoField.pWorldAngularAccelerationLabel);

      infoField.pTabWidget->addTab(createTab(widgets, true), "CSWIDynamic");
    }    
    if (CSWIEventDealable::PtrType pObj = std::dynamic_pointer_cast<CSWIEventDealable>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pInformationEventLabel = new QLabel;
      infoField.pInformationEventLabel->setTextFormat(Qt::PlainText);
      infoField.pMessageEventTextEdit = new QTextEdit;
      infoField.pMessageEventTextEdit->setFont(f);
      infoField.pMessageEventTextEdit->setAutoFormatting(QTextEdit::AutoNone);
      infoField.pMessageEventTextEdit->setLineWrapMode(QTextEdit::NoWrap);
      infoField.pMessageEventTextEdit->setWordWrapMode(QTextOption::NoWrap);
      infoField.pMessageEventTextEdit->setReadOnly(true);
      infoField.pMessageEventTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      widgets.push_back(infoField.pInformationEventLabel);
      widgets.push_back(infoField.pMessageEventTextEdit);

      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIEventDealable");
    }    
    if (CSWEquipment::PtrType pObj = std::dynamic_pointer_cast<CSWEquipment>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pEquipmentInformationWidget = pObj->getInformationWidget(NULL);
      if (!infoField.pEquipmentInformationWidget)
        infoField.pEquipmentInformationWidget = new QLabel("n.a.");
      widgets.push_back(infoField.pEquipmentInformationWidget);
    
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIEquipment");
    }    
    if (CSWIForceEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWIForceEmitter>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIForceEmitter");
    }    
    if (CSWIImpulsEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWIImpulsEmitter>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIImpulsEmitter");
    }    
    if (CSWIPythonable::PtrType pObj = std::dynamic_pointer_cast<CSWIPythonable>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIPythonable");
    }  
    if (CSWIResourceProvider::PtrType pObj = std::dynamic_pointer_cast<CSWIResourceProvider>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pConnectedObjectsLabel = new QLabel;
      infoField.pConnectedObjectsLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pConnectedObjectsLabel);
    
      infoField.pTabWidget->addTab(createTab(widgets, true), "CSWIResourceProvider");
    }    
    if (CSWIRechargeable::PtrType pObj = std::dynamic_pointer_cast<CSWIRechargeable>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pRechargeInformationLabel = new QLabel;
      infoField.pRechargeInformationLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pRechargeInformationLabel);
    
      infoField.pTabWidget->addTab(createTab(widgets, true), "CSWIRechargeable");
    }    
    if (CSWISolid::PtrType pObj = std::dynamic_pointer_cast<CSWISolid>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pSizeLabel = new QLabel;
      infoField.pSizeLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pSizeLabel);
    
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWISolid");
    }    
    if (CSWISoundEmitter::PtrType pObj = std::dynamic_pointer_cast<CSWISoundEmitter>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWISoundEmitter");
    }    
    if (CSWISoundReceiver::PtrType pObj = std::dynamic_pointer_cast<CSWISoundReceiver>(pObject))
    {
      QList<QWidget*> widgets;

      infoField.pSoundsLabel = new QLabel;
      infoField.pSoundsLabel->setTextFormat(Qt::PlainText);
      widgets.push_back(infoField.pSoundsLabel);
    
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWISoundReceiver");
    }    
    if (CSWIUpdateable::PtrType pObj = std::dynamic_pointer_cast<CSWIUpdateable>(pObject))
    {
      QList<QWidget*> widgets;
      QLabel* pLabel = new QLabel;
      pLabel->setText("not implemented");
      widgets.push_back(pLabel);
      infoField.pTabWidget->addTab(createTab(widgets, false), "CSWIUpdateable");
    }    
  }
    

  QWidget* InformationView::createTab(QList<QWidget*>& widgets, bool bScrollable)
  {
    QWidget* pContent = new QWidget;
    QVBoxLayout* pVboxLayout = new QVBoxLayout;
    pVboxLayout->setSpacing(2);
    pVboxLayout->setMargin(1);

    QList<QWidget*>::iterator it = widgets.begin();
    for (; it != widgets.end(); ++it)
    {
      (*it)->setParent(pContent);
      pVboxLayout->addWidget(*it);
    }    
    pVboxLayout->addStretch();
    pContent->setLayout(pVboxLayout);

    if (bScrollable)
    {
      QScrollArea* pSV = new QScrollArea;
      pSV->setWidget(pContent);
      pSV->setWidgetResizable(true);
      return pSV;
    }  
    return pContent;
  }


  void InformationView::buildTree(QTreeWidget* pParent, CSWObject::PtrType pRootObject)
  {
    std::map<QString, std::pair<bool, bool> > properties; //opened, selected
  
    QTreeWidgetItemIterator itItem(pParent);
    while (*itItem) 
    {
      properties[(*itItem)->text(0)] = std::make_pair((*itItem)->isExpanded(), (*itItem)->isSelected());
      ++itItem;
    }
  
    pParent->setUpdatesEnabled(false);

    QString strSelectedItem;
    QList<QTreeWidgetItem*> selectedItems = m_pObjectTree->selectedItems(); 
    if (!selectedItems.empty())
      strSelectedItem = selectedItems.front()->text(0);

    pParent->clear();

    CSWObject::ChildRange range = pRootObject->getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        buildChilds(pParent, pChild, properties);
    }  

    pParent->sortItems(0, Qt::AscendingOrder);
  
    QList<QTreeWidgetItem*> items = pParent->findItems(strSelectedItem, Qt::MatchExactly, 0);  
    if (!items.empty()) 
    {
      items.front()->setSelected(true);
      pParent->scrollToItem(items.front());
    }

    pParent->setUpdatesEnabled(true);
  }


  void InformationView::buildChilds(QTreeWidget* pParent, CSWObject::PtrType pObject, const std::map<QString, std::pair<bool, bool> >& properties)
  {
    if (std::dynamic_pointer_cast<CSWCamera>(pObject))
      return;
    if (!(CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_WEAPONS_IN_OBJECT_TREE) &&
        std::dynamic_pointer_cast<CSWWeapon>(pObject))
      return;
  
    QTreeWidgetItem* pItem = new QTreeWidgetItem(pParent);
    pItem->setText(0, pObject->getName().c_str());
  
    std::map<QString, std::pair<bool, bool> >::const_iterator itFound = properties.find(pObject->getName().c_str());
    if (itFound != properties.end()) 
    {
      pItem->setExpanded(itFound->second.first);
  //    pItem->setSelected(itFound->second.second);
    }
  
    CSWObject::ChildRange range = pObject->getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        buildChilds(pItem, pChild, properties);
    }  
  }


  void InformationView::buildChilds(QTreeWidgetItem* pParent, CSWObject::PtrType pObject, const std::map<QString, std::pair<bool, bool> >& properties)
  {
    if (std::dynamic_pointer_cast<CSWCamera>(pObject))
      return;
    if (!(CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_WEAPONS_IN_OBJECT_TREE) &&
        std::dynamic_pointer_cast<CSWWeapon>(pObject))
      return;
  
    QTreeWidgetItem* pItem = new QTreeWidgetItem(pParent);
    pItem->setText(0, pObject->getName().c_str());
  
    std::map<QString, std::pair<bool, bool> >::const_iterator itFound = properties.find(pObject->getName().c_str());
    if (itFound != properties.end()) 
    {
      pItem->setExpanded(itFound->second.first);
  //    pItem->setSelected(itFound->second.second);
    }
  
    CSWObject::ChildRange range = pObject->getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        buildChilds(pItem, pChild, properties);
    }  
  }

}

#include "InformationViewMoc.cpp"
