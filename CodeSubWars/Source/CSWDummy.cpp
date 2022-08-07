// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWObject.h"
#include "CSWDummy.h"


namespace CodeSubWars
{

  CSWDummy::PtrType CSWDummy::create(const std::string& strName)
  {
    return PtrType(new CSWDummy(strName));
  }


  CSWDummy::~CSWDummy()
  {
  }



  CSWDummy::CSWDummy(const std::string& strName)
  : CSWObject(strName)
  {
  }


}