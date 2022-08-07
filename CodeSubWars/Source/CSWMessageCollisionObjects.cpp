// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMessageCollisionObjects.h"

#include "CSWICollideable.h"
#include "CSWObject.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "CSWBattleStatistics.h"
#include "CSWTrigger.h"

/*
  @todo extract handling of collision in seperate class CollisionHandler with default collision method collide(CSWObject*, CSWObject*). 
  this methode can be specialized for different collision pairs e.g collide(CSWDynamic*, CSWBorder*)
*/

#include "CSWCollisionDetectedMessage.h"
#include "CSWEventManager.h"
#include "CSWEvent.h"


#include "CSWIDynamic.h"
#include "CSWIDamageable.h"


namespace CodeSubWars
{

  CSWMessageDeleteCollisionObjects::CSWMessageDeleteCollisionObjects()
  {
    DT_RemoveDefaultResponse(CSWWorld::getInstance()->getResponseTable(), 
                             &(CSWMessageInitializeCollisionObjects::collide));
  }


  CSWMessageDeleteCollisionObjects::~CSWMessageDeleteCollisionObjects()
  {
  }


  void CSWMessageDeleteCollisionObjects::evaluateObject(CSWObject::PtrType pObject)
  {
    //assumes that all objects that are collidable were attached onto the world tree
    deleteObjectCollision(pObject);
  }


  bool CSWMessageDeleteCollisionObjects::deleteCollision(std::shared_ptr<CSWObject> pObject)
  {
    bool bResult = deleteObjectCollision(pObject);
    if (!bResult)
      return false;

    CSWObject::ChildRange range = pObject->getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        bResult &= deleteCollision(pChild);
    }
  
