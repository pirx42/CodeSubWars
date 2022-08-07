// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWObject;

  /**
   * Objects with this property can emit forces to dynamic objects.
   */
  class CSWIForceEmitter
  {
    public:
      typedef std::shared_ptr<CSWIForceEmitter> PtrType;

      typedef std::pair<Vector3D, double> MassPoint;
      typedef std::vector<MassPoint> MassPointContainer; //first mass, second force
      typedef std::pair<MassPointContainer::iterator, MassPointContainer::iterator> MassPointContainerRange;

      virtual ~CSWIForceEmitter() {}

      //signals whether the emitting is active or not
      virtual bool isForceEmitterActive() const = 0;

      //is called before the emission to all objects starts. !every world cycle!
      virtual void initializeForceEmission() = 0;

      //returns the emited force to the given world position
      virtual void emitForce(std::shared_ptr<CSWObject> pObject) = 0;

      //is called after the emission to all objects ended. !every world cycle!
      virtual void finalizeForceEmission() = 0;
  };

}