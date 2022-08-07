// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "OpenGLView.h"

namespace CodeSubWars
{

  class CSWCamera;
  class ReplayDialog;

  class ReplayView : public OpenGLView
  {
    public:
      ReplayView(ReplayDialog* pReplayDialog);
    
      virtual void draw();
    
    protected:
      std::shared_ptr<CSWCamera> m_pMainCamera;
      ReplayDialog* m_pReplayDialog;
  };

}