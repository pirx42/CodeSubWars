// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#include <iosfwd>
#include <boost/qvm/vec.hpp>


namespace ARSTD
{

  /**
   * This represents a 3-dimensional vector
   */
  template <class Type>
  struct Vector3
  {
    public:

      template <class R>
      operator R() const
      {
        R r;
        assign(r, *this);
        return r;
      }

      static const Vector3<Type> ZERO;

      /**
       * Linear interpolation of two vectors
       * @return The interpolated vector
       */
      static Vector3<Type> lerp(const Vector3<Type>& vec1, const Vector3<Type>& vec2, const double& t);

      /**
       * Constructs a vector from given components
       * @param x x-component
       * @param y y-component
       * @param z z-component
       */
      Vector3(const Type& x = 0, const Type& y = 0, const Type& z = 0);

      template<class T2>
      explicit Vector3(const Vector3<T2>& other)
      : x(static_cast<Type>(other.x)),
        y(static_cast<Type>(other.y)),
        z(static_cast<Type>(other.z))
      {
      }

      /** 
       * Returns the length
       * @return  length of the vector
       */
      const Type getLength() const;

      /** 
       * Returns the squared length
       * @return Squared length of the vector.
       */
      const Type getSquaredLength() const;

      /**
       * Normalizes the vector to a length of 1
       * @pre  getLength() != 0.0
       * @post getLength() is 1
       * @return The normalized vector
       */
      const Vector3<Type>& normalize();

      /**
       * Returns the normalized version of this vector
       * @pre  getLength() != 0.0
       * @return The normalized vector
       */
      Vector3<Type> getNormalized() const;

      /**
       * Rotates the vector around given axis and angle
       * @param vecAxis the axis the vector is rotated around
       * @param angle the angle the vector is rotated around in rad
       */
      Vector3<Type> getRotated(const Vector3<Type>& vecAxis, const Type& angle) const;

      /**
       * Returns the distance to given vector
       */
      double getDistance(const Vector3<Type>& vec) const;

      /**
       * Returns the angle the given vector
       */
      Type getAngleBetween(const Vector3<Type>& vec) const;
      Type getAngleBetweenInDegree(const Vector3<Type>& vec) const;

      /**
       * Returns a some perpendicular vector
       * @return The perpendicular vector
       * @pre The length must be not zero
       */
      Vector3<Type> getPerpendicular() const;

      Vector3<Type>& operator += (const Vector3<Type>& vec);
      Vector3<Type>& operator -= (const Vector3<Type>& vec);
      Vector3<Type>& operator *= (const Type& scalar);
      Vector3<Type>& operator /= (const Type& scalar);

      union
      {
        Type pData[3];
        struct
        {
          Type x; ///<  The first component
          Type y; ///<  The second component
          Type z; ///<  The third component
        };
        struct
        {
          Type r; ///<  Alias of first component
          Type g; ///<  Alias of second component
          Type b; ///<  Alias of third component
        };
      };
  };


  template <class Vector>
  struct boost::qvm::vec_traits;

  template <class Type>
  struct boost::qvm::vec_traits<ARSTD::Vector3<Type>>
  {
    typedef Type scalar_type;
    static int const dim = 3;
  
    template <int I>
    static scalar_type read_element(ARSTD::Vector3<Type> const& x)
    {
      return x.pData[I];
    }

    template <int I>
    static scalar_type& write_element(ARSTD::Vector3<Type>& x)
    {
      return x.pData[I];
    }

    static scalar_type read_element_idx(int i, ARSTD::Vector3<Type> const& x)
    {
      return x.pData[i];
    }

    static scalar_type& write_element_idx(int i, ARSTD::Vector3<Type>& x)
    {
      return x.pData[i];
    }

  };

  typedef Vector3<float> Vector3F;
  typedef Vector3<double> Vector3D;


