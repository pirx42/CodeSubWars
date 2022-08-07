// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWWeaponSupply.h"


namespace CodeSubWars
{

  /** 
   * This class is a concrete resource provider for weapons.
   */
  class WeaponSupply : public CSWWeaponSupply
  {
    public:
      typedef std::shared_ptr<WeaponSupply> PtrType;
    
      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      virtual ~WeaponSupply();

    protected:
      WeaponSupply(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
  };

}