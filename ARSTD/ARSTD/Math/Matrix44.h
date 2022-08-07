// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#include <iosfwd>
#include <boost/qvm/mat.hpp>


namespace ARSTD
{

  template <class Type> class Matrix;

  /**
   * This represents a 4x4 Matrix
   */
  template <class Type>
  struct Matrix44
  {
    public:
      enum ERow { ROW0 = 0, ROW1, ROW2, ROW3 };
      enum EColumn { COL0 = 0, COL1, COL2, COL3 };
      enum EAxis { XAXIS = 0, YAXIS, ZAXIS };


      template <class R>
      operator R() const
      {
        R r;
        assign(r, *this);
        return r;
      }

      /**
       * The identity matrix.
       */
      static const Matrix44<Type> IDENTITY;

      /** 
       * Inverts the matrix
       * @param  mat    The 4x4 matrix
       * @return        The inverted matrix
       */
      static Matrix44<Type> inverse(const Matrix44<Type>& mat);

      /**
       * Spherical linear interpolation of two matrices
       * @return The interpolated matrix
       */
      static Matrix44<Type> slerp(const Matrix44<Type>& mat1, const Matrix44<Type>& mat2, const double& t);

      /**
       * Constructor, which is as default not initialized
       * @param bInitAsIdentity If set to false the matrix is not initialized, otherwise an identity matrix is initialized
       */
      explicit Matrix44(bool bInitAsIdentity = true);

      /**
       * Contructs a translation matrix by given translation vector
       * @param vecTranslation The translation vector
       */
      explicit Matrix44(const Vector3<Type>& vecTranslation);

      /**
       * Constructs a matrix with the given generic matrix elements
       * @param mat The generic matrix from where to set
       * @pre mat has to be a 4x4 matrix
       */
      explicit Matrix44(const Matrix<Type>& mat);

      /**
       * Constructs a matrix with a given rotation and translation
       * The homogenous value (ww) of the 4th column is set to 1
       * @param rotation The rotation 3x3 matrix
       * @param vecTranslation The translation vector
       */
      Matrix44(const Matrix33<Type>& rotation, const Vector3<Type>& vecTranslation);

      /**
       * Constructs a matrix with a given rotation and translation
       * The homogenous value (ww) of the 4th column is set to 1
       * @param rotation The rotation quaternion
       * @param vecTranslation The translation vector
       */
      Matrix44(const Quaternion<Type>& rotation, const Vector3<Type>& vecTranslation);

      /**
       * Constructs a matrix with 4 three-component vectors, one for each column
       * The forth row of the matrix is set to (0 0 0 1)
       * @param vecXAxis The x-axis vector
       * @param vecYAxis The y-axis vector
       * @param vecZAxis The z-axis vector
       * @param vecTranslation The translation vector
       */
      Matrix44(const Vector3<Type>& vecXAxis, const Vector3<Type>& vecYAxis, 
               const Vector3<Type>& vecZAxis, const Vector3<Type>& vecTranslation);

      /**
       * Constructs a the matrix with 4 four-component vectors, one for each column
       * @param vecXAxis The x-axis vector
       * @param vecYAxis The y-axis vector
       * @param vecZAxis The z-axis vector
       * @param vecTranslation The translation that vector.
       */
      Matrix44(const Vector4<Type>& vecColumn0, const Vector4<Type>& vecColumn1,
               const Vector4<Type>& vecColumn2, const Vector4<Type>& vecColumn3);

      template<class Type2>
      explicit Matrix44(const Matrix44<Type2>& other)
      { 
        for (int i = 0; i < 16; i++)
        {
          this->pData[i] = static_cast<Type>(other.pData[i]);
        }
      }

      /**
       * Matrix multiplication
       * @param  other    The other matrix
       * @return     The multiplied matrix
       */
      Matrix44<Type>& operator *= (const Matrix44<Type>& other);

      /**
       * Gives access to the x-axis of the 3x3 rotational part.
       */
      Vector3<Type>& getXAxis();

      /**
       * Returns the x-axis vector
       */
      const Vector3<Type>& getXAxis() const;

      /**
       * Sets the x-axis vector
       */
      void setXAxis(const Vector3<Type>& vecXAxis);

      /**
       * Gives access to y-axis vector
       */
      Vector3<Type>& getYAxis();

      /**
       * Returns the y-axis vector
       */
      const Vector3<Type>& getYAxis() const;

      /**
       * Sets the y-axis vector
       */
      void setYAxis(const Vector3<Type>& vecYAxis);

      /**
       * Gives access to the z-axis vector
       */
      Vector3<Type>& getZAxis();

      /**
       * Returns the z-axis vector
       */
      const Vector3<Type>& getZAxis() const;

      /**
       * Sets the z-axis vector
       */
      void setZAxis(const Vector3<Type>& vecZAxis);

