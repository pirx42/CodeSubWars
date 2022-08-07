// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWTrigger.h"
#include "CSWCollideable.h"


namespace CodeSubWars
{

  CSWTrigger::~CSWTrigger()
  {
  }


  void CSWTrigger::draw()
  {
    glPushMatrix();
    glLoadMatrixd(getWorldTransform().pData);
    m_pCollideable->draw(getTransform());
    glPopMatrix();
  }


  const Mesh::PtrType CSWTrigger::getCollisionMesh() const
  {
    return m_pCollideable->getMesh();
  }


  const BoundingBoxD& CSWTrigger::getCollisionBoundingBox() const
  {
    return m_pCollideable->getBoundingBox();
  }


  void CSWTrigger::initializeEnvironment(bool bSimplified)
  {
    BoundingBoxD bbox(m_pCollideable->getMesh()->getAxisAlignedBoundingBox());
    Size3D size = bbox.getSize();
    DT_ShapeHandle hDTShape = DT_NewBox(size.getWidth(), size.getHeight(), size.getDepth());

    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);

    Matrix44D matObjectTCollisionShape(Vector3D(bbox.getCenter()));
    Mesh::PtrType pCollisionMesh = Mesh::createBox(bbox.getCoordSystem(), bbox.getSize());

    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, matObjectTCollisionShape, pCollisionMesh);
  }


  void CSWTrigger::prepare()
  {
    m_pCollideable->prepare();
  }


  void CSWTrigger::setupTransform()
  {
    m_pCollideable->setupTransform(getSharedThis());
  }


  const Matrix44D& CSWTrigger::getObjectTCollisionShape()
  {
    return m_pCollideable->getObjectTCollisionShape();
  }

  void CSWTrigger::setHit(bool bHit)
  {
    m_pCollideable->setHit(bHit);
  }


  bool CSWTrigger::deleteEnvironment()
  {
    return m_pCollideable->deleteEnvironment();
  }


  void CSWTrigger::detectCollision(CSWObject::PtrType pObject, const Vector3D& vecObjectTPosition)
  {
  }


  CSWTrigger::CSWTrigger(const std::string& strName, const Matrix44D& matBaseTObject,
                         Mesh::PtrType pCollisionMesh)
  : CSWObject(strName, matBaseTObject),
    m_pCollideable(CSWCollideable::create(pCollisionMesh))
  {
  }

}