// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMoveForwardCommand.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWGyroCompass.h"
#include "CSWGPS.h"


namespace CodeSubWars
{

  CSWMoveForwardCommand::PtrType CSWMoveForwardCommand::create(CSWControlCenter::PtrType& pControlCenter,
                                                               CSWEngine::PtrType& pEngine, 
                                                               double fSetRelativePosition, double fPositionTolerance,
                                                               double fEndVelocity, double fVelocityTolerance, 
                                                               int nProperty, double fMaxIntensity)
  {
    return PtrType(new CSWMoveForwardCommand(pControlCenter, pEngine, 
                                             fSetRelativePosition, fPositionTolerance, 
                                             fEndVelocity, fVelocityTolerance, 
                                             nProperty, fMaxIntensity));
  }


  CSWMoveForwardCommand::PtrType CSWMoveForwardCommand::create(CSWControlCenter::PtrType& pControlCenter,
                                                               CSWEngine::PtrType& pEngine, 
                                                               const Vector3D& vecPos, double fPositionTolerance,
                                                               double fEndVelocity, double fVelocityTolerance, 
                                                               int nProperty, double fMaxIntensity)
  {
    return PtrType(new CSWMoveForwardCommand(pControlCenter, pEngine, 
                                             vecPos, fPositionTolerance, 
                                             fEndVelocity, fVelocityTolerance, 
                                             nProperty, fMaxIntensity));
  }


  CSWMoveForwardCommand::~CSWMoveForwardCommand()
  {
  }


  ARSTD::Command::PtrType CSWMoveForwardCommand::copy() const
  {
    return PtrType(new CSWMoveForwardCommand(*this));
  }


  void CSWMoveForwardCommand::initialize()
  {
    m_bConditionReached = false;

    CSWEngine::PtrType pEngine = m_pEngine.lock();
    CSWControlCenter::PtrType pControlCenter = m_pControlCenter.lock();
    if (!pEngine || !pControlCenter)
      return;

    pEngine->setDirection(0, 0);
    m_fInitialVelocity = pControlCenter->getMovingPropertiesSensor()->getForwardVelocity();
  
    if (m_nProperties & POSITION) 
    {
      m_vecAbsolutFromPosition = pControlCenter->getGPS()->getPosition();
      if (m_bUsePosition)
      {
        m_fSetRelativePosition = (m_vecPos - pControlCenter->getGPS()->getPosition())*pControlCenter->getGyroCompass()->getForwardDirection();
      }
    
      m_PIDController.setup(m_fSetRelativePosition);
    }
    else if (m_nProperties & VELOCITY)
    {
      m_PIDController.setup(m_fEndVelocity); 
    }
  }


  void CSWMoveForwardCommand::step()
  {
    if (m_bConditionReached)
      return;

    CSWEngine::PtrType pEngine = m_pEngine.lock();
    CSWControlCenter::PtrType pControlCenter = m_pControlCenter.lock();
    if (!pEngine || !pControlCenter)
    {
      cleanup();
      setProgress(1);
      getCommandProcessor()->finished();
      return;
    }

    setProgress(0.5);  

    double fCurrentRelativePosition = (pControlCenter->getGPS()->getPosition() - m_vecAbsolutFromPosition)*pControlCenter->getGyroCompass()->getForwardDirection();
    double fCurrentVelocity = pControlCenter->getMovingPropertiesSensor()->getForwardVelocity();

    if (m_nProperties & POSITION)
      m_PIDController.setNewProcessValue(fCurrentRelativePosition);
    else if (m_nProperties & VELOCITY)
      m_PIDController.setNewProcessValue(fCurrentVelocity);

    double fNewIntensity;
    if (m_PIDController.getControlValue() > m_fMaxIntensity)
      fNewIntensity = m_fMaxIntensity;
    else if (m_PIDController.getControlValue() < -m_fMaxIntensity)
      fNewIntensity = -m_fMaxIntensity;
    else
      fNewIntensity = m_PIDController.getControlValue();

    if (!(m_nProperties & POSITION) && m_nProperties & VELOCITY)
    {
      //if only velocity must be reached
      fNewIntensity *= 1000;
    }
  
    pEngine->setIntensity(fNewIntensity);

    m_bConditionReached = true;
    if (m_nProperties & POSITION)
      m_bConditionReached &= fabs(m_fSetRelativePosition - fCurrentRelativePosition) < m_fPositionTolerance;
    if (m_nProperties & VELOCITY)
      m_bConditionReached &= fabs(m_fEndVelocity - fCurrentVelocity) < m_fVelocityTolerance;
    if (m_bConditionReached)
    {
      cleanup();
      setProgress(1);
      getCommandProcessor()->finished();
    }
  }


