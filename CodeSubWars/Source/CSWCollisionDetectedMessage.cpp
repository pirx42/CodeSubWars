// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWCollisionDetectedMessage.h"


namespace CodeSubWars
{

  CSWCollisionDetectedMessage::PtrType CSWCollisionDetectedMessage::create(const Vector3D& vecWorldTPoint, const double& fDamage)
  {
    return PtrType(new CSWCollisionDetectedMessage(vecWorldTPoint, fDamage));
  }


  CSWCollisionDetectedMessage::~CSWCollisionDetectedMessage()
  {
  }


  CSWIMessage::PtrType CSWCollisionDetectedMessage::copy() const
  {
    return PtrType(new CSWCollisionDetectedMessage(*this));
  }


  std::string CSWCollisionDetectedMessage::getInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "CollisionDetectedMessage "
       << "World Position [m]: " << m_vecWorldTPoint.x << ", " << m_vecWorldTPoint.y << ", " << m_vecWorldTPoint.z << ", "
       << "Damage: " << m_fDamage;
    return ss.str();
  }


  const Vector3D& CSWCollisionDetectedMessage::getPoint() const
  {
    return m_vecWorldTPoint;
  }


  const double& CSWCollisionDetectedMessage::getDamage() const
  {
    return m_fDamage;
  }


  CSWCollisionDetectedMessage::CSWCollisionDetectedMessage(const Vector3D& vecWorldTPoint, const double& fDamage)
  : m_vecWorldTPoint(vecWorldTPoint),
    m_fDamage(fDamage)
  {
  }


  CSWCollisionDetectedMessage::CSWCollisionDetectedMessage(const CSWCollisionDetectedMessage& other)
  : m_vecWorldTPoint(other.m_vecWorldTPoint),
    m_fDamage(other.m_fDamage)
  {
  }

}