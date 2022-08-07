// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/qvm/quat_operations.hpp>


namespace ARSTD
{

  template <class Type>
  Quaternion<Type> Quaternion<Type>::slerp(const Quaternion<Type>& quat1, const Quaternion<Type>& quat2, const double& t)
  {
    if (t <= 0)
      return quat1;
    if (t >= 1)
      return quat2;

    Quaternion<Type> quat2Tmp(quat2);
    if (quat1.w*quat2.w + quat1.x*quat2.x + quat1.y*quat2.y + quat1.z*quat2.z < 0)
      quat2Tmp = -quat2Tmp;

    return boost::qvm::slerp(quat1, quat2Tmp, t);
  }


  template <class Type>
  Quaternion<Type>::Quaternion(const Type& w, const Type& x, const Type& y, const Type& z) 
  : w(w),
    x(x),
    y(y),
    z(z)
  {
  }

  template <class Type>
  Quaternion<Type>::Quaternion(const Vector3<Type>& vecAxis, const Type& angle)
  {
    set(vecAxis, angle);
  }


  template <class Type>
  Quaternion<Type>::Quaternion(const Matrix33<Type>& mat)
  {
    set(mat);
  }


  template <class Type>
  const bool Quaternion<Type>::equals(const Quaternion<Type>& lhs, const Quaternion<Type>& rhs, double fEpsilon)
  {
    if (&lhs == &rhs)
      return true;
    return boost::qvm::cmp(lhs, rhs, [fEpsilon](const Type& lc, const Type& rc) { return fabs(lc - rc) < fEpsilon; });
  }


  template <class Type>
  Type Quaternion<Type>::length() const
  {
    return boost::qvm::mag(*this);
  }


  template <class Type>
  Type Quaternion<Type>::lengthSqr() const
  {
    return boost::qvm::mag_sqr(*this);
  }
  

  template <class Type>
  Quaternion<Type> Quaternion<Type>::normalize()
  {
    boost::qvm::normalize(*this);
  }


  template <class Type>
  const Matrix33<Type> Quaternion<Type>::getAsMatrix33() const
  {
    return boost::qvm::convert_to<Matrix33<Type>>(*this);
  }


  template <class Type>
  Type Quaternion<Type>::getAngleBetween(const Quaternion<Type>& quat) const
  {
    if (&quat == this)
      return 0;
    if (quat == *this)
      return 0;
    if (-quat == *this)
      return PI;

    Quaternion<Type> tmp(quat);
    if (quat.w*this->w + quat.x*this->x + quat.y*this->y + quat.z*this->z < 0.0)
      tmp = -tmp;

    return (*this*tmp.getInversed()).w;
  }


  template <class Type>
  void Quaternion<Type>::set(const Quaternion<Type>& quat)
  {
    *this = quat;
  }

  
  template <class Type>
  void Quaternion<Type>::set(const Type& w, const Type& x, const Type& y, const Type& z)
  {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
  }


  template <class Type>
  void Quaternion<Type>::set(const Vector3<Type>& vecAxis, const Type& angle)
  {
    boost::qvm::set_rot(*this, vecAxis, angle);
  }


  template<class Type>
  void Quaternion<Type>::set(const Matrix33<Type>& mat)
  {
    *this = mat.getAsQuaternion();
  }


  template <class Type>
  Quaternion<Type> Quaternion<Type>::getInversed() const
  {
    return boost::qvm::inverse(*this);
  }


  template <class Type>
  Quaternion<Type> operator - (const Quaternion<Type>& other)
  {
    return boost::qvm::operator-(other);
  }


  template <class Type>
  Quaternion<Type> operator + (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs)
  {
    return boost::qvm::operator+(lhs, rhs);
  }


  template <class Type>
  Quaternion<Type> operator - (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs)
  {
    return boost::qvm::operator-(lhs, rhs);
  }


  template <class Type>
  Quaternion<Type> operator * (const Quaternion<Type>& lhs, const Type& rhs)
  {
    return boost::qvm::operator*(lhs, rhs);
  }


  template <class Type>
  Quaternion<Type> operator * (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs)
  {
    return boost::qvm::operator*(lhs, rhs);
  }


  template <class Type>
  Vector3<Type> operator * (const Quaternion<Type>& lhs, const Vector3<Type>& rhs)
  {
    return boost::qvm::operator*(lhs, rhs);
  }


  template <class Type>
  const bool operator == (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs)
  {
    return boost::qvm::operator==(lhs, rhs);
  }


} // namespace ARSTD
