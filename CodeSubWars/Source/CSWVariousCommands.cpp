// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWVariousCommands.h"
#include "CSWEngine.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"
#include "CSWWeapon.h"
#include "CSWWeaponBattery.h"


namespace CodeSubWars
{

  CSWSetEngineIntensityCommand::PtrType CSWSetEngineIntensityCommand::create(CSWEngine::PtrType& pEngine, const double& fIntensity)
  {
    return PtrType(new CSWSetEngineIntensityCommand(pEngine, fIntensity));
  }


  ARSTD::Command::PtrType CSWSetEngineIntensityCommand::copy() const
  {
    return ARSTD::Command::PtrType(new CSWSetEngineIntensityCommand(*this));
  }


  std::string CSWSetEngineIntensityCommand::getDetails() const
  {
    CSWEngine::PtrType pEngine = m_pEngine.lock();
    if (!pEngine)
      return "";
    std::stringstream ss;
    ss << "change intensity of " << pEngine->getName() << " to " << m_fIntensity;
    return ss.str();
  }


  CSWSetEngineIntensityCommand::~CSWSetEngineIntensityCommand()
  {
  }


  CSWSetEngineIntensityCommand::CSWSetEngineIntensityCommand(CSWEngine::PtrType pEngine, const double& fIntensity)
  : CSWExecuteCommand("SetEngineIntensity"),
    m_pEngine(pEngine),
    m_fIntensity(fIntensity)
  {
  }


  CSWSetEngineIntensityCommand::CSWSetEngineIntensityCommand(const CSWSetEngineIntensityCommand& other)
  : CSWExecuteCommand(other),
    m_pEngine(other.m_pEngine),
    m_fIntensity(other.m_fIntensity)
  {
  }


