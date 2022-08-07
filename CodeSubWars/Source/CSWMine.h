// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWWeapon.h"

namespace CodeSubWars
{

  //class CSWScanBox;

  /**
   * This class specializes a weapon to an ordinary mine. Ones the mine has been released it will explode if it is armed and 
   * something collides with it.
   */
  class CSWMine : public CSWWeapon
  {
    public:
      typedef std::shared_ptr<CSWMine> PtrType;

      virtual ~CSWMine();

  //    virtual void draw();

      //defined methods for dynamic
      virtual void addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls);

    protected:
      CSWMine(const std::string& strName, const Matrix44D& matBaseTObject, double fEdgeLength);

  //    std::shared_ptr<CSWScanBox> m_pScanBox;
  };

}