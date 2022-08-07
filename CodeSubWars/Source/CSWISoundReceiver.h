// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property receive sounds emitted by sound emitter.
   */
  class CSWISoundReceiver
  {
    public:
      typedef std::shared_ptr<CSWISoundReceiver> PtrType;

      virtual ~CSWISoundReceiver() {}
    
      //indicates in which direction the listener is listen in world coordinate system
      virtual const Vector3D& getDirection() const = 0;

      //returns the maximal angle to the listening direction in degree within objects are listened
      virtual const double& getAngleOfBeam() const = 0;
      virtual void setAngleOfBeam(const double& fAngle) = 0;
    
      virtual void addSound(const Vector3D& vecWorldTPosEmitter, const double& fLevel) = 0;

      virtual const double& getLevel() const = 0;

      //resets all stored sounds
      virtual void reset() = 0;

  };

}