  void CSWSetEngineIntensityCommand::run()
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
      pEngine->setIntensity(m_fIntensity);
  }





  CSWSetEngineDirectionCommand::PtrType CSWSetEngineDirectionCommand::create(CSWEngine::PtrType& pEngine, const double& fHAngle, const double& fVAngle)
  {
    return PtrType(new CSWSetEngineDirectionCommand(pEngine, fHAngle, fVAngle));
  }


  CSWSetEngineDirectionCommand::PtrType CSWSetEngineDirectionCommand::create(CSWEngine::PtrType& pEngine, const Vector3D& vecDirection)
  {
    return PtrType(new CSWSetEngineDirectionCommand(pEngine, vecDirection));
  }


  ARSTD::Command::PtrType CSWSetEngineDirectionCommand::copy() const
  {
    return ARSTD::Command::PtrType(new CSWSetEngineDirectionCommand(*this));
  }


  std::string CSWSetEngineDirectionCommand::getDetails() const
  {
    CSWEngine::PtrType pEngine = m_pEngine.lock();
    if (!pEngine)
      return "";

    std::stringstream ss;
    ss << "change direction of " << pEngine->getName() << " to ";
    if (m_bUseAngles)
      ss << m_fHAngle << ", " << m_fVAngle << " degree";
    else
      ss << m_vecDirection;
    return ss.str();
  }


  CSWSetEngineDirectionCommand::~CSWSetEngineDirectionCommand()
  {
  }


  CSWSetEngineDirectionCommand::CSWSetEngineDirectionCommand(CSWEngine::PtrType pEngine, const double& fHAngle, const double& fVAngle)
  : CSWExecuteCommand("SetEngineDirection"),
    m_pEngine(pEngine),
    m_bUseAngles(true),
    m_fHAngle(fHAngle),
    m_fVAngle(fVAngle),
    m_vecDirection(0, 0, 1)
  {
  }


  CSWSetEngineDirectionCommand::CSWSetEngineDirectionCommand(CSWEngine::PtrType pEngine, const Vector3D& vecDirection)
  : CSWExecuteCommand("SetEngineDirection"),
    m_pEngine(pEngine),
    m_bUseAngles(false),
    m_fHAngle(0),
    m_fVAngle(0),
    m_vecDirection(vecDirection)
  {
  }


  CSWSetEngineDirectionCommand::CSWSetEngineDirectionCommand(const CSWSetEngineDirectionCommand& other)
  : CSWExecuteCommand(other),
    m_pEngine(other.m_pEngine),
    m_bUseAngles(other.m_bUseAngles),
    m_fHAngle(other.m_fHAngle),
    m_fVAngle(other.m_fVAngle),
    m_vecDirection(other.m_vecDirection)
  {
  }


  void CSWSetEngineDirectionCommand::run()
  {
    CSWEngine::PtrType pEngine = m_pEngine.lock();
    if (!pEngine)
      return;

    if (m_bUseAngles)
      pEngine->setDirection(m_fHAngle, m_fVAngle);
    else
      pEngine->setDirection(m_vecDirection);
  }





  CSWSendEventCommand::PtrType CSWSendEventCommand::create(CSWEvent::PtrType pEvent)
  {
    return PtrType(new CSWSendEventCommand(pEvent));
  }


  ARSTD::Command::PtrType CSWSendEventCommand::copy() const
  {
    return ARSTD::Command::PtrType(new CSWSendEventCommand(*this));
  }


  std::string CSWSendEventCommand::getDetails() const
  {
    std::stringstream ss;
    ss << "Sending event from ";
    if (m_pEvent->getSender())
      ss << m_pEvent->getSender()->getName();
    else
      ss << "unknown";
    ss << " to ";
    if (m_pEvent->getReceiver())
      ss << m_pEvent->getReceiver()->getName();
    else
      ss << "all";
    return ss.str();
  }


  CSWSendEventCommand::~CSWSendEventCommand()
  {
  }


  CSWSendEventCommand::CSWSendEventCommand(CSWEvent::PtrType pEvent)
  : CSWExecuteCommand("SendEvent"),
    m_pEvent(pEvent)
  {
    if (!pEvent)
      throw std::runtime_error("the given event is not defined");
  }


  CSWSendEventCommand::CSWSendEventCommand(const CSWSendEventCommand& other)
  : CSWExecuteCommand(other),
    m_pEvent(other.m_pEvent)
  {
  }


  void CSWSendEventCommand::run()
  {
    CSWEventManager::getInstance()->send(m_pEvent);
  }






  CSWFireCommand::PtrType CSWFireCommand::create(CSWWeaponBattery::PtrType& pWeaponBattery, 
                                                 double fArmDelay, 
                                                 CSWWeapon::PtrType& pWeapon)
  {
    return PtrType(new CSWFireCommand(pWeaponBattery, fArmDelay, pWeapon));
  }


  ARSTD::Command::PtrType CSWFireCommand::copy() const
  {
    return ARSTD::Command::PtrType(new CSWFireCommand(*this));
  }


  std::string CSWFireCommand::getDetails() const
  {
    std::stringstream ss;
    CSWWeaponBattery::PtrType pWeaponBattery = m_pWeaponBattery.lock();
    if (!pWeaponBattery)
      return "";
  
    CSWWeapon::PtrType pWeapon = m_pWeapon.lock();
    ss << "Fire weapon " << (!pWeapon ? "next available" : pWeapon->getName()) << " from " << pWeaponBattery->getName();
    return ss.str();
  }


  CSWFireCommand::~CSWFireCommand()
  {
  }


  CSWFireCommand::CSWFireCommand(CSWWeaponBattery::PtrType pWeaponBattery, double fArmDelay, CSWWeapon::PtrType pWeapon)
  : CSWExecuteCommand("FireWeapon"),
    m_pWeaponBattery(pWeaponBattery),
    m_fArmDelay(fArmDelay), 
    m_pWeapon(pWeapon)
  {
  }


  CSWFireCommand::CSWFireCommand(const CSWFireCommand& other)
  : CSWExecuteCommand(other),
    m_pWeaponBattery(other.m_pWeaponBattery),
    m_fArmDelay(other.m_fArmDelay),
    m_pWeapon(other.m_pWeapon)
  {
  }


  void CSWFireCommand::run()
  {
    CSWWeaponBattery::PtrType pWeaponBattery = m_pWeaponBattery.lock();
    if (!pWeaponBattery)
      return;
  
    if (CSWWeapon::PtrType pWeapon = m_pWeapon.lock()) 
    {
      pWeapon->arm(m_fArmDelay);
      pWeaponBattery->release(pWeapon);
    }
    else pWeaponBattery->releaseNext(m_fArmDelay);
  }







  CSWRechargeWeaponBatteryCommand::PtrType CSWRechargeWeaponBatteryCommand::create(CSWWeaponBattery::PtrType& pWeaponBattery, 
                                                                                   const std::string& strSupplyName,
                                                                                   unsigned long nResourceIDs)
  {
    return PtrType(new CSWRechargeWeaponBatteryCommand(pWeaponBattery, strSupplyName, nResourceIDs));
  }


  ARSTD::Command::PtrType CSWRechargeWeaponBatteryCommand::copy() const
  {
    return ARSTD::Command::PtrType(new CSWRechargeWeaponBatteryCommand(*this));
  }


  std::string CSWRechargeWeaponBatteryCommand::getDetails() const
  {
    CSWWeaponBattery::PtrType pWeaponBattery = m_pRechargeableWeaponBattery.lock();
    if (!pWeaponBattery)
      return "";
    std::stringstream ss;
    ss << "request recharge of " << m_nResourceIDs << " to " << pWeaponBattery->getName() << " at " << m_strSupplyName;
    return ss.str();
  }


  CSWRechargeWeaponBatteryCommand::~CSWRechargeWeaponBatteryCommand() 
  {
  }


  CSWRechargeWeaponBatteryCommand::CSWRechargeWeaponBatteryCommand(CSWWeaponBattery::PtrType pWeaponBattery, 
                                                                   const std::string& strSupplyName,
                                                                   unsigned long nResourceIDs)
  : CSWExecuteCommand("RechargeWeaponBattery"),
    m_pRechargeableWeaponBattery(pWeaponBattery),
    m_strSupplyName(strSupplyName),
    m_nResourceIDs(nResourceIDs)
  {
  }


  CSWRechargeWeaponBatteryCommand::CSWRechargeWeaponBatteryCommand(const CSWRechargeWeaponBatteryCommand& other)
  : CSWExecuteCommand(other),
    m_pRechargeableWeaponBattery(other.m_pRechargeableWeaponBattery),
    m_strSupplyName(other.m_strSupplyName),
    m_nResourceIDs(other.m_nResourceIDs)
  {
  }


  void CSWRechargeWeaponBatteryCommand::run()
  {
    if (CSWWeaponBattery::PtrType pWeaponBattery = m_pRechargeableWeaponBattery.lock()) 
    {
      pWeaponBattery->tryRecharging(m_strSupplyName, m_nResourceIDs);
    }
  }

}