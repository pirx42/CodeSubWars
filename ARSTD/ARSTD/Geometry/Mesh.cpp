// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "arstd/Geometry/Mesh.h"
#include "windows.h"
#include "gl/gl.h"
#include <fstream>

#undef min
#undef max

namespace ARSTD
{

  using namespace std;


  Mesh::PtrType Mesh::create(const VectorContainer& vertices, 
                             const TriangleContainer& triangles,
                             const VectorContainer& normals)
  {
    return PtrType(new Mesh(vertices, triangles, normals));
  }


  Mesh::PtrType Mesh::readSTL(const std::string& strFilename)
  {
    std::fstream stream(strFilename, ios::in | ios::binary);
    return readSTL(stream);
  }


  Mesh::PtrType Mesh::readSTL(std::istream& iStream)
  {
    iStream.seekg(80, ios::cur);

    unsigned long nNumFaces;
    iStream.read(reinterpret_cast<char*>(&nNumFaces), 4);

    VectorContainer vertices;
    TriangleContainer triangles;
    VertexIndexMap vertexIndexMap;

    Vector3F vertex[3];
    while (!iStream.eof() && !iStream.bad() && iStream.tellg() != std::istream::pos_type(-1))
    {
      iStream.seekg(sizeof(Vector3F), ios::cur); //skip normal

      iStream.read(reinterpret_cast<char*>(vertex), sizeof(vertex));
      iStream.seekg(2, ios::cur); // skip last two bytes

      if (vertex[0] == vertex[1] || vertex[1] == vertex[2] || vertex[0] == vertex[2])
        continue;

      mergeVertices(vertices, vertexIndexMap, triangles, vertex);
    }

    return create(vertices, triangles);
  }


  Mesh::PtrType Mesh::createBox(const Matrix44D& matWorldTObject, const Size3D& size)
  {
    assert(size.getWidth() >= 0 && size.getHeight() >= 0 && size.getDepth() >= 0);
    Matrix44F mat(matWorldTObject);
    Size3F s(size);

    VectorContainer vertices;
    vertices.push_back(mat*Vector3F(           0,            0,            0));
    vertices.push_back(mat*Vector3F(s.getWidth(),            0,            0));
    vertices.push_back(mat*Vector3F(s.getWidth(),            0, s.getDepth()));
    vertices.push_back(mat*Vector3F(           0,            0, s.getDepth()));

    vertices.push_back(mat*Vector3F(           0, s.getHeight(),            0));
    vertices.push_back(mat*Vector3F(s.getWidth(), s.getHeight(),            0));
    vertices.push_back(mat*Vector3F(s.getWidth(), s.getHeight(), s.getDepth()));
    vertices.push_back(mat*Vector3F(           0, s.getHeight(), s.getDepth()));

    TriangleContainer triangles;
    triangles.push_back(Triangle(0, 1, 2));
    triangles.push_back(Triangle(0, 2, 3));

    triangles.push_back(Triangle(0, 3, 7));
    triangles.push_back(Triangle(0, 7, 4));

    triangles.push_back(Triangle(1, 5, 6));
    triangles.push_back(Triangle(1, 6, 2));

    triangles.push_back(Triangle(0, 5, 1));
    triangles.push_back(Triangle(0, 4, 5));

    triangles.push_back(Triangle(3, 2, 6));
    triangles.push_back(Triangle(3, 6, 7));

    triangles.push_back(Triangle(5, 4, 7));
    triangles.push_back(Triangle(5, 7, 6));

    return create(vertices, triangles);
  }


