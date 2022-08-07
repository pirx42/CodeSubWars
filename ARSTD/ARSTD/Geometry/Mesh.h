// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#pragma warning (disable: 4786)


#include <vector>
#include <map>
#include "arstd/Geometry/AxisAlignedBoundingBox.h"


namespace ARSTD
{


  class Mesh
  {
    public:
      struct Triangle
      {
        Triangle(uint i0 = 0, uint i1 = 0, uint i2 = 0)
        {
          indices[0] = i0;
          indices[1] = i1;
          indices[2] = i2;
        }

        uint indices[3];
      };

      typedef std::shared_ptr<Mesh> PtrType;
      typedef std::vector<Vector3F> VectorContainer;
      typedef VectorContainer::const_iterator VectorConstIterator;
      typedef std::vector<Triangle> TriangleContainer;

      static PtrType create(const VectorContainer& vertices, const TriangleContainer& triangles, const VectorContainer& normals = VectorContainer());

      static Mesh::PtrType readSTL(const std::string& strFilename);
      static Mesh::PtrType readSTL(std::istream& iStream);

      static Mesh::PtrType createBox(const Matrix44D& matWorldTObject, const Size3D& size);
      static Mesh::PtrType createCylinder(const Matrix44D& matWorldTObject, const double fLength, const double fBeginDiameter,
                                          const double fEndDiameter, const bool bWithDisks = false, const uint nSides = 15, const uint nSegments = 1);
      static Mesh::PtrType createEllipsoid(const Matrix44D& matWorldTObject, const Vector3D& vecDiameter,
                                           const uint nSides = 15, const uint nSegments = 15, 
                                           const uint nFromSegment = 0, const uint nToSegment = 15);

      static PtrType copy(const PtrType pMesh);

      PtrType transformed(const Matrix44D& mat);

      void render(int nNumerator = 1, int nDenominator = 1) const;

      VectorConstIterator getVerticesBegin() const;
      VectorConstIterator getVerticesEnd() const;
      uint getNumVertices() const;
      uint getNumTriangles() const;

      const AxisAlignedBoundingBoxD& getAxisAlignedBoundingBox() const;

      void add(const PtrType pMesh);

    protected:
      typedef std::multimap<double, uint> VertexIndexMap;

      Mesh();
      Mesh(const VectorContainer& vertices, const TriangleContainer& triangles, const VectorContainer& normals = VectorContainer());
      Mesh(const Mesh& other);

      void generateNormals();
      void updateAxisAlignedBoundingBox();

      static void mergeVertices(Mesh::VectorContainer& vertices,
                                VertexIndexMap& vertexIndexMap,
                                Mesh::TriangleContainer& triangles,
                                Vector3F vertex[3]);

      VectorContainer m_Vertices;
      TriangleContainer m_Triangles;
      VectorContainer m_Normals;
      AxisAlignedBoundingBoxD m_AxisAlignedBoundingBox;
  };


}
