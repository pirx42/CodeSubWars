// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "YellowMine.h"


namespace CodeSubWars
{

  YellowMine::PtrType YellowMine::create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  {
    return PtrType(new YellowMine(strName, matBaseTObject, fLength));
  }


  YellowMine::~YellowMine()
  {
  }


  YellowMine::YellowMine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWMagneticMine(strName, matBaseTObject, fLength)
  {
    setColor(Vector4D(1, 0.9, 0, 1));
  }

}