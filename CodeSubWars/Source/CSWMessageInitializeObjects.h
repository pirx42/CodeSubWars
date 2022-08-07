// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"
#include "CSWISolid.h"
#include "CSWWorld.h"
#include "CSWSettings.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageInitializeObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageInitializeObjects();
      virtual ~CSWMessageInitializeObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);
  };



  //implementation

  template<typename ObjectType>
  CSWMessageInitializeObjects<ObjectType>::CSWMessageInitializeObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageInitializeObjects<ObjectType>::~CSWMessageInitializeObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageInitializeObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    pObject->initialize();
  }

}