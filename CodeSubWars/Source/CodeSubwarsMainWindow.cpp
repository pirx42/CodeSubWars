// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "Constants.h"
#include "CSWLog.h"
#include "SceneView.h"
#include "InformationView.h"
#include "SystemView.h"
#include "CSWWorld.h"
#include "CodeSubwarsMainWindow.h"
#include "CSWSettings.h"
#include "AboutDialog.h"
#include "OptionDialog.h"
#include "ReplayDialog.h"
#include "NewBattleDialog.h"
#include "CSWUtilities.h"


namespace CodeSubWars
{

  CodeSubwarsMainWindow::CodeSubwarsMainWindow()
  : QMainWindow()
  {
    checkRequirements();

    setWindowTitle(QString("CodeSubWars ") + Constants::getVersion().getAsString().c_str());
    setEnabled(TRUE);

    setCentralWidget(new QWidget(this));

    QFont pFont("Curier", 8, QFont::Normal, FALSE);
    setFont(pFont); 

    setupMenu();
    setupGUI();
    setupConnections();

    newWorldDefault1();
  
    showNormal();
  }


  CodeSubwarsMainWindow::~CodeSubwarsMainWindow()
  {
    CSWWorld::getInstance()->finalizeWorld();
    CSWLog::getInstance()->setLogOutput(NULL);
  }


  void CodeSubwarsMainWindow::closeEvent(QCloseEvent* pEvent)
  {
    CSWWorld::getInstance()->finalizeWorld();
    m_UpdateTimer.stop();
    m_RecalcWorldTimer.stop();
    QMainWindow::closeEvent(pEvent);
  }


  void CodeSubwarsMainWindow::setTimeRatio(int nRatio)
  {
    m_pSliderTimeRatio->setText(QString("%1").arg(nRatio/4.0, 5, 'f', 2, QChar(' ')));//QString::number(nRatio/4.0, 'f', 2) + " ");
    ARSTD::Time::setTimeRatio(nRatio/4.0);
  }


  void CodeSubwarsMainWindow::recalcWorld()
  {
    CSWWorld::getInstance()->recalculate();
  }


  void CodeSubwarsMainWindow::updateTimeLabel()
  {
    double fSecs = ARSTD::Time::getTime();
    int nHSecs = (fSecs - static_cast<int>(fSecs))*100;
    int nHours = static_cast<int>(fSecs)/3600;
    int nMins = (static_cast<int>(fSecs)%3600)/60;
    int nSecs = static_cast<int>(fSecs)%60;
    m_pTimeLabel->setText(QString("%1:%2:%3.%4").arg(nHours, 3, 10, QChar(' '))
                                                .arg(nMins, 2, 10, QChar('0'))
                                                .arg(nSecs, 2, 10, QChar('0'))
                                                .arg(nHSecs, 2, 10, QChar('0')));
  }


