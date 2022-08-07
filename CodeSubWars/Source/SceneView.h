// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include <boost/weak_ptr.hpp>

#include "OpenGLView.h"

namespace CodeSubWars
{

  class CSWObject;

  class SceneView : public OpenGLView
  {
      Q_OBJECT

    public:
      SceneView(QWidget* pParent, bool bAbsolutCameraSelection,
                const QGLWidget* pShareWidget = 0, 
                Qt::WindowFlags f = 0);

      virtual ~SceneView();

      virtual void draw();

      void setCamera(const std::string& strName);

    signals:
      void selectionChanged();

    public slots:
      void rebuild();

    protected:
      virtual void mousePressEvent(QMouseEvent*);

      bool m_bAbsolutCameraSelection;
      std::string m_strObjectPostfix;
      std::weak_ptr<CSWObject> m_pLastSelectedObject;
  };

}