// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Rock.h"


namespace CodeSubWars
{

  Rock::PtrType Rock::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new Rock(strName, matBaseTObject, size));
  }


  Rock::~Rock()
  {
  }


  Rock::Rock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWRock(strName, matBaseTObject, size)
  {
  }


}