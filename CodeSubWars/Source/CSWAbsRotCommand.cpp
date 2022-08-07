// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWAbsRotCommand.h"
#include "CSWControlCommand.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"


namespace CodeSubWars
{

  ARSTD::MacroCommand::PtrType CSWAbsRotCommand::create(CSWControlCenter::PtrType pControlCenter, 
                                                        const CSWAbsRotController::EngineContainer& engines,
                                                        const Vector3D& vecGPSTCenterOfMass,
                                                        const Vector3D& vecParameter, double fDirectionTolerance,
                                                        double fEndVelocity, double fVelocityTolerance, 
                                                        int nProperty,
                                                        double fMaxIntensity)
  {
    ARSTD::MacroCommand::PtrType pCommand = ARSTD::MacroCommand::create("AbsoluteRotation");
    CSWAbsRotController::PtrType pController = CSWAbsRotController::create(pControlCenter, engines,
                                                                           vecGPSTCenterOfMass,
                                                                           vecParameter, fDirectionTolerance, 
                                                                           fEndVelocity, fVelocityTolerance, 
                                                                           nProperty, fMaxIntensity);
    pCommand->attach(CSWControlCommand::create(pController)); 

    return pCommand;
  }

}