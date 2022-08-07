// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorld.h"
#include "ReplayView.h"
#include "CSWCamera.h"
#include "ReplayDialog.h"
#include "IO/CSWBRImporter.h"


namespace CodeSubWars
{

  ReplayView::ReplayView(ReplayDialog* pReplayDialog)
  : OpenGLView(pReplayDialog),
    m_pReplayDialog(pReplayDialog),
    m_pMainCamera(CSWCamera::create("MainCamera", Matrix44D(Vector3D(1, 0, 0),
                                                            Vector3D(0, 0, 1),
                                                            Vector3D(0, -1, 0),
                                                            Vector3D(0, 2300, 0))))
  {
    m_pCamera = m_pMainCamera;
  }


  void ReplayView::draw()
  {
    CSWCamera::PtrType pCam = m_pCamera.lock();
    if (!pCam)
      return;
  
    glPushAttrib(GL_ALL_ATTRIB_BITS);
      setupCameraAndLight(pCam);

      glShadeModel(GL_SMOOTH);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);

      glPolygonMode(GL_FRONT, GL_FILL);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);

      CSWBRImporter::TimeSliceData data = m_pReplayDialog->getData();
      CSWBRImporter::TimeSliceData::const_iterator it = data.begin();
      CSWBRImporter::TimeSliceData::const_iterator itEnd = data.end();
      for (; it != itEnd; ++it)
      {
        if (it->second.strName.find("envBorder") == std::string::npos)
        {
          glColor4dv(it->second.color.pData);
          ARSTD::OpenGLTools::paintTripod(it->second.matWorldTObject, 30);
          Matrix44D mat(it->second.matWorldTObject);
          mat.getTranslation() -= it->second.matWorldTObject.getRotationAsMatrix33()*Vector3D(it->second.size.getWidth()*0.5,
                                                                                              it->second.size.getHeight()*0.5,
                                                                                              it->second.size.getDepth()*0.5);
          Mesh::createBox(mat, it->second.size)->render();
        }
      }


      //draw world cube
      glPushAttrib(GL_ALL_ATTRIB_BITS);

        glDisable(GL_CULL_FACE);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_DIFFUSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glShadeModel(GL_FLAT);
        glDisable(GL_LIGHTING);
        glColor4d(0, 0, 1, 0.2);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(false);

        BoundingBoxD bbox(Matrix44D(Vector3D(-CSWWorld::CUBE_SIZE.getDepth()/2 + CSWWorld::CUBE_THICKNESS, 
                                             -CSWWorld::CUBE_SIZE.getHeight()/2 + CSWWorld::CUBE_THICKNESS, 
                                             -CSWWorld::CUBE_SIZE.getDepth()/2 + CSWWorld::CUBE_THICKNESS)), 
                          Size3D(CSWWorld::CUBE_SIZE.getWidth() - CSWWorld::CUBE_THICKNESS*2, 
                                 CSWWorld::CUBE_SIZE.getHeight() - CSWWorld::CUBE_THICKNESS*2, 
                                 CSWWorld::CUBE_SIZE.getDepth() - CSWWorld::CUBE_THICKNESS*2));

        glBegin(GL_QUADS);
          for (int iCnt = 0; iCnt < 6; ++iCnt) 
          {
            glVertex3dv(bbox.getRectangle(iCnt).getPoint0().pData);
            glVertex3dv(bbox.getRectangle(iCnt).getPoint1().pData);
            glVertex3dv(bbox.getRectangle(iCnt).getPoint2().pData);
            glVertex3dv(bbox.getRectangle(iCnt).getPoint3().pData);
          } 
        glEnd();
  //      glDisable(GL_BLEND);

        glDisable(GL_LIGHTING);
        glColor3d(0, 0, 1);
        glLineWidth(2);
        glBegin(GL_LINES);
          for (int iCnt = 0; iCnt < 12; ++iCnt) 
          {
            glVertex3dv(bbox.getSegment(iCnt).getPoint1().pData);
            glVertex3dv(bbox.getSegment(iCnt).getPoint2().pData);
          } 
        glEnd();
        glDepthMask(true);
      glPopAttrib();



      //draw world coord tripod in lower left corner
      drawWorldTripod();

    glPopAttrib();
  }

}