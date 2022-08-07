// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

namespace ARSTD
{

  template <typename Type>
  class Matrix
  {
    public:
      explicit Matrix(const uint nRows = 1, const uint nColumns = 1, const Type& init = Type(0));
      explicit Matrix(const Matrix33<Type>& mat);
      explicit Matrix(const Matrix44<Type>& mat);

      Matrix(const Matrix<Type>& other);

      ~Matrix();

      static const std::pair<Matrix<Type>, bool> inverse(const Matrix<Type>& mat);

      const uint getRows() const;
      const uint getColumns() const;

      const Matrix<Type> getRow(const uint nRow) const;
      const Matrix<Type> getColumn(const uint nColumn) const;
      void setRow(const uint nRow, const Matrix<Type> vec);
      void setColumn(const uint nColumn, const Matrix<Type> vec);

      void setZero();
      void setIdentity();
      bool invert();
      void transpose();
      const Matrix<Type> getTransposed() const;

      Type& operator()(const uint nRow, const uint nColumn = 0);
      const Type& operator()(const uint nRow, const uint nColumn = 0) const;

      Type* getData();
      const Type* getData() const;

      void operator=(const Matrix<Type>& other);

    protected:
      static const std::pair<Matrix<Type>, bool> ordinaryInverse(const Matrix<Type>& mat);
      static const std::pair<Matrix<Type>, bool> pseudoInverse(const Matrix<Type>& mat);

      Type* m_pData;
      uint m_nRows;
      uint m_nColumns;
  };

  typedef Matrix<double> MatrixD;

}


#include "Matrix_Impl.h"
