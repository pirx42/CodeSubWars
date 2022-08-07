// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once


#include "arstd/Math/Math.h"
#include "arstd/Geometry/Ray3.h"
#include "arstd/Geometry/Segment3.h"
#include "arstd/Geometry/Line3.h"

namespace ARSTD
{

  template <class Type>
  class Plane
  {
    public:
      Plane();
      Plane(const Vector3<Type>& vecP0, const Vector3<Type>& vecP1, const Vector3<Type>& vecP2);
      Plane(const Vector3<Type>& vecOrigin, const Vector3<Type>& vecNormal);

      template<class Type2>
      explicit Plane(const Plane<Type2>& other)
      : m_vecNormal(static_cast<Vector3<Type>>(other.getNormal())),
        m_D(static_cast<Type>(other.getD())),
        m_vecOrigin(static_cast<Vector3<Type>>(other.getOrigin()))
      {
      }

      const Type& getA() const;
      const Type& getB() const;
      const Type& getC() const;
      const Type& getD() const;

      const Vector3<Type>& getNormal() const;
      void setNormal(const Vector3<Type>& vecNormal);

      const Vector3<Type>& getOrigin() const;
      void setOrigin(const Vector3<Type>& vecOrigin);

      const Vector3<Type> projectPoint(const Vector3<Type>& vecPoint) const;

      const Type distance(const Vector3<Type>& vecPoint) const;

      static bool equals(const Plane<Type>& lhs, const Plane<Type>& rhs, const Type& epsilon);

    protected:
      Vector3<Type> m_vecNormal;
      Type m_D;
      Vector3<Type> m_vecOrigin;
  };


  typedef Plane<double> PlaneD;


  template <class Type>
  Plane<Type>::Plane()
  : m_vecNormal(1, 0, 0),
    m_D(0.0),
    m_vecOrigin(0, 0, 0)
  {
  }


  template <class Type>
  Plane<Type>::Plane(const Vector3<Type>& vecP0, const Vector3<Type>& vecP1, const Vector3<Type>& vecP2)
  : m_vecOrigin(vecP0),
    m_vecNormal(((vecP1 - vecP0) % (vecP2 - vecP0)).getNormalized())
  {
    m_D = -(vecP0 * m_vecNormal);
  }


  template <class Type>
  Plane<Type>::Plane(const Vector3<Type>& vecOrigin, const Vector3<Type>& vecNormal)
  : m_vecOrigin(vecOrigin),
    m_vecNormal(vecNormal.getNormalized())
  {
    m_D = -(vecOrigin * m_vecNormal);
  }


  template <class Type>
  const Type& Plane<Type>::getA() const
  {
    return m_vecNormal.x;
  }


  template <class Type>
  const Type& Plane<Type>::getB() const
  {
    return m_vecNormal.y;
  }


  template <class Type>
  const Type& Plane<Type>::getC() const
  {
    return m_vecNormal.z;
  }


  template <class Type>
  const Type& Plane<Type>::getD() const
  {
    return m_D;
  }


  template <class Type>
  const Vector3<Type>& Plane<Type>::getNormal() const
  {
    return m_vecNormal;
  }


  template <class Type>
  void Plane<Type>::setNormal(const Vector3<Type>& vecNormal)
  {
    assert(vecNormal.length() == 1);

    m_vecNormal = vecNormal;
    m_D = -(m_vecOrigin * vecNormal);
  }


  template <class Type>
  const Vector3<Type>& Plane<Type>::getOrigin() const
  {
    return m_vecOrigin;
  }


  template <class Type>
  void Plane<Type>::setOrigin(const Vector3<Type>& vecOrigin)
  {
    m_vecOrigin = vecOrigin;
    m_D = -(m_vecOrigin * m_vecNormal);
  }


  template <class Type>
  const Vector3<Type> Plane<Type>::projectPoint(const Vector3<Type>& vecPoint) const
  {
    Type fTz = (getA() * vecPoint.x +
                getB() * vecPoint.y +
                getC() * vecPoint.z +
                getD()) * static_cast<Type>(-1.0);
    Type fTn = getA() * m_vecNormal.x +
               getB() * m_vecNormal.y +
               getC() * m_vecNormal.z;

    if (fabs(fTn) < EPSILON) 
      return Vector3<Type>::ZERO;

    return vecPoint + m_vecNormal*fTz/fTn;
  }


  template <class Type>
  const Type Plane<Type>::distance(const Vector3<Type>& vecPoint) const
  {
    return fabs((vecPoint - getOrigin())*getNormal());
  }


  template<class Type>
  bool Plane<Type>::equals(const Plane<Type>& lhs, const Plane<Type>& rhs, const Type& epsilon)
  {
    return lhs.getNormal() == rhs.getNormal() &&
           fabs(lhs.getD() - rhs.getD()) < epsilon;
  }


  template<class Type>
  inline bool operator == (const Plane<Type>& lhs, const Plane<Type>& rhs)
  {
    return Plane<Type>::equals(lhs, rhs, static_cast<Type>(EPSILON));
  }


  template<class Type>
  inline bool operator != (const Plane<Type>& lhs, const Plane<Type>& rhs)
  {
    return !(lhs == rhs);
  }


}  // namespace ARSTD