  void CSWMoveForwardCommand::cleanup()
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
      pEngine->setIntensity(0);
  }


  uint64 CSWMoveForwardCommand::getRessourceID() const
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
      return reinterpret_cast<uint64>(pEngine.get());
    return 0;
  }


  std::string CSWMoveForwardCommand::getName() const
  {
    return m_strName;
  }


  std::string CSWMoveForwardCommand::getDetails() const
  {
    std::stringstream ss;
    if (m_nProperties & POSITION) 
    {
      if (m_bUsePosition)
        ss << "Move Forward to " << m_vecPos << " m";
      else
        ss << "Move Forward about " << m_fSetRelativePosition << " m";
    }
    else if (m_nProperties & VELOCITY)
    {
      ss << "Move Forward to " << m_fEndVelocity << " degree/s";
    }
    return ss.str();
  }


  CSWMoveForwardCommand::CSWMoveForwardCommand(CSWControlCenter::PtrType pControlCenter,
                                               CSWEngine::PtrType pEngine, 
                                               double fSetRelativePosition, double fPositionTolerance,
                                               double fEndVelocity, double fVelocityTolerance, 
                                               int nProperty, double fMaxIntensity)
  : m_strName("MoveForward"),
    m_PIDController(500.0/10000, 0.0/10000, 0.0/10000),
    m_pControlCenter(pControlCenter),
    m_pEngine(pEngine),
    m_fSetRelativePosition(fSetRelativePosition),
    m_fPositionTolerance(fPositionTolerance),
    m_vecAbsolutFromPosition(pControlCenter->getGPS()->getPosition()),
    m_fEndVelocity(fEndVelocity),
    m_fVelocityTolerance(fVelocityTolerance),
    m_fMaxIntensity(fMaxIntensity),
    m_nProperties(nProperty),
    m_bConditionReached(false),
    m_bUsePosition(false),
    m_vecPos(0, 0, 0)
  {
  }


  CSWMoveForwardCommand::CSWMoveForwardCommand(CSWControlCenter::PtrType pControlCenter,
                                               CSWEngine::PtrType pEngine, 
                                               const Vector3D& vecPos, double fPositionTolerance,
                                               double fEndVelocity, double fVelocityTolerance, 
                                               int nProperty, double fMaxIntensity)
  : m_strName("MoveForward"),
    m_PIDController(500.0/10000, 0.0/10000, 0.0/10000),
    m_pControlCenter(pControlCenter),
    m_pEngine(pEngine),
    m_fSetRelativePosition(0),
    m_fPositionTolerance(fPositionTolerance),
    m_vecAbsolutFromPosition(pControlCenter->getGPS()->getPosition()),
    m_fEndVelocity(fEndVelocity),
    m_fVelocityTolerance(fVelocityTolerance),
    m_fMaxIntensity(fMaxIntensity),
    m_nProperties(nProperty),
    m_bConditionReached(false),
    m_vecPos(vecPos),
    m_bUsePosition(true)
  {
  }


  CSWMoveForwardCommand::CSWMoveForwardCommand(const CSWMoveForwardCommand& other)
  : ARSTD::Command(other),
    m_strName(other.m_strName),
    m_PIDController(other.m_PIDController),
    m_pControlCenter(other.m_pControlCenter),
    m_pEngine(other.m_pEngine),
    m_fSetRelativePosition(other.m_fSetRelativePosition),
    m_fPositionTolerance(other.m_fPositionTolerance),
    m_vecAbsolutFromPosition(other.m_vecAbsolutFromPosition),
    m_fInitialVelocity(other.m_fInitialVelocity),
    m_fEndVelocity(other.m_fEndVelocity),
    m_fVelocityTolerance(other.m_fVelocityTolerance),
    m_fMaxIntensity(other.m_fMaxIntensity),
    m_nProperties(other.m_nProperties),
    m_bConditionReached(other.m_bConditionReached),
    m_bUsePosition(other.m_bUsePosition),
    m_vecPos(other.m_vecPos)
  {
  }

}