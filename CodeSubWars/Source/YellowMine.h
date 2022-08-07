// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWMagneticMine.h"

namespace CodeSubWars
{

  /**
   * This class is a concrete magnetic mine.
   */
  class YellowMine : public CSWMagneticMine
  {
    public:
      typedef std::shared_ptr<YellowMine> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      virtual ~YellowMine();

    protected:
      YellowMine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
  };

}