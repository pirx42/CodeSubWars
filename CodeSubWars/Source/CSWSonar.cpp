// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSonar.h"
#include "CSWMap.h"


namespace CodeSubWars
{

  const double CSWSonar::ANGLULAR_VELOCITY = getInRad(180);

  //horizontal angle is 90 degree
  const double CSWSonar::HORIZONTAL_SCANNING_ANGLE = getInRad(45);
  //horizontal rotation velocity is 90 degree per second
  const double CSWSonar::HORIZONTAL_ANGULAR_VELOCITY = getInRad(90);


  CSWSonar::~CSWSonar()
  {
  }


  void CSWSonar::update()
  {
    double fHorizontalAngularVelocity = 0;
    double fHorizontalScanningAngle = 0;
    double fCurrentTime = ARSTD::Time::getTime();
    double fElapsedTime = fCurrentTime - m_fOldTime;
    m_fOldTime = fCurrentTime;

    if (m_bAutomaticRotationEnabled || (m_ScanDirectionMode != LOCAL_DIRECTION && m_ScanDirectionMode != GLOBAL_POSITION))
    {
      if (m_ScanVelocityMode == FAST)
      {
        fHorizontalAngularVelocity = HORIZONTAL_ANGULAR_VELOCITY;
      }
      else if (m_ScanVelocityMode == SLOW)
      {
        fHorizontalAngularVelocity = HORIZONTAL_ANGULAR_VELOCITY/4.5;
      }

      if (m_ScanRangeMode == NEAR_RANGE)
      {
        fHorizontalScanningAngle = HORIZONTAL_SCANNING_ANGLE;
      }
      else if (m_ScanRangeMode == FAR_RANGE)
      {
        fHorizontalScanningAngle = HORIZONTAL_SCANNING_ANGLE/4;
      }
    }

    if (m_bAutomaticRotationEnabled && m_ScanDirectionMode != LOCAL_DIRECTION && m_ScanDirectionMode != GLOBAL_POSITION)
    {
      //accumulate the active time period
      m_fActiveAutomaticRotationTimePeriod += fElapsedTime;
    }

    Vector3D vecNewDirection(1, 0, 0);
    if (m_ScanDirectionMode == FULL)
    {
      vecNewDirection = m_vecObjectTInitialDirection.getRotated(m_vecObjectTInitialUp, fHorizontalAngularVelocity*m_fActiveAutomaticRotationTimePeriod);
      vecNewDirection = getAdditionalDirection(vecNewDirection, m_vecObjectTInitialUp);
    }
    else if (m_ScanDirectionMode == FRONT)
    {
      vecNewDirection = m_vecObjectTInitialDirection.getRotated(m_vecObjectTInitialUp, fHorizontalScanningAngle*sin(2*fHorizontalAngularVelocity*m_fActiveAutomaticRotationTimePeriod));
      vecNewDirection = getAdditionalDirection(vecNewDirection, m_vecObjectTInitialUp);
    }
    else if (m_ScanDirectionMode == BACK)
    {
      vecNewDirection = m_vecObjectTInitialDirection.getRotated(m_vecObjectTInitialUp, ARSTD::PI + fHorizontalScanningAngle*sin(2*fHorizontalAngularVelocity*m_fActiveAutomaticRotationTimePeriod));
      vecNewDirection = getAdditionalDirection(vecNewDirection, m_vecObjectTInitialUp);
    }
    else if (m_ScanDirectionMode == LOCAL_DIRECTION)
    {
      vecNewDirection = m_vecMainScanDirection.getRotated(m_vecMainScanUp, fHorizontalScanningAngle/3*sin(2*fHorizontalAngularVelocity*1.5*fCurrentTime));
      vecNewDirection = getAdditionalDirection(vecNewDirection, m_vecMainScanUp);
    }
    else if (m_ScanDirectionMode == GLOBAL_POSITION)
    {
      Vector3D vecDir = m_vecWorldTPointToPosition - getWorldTransform().getTranslation();
      if (vecDir == Vector3D::ZERO)
        vecDir = Vector3D(0, 0, 1);
      else
        vecDir.normalize();
    
      Vector3D vecUp;
      if (vecDir == Vector3D(0, 1, 0) || vecDir == Vector3D(0, -1, 0))
        vecUp = vecDir.getPerpendicular().getNormalized();
      else
        vecUp = Vector3D(0, 1, 0);

      vecNewDirection = vecDir.getRotated(vecUp, fHorizontalScanningAngle/3*sin(2*fHorizontalAngularVelocity*1.5*fCurrentTime));
      vecNewDirection = getAdditionalDirection(vecNewDirection, vecUp);

      assert(fabs(vecNewDirection.getLength() - 1) < EPSILON);
      setWorldDirection(vecNewDirection);
      CSWSensor::update();
      return;
    }

    assert(fabs(vecNewDirection.getLength() - 1) < EPSILON);
    setDirection(vecNewDirection);
    CSWSensor::update();
  }


