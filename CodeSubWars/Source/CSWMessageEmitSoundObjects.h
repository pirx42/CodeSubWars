// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"
#include "CSWISoundEmitter.h"
#include "CSWISoundReceiver.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageEmitSoundObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageEmitSoundObjects(std::vector<std::shared_ptr<CSWObject> >& soundReceiverObjectContainer);
      virtual ~CSWMessageEmitSoundObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageEmitSoundObjects(const CSWMessageEmitSoundObjects& other);

      std::vector<std::shared_ptr<CSWObject> >& m_SoundReceiverObjectContainer;
  };



  //implementation

  template<typename ObjectType>
  CSWMessageEmitSoundObjects<ObjectType>::CSWMessageEmitSoundObjects(std::vector<std::shared_ptr<CSWObject> >& soundReceiverObjectContainer)
  : m_SoundReceiverObjectContainer(soundReceiverObjectContainer)
  {
  }


  template<typename ObjectType>
  CSWMessageEmitSoundObjects<ObjectType>::~CSWMessageEmitSoundObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageEmitSoundObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    //the user can access to sound receiver, therefore all receiver must be updated before the uses can have access
    //emit to sound receiver
    CSWISoundEmitter::PtrType pSoundEmitter = std::dynamic_pointer_cast<CSWISoundEmitter>(pObject);
    if (pSoundEmitter && pSoundEmitter->isSoundEmitterActive())
    {
      pSoundEmitter->initializeSoundEmission();
      std::vector<std::shared_ptr<CSWObject> >::iterator it = m_SoundReceiverObjectContainer.begin();    
      for (; it != m_SoundReceiverObjectContainer.end(); ++it)
      {
        if (pObject != *it)
        {
          assert(std::dynamic_pointer_cast<CSWISoundReceiver>(*it));
          pSoundEmitter->emitSound(*it);
        }
      }
      pSoundEmitter->finalizeSoundEmission();
    }
  }

}