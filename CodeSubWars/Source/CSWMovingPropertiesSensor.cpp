// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWSubmarine.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"

namespace CodeSubWars
{

  CSWMovingPropertiesSensor::PtrType CSWMovingPropertiesSensor::create(const std::string& strName, CSWEvent::PtrType pEvent)
  {
    return PtrType(new CSWMovingPropertiesSensor(strName, pEvent));
  }


  CSWMovingPropertiesSensor::~CSWMovingPropertiesSensor()
  {
  }


  void CSWMovingPropertiesSensor::update()
  {
    CSWSensor::update();
    m_bUpToDate = false;
    validate();
  
  //  if (m_pEvent)
  //    CSWEventManager::getInstance()->send(m_pEvent);
  }


  void CSWMovingPropertiesSensor::setEvent(CSWEvent::PtrType pEvent)
  {
    m_pEvent = pEvent;
  }


  const Vector3D& CSWMovingPropertiesSensor::getVelocity() const
  {
    return m_vecWorldTVelocity;
  }


  const Vector3D& CSWMovingPropertiesSensor::getAngularVelocity() const
  {
    return m_vecWorldTAngularVelocity;
  }


  const Vector3D& CSWMovingPropertiesSensor::getFrontAngularVelocity() const
  {
    return m_vecWorldTFrontAngularVelocity;
  }


  const Vector3D& CSWMovingPropertiesSensor::getAcceleration() const
  {
    return m_vecWorldTAcceleration;
  }


  const Vector3D& CSWMovingPropertiesSensor::getAngularAcceleration() const
  {
    return m_vecWorldTAngularAcceleration;
  }


  const Vector3D& CSWMovingPropertiesSensor::getFrontAngularAcceleration() const
  {
    return m_vecWorldTFrontAngularAcceleration;
  }


  double CSWMovingPropertiesSensor::getForwardVelocity() const
  {
    return m_vecObjectTVelocity.z;
  }


  double CSWMovingPropertiesSensor::getUpVelocity() const
  {
    return m_vecObjectTVelocity.y;
  }


  double CSWMovingPropertiesSensor::getLeftVelocity() const
  {
    return m_vecObjectTVelocity.x;
  }


  double CSWMovingPropertiesSensor::getUpAngularVelocity() const
  {
    return -m_vecObjectTAngularVelocity.x;
  }


  double CSWMovingPropertiesSensor::getLeftAngularVelocity() const
  {
    return m_vecObjectTAngularVelocity.y;
  }


  double CSWMovingPropertiesSensor::getAxialAngularVelocity() const
  {
    return m_vecObjectTAngularVelocity.z;
  }


  double CSWMovingPropertiesSensor::getForwardAcceleration() const
  {
    return m_vecObjectTAcceleration.z;
  }


  double CSWMovingPropertiesSensor::getUpAcceleration() const
  {
    return m_vecObjectTAcceleration.y;
  }


  double CSWMovingPropertiesSensor::getLeftAcceleration() const
  {
    return m_vecObjectTAcceleration.x;
  }


  double CSWMovingPropertiesSensor::getUpAngularAcceleration() const
  {
    return -m_vecObjectTAngularAcceleration.x;
  }


  double CSWMovingPropertiesSensor::getLeftAngularAcceleration() const
  {
    return m_vecObjectTAngularAcceleration.y;
  }


  double CSWMovingPropertiesSensor::getAxialAngularAcceleration() const
  {
    return m_vecObjectTAngularAcceleration.z;
  }


  CSWMovingPropertiesSensor::CSWMovingPropertiesSensor(const std::string& strName, CSWEvent::PtrType pEvent)
  : CSWSensor(strName, Matrix44D(), 0, 0),
    m_bUpToDate(false),
    m_vecWorldTVelocity(0, 0, 0),
    m_vecWorldTAngularVelocity(0, 0, 0),
    m_vecWorldTFrontAngularVelocity(0, 0, 0),
    m_vecWorldTAcceleration(0, 0, 0),
    m_vecWorldTAngularAcceleration(0, 0, 0),
    m_vecWorldTFrontAngularAcceleration(0, 0, 0),
    m_vecObjectTVelocity(0, 0, 0),
    m_vecObjectTAngularVelocity(0, 0, 0),
    m_vecObjectTAcceleration(0, 0, 0),
    m_vecObjectTAngularAcceleration(0, 0, 0),
    m_pEvent(pEvent)
  {
  }


  void CSWMovingPropertiesSensor::validate()
  {
    if (m_bUpToDate)
      return;

    CSWIDynamic::PtrType pDynamicObject = std::dynamic_pointer_cast<CSWIDynamic>(findFirstParent<CSWIDynamic>());
    if (pDynamicObject)
    {
      m_vecWorldTVelocity = pDynamicObject->getVelocity();
      m_vecWorldTAngularVelocity = pDynamicObject->getAngularVelocity()*180.0/ARSTD::PI;
      m_vecWorldTAcceleration = pDynamicObject->getAcceleration();
      m_vecWorldTAngularAcceleration = pDynamicObject->getAngularAcceleration()*180.0/ARSTD::PI;
  
      Matrix33D matObjectTWorld = getInvertWorldTransform().getRotationAsMatrix33();
    
      Vector3D vecObjectTFront;
      m_vecObjectTVelocity = matObjectTWorld*pDynamicObject->getVelocity();
      m_vecObjectTAngularVelocity = (matObjectTWorld*pDynamicObject->getAngularVelocity())*180.0/ARSTD::PI;
      m_vecObjectTAcceleration = matObjectTWorld*pDynamicObject->getAcceleration();
      m_vecObjectTAngularAcceleration = (matObjectTWorld*pDynamicObject->getAngularAcceleration())*180.0/ARSTD::PI;
    }
    else
    {
      m_vecWorldTVelocity = Vector3D(0, 0, 0);
      m_vecWorldTAngularVelocity = Vector3D(0, 0, 0);
      m_vecWorldTAcceleration = Vector3D(0, 0, 0);
      m_vecWorldTAngularAcceleration = Vector3D(0, 0, 0);

      m_vecObjectTVelocity = Vector3D(0, 0, 0);
      m_vecObjectTAngularVelocity = Vector3D(0, 0, 0);
      m_vecObjectTAcceleration = Vector3D(0, 0, 0);
      m_vecObjectTAngularAcceleration = Vector3D(0, 0, 0);
    }

    CSWISolid::PtrType pSolidObject = std::dynamic_pointer_cast<CSWISolid>(findFirstParent<CSWISolid>());
    if (pSolidObject && pDynamicObject)
    {
      Vector3D vecObjectTFront(0, 0, pSolidObject->getSurfaceBoundingBox().getSize().getDepth()/2);
      m_vecWorldTFrontAngularVelocity = pDynamicObject->getAngularVelocity(vecObjectTFront);
      m_vecWorldTFrontAngularAcceleration = pDynamicObject->getAngularAcceleration(vecObjectTFront);
    }
    else
    {
      m_vecWorldTFrontAngularVelocity = Vector3D(0, 0, 0);
      m_vecWorldTFrontAngularAcceleration = Vector3D(0, 0, 0);
    }

    m_bUpToDate = true;
  }

}