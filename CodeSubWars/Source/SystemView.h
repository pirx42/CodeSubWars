// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

namespace CodeSubWars
{

  class CSWObject;
  class CSWSettings;

  class SystemView : public QWidget
  {
      Q_OBJECT

    public:
      SystemView(QWidget* pParent);

      virtual ~SystemView();

      virtual QSize sizeHint() const;

    public slots:
      void updateContent();

    protected:
      QLabel* m_pLabel;

  };

}