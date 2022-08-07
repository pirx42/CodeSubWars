// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWAxialInclRotationController.h"

namespace CodeSubWars
{

  class CSWEngine;
  class CSWControlCenter;

  class CSWAxialInclRotateCommand
  {
    public:
      //direction stuff is in degree; positiv means cw
      //velocity stuff is in degree/s; positiv means cw
      static ARSTD::MacroCommand::PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter, 
                                                 std::shared_ptr<CSWEngine> pEngine, 
                                                 double fSetRelativeDirection, double fDirectionTolerance,
                                                 double fEndVelocity, double fVelocityTolerance, 
                                                 int nProperty = CSWAxialInclRotationController::DIRECTION | 
                                                                 CSWAxialInclRotationController::VELOCITY,
                                                 double fMaxIntensity = 1.0);

  };

}