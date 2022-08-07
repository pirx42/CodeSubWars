// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWBorder.h"
#include "CSWEvent.h"
#include "CSWCollisionDetectedMessage.h"

#include "CSWSolid.h"
#include "CSWCollideable.h"


namespace CodeSubWars
{

  CSWBorder::~CSWBorder()
  {
  }


  void CSWBorder::draw()
  {

  //   glPushAttrib(GL_ALL_ATTRIB_BITS);
  // 
  //     glDisable(GL_CULL_FACE);
  //     
  //     glEnable(GL_COLOR_MATERIAL);
  //     glColorMaterial(GL_FRONT, GL_DIFFUSE);
  // 
  //     glEnable(GL_BLEND);
  //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // 
  //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // 
  //     glShadeModel(GL_FLAT);
  //     glEnable(GL_LIGHTING);
  //     glEnable(GL_LIGHT0);
  //     glColor4dv(getColor().pData);
  // 
  //     glEnable(GL_DEPTH_TEST);
  //     glDepthMask(false);
  //     
  //     m_pSolid->getMesh()->render();
  // 
  //     
  //     glDisable(GL_BLEND);
  //     glDisable(GL_LIGHTING);
  //     glColor3dv(getColor().pData);
  //     glLineWidth(2);
  //     glBegin(GL_LINES);
  //       for (int iCnt = 0; iCnt < 12; ++iCnt) 
  //       {
  //         glVertex3dv(m_pSolid->getBoundingBox().getSegment(iCnt).getPoint1().pData);
  //           glVertex3dv(m_pSolid->getBoundingBox().getSegment(iCnt).getPoint2().pData);
  //       } 
  //     glEnd();
  //   glPopAttrib();

    //for debugging collision
    m_pCollideable->draw(getTransform());
  }


  const Mesh::PtrType CSWBorder::getSurface() const
  {
    return m_pSolid->getMesh();
  }


  const BoundingBoxD& CSWBorder::getSurfaceBoundingBox() const
  {
    return m_pSolid->getBoundingBox();
  }


  void CSWBorder::setColor(const Vector4D& vecColor)
  {
    return m_pSolid->setColor(vecColor);
  }

    
  const Vector4D& CSWBorder::getColor()
  {
    return m_pSolid->getColor();
  }

    
  CSWBorder::CSWBorder(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWObject(strName, matBaseTObject)
  {
    Matrix44D mat1(Vector3D(1, 0, 0),
                   Vector3D(0, 1, 0),
                   Vector3D(0, 0, 1),
                   Vector3D(-size.getWidth()/2.0, -size.getHeight()/2.0, -size.getDepth()/2.0));
    Mesh::PtrType pSurface = Mesh::createBox(mat1, size);
    m_pSolid = CSWSolid::create(pSurface);

    Matrix44D mat2(Vector3D(1, 0, 0),
                   Vector3D(0, 1, 0),
                   Vector3D(0, 0, 1),
                   Vector3D(-size.getWidth()/2.0, -size.getHeight()/2.0, -size.getDepth()/2.0));
    Mesh::PtrType pCollisionMesh = Mesh::createBox(mat2, size);
    m_pCollideable = CSWCollideable::create(pCollisionMesh);

    setColor(Vector4D(0, 0, 1, 0.2));
  }


  const Mesh::PtrType CSWBorder::getCollisionMesh() const
  {
    return m_pCollideable->getMesh();
  }


  const BoundingBoxD& CSWBorder::getCollisionBoundingBox() const
  {
    return m_pCollideable->getBoundingBox();
  }


  void CSWBorder::initializeEnvironment(bool bSimplified)
  {
    BoundingBoxD bbox(m_pCollideable->getMesh()->getAxisAlignedBoundingBox());
    Size3D size = bbox.getSize();
    DT_ShapeHandle hDTShape = DT_NewBox(size.getWidth(), size.getHeight(), size.getDepth());

    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);

    Matrix44D matObjectTCollisionShape(Vector3D(bbox.getCenter()));
    Mesh::PtrType pCollisionMesh = Mesh::createBox(bbox.getCoordSystem(), bbox.getSize());

    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, matObjectTCollisionShape, pCollisionMesh);
  }


  void CSWBorder::prepare()
  {
    m_pCollideable->prepare();
  }


  void CSWBorder::setupTransform()
  {
    m_pCollideable->setupTransform(getSharedThis());
  }


  const Matrix44D& CSWBorder::getObjectTCollisionShape()
  {
    return m_pCollideable->getObjectTCollisionShape();
  }

  void CSWBorder::setHit(bool bHit)
  {
    m_pCollideable->setHit(bHit);
  }


  bool CSWBorder::deleteEnvironment()
  {
    return m_pCollideable->deleteEnvironment();
  }

}