      /**
       * Gives access to the translation vector
       */
      Vector3<Type>& getTranslation();

      /**
       * Returns the translation vector
       */
      const Vector3<Type>& getTranslation() const;

      /**
       * Sets the translation vector
       */
      void setTranslation(const Vector3<Type>& vecTranslation);

      /**
       * Sets the matrix to the identity
       */
      void setIdentity();

      /** 
       * Checks if the matrix is to identity matrix
       * @param epsilon
       * @return  true, if the matrix is equal, otherwise false
       */
      bool isIdentity(const Type& epsilon = EPSILON) const;

      /**
       * Returns the rotation as 3x3 matrix
       * @return  The 3x3 matrix
       */
      const Matrix33<Type> getRotationAsMatrix33() const;

      /** 
       * Returns the rotation as quaternion
       * @return  The quaternion
       */
      const Quaternion<Type> getRotationAsQuaternion() const;

      /**
       * Sets the rotation using rotation axis and rotation angle
       * @param vecAxis    The rotation axis
       * @param angle      The rotation angle
       */
      void setRotation(const Vector3<Type>& vecAxis, const Type& angle);
    
      /**
       * Sets the rotation using 3x3 matrix
       * @param mat The 3x3 Matrix
       */
      void setRotation(const Matrix33<Type>& mat);

      /**
       * Sets the rotation using quaternion
       * @param rotation  The quaternion
       */
      void setRotation(const Quaternion<Type>& quat);

      /**
       * Transposes the matrix
       */
      void transpose();

      /**
       * Transposes the rotational part of the matrix only
       */
      void transposeRot();

      /** 
       * Inverts the matrix
       */
      void invert();

      /**
       * Rebuilds a homogeneous transformation matrix by discarding a given axis
       * @param   axis   The axis, that is discarded
       */
      void makeHomogeneous(const EAxis& axis);

      union
      {
        Type pData[16];
        Type pData2[4][4];
        struct
        {
          Type xx, yx, zx, wx;
          Type xy, yy, zy, wy;
          Type xz, yz, zz, wz;
          Type xw, yw, zw, ww;
        };
      };
  };


  template <class Matrix>
  struct boost::qvm::mat_traits;

  template <class Type>
  struct boost::qvm::mat_traits<ARSTD::Matrix44<Type>>
  {
    typedef Type scalar_type;
    static int const rows = 4;
    static int const cols = 4;

    template <int Row, int Col>
    static scalar_type read_element(ARSTD::Matrix44<Type> const& x)
    {
      return x.pData2[Col][Row];
    }

    template <int Row, int Col>
    static scalar_type& write_element(ARSTD::Matrix44<Type>& x)
    {
      return x.pData2[Col][Row];
    }

    static scalar_type read_element_idx(int row, int col, ARSTD::Matrix44<Type> const& x)
    {
      return x.pData2[col][row];
    }

    static scalar_type& write_element_idx(int row, int col, ARSTD::Matrix44<Type>& x)
    {
      return x.pData2[col][row];
    }
  };


  typedef Matrix44<float> Matrix44F;
  typedef Matrix44<double> Matrix44D;


  /**
   * Matrix matrix multiplication
   * @relates Matrix44
   */
  template <class Type>
  const Matrix44<Type> operator * (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs);

  /**
   * Matrix vector multiplication assuming the 4th component of the vector to be 1
   * @relates Matrix44
   */
  template <class Type>
  const Vector3<Type> operator * (const Matrix44<Type>& lhs, const Vector3<Type>& rhs);

  /**
   * Matrix vector multiplication
   * @relates Matrix44
   */
  template <class Type>
  const Vector4<Type> operator * (const Matrix44<Type>& lhs, const Vector4<Type>& rhs);

  /**
   * Compares two matrices with the epsilon given by the parameter fEpsilon
   * @sa EPSILON
   * @relates Matrix44
   */
  template <class Type>
  bool equals(const Matrix44<Type>& lhs, const Matrix44<Type>& rhs, double fEpsilon = EPSILON);

  /**
   * Compares two matrices using default EPSILON
   * @sa EPSILON
   * @relates Matrix44
   */
  template <class Type>
  bool operator == (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs);

  /**
   * Compares two matrices using default EPSILON
   * @sa EPSILON
   * @relates Matrix44
   */
  template <class Type>
  bool operator != (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs);

  /**
   * Writes the matrix to a std output stream
   * @relates Matrix44
   */
  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Matrix44<Type>& mat);

  /**
   * Reads a matrix from a std input stream
   * @relates Matrix44
   */
  template <class Type>
  std::istream& operator >> (std::istream& stream, Matrix44<Type>& mat);


} // namespace ARSTD


#include "Matrix44_Impl.h"
