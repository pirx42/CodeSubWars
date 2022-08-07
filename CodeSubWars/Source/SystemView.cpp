// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWSettings.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "SystemView.h"


namespace CodeSubWars
{

  SystemView::SystemView(QWidget* parent)
  : QWidget(parent)
  {
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(1);

    QFont f;
    f.setFamily("Courier");
    f.setPointSize(8);
    m_pLabel = new QLabel(this);
    m_pLabel->setFont(f); 
    m_pLabel->setTextFormat(Qt::PlainText);
    pLayout->addWidget(m_pLabel);
  }


  SystemView::~SystemView()
  {
  }


  QSize SystemView::sizeHint() const
  {
    return QSize(parentWidget()->width(), m_pLabel->height());
  }


  void SystemView::updateContent()
  {
    const std::vector<boost::tuples::tuple<std::string, double, double> >& load = CSWWorld::getInstance()->getLoad();
    if (load.empty())
      return;

    std::stringstream ss;
    ss.precision(1);
    ss.setf(std::ios::fixed | std::ios::right);

    std::vector<boost::tuples::tuple<std::string, double, double> >::const_iterator it = load.begin();
    ss << boost::tuples::get<0>(*it) << " ";
    ss.width(5);
    ss << boost::tuples::get<1>(*it);
    ss << "/";
    ss.width(5);
    ss << boost::tuples::get<2>(*it);
    ++it;
    for (; it != load.end(); ++it)
    {
      ss << ",  " << boost::tuples::get<0>(*it) << " ";
      ss.width(5);
      ss << boost::tuples::get<1>(*it);
      ss << "/";
      ss.width(5);
      ss << boost::tuples::get<2>(*it);
    }

    m_pLabel->setText(QString("Timings [ms]: ") + ss.str().c_str());

    update();
  }

}

#include "SystemViewMoc.cpp"
