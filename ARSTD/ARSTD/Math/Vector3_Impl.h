// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/qvm/vec_operations.hpp>
#include "ARSTD/Math/Quaternion.h"

namespace ARSTD
{

  template <class Type>
  const Vector3<Type> Vector3<Type>::ZERO(0, 0, 0);


  template <class Type>
  Vector3<Type> Vector3<Type>::lerp(const Vector3<Type>& vec1, const Vector3<Type>& vec2, const double& t)
  {
    if (t <= 0)
      return vec1;
    if (t >= 1)
      return vec2;

    return vec1 + (vec2 - vec1)*t;
  }


  template <class Type>
  Vector3<Type>::Vector3(const Type& x, const Type& y, const Type& z)
  : x(x),
    y(y),
    z(z)
  {
  }


  template <class Type>
  const Type Vector3<Type>::getLength() const
  {
    return boost::qvm::mag(*this);
  }


  template <class Type>
  const Type Vector3<Type>::getSquaredLength() const
  {
    return boost::qvm::mag_sqr(*this);
  }


  template <class Type>
  const Vector3<Type>& Vector3<Type>::normalize()
  {
    boost::qvm::normalize(*this);
    return *this;
  }


  template <class Type>
  Vector3<Type> Vector3<Type>::getNormalized() const
  {
    return boost::qvm::normalized(*this);
  }


  template <class Type>
  Vector3<Type> Vector3<Type>::getRotated(const Vector3<Type>& vecAxis, const Type& angle) const
  {
    return Quaternion<Type>(vecAxis, angle)*(*this);
  }


  template <class Type>
  double Vector3<Type>::getDistance(const Vector3<Type>& vec) const
  {
    return distance(*this, vec);
  }


  template <class Type>
  Type Vector3<Type>::getAngleBetween(const Vector3<Type>& vec) const
  {
    if (&vec == this)
      return 0;
    if (vec == *this)
      return 0;
    if (-vec == *this)
      return PI;
    if (vec == ZERO)
      return 0;
    if (*this == ZERO)
      return 0;

    Type s = std::sqrt(getSquaredLength()*vec.getSquaredLength());
    assert(s != 0.0);
    return acos(*this*vec)/s;
  }


  template <class Type>
  Type Vector3<Type>::getAngleBetweenInDegree(const Vector3<Type>& vec) const
  {
    return getInDegree(getAngleBetween(vec));
  }


  template <class Type>
  Vector3<Type> Vector3<Type>::getPerpendicular() const
  {
    if (*this == ZERO)
      return Vector3<Type>(0, 0, 1);
    Vector3<Type> vec;
    vec.y = -this->x;
    vec.z = this->y;
    vec.x = this->z;

    assert(vec.x != this->x || vec.y != this->y || vec.z != this->z);
    assert(vec.getLength() > EPSILON);
    return *this % vec;
  }


  template <class Type>
  Vector3<Type>& Vector3<Type>::operator += (const Vector3<Type>& vec)
  {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    return *this;
  }


  template <class Type>
  Vector3<Type>& Vector3<Type>::operator -= (const Vector3<Type>& vec)
  {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    return *this;
  }


  template <class Type>
  Vector3<Type>& Vector3<Type>::operator *= (const Type& scalar)
  {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
  }


  template <class Type>
  Vector3<Type>& Vector3<Type>::operator /= (const Type& scalar)
  {
    return this->operator *= (static_cast<Type>(1.0)/scalar);
  }


  template <class Type>
  Vector3<Type> operator - (const Vector3<Type>& vec)

  {
    return Vector3<Type>(-vec.x, -vec.y, -vec.z);
  }


  template <class Type>
  const Type operator * (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return boost::qvm::dot(lhs, rhs);
  }


  template <class Type>
  Vector3<Type> operator % (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return boost::qvm::cross(lhs, rhs);
  }


  template <class Type>
  Vector3<Type> operator + (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return Vector3<Type>(lhs) += rhs;
  }


  template <class Type>
  Vector3<Type> operator - (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return Vector3<Type>(lhs) -= rhs;
  }


  template <class Type>
  Vector3<Type> operator * (const Vector3<Type>& lhs, const Type& rhs)
  {
    return Vector3<Type>(lhs) *= rhs;
  }


  template <class Type>
  Vector3<Type> operator * (const Type& lhs, const Vector3<Type>& rhs)
  {
    return Vector3<Type>(rhs) *= lhs;
  }


  template <class Type>
  Vector3<Type> operator / (const Vector3<Type>& lhs, const Type& rhs)
  {
    return Vector3<Type>(lhs) *= static_cast<Type>(1.0)/rhs;
  }


  template <class Type>
  bool equals(const Vector3<Type>& lhs, const Vector3<Type>& rhs, double fEpsilon)
  {
    if (&lhs == &rhs)
      return true;

    return fabs(lhs.x - rhs.x) < fEpsilon &&
           fabs(lhs.y - rhs.y) < fEpsilon &&
           fabs(lhs.z - rhs.z) < fEpsilon;
  }


  template <class Type>
  bool operator == (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return equals(lhs, rhs, EPSILON);
  }


  template <class Type>
  bool operator != (const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    return !(lhs == rhs);
  }


  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Vector3<Type>& vec)
  {
    stream << vec.x << " " << vec.y << " " << vec.z;
    return stream;
  }

 
  template <class Type>
  std::istream& operator >> (std::istream& stream, Vector3<Type>& vec)
  {
    stream >> vec.x >> vec.y >> vec.z;
    return stream;
  }


  template <class Type>
  Type distance(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    Type dx = lhs.x - rhs.x;
    Type dy = lhs.y - rhs.y;
    Type dz = lhs.z - rhs.z;
    return static_cast<Type>(std::sqrt(dx*dx + dy*dy + dz*dz));
  }


} // namespace ARSTD
