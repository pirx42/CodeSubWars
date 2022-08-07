// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace ARSTD
{

  template <typename Type>
  Matrix<Type>::Matrix(const uint nRows, const uint nColumns, const Type& init)
  : m_pData(new Type[nRows*nColumns]),
    m_nRows(nRows),
    m_nColumns(nColumns)
  {
    assert(nRows > 0 && nColumns > 0);

    uint nElements = nRows*nColumns;
    for (uint nCnt = 0; nCnt < nElements; ++nCnt)
    {
      m_pData[nCnt] = init;
    }
  }


  template <typename Type>
  Matrix<Type>::Matrix(const Matrix33<Type>& mat)
  : m_pData(new Type[3*3]),
    m_nRows(3),
    m_nColumns(3)
  {
    memcpy(m_pData, mat.pData, 3*3*sizeof(Type));
  }


  template <typename Type>
  Matrix<Type>::Matrix(const Matrix44<Type>& mat)
  : m_pData(new Type[4*4]),
    m_nRows(4),
    m_nColumns(4)
  {
    memcpy(m_pData, mat.pData, 4*4*sizeof(Type));
  }


  template <typename Type>
  Matrix<Type>::Matrix(const Matrix<Type>& other)
  : m_pData(new Type[other.m_nRows*other.m_nColumns]),
    m_nRows(other.m_nRows),
    m_nColumns(other.m_nColumns)
  {
    memcpy(m_pData, other.m_pData, other.m_nRows*other.m_nColumns*sizeof(Type));
  }


  template <typename Type>
  Matrix<Type>::~Matrix()
  {
    delete [] m_pData;
  }


  template <typename Type>
  const uint Matrix<Type>::getRows() const
  {
    return m_nRows;
  }


  template <typename Type>
  const uint Matrix<Type>::getColumns() const
  {
    return m_nColumns;
  }


  template <typename Type>
  const Matrix<Type> Matrix<Type>::getRow(const uint nRow) const
  {
    assert(nRow < m_nRows);

    Matrix vec(1, m_nColumns);
    
    uint nSrcIdx(nRow);
    for (uint i = 0; i < m_nColumns; ++i)
    {
      vec.getData()[i] = m_pData[nSrcIdx];
      nSrcIdx += m_nRows;
    }

    return vec;
  }

  
  template <typename Type>
  const Matrix<Type> Matrix<Type>::getColumn(const uint nColumn) const
  {
    assert(nColumn < m_nColumns);

    Matrix vec(m_nRows);
    memcpy(vec.getData(), m_pData + nColumn*m_nRows, m_nRows*sizeof(Type));

    return vec;
  }


  template <typename Type>
  void Matrix<Type>::setRow(const uint nRow, const Matrix<Type> vec)
  {
    assert(nRow < m_nRows &&
           vec.getColumns() == m_nColumns &&
           vec.getRows() == 1);

    uint nDstIdx(nRow);
    for (uint i = 0; i < m_nColumns; ++i)
    {
      m_pData[nDstIdx] = vec.getData()[i];
      nDstIdx += m_nRows;
    }
  }


  template <typename Type>
  void Matrix<Type>::setColumn(const uint nColumn, const Matrix<Type> vec)
  {
    assert(nColumn < m_nColumns &&
           vec.getRows() == m_nRows &&
           vec.getColumns() == 1);
    
    memcpy(m_pData + nColumn*m_nRows, vec.getData(), m_nRows*sizeof(Type));
  }


  template <typename Type>
  void Matrix<Type>::setZero()
  {
    memset(m_pData, 0, m_nRows*m_nColumns*sizeof(Type));
  }


  template <typename Type>
  void Matrix<Type>::setIdentity()
  {
    this->setZero();
    uint nMax(std::min(m_nRows, m_nColumns));
    uint nInc(m_nRows + 1);
    uint nIdx(0);
    for (uint i = 0; i < nMax; ++i)
    {
      m_pData[nIdx] = 1;
      nIdx += nInc;
    }
  }


  template <typename Type>
  const std::pair<Matrix<Type>, bool> Matrix<Type>::inverse(const Matrix<Type>& mat)
  {
    if (mat.getRows() == mat.getColumns())
      return ordinaryInverse(mat);
    else
      return pseudoInverse(mat);
  }


  template <typename Type>
  bool Matrix<Type>::invert()
  {
    std::pair<Matrix<Type>, bool> result = inverse(*this);
    if (result.second)
    {
      m_nRows = result.first.getRows();
      m_nColumns = result.first.getColumns();

      delete [] m_pData;
      m_pData = new Type[m_nRows*m_nColumns];
      memcpy(m_pData, result.first.getData(), m_nRows*m_nColumns*sizeof(Type));
    }

    return result.second;
  }


  template <typename Type>
  void Matrix<Type>::transpose()
  {
    if (m_nRows == 1 || m_nColumns == 1)
    {
      std::swap(m_nRows, m_nColumns);
    }
    else
    {
      Type* pNewData = new Type[m_nRows*m_nColumns];

      uint nNewRows(m_nColumns);
      uint nNewColumns(m_nRows);
      uint nDestIdx(0);
      uint nSrcIdx(0);
      uint i, j;
      for (j = 0; j < nNewColumns; ++j)
      {
        nSrcIdx = j;
        for (i = 0; i < nNewRows; ++i)
        {
          pNewData[nDestIdx] = m_pData[nSrcIdx];
          ++nDestIdx;
          nSrcIdx += m_nRows;
        }
      }

      delete [] m_pData;
      m_pData = pNewData;
      m_nRows = nNewRows;
      m_nColumns = nNewColumns;
    }
  }


  template <typename Type>
  const Matrix<Type> Matrix<Type>::getTransposed() const
  {
    Matrix<Type> mat(*this);
    mat.transpose();
    
    return mat;
  }


  template <typename Type>
  typename Type& Matrix<Type>::operator()(const uint nRow, const uint nColumn)
  {
    assert(nRow < m_nRows && nColumn < m_nColumns);

    return m_pData[nRow + nColumn*m_nRows];
  }


  template <typename Type>
  typename const Type& Matrix<Type>::operator()(const uint nRow, const uint nColumn) const
  {
    assert(nRow < m_nRows && nColumn < m_nColumns);

    return m_pData[nRow + nColumn*m_nRows];
  }


  template <typename Type>
  Type* Matrix<Type>::getData()
  {
    return m_pData;
  }


  template <typename Type>
  const Type* Matrix<Type>::getData() const
  {
    return m_pData;
  }


  template <typename Type>
  void Matrix<Type>::operator=(const Matrix<Type>& other)
  {
    if (this == &other)
      return;

    if (m_nRows != other.getRows() ||
        m_nColumns != other.getColumns())
    {
      delete [] m_pData;
      m_pData = new Type[other.m_nRows*other.m_nColumns];

      m_nRows = other.m_nRows;
      m_nColumns = other.m_nColumns;
    }

    memcpy(m_pData, other.m_pData, other.m_nRows*other.m_nColumns*sizeof(Type));
  }


  template <typename Type>
  const std::pair<Matrix<Type>, bool> Matrix<Type>::ordinaryInverse(const Matrix<Type>& mat)
  {
    assert(mat.getRows() == mat.getColumns());

    uint i, j;
    boost::numeric::ublas::matrix<Type> input(mat.getRows(), mat.getColumns());
    for (j = 0; j < mat.getColumns(); ++j)
    {
      for (i = 0; i < mat.getRows(); ++i)
        input(i, j) = mat(i, j);
    }

    boost::numeric::ublas::permutation_matrix<std::size_t> permutationMatrix(input.size1());

    int result = boost::numeric::ublas::lu_factorize(input, permutationMatrix);
    if (result != 0)
      return std::make_pair(mat, false);

    boost::numeric::ublas::matrix<Type> inverseOfInput(input.size1(), input.size2());
    inverseOfInput.assign(boost::numeric::ublas::identity_matrix<Type>(input.size1()));

    boost::numeric::ublas::lu_substitute(input, permutationMatrix, inverseOfInput);

    Matrix<Type> tmp(inverseOfInput.size1(), inverseOfInput.size2());
    for (j = 0; j < inverseOfInput.size2(); ++j)
    {
      for (i = 0; i < inverseOfInput.size1(); ++i)
        tmp(j, i) = inverseOfInput(i, j);
    }

    return std::make_pair(tmp, true);
  }


  template <typename Type>
  const std::pair<Matrix<Type>, bool> Matrix<Type>::pseudoInverse(const Matrix<Type>& mat)
  {
    assert(mat.getRows() != mat.getColumns());

    Matrix<Type> matToInvertTransposed(mat);
    matToInvertTransposed.transpose();

    std::pair<Matrix<Type>, bool> result = ordinaryInverse(matToInvertTransposed * mat);
    if (!result.second)
      return result;

    return std::make_pair(result.first * matToInvertTransposed, true);
  }


  template <typename Type>
  const Matrix<Type> operator*(const Matrix<Type>& lhs, const Matrix<Type>& rhs)
  {
    assert(lhs.getColumns() == rhs.getRows());

    uint nRows = lhs.getRows();
    uint nColumns = rhs.getColumns();
    uint nElements = lhs.getColumns();
    uint i, j, k;
    Matrix<Type> res(nRows, nColumns);

    for (i = 0; i < nRows; ++i)
    {
      for (j = 0; j < nColumns; ++j)
      {
        for (k = 0; k < nElements; ++k)
        {
          res(i, j) += lhs(i, k)*rhs(k, j);
        }
      }
    }

    return res;
  }

}
