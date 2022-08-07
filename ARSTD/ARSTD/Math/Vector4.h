// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#include <iosfwd>
#include <boost/qvm/vec.hpp>


namespace ARSTD
{

  /**
   * This represents a 4-dimensional vector
   */
  template <class Type>
  struct Vector4
  {
    public:

      template <class R>
      operator R() const
      {
        R r;
        assign(r, *this);
        return r;
      }

      static const Vector4<Type> ZERO;

      /**
       * Constructs a vector from given components
       * @param x    x-component
       * @param y    y-component
       * @param z    z-component
       * @param w    w-component
       */
      Vector4(const Type& x = 0, const Type& y = 0, const Type& z = 0, const Type& w = 1);

      template <class Type2>
      Vector4(const Vector3<Type2>& vec, const Type& w)
      : x(static_cast<Type>(vec.x)),
        y(static_cast<Type>(vec.y)),
        z(static_cast<Type>(vec.z)),
        w(w)
      {
      }

      template<class Type2>
      explicit Vector4(const Vector4<Type2>& other)
      {
        x = static_cast<Type>(other.x);
        y = static_cast<Type>(other.y);
        z = static_cast<Type>(other.z);
        w = static_cast<Type>(other.w);
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
      const Vector4<Type>& normalize();

      /**
       * Returns the normalized version of this vector
       * @pre  getLength() != 0.0
       * @return The normalized vector
       */
      Vector4<Type> getNormalized() const;

      Vector4<Type>& operator += (const Vector4<Type>& vec);
      Vector4<Type>& operator -= (const Vector4<Type>& vec);
      Vector4<Type>& operator *= (const Type& scalar);
      Vector4<Type>& operator /= (const Type& scalar);

      union
      {
        Type pData[4];
        struct
        {
          Type x; ///<  The first component
          Type y; ///<  The second component
          Type z; ///<  The third component
          Type w; ///<  The fourth component
        };
        struct
        {
          Type r; ///<  Alias of first component
          Type g; ///<  Alias of second component
          Type b; ///<  Alias of third component
          Type a; ///<  Alias of fourth component
        };
      };
  };

  template <class Vector>
  struct boost::qvm::vec_traits;

  template <class Type>
  struct boost::qvm::vec_traits<ARSTD::Vector4<Type>>
  {
    typedef Type scalar_type;
    static int const dim = 4;

    template <int I>
    static scalar_type read_element(ARSTD::Vector4<Type> const& x)
    {
      return x.pData[I];
    }

    template <int I>
    static scalar_type& write_element(ARSTD::Vector4<Type>& x)
    {
      return x.pData[I];
    }

    static scalar_type read_element_idx(int i, ARSTD::Vector4<Type> const& x)
    {
      return x.pData[i];
    }

    static scalar_type& write_element_idx(int i, ARSTD::Vector4<Type>& x)
    {
      return x.pData[i];
    }
  };


  typedef Vector4<float> Vector4F;
  typedef Vector4<double> Vector4D;


  /**
   * Compares two vectors with the epsilon given by the parameter fEpsilon
   * @return  true, if the vectors are considered equal, false otherwise
   * @relates Vector4
   */
  template <class Type>
  bool equals(const Vector4<Type>& lhs, const Vector4<Type>& rhs, double fEpsilon = EPSILON);

  /**
   * Compares two matrices using default EPSILON
   * @return  true, if the vectors are considered equal, false otherwise
   * @relates Vector4
   */
  template <class Type>
  bool operator == (const Vector4<Type>& lhs, const Vector4<Type>& rhs);

  /**
   * Compares two matrices using default EPSILON
   * @return  true, if the vectors are considered as not equal, false otherwise
   * @relates Vector4
   */
  template <class Type>
  bool operator != (const Vector4<Type>& lhs, const Vector4<Type>& rhs);

  /**
   * Writes the vector to a std output stream
   * @relates Vector4
   */
  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Vector4<Type>& vec);

  /**
   * Reads the vector from a std input stream
   * @relates Vector4
   */
  template <class Type>
  std::istream& operator >> (std::istream& stream, Vector4<Type>& vec);


} // namespace ARSTD


#include "Vector4_Impl.h"
