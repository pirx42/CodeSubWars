// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#include <boost/qvm/mat.hpp>


namespace ARSTD
{

  template <class Type> class Matrix;


  template <class Type>
  struct Matrix33
  {
    public:
      enum ERow { ROW0 = 0, ROW1, ROW2};
      enum EColumn { COL0 = 0, COL1, COL2};

      template <class R>
      operator R() const
      {
        R r;
        assign(r, *this);
        return r;
      }

      static const Matrix33<Type> IDENTITY;

      static Matrix33<Type> inverse(const Matrix33<Type>& mat);

      explicit Matrix33(bool bInitAsIdentity = true);
      explicit Matrix33(const Quaternion<Type>& quat);
      explicit Matrix33(const Matrix<Type>& mat);
      Matrix33(const Vector3<Type>& vecXAxis, const Vector3<Type>& vecYAxis, const Vector3<Type>& vecZAxis);

      template<class Type2>
      explicit Matrix33(const Matrix33<Type2>& other)
      {
        for (int i = 0; i < 9; i++)
        {
          this->pData[i] = static_cast<Type>(other.pData[i]);
        }
      }

      Matrix33<Type>& operator *= (const Type& rhs);
      Matrix33<Type>& operator += (const Matrix33<Type>& rhs);
      Matrix33<Type>& operator -= (const Matrix33<Type>& rhs);
      Matrix33<Type>& operator *= (const Matrix33<Type>& rhs);

      Vector3<Type>& getXAxis();
      const Vector3<Type>& getXAxis() const;
      void setXAxis(const Vector3<Type>& vecXAxis);

      Vector3<Type>& getYAxis();
      const Vector3<Type>& getYAxis() const;
      void setYAxis(const Vector3<Type>& vecYAxis);

      Vector3<Type>& getZAxis();
      const Vector3<Type>& getZAxis() const;
      void setZAxis(const Vector3<Type>& vecZAxis);

      void get(Type& rotX, Type& rotY, Type& rotZ) const;

      Quaternion<Type> getAsQuaternion() const;

      void setIdentity();

      bool isIdentity(const Type& epsilon = EPSILON) const;

      void set(const Matrix33<Type>& mat);
      void set(const Quaternion<Type>& quat);
      void set(const Vector3<Type>& vecAxis, const Type& angle);

      void transpose();

      const Type determinant() const;

      void makeRotationMatrix(const EColumn& axis);

      union
      {
        Type pData[9];
        Type pData2[3][3];
        struct
        {
          Type xx, yx, zx;
          Type xy, yy, zy;
          Type xz, yz, zz;
        };
      };
  };


  template <class Matrix>
  struct boost::qvm::mat_traits;

  template <class Type>
  struct boost::qvm::mat_traits<ARSTD::Matrix33<Type>>
  {
    typedef Type scalar_type;
    static int const rows = 3;
    static int const cols = 3;

    template <int Row, int Col>
    static scalar_type read_element(ARSTD::Matrix33<Type> const& x)
    {
      return x.pData2[Col][Row];
    }

    template <int Row, int Col>
    static scalar_type& write_element(ARSTD::Matrix33<Type>& x)
    {
      return x.pData2[Col][Row];
    }

    static scalar_type read_element_idx(int row, int col, ARSTD::Matrix33<Type> const& x)
    {
      return x.pData2[col][row];
    }

    static scalar_type& write_element_idx(int row, int col, ARSTD::Matrix33<Type>& x)
    {
      return x.pData2[col][row];
    }
  };


  typedef Matrix33<float> Matrix33F;
  typedef Matrix33<double> Matrix33D;


  template <class Type, class Type2>
  const Matrix33<Type> operator * (const Matrix33<Type>& lhs, const Type2& rhs);

  template <class Type>
  const Matrix33<Type> operator * (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs);

  template <class Type>
  const Vector3<Type> operator * (const Matrix33<Type>& lhs, const Vector3<Type>& rhs);

  template <class Type>
  bool equals(const Matrix33<Type>& lhs, const Matrix33<Type>& rhs, double fEpsilon = EPSILON);

  template <class Type>
  bool operator == (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs);

  template <class Type>
  bool operator != (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs);


} // namespace ARSTD


#include "Matrix33_Impl.h"
