// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWBorder.h"


namespace CodeSubWars
{

  /** 
   * This class is a concrete border.
   */
  class Border : public CSWBorder
  {
    public:
      typedef std::shared_ptr<Border> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~Border();

    protected:
      Border(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}