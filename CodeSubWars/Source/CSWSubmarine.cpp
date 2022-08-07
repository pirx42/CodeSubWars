// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

    
#include "PrecompiledHeader.h"
#include "Constants.h"
#include "CSWSubmarine.h"
#include "CSWWorld.h"
#include "CSWWorldGuard.h"
#include "CSWLog.h"
#include "CSWSettings.h"
#include "CSWBattleStatistics.h"
#include "CSWExplosionVisualizer.h"
#include "CSWSoundVisualizer.h"

#include "CSWCommandable.h"
#include "CSWEventDealable.h"
#include "CSWSolid.h"
#include "CSWCollideable.h"
#include "CSWPythonable.h"

#include "CSWEventManager.h"
#include "CSWEvent.h"
#include "CSWExplosionDetectedMessage.h"

#include "CSWEngine.h"
#include "CSWControlCenter.h"
#include "CSWActiveSonar.h"
#include "CSWPassiveSonar.h"

#include "CSWWeaponBattery.h"
#include "CSWWeaponSupply.h"

#include "CSWSubmarineMesh.h"

#include "GreenMine.h"
#include "YellowMine.h"
#include "GreenTorpedo.h"
#include "RedTorpedo.h"
#include "BlueTorpedo.h"


namespace CodeSubWars
{

  const double CSWSubmarine::MAX_OAR_ANGLE = PI/2;
  const int CSWSubmarine::NUM_CYCLES = 100;  

  
  CSWSubmarine::~CSWSubmarine()
  {
  }


  void CSWSubmarine::updatePosition()
  {
    if (isAlive())
      CSWDamDynSolCol::updatePosition();
  }


  void CSWSubmarine::receiveEvent(CSWEvent::PtrType pEvent)
  {
    if (isAlive())
      m_pEventDealable->receiveEvent(pEvent);
  }


  void CSWSubmarine::processReceivedQueuedEvents()
  {
    if (isAlive())
      m_pEventDealable->processReceivedQueuedEvents();
  }


  std::string CSWSubmarine::getCurrentInformation() const
  {
    return m_pEventDealable->getCurrentInformation();
  }


  std::string CSWSubmarine::getCurrentMessage() const
  {
    return m_pEventDealable->getCurrentMessage();
  }


  void CSWSubmarine::step()
  {
    if (!isAlive())
      return;
    try
    {
      double fStart = ARSTD::Time::getRealTime();
    
      std::string strCurrentCommandName = m_pCommandable->getCommandProcessor()->getCurrentCommandName();
      m_pCommandable->step();
    
      m_Times.push_back(ARSTD::Time::getRealTime() - fStart);
      double fAvgTime = std::accumulate(m_Times.begin(), m_Times.end(), 0) / m_Times.capacity();
      if (fAvgTime > Constants::CRITICAL_CONSUMING_TIME)
      {
        CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
        std::stringstream ss;
        ss << getName() << "::" << strCurrentCommandName << " causes critical processor load (" 
            << fAvgTime*1000.0 << "ms)";
        CSWLog::getInstance()->log(ss.str());
      }
    }
    catch (...)
    {
      CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
      PyErr_Print();
    }
  }


  ARSTD::CommandProcessor::PtrType CSWSubmarine::getCommandProcessor()
  {
    return m_pCommandable->getCommandProcessor();
  }


  void CSWSubmarine::update()
  {
  }


  void CSWSubmarine::initializeEnvironment(bool bSimplified)
  {
    AxisAlignedBoundingBoxD aabb = getSurface()->getAxisAlignedBoundingBox();

    double fRadius = std::max(aabb.getWidth(), aabb.getHeight())*0.5;
    Vector3D vecDiameter(fRadius*2, fRadius*2, fRadius*2);

    Vector3D vecStart = aabb.getMin() + Vector3D(aabb.getWidth()*0.5, aabb.getHeight()*0.5, fRadius);
    Vector3D vecEnd = aabb.getMax() - Vector3D(aabb.getWidth()*0.5, aabb.getHeight()*0.5, fRadius);

    DT_ShapeHandle hDTShape = DT_NewLineSegment(static_cast<Vector3F>(vecStart).pData, static_cast<Vector3F>(vecEnd).pData);
    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);
    DT_SetMargin(hDTObject, fRadius);

