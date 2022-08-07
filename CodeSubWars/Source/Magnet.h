// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWMagnet.h"


namespace CodeSubWars
{

  class Magnet : public CSWMagnet
  {
    public:
      typedef std::shared_ptr<Magnet> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~Magnet();

    protected:
      Magnet(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}