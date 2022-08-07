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
   * This class represents a border. This is only used to surround the world to prevent objects to escape.
   */
  class CSWBorder : public CSWObject,
                    public CSWISolid,
                    public CSWICollideable
  {
    public:
      typedef std::shared_ptr<CSWBorder> PtrType;

      virtual ~CSWBorder();

      //defined methods for solid
      virtual void draw();
      virtual const Mesh::PtrType getSurface() const;
      virtual const BoundingBoxD& getSurfaceBoundingBox() const;
      virtual void setColor(const Vector4D& vecColor);
      virtual const Vector4D& CSWBorder::getColor();

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
      CSWBorder(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      //properties
      std::shared_ptr<CSWSolid> m_pSolid;
      std::shared_ptr<CSWCollideable> m_pCollideable;

      //other stuff
  };

}