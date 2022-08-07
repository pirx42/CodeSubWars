// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorld.h"
#include "CSWBattleStatistics.h"
#include "CSWExplosionVisualizer.h"
#include "CSWSoundVisualizer.h"
#include "CSWWeapon.h"
#include "CSWEventManager.h"
#include "CSWEvent.h"
#include "CSWExplosionDetectedMessage.h"
#include "CSWISoundReceiver.h"
#include "CSWCollideable.h"


namespace CodeSubWars
{

  CSWWeapon::~CSWWeapon()
  {
  }


  bool CSWWeapon::isImpulsEmitterActive() const
  {
    return isLaunched() && isArmed() && isFired();
  }


  void CSWWeapon::initializeImpulsEmission()
  {
    assert(isImpulsEmitterActive());
    CSWWorld::getInstance()->getExplosionVisualizer()->addExplosion(getWorldTransform().getTranslation(), m_fRadius);
  }


  void CSWWeapon::emitImpuls(CSWObject::PtrType pObject)
  {
    assert(isImpulsEmitterActive());
    CSWIDynamic::PtrType pDynObj = std::dynamic_pointer_cast<CSWIDynamic>(pObject);
    CSWIDamageable::PtrType pDamObj = std::dynamic_pointer_cast<CSWIDamageable>(pObject);
    if (!pDynObj && !pDamObj)
      return;

    if (pDamObj && !pDamObj->isAlive())
      return;

    Vector3D vecDistance = pObject->getWorldTransform().getTranslation() - getWorldTransform().getTranslation();
    bool bIsInRange = false;
    double fDistance = std::numeric_limits<double>::max();
    if (CSWISolid::PtrType pSolidObj = std::dynamic_pointer_cast<CSWISolid>(pObject))
    {
      BoundingBoxD bbWorldTObject = pSolidObj->getSurfaceBoundingBox();
      bbWorldTObject.transform(pObject->getWorldTransform());
      double fMaxBoxDimension = std::max(bbWorldTObject.getWidth(), std::max(bbWorldTObject.getHeight(), bbWorldTObject.getDepth()));
      if (vecDistance.getLength() < m_fRadius + fMaxBoxDimension*0.5)
      {
        //calculate the correct distance to bounding box (this approximate the distance to the surface)
        double fMinimalDistance = std::numeric_limits<double>::max();
        for (int nCnt = 0; nCnt < 6; ++nCnt)
        {
          double fCurrentDistance = bbWorldTObject.getRectangle(nCnt).getDistanceTo(getWorldTransform().getTranslation());
          if (fCurrentDistance < fMinimalDistance)
            fMinimalDistance = fCurrentDistance;
        }
        fDistance = fMinimalDistance;
        bIsInRange = fDistance < m_fRadius;
      }
    }
    else
    {
      fDistance = vecDistance.getLength();
      bIsInRange = fDistance < m_fRadius;
    }
  
    if (bIsInRange)
    {
      double fDistanceExplosionReduceFactor = pow(1.1, -fDistance);

      CSWExplosionDetectedMessage::PtrType pMessage = CSWExplosionDetectedMessage::create(getWorldTransform().getTranslation(), 
                                                                                          m_fMaxDamage*fDistanceExplosionReduceFactor);
      CSWEventManager::getInstance()->send(CSWEvent::create(getSharedThis(), pObject, pMessage, 1));
  
      if (pDynObj)
      {
        //find main axis where the weapon is closest to (which component x,y,z is the smallest in object coordinates)
        Vector3D vecDestinationObjectTWeaponPosition = pObject->getInvertWorldTransform()*getWorldTransform().getTranslation();
        double fMinComponentValue = std::numeric_limits<double>::max();
        int nIdx = -1;
        double fCurrentSquaredAxisDistance = vecDestinationObjectTWeaponPosition.y*vecDestinationObjectTWeaponPosition.y + 
                                             vecDestinationObjectTWeaponPosition.z*vecDestinationObjectTWeaponPosition.z;
        if (fCurrentSquaredAxisDistance < fMinComponentValue)
        { 
          fMinComponentValue = fCurrentSquaredAxisDistance; nIdx = 0;
        }
        fCurrentSquaredAxisDistance = vecDestinationObjectTWeaponPosition.z*vecDestinationObjectTWeaponPosition.z + 
                                      vecDestinationObjectTWeaponPosition.x*vecDestinationObjectTWeaponPosition.x;
        if (fCurrentSquaredAxisDistance < fMinComponentValue)
        {
          fMinComponentValue = fCurrentSquaredAxisDistance; nIdx = 1;
        }
        fCurrentSquaredAxisDistance = vecDestinationObjectTWeaponPosition.x*vecDestinationObjectTWeaponPosition.x + 
                                      vecDestinationObjectTWeaponPosition.y*vecDestinationObjectTWeaponPosition.y;
        if (fCurrentSquaredAxisDistance < fMinComponentValue)
        {
          fMinComponentValue = fCurrentSquaredAxisDistance; nIdx = 2;
        }
        assert(nIdx >= 0 && nIdx < 3);
      
        Vector3D vecObjectTClosestPoint(0, 0, 0);
        vecObjectTClosestPoint.pData[nIdx] = vecDestinationObjectTWeaponPosition.pData[nIdx];
        if (vecObjectTClosestPoint == vecDestinationObjectTWeaponPosition)
        {
          //if the weapon already lies on a main axis use the mass center as closest point
          vecObjectTClosestPoint = pDynObj->getCenterOfMass();
        }

        assert(vecObjectTClosestPoint != vecDestinationObjectTWeaponPosition && "corresponding explosion point on object not correctly determined");
        Vector3D vecWorldTImpulsDirection = pObject->getWorldTransform().getRotationAsMatrix33()*
                                            (vecObjectTClosestPoint - vecDestinationObjectTWeaponPosition);
        vecWorldTImpulsDirection.normalize();

        double j = m_fPower*fDistanceExplosionReduceFactor;
        pDynObj->addImpuls(getWorldTransform().getTranslation(), vecWorldTImpulsDirection*j);
      }
    
      if (pDamObj)
      {
        pDamObj->addDamage(m_fMaxDamage*fDistanceExplosionReduceFactor);

        CSWWorld::getInstance()->getBattleStatistics()->reportHit(getSharedThis(), pObject);
        if (!pDamObj->isAlive())
          CSWWorld::getInstance()->getBattleStatistics()->reportKill(getSharedThis(), pObject);
      }
    }  
  }


