// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * Objects with this property can emit impulses to dynamic objects.
   */
  class CSWIImpulsEmitter
  {
    public:
      typedef std::shared_ptr<CSWIImpulsEmitter> PtrType;

      virtual ~CSWIImpulsEmitter() {}

      //signals whether the emitting is active or not
      virtual bool isImpulsEmitterActive() const = 0;

      //is called before the emission to all objects starts. !every world cycle!
      virtual void initializeImpulsEmission() = 0;

      //returns the emited impuls to the given world position
      virtual void emitImpuls(std::shared_ptr<CSWObject> pObject) = 0;

      //is called after the emission to all objects ended. !every world cycle!
      virtual void finalizeImpulsEmission() = 0;
  };

}