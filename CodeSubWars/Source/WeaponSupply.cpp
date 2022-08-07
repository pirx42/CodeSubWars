// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "WeaponSupply.h"


namespace CodeSubWars
{

  WeaponSupply::PtrType WeaponSupply::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new WeaponSupply(strName, matBaseTObject, size));
  }


  WeaponSupply::~WeaponSupply()
  {
  }


  WeaponSupply::WeaponSupply(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWWeaponSupply(strName, matBaseTObject, size)
  {
  }


}