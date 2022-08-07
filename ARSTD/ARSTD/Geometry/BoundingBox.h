// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include <cassert>
#include "arstd/Common/Size3.h"
#include "arstd/Geometry/Rectangle3.h"
#include "arstd/Geometry/Segment3.h"
#include "arstd/Geometry/AxisAlignedBoundingBox.h"


namespace ARSTD 
{

  template <class Type>
  class BoundingBox
  {
    public:
      BoundingBox();
      BoundingBox(const Matrix44<Type>& matCoordSys, const Size3<Type>& size);
      explicit BoundingBox(const AxisAlignedBoundingBox<Type>& aabb);

      template<class Type2>
      explicit BoundingBox(const BoundingBox<Type2>& other)
      : m_matCoordSys(static_cast< Matrix44<Type> >(other.getCoordSystem())),
        m_Size(static_cast< Size3<Type> >(other.getSize())),
        m_vecScalingCenter(static_cast<Vector3<Type>>(other.getScalingCenter()))
      {
        validate();
      }

      void set(const Matrix44<Type>& matCoordSys, const Size3<Type>& size);

      const Matrix44<Type>& getCoordSystem() const;

      const Type getWidth() const; 
      const Type getHeight() const; 
      const Type getDepth() const; 

      const Size3<Type>& getSize() const;
      const Type getDiagonalLength() const;

      const Rectangle3<Type> getRectangle(const unsigned int idx) const;
      const Segment3<Type> getSegment(const unsigned int idx) const;
      const Vector3<Type>& getPoint(const unsigned int idx) const;

      bool isValid() const; 
      const Vector3<Type> getCenter() const;

      void transform(const Matrix44<Type>& mat);


    protected:
      void validate();

      Matrix44<Type> m_matCoordSys;
      Size3<Type> m_Size;
      Vector3<Type> m_pPoints[8];
  };


  typedef BoundingBox<double> BoundingBoxD;


  template <class Type>
  BoundingBox<Type>::BoundingBox()
  {
    set(Matrix44<Type>(true), Size3<Type>(0, 0, 0));
  }

  
  template <class Type>
  BoundingBox<Type>::BoundingBox(const Matrix44<Type>& matCoordSys, const Size3<Type>& size)
  {
    set(matCoordSys, size);
  }


  template <class Type>
  BoundingBox<Type>::BoundingBox(const AxisAlignedBoundingBox<Type>& aabb)
  {
    set(Matrix44<Type>(aabb.getMin()),  Size3<Type>(aabb.getMax() - aabb.getMin()));
  }


  template <class Type>
  void BoundingBox<Type>::set(const Matrix44<Type>& matCoordSys, const Size3<Type>& size)
  {
    assert(size.getWidth() >= 0.0f && size.getHeight() >= 0.0f && size.getDepth() >= 0.0f);

    m_matCoordSys = matCoordSys;
    m_Size = size;
    validate();
  }


  template <class Type>
  inline const Vector3<Type>& BoundingBox<Type>::getPoint(const unsigned int idx) const
  {
    assert(idx < 8);

    return m_pPoints[idx];
  }


  template <class Type>
  const Matrix44<Type>& BoundingBox<Type>::getCoordSystem() const
  {
    return m_matCoordSys;
  }


  template <class Type>
  const Size3<Type>& BoundingBox<Type>::getSize() const
  {
    return m_Size;
  }


  template <class Type>
  inline const Rectangle3<Type> BoundingBox<Type>::getRectangle(const unsigned int idx) const
  {
    assert(idx < 6);

    switch (idx) {
      case 0: return Rectangle3<Type>(getPoint(0), getPoint(1), getPoint(2), getPoint(3)); break;
      case 1: return Rectangle3<Type>(getPoint(0), getPoint(4), getPoint(5), getPoint(1)); break;
      case 2: return Rectangle3<Type>(getPoint(0), getPoint(3), getPoint(7), getPoint(4)); break;
      case 3: return Rectangle3<Type>(getPoint(4), getPoint(7), getPoint(6), getPoint(5)); break;
      case 4: return Rectangle3<Type>(getPoint(3), getPoint(2), getPoint(6), getPoint(7)); break;
      case 5: return Rectangle3<Type>(getPoint(1), getPoint(5), getPoint(6), getPoint(2)); break;
      default: return Rectangle3<Type>(getPoint(0), getPoint(1), getPoint(2), getPoint(3)); break;
    }
  }


