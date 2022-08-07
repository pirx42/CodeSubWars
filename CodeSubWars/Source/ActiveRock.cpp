// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "ActiveRock.h"


namespace CodeSubWars
{

  ActiveRock::PtrType ActiveRock::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new ActiveRock(strName, matBaseTObject, size));
  }


  ActiveRock::~ActiveRock()
  {
  }


  ActiveRock::ActiveRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWActiveRock(strName, matBaseTObject, size)
  {
  }


}