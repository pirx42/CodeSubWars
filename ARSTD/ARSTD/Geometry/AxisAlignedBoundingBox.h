// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include <vector>
#include "arstd/Common/Size3.h"

namespace ARSTD
{

  template <typename Type>
  class AxisAlignedBoundingBox
  {
    public:
      static const AxisAlignedBoundingBox<Type> buildAroundPoints(const std::vector<Vector3<Type>>& pointContainer);

      AxisAlignedBoundingBox();
      AxisAlignedBoundingBox(const Vector3<Type>& vecMin, const Vector3<Type>& vecMax);
      AxisAlignedBoundingBox(const Vector3<Type>& vecMin, const Size3<Type>& size);

      template <typename Type2>
      explicit AxisAlignedBoundingBox(const AxisAlignedBoundingBox<Type2>& other)
      : m_vecMin(static_cast<Vector3<Type>>(other.getMin())),
        m_vecMax(static_cast<Vector3<Type>>(other.getMax()))
      {
      }
      
      const Vector3<Type>& getMin() const;
      void setMin(const Vector3<Type>& vecMin);

      const Vector3<Type>& getMax() const;
      void setMax(const Vector3<Type>& vecMax);

      const Size3<Type> getSize() const;

      const Type getWidth() const;
      const Type getHeight() const;
      const Type getDepth() const;

    protected:
      Vector3<Type> m_vecMin;
      Vector3<Type> m_vecMax;
   };


  typedef AxisAlignedBoundingBox<double> AxisAlignedBoundingBoxD;



  template <typename Type>
  AxisAlignedBoundingBox<Type>::AxisAlignedBoundingBox()
  {
  }


  template <typename Type>
  AxisAlignedBoundingBox<Type>::AxisAlignedBoundingBox(const Vector3<Type>& vecMin, const Vector3<Type>& vecMax)
  : m_vecMin(vecMin),
    m_vecMax(vecMax)
  {
  }


  template <typename Type>
  AxisAlignedBoundingBox<Type>::AxisAlignedBoundingBox(const Vector3<Type>& vecMin, const Size3<Type>& size)
  : m_vecMin(vecMin),
    m_vecMax(vecMin + Vector3<Type>(size.getWidth(), size.getHeight(), size.getDepth()))
  {
    assert(size.getWidth() >= 0 && size.getHeight() >= 0 && size.getDepth() >= 0);
  }


  template <typename Type>
  const Vector3<Type>& AxisAlignedBoundingBox<Type>::getMin() const
  {
    return m_vecMin;
  }


  template <typename Type>
  void AxisAlignedBoundingBox<Type>::setMin(const Vector3<Type>& vecMin)
  {
    m_vecMin = vecMin;
  }


  template <typename Type>
  const Vector3<Type>& AxisAlignedBoundingBox<Type>::getMax() const
  {
    return m_vecMax;
  }


  template <typename Type>
  void AxisAlignedBoundingBox<Type>::setMax(const Vector3<Type>& vecMax)
  {
    m_vecMax = vecMax;
  }


  template <typename Type>
  const Size3<Type> AxisAlignedBoundingBox<Type>::getSize() const
  {
    return Size3<Type>(m_vecMax - m_vecMin);
  }


  template <typename Type>
  const Type AxisAlignedBoundingBox<Type>::getWidth() const
  {
    return m_vecMax.x - m_vecMin.x;
  }


  template <typename Type>
  const Type AxisAlignedBoundingBox<Type>::getHeight() const
  {
    return m_vecMax.y - m_vecMin.y;
  }


  template <typename Type>
  const Type AxisAlignedBoundingBox<Type>::getDepth() const
  {
    return m_vecMax.z - m_vecMin.z;
  }


  template <typename Type>
  const AxisAlignedBoundingBox<Type> AxisAlignedBoundingBox<Type>::buildAroundPoints(const std::vector<Vector3<Type>>& points)
  {
    if (points.empty())
      return AxisAlignedBoundingBox<Type>();

    auto minmax = getMinMax(points.begin(), points.end());
    return AxisAlignedBoundingBox(minmax.first, minmax.second);
  }


} // namespace ARSTD
