// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWObject;

  class CSWCollideable
  {
    public:
      typedef std::shared_ptr<CSWCollideable> PtrType;

      static PtrType create(const Mesh::PtrType pCollisionMesh);
      ~CSWCollideable();

      void draw(const Matrix44D& mat);

      //given in locale coordsystem
      const Mesh::PtrType getMesh() const;
      const BoundingBoxD& getBoundingBox() const;

      //given in locale coordsystem
      void setMesh(const Mesh::PtrType pCollisionMesh);

      void initializeEnvironment(DT_ShapeHandle hDTShape, DT_ObjectHandle hDTObject,
                                 const Matrix44D& matObjectTCollisionShape, Mesh::PtrType pCollisionMesh);
      void prepare();
      void setupTransform(std::shared_ptr<CSWObject> pObject);
      const Matrix44D& getObjectTCollisionShape();    
      void setHit(bool bHit);
      bool deleteEnvironment();

    protected:
      CSWCollideable(const Mesh::PtrType pCollisionMesh);

      Mesh::PtrType m_pCollisionMesh;
      BoundingBoxD m_BoundingBox;
      bool m_bInitialized;
      DT_ShapeHandle m_hDTShape;
      DT_ObjectHandle m_hDTObject;
      bool m_bHit;
      Matrix44D m_matObjectTCollisionShape;
  };

}