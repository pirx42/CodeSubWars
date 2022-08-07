// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "CSWDynSolCol.h"
#include "CSWSolid.h"
#include "CSWCollideable.h"

#include "CSWEngine.h"


namespace CodeSubWars
{

  CSWDynSolCol::~CSWDynSolCol()
  {
  }


  void CSWDynSolCol::updatePosition()
  {
    //collect all forces and applying points from childs
    ARSTD::Node::ChildConstRange range = getChildRange();
    ARSTD::Node::ChildConstIterator it;
    for (it = range.first; it != range.second; ++it)
    {
      CSWEngine::PtrType pEngine = std::dynamic_pointer_cast<CSWEngine>(*it);
      if (pEngine)
      {
        m_pDynamic->addForce(pEngine->getWorldTransform().getTranslation(), pEngine->getAppliedForce());
      }
    }

    setTransform(m_pDynamic->updatePosition());
  }


  void CSWDynSolCol::resetForce()
  {
    m_pDynamic->resetForce();
  }


  void CSWDynSolCol::addForceToCM(const Vector3D& vecWorldTForce)
  {
    m_pDynamic->addForceToCM(vecWorldTForce);
  }


  void CSWDynSolCol::addForce(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTForce)
  {
    m_pDynamic->addForce(vecWorldTPosition, vecWorldTForce);
  }


