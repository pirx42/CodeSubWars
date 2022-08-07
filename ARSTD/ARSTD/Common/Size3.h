// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "arstd/Math/Math.h"


namespace ARSTD
{

  template <class Type>
  class Size3
  {
    public:
      Size3(const Type& width = 0, const Type& height = 0, const Type& depth = 0);
      explicit Size3(const Vector3<Type>& vecSize);

      template <class T2>
      explicit Size3(const Size3<T2>& other)
      : m_Width(static_cast<Type>(other.getWidth())),
        m_Height(static_cast<Type>(other.getHeight())),
        m_Depth(static_cast<Type>(other.getDepth()))
      {
      }

      const Type& getWidth() const;
      void setWidth(const Type& width);

      const Type& getHeight() const;
      void setHeight(const Type& height);

      const Type& getDepth() const;
      void setDepth(const Type& depth);

      const Type getVolume() const;

      Size3<Type>& operator *= (const Size3<Type>& size);
      Size3<Type>& operator /= (const Size3<Type>& size);

    protected:
      Type m_Width;
      Type m_Height;
      Type m_Depth;
  };


  typedef Size3<int> Size3I;
  typedef Size3<unsigned int> Size3UI;
  typedef Size3<float> Size3F;
  typedef Size3<double> Size3D;


  template <class Type>
  Size3<Type>::Size3(const Type& width, const Type& height, const Type& depth) 
  : m_Width(width),
    m_Height(height),
    m_Depth(depth)
  {
  }


  template <class Type>
  Size3<Type>::Size3(const Vector3<Type>& vecSize)
  : m_Width(vecSize.x),
    m_Height(vecSize.y),
    m_Depth(vecSize.z)
  {
  }


  template <class Type>
  const Type& Size3<Type>::getWidth() const 
  { 
    return m_Width;
  }


  template <class Type>
  void Size3<Type>::setWidth(const Type& width) 
  { 
    m_Width = width;
  }


  template <class Type>
  const Type& Size3<Type>::getHeight() const 
  { 
    return m_Height;
  }
  

  template <class Type>
  void Size3<Type>::setHeight(const Type& height) 
  { 
    m_Height = height;
  }


  template <class Type>
  const Type& Size3<Type>::getDepth() const 
  { 
    return m_Depth;
  }


  template <class Type>
  void Size3<Type>::setDepth(const Type& depth) 
  { 
    m_Depth = depth;
  }


  template <class Type>
  const Type Size3<Type>::getVolume() const 
  { 
    return m_Width*m_Height*m_Depth;
  }


  template <class Type>
  Size3<Type>& Size3<Type>::operator /= (const Size3<Type>& size)
  {
    m_Width /= size.m_Width;
    m_Height /= size.m_Height;
    m_Depth /= size.m_Depth;
    return *this;
  }


  template <class Type>
  const Size3<Type> operator * (const Size3<Type>& lhs, const Type& rhs)
  {
    return Size3<Type>(lhs.getWidth() * rhs, lhs.getHeight() * rhs, lhs.getDepth() * rhs);
  }


  template <class Type>
  const Size3<Type> operator / (const Size3<Type>& lhs, const Size3<Type>& rhs)
  {
    return Size3<Type>(lhs) /= rhs;
  }


  template <class Type1, class Type2>
  bool equals(const Size3<Type1>& lhs, const Size3<Type2>& rhs, double fEpsilon)
  {
    return fabs(lhs.getWidth() - rhs.getWidth()) < fEpsilon &&
           fabs(lhs.getHeight() - rhs.getHeight()) < fEpsilon &&
           fabs(lhs.getDepth() - rhs.getDepth()) < fEpsilon;
  }


  template <class Type1, class Type2>
  bool operator == (const Size3<Type1>& lhs, const Size3<Type2>& rhs)
  {
    return equals(lhs, rhs, EPSILON);
  }


} // namespace ARSTD
