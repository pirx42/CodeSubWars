// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWObject.h"

#include "CSWICollideable.h"


namespace CodeSubWars
{

  class CSWCollideable;

  // the base class for tirgger objects
  class CSWTrigger : public CSWObject,
                     public CSWICollideable
  {
    public:
      typedef std::shared_ptr<CSWTrigger> PtrType;

      virtual ~CSWTrigger();

      void draw();

      //defined methods for collideable
      virtual const Mesh::PtrType getCollisionMesh() const;
      virtual const BoundingBoxD& getCollisionBoundingBox() const;
      virtual void initializeEnvironment(bool bSimplified);
      virtual void prepare();
      virtual void setupTransform();
      virtual const Matrix44D& getObjectTCollisionShape();
      virtual void setHit(bool bHit);
      virtual bool deleteEnvironment();

      //is called if a collision with another object (not trigger) is detected. vecObjectTPosition is in coordinate 
      //system of trigger not detected object.
      virtual void detectCollision(std::shared_ptr<CSWObject> pObject, const Vector3D& vecObjectTPosition);

    protected: 
      CSWTrigger(const std::string& strName, const Matrix44D& matBaseTObject,
                 Mesh::PtrType pCollisionMesh);

      //property implementations
      std::shared_ptr<CSWCollideable> m_pCollideable;
    
  };

}