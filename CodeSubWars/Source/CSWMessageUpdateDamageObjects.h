// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "CSWWorld.h"
#include "CSWIDamageable.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageUpdateDamageObjects : public ARSTD::Message<ObjectType>
  {
    public:
      typedef std::vector<typename ObjectType::PtrType> ObjectContainer;
  
      CSWMessageUpdateDamageObjects();
      virtual ~CSWMessageUpdateDamageObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

      const ObjectContainer& getCollectedObjects() const;

    protected:
      CSWMessageUpdateDamageObjects(const CSWMessageUpdateDamageObjects& other);
    
      ObjectContainer m_ObjectContainer;
  };



  //implementation

  template<typename ObjectType>
  CSWMessageUpdateDamageObjects<ObjectType>::CSWMessageUpdateDamageObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageUpdateDamageObjects<ObjectType>::~CSWMessageUpdateDamageObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageUpdateDamageObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    CSWIDamageable::PtrType pDamageable = std::dynamic_pointer_cast<CSWIDamageable>(pObject);
    if (!pDamageable)
      return;

    if (pDamageable->isDead())
    {
      m_ObjectContainer.push_back(pObject);
    }
  }


  template<typename ObjectType>
  const typename CSWMessageUpdateDamageObjects<ObjectType>::ObjectContainer& CSWMessageUpdateDamageObjects<ObjectType>::getCollectedObjects() const
  {
    return m_ObjectContainer;
  }

}