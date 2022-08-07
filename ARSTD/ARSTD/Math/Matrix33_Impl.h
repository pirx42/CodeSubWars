// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/qvm/mat_operations.hpp>

namespace ARSTD
{


  template <class Type>
  const Matrix33<Type> Matrix33<Type>::IDENTITY = Matrix33<Type>(true);


  template <class Type>
  Matrix33<Type> Matrix33<Type>::inverse(const Matrix33<Type>& mat)
  {
    return boost::qvm::inverse(mat);
  }


  template <class Type>
  Matrix33<Type>::Matrix33(bool bInitAsIdentity)
  {
    if (bInitAsIdentity)
      setIdentity();
  }


  template <class Type>
  Matrix33<Type>::Matrix33(const Quaternion<Type>& quat)
  {
    set(quat);
  }


  template <class Type>
  Matrix33<Type>::Matrix33(const Matrix<Type>& mat)
  {
    assert(mat.getColumns() == 3 && mat.getRows() == 3);
    memcpy(this->pData, mat.getData(), 9*sizeof(Type));
  }


  template <class Type>
  Matrix33<Type>::Matrix33(const Vector3<Type>& vecXAxis, const Vector3<Type>& vecYAxis, 
                           const Vector3<Type>& vecZAxis)
  : xx(vecXAxis.x),
    yx(vecXAxis.y),
    zx(vecXAxis.z),
    xy(vecYAxis.x),
    yy(vecYAxis.y),
    zy(vecYAxis.z),
    xz(vecZAxis.x),
    yz(vecZAxis.y),
    zz(vecZAxis.z) 
  {
  }


  template <class Type>
  Matrix33<Type>& Matrix33<Type>::operator *= (const Type& rhs)
  {
    this->xx *= rhs; this->xy *= rhs; this->xz *= rhs;
    this->yx *= rhs; this->yy *= rhs; this->yz *= rhs;
    this->zx *= rhs; this->zy *= rhs; this->zz *= rhs;

    return *this;
  }


  template <class Type>
  Matrix33<Type>& Matrix33<Type>::operator += (const Matrix33<Type>& rhs)
  {
    this->xx += rhs.xx; this->xy += rhs.xy; this->xz += rhs.xz;
    this->yx += rhs.yx; this->yy += rhs.yy; this->yz += rhs.yz;
    this->zx += rhs.zx; this->zy += rhs.zy; this->zz += rhs.zz;

    return *this;
  }


  template <class Type>
  Matrix33<Type>& Matrix33<Type>::operator -= (const Matrix33<Type>& rhs)
  {
    this->xx -= rhs.xx; this->xy -= rhs.xy; this->xz -= rhs.xz;
    this->yx -= rhs.yx; this->yy -= rhs.yy; this->yz -= rhs.yz;
    this->zx -= rhs.zx; this->zy -= rhs.zy; this->zz -= rhs.zz;

    return *this;
  }


  template <class Type>
  Matrix33<Type>& Matrix33<Type>::operator *= (const Matrix33<Type>& rhs)
  {
    return boost::qvm::operator*=(*this, rhs);
  }


  template <class Type>
  Vector3<Type>& Matrix33<Type>::getXAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xx);
  }


  template <class Type>
  const Vector3<Type>& Matrix33<Type>::getXAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xx);
  }


  template <class Type>
  void Matrix33<Type>::setXAxis(const Vector3<Type>& vecXAxis)
  {
    getXAxis() = vecXAxis;
  }


  template <class Type>
  Vector3<Type>& Matrix33<Type>::getYAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xy);
  }


  template <class Type>
  const Vector3<Type>& Matrix33<Type>::getYAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xy);
  }


  template <class Type>
  void Matrix33<Type>::setYAxis(const Vector3<Type>& vecYAxis)
  {
    getYAxis() = vecYAxis;
  }


  template <class Type>
  Vector3<Type>& Matrix33<Type>::getZAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xz);
  }


  template <class Type>
  const Vector3<Type>& Matrix33<Type>::getZAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xz);
  }


  template <class Type>
  void Matrix33<Type>::setZAxis(const Vector3<Type>& vecZAxis)
  {
    getZAxis() = vecZAxis;
  }


  template <class Type>
  void Matrix33<Type>::get(Type& rotX, Type& rotY, Type& rotZ) const
  {
    rotX = static_cast<Type>(atan2(this->yz, this->zz));
    rotY = static_cast<Type>(asin(-this->xz));
    rotZ = static_cast<Type>(atan2(this->xy, this->xx));
  }


  template <class Type>
  Quaternion<Type> Matrix33<Type>::getAsQuaternion() const
  {
    return boost::qvm::convert_to<Quaternion<Type>>(*this);
  }


  template <class Type>
  void Matrix33<Type>::setIdentity()
  {
    boost::qvm::set_identity(*this);
  }


  template <class Type>
  bool Matrix33<Type>::isIdentity(const Type& epsilon) const
  {
    return equals(IDENTITY, *this, epsilon);
  }


  template <class Type>
  void Matrix33<Type>::set(const Matrix33<Type>& mat)
  {
    *this = mat;
  }


  template <class Type>
  void Matrix33<Type>::set(const Quaternion<Type>& quat)
  {
    *this = quat.getAsMatrix33();
  }


  template <class Type>
  void Matrix33<Type>::set(const Vector3<Type>& vecAxis, const Type& angle)
  {
    boost::qvm::set_rot(*this, vecAxis, angle);
  }


  template <class Type>
  void Matrix33<Type>::transpose()
  {
    std::swap(this->yx, this->xy);
    std::swap(this->zx, this->xz);
    std::swap(this->zy, this->yz);
  }


  template <class Type>
  const Type Matrix33<Type>::determinant() const
  {
    return boost::qvm::determinant(*this);
  }


  template <class Type>
  void Matrix33<Type>::makeRotationMatrix(const EColumn& axis)
  {
    switch (axis)
    {
      case COL0:
        getZAxis().normalize();
        setXAxis((getYAxis() % getZAxis()).getNormalized());
        setYAxis(getZAxis() % getXAxis());
        break;

      case COL1:
        getXAxis().normalize();
        setYAxis((getZAxis() % getXAxis()).getNormalized());
        setZAxis(getXAxis() % getYAxis());
        break;

      case COL2:
        getYAxis().normalize();
        setZAxis((getXAxis() % getYAxis()).getNormalized());
        setXAxis(getYAxis() % getZAxis());
        break;
    }
  }


  template <class Type>
  const Matrix33<Type> operator * (const Matrix33<Type>& lhs, const Type& rhs)
  {
    return Matrix33<Type>(lhs) *= rhs;
  }


  template <class Type>
  const Vector3<Type> operator * (const Matrix33<Type>& mat, const Vector3<Type>& vec)
  {
    return boost::qvm::operator*(mat, vec);
  }


  template <class Type>
  const Matrix33<Type> operator * (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs)
  {
    return Matrix33<Type>(lhs) *= rhs;
  }


  template <class Type>
  bool equals(const Matrix33<Type>& lhs, const Matrix33<Type>& rhs, double fEpsilon)
  {
    if (&lhs == &rhs)
      return true;
    return boost::qvm::cmp(lhs, rhs, [fEpsilon](const Type& lc, const Type& rc) { return fabs(lc - rc) < fEpsilon; });
  }


  template <class Type>
  bool operator == (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs)
  {
    return equals(lhs, rhs, EPSILON);
  }


  template <class Type>
  bool operator != (const Matrix33<Type>& lhs, const Matrix33<Type>& rhs)
  {
    return !operator==(lhs, rhs);
  }


} // namespace ARSTD
