// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWAbsRotController.h"

namespace CodeSubWars
{

  class CSWEngine;
  class CSWControlCenter;

  class CSWAbsRotCommand
  {
    public:
      //direction stuff is in degree; positiv means cw
      //velocity stuff is in degree/s; positiv means ccw
      //its not recommend to use a direction tolerance lower than 0.1 degree
      static ARSTD::MacroCommand::PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter, 
                                                 const CSWAbsRotController::EngineContainer& engines,
                                                 const Vector3D& vecGPSTCenterOfMass,
                                                 const Vector3D& vecParameter, double fDirectionTolerance, //tolerance in degree
                                                 double fEndVelocity, double fVelocityTolerance,
                                                 int nProperties = CSWAbsRotController::INTERPRET_AS_DIRECTION | 
                                                                   CSWAbsRotController::INTERPRET_AS_ABSOLUTE | 
                                                                   CSWAbsRotController::CONTROL_DIRECTION | 
                                                                   CSWAbsRotController::CONTROL_VELOCITY,
                                                 double fMaxIntensity = 1.0);
  };

}