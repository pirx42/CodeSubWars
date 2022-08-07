// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSensor.h"

namespace CodeSubWars
{

  CSWSensor::~CSWSensor()
  {
  }


  CSWSensor::CSWSensor(const std::string& strName, const Matrix44D& matBaseTObject,
                       const double& fMaxAngle, const double& fAngularVelocity)
  : CSWEquipment(strName, matBaseTObject, fMaxAngle, fAngularVelocity)
  {
  }

}