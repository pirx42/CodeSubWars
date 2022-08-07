// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"
#include "CSWISolid.h"
#include "CSWWorld.h"
#include "CSWSettings.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageDrawObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageDrawObjects();
      virtual ~CSWMessageDrawObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);
  };



  //implementation

  template<typename ObjectType>
  CSWMessageDrawObjects<ObjectType>::CSWMessageDrawObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageDrawObjects<ObjectType>::~CSWMessageDrawObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageDrawObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    if (CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::DISPLAY_OBJECT_TREE)
    {
      if (CSWObject* pParent = dynamic_cast<CSWObject*>(pObject->getParent()))
      {
        //"debuging" line that connects child with parent
        glPushAttrib(GL_ALL_ATTRIB_BITS);
          glDisable(GL_LIGHTING);
          glShadeModel(GL_SMOOTH);
          glLineWidth(1);
          glColor3f(1.0, 0.0, 0.0); 
          glBegin(GL_LINES);
            glVertex3dv(pParent->getWorldTransform().getTranslation().pData);
            glVertex3dv(pObject->getWorldTransform().getTranslation().pData);
          glEnd();
        glPopAttrib();
      }
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glPushMatrix();
      glLoadMatrixd(pObject->getWorldTransform().pData);

      Size3D objSize(10, 10, 10);
      if (CSWISolid::PtrType pSolidObject = std::dynamic_pointer_cast<CSWISolid>(pObject))
      {
        pSolidObject->draw();
        objSize = pSolidObject->getSurfaceBoundingBox().getSize()*1.5;
      }

      if (CSWWorld::getInstance()->getSettings()->getSelectedObject() == pObject)
      {
        glDisable(GL_DEPTH_TEST);
        double fTripodLength = std::max(std::max(objSize.getWidth(), objSize.getHeight()), objSize.getDepth())*0.1;
        ARSTD::OpenGLTools::paintTripod(pObject->getWorldTransform(), fTripodLength);
      }

      if (CSWWorld::getInstance()->getSettings()->getSelectedSolidObject() == pObject)
      {
        glEnable(GL_DEPTH_TEST);
        ARSTD::OpenGLTools::paintHighLightBox(pObject->getWorldTransform(), objSize, Vector4D(1, 0.9, 0, 0.6));
      }

      glPopMatrix();
    glPopAttrib();
  }

}