  /** 
   * Negates all components
   * @return   The negated vector
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator - (const Vector3<Type>& vec);

  /**
   * Calculates the dot product
   * @return   The dot product
   * @relates Vector3
   */
  template <class Type>
  const Type operator * (const Vector3<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Calculates the cross product
   * @return   The cross product
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator % (const Vector3<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Calculates the addition
   * @return   The added vector
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator + (const Vector3<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Multiplies a scalar by each component of a vector
   * @return   The result of the operation
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator * (const Vector3<Type>& lhs, const Type& rhs);

  /**
   * Multiplies a scalar by each component of a vector
   * @return   The result of the operation
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator * (const Type& lhs, const Vector3<Type>& rhs);

  /**
   * Divides each component of a vector by a scalar
   * @return   The result of the operation
   * @relates Vector3
   */
  template <class Type>
  Vector3<Type> operator / (const Vector3<Type>& lhs, const Type& rhs);

  /**
   * Compares two vectors with the epsilon given by the parameter fEpsilon
   * @return  true, if the vectors are considered equal, false otherwise
   * @relates Vector3
   */
  template <class Type>
  bool equals(const Vector3<Type>& lhs, const Vector3<Type>& rhs, double fEpsilon = EPSILON);

  /**
   * Compares two matrices using default EPSILON
   * @return  true, if the vectors are considered equal, false otherwise
   * @relates Vector3
   */
  template <class Type>
  bool operator == (const Vector3<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Compares two matrices using default EPSILON
   * @return  true, if the vectors are considered as not equal, false otherwise
   * @relates Vector3
   */
  template <class Type>
  bool operator != (const Vector3<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Writes the vector to a std output stream
   * @relates Vector3
   */
  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Vector3<Type>& vec);

  /**
   * Reads the vector from a std input stream
   * @relates Vector3
   */
  template <class Type>
  std::istream& operator >> (std::istream& stream, Vector3<Type>& vec);

  /**
   * Calculates the distance between to vectors
   * @relates Vector3
   */
  template <class Type>
  Type distance(const Vector3<Type>& vec1, const Vector3<Type>& vec2);

  /**
   * Calculates the component wise minimal vector from the given ones
   * @relates Vector3
   */
  template <typename Type>
  Vector3<Type> minimum(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    Vector3<Type> tmp(lhs);
    if (tmp.x > rhs.x) tmp.x = rhs.x;
    if (tmp.y > rhs.y) tmp.y = rhs.y;
    if (tmp.z > rhs.z) tmp.z = rhs.z;
    return tmp;
  }

  /**
   * Calculates the component wise maximal vector from the given ones
   * @relates Vector3
   */
  template <typename Type>
  Vector3<Type> maximum(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
  {
    Vector3<Type> tmp(lhs);
    if (tmp.x < rhs.x) tmp.x = rhs.x;
    if (tmp.y < rhs.y) tmp.y = rhs.y;
    if (tmp.z < rhs.z) tmp.z = rhs.z;
    return tmp;
  }


} // namespace ARSTD


template<> class std::numeric_limits<ARSTD::Vector3F> : public std::_Num_float_base
{ 
  public:
    typedef ARSTD::Vector3F _Ty;

    static _Ty (__cdecl min)()
    {
      return _Ty(-std::numeric_limits<float>::max(),
                 -std::numeric_limits<float>::max(),
                 -std::numeric_limits<float>::max());
    }

    static _Ty (__cdecl max)()
    {
      return _Ty(std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max());
    }
};

template<> class std::numeric_limits<ARSTD::Vector3D> : public std::_Num_float_base
{
public:
  typedef ARSTD::Vector3D _Ty;

  static _Ty(__cdecl min)()
  {
    return ARSTD::Vector3D(-std::numeric_limits<double>::max(),
      -std::numeric_limits<double>::max(),
      -std::numeric_limits<double>::max());
  }

  static _Ty(__cdecl max)()
  {
    return ARSTD::Vector3D(std::numeric_limits<double>::max(),
      std::numeric_limits<double>::max(),
      std::numeric_limits<double>::max());
  }
};

#include "Vector3_Impl.h"
