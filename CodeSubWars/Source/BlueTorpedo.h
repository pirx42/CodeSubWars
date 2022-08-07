// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSoundHomingTorpedo.h"

namespace CodeSubWars
{

  /**
   * This class is a concrete sound based homing torpedo.
   */
  class BlueTorpedo : public CSWSoundHomingTorpedo
  {
    public:
      typedef std::shared_ptr<BlueTorpedo> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      virtual ~BlueTorpedo();

      virtual bool launch(double fLaunchDelay = 0.0);

    protected:
      BlueTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
  };

}