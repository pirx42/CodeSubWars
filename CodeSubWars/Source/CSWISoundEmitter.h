// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property are able to emit sounds to sound receiver.
   */
  class CSWISoundEmitter
  {
    public:
      typedef std::shared_ptr<CSWISoundEmitter> PtrType;

      virtual ~CSWISoundEmitter() {}

      //signals whether the emitting is active or not
      virtual bool isSoundEmitterActive() const = 0;

      //is called before the emission to all objects starts. !every world cycle!
      virtual void initializeSoundEmission() = 0;

      //emits sound to the given object
      virtual void emitSound(std::shared_ptr<CSWObject> pObject) = 0;

      //is called after the emission to all objects ended. !every world cycle!
      virtual void finalizeSoundEmission() = 0;
  };

}