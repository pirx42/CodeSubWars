// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Border.h"


namespace CodeSubWars
{

  Border::PtrType Border::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new Border(strName, matBaseTObject, size));
  }


  Border::~Border()
  {
  }


  Border::Border(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWBorder(strName, matBaseTObject, size)
  {
  }


}