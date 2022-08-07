// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWObject.h"

#include "CSWISolid.h"
#include "CSWICollideable.h"

namespace CodeSubWars
{

  class CSWSolid;
  class CSWCollideable;

  /** 
   * This class represents a wall. A wall is not dynamic, i.e. it can not move or be moved within the world. Also its not possible to
   * destroy walls.
   */
  class CSWWall : public CSWObject,
                  public CSWISolid,
                  public CSWICollideable
  {
    public:
      typedef std::shared_ptr<CSWWall> PtrType;

      virtual ~CSWWall();

      //defined methods for solid
      virtual void draw();
      virtual const Mesh::PtrType getSurface() const;
      virtual const BoundingBoxD& getSurfaceBoundingBox() const;
      virtual void setColor(const Vector4D& vecColor);
      virtual const Vector4D& getColor();

      //defined methods for collideable
      virtual const Mesh::PtrType getCollisionMesh() const;
      virtual const BoundingBoxD& getCollisionBoundingBox() const;
      virtual void initializeEnvironment(bool bSimplified);
      virtual void prepare();
      virtual void setupTransform();
      virtual const Matrix44D& getObjectTCollisionShape();
      virtual void setHit(bool bHit);
      virtual bool deleteEnvironment();

    protected:
      CSWWall(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
    
      //properties
      std::shared_ptr<CSWSolid> m_pSolid;
      std::shared_ptr<CSWCollideable> m_pCollideable;

      //other stuff
  };

}