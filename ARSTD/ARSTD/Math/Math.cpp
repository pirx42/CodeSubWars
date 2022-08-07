// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once


#include "Math.h"


namespace ARSTD
{


  double log2(double f)
  {
    static double fLog2Rec = 1.0/log(2.0);
    if (f < 1.0)
      return 0;

    return log(f)*fLog2Rec;
  }


  double log10(double f)
  {
    static double fLog10Rec = 1.0/log(10.0);
    if (f < 1.0)
      return 0;

    return log(f)*fLog10Rec;
  }


  double getInDegree(double fRad)
  {
    static double fVal = 180.0/PI;
    return fRad*fVal;
  }


  double getInRad(double fDegree)
  {
    static double fVal = PI/180.0;
    return fDegree*fVal;
  }


} // namespace ARSTD
