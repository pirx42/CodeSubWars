// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once


#include "arstd/Math/Math.h"
#include <arstd/Geometry/Ray3.h>
#include <arstd/Geometry/Segment3.h>


namespace ARSTD
{

  template <typename Type>
  class Line3
  {
    public:
      Line3(const Vector3<Type>& vecPointOnLine, const Vector3<Type>& vecDir);

      explicit Line3(const Ray3<Type>& ray);

      explicit Line3(const Segment3<Type>& segment);

      template<class Type2>
      explicit Line3(const Line3<Type2>& line)
      : m_vecPoint(static_cast<Vector3<Type>>(line.getPos())),
        m_vecDir(static_cast<Vector3<Type>>(line.getDir()))
      {
      }

      const Vector3<Type>& getPos() const;
      const Vector3<Type>& getDir() const;

    protected:
      Vector3<Type> m_vecPoint;
      Vector3<Type> m_vecDir;
  };


  typedef Line3<double> Line3D;


  template <class Type>
  Line3<Type>::Line3(const Vector3<Type>& vecPointOnLine, const Vector3<Type>& vecDir)
  : m_vecPoint(vecPointOnLine),
    m_vecDir(vecDir)
  {
    assert((vecDir.getLength() < 1 + EPSILON) && (vecDir.getLength() > 1 - EPSILON));
  }


  template <class Type>
  Line3<Type>::Line3(const Ray3<Type>& ray)
  : m_vecPoint(ray.getPos()),
    m_vecDir(ray.getDir())
  {
  }


  template <class Type>
  Line3<Type>::Line3(const Segment3<Type>& segment)
  : m_vecPoint(segment.getPoint1()),
    m_vecDir(segment.getDir())
  {
  }


  template <class Type>
  const Vector3<Type>& Line3<Type>::getPos() const
  {
    return m_vecPoint;
  }


  template <class Type>
  const Vector3<Type>& Line3<Type>::getDir() const
  {
    return m_vecDir;
  }


} // namespace ARSTD
