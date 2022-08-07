// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWUtilities.h"
#include "CSWGyroCompass.h"
#include "CSWSubmarine.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"

namespace CodeSubWars
{

  CSWGyroCompass::PtrType CSWGyroCompass::create(const std::string& strName, CSWEvent::PtrType pEvent)
  {
    return PtrType(new CSWGyroCompass(strName, pEvent));
  }


  CSWGyroCompass::~CSWGyroCompass()
  {
  }


  void CSWGyroCompass::update()
  {
    CSWSensor::update();
    m_bUpToDate = false;
    validate();
  //  if (fabs(getDirection() - 30) < 5)
  //  {
  //    if (m_pEvent)
  //      CSWEventManager::getInstance()->send(m_pEvent);
  //  }
  }


  void CSWGyroCompass::setEvent(CSWEvent::PtrType pEvent)
  {
    m_pEvent = pEvent;
  }


  const Vector3D& CSWGyroCompass::getForwardDirection() const
  {
    return getWorldTransform().getZAxis();
  }


  const Vector3D& CSWGyroCompass::getUpDirection() const
  {
    return getWorldTransform().getYAxis();
  }


  Matrix33D CSWGyroCompass::getOrientation() const
  {
    return getWorldTransform().getRotationAsMatrix33();
  }


  double CSWGyroCompass::getDirection()
  {
  //  validate();
    return m_fDirection;
  }


  double CSWGyroCompass::getInclination()
  {
  //  validate();
    return m_fInclination;
  }


  double CSWGyroCompass::getAxialInclination()
  {
  //  validate();
    return m_fAxialInclination;
  }


  CSWGyroCompass::CSWGyroCompass(const std::string& strName, CSWEvent::PtrType pEvent)
  : CSWSensor(strName, Matrix44D(), 0, 0),
    m_bUpToDate(false),
    m_fDirection(0),
    m_fInclination(0),
    m_fAxialInclination(0),
    m_pEvent(pEvent)
  {
  }


  void CSWGyroCompass::validate()
  {
    if (m_bUpToDate)
      return;

    Vector3D angles = CSWUtilities::determineAngles(getWorldTransform());
    m_fDirection = angles.y;
    m_fInclination = angles.x;
    m_fAxialInclination = angles.z;

    m_bUpToDate = true;
  }


}