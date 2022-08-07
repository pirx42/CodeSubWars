// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <cassert>

#undef min
#undef max


#include "arstd/Common/Types.h"
#include "Constants.h"


#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include "Matrix.h"


namespace ARSTD
{

  double log2(double);
  double log10(double);

  double getInDegree(double fRad);
  double getInRad(double fDegree);

  template <typename Type>
  Type minimum(const Type& lhs, const Type& rhs)
  {
    return std::min(lhs, rhs);
  }


  template <typename Type>
  Type maximum(const Type& lhs, const Type& rhs)
  {
    return std::max(lhs, rhs);
  }


  template <typename ForwardIterator>
  std::pair<typename std::iterator_traits<ForwardIterator>::value_type,
            typename std::iterator_traits<ForwardIterator>::value_type>
  getMinMax(ForwardIterator itBegin, ForwardIterator itEnd)
  {
    typedef std::iterator_traits<ForwardIterator>::value_type ValueType;
    if (itBegin == itEnd)
      return std::make_pair(ValueType(), ValueType());

    ValueType min = std::numeric_limits<ValueType>::max();
    ValueType max = std::numeric_limits<ValueType>::min();
    for (ForwardIterator it = itBegin; it != itEnd; ++it)
    {
      min = minimum(min, *it);
      max = maximum(max, *it);
    }

    return std::make_pair(min, max);
  }
 
 }
