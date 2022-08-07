// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWHomingTorpedo.h"

namespace CodeSubWars
{

  /**
   * This class is a concrete homing torpedo.
   */
  class RedTorpedo : public CSWHomingTorpedo
  {
    public:
      typedef std::shared_ptr<RedTorpedo> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      virtual ~RedTorpedo();

      virtual bool launch(double fLaunchDelay = 0.0);

    protected:
      RedTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
  };

}