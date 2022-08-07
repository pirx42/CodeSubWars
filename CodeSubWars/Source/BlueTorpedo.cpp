// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "BlueTorpedo.h"
#include "CSWEngine.h"


namespace CodeSubWars
{

  BlueTorpedo::PtrType BlueTorpedo::create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  {
    return PtrType(new BlueTorpedo(strName, matBaseTObject, fLength));
  }


  BlueTorpedo::~BlueTorpedo()
  {
  }


  bool BlueTorpedo::launch(double fLaunchDelay)
  {
    getMainEngine()->setIntensity(1.0);
    return CSWSoundHomingTorpedo::launch(fLaunchDelay);
  }


  BlueTorpedo::BlueTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWSoundHomingTorpedo(strName, matBaseTObject, fLength)
  {
    setColor(Vector4D(0.1, 0, 1, 1));
  }

}