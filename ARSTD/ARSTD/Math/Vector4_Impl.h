// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/qvm/vec_operations.hpp>


namespace ARSTD
{

  template <class Type>
  const Vector4<Type> Vector4<Type>::ZERO(0, 0, 0, 0);


  template <class Type>
  Vector4<Type>::Vector4(const Type& x, const Type& y, const Type& z, const Type& w)
  : x(x),
    y(y),
    z(z),
    w(w)
  {
  }


  template <class Type>
  const Type Vector4<Type>::getLength() const
  {
    return boost::qvm::mag(*this);
  }


  template <class Type>
  const Type Vector4<Type>::getSquaredLength() const
  {
    return boost::qvm::mag_sqr(*this);
  }


  template <class Type>
  const Vector4<Type>& Vector4<Type>::normalize()
  {
    boost::qvm::normalize(*this);
    return *this;
  }


  template <class Type>
  Vector4<Type> Vector4<Type>::getNormalized() const
  {
    return boost::qvm::normalized(*this);
  }


  template <class Type>
  Vector4<Type>& Vector4<Type>::operator += (const Vector4<Type>& vec)
  {
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    this->w += vec.w;
    return *this;
  }


  template <class Type>
  Vector4<Type>& Vector4<Type>::operator -= (const Vector4<Type>& vec)
  {
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    this->w -= vec.w;
    return *this;
  }


  template <class Type>
  Vector4<Type>& Vector4<Type>::operator *= (const Type& scalar)
  {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    this->w *= scalar;
    return *this;
  }


  template <class Type>
  Vector4<Type>& Vector4<Type>::operator /= (const Type& scalar)
  {
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    this->w /= scalar;
    return *this;
  }


  template <class Type>
  bool equals(const Vector4<Type>& lhs, const Vector4<Type>& rhs, double fEpsilon)
  {
    if (&lhs == &rhs)
      return true;

    return fabs(lhs.x - rhs.x) < fEpsilon &&
           fabs(lhs.y - rhs.y) < fEpsilon &&
           fabs(lhs.z - rhs.z) < fEpsilon &&
           fabs(lhs.w - rhs.w) < fEpsilon;
  }


  template <class Type>
  bool operator == (const Vector4<Type>& lhs, const Vector4<Type>& rhs)
  {
    return equals(lhs, rhs, EPSILON);
  }

  
  template <class Type>
  bool operator != (const Vector4<Type>& lhs, const Vector4<Type>& rhs)
  {
    return !(lhs == rhs);
  }


  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Vector4<Type>& vec)
  {
    stream << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
    return stream;
  }

 
  template <class Type>
  std::istream& operator >> (std::istream& stream, Vector4<Type>& vec)
  {
    stream >> vec.x >> vec.y >> vec.z >> vec.w;
    return stream;
  }


} // namespace ARSTD