  void CSWWeapon::finalizeImpulsEmission()
  {
    //only one impuls has to be emitted to each object
    //after explosion the weapon is dead
    setDead(true);
  }


  bool CSWWeapon::isSoundEmitterActive() const
  {
    return isImpulsEmitterActive();
  }


  void CSWWeapon::initializeSoundEmission()
  {
    CSWWorld::getInstance()->getSoundVisualizer()->addSound(getWorldTransform().getTranslation(), m_fPower*1e6, reinterpret_cast<unsigned long long>(this));
  }


  void CSWWeapon::emitSound(CSWObject::PtrType pObject)
  {
    assert(isSoundEmitterActive());
    if (CSWISoundReceiver::PtrType pSoundReceiver = std::dynamic_pointer_cast<CSWISoundReceiver>(pObject))
    {
      pSoundReceiver->addSound(getWorldTransform().getTranslation(), m_fPower*1e6);
    }
  }


  void CSWWeapon::finalizeSoundEmission()
  {
  }


  void CSWWeapon::initializeEnvironment(bool bSimplified)
  {
    AxisAlignedBoundingBoxD aabb = getSurface()->getAxisAlignedBoundingBox();

    double fRadius = std::max(aabb.getWidth(), aabb.getHeight())*0.5;
    Vector3D vecDiameter(fRadius*2, fRadius*2, fRadius*2);

    Vector3D vecStart = aabb.getMin() + Vector3D(aabb.getWidth()*0.5, aabb.getHeight()*0.5, fRadius);
    Vector3D vecEnd = aabb.getMax() - Vector3D(aabb.getWidth()*0.5, aabb.getHeight()*0.5, fRadius);

    DT_ShapeHandle hDTShape = NULL;
    Mesh::PtrType pCollisionMesh;
    if (vecStart.getDistance(vecEnd) < EPSILON)
    {
      hDTShape = DT_NewPoint(static_cast<Vector3F>(vecStart).pData);
      pCollisionMesh = Mesh::createEllipsoid(Matrix44D(vecStart), vecDiameter);
    }
    else
    {
      hDTShape = DT_NewLineSegment(static_cast<Vector3F>(vecStart).pData, static_cast<Vector3F>(vecEnd).pData);
      Matrix44D matStart(vecStart);
      matStart.getXAxis() = (vecEnd - vecStart).getNormalized();
      matStart.getYAxis() = matStart.getXAxis().getPerpendicular();
      matStart.getZAxis() = matStart.getXAxis()%matStart.getYAxis();
      Matrix44D matEnd(matStart);
      matEnd.getTranslation() = vecEnd;

      pCollisionMesh = Mesh::createEllipsoid(matStart, vecDiameter, 16, 16, 0, 8);
      pCollisionMesh->add(Mesh::createCylinder(matStart, (vecEnd - vecStart).getLength(), fRadius*2, fRadius*2));
      pCollisionMesh->add(Mesh::createEllipsoid(matEnd, vecDiameter, 16, 16, 8, 16));
    }

    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);
    DT_SetMargin(hDTObject, fRadius);
    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, Matrix44D::IDENTITY, pCollisionMesh);
  }


  bool CSWWeapon::launch(double fLaunchDelay)
  {
    if (fLaunchDelay < 0)
      return false;
    if (isLaunched())
      return false;

    m_fAbsLaunchTime = ARSTD::Time::getTime() + fLaunchDelay;
    return true;
  }


  bool CSWWeapon::arm(double fArmDelay)
  {
    if (fArmDelay < 0)
      return false;
    if (isLaunched() || isFired())
      return false;

    m_fAbsArmTime = ARSTD::Time::getTime() + fArmDelay;
    if (m_fAbsFireTime < m_fAbsArmTime)
      m_fAbsFireTime = m_fAbsArmTime;
  
    assert(m_fAbsArmTime <= m_fAbsFireTime);
    return true;
  }


  bool CSWWeapon::fire(double fFireDelay)
  {
    if (fFireDelay < 0)
      return false;
    if (isLaunched() || isFired())
      return false;

    m_fAbsFireTime = ARSTD::Time::getTime() + fFireDelay;
    if (m_fAbsFireTime < m_fAbsArmTime)
      m_fAbsArmTime = m_fAbsFireTime;
  
    assert(m_fAbsArmTime <= m_fAbsFireTime);
    return true;
  }


  bool CSWWeapon::isLaunched() const
  {
    return m_fAbsLaunchTime <= ARSTD::Time::getTime();
  }


  bool CSWWeapon::isArmed() const
  {
    return m_fAbsArmTime <= ARSTD::Time::getTime();
  }


  bool CSWWeapon::isFired() const
  {
    return m_fAbsFireTime <= ARSTD::Time::getTime();
  }


  void CSWWeapon::setShooterName(const std::string& strName)
  {
    m_strShooterName = strName;
  }


  const std::string& CSWWeapon::getShooterName() const
  {
    return m_strShooterName;
  }


  bool CSWWeapon::isShooterSubmarine() const
  {
    return m_bShooterSubmarine;
  }


  void CSWWeapon::setShooterSubmarine(bool bVal)
  {
    m_bShooterSubmarine = bVal;
  }


  CSWWeapon::CSWWeapon(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWDamDynSolCol(strName, matBaseTObject, 
                    Mesh::createEllipsoid(Matrix44D(Vector3D(0, 0, 0)),
                                          Vector3D(size.getWidth(),
                                                   size.getHeight(), 
                                                   size.getDepth())),
                    true),
    m_fAbsLaunchTime(std::numeric_limits<double>::max()),
    m_fAbsArmTime(std::numeric_limits<double>::max()),
    m_fAbsFireTime(std::numeric_limits<double>::max())
  {
    m_fPower = size.getVolume() * 5e+7;
    assert(m_fPower > 0);

    m_fMaxDamage = 0.1 * size.getVolume() / (0.5 * 0.5 * 5);
    assert(m_fMaxDamage < 1);

    //determine radius on which the damage is 0.0001
    m_fRadius = log(m_fMaxDamage/0.0001)/log(1.1);
    assert(m_fRadius < 100);
  }


  void CSWWeapon::fireImmediately(bool bIgnoreArmTime)
  {
    if (!isLaunched())
      return;
  
    if (isArmed())
      m_fAbsFireTime = ARSTD::Time::getTime();
    else if (bIgnoreArmTime)
    {
      m_fAbsArmTime = ARSTD::Time::getTime();
      m_fAbsFireTime = ARSTD::Time::getTime();
    }
  }


  double CSWWeapon::getAbsLaunchTime() const
  {
    return m_fAbsLaunchTime;
  }


  double CSWWeapon::getAbsArmTime() const
  {
    return m_fAbsArmTime;
  }


  double CSWWeapon::getAbsFireTime() const
  {
    return m_fAbsFireTime;
  }

}