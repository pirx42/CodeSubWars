// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWExplosionDetectedMessage.h"


namespace CodeSubWars
{

  CSWExplosionDetectedMessage::PtrType CSWExplosionDetectedMessage::create(const Vector3D& vecWorldTPoint, const double& fDamage)
  {
    return PtrType(new CSWExplosionDetectedMessage(vecWorldTPoint, fDamage));
  }


  CSWExplosionDetectedMessage::~CSWExplosionDetectedMessage()
  {
  }


  CSWIMessage::PtrType CSWExplosionDetectedMessage::copy() const
  {
    return PtrType(new CSWExplosionDetectedMessage(*this));
  }


  std::string CSWExplosionDetectedMessage::getInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "ExplosionDetectedMessage "
       << "World Position [m]: " << m_vecWorldTPoint.x << ", " << m_vecWorldTPoint.y << ", " << m_vecWorldTPoint.z << ", "
       << "Damage: " << m_fDamage;
    return ss.str();
  }


  const Vector3D& CSWExplosionDetectedMessage::getPoint() const
  {
    return m_vecWorldTPoint;
  }


  const double& CSWExplosionDetectedMessage::getDamage() const
  {
    return m_fDamage;
  }


  CSWExplosionDetectedMessage::CSWExplosionDetectedMessage(const Vector3D& vecWorldTPoint, const double& fDamage)
  : m_vecWorldTPoint(vecWorldTPoint),
    m_fDamage(fDamage)
  {
  }


  CSWExplosionDetectedMessage::CSWExplosionDetectedMessage(const CSWExplosionDetectedMessage& other)
  : m_vecWorldTPoint(other.m_vecWorldTPoint),
    m_fDamage(other.m_fDamage)
  {
  }

}