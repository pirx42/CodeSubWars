// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "boost/shared_ptr.hpp"

#include "arstd/Message/Message.h"

#include "CSWEquipment.h"


template<typename ObjectType>
class CSWMessageUpdateEquipmentObjects : public ARSTD::Message<ObjectType>
{
  public:
    CSWMessageUpdateEquipmentObjects();
    virtual ~CSWMessageUpdateEquipmentObjects();

    void evaluateObject(typename ObjectType::PtrType pObject);

  private:
    CSWMessageUpdateEquipmentObjects(const CSWMessageUpdateEquipmentObjects& other);
};



//implementation

template<typename ObjectType>
CSWMessageUpdateEquipmentObjects<ObjectType>::CSWMessageUpdateEquipmentObjects()
{
}


template<typename ObjectType>
CSWMessageUpdateEquipmentObjects<ObjectType>::~CSWMessageUpdateEquipmentObjects()
{
}


template<typename ObjectType>
void CSWMessageUpdateEquipmentObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
{
  CSWEquipment::PtrType pEquipment = boost::shared_dynamic_cast<CSWEquipment>(pObject);
  if (!pEquipment) return;

  pEquipment->update();
}