  void CSWSonar::setMap(CSWMap::PtrType pMap)
  {
    m_pMap = pMap;
  }


  void CSWSonar::setEnableAutomaticRotation(bool bValue)
  {
    if (m_bAutomaticRotationEnabled == bValue)
      return;
    m_bAutomaticRotationEnabled = bValue;
    CSWSonar::update();
  }


  bool CSWSonar::isAutomaticRotationEnabled() const
  {
    return m_bAutomaticRotationEnabled;
  }


  void CSWSonar::setScanDirectionMode(int directionMode)
  {
    if (m_ScanDirectionMode == static_cast<ScanDirectionMode>(directionMode))
      return;
    m_ScanDirectionMode = static_cast<ScanDirectionMode>(directionMode);
    CSWSonar::update();
  }


  void CSWSonar::setScanVelocityMode(int velocityMode)
  {
    if (m_ScanVelocityMode == static_cast<ScanVelocityMode>(velocityMode))
      return;
    m_ScanVelocityMode = static_cast<ScanVelocityMode>(velocityMode);
    CSWSonar::update();
  }


  void CSWSonar::setScanRangeMode(int rangeMode)
  {
    if (m_ScanRangeMode == static_cast<ScanRangeMode>(rangeMode))
      return;
    m_ScanRangeMode = static_cast<ScanRangeMode>(rangeMode);
    CSWSonar::update();
  }


  CSWSonar::ScanDirectionMode CSWSonar::getScanDirectionMode() const
  {
    return m_ScanDirectionMode;
  }


  CSWSonar::ScanVelocityMode CSWSonar::getScanVelocityMode() const
  {
    return m_ScanVelocityMode;
  }


  CSWSonar::ScanRangeMode CSWSonar::getScanRangeMode() const
  {
    return m_ScanRangeMode;
  }


  void CSWSonar::setScanDirection(const Vector3D& vecDirection)
  {
    if (vecDirection != Vector3D::ZERO)
      m_vecMainScanDirection = vecDirection.getNormalized();
    else
      m_vecMainScanDirection = Vector3D(0, 0, 1);
  
    if (m_vecMainScanDirection == m_vecObjectTInitialUp || m_vecMainScanDirection == -m_vecObjectTInitialUp)
    {
      //use some perpendicular direction e.g initial direction
      m_vecMainScanUp = m_vecObjectTInitialDirection;
    }
    else
    {
      m_vecMainScanUp = m_vecObjectTInitialUp;
    }
    Vector3D vecLeft = m_vecMainScanUp % m_vecMainScanDirection;
    m_vecMainScanUp = m_vecMainScanDirection % vecLeft;
    CSWSonar::update();
  }


  const Vector3D& CSWSonar::getScanDirection() const
  {
    return m_vecMainScanDirection;
  }


  void CSWSonar::pointToGlobalPosition(const Vector3D& vecPosition)
  {
    m_vecWorldTPointToPosition = vecPosition;
    CSWSonar::update();
  }


  void CSWSonar::setDirection(const Vector3D& vecDirection)
  {
    setRelativeDirection(vecDirection);
  }


  const Vector3D& CSWSonar::getDirection() const
  {
    return getWorldTransform().getZAxis();
  }


  void CSWSonar::setWorldDirection(const Vector3D& vecWorldTDirection)
  {
    setAbsoluteDirection(vecWorldTDirection);
  }


  CSWSonar::CSWSonar(const std::string& strName, const Matrix44D& matBaseTObject)
  : CSWSensor(strName, matBaseTObject, getInRad(360), ANGLULAR_VELOCITY),
    m_bAutomaticRotationEnabled(true),
    m_ScanDirectionMode(FULL),
    m_ScanVelocityMode(FAST),
    m_ScanRangeMode(NEAR_RANGE),
    m_fActiveAutomaticRotationTimePeriod((rand()%4000)/1000.0), //initialize with random period so the sonar scanner have different relative directions
    m_fOldTime(0),
    m_vecMainScanDirection(0, 0, 1),
    m_vecMainScanUp(0, 1, 0),
    m_vecWorldTPointToPosition(0, 0, 0)
  {
  }


  CSWMap::PtrType CSWSonar::getMap()
  {
    return m_pMap.lock();
  }

}