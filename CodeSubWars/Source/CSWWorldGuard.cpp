// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorldGuard.h"
#include "CSWWorld.h"
#include "CSWLog.h"
#include "CSWObject.h"

#include "CSWMessageCollectObjects.h"
#include "CSWMessageCollisionObjects.h"

#include "CSWEventManager.h"
#include "CSWEvent.h"
#include "CSWSystemMessage.h"

#include "CSWSubmarine.h"

#include "CSWIDynamic.h"
#include "CSWIPythonable.h"
#include "PythonContext.h"


namespace CodeSubWars
{

  double CSWWorldGuard::CRITICAL_OUTSIDE_TIME = 90;


  CSWWorldGuard::PtrType CSWWorldGuard::create()
  {
    return PtrType(new CSWWorldGuard());
  }


  CSWWorldGuard::~CSWWorldGuard()
  {
  }


  void CSWWorldGuard::update()
  {
    //check whether objects are outside of world
    {
      typedef CSWMessageCollectObjects1<CSWObject, CSWIDynamic> MessageCollectDynamic;
      MessageCollectDynamic collectMessage;
      MessageCollectDynamic::broadcastMessage(CSWWorld::getInstance()->getObjectTree(), collectMessage, 1);
      OutsideObjectContainer newOutsideObjects;
      MessageCollectDynamic::ObjectContainer dynamicObjects = collectMessage.getCollectedObjects()[0];
      MessageCollectDynamic::ObjectContainer::iterator itDynObj = dynamicObjects.begin();
      for (; itDynObj != dynamicObjects.end(); ++itDynObj)
      {
        if (CSWWorld::getInstance()->isOutside(*itDynObj))
        {
          OutsideObjectContainer::iterator itFoundOldOutsideObj = m_ObjectsOutsideWorld.find(*itDynObj);
          if (itFoundOldOutsideObj == m_ObjectsOutsideWorld.end())
          {
            //current outside object is first time outside -> mark and send warning
            newOutsideObjects[*itDynObj] = ARSTD::Time::getTime();
            CSWSystemMessage::PtrType pMessage = CSWSystemMessage::create("get immediately inside the world within " + 
                                                                          ARSTD::Utilities::toString(CRITICAL_OUTSIDE_TIME) + "s or get kicked");
            CSWEventManager::getInstance()->send(CSWEvent::createAnonymous(*itDynObj, pMessage, 1));
          }
          else
          {
            //current outside object was already outside before
            if (ARSTD::Time::getTime() - itFoundOldOutsideObj->second < CRITICAL_OUTSIDE_TIME)
            {
              //timout is not over -> keep
              newOutsideObjects.insert(*itFoundOldOutsideObj);
            }
            else
            {
              //timout -> kick object
              addErroneousObject(itFoundOldOutsideObj->first, CSWWorldGuard::FATAL);
              CSWLog::getInstance()->log(itFoundOldOutsideObj->first->getName() + 
                                        " is outside world for more than " + 
                                        ARSTD::Utilities::toString(CRITICAL_OUTSIDE_TIME) + "s");
            }
          }
        }
      }
      m_ObjectsOutsideWorld.swap(newOutsideObjects);
    }


  
    //remove erroneous objects
    std::vector<CSWObject::PtrType> objectsToKick(getObjectsToKick(10));
    std::vector<CSWObject::PtrType>::iterator itObjectToKick = objectsToKick.begin();
    for (; itObjectToKick != objectsToKick.end(); ++itObjectToKick)
    {
      CSWLog::getInstance()->log(std::string()+ "kick object " + (*itObjectToKick)->getName());  
      bool bResult = CSWMessageDeleteCollisionObjects::deleteCollision(*itObjectToKick);  
      (*itObjectToKick)->finalize();
      bResult &= CSWWorld::getInstance()->getObjectTree()->detach(*itObjectToKick);
      if (!bResult)
        throw std::runtime_error((*itObjectToKick)->getName() + ": is erroneous and could not be correctly removed from world");
      removeErroneousObject(*itObjectToKick);

      //PyThreadState* state = NULL;
      PythonContext::PtrType pyContext;
      if (CSWIPythonable::PtrType pPyObj = std::dynamic_pointer_cast<CSWIPythonable>(*itObjectToKick))
      {
        //state = pPyObj->getThreadState();
        pyContext = pPyObj->getPythonContext();
      }
      if (pyContext)
      {
        auto lck = pyContext->makeCurrent();
        itObjectToKick->reset();
      }
      //if (state)
      //{
      //  //gil must not be held
      //  PyEval_AcquireThread(state);
      //  //gil held

      //  itObjectToKick->reset();

      //  Py_EndInterpreter(state);
      //  //gil held

      //  PyThreadState_Swap(CSWWorld::getInstance()->getPyMainState());
      //  //gil must be held
      //  PyEval_ReleaseThread(CSWWorld::getInstance()->getPyMainState());
      //  //gil not held
      //}
    }

    determineBattleState();
  }


