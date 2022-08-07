// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Magnet.h"


namespace CodeSubWars
{

  Magnet::PtrType Magnet::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new Magnet(strName, matBaseTObject, size));
  }


  Magnet::~Magnet()
  {
  }


  Magnet::Magnet(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWMagnet(strName, matBaseTObject, size)
  {
  }


}