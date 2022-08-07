// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"

#include "CSWIUpdateable.h"
#include "CSWIEventDealable.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageUpdateProcessEventObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageUpdateProcessEventObjects();
      virtual ~CSWMessageUpdateProcessEventObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageUpdateProcessEventObjects(const CSWMessageUpdateProcessEventObjects& other);
  };



  //implementation

  template<typename ObjectType>
  CSWMessageUpdateProcessEventObjects<ObjectType>::CSWMessageUpdateProcessEventObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageUpdateProcessEventObjects<ObjectType>::~CSWMessageUpdateProcessEventObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageUpdateProcessEventObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    //event handling
    //verteile alle angesammelten events -> stosse jedes objekt an seine empfangenen events zu verarbeiten
    if (CSWIEventDealable::PtrType pObj = std::dynamic_pointer_cast<CSWIEventDealable>(pObject))
    {
      pObj->processReceivedQueuedEvents();
    }

    //updaten (betrifft updateable)
    if (CSWIUpdateable::PtrType pObj = std::dynamic_pointer_cast<CSWIUpdateable>(pObject))
    {
      pObj->update();
    }
  }

}