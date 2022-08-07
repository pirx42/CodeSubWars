// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWInclRotationController.h"

namespace CodeSubWars
{

  class CSWEngine;
  class CSWControlCenter;

  class CSWInclRotateCommand
  {
    public:
      //direction stuff is in degree; positiv means upwards
      //velocity stuff is in degree/s; positiv means downwards
      static ARSTD::MacroCommand::PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter, 
                                                 std::shared_ptr<CSWEngine> pEngine, 
                                                 double fSetRelativeDirection, double fDirectionTolerance,
                                                 double fEndVelocity, double fVelocityTolerance, 
                                                 int nProperty = CSWInclRotationController::DIRECTION | 
                                                                 CSWInclRotationController::VELOCITY,
                                                 double fMaxIntensity = 1.0);

  };

}