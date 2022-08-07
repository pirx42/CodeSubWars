// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWUtilities.h"
#include "CSWAbsRotController.h"
#include "CSWControlCenter.h"
#include "CSWEngine.h"
#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWGPS.h"


namespace CodeSubWars
{

  CSWAbsRotController::PtrType CSWAbsRotController::create(CSWControlCenter::PtrType pControlCenter,
                                                           const EngineContainer& engines,
                                                           const Vector3D& vecGPSTCenterOfMass,
                                                           const Vector3D& vecParameter, double fDirectionTolerance,
                                                           double fEndVelocity, double fVelocityTolerance, 
                                                           int nProperties, 
                                                           double fMaxIntensity)
  {
    return PtrType(new CSWAbsRotController(pControlCenter, engines, 
                                           vecGPSTCenterOfMass,
                                           vecParameter, fDirectionTolerance, 
                                           fEndVelocity, fVelocityTolerance,
                                           nProperties, 
                                           fMaxIntensity));
  }


  CSWAbsRotController::~CSWAbsRotController()
  {
  }

    
  void CSWAbsRotController::initialize()
  {
    m_fProgress = 0;
    m_bConditionReached = false;

    determineRotationDestDirection(true);
    assert(fabs(m_vecDir.getLength() - 1.0) < EPSILON);

    double fAngleToRotationDest = ARSTD::getInDegree(m_pControlCenter->getGyroCompass()->getForwardDirection().getAngleBetween(m_vecDir));
    if (fAngleToRotationDest < m_fDirectionTolerance)
    {
      //direction already reached -> all done
      m_bConditionReached = true;
      m_fProgress = 1.0;
      return;
    }

    if (fAngleToRotationDest < EPSILON ||
        m_pControlCenter->getGyroCompass()->getForwardDirection() == -m_vecDir)
    {
      m_vecWorldTRotationAxis = m_vecDir.getPerpendicular();
    }
    else
    {
      m_vecWorldTRotationAxis = m_pControlCenter->getGyroCompass()->getForwardDirection() % m_vecDir;
    }
    m_vecWorldTRotationAxis.normalize();


    m_fInitialVelocity = m_pControlCenter->getMovingPropertiesSensor()->getFrontAngularVelocity().getLength();

    if (m_nProperties & CONTROL_DIRECTION) 
    {
      m_PIDController.setup(0);
    }
    else if (m_nProperties & CONTROL_VELOCITY)
    {
      //for reaching a given velocity there is no need to control something. just raise the engine and wait till new velocity reached.
      if (m_fEndVelocity - m_fInitialVelocity < -EPSILON)
        m_PIDController.setup(100); 
      else if (m_fEndVelocity - m_fInitialVelocity > EPSILON)
        m_PIDController.setup(-100); 
    }
  }


