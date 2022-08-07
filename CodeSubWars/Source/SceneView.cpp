// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWSettings.h"
#include "CSWWorld.h"
#include "CSWCamera.h"
#include "SceneView.h"
#include "CSWISolid.h"
#include "CSWDetermineIntersectionWithObjects.h"
#include "CSWBorder.h"
#include "CSWTrigger.h"


namespace CodeSubWars
{

  SceneView::SceneView(QWidget* parent, bool bAbsolutCameraSelection,
                       const QGLWidget* pShareWidget, Qt::WindowFlags f)
  : OpenGLView(parent, pShareWidget, f),
    m_bAbsolutCameraSelection(bAbsolutCameraSelection)
  {
  }


  SceneView::~SceneView()
  {
  }


  void SceneView::rebuild()
  {
    if (m_bAbsolutCameraSelection)
    {
      CSWCamera::PtrType pCam = m_pCamera.lock();
      if (!pCam || (pCam && pCam->getName() != m_strObjectPostfix))
        m_pCamera = std::dynamic_pointer_cast<CSWCamera>(CSWObject::findElement(CSWWorld::getInstance()->getObjectTree(), m_strObjectPostfix));
    }  
    else if (CSWObject::PtrType pObj = CSWWorld::getInstance()->getSettings()->getSelectedObject())
    {
      CSWObject::PtrType pLastSelectedObj = m_pLastSelectedObject.lock();
      if (pLastSelectedObj == pObj || std::dynamic_pointer_cast<CSWCamera>(pObj))
        return;
    
      CSWCamera::PtrType pCam = m_pCamera.lock();
      if (pLastSelectedObj && pCam)
        pLastSelectedObj->detach(pCam);
    
      Size3D size(0, 5, 20);
      if (CSWISolid::PtrType pSolidObj = std::dynamic_pointer_cast<CSWISolid>(pObj))
        size = pSolidObj->getSurfaceBoundingBox().getSize();
    
      Matrix44D mat;
      double fZ = -size.getDepth()*1.3;
      if (fZ > -3)
        fZ = -3;
      mat.getTranslation() = Vector3D(0, size.getHeight()*1.0, fZ);
      mat.getXAxis() = Vector3D(1, 0, 0);
      mat.getZAxis() = mat.getTranslation().getNormalized()*-1.0;
      mat.getYAxis() = mat.getZAxis() % mat.getXAxis();
      pCam = CSWCamera::create(pObj->getName() + ".3rdPersonCamera", mat);
      pObj->attach(pCam);
    
      m_pCamera = pCam;
      m_pLastSelectedObject = pObj;
    }
  }


  void SceneView::setCamera(const std::string& strName)
  {
    m_strObjectPostfix = strName;
    rebuild();  
  }


  void SceneView::draw()
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
      setupCameraAndLight(pCam);

      //draw the world
      CSWWorld::getInstance()->draw();

      //draw world coord tripod in lower left corner
      drawWorldTripod();
    
    glPopAttrib();
  }


  void SceneView::mousePressEvent(QMouseEvent* ev)
  { 
    OpenGLView::mousePressEvent(ev);

    // calculate the volume position where the mouse was pressed
    std::pair<bool, Ray3D> resultRay = getWorldPickRay(ev->x(), ev->y());
    if (resultRay.first)
    {
      //handle mouse pressing
      float fIntersection = 0;
      Vector3F vecNormal;
      void* pCastResult = DT_RayCast(CSWWorld::getInstance()->getScene(), NULL,
                                     static_cast<Vector3F>(resultRay.second.getPos()).pData,
                                     static_cast<Vector3F>(resultRay.second.getPos() + resultRay.second.getDir()*100000.0).pData,
                                     1.0, &fIntersection, vecNormal.pData);    
      CSWObject* pObj = reinterpret_cast<CSWObject*>(pCastResult);
      if (!pObj)
        return;
    
      CSWObject::PtrType pObject = pObj->getSharedThis();
       if (!std::dynamic_pointer_cast<CSWBorder>(pObject) && //the world border should not be selectable by clicking on it
           !std::dynamic_pointer_cast<CSWTrigger>(pObject) && //the trigger objects should not be selectable by clicking on it
           pObject != CSWWorld::getInstance()->getSettings()->getSelectedObject())
      {
        CSWWorld::getInstance()->getSettings()->setSelectedObject(pObject);
        emit selectionChanged();
      }    
    }
  }

}

#include "SceneViewMoc.cpp"
