// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"

#include "CSWICommandable.h"

#include "CSWIForceEmitter.h"
#include "CSWIImpulsEmitter.h"

#include "CSWIDynamic.h"

#include "CSWIResourceProvider.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageRecalculateObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageRecalculateObjects(std::vector<std::shared_ptr<CSWObject> >& dynamicObjectContainer);
      virtual ~CSWMessageRecalculateObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageRecalculateObjects(const CSWMessageRecalculateObjects& other);

      std::vector<std::shared_ptr<CSWObject> >& m_DynamicObjectContainer;
  };



  //implementation

  template<typename ObjectType>
  CSWMessageRecalculateObjects<ObjectType>::CSWMessageRecalculateObjects(std::vector<std::shared_ptr<CSWObject> >& dynamicObjectContainer)
  : m_DynamicObjectContainer(dynamicObjectContainer)
  {
  }


  template<typename ObjectType>
  CSWMessageRecalculateObjects<ObjectType>::~CSWMessageRecalculateObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageRecalculateObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    //selbst initiierte krafte (betrifft commandable)
    if (CSWICommandable::PtrType pObj = std::dynamic_pointer_cast<CSWICommandable>(pObject))
    {
      pObj->step();
    }

    //emit to dynamic
    CSWIForceEmitter::PtrType pForceEmitter = std::dynamic_pointer_cast<CSWIForceEmitter>(pObject);
    CSWIImpulsEmitter::PtrType pImpulsEmitter = std::dynamic_pointer_cast<CSWIImpulsEmitter>(pObject);
    bool bEmitForce = pForceEmitter ? pForceEmitter->isForceEmitterActive() : false;
    bool bEmitImpuls = pImpulsEmitter ? pImpulsEmitter->isImpulsEmitterActive() : false;
    if (bEmitForce || bEmitImpuls)
    {
      if (bEmitForce)
        pForceEmitter->initializeForceEmission();
      if (bEmitImpuls)
        pImpulsEmitter->initializeImpulsEmission();
      std::vector<std::shared_ptr<CSWObject> >::iterator it = m_DynamicObjectContainer.begin();    
      for (; it != m_DynamicObjectContainer.end(); ++it)
      {
        if (pObject != *it)
        {
          assert(std::dynamic_pointer_cast<CSWIDynamic>(*it));
          if (bEmitForce)
            pForceEmitter->emitForce(*it);
          if (bEmitImpuls)
            pImpulsEmitter->emitImpuls(*it);
        }
      }
      if (bEmitForce)
        pForceEmitter->finalizeForceEmission();
      if (bEmitImpuls)
        pImpulsEmitter->finalizeImpulsEmission();
    }

    //updating resource provider
    if (CSWIResourceProvider::PtrType pObj = std::dynamic_pointer_cast<CSWIResourceProvider>(pObject))
    {
      pObj->updateRecharging();
    }

    //aktualisierung der position (betrifft dynamic)
    if (CSWIDynamic::PtrType pObj = std::dynamic_pointer_cast<CSWIDynamic>(pObject))
    {
      pObj->updatePosition();
    }
  }

}