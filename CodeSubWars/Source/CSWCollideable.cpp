// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWCollideable.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "CSWSettings.h"


namespace CodeSubWars
{

  CSWCollideable::PtrType CSWCollideable::create(const Mesh::PtrType pCollisionMesh)
  {
    assert(pCollisionMesh);
    return PtrType(new CSWCollideable(pCollisionMesh));
  }


  CSWCollideable::~CSWCollideable()
  {
  }


  void CSWCollideable::draw(const Matrix44D& mat)
  {
    if (CSWWorld::getInstance()->getSettings()->getCollidableProperties() & CSWSettings::DISPLAY_COLLISION_MESH)
    {
      //only for debugging
      glPushAttrib(GL_ALL_ATTRIB_BITS);
        glShadeModel(GL_FLAT);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3fv((m_bHit ? Vector3F(0, 1, 1) : Vector3F(0, 0, 1)).pData);
        glLineWidth(1);

        m_pCollisionMesh->render();
      glPopAttrib();
    }
  }


  const Mesh::PtrType CSWCollideable::getMesh() const
  {
    return m_pCollisionMesh;
  }


  const BoundingBoxD& CSWCollideable::getBoundingBox() const
  {
    return m_BoundingBox;
  }


  void CSWCollideable::setMesh(const Mesh::PtrType pCollisionMesh)
  {
    m_pCollisionMesh = pCollisionMesh;
    m_BoundingBox = BoundingBoxD(m_pCollisionMesh->getAxisAlignedBoundingBox());
  }


  void CSWCollideable::initializeEnvironment(DT_ShapeHandle hDTShape, DT_ObjectHandle hDTObject,
                                             const Matrix44D& matObjectTCollisionShape, Mesh::PtrType pCollisionMesh)
  {
    if (m_bInitialized)
      return;
    assert(!m_hDTShape);

    m_hDTShape = hDTShape;
    m_hDTObject = hDTObject;
    m_matObjectTCollisionShape = matObjectTCollisionShape;
    m_pCollisionMesh = pCollisionMesh;

  //     Mesh::VectorConstIterator itVertexBegin = m_pCollisionMesh->getVectorRange().first; 
  //     Mesh::TriangleRange triangleRange = m_pCollisionMesh->getTriangleRange(); 
  //     Mesh::TriangleConstIterator itTriangle = triangleRange.first; 
  //     m_hDTShape = DT_NewComplexShape(NULL); 
  //     for (; itTriangle != triangleRange.second; ++itTriangle) 
  //     { 
  //       Vector3F vertex1 = static_cast<Vector3F>(*(itVertexBegin + itTriangle->indices[0])); 
  //       Vector3F vertex2 = static_cast<Vector3F>(*(itVertexBegin + itTriangle->indices[1])); 
  //       Vector3F vertex3 = static_cast<Vector3F>(*(itVertexBegin + itTriangle->indices[2])); 
  //        
  //       if (vertex1 != vertex2 && vertex2 != vertex3 && vertex1 != vertex3)  
  //       { 
  //         DT_Begin(); 
  //           DT_Vertex(vertex1.pData); 
  //           DT_Vertex(vertex2.pData); 
  //           DT_Vertex(vertex3.pData); 
  //         DT_End(); 
  //       } 
  //     } 
  //     DT_EndComplexShape(); 
  //     m_matObjectTCollisionShape = Matrix44D::IDENTITY; 
  //      
  //     m_hDTObject = DT_CreateObject(pObject.get(), m_hDTShape);

    if (!m_hDTShape)
      throw std::runtime_error("CSWCollideable::initializeEnvironment(): shape was not correctly created");

    DT_AddObject(CSWWorld::getInstance()->getScene(), m_hDTObject);
    DT_SetResponseClass(CSWWorld::getInstance()->getResponseTable(),    
                        m_hDTObject, CSWWorld::getInstance()->getResponseClass());
    m_bInitialized = true;
  }


  void CSWCollideable::prepare()
  {
    m_bHit = false;
  }


  void CSWCollideable::setupTransform(std::shared_ptr<CSWObject> pObject)
  {
    DT_SetMatrixd(m_hDTObject, (pObject->getWorldTransform()*m_matObjectTCollisionShape).pData);
  }


  const Matrix44D& CSWCollideable::getObjectTCollisionShape()
  {
    return m_matObjectTCollisionShape;
  }


  void CSWCollideable::setHit(bool bHit)
  {
    m_bHit = bHit;
  }


  bool CSWCollideable::deleteEnvironment()
  {
    if (!m_bInitialized)
      return false;

    DT_RemoveObject(CSWWorld::getInstance()->getScene(), m_hDTObject);
    DT_DestroyObject(m_hDTObject);
    DT_DeleteShape(m_hDTShape);
    m_hDTShape = NULL;
        
    m_bInitialized = false;
    return true;
  }


  CSWCollideable::CSWCollideable(const Mesh::PtrType pCollisionMesh)
  : m_pCollisionMesh(pCollisionMesh),
    m_BoundingBox(pCollisionMesh->getAxisAlignedBoundingBox()),
    m_bInitialized(false),
    m_hDTShape(NULL),
    m_bHit(false),
    m_matObjectTCollisionShape(Matrix44D::IDENTITY)
  {
    assert(m_pCollisionMesh);
  }

}
