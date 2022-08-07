// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWTorpedo.h"

namespace CodeSubWars
{

  /**
   * This class is a concrete ordinary torpedo.
   */
  class GreenTorpedo : public CSWTorpedo
  {
    public:
      typedef std::shared_ptr<GreenTorpedo> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      virtual ~GreenTorpedo();

      virtual bool launch(double fLaunchDelay = 0.0);

    protected:
      GreenTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
  };

}