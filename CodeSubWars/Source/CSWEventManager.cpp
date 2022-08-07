// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWObject.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"
#include "CSWIEventDealable.h"
#include "CSWMessageReceiveEventObjects.h"
#include "CSWWorld.h"


namespace CodeSubWars
{

  CSWEventManager::PtrType CSWEventManager::getInstance()
  {
    static PtrType pInstance = PtrType(new CSWEventManager());
    return pInstance;
  }


  void CSWEventManager::send(CSWEvent::PtrType pEvent)
  {
    if (pEvent)
    {
      if (!pEvent->getReceiver() ||
          std::dynamic_pointer_cast<CSWIEventDealable>(pEvent->getReceiver()))
      {    
        pEvent->m_fSendTime = ARSTD::Time::getTime();
        m_EventContainer.push_back(pEvent);
      }
    }
  }


  void CSWEventManager::deliverAllEvents()
  {
    EventContainer::iterator it = m_EventContainer.begin();
    for (; it != m_EventContainer.end(); ++it) 
    {
      if (!(*it)->hasExpired())
      {
        CSWObject::PtrType pObject = (*it)->getReceiver();
        if (!pObject)
        {
          //receiver is not defined -> broadcast message to ALL objects in world tree
          CSWMessageReceiveEventObjects<CSWObject> receiveEventMessage(*it);
          ARSTD::Message<CSWObject>::broadcastMessage(CSWWorld::getInstance()->getObjectTree(), receiveEventMessage);          
        }
        else if (CSWIEventDealable::PtrType pEventDealable = std::dynamic_pointer_cast<CSWIEventDealable>(pObject))
        {
          pEventDealable->receiveEvent(*it);
        }
      }
    }
    m_EventContainer.clear();
  }


  CSWEventManager::CSWEventManager()
  {
  }

}