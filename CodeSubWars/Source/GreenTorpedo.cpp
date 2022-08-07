// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "GreenTorpedo.h"
#include "CSWEngine.h"


namespace CodeSubWars
{

  GreenTorpedo::PtrType GreenTorpedo::create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  {
    return PtrType(new GreenTorpedo(strName, matBaseTObject, fLength));
  }


  GreenTorpedo::~GreenTorpedo()
  {
  }


  bool GreenTorpedo::launch(double fLaunchDelay)
  {
    getMainEngine()->setIntensity(1.0);
    return CSWTorpedo::launch(fLaunchDelay);
  }


  GreenTorpedo::GreenTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWTorpedo(strName, matBaseTObject, fLength)
  {
    setColor(Vector4D(0, 1, 0.1, 1));
  }


}