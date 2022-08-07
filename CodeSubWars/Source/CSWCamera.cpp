// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWCamera.h"


namespace CodeSubWars
{

  CSWCamera::PtrType CSWCamera::create(const std::string& strName, const Matrix44D& matBaseTObject)
  {
    return PtrType(new CSWCamera(strName, matBaseTObject));
  }


  CSWCamera::~CSWCamera()
  {
  }


  double CSWCamera::getFieldOfView() const
  {
    return m_fFieldOfView;
  }


  void CSWCamera::setFieldOfView(double fFieldOfView)
  {
    m_fFieldOfView = fFieldOfView;
  }


  void CSWCamera::setRotationCenter(const Vector3D& vecRotationCenter)
  {
    m_vecRotationCenter = vecRotationCenter;
  }


  const Vector3D& CSWCamera::getRotationCenter() const
  {
    return m_vecRotationCenter;
  }


  const Vector3D& CSWCamera::getEyePosition() const
  {
    return getWorldTransform().getTranslation();
  }


  const Vector3D& CSWCamera::getDirection() const
  {
    return getWorldTransform().getZAxis();
  }


  const Vector3D& CSWCamera::getUpDirection() const
  {
    return getWorldTransform().getYAxis();
  }


  CSWCamera::CSWCamera(const std::string& strName, const Matrix44D& matBaseTObject)
  : CSWObject(strName, matBaseTObject),
    m_fFieldOfView(45),
    m_vecRotationCenter(0, 0, 0)
  {
  }


}