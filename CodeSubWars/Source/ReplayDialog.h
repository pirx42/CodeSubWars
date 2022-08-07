// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "IO/CSWBRImporter.h"

namespace CodeSubWars
{

  class ReplayView;

  class ReplayDialog : public QDialog
  {
      Q_OBJECT

    public:
      ReplayDialog(QString strFileName, QWidget* pParent = NULL);
      virtual ~ReplayDialog();

      const CSWBRImporter::TimeSliceData& getData() const;

    signals:
      void redraw();

    protected slots:
      void mult2();
      void reverse(bool bOn);
      void play(bool bOn);
      void div2();

      void setTime(int nProgressMult10000);
    
      void holdOn();
      void goOn();
    
      void updateContent();

    protected:
      ReplayView* m_pMainSceneView;
      QLabel* m_pTimeLabel;
      QPushButton* m_pPlayButton;
      QPushButton* m_pReverseButton;
      QSlider* m_pTimeSlider;
      QTimer m_RedrawTimer;
      QTimer m_UpdateTimer;
    
      CSWBRImporter::TimeSliceDataContainer m_TimeSliceDataContainer;    
      CSWBRImporter::TimeSliceData m_CurrentTimeSliceData;
      double m_fMinTime;
      double m_fMaxTime;
      double m_fLastTimeRatio;
  };

}