  Mesh::PtrType Mesh::createCylinder(const Matrix44D& matWorldTObject, const double fLength, const double fBeginDiameter, 
                                     const double fEndDiameter, const bool bWithDisks, const uint nSides, const uint nSegments)
  {
    assert(fLength >= 0 && fBeginDiameter >= 0 && fEndDiameter >= 0);

    Matrix44F mat(matWorldTObject);

    uint nCntSegment;
    uint nCntSide;
    double fRadian;

    VectorContainer vertices;
    if (bWithDisks)
      vertices.push_back(mat*Vector3F(0, 0, 0));
    for (nCntSegment = 0; nCntSegment < nSegments + 1; ++nCntSegment)
    {
      fRadian = ((fEndDiameter - fBeginDiameter)*nCntSegment/nSegments + fBeginDiameter)*0.5;
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        double fAngle = 2*PI*static_cast<double>(nCntSide)/static_cast<double>(nSides);
        vertices.push_back(mat*Vector3F(static_cast<float>(fLength*nCntSegment/nSegments), static_cast<float>(sin(fAngle)*fRadian), static_cast<float>(cos(fAngle)*fRadian)));
      }
    }
    if (bWithDisks)
      vertices.push_back(mat*Vector3F(static_cast<float>(fLength), 0, 0));

