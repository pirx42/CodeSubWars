// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWActiveRock.h"


namespace CodeSubWars
{

  class ActiveRock : public CSWActiveRock
  {
    public:
      typedef std::shared_ptr<ActiveRock> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~ActiveRock();

    protected:
      ActiveRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}