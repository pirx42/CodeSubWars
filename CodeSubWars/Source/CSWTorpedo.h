// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWWeapon.h"

namespace CodeSubWars
{

  class CSWEngine;

  /**
   * This class specializes a weapon to an torpedo. This torpedo has a main engine. The power of this engine is depend on 
   * the torpedo length. Larger torpedos have an engine with more power.
   * If it is armed and someone collides with it, it will explode. After launching the torpedo explodes automatically after
   * 60 seconds.
   */
  class CSWTorpedo : public CSWWeapon
  {
    public:
      typedef std::shared_ptr<CSWTorpedo> PtrType;

      virtual ~CSWTorpedo();

      //defined methods for dynamic
      virtual void addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls);

      virtual bool launch(double fLaunchDelay);


    protected:
      CSWTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      std::shared_ptr<CSWEngine> getMainEngine();

      //equipment
      std::shared_ptr<CSWEngine> m_pMainEngine;
  };

}