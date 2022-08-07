// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "RedTorpedo.h"
#include "CSWEngine.h"


namespace CodeSubWars
{

  RedTorpedo::PtrType RedTorpedo::create(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  {
    return PtrType(new RedTorpedo(strName, matBaseTObject, fLength));
  }


  RedTorpedo::~RedTorpedo()
  {
  }


  bool RedTorpedo::launch(double fLaunchDelay)
  {
    getMainEngine()->setIntensity(1.0);
    return CSWHomingTorpedo::launch(fLaunchDelay);
  }


  RedTorpedo::RedTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWHomingTorpedo(strName, matBaseTObject, fLength)
  {
    setColor(Vector4D(1, 0.1, 0, 1));
  }

}