// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "CSWICollideable.h"
#include "CSWObject.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageUpdateCollisionObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageUpdateCollisionObjects();
      virtual ~CSWMessageUpdateCollisionObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageUpdateCollisionObjects(const CSWMessageUpdateCollisionObjects& other);
  };



  //implementation

  template<typename ObjectType>
  CSWMessageUpdateCollisionObjects<ObjectType>::CSWMessageUpdateCollisionObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageUpdateCollisionObjects<ObjectType>::~CSWMessageUpdateCollisionObjects()
  {
    DT_Test(CSWWorld::getInstance()->getScene(), CSWWorld::getInstance()->getResponseTable());
  }


  template<typename ObjectType>
  void CSWMessageUpdateCollisionObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    CSWICollideable::PtrType pCollideableObject = std::dynamic_pointer_cast<CSWICollideable>(pObject);
    if (!pCollideableObject)
      return;

    pCollideableObject->prepare();
    pCollideableObject->setupTransform();
  }

}