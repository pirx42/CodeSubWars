// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class SceneView;
  class InformationView;
  class SystemView;

  class CodeSubwarsMainWindow : public QMainWindow
  {
      Q_OBJECT

    public:
      CodeSubwarsMainWindow();

      virtual ~CodeSubwarsMainWindow();

    signals:
      void redraw();

    protected slots:
      virtual void closeEvent(QCloseEvent*);
      void setTimeRatio(int);
      void recalcWorld();
      void updateTimeLabel();

      void newWorldDefault1();
      void newWorldDefault2();
      void newWorldDefault3();
      void newWorldDefault4();
      void newWorldDefault5();
      void newWorldLoad();

      void newBattle();

      void stopBattle();

      void replayBattle();

      void preferences();
      void help();
      void manual();
      void website();
      void about();
      void makeScreenshot();

    protected:
      void checkRequirements();
  
      void setupMenu();
      void setupGUI();
      void setupConnections();

      SceneView* m_pMainSceneView;
      SceneView* m_pDetailUpperSceneView;
      InformationView* m_pDetailLowerInformationView;
      SystemView* m_pSystemView;

      QLabel* m_pTimeLabel;
      QLabel* m_pSliderTimeRatio;
      QSlider* m_pTimeSlider;

      QTimer m_UpdateTimer;
      QTimer m_RecalcWorldTimer;
  };

}