    Matrix44D matStart(vecStart);
    matStart.getXAxis() = (vecEnd - vecStart).getNormalized();
    matStart.getYAxis() = matStart.getXAxis().getPerpendicular();
    matStart.getZAxis() = matStart.getXAxis()%matStart.getYAxis();
    Matrix44D matEnd(matStart);
    matEnd.getTranslation() = vecEnd;

    Mesh::PtrType pCollisionMesh = Mesh::createEllipsoid(matStart, vecDiameter, 16, 16, 0, 8);
    pCollisionMesh->add(Mesh::createCylinder(matStart, (vecEnd - vecStart).getLength(), fRadius*2, fRadius*2));
    pCollisionMesh->add(Mesh::createEllipsoid(matEnd, vecDiameter, 16, 16, 8, 16));
  
    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, Matrix44D::IDENTITY, pCollisionMesh);
  }


  bool CSWSubmarine::isImpulsEmitterActive() const
  {
    return isZombie();
  }


  void CSWSubmarine::initializeImpulsEmission()
  {
    assert(isImpulsEmitterActive());
    CSWWorld::getInstance()->getExplosionVisualizer()->addExplosion(getWorldTransform().getTranslation(), m_fExplosionRadius);
  }


  void CSWSubmarine::emitImpuls(CSWObject::PtrType pObject)
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
      if (vecDistance.getLength() < m_fExplosionRadius + fMaxBoxDimension*0.5)
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
        bIsInRange = fDistance < m_fExplosionRadius;
      }
    }
    else
    {
      fDistance = vecDistance.getLength();
      bIsInRange = fDistance < m_fExplosionRadius;
    }
  
    if (bIsInRange)
    {
      double fDistanceExplosionReduceFactor = pow(1.05, -fDistance);

      CSWExplosionDetectedMessage::PtrType pMessage = CSWExplosionDetectedMessage::create(getWorldTransform().getTranslation(), 
                                                                                          m_fMaxExplosionDamage*fDistanceExplosionReduceFactor);
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

        double j = m_fExplosionPower*fDistanceExplosionReduceFactor;
        pDynObj->addImpuls(getWorldTransform().getTranslation(), vecWorldTImpulsDirection*j);
      }

      if (pDamObj)
      {
        pDamObj->addDamage(m_fMaxExplosionDamage*fDistanceExplosionReduceFactor);

        CSWWorld::getInstance()->getBattleStatistics()->reportHit(getSharedThis(), pObject);
        if (!pDamObj->isAlive())
          CSWWorld::getInstance()->getBattleStatistics()->reportKill(getSharedThis(), pObject);
      }
    }
  }


  void CSWSubmarine::finalizeImpulsEmission()
  {
    setDead(true);
  }


  bool CSWSubmarine::isSoundEmitterActive() const
  {
    return isImpulsEmitterActive();
  }


  void CSWSubmarine::initializeSoundEmission()
  {
    CSWWorld::getInstance()->getSoundVisualizer()->addSound(getWorldTransform().getTranslation(), m_fExplosionPower*1e6, reinterpret_cast<unsigned long long>(this));
  }


  void CSWSubmarine::emitSound(CSWObject::PtrType pObject)
  {
    assert(isSoundEmitterActive());
    if (CSWISoundReceiver::PtrType pSoundReceiver = std::dynamic_pointer_cast<CSWISoundReceiver>(pObject))
    {
      pSoundReceiver->addSound(getWorldTransform().getTranslation(), m_fExplosionPower*1e6);
    }
  }


  void CSWSubmarine::finalizeSoundEmission()
  {
  }


  void CSWSubmarine::draw()
  {
    if (!isAlive())
      return;
  
    CSWDamDynSolCol::draw();
  
    if (CSWWorld::getInstance()->getSettings()->getSelectedSolidObject() == getSharedThis())
    {
      m_pControlCenter->getActiveSonar()->draw();
      m_pControlCenter->getPassiveSonar()->draw();
    }
  }


  Vector3D CSWSubmarine::makeLocalDirection(const Vector3D& vecWorldTDirection)
  {
    return getInvertWorldTransform().getRotationAsMatrix33()*vecWorldTDirection;
  }


  Vector3D CSWSubmarine::makeGlobalDirection(const Vector3D& vecObjectTDirection)
  {
    return getWorldTransform().getRotationAsMatrix33()*vecObjectTDirection;
  }


  Vector3D CSWSubmarine::makeLocalPosition(const Vector3D& vecWorldTPosition)
  {
    return getInvertWorldTransform()*vecWorldTPosition;
  }


  Vector3D CSWSubmarine::makeGlobalPosition(const Vector3D& vecObjectTPosition)
  {
    return getWorldTransform()*vecObjectTPosition;
  }


  int CSWSubmarine::getTeamID() const
  {
    return m_nTeamID;
  }


  void CSWSubmarine::setTeamID(int nID)
  {
    m_nTeamID = nID;
  }


  CSWSubmarine::CSWSubmarine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWDamDynSolCol(strName, matBaseTObject,
                    Mesh::readSTL(std::istrstream(reinterpret_cast<const char*>(SUBMARINE_MESH), sizeof(SUBMARINE_MESH)))->transformed(Matrix44D(Vector4D(fLength, 0, 0, 0),
                                                                                                                                                 Vector4D(0, fLength, 0, 0),
                                                                                                                                                 Vector4D(0, 0, fLength, 0),
                                                                                                                                                 Vector4D(0, 0, 0, 1))),
                    false),
    m_pEventDealable(CSWEventDealable::create()),
    m_pCommandable(CSWCommandable::create()),
    m_pControlCenter(CSWControlCenter::create(strName + ".ControlCenter", Matrix44D())),
    m_Times(NUM_CYCLES),
    m_nTeamID(0)
  {
    if (fLength < 30 || fLength > 110)
      throw std::runtime_error("submarine has no valid length");

    attach(m_pControlCenter);

    Size3D sizeSurface = m_pSolid->getMesh()->getAxisAlignedBoundingBox().getSize();

    m_fExplosionPower = sizeSurface.getVolume() * 1e+5;
    assert(m_fExplosionPower > 0);

    m_fMaxExplosionDamage = 0.2;

    //determine radius on which the damage is 0.0001
    m_fExplosionRadius = log(m_fMaxExplosionDamage/0.0001)/log(1.05);
    assert(m_fExplosionRadius < 200);


    //create and add equipment
    m_pMainEngine = CSWEngine::create(strName + ".MainEngine", 
                                      Matrix44D(m_pDynamic->getCenterOfMass() - Vector3D(0, 0, sizeSurface.getDepth()/2)), 
                                      sizeSurface/Size3D(5.5, 5.5, 5.5),
                                      true);
    attach(m_pMainEngine);

    m_pBuoyancyTank = CSWEngine::create(strName + ".BuoyancyTank",  
                                        Matrix44D(Vector3D(1, 0, 0), 
                                                  Vector3D(0, 0, -1), 
                                                  Vector3D(0, 1, 0), 
                                                  m_pDynamic->getCenterOfMass()), 
                                        sizeSurface/Size3D(50, 50, 50));
    attach(m_pBuoyancyTank);

    m_pBowsJetOar = CSWEngine::create(strName + ".BowsJetOar", 
                                      Matrix44D(Vector3D(0, 0, 1), 
                                                Vector3D(0, 1, 0), 
                                                Vector3D(-1, 0, 0), 
                                                m_pDynamic->getCenterOfMass() + Vector3D(0, 0, sizeSurface.getDepth()/2)), 
                                      sizeSurface/Size3D(15, 15, 15));
    attach(m_pBowsJetOar);

    m_pInclinationJetOar = CSWEngine::create(strName + ".InclinationJetOar", 
                                             Matrix44D(Vector3D(1, 0, 0), 
                                                       Vector3D(0, 0, -1), 
                                                       Vector3D(0, 1, 0), 
                                                       m_pDynamic->getCenterOfMass() + Vector3D(0, 0, sizeSurface.getDepth()/2)), 
                                             sizeSurface/Size3D(15, 15, 15));
    attach(m_pInclinationJetOar);

    m_pAxialInclinationJetOar = CSWEngine::create(strName + ".AxialInclinationJetOar", 
                                                  Matrix44D(Vector3D(0, 0, 1), 
                                                            Vector3D(0, 1, 0), 
                                                            Vector3D(-1, 0, 0), 
                                                            m_pDynamic->getCenterOfMass() + Vector3D(0, sizeSurface.getHeight()/2, 0)), 
                                                  sizeSurface/Size3D(20, 20, 20));
    attach(m_pAxialInclinationJetOar);


    //add a little offset so that the battery is out of the collision surface. this simplifies the launching of weapons
    m_pFrontLeftWeaponBattery = CSWWeaponBattery::create(strName + ".FrontLeftWeaponBattery", Matrix44D(Vector3D(2, 0, fLength/2.0 + 5)), 
                                                         fLength/2.5, CSWWeaponSupply::GREEN_TORPEDO_ID);
    attach(m_pFrontLeftWeaponBattery);

    //load the weapons
    int nCnt = 0;
    while (!getFrontLeftWeaponBattery()->isFull())
    {
      getFrontLeftWeaponBattery()->insert(GreenTorpedo::create(strName + ".GreenTorpedo" + ARSTD::Utilities::toString(++nCnt), Matrix44D(), 5));
    }


    //add a little offset so that the battery is out of the collision surface. this simplifies the launching of weapons
    m_pFrontRightWeaponBattery = CSWWeaponBattery::create(strName + ".FrontRightWeaponBattery", Matrix44D(Vector3D(-2, 0, fLength/2.0 + 5)), 
                                                          fLength/10, CSWWeaponSupply::RED_TORPEDO_ID | CSWWeaponSupply::BLUE_TORPEDO_ID);
    attach(m_pFrontRightWeaponBattery);

    //load the weapons
    nCnt = 0;
    while (!getFrontRightWeaponBattery()->isFull())
    {
      getFrontRightWeaponBattery()->insert(RedTorpedo::create(strName + ".RedTorpedo" + ARSTD::Utilities::toString(++nCnt), Matrix44D(), 5));
      getFrontRightWeaponBattery()->insert(BlueTorpedo::create(strName + ".BlueTorpedo" + ARSTD::Utilities::toString(nCnt), Matrix44D(), 5));
    }


    //add a little offset so that the battery is out of the collision surface. this simplifies the launching of weapons
    m_pBackWeaponBattery = CSWWeaponBattery::create(strName + ".BackWeaponBattery", Matrix44D(Vector3D(-1, 0, 0), 
                                                                                             Vector3D(0, 1, 0), 
                                                                                             Vector3D(0, 0, -1), 
                                                                                             Vector3D(0, 0, -(fLength/2.0 + 5))), 
                                                    fLength/5, CSWWeaponSupply::GREEN_MINE_ID | CSWWeaponSupply::YELLOW_MINE_ID);
    attach(m_pBackWeaponBattery);

    //load the weapons
    nCnt = 0;
    while (!getBackWeaponBattery()->isFull())
    {
      getBackWeaponBattery()->insert(GreenMine::create(strName + ".GreenMine" + ARSTD::Utilities::toString(++nCnt), Matrix44D(), 1));
      getBackWeaponBattery()->insert(YellowMine::create(strName + ".YellowMine" + ARSTD::Utilities::toString(nCnt), Matrix44D(), 1));
    }
  }


  CSWEngine::PtrType CSWSubmarine::getMainEngine()
  {
    return m_pMainEngine;
  }


  CSWEngine::PtrType CSWSubmarine::getBuoyancyTank()
  {
    return m_pBuoyancyTank;
  }


  CSWEngine::PtrType CSWSubmarine::getBowsJetOar()
  {
    return m_pBowsJetOar;
  }


  CSWEngine::PtrType CSWSubmarine::getInclinationJetOar()
  {
    return m_pInclinationJetOar;
  }


  CSWEngine::PtrType CSWSubmarine::getAxialInclinationJetOar()
  {
    return m_pAxialInclinationJetOar;
  }


  CSWControlCenter::PtrType CSWSubmarine::getControlCenter()
  {
    return m_pControlCenter;
  }


  CSWWeaponBattery::PtrType CSWSubmarine::getFrontLeftWeaponBattery()
  {
    return m_pFrontLeftWeaponBattery;
  }


  CSWWeaponBattery::PtrType CSWSubmarine::getFrontRightWeaponBattery()
  {
    return m_pFrontRightWeaponBattery;
  }


  CSWWeaponBattery::PtrType CSWSubmarine::getBackWeaponBattery()
  {
    return m_pBackWeaponBattery;
  }

}