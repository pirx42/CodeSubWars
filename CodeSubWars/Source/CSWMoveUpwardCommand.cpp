// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMoveUpwardCommand.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWGPS.h"
#include "CSWGyroCompass.h"


namespace CodeSubWars
{

  CSWMoveUpwardCommand::PtrType CSWMoveUpwardCommand::create(CSWControlCenter::PtrType& pControlCenter,
                                                             CSWEngine::PtrType& pEngine, 
                                                             double fSetRelativePosition, double fPositionTolerance,
                                                             double fEndVelocity, double fVelocityTolerance, 
                                                             int nProperty, double fMaxIntensity)
  {
    return PtrType(new CSWMoveUpwardCommand(pControlCenter, pEngine, 
                                            fSetRelativePosition, fPositionTolerance, 
                                            fEndVelocity, fVelocityTolerance, 
                                            nProperty, fMaxIntensity));
  }


  CSWMoveUpwardCommand::PtrType CSWMoveUpwardCommand::create(CSWControlCenter::PtrType& pControlCenter,
                                                             CSWEngine::PtrType& pEngine, 
                                                             const Vector3D& vecPos, double fPositionTolerance,
                                                             double fEndVelocity, double fVelocityTolerance, 
                                                             int nProperty, double fMaxIntensity)
  {
    return PtrType(new CSWMoveUpwardCommand(pControlCenter, pEngine, 
                                            vecPos, fPositionTolerance, 
                                            fEndVelocity, fVelocityTolerance, 
                                            nProperty, fMaxIntensity));
  }


  CSWMoveUpwardCommand::~CSWMoveUpwardCommand()
  {
  }


  ARSTD::Command::PtrType CSWMoveUpwardCommand::copy() const
  {
    return PtrType(new CSWMoveUpwardCommand(*this));
  }


  void CSWMoveUpwardCommand::initialize()
  {
    m_bConditionReached = false;

    CSWEngine::PtrType pEngine = m_pEngine.lock();
    CSWControlCenter::PtrType pControlCenter = m_pControlCenter.lock();
    if (!pEngine || !pControlCenter)
      return;

    pEngine->setDirection(0, 0);
    m_fInitialVelocity = pControlCenter->getMovingPropertiesSensor()->getUpVelocity();
  
    if (m_nProperties & POSITION) 
    {
      m_vecAbsolutFromPosition = pControlCenter->getGPS()->getPosition();
      if (m_bUsePosition)
      {
        m_fSetRelativePosition = (m_vecPos - pControlCenter->getGPS()->getPosition())*pControlCenter->getGyroCompass()->getUpDirection();
      }
    
      m_PIDController.setup(m_fSetRelativePosition);
    }
    else if (m_nProperties & VELOCITY)
    {
      m_PIDController.setup(m_fEndVelocity); 
    }
  }


  void CSWMoveUpwardCommand::step()
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

    double fCurrentRelativePosition = (pControlCenter->getGPS()->getPosition() - m_vecAbsolutFromPosition)*pControlCenter->getGyroCompass()->getUpDirection();
    double fCurrentVelocity = pControlCenter->getMovingPropertiesSensor()->getUpVelocity();

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


  void CSWMoveUpwardCommand::cleanup()
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
      pEngine->setIntensity(0);
  }


  uint64 CSWMoveUpwardCommand::getRessourceID() const
  {
    if (CSWEngine::PtrType pEngine = m_pEngine.lock())
      return reinterpret_cast<uint64>(pEngine.get());
    return 0;
  }


  std::string CSWMoveUpwardCommand::getName() const
  {
    return m_strName;
  }


  std::string CSWMoveUpwardCommand::getDetails() const
  {
    std::stringstream ss;
    if (m_nProperties & POSITION) 
    {
      if (m_bUsePosition)
        ss << "Move Upward to " << m_vecPos << " m";
      else
        ss << "Move Upward about " << m_fSetRelativePosition << " m";
    }
    else if (m_nProperties & VELOCITY)
    {
      ss << "Move Upward to " << m_fEndVelocity << " degree/s";
    }
    return ss.str();
  }


  CSWMoveUpwardCommand::CSWMoveUpwardCommand(CSWControlCenter::PtrType pControlCenter,
                                               CSWEngine::PtrType pEngine, 
                                               double fSetRelativePosition, double fPositionTolerance,
                                               double fEndVelocity, double fVelocityTolerance, 
                                               int nProperty, double fMaxIntensity)
  : m_strName("MoveUpward"),
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


  CSWMoveUpwardCommand::CSWMoveUpwardCommand(CSWControlCenter::PtrType pControlCenter,
                                               CSWEngine::PtrType pEngine, 
                                               const Vector3D& vecPos, double fPositionTolerance,
                                               double fEndVelocity, double fVelocityTolerance, 
                                               int nProperty, double fMaxIntensity)
  : m_strName("MoveUpward"),
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


  CSWMoveUpwardCommand::CSWMoveUpwardCommand(const CSWMoveUpwardCommand& other)
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