  void CSWWorldGuard::addErroneousObject(CSWObject::PtrType pObject, ErrorType type)
  {
    double fErrorPoints = 0;
    if (type == CRITICAL)
      fErrorPoints = 1;
    else if (type == FATAL)
      fErrorPoints = 100;
  
    CriticalObjectContainer::iterator itFound = m_CriticalObjects.find(pObject);
    if (itFound != m_CriticalObjects.end())
      itFound->second += fErrorPoints;
    else
      m_CriticalObjects[pObject] = fErrorPoints;
  }


  void CSWWorldGuard::addErroneousObject(const std::string& strName, ErrorType type)
  {
    CSWObject::PtrType pObject = std::dynamic_pointer_cast<CSWObject>(ARSTD::Element::findElement(CSWWorld::getInstance()->getObjectTree(), strName));
    if (pObject)
      addErroneousObject(pObject, type);
  }


  void CSWWorldGuard::removeErroneousObject(CSWObject::PtrType pObject)
  {
    CriticalObjectContainer::iterator itFound = m_CriticalObjects.find(pObject);
    if (itFound != m_CriticalObjects.end())
      m_CriticalObjects.erase(itFound);
  }


  bool CSWWorldGuard::isBattleRunning() const
  {
    return m_bBattleRunning;
  }


  void CSWWorldGuard::clear()
  {
    m_CriticalObjects.clear();
    m_ObjectsOutsideWorld.clear();
    m_bBattleRunning = false;
  }


  CSWWorldGuard::CSWWorldGuard()
  : m_bBattleRunning(false)
  {
  }


  std::vector<CSWObject::PtrType> CSWWorldGuard::getObjectsToKick(double fErrorThreshold)
  {
    std::vector<CSWObject::PtrType> objects;
    CriticalObjectContainer::const_iterator it = m_CriticalObjects.begin();
    for (; it != m_CriticalObjects.end(); ++it)
    {
      if (it->second >= fErrorThreshold)
      {
        objects.push_back(it->first);
      }
    }
    return objects;
  }


  void CSWWorldGuard::determineBattleState()
  {
    std::map<int, int> teams; //team id, number of member
    ARSTD::Node::ChildRange range = CSWWorld::getInstance()->getObjectTree()->getChildRange();
    ARSTD::Node::ChildIterator itChild = range.first;
    for (; itChild != range.second; ++itChild)
    {
      if (CSWSubmarine::PtrType pSubmarine = std::dynamic_pointer_cast<CSWSubmarine>(*itChild))
      {
        std::map<int, int>::iterator itFound = teams.find(pSubmarine->getTeamID());
        if (itFound != teams.end())
        {
          ++(itFound->second);
        }
        else
        {
          teams[pSubmarine->getTeamID()] = 1;
        }
      }
    }

    m_bBattleRunning = teams.size() > 1;
                       //ARSTD::Time::getTime() < 10;
  }

}