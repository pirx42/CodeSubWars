// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWRotationController.h"

namespace CodeSubWars
{

  class CSWEngine;
  class CSWControlCenter;

  class CSWRotateCommand
  {
    public:
      //direction stuff is in degree; positiv means cw
      //velocity stuff is in degree/s; positiv means ccw
      static ARSTD::MacroCommand::PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter, 
                                                 std::shared_ptr<CSWEngine> pEngine, 
                                                 double fSetRelativeDirection, double fDirectionTolerance,
                                                 double fEndVelocity, double fVelocityTolerance,
                                                 int nProperty = CSWRotationController::DIRECTION | 
                                                                 CSWRotationController::VELOCITY,
                                                 double fMaxIntensity = 1.0);

  };

}