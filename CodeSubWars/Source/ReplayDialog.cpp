// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "ReplayDialog.h"
#include "ReplayView.h"
#include "CSWUtilities.h"


namespace CodeSubWars
{

  bool timeSliceLess(const std::pair<double, CSWBRImporter::TimeSliceData>& lhs, 
                     const std::pair<double, CSWBRImporter::TimeSliceData>& rhs)
  {
    return lhs.first < rhs.first;
  }


  ReplayDialog::ReplayDialog(QString strFileName, QWidget* pParent)
  : QDialog(pParent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint)
  {
    setWindowTitle("Replay");
    setSizeGripEnabled(true);

    QFont f;
    f.setFamily("Courier");
    f.setPointSize(8);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(10);

      m_pMainSceneView = new ReplayView(this);
      pMainLayout->addWidget(m_pMainSceneView);

      QHBoxLayout* pControlLayout = new QHBoxLayout;
      pMainLayout->addLayout(pControlLayout);
    
        pControlLayout->addStretch();

        QLabel* pTimeDesc = new QLabel("Time:");
        pTimeDesc->setFont(f);
        pControlLayout->addWidget(pTimeDesc);

        m_pTimeLabel = new QLabel("0:00:00.00");
        m_pTimeLabel->setFont(f);
        pControlLayout->addWidget(m_pTimeLabel);

        pControlLayout->addStretch();

        QPushButton* pButton = new QPushButton(tr("x2"), this);
        pButton->setFocus();
        pControlLayout->addWidget(pButton);
        connect(pButton, SIGNAL(clicked()), this, SLOT(mult2()));      

        pButton = new QPushButton(tr("/2"), this);
        pButton->setFocus();
        pControlLayout->addWidget(pButton);
        connect(pButton, SIGNAL(clicked()), this, SLOT(div2()));      

        m_pReverseButton = new QPushButton(tr("<"), this);
        m_pReverseButton->setFocus();
        m_pReverseButton->setCheckable(true);
        pControlLayout->addWidget(m_pReverseButton);
        connect(m_pReverseButton, SIGNAL(toggled(bool)), this, SLOT(reverse(bool)));      

        m_pPlayButton = new QPushButton(tr(">"), this);
        m_pPlayButton->setFocus();
        m_pPlayButton->setCheckable(true);
        pControlLayout->addWidget(m_pPlayButton);
        connect(m_pPlayButton, SIGNAL(toggled(bool)), this, SLOT(play(bool)));      

        pControlLayout->addStretch();
      
      m_pTimeSlider = new QSlider(Qt::Horizontal, this);
      m_pTimeSlider->setRange(0, 10000);
      m_pTimeSlider->setSingleStep(1);
      m_pTimeSlider->setSliderPosition(0);
      m_pTimeSlider->setTickPosition(QSlider::TicksBelow);
      m_pTimeSlider->setTickInterval(1000);
      pMainLayout->addWidget(m_pTimeSlider);

    
    connect(m_pTimeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setTime(int)));
    connect(m_pTimeSlider, SIGNAL(sliderPressed()), this, SLOT(holdOn()));
    connect(m_pTimeSlider, SIGNAL(sliderReleased()), this, SLOT(goOn()));

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  
    bool bResult = CSWBRImporter::loadREC(strFileName, m_TimeSliceDataContainer);
    if (!bResult) m_TimeSliceDataContainer.clear();

    QApplication::restoreOverrideCursor();
  
    if (!m_TimeSliceDataContainer.empty())
    {
      m_fMinTime = m_TimeSliceDataContainer.begin()->first;
      m_fMaxTime = boost::prior(m_TimeSliceDataContainer.end())->first;
    }
    else
    {
      QMessageBox::information(NULL, "Error", "CSW battle record file could not read successfully.");
      m_fMinTime = 0;
      m_fMaxTime = 0;
    }
  
    ARSTD::Time::reset(ARSTD::Time::AUTOMATIC, m_fMinTime);
    ARSTD::Time::setTimeRatio(1);
    m_pPlayButton->setChecked(true);


    connect(&m_RedrawTimer, SIGNAL(timeout()), m_pMainSceneView, SLOT(update()));
    connect(&m_UpdateTimer, SIGNAL(timeout()), this, SLOT(updateContent()));

