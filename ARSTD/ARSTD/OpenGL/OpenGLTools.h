// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

namespace ARSTD
{
  class OpenGLTools
  {
    public:
      static void paintTripod(const Matrix44D& mat, const double fSize = 10);

      static void paintHighLightBox(const Matrix44D& mat, const Size3D& size,
                                    const Vector4D& vecColor);

      static void paintHighLightCorner(const Vector3D& vecOrigin, const Vector3D& vecPoint1, const Vector3D& vecPoint2,
                                       const Vector3D& vecPoint3, const Vector4D& vecColor);
  };


} // namespace ARSTD