    TriangleContainer triangles;
    int nOff = 0;
    if (bWithDisks)
      nOff = 1;
    for (nCntSegment = 0; nCntSegment < nSegments; ++nCntSegment)
    {
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        triangles.push_back(Triangle(nOff + nCntSide + nCntSegment*nSides,
                                     nOff + nCntSide + (nCntSegment + 1)*nSides,
                                     nOff + (nCntSide + 1) % nSides + (nCntSegment + 1)*nSides));
        triangles.push_back(Triangle(nOff + nCntSide + nCntSegment*nSides,
                                     nOff + (nCntSide + 1) % nSides + (nCntSegment + 1)*nSides, 
                                     nOff + (nCntSide + 1) % nSides + nCntSegment*nSides));
      }
    }

    if (bWithDisks)
    {
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
        triangles.push_back(Triangle(0, 1 + nCntSide, 1 + (nCntSide + 1) % nSides));

      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        triangles.push_back(Triangle(static_cast<uint>(vertices.size() - 1), 
                                     static_cast<uint>(vertices.size() - 2 - nCntSide), 
                                     static_cast<uint>(vertices.size() - 2 - (nCntSide + 1) % nSides)));
      }
    }

    return create(vertices, triangles);
  }


  Mesh::PtrType Mesh::createEllipsoid(const Matrix44D& matWorldTObject, const Vector3D& vecDiameter,
                                      const uint nSides, const uint nSegments, 
                                      const uint nFromSegment, const uint nToSegment)
  {
    assert(nToSegment <= nSegments);
    assert(nFromSegment <= nToSegment);

    Matrix44F mat(matWorldTObject);

    Vector3D vecRadian = vecDiameter/2.0;

    uint nCntSegment;
    uint nCntSide;
    VectorContainer vertices;
    vertices.push_back(mat*Vector3F(static_cast<float>(-vecRadian.x), 0, 0));
    for (nCntSegment = 0; nCntSegment < nSegments; ++nCntSegment)
    {
      double fNormalizedRadianAroundX = sin(PI*static_cast<double>(nCntSegment)/static_cast<double>(nSegments));
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        double fAngle = 2*PI*static_cast<double>(nCntSide)/static_cast<double>(nSides);
        Vector3F vecPoint(static_cast<float>(-cos(PI*static_cast<double>(nCntSegment)/static_cast<double>(nSegments))*vecRadian.x),
                          static_cast<float>(sin(fAngle)*fNormalizedRadianAroundX*vecRadian.y),
                          static_cast<float>(cos(fAngle)*fNormalizedRadianAroundX*vecRadian.z));

        vertices.push_back(mat*vecPoint);
      }
    }
    vertices.push_back(mat*Vector3F(static_cast<float>(vecRadian.x), 0, 0));

    TriangleContainer triangles;
    uint nMaxSegment = std::min(nToSegment, nSegments - 1);
    for (nCntSegment = nFromSegment; nCntSegment < nMaxSegment; ++nCntSegment)
    {
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        triangles.push_back(Triangle(1 + nCntSide + nCntSegment*nSides, 
                                     1 + nCntSide + (nCntSegment + 1)*nSides,
                                     1 + (nCntSide + 1) % nSides + (nCntSegment + 1)*nSides));
        triangles.push_back(Triangle(1 + nCntSide + nCntSegment*nSides, 
                                     1 + (nCntSide + 1) % nSides + (nCntSegment + 1)*nSides, 
                                     1 + (nCntSide + 1) % nSides + nCntSegment*nSides));
      }
    }

    if (nFromSegment == 0)
    {
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        triangles.push_back(Triangle(0, 1 + nCntSide, 1 + (nCntSide + 1) % nSides));
      }
    }

    if (nToSegment == nSegments)
    {
      for (nCntSide = 0; nCntSide < nSides; ++nCntSide)
      {
        triangles.push_back(Triangle(static_cast<uint>(vertices.size() - 1),
                                     static_cast<uint>(vertices.size() - 2 - nCntSide),
                                     static_cast<uint>(vertices.size() - 2 - (nCntSide + 1) % nSides)));
      }
    }

    return create(vertices, triangles);
  }


  Mesh::PtrType Mesh::copy(const PtrType pMesh)
  {
    return PtrType(new Mesh(*(pMesh.get())));
  }


  Mesh::PtrType Mesh::transformed(const Matrix44D& mat)
  {
    if (m_Vertices.size() != m_Normals.size())
      return create(m_Vertices, m_Triangles, m_Normals);

    Matrix44F matF(mat);
    VectorContainer vertices(m_Vertices);
    for (auto& v : vertices)
      v = matF*v;

    return create(vertices, m_Triangles);
  }


  void Mesh::render(int nNumerator, int nDenominator) const
  {
    assert(nNumerator > 0 && nDenominator > 0 && nNumerator <= nDenominator);

    glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT);

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, &m_Normals.front());
    glVertexPointer(3, GL_FLOAT, 0, &m_Vertices.front());

    int nTrisToDrawAtOnce = static_cast<int>(m_Triangles.size())/nDenominator;
    glDrawElements(GL_TRIANGLES, nTrisToDrawAtOnce*3, GL_UNSIGNED_INT, &m_Triangles.front() + (nNumerator - 1)*nTrisToDrawAtOnce);
    
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
////draw normals    
//glColor3d(0, 1, 1);
//glBegin(GL_LINES);
//  VectorContainer::const_iterator itVert = m_Vertices.begin();
//  VectorContainer::const_iterator itNormal = m_Normals.begin();
//  for (; itVert != m_Vertices.end() && itNormal != m_Normals.end(); ++itVert, ++itNormal)
//  {
//    glVertex3dv(itVert->pData);
//    glVertex3dv((*itVert + *itNormal*3.0).pData);
//  }
//glEnd();

    glPopAttrib();
  }


  Mesh::VectorConstIterator Mesh::getVerticesBegin() const
  { 
    return m_Vertices.begin(); 
  }


  Mesh::VectorConstIterator Mesh::getVerticesEnd() const
  { 
    return m_Vertices.end(); 
  }


  uint Mesh::getNumVertices() const 
  { 
    return static_cast<unsigned int>(m_Vertices.size()); 
  }


  uint Mesh::getNumTriangles() const
  {
    return static_cast<unsigned int>(m_Triangles.size());
  }


  const AxisAlignedBoundingBoxD& Mesh::getAxisAlignedBoundingBox() const
  {
    return m_AxisAlignedBoundingBox;
  }


  void Mesh::add(const PtrType pMesh)
  {
    if (!pMesh) return;
    
    uint nSizeVertices = static_cast<uint>(m_Vertices.size());
    
    m_Vertices.insert(m_Vertices.end(), pMesh->m_Vertices.begin(), pMesh->m_Vertices.end());
    m_Normals.insert(m_Normals.end(), pMesh->m_Normals.begin(), pMesh->m_Normals.end());

    TriangleContainer::iterator it = pMesh->m_Triangles.begin();
    for (; it != pMesh->m_Triangles.end(); ++it)
    {
      it->indices[0] += nSizeVertices;
      it->indices[1] += nSizeVertices;
      it->indices[2] += nSizeVertices;
      m_Triangles.push_back(*it);
    }

    generateNormals();
    updateAxisAlignedBoundingBox();
  }


  Mesh::Mesh()
  {
  }


  Mesh::Mesh(const VectorContainer& vertices, 
             const TriangleContainer& triangles,
             const VectorContainer& normals)
  : m_Vertices(vertices),
    m_Triangles(triangles),
    m_Normals(normals)
  {
    if (normals.size() != vertices.size())
      generateNormals();
    updateAxisAlignedBoundingBox();
  }


  Mesh::Mesh(const Mesh& other)
  : m_Vertices(other.m_Vertices),
    m_Triangles(other.m_Triangles),
    m_Normals(other.m_Normals),
    m_AxisAlignedBoundingBox(other.m_AxisAlignedBoundingBox)
  {
  }


  void Mesh::generateNormals()
  {
    m_Normals.resize(m_Vertices.size());

    for (uint i = 0; i < m_Triangles.size(); i++)
    {
      uint i0 = m_Triangles[i].indices[0];
      uint i1 = m_Triangles[i].indices[1];
      uint i2 = m_Triangles[i].indices[2];

      assert(i0 < m_Vertices.size() &&
             i1 < m_Vertices.size() &&
             i2 < m_Vertices.size());

      const Vector3F& vec0 = m_Vertices[i0];
      const Vector3F& vec1 = m_Vertices[i1];
      const Vector3F& vec2 = m_Vertices[i2];

      Vector3F vecNormal = (vec1 - vec0)%(vec2 - vec0);

      m_Normals[i0] += vecNormal;
      m_Normals[i1] += vecNormal;
      m_Normals[i2] += vecNormal;
    }

    for (auto& n : m_Normals)
    {
      float length = n.getLength();
      if (fabs(length) > EPSILON)
        n.normalize();
    }
  }


  void Mesh::updateAxisAlignedBoundingBox()
  {
    if (m_Vertices.size() == 0) 
    {
      m_AxisAlignedBoundingBox = AxisAlignedBoundingBoxD();
    }
    else
    {
      auto minMax = getMinMax(m_Vertices.begin(), m_Vertices.end());
      m_AxisAlignedBoundingBox = AxisAlignedBoundingBoxD(Vector3D(minMax.first), Vector3D(minMax.second));
    }
  }


  void Mesh::mergeVertices(VectorContainer& vertices,
                           VertexIndexMap& vertexIndexMap,
                           TriangleContainer& triangles,
                           Vector3F vertex[3])
  {
    uint nIndex[3];

    for (int i = 0; i < 3; ++i)
    {
      auto range = vertexIndexMap.equal_range(vertex[i].x);
      auto it = range.first;

      while (it != range.second)
      {
        if (vertices[it->second] == vertex[i])
          break;
        ++it;
      }

      if (it == range.second)
      {
        //vertex not found -> add new one
        vertices.push_back(vertex[i]);
        nIndex[i] = static_cast<uint>(vertices.size() - 1);
        vertexIndexMap.insert(std::make_pair(vertex[i].x, nIndex[i]));
      }
      else
      {
        //vertex found -> set corresponding index 
        nIndex[i] = it->second;
      }
    }

    assert(nIndex[0] != nIndex[1] && nIndex[0] != nIndex[2] && nIndex[1] != nIndex[2]);

    triangles.push_back(Mesh::Triangle(nIndex[0], nIndex[1], nIndex[2]));
  }

}
