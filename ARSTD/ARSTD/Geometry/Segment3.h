// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "arstd/Math/Math.h"


namespace ARSTD
{


  template <class Type>
  class Segment3
  {
    public:
      typedef std::pair<bool, Vector3<Type>> ProjectionPoint;

      Segment3();
      Segment3(const Vector3<Type>& vecPoint, const Vector3<Type>& vecPoint2);

      template<class Type2>
      explicit Segment3(const Segment3<Type2>& other)
      : m_vecPoint1(static_cast<Vector3<Type>>(other.getPoint1())),
        m_vecPoint2(static_cast<Vector3<Type>>(other.getPoint2()))
      {
      }

      const Vector3<Type>& getPoint1() const;
      void setPoint1(const Vector3<Type>& vec);
      const Vector3<Type>& getPoint2() const;
      void setPoint2(const Vector3<Type>& vec);

      Vector3<Type> getDir() const;

      Type getLength() const;
      Type getLengthSqr() const;

      const ProjectionPoint projectPoint(const Vector3<Type>& vecPoint) const;

      Type getDistanceToPoint(const Vector3<Type>& vecPoint) const;
      Type getSquaredDistanceToPoint(const Vector3<Type>& vecPoint) const;

    protected:
      Vector3<Type> m_vecPoint1;
      Vector3<Type> m_vecPoint2;
  };


  typedef Segment3<double> Segment3D;


  template <class Type>
  Segment3<Type>::Segment3()
  : m_vecPoint1(0, 0, 0),
    m_vecPoint2(0, 0, 0)
  {
  }


  template <class Type>
  Segment3<Type>::Segment3(const Vector3<Type>& vecPoint1, const Vector3<Type>& vecPoint2)
  : m_vecPoint1(vecPoint1),
    m_vecPoint2(vecPoint2)
  {
  }


  template <class Type>
  const Vector3<Type>& Segment3<Type>::getPoint1() const
  {
    return m_vecPoint1;
  }


  template <class Type>
  void Segment3<Type>::setPoint1(const Vector3<Type>& vec)
  {
    m_vecPoint1 = vec;
  }


  template <class Type>
  const Vector3<Type>& Segment3<Type>::getPoint2() const
  {
    return m_vecPoint2;
  }


  template <class Type>
  void Segment3<Type>::setPoint2(const Vector3<Type>& vec)
  {
    m_vecPoint2 = vec;
  }


  template <class Type>
  Vector3<Type> Segment3<Type>::getDir() const
  {
    return m_vecPoint2 - m_vecPoint1;
  }


  template <class Type>
  Type Segment3<Type>::getLength() const
  {
    return std::sqrt(getLengthSqr());
  }


  template <class Type>
  Type Segment3<Type>::getLengthSqr() const
  {
    return (m_vecPoint1 - m_vecPoint2).getSquaredLength();
  }


  template <class Type>
  const typename Segment3<Type>::ProjectionPoint Segment3<Type>::projectPoint(const Vector3<Type>& vecPoint) const
  {
    Vector3<Type> vecDir(m_vecPoint2 - m_vecPoint1);
    vecDir.normalize();

    double fProjection = (vecPoint - m_vecPoint1)*vecDir;
    if (fProjection >= 0 && fProjection <= getLength())
      return ProjectionPoint(true, m_vecPoint1 + vecDir*fProjection);

    return ProjectionPoint(false, Vector3<Type>());
  }


  template <class Type>
  Type Segment3<Type>::getDistanceToPoint(const Vector3<Type>& vecPoint) const
  {
    return std::sqrt(getSquaredDistanceToPoint(vecPoint));
  }


  template <class Type>
  Type Segment3<Type>::getSquaredDistanceToPoint(const Vector3<Type>& vecPoint) const
  {
    ProjectionPoint result = projectPoint(vecPoint);
    if (!result.first)
    {
      return std::min((m_vecPoint1 - vecPoint).getSquaredLength(),
                      (m_vecPoint2 - vecPoint).getSquaredLength());
    }

    return (result.second - vecPoint).getSquaredLength();
  }


}  // namespace ARSTD
