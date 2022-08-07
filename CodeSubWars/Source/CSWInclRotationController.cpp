// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWUtilities.h"
#include "CSWInclRotationController.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"
#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"


namespace CodeSubWars
{

  CSWInclRotationController::PtrType CSWInclRotationController::create(CSWControlCenter::PtrType pControlCenter,
                                                                       CSWEngine::PtrType pEngine, 
                                                                       double fEndDirection, double fDirectionTolerance,
                                                                       double fEndVelocity, double fVelocityTolerance, 
                                                                       int nProperties, 
                                                                       double fMaxIntensity)
  {
    return PtrType(new CSWInclRotationController(pControlCenter, pEngine, 
                                                 fEndDirection, fDirectionTolerance, 
                                                 fEndVelocity, fVelocityTolerance,
                                                 nProperties, 
                                                 fMaxIntensity));
  }


  CSWInclRotationController::~CSWInclRotationController()
  {
  }

    
  void CSWInclRotationController::initialize()
  {
    m_fProgress = 0;
    m_bConditionReached = false;
  
    //store intial orientation that is used as reference to calculate angles about x,y,z
    m_matInitialOrientationTWorld = m_pControlCenter->getGyroCompass()->getInvertWorldTransform().getRotationAsMatrix33();
    Vector3D vecAngles = calcCurrentAngles();
    assert(fabs(vecAngles.x) < EPSILON && fabs(vecAngles.y) < EPSILON && fabs(vecAngles.z) < EPSILON);

    m_fInitialVelocity = m_pControlCenter->getMovingPropertiesSensor()->getUpAngularVelocity();  
  
    if (m_nProperties & DIRECTION) 
    { 
      m_fAbsDestDirection = vecAngles.x + m_fEndDirection;
      if (m_fAbsDestDirection < -90)
        m_fAbsDestDirection = -90;
      else if (m_fAbsDestDirection > 90)
        m_fAbsDestDirection = 90;
      m_PIDController.setup(m_fAbsDestDirection);
    }
    else if (m_nProperties & VELOCITY)
    {
      //for reaching a given velocity there is no need to control something. just raise the engine and wait till new velocity reached.
      if (m_fEndVelocity - m_fInitialVelocity < -EPSILON)
        m_PIDController.setup(100); 
      else if (m_fEndVelocity - m_fInitialVelocity > EPSILON)
        m_PIDController.setup(-100); 
    }
  }


  void CSWInclRotationController::update()
  {
    if (m_bConditionReached)
      return;
  
    m_fProgress = 0.5;

    Vector3D vecAngles = calcCurrentAngles();

    double fAbsCurrentDirection = vecAngles.x;
    
    double fCurrentVelocity = m_pControlCenter->getMovingPropertiesSensor()->getUpAngularVelocity();
    if (m_nProperties & VELOCITY_RELATIVE)
      fCurrentVelocity -= m_fInitialVelocity;


    if (m_nProperties & DIRECTION)
      m_PIDController.setNewProcessValue(fAbsCurrentDirection);
    else if (m_nProperties & VELOCITY)
      m_PIDController.setNewProcessValue(fCurrentVelocity);


    if (m_PIDController.getControlValue() > m_fMaxIntensity)
      m_pEngine->setIntensity(m_fMaxIntensity);
    else if (m_PIDController.getControlValue() < -m_fMaxIntensity)
      m_pEngine->setIntensity(-m_fMaxIntensity);
    else
      m_pEngine->setIntensity(m_PIDController.getControlValue());


    m_bConditionReached = true;
    if (m_nProperties & DIRECTION)
      m_bConditionReached &= fabs(m_fAbsDestDirection - fAbsCurrentDirection) < m_fDirectionTolerance;
    if (m_nProperties & VELOCITY)
      m_bConditionReached &= fabs(m_fEndVelocity - fCurrentVelocity) < m_fVelocityTolerance;
    if (m_bConditionReached)
    {
      m_fProgress = 1.0;
    }
  }


  void CSWInclRotationController::finalize()
  {
    m_pEngine->setIntensity(0);
  }


  bool CSWInclRotationController::hasFinished()
  {
    return m_bConditionReached;
  }


  const std::string& CSWInclRotationController::getName() const
  {
    return m_strName;
  }


  std::string CSWInclRotationController::getDetails() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    if (m_nProperties & DIRECTION) 
    {
      ss << "Relative Inclination Rotation about " << m_fEndDirection << " degree";
    }
    else if (m_nProperties & VELOCITY)
    {
      if (m_nProperties & VELOCITY_RELATIVE)
      {
        ss << "Relative Inclination Rotation from " << m_fInitialVelocity 
           << " degree/s to " << m_fInitialVelocity + m_fEndVelocity << " degree/s";
      }
      else
      {
        ss << "Relative Inclination Rotation to " << m_fEndVelocity << " degree/s";
      }
    }
    return ss.str();
  }


  double CSWInclRotationController::getProgress() const
  {
    return m_fProgress;
  }


  CSWInclRotationController::CSWInclRotationController(CSWControlCenter::PtrType pControlCenter,
                                                       CSWEngine::PtrType pEngine, 
                                                       double fEndDirection, double fDirectionTolerance,
                                                       double fEndVelocity, double fVelocityTolerance, 
                                                       int nProperties, 
                                                       double fMaxIntensity)
  : m_strName("InclinationRotationController"),
    m_nProperties(nProperties),
    m_bConditionReached(false),
    m_PIDController(500.0/10000, 0.0/10000, 0.0/10000),
    m_pControlCenter(pControlCenter),
    m_pEngine(pEngine),
    m_fEndDirection(fEndDirection),
    m_fDirectionTolerance(fDirectionTolerance),
    m_fInitialVelocity(0),
    m_fEndVelocity(fEndVelocity),
    m_fVelocityTolerance(fVelocityTolerance),
    m_fMaxIntensity(fMaxIntensity),
    m_fProgress(0)
  {
  }


  Vector3D CSWInclRotationController::calcCurrentAngles()
  {
    Matrix33D matInitialOrientationTCurrentOrientation = m_matInitialOrientationTWorld*m_pControlCenter->getGyroCompass()->getOrientation();
    return CSWUtilities::determineAngles(matInitialOrientationTCurrentOrientation);
  }

}