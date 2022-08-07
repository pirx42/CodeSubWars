// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWWall.h"


namespace CodeSubWars
{

  /** 
   * This class is a concrete wall.
   */
  class Wall : public CSWWall
  {
    public:
      typedef std::shared_ptr<Wall> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~Wall();

    protected:
      Wall(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}