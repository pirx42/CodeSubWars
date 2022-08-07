// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSolid.h"



namespace CodeSubWars
{

  CSWSolid::PtrType CSWSolid::create(const Mesh::PtrType pSurface)
  {
    return PtrType(new CSWSolid(pSurface));
  }


  CSWSolid::~CSWSolid()
  {
  }


  void CSWSolid::draw(const Matrix44D& mat)
  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glShadeModel(GL_SMOOTH);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);

      glPolygonMode(GL_FRONT, GL_FILL);

      glCullFace(GL_BACK);    
      glEnable(GL_CULL_FACE);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);

      glColor4dv(m_vecColor.pData);

      m_pSurface->render();

  //  //bounding box for debugging
  //  glShadeModel(GL_FLAT);
  //  glDisable(GL_LIGHTING);
  //  glBegin(GL_LINES);
  //    for (int iCnt = 0; iCnt < 12; ++iCnt) 
  //    {
  //      glVertex3dv(m_BoundingBox.getSegment(iCnt).getPoint1().pData);
  //      glVertex3dv(m_BoundingBox.getSegment(iCnt).getPoint2().pData);
  //    }
  //  glEnd();
    glPopAttrib();
  }


  const Mesh::PtrType CSWSolid::getMesh() const
  {
    return m_pSurface;
  }


  const BoundingBoxD& CSWSolid::getBoundingBox() const
  {
    return m_BoundingBox;
  }


  void CSWSolid::setMesh(const Mesh::PtrType pSurface)
  {
    m_pSurface = pSurface;
    m_BoundingBox = BoundingBoxD(m_pSurface->getAxisAlignedBoundingBox());
    validate();
  }


  CSWSolid::CSWSolid(const Mesh::PtrType pSurface)
  : m_pSurface(pSurface),
    m_BoundingBox(m_pSurface->getAxisAlignedBoundingBox()),
    m_vecColor(1, 1, 1, 1)
  {
    assert(m_pSurface);
    validate();
  }


  const Vector3D& CSWSolid::getCenter() const
  {
    return m_vecCenter;
  }


  void CSWSolid::setColor(const Vector4D& vecColor)
  {
    m_vecColor = vecColor;
  }


  const Vector4D& CSWSolid::getColor()
  {
    return m_vecColor;
  }


  void CSWSolid::validate()
  {
    assert(m_pSurface);
  
    m_vecCenter = Vector3D(0, 0, 0);
    Mesh::VectorConstIterator itVertex = m_pSurface->getVerticesBegin();
    for (; itVertex != m_pSurface->getVerticesEnd(); ++itVertex)
    {
      m_vecCenter += Vector3D(*itVertex);
    }
    if (m_pSurface->getNumVertices())
      m_vecCenter /= m_pSurface->getNumVertices();
  }

}