// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Wall.h"


namespace CodeSubWars
{

  Wall::PtrType Wall::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new Wall(strName, matBaseTObject, size));
  }


  Wall::~Wall()
  {
  }


  Wall::Wall(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWWall(strName, matBaseTObject, size)
  {
  }


}