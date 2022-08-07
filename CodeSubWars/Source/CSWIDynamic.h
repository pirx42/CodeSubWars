// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * Objects with this property are able to move/or rotate in the world. That means not that they have a machine and 
   * can move forward. This property only allows an object a possibility to move/rotate. If an object has no equipment 
   * for moving but this property it can be moved/rotated by global forces, by collisions with other objects, etc. Objects 
   * that have not this property are also called as "static".
   */ 
  class CSWIDynamic
  {
    public:
      typedef std::shared_ptr<CSWIDynamic> PtrType;
      typedef std::vector<std::pair<Vector3D, Vector3D> > ForceContainer;
      typedef std::pair<Vector3D, double> MassPoint; //position and mass
      typedef std::vector<MassPoint> MassPointContainer;
      typedef std::pair<MassPointContainer::iterator, MassPointContainer::iterator> MassPointContainerRange;

      virtual ~CSWIDynamic() {}

      virtual void updatePosition() = 0;

      //in world coordsystem
      virtual void resetForce() = 0;
    
      virtual void addForceToCM(const Vector3D& vecWorldTForce) = 0;
      virtual void addForce(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTForce) = 0;

      virtual void addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls) = 0;

      //in world system
      virtual const Vector3D& getVelocity() const = 0;
      virtual const Vector3D& getAcceleration() const = 0;
      virtual const Vector3D& getAngularVelocity() const = 0;
      virtual Vector3D getAngularVelocity(const Vector3D& vecObjectTPosition) const = 0;
      virtual const Vector3D& getAngularAcceleration() const = 0;
      virtual Vector3D getAngularAcceleration(const Vector3D& vecObjectTPosition) const = 0;
      virtual const Vector3D& getAngularMomentum() const = 0;
      virtual const Matrix33D& getInvertedMomentOfIntertia() const = 0;

      virtual const double& getTotalMass() const = 0;

      virtual void setNewTransform(const Matrix44D& matWorldTObject) = 0;
      virtual void setNewVelocity(const Vector3D& vecWorldTVelocityCM,
                                  const Vector3D& vecWorldTAngularMomentumCM) = 0;

      //given in local coordsystem
      virtual const Vector3D& getCenterOfMass() const = 0;
      virtual MassPointContainerRange getMassPointRange() = 0;
  };

}