// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWEngineVisualizer.h"
#include "CSWEngine.h"


namespace CodeSubWars
{

  CSWEngineVisualizer::CSWEngineVisualizer(QWidget* pParent, CSWEngine::PtrType pEngine)
  : QWidget(pParent),
    m_pEngine(pEngine)
  {
    QHBoxLayout* pLayout = new QHBoxLayout(this); 
    pLayout->setSpacing(2);
    pLayout->setMargin(0);
    m_pLabel = new QLabel(this);
    m_pLabel->setTextFormat(Qt::PlainText);
    pLayout->addWidget(m_pLabel, 0, Qt::AlignTop | Qt::AlignLeft);
  }


  void CSWEngineVisualizer::updateContent()
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
    {
      std::stringstream ss;
      ss.precision(3);
      ss.setf(std::ios::fixed);
      ss << "Maximal Force [MN]: " << pEngine->m_fMaxForce/1e+6 << "\n"
         << "Intensity [%]: " << pEngine->m_fIntensity*100 << "\n"
         << "Force [MN]: " << pEngine->m_fIntensity*pEngine->m_fMaxForce/1e+6 << "\n"       
         << "Direction changes possible: " << pEngine->m_bAllowDirectionChanges << "\n";       
      m_pLabel->setText(ss.str().c_str());
      update();
    }  
  }

}