// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property can collide with other objects having this property. 
   */
  class CSWICollideable 
  {
    public:
      typedef std::shared_ptr<CSWICollideable> PtrType;

      virtual ~CSWICollideable() {}

      //given in locale coordsystem
      virtual const Mesh::PtrType getCollisionMesh() const = 0;
      virtual const BoundingBoxD& getCollisionBoundingBox() const = 0;
    
      virtual void initializeEnvironment(bool bSimplified) = 0;
      virtual void prepare() = 0;
      virtual void setupTransform() = 0;
      virtual const Matrix44D& getObjectTCollisionShape() = 0;
      virtual void setHit(bool bHit) = 0;
      virtual bool deleteEnvironment() = 0;
  };

}