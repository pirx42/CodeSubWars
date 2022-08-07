// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include <cassert>
#include "arstd/Math/Math.h"
#include "arstd/Geometry/Segment3.h"


namespace ARSTD
{

  template <class Type>
  class Rectangle3
  {
    public:
      typedef std::pair<bool, Vector3<Type>> ProjectionPoint;

      Rectangle3(const Vector3<Type>& vecPoint0, const Vector3<Type>& vecPoint1,
                 const Vector3<Type>& vecPoint2, const Vector3<Type>& vecPoint3);

      template<class Type2>
      explicit Rectangle3(const Rectangle3<Type2>& other)
      : m_vecPoint0(static_cast<Vector3<Type>>(other.getPoint0())),
        m_vecPoint1(static_cast<Vector3<Type>>(other.getPoint1())),
        m_vecPoint2(static_cast<Vector3<Type>>(other.getPoint2())),
        m_vecPoint3(static_cast<Vector3<Type>>(other.getPoint3()))
      {
      }

      const Vector3<Type>& getPoint0() const;
      void setPoint0(const Vector3<Type>& vec);
      const Vector3<Type>& getPoint1() const;
      void setPoint1(const Vector3<Type>& vec);
      const Vector3<Type>& getPoint2() const;
      void setPoint2(const Vector3<Type>& vec);
      const Vector3<Type>& getPoint3() const;
      void setPoint3(const Vector3<Type>& vec);

      const Segment3<Type> getSegment(uint nIdx) const;

      const Vector3<Type> getNormal() const;

      const double getWidth() const;
      const double getHeight() const;

      ProjectionPoint project(const Vector3<Type>& vecPoint) const;

      Type getDistanceTo(const Vector3<Type>& vecPoint) const;


    protected:
      Vector3<Type> m_vecPoint0;
      Vector3<Type> m_vecPoint1;
      Vector3<Type> m_vecPoint2;
      Vector3<Type> m_vecPoint3;
  };


  typedef Rectangle3<double> Rectangle3D;


  template<class Type>
  Rectangle3<Type>::Rectangle3(const Vector3<Type>& vecPoint0, const Vector3<Type>& vecPoint1, 
                               const Vector3<Type>& vecPoint2, const Vector3<Type>& vecPoint3)
  : m_vecPoint0(vecPoint0),
    m_vecPoint1(vecPoint1),
    m_vecPoint2(vecPoint2),
    m_vecPoint3(vecPoint3)
  {
  }


  template<class Type>
  bool operator == (const Rectangle3<Type>& rect1, const Rectangle3<Type>& rect2)
  {
    return (rect1.getPoint0() == rect2.getPoint0() &&
            rect1.getPoint1() == rect2.getPoint1() &&
            rect1.getPoint2() == rect2.getPoint2() &&
            rect1.getPoint3() == rect2.getPoint3());
  }


  template <class Type>
  const Vector3<Type>& Rectangle3<Type>::getPoint0() const
  {
    return m_vecPoint0;
  }

  
  template <class Type>
  void Rectangle3<Type>::setPoint0(const Vector3<Type>& vec)
  {
    m_vecPoint0 = vec;
  }


  template <class Type>
  const Vector3<Type>& Rectangle3<Type>::getPoint1() const
  {
    return m_vecPoint1;
  }


  template <class Type>
  void Rectangle3<Type>::setPoint1(const Vector3<Type>& vec)
  {
    m_vecPoint1 = vec;
  }


  template <class Type>
  const Vector3<Type>& Rectangle3<Type>::getPoint2() const
  {
    return m_vecPoint2;
  }

  
  template <class Type>
  void Rectangle3<Type>::setPoint2(const Vector3<Type>& vec)
  {
    m_vecPoint2 = vec;
  }


  template <class Type>
  const Vector3<Type>& Rectangle3<Type>::getPoint3() const
  {
    return m_vecPoint3;
  }

  
  template <class Type>
  void Rectangle3<Type>::setPoint3(const Vector3<Type>& vec)
  {
    m_vecPoint3 = vec;
  }


  template <class Type>
  const Segment3<Type> Rectangle3<Type>::getSegment(uint nIdx) const
  {
    assert(nIdx <= 3);
    switch (nIdx)
    {
      case 0: return Segment3<Type>(getPoint0(), getPoint1()); break;
      case 1: return Segment3<Type>(getPoint1(), getPoint2()); break;
      case 2: return Segment3<Type>(getPoint2(), getPoint3()); break;
      case 3: return Segment3<Type>(getPoint3(), getPoint0()); break;
      default: return Segment3<Type>(getPoint0(), getPoint1()); break;
    }
  }


  template <class Type>
  const Vector3<Type> Rectangle3<Type>::getNormal() const
  {
    return ((getPoint1() - getPoint2()) % (getPoint3() - getPoint2())).getNormalized();
  }


  template <class Type>
  const double Rectangle3<Type>::getWidth() const
  {
    return distance(getPoint0(), getPoint1());
  }


  template <class Type>
  const double Rectangle3<Type>::getHeight() const
  {
    return distance(getPoint0(), getPoint3());
  }


  template <class Type>
  typename Rectangle3<Type>::ProjectionPoint Rectangle3<Type>::project(const Vector3<Type>& vecPoint) const
  {
    Plane<Type> plane(getPoint0(), getNormal());
    Vector3<Type> vecPlaneProjectedPoint = plane.projectPoint(vecPoint);

    Vector3<Type> vecXDir = (getPoint1() - getPoint0()).getNormalized();
    Vector3<Type> vecYDir = (getPoint3() - getPoint0()).getNormalized();

    double fXProjection = (vecPlaneProjectedPoint - getPoint0()) * vecXDir;
    double fYProjection = (vecPlaneProjectedPoint - getPoint0()) * vecYDir;

    if (fXProjection >= 0 && fXProjection <= getWidth() &&
        fYProjection >= 0 && fYProjection <= getHeight())
    {
      return ProjectionPoint(true, vecPlaneProjectedPoint);
    }
    else
    {
      return ProjectionPoint(false, Vector3<Type>());
    }
  }


  template <class Type>
  Type Rectangle3<Type>::getDistanceTo(const Vector3<Type>& vecPoint) const
  {
    ProjectionPoint projectedPoint = project(vecPoint);
    if (projectedPoint.first)
      return (projectedPoint.second - vecPoint).getLength();

    Type minDistance = std::numeric_limits<Type>::max();
    for (int nCnt = 0; nCnt < 4; ++nCnt)
    {
      Type dist = getSegment(nCnt).getDistanceToPoint(vecPoint);
      if (dist < minDistance)
        minDistance = dist;
    }
    return minDistance;
  }


} // namespace ARSTD
