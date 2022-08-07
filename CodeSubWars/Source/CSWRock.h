// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDamDynSolCol.h"


namespace CodeSubWars
{

  /** 
   * This class represents a rock (or asteroid).
   */
  class CSWRock : public CSWDamDynSolCol
  {
    public:
      typedef std::shared_ptr<CSWRock> PtrType;

      virtual ~CSWRock();

    protected:
      CSWRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
    
      //other stuff
  };

}