// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWall.h"
#include "CSWSolid.h"
#include "CSWCollideable.h"
#include "CSWEvent.h"
#include "CSWCollisionDetectedMessage.h"

#include "CSWCamera.h"


namespace CodeSubWars
{

  CSWWall::~CSWWall()
  {
  }


  void CSWWall::draw()
  {
    m_pSolid->draw(getTransform());

    //for debugging collision
    m_pCollideable->draw(getTransform());
  }


  const Mesh::PtrType CSWWall::getSurface() const
  {
    return m_pSolid->getMesh();
  }


  const BoundingBoxD& CSWWall::getSurfaceBoundingBox() const
  {
    return m_pSolid->getBoundingBox();
  }


  void CSWWall::setColor(const Vector4D& vecColor)
  {
    return m_pSolid->setColor(vecColor);
  }


  const Vector4D& CSWWall::getColor()
  {
    return m_pSolid->getColor();
  }


  CSWWall::CSWWall(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
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
  
    setColor(Vector4D(1, 1, 0, 1));
  }


  const Mesh::PtrType CSWWall::getCollisionMesh() const
  {
    return m_pCollideable->getMesh();
  }


  const BoundingBoxD& CSWWall::getCollisionBoundingBox() const
  {
    return m_pCollideable->getBoundingBox();
  }


  void CSWWall::initializeEnvironment(bool bSimplified)
  {
    BoundingBoxD bbox(m_pCollideable->getMesh()->getAxisAlignedBoundingBox());
    Size3D size = bbox.getSize();
    DT_ShapeHandle hDTShape = DT_NewBox(size.getWidth(), size.getHeight(), size.getDepth());

    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);

    Matrix44D matObjectTCollisionShape(Vector3D(bbox.getCenter()));
    Mesh::PtrType pCollisionMesh = Mesh::createBox(bbox.getCoordSystem(), bbox.getSize());

    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, matObjectTCollisionShape, pCollisionMesh);
  }


  void CSWWall::prepare()
  {
    m_pCollideable->prepare();
  }


  void CSWWall::setupTransform()
  {
    m_pCollideable->setupTransform(getSharedThis());
  }


  const Matrix44D& CSWWall::getObjectTCollisionShape()
  {
    return m_pCollideable->getObjectTCollisionShape();
  }


  void CSWWall::setHit(bool bHit)
  {
    m_pCollideable->setHit(bHit);
  }


  bool CSWWall::deleteEnvironment()
  {
    return m_pCollideable->deleteEnvironment();
  }

}