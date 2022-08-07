// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWCommands.h"
#include "CSWSubmarine.h"
#include "CSWWeaponBattery.h"

#include "CSWAbsRotCommand.h"
#include "CSWAxialInclRotateCommand.h"
#include "CSWMoveForwardCommand.h"
#include "CSWMoveUpwardCommand.h"
#include "CSWInclRotateCommand.h"
#include "CSWRotateCommand.h"

#include "CSWWaitCommand.h"
#include "CSWVariousCommands.h"

#include "CSWEngine.h"
#include "CSWControlCenter.h"
#include "CSWGPS.h"

using ARSTD::Command;
using ARSTD::MacroCommand;

namespace CodeSubWars
{

  Command::PtrType CSWCommands::setMainEngineIntensity(CSWSubmarine* pSubmarine, double fIntensity)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getMainEngine(), fIntensity);
  }


  Command::PtrType CSWCommands::setMainEngineDirection(CSWSubmarine* pSubmarine, const double& fHAngle, const double& fVAngle)
  {
    return CSWSetEngineDirectionCommand::create(pSubmarine->getMainEngine(), fHAngle, fVAngle);
  }


  Command::PtrType CSWCommands::setMainEngineDirection(CSWSubmarine* pSubmarine, const Vector3D& vecDirection)
  {
    return CSWSetEngineDirectionCommand::create(pSubmarine->getMainEngine(), vecDirection);
  }


  Command::PtrType CSWCommands::disableMainEngine(CSWSubmarine* pSubmarine)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getMainEngine(), 0);
  }


  Command::PtrType CSWCommands::setBuoyancyTank(CSWSubmarine* pSubmarine, double fIntensity)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getBuoyancyTank(), fIntensity);
  }


  Command::PtrType CSWCommands::disableBuoyancyTank(CSWSubmarine* pSubmarine)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getBuoyancyTank(), 0);
  }


  Command::PtrType CSWCommands::setAxialInclinationJetOar(CSWSubmarine* pSubmarine, double fIntensity)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getAxialInclinationJetOar(), fIntensity);
  }


  Command::PtrType CSWCommands::disableAxialInclinationJetOar(CSWSubmarine* pSubmarine)
  {
    return CSWSetEngineIntensityCommand::create(pSubmarine->getAxialInclinationJetOar(), 0);
  }


  Command::PtrType CSWCommands::disableAllJetOars(CSWSubmarine* pSubmarine)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("DisableAllJetOars");
    pCmd->attach(CSWSetEngineIntensityCommand::create(pSubmarine->getAxialInclinationJetOar(), 0));
    pCmd->attach(CSWSetEngineIntensityCommand::create(pSubmarine->getInclinationJetOar(), 0));
    pCmd->attach(CSWSetEngineIntensityCommand::create(pSubmarine->getBowsJetOar(), 0));
    return pCmd;                                                        
  }


  Command::PtrType CSWCommands::disableAllEngines(CSWSubmarine* pSubmarine)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("DisableAllEngines");
    pCmd->attach(disableAllJetOars(pSubmarine));
    pCmd->attach(disableMainEngine(pSubmarine));
    pCmd->attach(disableBuoyancyTank(pSubmarine));
    return pCmd;                                                        
  }


  Command::PtrType CSWCommands::stopRotation(CSWSubmarine* pSubmarine)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("StopRotation");
    pCmd->attach(CSWAxialInclRotateCommand::create(pSubmarine->getControlCenter(), pSubmarine->getAxialInclinationJetOar(), 0, 1, 0, 0.1,
                                                   CSWAxialInclRotationController::VELOCITY,
                                                   1));
    pCmd->attach(CSWInclRotateCommand::create(pSubmarine->getControlCenter(), pSubmarine->getInclinationJetOar(), 0, 1, 0, 0.1,
                                              CSWInclRotationController::VELOCITY,
                                              1));
    pCmd->attach(CSWRotateCommand::create(pSubmarine->getControlCenter(), pSubmarine->getBowsJetOar(), 0, 1, 0, 0.1,
                                          CSWRotationController::VELOCITY,
                                          1));
    return pCmd;                                        
  }


  Command::PtrType CSWCommands::setOrientationAbsoluteTowardPosition(CSWSubmarine* pSubmarine, const Vector3D& vecPos,
                                                                     double fRotationTolerance, 
                                                                     double fVelocityTolerance)          
  {
    std::vector<CSWEngine::PtrType> engines;
    engines.push_back(pSubmarine->getInclinationJetOar());
    engines.push_back(pSubmarine->getBowsJetOar());
    engines.push_back(pSubmarine->getAxialInclinationJetOar());
    Matrix44D matGPSTWorld = pSubmarine->getControlCenter()->getGPS()->getInvertWorldTransform();
    Vector3D vecGPSTCenterOfMass = matGPSTWorld*pSubmarine->getWorldTransform()*pSubmarine->getCenterOfMass();

    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("SetOrientationAbsolutTowardPosition");

    pCmd->attach(CSWAbsRotCommand::create(pSubmarine->getControlCenter(),
                                          engines,
                                          vecGPSTCenterOfMass,
                                          vecPos, fRotationTolerance,
                                          0, fVelocityTolerance,
                                          CSWAbsRotController::INTERPRET_AS_POSITION | 
                                          CSWAbsRotController::INTERPRET_AS_ABSOLUTE | 
                                          CSWAbsRotController::CONTROL_DIRECTION | 
                                          CSWAbsRotController::CONTROL_VELOCITY,
                                          1));
    return pCmd;
  }


  Command::PtrType CSWCommands::setOrientationAbsoluteTowardDirection(CSWSubmarine* pSubmarine, const Vector3D& vecDir,
                                                                      double fRotationTolerance, 
                                                                      double fVelocityTolerance)          
  {
    std::vector<CSWEngine::PtrType> engines;
    engines.push_back(pSubmarine->getInclinationJetOar());
    engines.push_back(pSubmarine->getBowsJetOar());
    engines.push_back(pSubmarine->getAxialInclinationJetOar());
    Matrix44D matGPSTWorld = pSubmarine->getControlCenter()->getGPS()->getInvertWorldTransform();
    Vector3D vecGPSTCenterOfMass = matGPSTWorld*pSubmarine->getWorldTransform()*pSubmarine->getCenterOfMass();

    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("SetOrientationAbsolutTowardDirection");

    pCmd->attach(CSWAbsRotCommand::create(pSubmarine->getControlCenter(),
                                          engines,
                                          vecGPSTCenterOfMass,
                                          vecDir, fRotationTolerance,
                                          0, fVelocityTolerance,
                                          CSWAbsRotController::INTERPRET_AS_DIRECTION | 
                                          CSWAbsRotController::INTERPRET_AS_ABSOLUTE | 
                                          CSWAbsRotController::CONTROL_DIRECTION | 
                                          CSWAbsRotController::CONTROL_VELOCITY,
                                          1));
    return pCmd;
  }


  Command::PtrType CSWCommands::stopMoving(CSWSubmarine* pSubmarine)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("StopMove");
    pCmd->attach(CSWMoveForwardCommand::create(pSubmarine->getControlCenter(), pSubmarine->getMainEngine(), 
                                               0, 1,
                                               0, 0.2,
                                               CSWMoveForwardCommand::VELOCITY,
                                               1));
    pCmd->attach(CSWMoveUpwardCommand::create(pSubmarine->getControlCenter(), pSubmarine->getBuoyancyTank(), 
                                              0, 1,
                                              0, 0.2,
                                              CSWMoveUpwardCommand::VELOCITY,
                                              1));
    return pCmd;
  }


  Command::PtrType CSWCommands::moveForward(CSWSubmarine* pSubmarine, double fDistance,
                                            double fEndMoveVelocity, double fEndMoveVelocityTolerance)
  {
    return CSWMoveForwardCommand::create(pSubmarine->getControlCenter(), pSubmarine->getMainEngine(), 
                                         fDistance, 2,
                                         fEndMoveVelocity, fEndMoveVelocityTolerance,
                                         CSWMoveForwardCommand::POSITION | CSWMoveForwardCommand::VELOCITY,
                                         1);
  }


  Command::PtrType CSWCommands::moveUpward(CSWSubmarine* pSubmarine, double fDistance,
                                           double fEndMoveVelocity, double fEndMoveVelocityTolerance)
  {
    return CSWMoveUpwardCommand::create(pSubmarine->getControlCenter(), pSubmarine->getBuoyancyTank(), 
                                        fDistance, 2,
                                        fEndMoveVelocity, fEndMoveVelocityTolerance,
                                        CSWMoveUpwardCommand::POSITION | CSWMoveUpwardCommand::VELOCITY,
                                        1);
  }


  Command::PtrType CSWCommands::stopAll(CSWSubmarine* pSubmarine)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("StopAll");
    pCmd->attach(stopRotation(pSubmarine));
    pCmd->attach(stopMoving(pSubmarine));
    return pCmd;
  }


  Command::PtrType CSWCommands::fireSalvo(CSWWeaponBattery::PtrType& pWeaponBattery, int nNumTorpedos, double fTimeBetweenLaunches)
  {
    MacroCommand::PtrType pCmd = ARSTD::MacroCommand::create("FireSalvo");
    for (int i = 0; i < nNumTorpedos; ++i)
    {
      if (i != 0)
        pCmd->attach(CSWWaitCommand::create(fTimeBetweenLaunches));
      pCmd->attach(CSWFireCommand::create(pWeaponBattery));
    }
    return pCmd;
  }

}