  void CSWDynSolCol::addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls)
  {
    m_pDynamic->addImpuls(vecWorldTPosition, vecWorldTImpuls);
  }


  void CSWDynSolCol::draw()
  {
    m_pSolid->draw(getTransform());

    //for debugging
    m_pDynamic->draw();
    m_pCollideable->draw(getTransform());

  //  Vector3D v = getWorldTransform()*m_pDynamic->getCenterOfMass() + m_vecCollisionPoint;
  //  v = getInvertWorldTransform()*v;
  //  if (m_vecCollisionNormal.getLength() > 0.0)
  //  {
  //    Matrix44D mat;
  //    mat.getXAxis() = m_vecCollisionNormal.getNormalized();
  //    mat.getYAxis() = mat.getXAxis().getPerpendicular().getNormalized();
  //    mat.getZAxis() = mat.getXAxis()%mat.getYAxis();
  //    mat.getTranslation() = v - mat.getYAxis()*50.0 - mat.getZAxis()*50.0;
  //    BoundingBoxD bbox(mat, Size3D(1, 100, 100));
  //    glShadeModel(GL_FLAT);
  //    glDisable(GL_LIGHTING);
  //    glBegin(GL_LINES);
  //      glColor3f(0.0f, 1.0f, 0.0f);
  //      for (int iCnt = 0; iCnt < 12; ++iCnt) 
  //      {
  //        glVertex3dv(bbox.getSegment(iCnt).getPoint1().pData);
  //        glVertex3dv(bbox.getSegment(iCnt).getPoint2().pData);
  //      }
  //    glEnd();
  //  }
  //
  //  glShadeModel(GL_FLAT);
  //  glDisable(GL_LIGHTING);
  //  glPointSize(10);
  //  glBegin(GL_POINTS);
  //    glColor3f(1.0f, 0.0f, 0.0f);
  //    glVertex3dv(v.pData);
  //  glEnd();  
  }


  const Mesh::PtrType CSWDynSolCol::getSurface() const
  {
    return m_pSolid->getMesh();
  }


  const BoundingBoxD& CSWDynSolCol::getSurfaceBoundingBox() const
  {
    return m_pSolid->getBoundingBox();
  }


  void CSWDynSolCol::setColor(const Vector4D& vecColor)
  {
    m_pSolid->setColor(vecColor);
  }


  const Vector4D& CSWDynSolCol::getColor()
  {
    return m_pSolid->getColor();
  }

    
  CSWDynamic::MassPointContainerRange CSWDynSolCol::getMassPointRange()
  {
    return m_pDynamic->getMassPointRange();
  }


  const Vector3D& CSWDynSolCol::getVelocity() const
  {
    return m_pDynamic->getVelocity();
  }


  const Vector3D& CSWDynSolCol::getAcceleration() const
  {
    return m_pDynamic->getAcceleration();
  }


  const Vector3D& CSWDynSolCol::getAngularVelocity() const
  {
    return m_pDynamic->getAngularVelocity();
  }


  Vector3D CSWDynSolCol::getAngularVelocity(const Vector3D& vecObjectTPosition) const
  {
    return m_pDynamic->getAngularVelocity(vecObjectTPosition);
  }


  const Vector3D& CSWDynSolCol::getAngularAcceleration() const
  {
    return m_pDynamic->getAngularAcceleration();
  }


  Vector3D CSWDynSolCol::getAngularAcceleration(const Vector3D& vecObjectTPosition) const
  {
    return m_pDynamic->getAngularAcceleration(vecObjectTPosition);
  }


  const Vector3D& CSWDynSolCol::getAngularMomentum() const
  {
    return m_pDynamic->getAngularMomentum();
  }


  const Matrix33D& CSWDynSolCol::getInvertedMomentOfIntertia() const
  {
    return m_pDynamic->getInvertedMomentOfIntertia();
  }


  const double& CSWDynSolCol::getTotalMass() const
  {
    return m_pDynamic->getTotalMass();
  }


  void CSWDynSolCol::setNewTransform(const Matrix44D& matWorldTObject)
  {
    return m_pDynamic->setNewTransform(matWorldTObject);
  }


  void CSWDynSolCol::setNewVelocity(const Vector3D& vecWorldTVelocityCM,
                                    const Vector3D& vecWorldTAngularMomentumCM)
  {
    return m_pDynamic->setNewVelocity(vecWorldTVelocityCM, vecWorldTAngularMomentumCM);
  }


  const Vector3D& CSWDynSolCol::getCenterOfMass() const
  {
    return m_pDynamic->getCenterOfMass();
  }


  void CSWDynSolCol::initializeDynamic(const Matrix44D& matWorldTPositionCM, 
                                             const Vector3D& vecWorldTVelocityCM,
                                             const Vector3D& vecWorldTAngularMomentumCM)
  {
    setTransform(matWorldTPositionCM);

    //all childs must be recaclulate its positions immediatly
    calcWorldTransforms();

    setNewVelocity(vecWorldTVelocityCM, vecWorldTAngularMomentumCM);
    setNewTransform(matWorldTPositionCM);
  }


  CSWDynSolCol::CSWDynSolCol(const std::string& strName, const Matrix44D& matBaseTObject, 
                             Mesh::PtrType pSurface)
  : CSWObject(strName, matBaseTObject),
    m_pSolid(CSWSolid::create(pSurface)),
    m_pCollideable(CSWCollideable::create(pSurface))
  {
    Size3D sizeSurface = pSurface->getAxisAlignedBoundingBox().getSize();
    Matrix44D mat(Vector3D(pSurface->getAxisAlignedBoundingBox().getMin()));
    Mesh::PtrType pMassMesh = Mesh::createBox(mat, sizeSurface);
    m_pDynamic = CSWDynamic::create(matBaseTObject, pMassMesh);
  }


  const Mesh::PtrType CSWDynSolCol::getCollisionMesh() const
  {
    return m_pCollideable->getMesh();
  }


  const BoundingBoxD& CSWDynSolCol::getCollisionBoundingBox() const
  {
    return m_pCollideable->getBoundingBox();
  }


  void CSWDynSolCol::initializeEnvironment(bool bSimplified)
  {
    BoundingBoxD bbox(m_pCollideable->getMesh()->getAxisAlignedBoundingBox());
    Size3D size = bbox.getSize();
    DT_ShapeHandle hDTShape = DT_NewBox(size.getWidth(), size.getHeight(), size.getDepth());

    DT_ObjectHandle hDTObject = DT_CreateObject(this, hDTShape);

    Matrix44D matObjectTCollisionShape(Vector3D(bbox.getCenter()));
    Mesh::PtrType pCollisionMesh = Mesh::createBox(bbox.getCoordSystem(), bbox.getSize());

    m_pCollideable->initializeEnvironment(hDTShape, hDTObject, matObjectTCollisionShape, pCollisionMesh);
  }


  void CSWDynSolCol::prepare()
  {
    m_pCollideable->prepare();
  }


  void CSWDynSolCol::setupTransform()
  {
    m_pCollideable->setupTransform(getSharedThis());
  }


  const Matrix44D& CSWDynSolCol::getObjectTCollisionShape()
  {
    return m_pCollideable->getObjectTCollisionShape();
  }


  void CSWDynSolCol::setHit(bool bHit)
  {
    m_pCollideable->setHit(bHit);
  }


  bool CSWDynSolCol::deleteEnvironment()
  {
    return m_pCollideable->deleteEnvironment();
  }

}