    m_UpdateTimer.start(50);
    m_RedrawTimer.start(40);
  }


  ReplayDialog::~ReplayDialog()
  {
  }


  const CSWBRImporter::TimeSliceData& ReplayDialog::getData() const
  {
    return m_CurrentTimeSliceData;
  }


  void ReplayDialog::mult2()
  {
    ARSTD::Time::setTimeRatio(ARSTD::Time::getTimeRatio()*2);
  }


  void ReplayDialog::reverse(bool bOn)
  {
    m_pPlayButton->blockSignals(true);
    m_pPlayButton->setChecked(false);
    m_pPlayButton->blockSignals(false);

    if (ARSTD::Time::getTime() <= m_fMinTime)
    {
      m_pReverseButton->setChecked(false);
      return;
    }

    if (bOn)
    {
      double fRatio = ARSTD::Time::getTimeRatio();
      if (fabs(fRatio) < EPSILON) fRatio = 1;
      ARSTD::Time::setTimeRatio(-fabs(fRatio));
    }
    else
    {
      ARSTD::Time::setTimeRatio(0);
    }
  }


  void ReplayDialog::play(bool bOn)
  {
    m_pReverseButton->blockSignals(true);
    m_pReverseButton->setChecked(false);
    m_pReverseButton->blockSignals(false);

    if (ARSTD::Time::getTime() >= m_fMaxTime)
    {
      m_pPlayButton->setChecked(false);
      return;
    }

    if (bOn)
    {
      double fRatio = ARSTD::Time::getTimeRatio();
      if (fabs(fRatio) < EPSILON) fRatio = 1;
      ARSTD::Time::setTimeRatio(fabs(fRatio));
    }
    else
    {
      ARSTD::Time::setTimeRatio(0);
    }
  }


  void ReplayDialog::div2()
  {
    ARSTD::Time::setTimeRatio(ARSTD::Time::getTimeRatio()/2);
  }


  void ReplayDialog::setTime(int nProgressMult10000)
  {
    ARSTD::Time::setTime(m_fMinTime + (m_fMaxTime - m_fMinTime)*nProgressMult10000/10000);
  }


  void ReplayDialog::holdOn()
  {
    m_fLastTimeRatio = ARSTD::Time::getTimeRatio();
    ARSTD::Time::setTimeRatio(0);
  }


  void ReplayDialog::goOn()
  {
    ARSTD::Time::setTimeRatio(m_fLastTimeRatio);
  }


  void ReplayDialog::updateContent()
  {
    if (m_TimeSliceDataContainer.empty()) return;
  
    double fCurrentTime = ARSTD::Time::getTime();
    if (fCurrentTime < m_fMinTime) 
    {
      ARSTD::Time::setTime(m_fMinTime);
      ARSTD::Time::setTimeRatio(0);
      m_pReverseButton->setChecked(false);
      m_pPlayButton->setChecked(false);
    }
    if (fCurrentTime > m_fMaxTime) 
    {
      ARSTD::Time::setTime(m_fMaxTime);
      ARSTD::Time::setTimeRatio(0);
      m_pReverseButton->setChecked(false);
      m_pPlayButton->setChecked(false);
    }

    double fSecs = ARSTD::Time::getTime();
    int nHSecs = (fSecs - static_cast<int>(fSecs))*100;
    int nHours = static_cast<int>(fSecs)/3600;
    int nMins = (static_cast<int>(fSecs)%3600)/60;
    int nSecs = static_cast<int>(fSecs)%60;
    m_pTimeLabel->setText(QString("%1:%2:%3:%4").arg(nHours, 3, 10, QChar(' '))
                                                .arg(nMins, 2, 10, QChar('0'))
                                                .arg(nSecs, 2, 10, QChar('0'))
                                                .arg(nHSecs, 2, 10, QChar('0')));


    double fValue = (fCurrentTime - m_fMinTime)/(m_fMaxTime - m_fMinTime);
    m_pTimeSlider->blockSignals(true);
    m_pTimeSlider->setValue(fValue*10000);
    m_pTimeSlider->blockSignals(false);

    //load regarding data to current time
    CSWBRImporter::TimeSliceDataContainer::const_iterator itFound = lower_bound(m_TimeSliceDataContainer.begin(), 
                                                                                m_TimeSliceDataContainer.end(),
                                                                                std::make_pair(fCurrentTime, CSWBRImporter::TimeSliceData()),
                                                                                timeSliceLess);
    if (itFound == m_TimeSliceDataContainer.end()) return;
    if (itFound == m_TimeSliceDataContainer.begin())
    {
      m_CurrentTimeSliceData = itFound->second;
      return;
    }
  
    CSWBRImporter::TimeSliceDataContainer::const_iterator itFoundPrior = boost::prior(itFound);
  
    //linear interpolate
    double fInterpolateFactor = (fCurrentTime - itFoundPrior->first)/(itFound->first - itFoundPrior->first);

    const CSWBRImporter::TimeSliceData& firstObjectData = itFoundPrior->second;
    const CSWBRImporter::TimeSliceData& secondObjectData = itFound->second;

    std::set<std::string> objectNames;
    CSWBRImporter::TimeSliceData::const_iterator itFirstData = firstObjectData.begin();
    for (; itFirstData != firstObjectData.end(); ++itFirstData)
    {
      objectNames.insert(itFirstData->first);
    }

    CSWBRImporter::TimeSliceData::const_iterator itSecondData = secondObjectData.begin();
    for (; itSecondData != secondObjectData.end(); ++itSecondData)
    {
      objectNames.insert(itSecondData->first);
    }

  
    CSWBRImporter::ObjectData objData;
    m_CurrentTimeSliceData.clear();
    std::set<std::string>::const_iterator itName = objectNames.begin();
    for (; itName != objectNames.end(); ++itName)
    {
      CSWBRImporter::TimeSliceData::const_iterator itFoundObject1 = firstObjectData.find(*itName);
      CSWBRImporter::TimeSliceData::const_iterator itFoundObject2 = secondObjectData.find(*itName);
      if (itFoundObject1 != firstObjectData.end() &&
          itFoundObject2 != secondObjectData.end())
      {
        objData = itFoundObject1->second;
        objData.matWorldTObject = ARSTD::Matrix44D::slerp(itFoundObject1->second.matWorldTObject, 
                                                          itFoundObject2->second.matWorldTObject, 
                                                          fInterpolateFactor);
        objData.color = CSWUtilities::interpolateComponentsLinear(itFoundObject1->second.color, 
                                                                  itFoundObject2->second.color, 
                                                                  fInterpolateFactor);
      
        m_CurrentTimeSliceData[*itName] = objData;
      }
      else if (itFoundObject1 != firstObjectData.end())
      {
        m_CurrentTimeSliceData[*itName] = itFoundObject1->second;
      }
    }
  }

}



#include "ReplayDialogMoc.cpp"
