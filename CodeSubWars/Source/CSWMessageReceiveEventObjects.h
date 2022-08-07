// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "CSWIEventDealable.h"
#include "CSWEvent.h"

namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageReceiveEventObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageReceiveEventObjects(CSWEvent::PtrType pEvent);
      virtual ~CSWMessageReceiveEventObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageReceiveEventObjects(const CSWMessageReceiveEventObjects& other);
    
      CSWEvent::PtrType m_pEvent;
  };



  //implementation

  template<typename ObjectType>
  CSWMessageReceiveEventObjects<ObjectType>::CSWMessageReceiveEventObjects(CSWEvent::PtrType pEvent)
  : m_pEvent(pEvent)
  {
  }


  template<typename ObjectType>
  CSWMessageReceiveEventObjects<ObjectType>::~CSWMessageReceiveEventObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageReceiveEventObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    CSWIEventDealable::PtrType pEventDealableObject = std::dynamic_pointer_cast<CSWIEventDealable>(pObject);
    if (!pEventDealableObject)
      return;

    if (m_pEvent->getSender() != pObject)
    {
      CSWEvent::PtrType pCopiedEvent = m_pEvent->copy();
      pCopiedEvent->setReceiver(pObject);
      pEventDealableObject->receiveEvent(pCopiedEvent);
    }
  }

}