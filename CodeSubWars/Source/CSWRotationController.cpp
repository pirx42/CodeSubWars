// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWUtilities.h"
#include "CSWRotationController.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"
#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"


namespace CodeSubWars
{

  CSWRotationController::PtrType CSWRotationController::create(CSWControlCenter::PtrType pControlCenter,
                                                               CSWEngine::PtrType pEngine, 
                                                               double fEndDirection, double fDirectionTolerance,
                                                               double fEndVelocity, double fVelocityTolerance, 
                                                               int nProperties, 
                                                               double fMaxIntensity)
  {
    return PtrType(new CSWRotationController(pControlCenter, pEngine, 
                                             fEndDirection, fDirectionTolerance, 
                                             fEndVelocity, fVelocityTolerance,
                                             nProperties, 
                                             fMaxIntensity));
  }


  CSWRotationController::~CSWRotationController()
  {
  }

    
  void CSWRotationController::initialize()
  {
    m_fProgress = 0;
    m_bConditionReached = false;
    m_nNumRotationOffset = 0;
  
    //store intial orientation that is used as reference to calculate angles about x,y,z
    m_matInitialOrientationTWorld = m_pControlCenter->getGyroCompass()->getInvertWorldTransform().getRotationAsMatrix33();
    Vector3D vecAngles = calcCurrentAngles();
    assert(fabs(vecAngles.x) < EPSILON && fabs(vecAngles.y) < EPSILON && fabs(vecAngles.z) < EPSILON);
  
    m_fInitialVelocity = m_pControlCenter->getMovingPropertiesSensor()->getLeftAngularVelocity();
  
    if (m_nProperties & DIRECTION) 
    {
      m_fAbsDestDirection = m_nNumRotationOffset*360 + vecAngles.y + m_fEndDirection;
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


  void CSWRotationController::update()
  {
    if (m_bConditionReached)
      return;
  
    m_fProgress = 0.5;
  
    Vector3D vecAngles = calcCurrentAngles();

    //determine rotation cycles. assumes that in one step a direction change is below 10 degree!
    static double fLastDirection = vecAngles.y;
    if (fLastDirection - vecAngles.y > 350)
      ++m_nNumRotationOffset; //cw
    else if (fLastDirection - vecAngles.y < -350)
      --m_nNumRotationOffset; //ccw
    fLastDirection = vecAngles.y;


    double fAbsCurrentDirection = m_nNumRotationOffset*360 + vecAngles.y;
    
    double fCurrentVelocity = m_pControlCenter->getMovingPropertiesSensor()->getLeftAngularVelocity();
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


  void CSWRotationController::finalize()
  {
    m_pEngine->setIntensity(0);
  }


  bool CSWRotationController::hasFinished()
  {
    return m_bConditionReached;
  }


  const std::string& CSWRotationController::getName() const
  {
    return m_strName;
  }


  std::string CSWRotationController::getDetails() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    if (m_nProperties & DIRECTION) 
    {
      ss << "Relative Rotation about " << m_fEndDirection << " degree";
    }
    else if (m_nProperties & VELOCITY)
    {
      if (m_nProperties & VELOCITY_RELATIVE)
      {
        ss << "Relative Rotation from " << m_fInitialVelocity 
           << " degree/s to " << m_fInitialVelocity + m_fEndVelocity << " degree/s";
      }
      else
      {
        ss << "Relative Rotation to " << m_fEndVelocity << " degree/s";
      }
    }
    return ss.str();
  }


  double CSWRotationController::getProgress() const
  {
    return m_fProgress;
  }


  CSWRotationController::CSWRotationController(CSWControlCenter::PtrType pControlCenter,
                                               CSWEngine::PtrType pEngine, 
                                               double fEndDirection, double fDirectionTolerance,
                                               double fEndVelocity, double fVelocityTolerance, 
                                               int nProperties, 
                                               double fMaxIntensity)
  : m_strName("RotationController"),
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
    m_fProgress(0),
    m_nNumRotationOffset(0)
  {
  }


  Vector3D CSWRotationController::calcCurrentAngles()
  {
    Matrix33D matInitialOrientationTCurrentOrientation = m_matInitialOrientationTWorld*m_pControlCenter->getGyroCompass()->getOrientation();
    return CSWUtilities::determineAngles(matInitialOrientationTCurrentOrientation);
  }

}