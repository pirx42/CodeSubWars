// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWEquipment.h"


namespace CodeSubWars
{

  CSWEquipment::~CSWEquipment()
  {
  }


  void CSWEquipment::update()
  {
    double fCurrentTime = ARSTD::Time::getTime();
    double fElapsedTime = fCurrentTime - m_fLastTime;
    m_fLastTime = fCurrentTime;
  
    std::pair<bool, Matrix44D> result = calcNewTransform(fElapsedTime);
    m_bMoving = result.first;

    if (!m_bMoving)
      return;
    getTransform() = result.second;

    CSWObject* pParent = dynamic_cast<CSWObject*>(getParent());
    Matrix44D matWorldTParent;
    if (pParent)
      matWorldTParent = pParent->getWorldTransform();

    setWorldTransform(matWorldTParent*getTransform());
  }


  void CSWEquipment::setRelativeDirection(const Vector3D& vecDirection)
  {
    Vector3D vecClampedDirection(vecDirection.getNormalized());  
    double fAngle = m_vecObjectTInitialDirection.getAngleBetween(vecClampedDirection);
    if (fAngle > m_fMaxAngle)
    {
      Vector3D vecRotationAxis = vecClampedDirection % m_vecObjectTInitialDirection;
      vecClampedDirection = vecClampedDirection.getRotated(vecRotationAxis, fAngle - m_fMaxAngle);
    }

    double fOldToNewAngle = m_matParentTObjectDestination.getZAxis().getAngleBetween(vecClampedDirection);
    if (fOldToNewAngle < EPSILON)
      return;
  
    m_matParentTObjectDestination.getTranslation() = getTransform().getTranslation();
    m_matParentTObjectDestination.getZAxis() = vecClampedDirection;
    if (m_matParentTObjectDestination.getZAxis() == m_vecObjectTInitialUp || 
        m_matParentTObjectDestination.getZAxis() == -m_vecObjectTInitialUp)
    {
      //use some perpendicular direction e.g initial direction
      m_matParentTObjectDestination.getYAxis() = m_vecObjectTInitialDirection;
    }
    else
    {
      m_matParentTObjectDestination.getYAxis() = m_vecObjectTInitialUp;
    }
    m_matParentTObjectDestination.getXAxis() = (m_matParentTObjectDestination.getYAxis() % m_matParentTObjectDestination.getZAxis()).getNormalized();
    m_matParentTObjectDestination.getYAxis() = m_matParentTObjectDestination.getZAxis() % m_matParentTObjectDestination.getXAxis();
  }


  void CSWEquipment::setAbsoluteDirection(const Vector3D& vecWorldTDirection)
  {
    CSWObject* pParent = dynamic_cast<CSWObject*>(getParent());
    Matrix33D matParentTWorld;
    if (pParent)
      matParentTWorld = pParent->getInvertWorldTransform().getRotationAsMatrix33();
  
    setRelativeDirection(matParentTWorld*vecWorldTDirection);
  }


  void CSWEquipment::setMaxAngle(const double& fMaxAngle)
  {
    m_fMaxAngle = fMaxAngle;
  }


  void CSWEquipment::setVelocity(const double& fVelocity)
  {
    m_fVelocity = fVelocity;
  }


  void CSWEquipment::setAngularVelocity(const double& fAngularVelocity)
  {
    m_fAngularVelocity = fAngularVelocity;
  }


  bool CSWEquipment::isMoving() const
  {
    return m_bMoving;
  }


  QWidget* CSWEquipment::getInformationWidget(QWidget* pParent)
  {
    return NULL;
  }


  CSWEquipment::CSWEquipment(const std::string& strName, const Matrix44D& matBaseTObject,
                             const double& fMaxAngle, const double& fAngularVelocity)
  : CSWObject(strName, matBaseTObject),
    m_vecObjectTInitialDirection(matBaseTObject.getZAxis()),
    m_vecObjectTInitialUp(matBaseTObject.getYAxis()),
    m_matParentTObjectDestination(matBaseTObject),
    m_bMoving(false),
    m_fLastTime(0),
    m_fVelocity(0),
    m_fAngularVelocity(fAngularVelocity),
    m_fMaxAngle(fMaxAngle)
  {
  }


  std::pair<bool, Matrix44D> CSWEquipment::calcNewTransform(const double& fElapsedTime)
  {
    QuaternionD quatRot(getTransform().getRotationAsQuaternion());
    double fAngle = quatRot.getAngleBetween(m_matParentTObjectDestination.getRotationAsQuaternion());
    if (fAngle > EPSILON)
    {
      quatRot = QuaternionD::slerp(quatRot, m_matParentTObjectDestination.getRotationAsQuaternion(), 
                                   m_fAngularVelocity*fElapsedTime/fAngle);
    }                                          
                                           
    Vector3D vecTrans(getTransform().getTranslation());
    double fDistance = (vecTrans - m_matParentTObjectDestination.getTranslation()).getSquaredLength();
    if (fDistance > EPSILON_SQUARED)
    {
      vecTrans = Vector3D::lerp(vecTrans, m_matParentTObjectDestination.getTranslation(), 
                                m_fVelocity*fElapsedTime/fDistance);
    }
  
    return std::make_pair(fAngle > EPSILON || fDistance > EPSILON_SQUARED, Matrix44D(quatRot, vecTrans));
  }

}