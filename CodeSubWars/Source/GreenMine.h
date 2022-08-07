// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWMine.h"

namespace CodeSubWars
{

  /**
   * This class is a concrete ordinary mine.
   */
  class GreenMine : public CSWMine
  {
    public:
      typedef std::shared_ptr<GreenMine> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      virtual ~GreenMine();

    protected:
      GreenMine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
  };

}