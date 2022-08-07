// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "GreenMine.h"


namespace CodeSubWars
{

  GreenMine::PtrType GreenMine::create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  {
    return PtrType(new GreenMine(strName, matBaseTObject, fLength));
  }


  GreenMine::~GreenMine()
  {
  }


  GreenMine::GreenMine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWMine(strName, matBaseTObject, fLength)
  {
    setColor(Vector4D(0, 1, 0.1, 1));
  }

}