  void CSWAbsRotController::update()
  {
    if (m_bConditionReached)
      return;
    
    m_fProgress = 0.5;

    if (m_nProperties & INTERPRET_AS_ABSOLUTE)
    {
      //only in absolute mode recalculate destination direction
      determineRotationDestDirection(false);
      assert(fabs(m_vecDir.getLength() - 1.0) < EPSILON);
    }

    double fAngleToRotationDest = ARSTD::getInDegree(m_pControlCenter->getGyroCompass()->getForwardDirection().getAngleBetween(m_vecDir));

  //die fordere geschwindigkeit iregendwie fuer die berechnung mit nutzen
  //  Vector3 vecCurrentVelocity = m_pControlCenter->getMovingPropertiesSensor()->getFrontAngularVelocity();

    if (fAngleToRotationDest > EPSILON && fAngleToRotationDest < 170)
    {
      //world rotation axis will only be recalculated if current axis and destination 
      //axis is not nearly parallel
      m_vecWorldTRotationAxis = m_pControlCenter->getGyroCompass()->getForwardDirection() % m_vecDir;
      m_vecWorldTRotationAxis.normalize();
    }

    double fCurrentVelocity = m_pControlCenter->getMovingPropertiesSensor()->getFrontAngularVelocity().getLength();
    if (m_nProperties & CONTROL_VELOCITY_RELATIVE)
      fCurrentVelocity -= m_fInitialVelocity;


    if (m_nProperties & CONTROL_DIRECTION)
      m_PIDController.setNewProcessValue(fAngleToRotationDest*5);//*500);
    else if (m_nProperties & CONTROL_VELOCITY)
      m_PIDController.setNewProcessValue(fCurrentVelocity);


    double fNewIntensity = m_PIDController.getControlValue();

    if (!(m_nProperties & CONTROL_VELOCITY))
    {
      //if velocity is not important try to reach destination as soon as possible (raise intensity)
      fNewIntensity *= 10;//10000;
    }

    Vector3D vecWorldTSubmarineCenterOfMass = m_pControlCenter->getGPS()->getWorldTransform()*m_vecGPSTCenterOfMass;

    //determine which engines can be used for that rotation and calculate the intensities
    int nNumUseableEngines = 0;
    std::vector<double> engineIntensities;
    double fMaxEngineIntensity = -std::numeric_limits<double>::max();
    PlaneD rotationPlane(Vector3D(0, 0, 0), m_vecWorldTRotationAxis);
    EngineContainer::const_iterator itEngine = m_Engines.begin();
    for (; itEngine != m_Engines.end(); ++itEngine)
    {
      Vector3D vecWorldTEngineDirection = (*itEngine)->getDirection();
      Vector3D vecWorldTCMEngine = (*itEngine)->getWorldTransform().getTranslation() - vecWorldTSubmarineCenterOfMass;

      Vector3D vecPlaneTEngineDirection = rotationPlane.projectPoint(vecWorldTEngineDirection);  
      Vector3D vecPlaneTCMEngine = rotationPlane.projectPoint(vecWorldTCMEngine);
      
      //engine force direction must not be parallel to rotation axis
      //engine must not be lying on the rotation axis
      //engine force direction must not be parallel to direction built from engine position to center of mass 
      if (vecPlaneTEngineDirection != Vector3D(0, 0, 0) &&
          vecPlaneTCMEngine != Vector3D(0, 0, 0) &&
          (vecPlaneTEngineDirection % vecPlaneTCMEngine) != Vector3D(0, 0, 0))
      {
        //determine the intensity for this engine
        Vector3D vecWorldTRotationDirection = vecWorldTCMEngine % m_vecWorldTRotationAxis;
        vecWorldTRotationDirection.normalize();
      
        double fEngineIntensity = (vecWorldTEngineDirection * vecWorldTRotationDirection) * fNewIntensity;
        engineIntensities.push_back(fEngineIntensity);
        if (fMaxEngineIntensity < fabs(fEngineIntensity))
        {
          fMaxEngineIntensity = fabs(fEngineIntensity);
        }
        ++nNumUseableEngines;
      }
      else
      {
        //engine could currently not be used for rotation -> disable it
        engineIntensities.push_back(0);
      }
    }

    if (!nNumUseableEngines)
    {
      // no engine can be used for that rotation -> done
      m_bConditionReached = true;
      m_fProgress = 1.0;
      return;
    }


    //normalize intensities so that no engine exceed the maximum intensity
    itEngine = m_Engines.begin();
    std::vector<double>::const_iterator itIntensity = engineIntensities.begin();
    for (; itEngine != m_Engines.end() && itIntensity != engineIntensities.end(); ++itEngine, ++itIntensity)
    {
      if (fMaxEngineIntensity > 1)
        (*itEngine)->setIntensity(*itIntensity/fMaxEngineIntensity);
      else
        (*itEngine)->setIntensity(*itIntensity);
    }

    bool bTmpConditionReached = true;
    if (m_nProperties & CONTROL_DIRECTION)
      bTmpConditionReached &= fAngleToRotationDest < m_fDirectionTolerance;
    if (m_nProperties & CONTROL_VELOCITY)
      bTmpConditionReached &= fabs(m_fEndVelocity - fCurrentVelocity) < m_fVelocityTolerance;

    m_bConditionReached = bTmpConditionReached;
    if (m_bConditionReached)
    {
      m_fProgress = 1.0;
    }
  }


  void CSWAbsRotController::finalize()
  {
    EngineContainer::const_iterator itEngine = m_Engines.begin();
    for (; itEngine != m_Engines.end(); ++itEngine)
    {
      (*itEngine)->setIntensity(0);
    }  
  }


  bool CSWAbsRotController::hasFinished()
  {
    return m_bConditionReached;
  }


  const std::string& CSWAbsRotController::getName() const
  {
    return m_strName;
  }


