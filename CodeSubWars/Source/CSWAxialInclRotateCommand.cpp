// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWAxialInclRotateCommand.h"
#include "CSWAxialInclRotationController.h"
#include "CSWControlCommand.h"

namespace CodeSubWars
{

  ARSTD::MacroCommand::PtrType CSWAxialInclRotateCommand::create(std::shared_ptr<CSWControlCenter> pControlCenter, 
                                                                 std::shared_ptr<CSWEngine> pEngine, 
                                                                 double fSetRelativeDirection, double fDirectionTolerance,
                                                                 double fEndVelocity, double fVelocityTolerance, 
                                                                 int nProperty,
                                                                 double fMaxIntensity)
  {
    ARSTD::MacroCommand::PtrType pCommand = ARSTD::MacroCommand::create("RelativeAxialInclinationRotation");
    CSWAxialInclRotationController::PtrType pController = CSWAxialInclRotationController::create(pControlCenter, pEngine, 
                                                                                                 fSetRelativeDirection, fDirectionTolerance, 
                                                                                                 fEndVelocity, fVelocityTolerance, 
                                                                                                 nProperty, fMaxIntensity);
    pCommand->attach(CSWControlCommand::create(pController)); 

    return pCommand;
  }

}