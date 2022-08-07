// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWSolid
  {
    public:
      typedef std::shared_ptr<CSWSolid> PtrType;

      static PtrType create(const Mesh::PtrType pSurface);
      ~CSWSolid();

      void draw(const Matrix44D& mat);

      //given in local coordsystem
      const Mesh::PtrType getMesh() const;
      const BoundingBoxD& getBoundingBox() const;

      //given in local coordsystem
      void setMesh(const Mesh::PtrType pSurface);

      void setColor(const Vector4D& vecColor);
      const Vector4D& getColor();

      const Vector3D& getCenter() const;

    protected:
      CSWSolid(const Mesh::PtrType pSurface);

      void validate();

      Mesh::PtrType m_pSurface;
      BoundingBoxD m_BoundingBox;
      Vector3D m_vecCenter; //in local system
      Vector4D m_vecColor;
  };

}