  void CodeSubwarsMainWindow::newWorldDefault1()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::DEFAULT_1);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newWorldDefault2()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::DEFAULT_2);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newWorldDefault3()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::DEFAULT_3);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newWorldDefault4()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::DEFAULT_4);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newWorldDefault5()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::DEFAULT_5);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newWorldLoad()
  {
    m_pTimeSlider->setValue(4);
    m_UpdateTimer.start(40);

    CSWWorld::getInstance()->newWorld(CSWWorld::LOAD_WORLD);
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::newBattle()
  {
    NewBattleDialog dlg(qApp->applicationDirPath() + "/submarines");
    if (dlg.exec() == QDialog::Rejected)
      return;

    m_pTimeSlider->setValue(4);
    m_RecalcWorldTimer.start(10);
    m_UpdateTimer.start(40);
 
    CSWWorld::getInstance()->newBattle(dlg.getSubmarines(), 
                                       static_cast<CSWWorld::BattleType>(dlg.getBattleMode()),
                                       dlg.getTeamSize());
  
    m_pMainSceneView->setCamera("envMainCamera");
    m_pDetailLowerInformationView->enforceCompleteRebuild(true);
  }


  void CodeSubwarsMainWindow::stopBattle()
  {
    m_RecalcWorldTimer.stop();
    m_UpdateTimer.stop();

    CSWWorld::getInstance()->finalizeBattle();

    m_pMainSceneView->setCamera("envMainCamera");
    m_pTimeSlider->setValue(4);
    emit redraw();
  }


  void CodeSubwarsMainWindow::replayBattle()
  {
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                       "Choose a battle record file to open",
                                                       qApp->applicationDirPath() + "/records",
                                                       "CSW Battle Records (*.cbr)");
    if (strFileName.isEmpty())
      return;
    
  //  bool bRecalcRunning = m_RecalcWorldTimer.isActive();
  //  bool bUpdateRunning = m_UpdateTimer.isActive();
  //  double fCurrentTime = ARSTD::Time::getTime();
  //  double fCurrentTimeRatio = ARSTD::Time::getTimeRatio();
  //
  //  m_RecalcWorldTimer.stop();
  //  m_UpdateTimer.stop();
  //
  //  emit redraw();

    stopBattle();

    ReplayDialog dlg(strFileName);
    dlg.exec();

    ARSTD::Time::reset();
    ARSTD::Time::setTimeRatio(0);

    emit redraw();

  //  ARSTD::Time::setTime(fCurrentTime);
  //  ARSTD::Time::setTimeRatio(fCurrentTimeRatio);
  //  if (bRecalcRunning)
  //    m_RecalcWorldTimer.start(10);
  //  if (bUpdateRunning)
  //    m_UpdateTimer.start(40);
  }


  void CodeSubwarsMainWindow::preferences()
  {
    OptionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
      CSWWorld::getInstance()->getSettings()->setCollidableProperties(dlg.getCollidableProperties());
      CSWWorld::getInstance()->getSettings()->setDamageableProperties(dlg.getDamageableProperties());
      CSWWorld::getInstance()->getSettings()->setDynamicProperties(dlg.getDynamicProperties());
      CSWWorld::getInstance()->getSettings()->setVariousProperties(dlg.getVariousProperties());
    }
  }


  void CodeSubwarsMainWindow::help()
  {
    ShellExecuteA(NULL, NULL, (LPCSTR)(qApp->applicationDirPath() + "/doc/html/index.html").toStdString().c_str(), NULL, NULL, SW_SHOWNORMAL);
  }


  void CodeSubwarsMainWindow::manual()
  {
    ShellExecuteA(NULL, NULL, (LPCSTR)(qApp->applicationDirPath() + "/doc/Manual.pdf").toStdString().c_str(), NULL, NULL, SW_SHOWNORMAL);
  }


  void CodeSubwarsMainWindow::website()
  {
    ShellExecuteA(NULL, NULL, (LPCSTR)Constants::WEBSITE.toStdString().c_str(), NULL, NULL, SW_SHOWNORMAL);
  }


  void CodeSubwarsMainWindow::about()
  {
    AboutDialog(this).exec();
  }


  void CodeSubwarsMainWindow::makeScreenshot()
  {
    QPixmap screenshot = QPixmap::grabWindow(this->winId());
    QString strPath = qApp->applicationDirPath() + "/Screenshots";
    QDir().mkdir(strPath);
    screenshot.save(strPath + "/" + CSWUtilities::getUniqueFilename(strPath) + ".png", "PNG");
  }


  void CodeSubwarsMainWindow::checkRequirements()
  {
    const GLubyte* nGLVersion = glGetString(GL_VERSION);
  }


  void CodeSubwarsMainWindow::setupMenu()
  {
    QMenu* pBattleMenu = menuBar()->addMenu(tr("&Battle"));
      connect(pBattleMenu->addAction("&New Battle..."), SIGNAL(triggered()), this, SLOT(newBattle()));
      connect(pBattleMenu->addAction("&Stop Battle"), SIGNAL(triggered()), this, SLOT(stopBattle()));
      connect(pBattleMenu->addAction("&Replay Battle"), SIGNAL(triggered()), this, SLOT(replayBattle()));
      connect(pBattleMenu->addAction("&Exit"), SIGNAL(triggered()), this, SLOT(close()));

    QMenu* pWorldMenu = menuBar()->addMenu(tr("&World"));
      connect(pWorldMenu->addAction("Default &1 (Empty)"), SIGNAL(triggered()), this, SLOT(newWorldDefault1()));
      connect(pWorldMenu->addAction("Default &2 (Rocks)"), SIGNAL(triggered()), this, SLOT(newWorldDefault2()));
      connect(pWorldMenu->addAction("Default &3 (Weapon supply)"), SIGNAL(triggered()), this, SLOT(newWorldDefault3()));
      connect(pWorldMenu->addAction("Default &4 (Black hole)"), SIGNAL(triggered()), this, SLOT(newWorldDefault4()));
      connect(pWorldMenu->addAction("Default &5 (Active Rocks)"), SIGNAL(triggered()), this, SLOT(newWorldDefault5()));

    QMenu* pOptionsMenu = menuBar()->addMenu(tr("&Options"));
      connect(pOptionsMenu->addAction("&Preferences"), SIGNAL(triggered()), this, SLOT(preferences()));

    QMenu* pHelpMenu = menuBar()->addMenu(tr("&Help"));
      connect(pHelpMenu->addAction("&Html Reference"), SIGNAL(triggered()), this, SLOT(help()));
      connect(pHelpMenu->addAction("&Manual"), SIGNAL(triggered()), this, SLOT(manual()));
      connect(pHelpMenu->addAction("&Website"), SIGNAL(triggered()), this, SLOT(website()));
      connect(pHelpMenu->addAction("&About"), SIGNAL(triggered()), this, SLOT(about()));
  }


  void CodeSubwarsMainWindow::setupGUI()
  {
    QFont f;
    f.setFamily("Courier");
    f.setPointSize(8);

    QVBoxLayout* pVBoxLayout = new QVBoxLayout(centralWidget());
    pVBoxLayout->setSpacing(2);
    pVBoxLayout->setMargin(0);

    QSplitter* pVerticalSplitter = new QSplitter(Qt::Vertical, centralWidget());
    pVBoxLayout->addWidget(pVerticalSplitter);

      QSplitter* pHorizontalSplitter = new QSplitter(Qt::Horizontal);
      pVerticalSplitter->addWidget(pHorizontalSplitter);

        m_pMainSceneView = new SceneView(NULL, true);
        pHorizontalSplitter->addWidget(m_pMainSceneView);

        QSplitter* pDetailVerticalSplitter = new QSplitter(Qt::Vertical);
        pHorizontalSplitter->addWidget(pDetailVerticalSplitter);
      
          m_pDetailUpperSceneView = new SceneView(NULL, false);
          pDetailVerticalSplitter->addWidget(m_pDetailUpperSceneView);
          m_pDetailLowerInformationView = new InformationView;
          pDetailVerticalSplitter->addWidget(m_pDetailLowerInformationView);
 
        QList<int> list;
        list.push_back(height()*0.5);
        list.push_back(height()*0.5);
        pDetailVerticalSplitter->setSizes(list);
  //   
      QTextEdit* pLogView = new QTextEdit;
  //    pLogView->setTextFormat(Qt::LogText);  
      pLogView->setAutoFormatting(QTextEdit::AutoNone);
      pLogView->setLineWrapMode(QTextEdit::NoWrap);
      pLogView->setWordWrapMode(QTextOption::NoWrap);
      pLogView->setReadOnly(true);
      pLogView->setFont(f);
      CSWLog::getInstance()->setLogOutput(pLogView);
      pVerticalSplitter->addWidget(pLogView);
    

    QHBoxLayout* pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setMargin(10);
    pHBoxLayout->setSpacing(10);
    pVBoxLayout->addLayout(pHBoxLayout);

      QLabel* pTimeDesc = new QLabel("Time:");
      pTimeDesc->setFont(f);
      pHBoxLayout->addWidget(pTimeDesc);

      m_pTimeLabel = new QLabel("0:00:00.00");
      m_pTimeLabel->setFont(f);
      pHBoxLayout->addWidget(m_pTimeLabel);


      QLabel* pSliderDesc = new QLabel("Time ratio: x");
      pSliderDesc->setFont(f);
      pHBoxLayout->addWidget(pSliderDesc);

      m_pSliderTimeRatio = new QLabel("1.00");
      m_pSliderTimeRatio->setFont(f);
      pHBoxLayout->addWidget(m_pSliderTimeRatio);


      m_pTimeSlider = new QSlider(Qt::Horizontal);
      m_pTimeSlider->setRange(0.25*4, 10*4);
      m_pTimeSlider->setSingleStep(1*4);
      m_pTimeSlider->setSliderPosition(0.25*4);
      m_pTimeSlider->setTickInterval(4);
      m_pTimeSlider->setTickPosition(QSlider::TicksBelow);
      pHBoxLayout->addWidget(m_pTimeSlider);

    m_pSystemView = new SystemView(NULL);
    statusBar()->addPermanentWidget(m_pSystemView);
  
    connect(m_pTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(setTimeRatio(int)));
  }


  void CodeSubwarsMainWindow::setupConnections()
  {
    QAction* pMakeScreenshot = new QAction(this);
    pMakeScreenshot->setShortcut(QKeySequence(Qt::Key_P));
    connect(pMakeScreenshot, SIGNAL(triggered()), this, SLOT(makeScreenshot()));
    addAction(pMakeScreenshot);

    connect(&m_UpdateTimer, SIGNAL(timeout()), this, SIGNAL(redraw()));
    connect(&m_RecalcWorldTimer, SIGNAL(timeout()), this, SLOT(recalcWorld()));

    connect(this, SIGNAL(redraw()), m_pMainSceneView, SLOT(update()));
    connect(this, SIGNAL(redraw()), m_pDetailUpperSceneView, SLOT(update()));
    connect(this, SIGNAL(redraw()), m_pDetailLowerInformationView, SLOT(updateContent()));
    connect(this, SIGNAL(redraw()), m_pSystemView, SLOT(updateContent()));
    connect(this, SIGNAL(redraw()), this, SLOT(updateTimeLabel()));
 
 
    connect(m_pMainSceneView, SIGNAL(selectionChanged()), m_pDetailUpperSceneView, SLOT(rebuild()));
    connect(m_pMainSceneView, SIGNAL(selectionChanged()), m_pDetailLowerInformationView, SLOT(rebuild()));
 
    connect(m_pDetailLowerInformationView, SIGNAL(selectionChanged()), m_pDetailUpperSceneView, SLOT(rebuild()));
 
    connect(m_pDetailUpperSceneView, SIGNAL(selectionChanged()), m_pDetailLowerInformationView, SLOT(rebuild()));
    connect(m_pDetailUpperSceneView, SIGNAL(selectionChanged()), m_pDetailUpperSceneView, SLOT(rebuild()));
  }


}

#include "CodeSubwarsMainWindowMoc.cpp"
