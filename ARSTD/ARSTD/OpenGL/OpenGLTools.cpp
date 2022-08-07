// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "windows.h"
#include "gl/gl.h"
#include "arstd/Math/Math.h"
#include "arstd/Common/Size3.h"
#include "OpenGLTools.h"
#include "arstd/Geometry/BoundingBox.h"

namespace ARSTD
{


  void OpenGLTools::paintTripod(const ARSTD::Matrix44D& mat, const double fSize)
  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glPushMatrix();
      glLoadMatrixd(mat.pData);

      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);

      glLineWidth(2);
        glBegin(GL_LINES);
        glColor4d(1.0, 0, 0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(fSize, 0.0, 0.0);

        glColor4d(0, 1.0, 0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, fSize, 0.0);

        glColor4d(0, 0, 1.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, fSize);
      glEnd();

      glPopMatrix();
    glPopAttrib();
  }


  void OpenGLTools::paintHighLightBox(const ARSTD::Matrix44D& mat, const Size3D& size, const Vector4D& vecColor)
  {
    Matrix44D tmp(mat);
    tmp.getTranslation() -= tmp.getXAxis()*size.getWidth()/2.0;
    tmp.getTranslation() -= tmp.getYAxis()*size.getHeight()/2.0;
    tmp.getTranslation() -= tmp.getZAxis()*size.getDepth()/2.0;
    BoundingBoxD bbox(tmp, size);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glPushMatrix();
      glLoadIdentity();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        paintHighLightCorner(bbox.getPoint(0), bbox.getPoint(1), bbox.getPoint(3), bbox.getPoint(4), vecColor);
        paintHighLightCorner(bbox.getPoint(1), bbox.getPoint(0), bbox.getPoint(2), bbox.getPoint(5), vecColor);
        paintHighLightCorner(bbox.getPoint(2), bbox.getPoint(1), bbox.getPoint(3), bbox.getPoint(6), vecColor);
        paintHighLightCorner(bbox.getPoint(3), bbox.getPoint(0), bbox.getPoint(2), bbox.getPoint(7), vecColor);
        paintHighLightCorner(bbox.getPoint(4), bbox.getPoint(0), bbox.getPoint(5), bbox.getPoint(7), vecColor);
        paintHighLightCorner(bbox.getPoint(5), bbox.getPoint(1), bbox.getPoint(4), bbox.getPoint(6), vecColor);
        paintHighLightCorner(bbox.getPoint(6), bbox.getPoint(2), bbox.getPoint(5), bbox.getPoint(7), vecColor);
        paintHighLightCorner(bbox.getPoint(7), bbox.getPoint(3), bbox.getPoint(4), bbox.getPoint(6), vecColor);
      glPopMatrix();
    glPopAttrib();
  }


  void OpenGLTools::paintHighLightCorner(const Vector3D& vecOrigin, const Vector3D& vecPoint1, const Vector3D& vecPoint2,
                                         const Vector3D& vecPoint3, const Vector4D& vecColor)
  {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);

      glColor4dv(vecColor.pData);
      glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3dv(vecOrigin.pData);
        glVertex3dv((vecOrigin + (vecPoint1 - vecOrigin)*0.2).pData);

        glVertex3dv(vecOrigin.pData);
        glVertex3dv((vecOrigin + (vecPoint2 - vecOrigin)*0.2).pData);

        glVertex3dv(vecOrigin.pData);
        glVertex3dv((vecOrigin + (vecPoint3 - vecOrigin)*0.2).pData);
      glEnd();

    glPopAttrib();
  }


} // namespace ARSTD