// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#include <boost/qvm/quat.hpp>


namespace ARSTD
{

  template <class Type> struct Matrix33;


  template <class Type>
  struct Quaternion
  {

    template <class R>
    operator R() const
    {
      R r;
      assign(r, *this);
      return r;
    }

    static Quaternion<Type> slerp(const Quaternion<Type>& quat1, const Quaternion<Type>& quat2, const double& t);

    explicit Quaternion(const Type& w = 1, const Type& x = 0, const Type& y = 0, const Type& z = 0);
    explicit Quaternion(const Vector3<Type>& vecAxis, const Type& angle);
    explicit Quaternion(const Matrix33<Type>& mat);

    template<class Type2>
    explicit Quaternion(const Quaternion<Type2>& other)
    {
      w = static_cast<Type>(other.w);
      x = static_cast<Type>(other.x);
      y = static_cast<Type>(other.y);
      z = static_cast<Type>(other.z);
    }


    Type length() const;
    Type lengthSqr() const;

    Quaternion<Type> normalize();

    const Matrix33<Type> getAsMatrix33() const;

    Type getAngleBetween(const Quaternion<Type>& quat) const;
 
    void set(const Quaternion<Type>& quat);
    void set(const Type& w, const Type& x, const Type& y, const Type& z);
    void set(const Vector3<Type>& vecAxis, const Type& angle);
    void set(const Matrix33<Type>& mat);

    Quaternion<Type> getInversed() const;

    static const bool equals(const Quaternion<Type>& lhs, const Quaternion<Type>& rhs, double fEpsilon);

    template <class Type2>
    Quaternion<Type>& operator += (const Quaternion<Type2>& other)
    {
      return boost::qvm::operator+=(*this, other);
    }

    template <class Type2>
    Quaternion<Type>& operator -= (const Quaternion<Type2>& other)
    {
      return boost::qvm::operator-=(*this, other);
    }

    template <class Type2>
    Quaternion<Type>& operator *= (const Type2& scalar)
    {
      return boost::qvm::operator*=(*this, scalar);
    }


    union
    {
      Type pData[4];
      
      struct 
      {
        Type w;
        Type x;
        Type y;
        Type z;
      };
    };

  };


  template <class Quaternion>
  struct boost::qvm::quat_traits;

  template <class Type>
  struct boost::qvm::quat_traits<ARSTD::Quaternion<Type>>
  {
    typedef Type scalar_type;

    template <int I>
    static scalar_type read_element(ARSTD::Quaternion<Type> const& x)
    {
      return x.pData[I];
    }

    template <int I>
    static scalar_type& write_element(ARSTD::Quaternion<Type>& x)
    {
      return x.pData[I];
    }
  };


  typedef Quaternion<double> QuaternionD;


  template <class Type>
  Quaternion<Type> operator - (const Quaternion<Type>& other);

  template <class Type>
  Quaternion<Type> operator + (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs);

  template <class Type>
  Quaternion<Type> operator - (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs);

  template <class Type>
  Quaternion<Type> operator * (const Quaternion<Type>& lhs, const Type& rhs);

  template <class Type>
  Quaternion<Type> operator * (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs);

  template <class Type>
  Vector3<Type> operator * (const Quaternion<Type>& lhs, const Vector3<Type>& rhs);

  template <class Type>
  const bool operator == (const Quaternion<Type>& lhs, const Quaternion<Type>& rhs);


} // namespace ARSTD


#include "Quaternion_Impl.h"