    return bResult;
  }


  bool CSWMessageDeleteCollisionObjects::deleteObjectCollision(std::shared_ptr<CSWObject> pObject)
  {
    CSWICollideable::PtrType pCollideableObject = std::dynamic_pointer_cast<CSWICollideable>(pObject);
    if (!pCollideableObject)
      return true;

    return pCollideableObject->deleteEnvironment();
  }


  CSWMessageInitializeCollisionObjects::CSWMessageInitializeCollisionObjects()
  {
  //  dtEnableCaching();

    DT_AddDefaultResponse(CSWWorld::getInstance()->getResponseTable(), 
                          &(CSWMessageInitializeCollisionObjects::collide),
                          DT_DEPTH_RESPONSE, NULL);
  }


  CSWMessageInitializeCollisionObjects::~CSWMessageInitializeCollisionObjects()
  {
  }


  void CSWMessageInitializeCollisionObjects::evaluateObject(CSWObject::PtrType pObject)
  {
    initializeCollision(pObject);
  }


  bool CSWMessageInitializeCollisionObjects::initializeCollision(std::shared_ptr<CSWObject> pObject)
  {
    bool bResult = initializeObjectCollision(pObject);
    if (!bResult)
      return false;

    CSWObject::ChildRange range = pObject->getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        bResult &= initializeCollision(pChild);
    }
  
    return bResult;
  }


  bool CSWMessageInitializeCollisionObjects::initializeObjectCollision(std::shared_ptr<CSWObject> pObject)
  {
    CSWICollideable::PtrType pCollideableObject = std::dynamic_pointer_cast<CSWICollideable>(pObject);
    if (!pCollideableObject)
      return true;

    pCollideableObject->initializeEnvironment(true);
    pCollideableObject->prepare();
    pCollideableObject->setupTransform();
    return true;
  }


  DT_Bool CSWMessageInitializeCollisionObjects::collide(void* client_data, void* pObj1, void* pObj2,
                                                        const DT_CollData *coll_data)
  {
    if (!pObj1 || !pObj2)
      return DT_CONTINUE;
    CSWObject* pA = reinterpret_cast<CSWObject*>(pObj1);
    CSWObject* pB = reinterpret_cast<CSWObject*>(pObj2);
    assert(pA && pB);
    if (!pA || !pB)
      return DT_CONTINUE;

    CSWObject::PtrType pObjectA = pA->getSharedThis();
    CSWObject::PtrType pObjectB = pB->getSharedThis();
    //check if the objects exists. it could be possible that objects died and therefor removed while collision detection. 
    //to prevent solving collision of removed objects break here.
    if (!pObjectA || !pObjectB)
      return DT_CONTINUE;

    CSWICollideable::PtrType pCollideableObjA = std::dynamic_pointer_cast<CSWICollideable>(pObjectA);
    CSWICollideable::PtrType pCollideableObjB = std::dynamic_pointer_cast<CSWICollideable>(pObjectB);
    if (!pCollideableObjA || !pCollideableObjB)
      throw std::runtime_error("tried to solve collision for non collidable objects");

    if (std::dynamic_pointer_cast<CSWTrigger>(pObjectA) && 
        std::dynamic_pointer_cast<CSWTrigger>(pObjectB))
    {
      if (CSWWorld::getInstance()->getSettings()->getCollidableProperties() & CSWSettings::DISPLAY_TRIGGER_TRIGGER_COLLISION)
      {
        pCollideableObjA->setHit(true);
        pCollideableObjB->setHit(true);
      }
      return DT_CONTINUE;  
    }
  

    Vector3D vecWorldTP1(coll_data->point1[0], coll_data->point1[1], coll_data->point1[2]);
    Vector3D vecWorldTP2(coll_data->point2[0], coll_data->point2[1], coll_data->point2[2]);

    Vector3D vecObjectATP1 = pObjectA->getInvertWorldTransform()*vecWorldTP1;
    Vector3D vecObjectBTP2 = pObjectB->getInvertWorldTransform()*vecWorldTP2;

    //given normal is directed away from ObjectB
    Vector3D vecWorldTP2ToP1(coll_data->normal[0], coll_data->normal[1], coll_data->normal[2]);
    vecWorldTP2ToP1 = -vecWorldTP2ToP1;
    double fPenetrationDepth = vecWorldTP2ToP1.getLength();
    vecWorldTP2ToP1 /= fPenetrationDepth;

    if (fPenetrationDepth < EPSILON)
      return DT_CONTINUE;


    if (CSWTrigger::PtrType pTrigger = std::dynamic_pointer_cast<CSWTrigger>(pObjectA))
    {
      if (CSWWorld::getInstance()->getSettings()->getCollidableProperties() & CSWSettings::DISPLAY_OBJECT_TRIGGER_COLLISION)
      {
        pCollideableObjA->setHit(true);
        pCollideableObjB->setHit(true);
      }
      pTrigger->detectCollision(pObjectB, vecObjectATP1);
      return DT_CONTINUE;
    }
    else if (CSWTrigger::PtrType pTrigger = std::dynamic_pointer_cast<CSWTrigger>(pObjectB))
    {
      if (CSWWorld::getInstance()->getSettings()->getCollidableProperties() & CSWSettings::DISPLAY_OBJECT_TRIGGER_COLLISION)
      {
        pCollideableObjA->setHit(true);
        pCollideableObjB->setHit(true);
      }
      pTrigger->detectCollision(pObjectA, vecObjectBTP2);
      return DT_CONTINUE;
    }


    if (CSWWorld::getInstance()->getSettings()->getCollidableProperties() & CSWSettings::DISPLAY_OBJECT_OBJECT_COLLISION)
    {
      pCollideableObjA->setHit(true);
      pCollideableObjB->setHit(true);
    }

  

    CSWWorld::getInstance()->getBattleStatistics()->reportCollision(pObjectA, pObjectB);
    

    //calculate j depend on the objects properties
    Vector3D vecRAP(0, 0, 0);
    Vector3D vecRBP(0, 0, 0);
    Vector3D vecVelocityA(0, 0, 0);
    Vector3D vecVelocityB(0, 0, 0);
    Vector3D vecTmpAngularVelocityStuffA(0, 0, 0);
    Vector3D vecTmpAngularVelocityStuffB(0, 0, 0);
    double fMassARez(0);
    double fMassBRez(0);

    CSWIDynamic::PtrType pDynObjA = std::dynamic_pointer_cast<CSWIDynamic>(pObjectA);
    CSWIDynamic::PtrType pDynObjB = std::dynamic_pointer_cast<CSWIDynamic>(pObjectB);
        
    if (pDynObjA)
    {
      vecRAP = pObjectA->getWorldTransform().getRotationAsMatrix33()*(vecObjectATP1 - pDynObjA->getCenterOfMass());
      vecVelocityA = pDynObjA->getVelocity() + pDynObjA->getAngularVelocity()%vecRAP;
      vecTmpAngularVelocityStuffA = (pDynObjA->getInvertedMomentOfIntertia()*(vecRAP % vecWorldTP2ToP1)) % vecRAP;
      fMassARez = 1.0/pDynObjA->getTotalMass();
    }

    if (pDynObjB)
    {
      vecRBP = pObjectB->getWorldTransform().getRotationAsMatrix33()*(vecObjectBTP2 - pDynObjB->getCenterOfMass());
      vecVelocityB = pDynObjB->getVelocity() + pDynObjB->getAngularVelocity()%vecRBP;
      vecTmpAngularVelocityStuffB = (pDynObjB->getInvertedMomentOfIntertia()*(vecRBP % vecWorldTP2ToP1)) % vecRBP;
      fMassBRez = 1.0/pDynObjB->getTotalMass();
    }


    Vector3D vecVelocityAB = vecVelocityA - vecVelocityB;

  //  double fFriction = 0.5;

    double fRestitution = 1;
    double fContactThreshold = 10.0;
    double fNormalVelocityAB = vecWorldTP2ToP1*vecVelocityAB;
    //elastic factor e: 1 = total elastic (superball); 0 = total plastic (clay)
    double e = fRestitution*std::max(0.0, std::min(1.0, fNormalVelocityAB/fContactThreshold + fPenetrationDepth*3));

    //impuls value is depend on difference velocity of impuls direction and penetration depth to prevent
    //interpenetration
    double j = -(1.0 + e)*(fNormalVelocityAB + fPenetrationDepth*3)/
               (vecWorldTP2ToP1*vecWorldTP2ToP1*(fMassARez + fMassBRez) + 
                (vecTmpAngularVelocityStuffA + vecTmpAngularVelocityStuffB)*vecWorldTP2ToP1);



    double fFactorMassAToMassB = 1;
    if (pDynObjA && pDynObjB)
      fFactorMassAToMassB = pDynObjA->getTotalMass()/pDynObjB->getTotalMass();

    double fMaxDamage = 0.1;
    double fDamage = fMaxDamage*std::max(0.0, std::min(1.0, fNormalVelocityAB/15.0));

    //send events to inform the objects  
    CSWCollisionDetectedMessage::PtrType pMessageToObj1 = CSWCollisionDetectedMessage::create(vecWorldTP1, fFactorMassAToMassB < 500 ? fDamage : 0);
    CSWEventManager::getInstance()->send(CSWEvent::create(pObjectB, pObjectA, pMessageToObj1, 1));
    CSWCollisionDetectedMessage::PtrType pMessageToObj2 = CSWCollisionDetectedMessage::create(vecWorldTP2, 1/fFactorMassAToMassB < 500 ? fDamage : 0);
    CSWEventManager::getInstance()->send(CSWEvent::create(pObjectA, pObjectB, pMessageToObj2, 1));
  
    if (CSWIDamageable::PtrType pDamObjA = std::dynamic_pointer_cast<CSWIDamageable>(pObjectA))
    {
      if (pDamObjA->isAlive()) 
      {
        if (fFactorMassAToMassB < 500)
          pDamObjA->addDamage(fDamage);
        if (!pDamObjA->isAlive())
          CSWWorld::getInstance()->getBattleStatistics()->reportKill(pObjectA, pObjectA);
      }
    }
    if (CSWIDamageable::PtrType pDamObjB = std::dynamic_pointer_cast<CSWIDamageable>(pObjectB))
    {
      if (pDamObjB->isAlive())
      {
        if (1/fFactorMassAToMassB < 500)
          pDamObjB->addDamage(fDamage);
        if (!pDamObjB->isAlive())
          CSWWorld::getInstance()->getBattleStatistics()->reportKill(pObjectB, pObjectB);
      }
    }



    if (pDynObjA)
    {
      pDynObjA->addImpuls(vecWorldTP1, vecWorldTP2ToP1*j);

  //     //friction to object A
  //     double fNormalVelocity = vecWorldTP2ToP1*vecVelocityA;
  //     Vector3D vecLatVelocity = vecVelocityA - vecWorldTP2ToP1*fNormalVelocity;
  //     double fLatVelocity = vecLatVelocity.getLength();
  //     if (fLatVelocity > GRANULARITY)
  //     {
  //       vecLatVelocity /= fLatVelocity;
  //       Vector3D vecTmp = pDynObjA->getInvertedMomentOfIntertia()*(vecObjectATP1 % vecLatVelocity);
  //       double fFrictionImpuls = -fLatVelocity/(fMassARez + vecLatVelocity*(vecTmp % vecObjectATP1));
  //       fFrictionImpuls = std::min(fFrictionImpuls, j*fFriction);
  //       pDynObjA->addImpuls(vecWorldTP1, vecLatVelocity*fFrictionImpuls);
  //     }
    }
  
    if (pDynObjB)
    {
      pDynObjB->addImpuls(vecWorldTP2, vecWorldTP2ToP1*-j);

  //     //friction to object B
  //     double fNormalVelocity = vecWorldTP2ToP1*vecVelocityB;
  //     Vector3D vecLatVelocity = vecVelocityB - vecWorldTP2ToP1*fNormalVelocity;
  //     double fLatVelocity = vecLatVelocity.getLength();
  //     if (fLatVelocity > GRANULARITY)
  //     {
  //       vecLatVelocity /= fLatVelocity;
  //       Vector3D vecTmp = pDynObjB->getInvertedMomentOfIntertia()*(vecObjectBTP2 % vecLatVelocity);
  //       double fFrictionImpuls = -fLatVelocity/(fMassBRez + vecLatVelocity*(vecTmp % vecObjectBTP2));
  //       fFrictionImpuls = std::min(fFrictionImpuls, j*fFriction);
  //       pDynObjB->addImpuls(vecWorldTP2, vecLatVelocity*fFrictionImpuls);
  //     }
    }
    return DT_CONTINUE;
  }

}