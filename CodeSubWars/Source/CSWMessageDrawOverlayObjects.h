// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"
#include "CSWISolid.h"
#include "CSWIDamageable.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "CSWWeapon.h"
#include "glut.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageDrawOverlayObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageDrawOverlayObjects();
      virtual ~CSWMessageDrawOverlayObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);
    
    protected:
      static GLubyte m_Border[20];
  };



  //implementation

  template<typename ObjectType>
  GLubyte CSWMessageDrawOverlayObjects<ObjectType>::m_Border[20] = {0xff, 0xff, 0xff, 0xff, 0xff,
                                                                    0x80, 0x00, 0x00, 0x00, 0x40,
                                                                    0x80, 0x00, 0x00, 0x00, 0x40,
                                                                    0xff, 0xff, 0xff, 0xff, 0xff};


  template<typename ObjectType>
  CSWMessageDrawOverlayObjects<ObjectType>::CSWMessageDrawOverlayObjects()
  {
  }


  template<typename ObjectType>
  CSWMessageDrawOverlayObjects<ObjectType>::~CSWMessageDrawOverlayObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageDrawOverlayObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    if (!(CSWWorld::getInstance()->getSettings()->getDamageableProperties() & CSWSettings::DISPLAY_HEALTH) &&
        !(CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::INCREASE_VISIBILITY_OF_SMALL_OBJECTS) &&
        CSWWorld::getInstance()->getSettings()->getSelectedObject() != pObject)
    return;

    AxisAlignedBoundingBoxD bbObject;  
    Vector3D vecWindowTPosition;
    Vector3D vecWindowTCenterPosition;
    if (CSWISolid::PtrType pSolid = std::dynamic_pointer_cast<CSWISolid>(pObject))
    {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadMatrixd(pObject->getWorldTransform().pData);
      std::vector<Vector3D> points(9);
      points[0] = Vector3D(0, 0, 0);
      for (int i = 1; i < points.size(); ++i) points[i] = pSolid->getSurfaceBoundingBox().getPoint(i - 1);
      std::vector<Vector3D> windowPoints = CSWUtilities::getWindowCoords(points);
      if (windowPoints.empty())
        return;

      bbObject = ARSTD::AxisAlignedBoundingBoxD::buildAroundPoints(windowPoints);
      vecWindowTPosition = Vector3D(windowPoints[0].x, bbObject.getMin().y, windowPoints[0].z);
      vecWindowTCenterPosition = windowPoints[0];
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }
    else
    {
      std::pair<bool, Vector3D> result = CSWUtilities::getWindowCoords(pObject->getWorldTransform().getTranslation());
      if (!result.first)
        return;
      bbObject.setMin(result.second);
      bbObject.setMax(result.second);
      vecWindowTPosition = result.second;
      vecWindowTCenterPosition = result.second;
    }

    if (vecWindowTCenterPosition.z < 0 || vecWindowTCenterPosition.z >= 1)
      return;
  
    glPushAttrib(GL_ALL_ATTRIB_BITS);

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
    
      int viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(0, viewport[2], 0, viewport[3], -10, 10);

      if (CSWWorld::getInstance()->getSettings()->getDamageableProperties() & CSWSettings::DISPLAY_HEALTH &&
          !std::dynamic_pointer_cast<CSWWeapon>(pObject))
      {
        if (CSWIDamageable::PtrType pDamageableObject = std::dynamic_pointer_cast<CSWIDamageable>(pObject))
        {
          GLubyte bar[8];
        
          unsigned __int32 val = (1 << (31 - static_cast<int>(pDamageableObject->getHealth()*31))) - 1;
          val = ~val;
          bar[0] = (val & 0xff000000) >> 24;
          bar[1] = (val & 0x00ff0000) >> 16;
          bar[2] = (val & 0x0000ff00) >> 8;
          bar[3] = (val & 0x000000ff);
          bar[4] = bar[0]; bar[5] = bar[1]; bar[6] = bar[2]; bar[7] = bar[3];

          Vector3D vecColor(0, 0, 0);
          vecColor.r = pDamageableObject->getHealth() < 0.5 ? 1.0 : -2.0*pDamageableObject->getHealth() + 2;
          vecColor.g = pDamageableObject->getHealth() < 0.5 ? 2.0*pDamageableObject->getHealth() : 1.0;

          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

          glColor3dv(vecColor.pData);
          glRasterPos3i(vecWindowTPosition.x, vecWindowTPosition.y - 20, vecWindowTPosition.z);
          glBitmap(32, 2, 16.0, 1.0, 0.0, 0.0, bar);

          glColor3d(0.4, 0.4, 0.4);
          //seting up the raster position again makes the color to be set!
          glRasterPos3i(vecWindowTPosition.x, vecWindowTPosition.y - 20, vecWindowTPosition.z);
          glBitmap(34, 4, 17.0, 2.0, 0.0, 0.0, m_Border);
        }
      }
            
      if (CSWWorld::getInstance()->getSettings()->getSelectedObject() == pObject)
      {
        glColor3d(1.0, 1.0, 1.0);
        glRasterPos3i(vecWindowTPosition.x - pObject->getName().size()*9/2, vecWindowTPosition.y - 40, vecWindowTPosition.z);
        glListBase(1000);
        glCallLists((GLsizei)pObject->getName().size(), GL_UNSIGNED_BYTE, pObject->getName().c_str()); 
      }

      double fSquaredSize = bbObject.getWidth()*bbObject.getWidth() + bbObject.getHeight()*bbObject.getHeight();
      if ((CSWWorld::getInstance()->getSettings()->getVariousProperties() & CSWSettings::INCREASE_VISIBILITY_OF_SMALL_OBJECTS) &&
          fSquaredSize < 144 &&
          std::dynamic_pointer_cast<CSWISolid>(pObject))
      {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadMatrixd(Matrix44D(vecWindowTCenterPosition).pData);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(false);
        glColor4d(1.0, 1.0, 0.0, fSquaredSize < 64 ? 0.3 : -0.3/80*fSquaredSize + 0.54);

        GLUquadricObj* pSphere = gluNewQuadric();
        gluQuadricDrawStyle(pSphere, GLU_FILL);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gluDisk(pSphere, 0.0, 4.0, 10, 1);
        gluDeleteQuadric(pSphere);

        glDepthMask(true);
        glPopMatrix();
      }

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();

    glPopAttrib();
  }

}