  template <class Type>
  inline const Segment3<Type> BoundingBox<Type>::getSegment(const unsigned int idx) const
  {
    assert(idx < 12);

    switch (idx) {
      case 0: return Segment3<Type>(getPoint(0), getPoint(1)); break;
      case 1: return Segment3<Type>(getPoint(1), getPoint(2)); break;
      case 2: return Segment3<Type>(getPoint(2), getPoint(3)); break;
      case 3: return Segment3<Type>(getPoint(3), getPoint(0)); break;
      case 4: return Segment3<Type>(getPoint(4), getPoint(5)); break;
      case 5: return Segment3<Type>(getPoint(5), getPoint(6)); break;
      case 6: return Segment3<Type>(getPoint(6), getPoint(7)); break;
      case 7: return Segment3<Type>(getPoint(7), getPoint(4)); break;
      case 8: return Segment3<Type>(getPoint(0), getPoint(4)); break;
      case 9: return Segment3<Type>(getPoint(1), getPoint(5)); break;
      case 10: return Segment3<Type>(getPoint(2), getPoint(6)); break;
      case 11: return Segment3<Type>(getPoint(3), getPoint(7)); break;
      default: return Segment3<Type>(getPoint(0), getPoint(1)); break;
    }
  }


  template <class Type>
  void BoundingBox<Type>::transform(const Matrix44<Type>& mat)
  {
    m_matCoordSys = mat * m_matCoordSys;
    validate();
  }


  template <class Type>
  inline bool BoundingBox<Type>::isValid() const
  {
    return static_cast<bool>(getDiagonalLength() > 0.0);
  }


  template <class Type>
  inline const Type BoundingBox<Type>::getWidth() const
  {
    return m_Size.getWidth();
  }


  template <class Type>
  inline const Type BoundingBox<Type>::getHeight() const
  {
    return m_Size.getHeight();
  }


  template <class Type>
  inline const Type BoundingBox<Type>::getDepth() const
  {
    return m_Size.getDepth();
  }


  template <class Type>
  inline const Type BoundingBox<Type>::getDiagonalLength() const
  {
    return static_cast<Type>((m_matCoordSys.getXAxis() * m_Size.getWidth() +
                              m_matCoordSys.getYAxis() * m_Size.getHeight() +
                              m_matCoordSys.getZAxis() * m_Size.getDepth()).getLength());
  }


  template <class Type>
  inline const Vector3<Type> BoundingBox<Type>::getCenter() const
  {
    return m_matCoordSys.getTranslation() + m_matCoordSys.getXAxis() * m_Size.getWidth() * 0.5
                                          + m_matCoordSys.getYAxis() * m_Size.getHeight() * 0.5
                                          + m_matCoordSys.getZAxis() * m_Size.getDepth() * 0.5;
  }


  template <class Type>
  inline void BoundingBox<Type>::validate()
  {
    m_pPoints[0] = m_matCoordSys.getTranslation();
    m_pPoints[1] = m_pPoints[0] + m_matCoordSys.getXAxis() * m_Size.getWidth();
    m_pPoints[2] = m_pPoints[1] + m_matCoordSys.getZAxis() * m_Size.getDepth();
    m_pPoints[3] = m_pPoints[0] + m_matCoordSys.getZAxis() * m_Size.getDepth();

    Vector3<Type> vec(m_matCoordSys.getYAxis() * m_Size.getHeight());
    m_pPoints[4] = m_pPoints[0] + vec;
    m_pPoints[5] = m_pPoints[1] + vec;
    m_pPoints[6] = m_pPoints[2] + vec;
    m_pPoints[7] = m_pPoints[3] + vec;
  }


} //namespace ARSTD
