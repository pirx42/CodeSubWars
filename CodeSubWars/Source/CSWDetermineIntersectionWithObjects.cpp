// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

 
 #include "PrecompiledHeader.h"
// #include "CSWWorld.h"
// #include "CSWObject.h"
// #include "CSWDummy.h"
// #include "CSWISolid.h"
// #include "CSWICollideable.h"
// #include "CSWDetermineIntersectionWithObjects.h"
// #include "CSWMessageCollisionObjects.h"
// 
// 
//namespace CodeSubWars
//{
//
// 
// CSWDetermineIntersectionWithObjects::CSWDetermineIntersectionWithObjects(CSWObject::PtrType pSelfObject, const Ray3D& ray)
// : m_pSelfObject(pSelfObject),
//   m_pRayObject(CSWDummy::create("RayObject")),
//   m_Ray(ray),
//   m_vecNearestIntersectionPoint(std::numeric_limits<double>::max(), 
//                                 std::numeric_limits<double>::max(), 
//                                 std::numeric_limits<double>::max()),
//   m_fNearestIntersectionDistance(std::numeric_limits<double>::max()),
//   m_bIntersecting(false)
// {
//   double fCylinderLength = 3000;
//   //the cylinder shape will be centered at its origin and points along the y-axis
//   m_dtRayShape = NULL;
//   m_dtRayShape = DT_NewCylinder(0.5, fCylinderLength);
//   if (!m_dtRayShape)
//     throw std::runtime_error("could not create cylinder shape");
//   m_dtObjectHandle = DT_CreateObject(m_pRayObject.get(), m_dtRayShape);
//   if (!m_dtObjectHandle)
//     throw std::runtime_error("could not create object");
// 
//   Matrix44D matWorldTRay;
//   matWorldTRay.getYAxis() = ray.getDir();
//   matWorldTRay.getXAxis() = matWorldTRay.getYAxis().getPerpendicular();
//   matWorldTRay.getZAxis() = matWorldTRay.getXAxis() % matWorldTRay.getYAxis();
//   matWorldTRay.getTranslation() = ray.getPos() + ray.getDir()*fCylinderLength*0.5;
//   DT_SetMatrixd(m_dtObjectHandle, matWorldTRay.pData);
// 
//   m_dtResponseTableHandle = DT_CreateRespTable();
//   
//   dtSetObjectResponse(m_pRayObject.get(), &(CSWDetermineIntersectionWithObjects::collide), DT_WITNESSED_RESPONSE, this);
//   
// }
// 
// 
// CSWDetermineIntersectionWithObjects::~CSWDetermineIntersectionWithObjects()
// {
//   dtResetObjectResponse(m_pRayObject.get());
//   DT_DestroyObject(m_dtObjectHandle);
//   dtDeleteShape(m_dtRayShape);
// }
// 
// 
// void CSWDetermineIntersectionWithObjects::start()
// {
//   //clear default response so that only ray with all other object will be tested
//   dtClearDefaultResponse();
// 
//   dtTest();
//   
//   if (m_bIntersecting)
//     m_fNearestIntersectionDistance = sqrt(m_fNearestIntersectionDistance);
// 
//   //restore the default response
//   dtSetDefaultResponse(&(CSWMessageInitializeCollisionObjects::collide), DT_SMART_RESPONSE, NULL);  
// }
// 
// bool CSWDetermineIntersectionWithObjects::intersectionFound() const
// {
//   return m_bIntersecting;
// }
// 
// 
// Vector3D CSWDetermineIntersectionWithObjects::getNearestIntersection() const
// {
//   return m_vecNearestIntersectionPoint;
// }
// 
// 
// CSWObject::PtrType CSWDetermineIntersectionWithObjects::getIntersectingObject() const
// {
//   if (!m_bIntersecting)
//     return CSWObject::PtrType();
//   
//   assert(m_pIntersectingObject);
//   return m_pIntersectingObject;
// }
// 
// 
// double CSWDetermineIntersectionWithObjects::getIntersectingDistance() const
// {
//   return m_fNearestIntersectionDistance;
// }
// 
// 
// void CSWDetermineIntersectionWithObjects::collide(void* client_data, DtObjectRef pObj1, DtObjectRef pObj2,
//                                                   const DtCollData *coll_data)
// {
//   if (!pObj1 || !pObj2)
//     return;
// 
//   CSWDetermineIntersectionWithObjects* pThis = reinterpret_cast<CSWDetermineIntersectionWithObjects*>(client_data);
//   assert(pThis);
// 
//   CSWObject* pObjectA = reinterpret_cast<CSWObject*>(pObj1);
//   CSWObject* pObjectB = reinterpret_cast<CSWObject*>(pObj2);
//   assert(pObjectA && pObjectB);
//   if (!pObjectA || !pObjectB)
//     return;
// 
//   CSWObject::PtrType pSharedObjectA = pObjectA->getSharedThis();
//   CSWObject::PtrType pSharedObjectB = pObjectB->getSharedThis();
// 
//   //check if the objects exists. it could be possible that objects died and therefor removed while collision detection. 
//   //to prevent solving collision of removed objects break here.
//   if (!pSharedObjectA || !pSharedObjectB)
//     return;
// 
//   if (pSharedObjectA == pThis->m_pSelfObject || pSharedObjectB == pThis->m_pSelfObject)
//     return;
//   
//   if (pSharedObjectA != pThis->m_pRayObject && pSharedObjectB != pThis->m_pRayObject)
//     throw std::runtime_error("one of the colliding objects must be the ray object");
// 
//   Vector3D vecObjectTP;
//   CSWObject::PtrType pObject;
//   if (std::dynamic_pointer_cast<CSWICollideable>(pSharedObjectA))
//   {
//     pObject = pSharedObjectA;
//     vecObjectTP = Vector3D(coll_data->point1[0], coll_data->point1[1], coll_data->point1[2]);
//   }
//   else if (std::dynamic_pointer_cast<CSWICollideable>(pSharedObjectB))
//   {
//     pObject = pSharedObjectB;
//     vecObjectTP = Vector3D(coll_data->point2[0], coll_data->point2[1], coll_data->point2[2]);
//   }
//   else 
//   {
//     throw std::runtime_error("no found object is an ordinary collidable object of the world");
//   }
//   assert(pObject);
// 
//   Vector3D vecWorldTP = pObject->getWorldTransform()*vecObjectTP;
//   double fCurrentSquaredDistance = (pThis->m_Ray.getPos() - vecWorldTP).getSquaredLength();
//   if (fCurrentSquaredDistance < pThis->m_fNearestIntersectionDistance)
//   {
//     pThis->m_fNearestIntersectionDistance = fCurrentSquaredDistance;
//     pThis->m_pIntersectingObject = pObject;
//     pThis->m_vecNearestIntersectionPoint = vecWorldTP;
//     pThis->m_bIntersecting = true;
//   }
//   
// }
//
//}