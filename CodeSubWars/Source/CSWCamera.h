// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWObject.h"

namespace CodeSubWars
{

  class CSWCamera : public CSWObject
  {
    public:
      typedef std::shared_ptr<CSWCamera> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject);

      virtual ~CSWCamera();

      double getFieldOfView() const;
      void setFieldOfView(double fFieldOfView);

      const Vector3D& getEyePosition() const;
      const Vector3D& getDirection() const;
      const Vector3D& getUpDirection() const;

      //in local coordinates
      void setRotationCenter(const Vector3D& vecRotationCenter);
      const Vector3D& getRotationCenter() const;

    protected:
      CSWCamera(const std::string& strName, const Matrix44D& matBaseTObject);

      double m_fFieldOfView;
      Vector3D m_vecRotationCenter;
  };

}