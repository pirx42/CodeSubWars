// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWActuator.h"

namespace CodeSubWars
{

  CSWActuator::~CSWActuator()
  {
  }


  CSWActuator::CSWActuator(const std::string& strName, const Matrix44D& matBaseTObject,
                           const double& fMaxAngle, const double& fAngularVelocity)
  : CSWEquipment(strName, matBaseTObject, fMaxAngle, fAngularVelocity)
  {
  }

}