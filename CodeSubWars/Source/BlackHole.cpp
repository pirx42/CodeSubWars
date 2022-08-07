// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "BlackHole.h"


namespace CodeSubWars
{

  BlackHole::PtrType BlackHole::create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  {
    return PtrType(new BlackHole(strName, matBaseTObject, size));
  }


  BlackHole::~BlackHole()
  {
  }


  BlackHole::BlackHole(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWBlackHole(strName, matBaseTObject, size)
  {
  }


}