// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWRock.h"


namespace CodeSubWars
{

  /** 
   * This class is a concrete rock.
   */
  class Rock : public CSWRock
  {
    public:
      typedef std::shared_ptr<Rock> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~Rock();

    protected:
      Rock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}