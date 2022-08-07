// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

// 
// #pragma once
// 
//namespace CodeSubWars
//{
//
// class CSWObject;
// class CSWDummy;
// 
// class CSWDetermineIntersectionWithObjects
// {
//   public:
//     //ray in world coords
//     CSWDetermineIntersectionWithObjects(std::shared_ptr<CSWObject> pSelfObject, const Ray3D& ray);
//     virtual ~CSWDetermineIntersectionWithObjects();
// 
//     void start();
//     
//     bool intersectionFound() const;
//     
//     //in world coords
//     Vector3D getNearestIntersection() const;
// 
//     std::shared_ptr<CSWObject> getIntersectingObject() const;
//     
//     double getIntersectingDistance() const;
// 
// 
//     static void collide(void* client_data, void* pObj1, void* pObj2,
//                         const DT_CollData *coll_data);
// 
//   protected:
//     CSWDetermineIntersectionWithObjects(const CSWDetermineIntersectionWithObjects& other);
//     
//     std::shared_ptr<CSWObject> m_pSelfObject;
//     DT_ShapeHandle m_dtRayShape;
//     DT_ObjectHandle m_dtObjectHandle;
//     DT_RespTableHandle m_dtResponseTableHandle;
//     std::shared_ptr<CSWDummy> m_pRayObject;
//     
//     Ray3D m_Ray;
//     Vector3D m_vecNearestIntersectionPoint;
//     double m_fNearestIntersectionDistance;
//     std::shared_ptr<CSWObject> m_pIntersectingObject;
//     bool m_bIntersecting;
// };
//
//}