  std::string CSWAbsRotController::getDetails() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    if (m_nProperties & CONTROL_DIRECTION) 
    {
      ss << "Absolute Rotation ";
      if (m_nProperties & INTERPRET_AS_POSITION)
      {
        ss << "toward absolute position " << m_vecParameter << " m";
      } 
      else if (m_nProperties & INTERPRET_AS_DIRECTION)
      {
        ss << "toward absolute direction ";
        if (m_nProperties & INTERPRET_AS_ABSOLUTE)
        {
          ss << m_vecParameter << " m";
        }
        else
        {
          ss << m_vecDir << " m";
        }
      }
      ss << " (" << ARSTD::getInDegree(m_pControlCenter->getGyroCompass()->getForwardDirection().getAngleBetween(m_vecDir)) << " degree to destination)";
    }

    if ((m_nProperties & CONTROL_DIRECTION) && (m_nProperties & CONTROL_VELOCITY))
    {
      ss << " and ";
    }
  
    if (m_nProperties & CONTROL_VELOCITY)
    {
      if (m_nProperties & CONTROL_VELOCITY_RELATIVE)
      {
        ss << "Absolute Rotation from " << m_fInitialVelocity 
           << " degree/s to " << m_fInitialVelocity + m_fEndVelocity << " degree/s";
      }
      else
      {
        ss << "Absolute Rotation to " << m_fEndVelocity << " degree/s";
      }
    }
    return ss.str();
  }


  double CSWAbsRotController::getProgress() const
  {
    return m_fProgress;
  }


  CSWAbsRotController::CSWAbsRotController(CSWControlCenter::PtrType pControlCenter,
                                           const EngineContainer& engines, 
                                           const Vector3D& vecGPSTCenterOfMass,
                                           const Vector3D& vecParameter, double fDirectionTolerance,
                                           double fEndVelocity, double fVelocityTolerance, 
                                           int nProperties, 
                                           double fMaxIntensity)
  : m_strName("AbsoluteRotationController"),
    m_nProperties(nProperties),
    m_bConditionReached(false),
    m_PIDController(500.0/10000, 0.0/10000, 0.0/10000),
    m_pControlCenter(pControlCenter),
    m_vecParameter(vecParameter),
    m_fDirectionTolerance(fDirectionTolerance),
    m_fInitialVelocity(0),
    m_fEndVelocity(fEndVelocity),
    m_fVelocityTolerance(fVelocityTolerance),
    m_fMaxIntensity(fMaxIntensity),
    m_fProgress(0),
    m_Engines(engines),
    m_vecGPSTCenterOfMass(vecGPSTCenterOfMass),
    m_vecWorldTRotationAxis(1, 0, 0),
    m_vecDir(0, 0, 1)
  {
    assert(!m_Engines.empty());
    assert(fMaxIntensity >= -1.0 && fMaxIntensity <= 1.0);
  }


  void CSWAbsRotController::determineRotationDestDirection(bool bInitialize)
  {
    //determine the absolute destination direction from the input parameter
  
    if (m_nProperties & INTERPRET_AS_DIRECTION)
    {
      if (m_nProperties & INTERPRET_AS_RELATIVE)
      {
        m_vecParameter = m_pControlCenter->getGPS()->getWorldTransform().getRotationAsMatrix33()*m_vecParameter;
      }
      m_vecDir = m_vecParameter.getNormalized();
    }
    else if (m_nProperties & INTERPRET_AS_POSITION)
    {
      if (m_nProperties & INTERPRET_AS_RELATIVE)
      {
        m_vecParameter = m_pControlCenter->getGPS()->getWorldTransform()*m_vecParameter;
      }
    
      double fDist = (m_vecParameter - m_pControlCenter->getGPS()->getPosition()).getLength();
      if (fDist < 0.2)
      {
        //destination position nearly reached -> done
        m_bConditionReached = true;
        m_fProgress = 1.0;
        m_vecDir = Vector3D(0, 0, 1);
        return;
      }
    
      //the rotation center is mostly not the center position of an object! 
      //if the destination position is within a sphere centered at rotation center with radius of 
      //rotation center minus object center, the rotation will probably not termiate. this is because 
      //the determined destination direction points alway to the back side.
      //to prevent this problem the destination direction to those close destination positions is
      //only determined once at initialization.
      if (fDist > 150 || bInitialize)
      {
        m_vecDir = (m_vecParameter - m_pControlCenter->getGPS()->getPosition()).getNormalized();    
      }
    }
    else if (m_nProperties & INTERPRET_AS_DIRECTION_ANGLES)
    {
  assert(false && "currently not implemented!");  
      if (m_nProperties & INTERPRET_AS_RELATIVE)
      {
        m_vecParameter = m_pControlCenter->getGPS()->getWorldTransform()*m_vecParameter;
      }
      m_vecDir = Vector3D(0, 0, 1);
    }
  
  }

}