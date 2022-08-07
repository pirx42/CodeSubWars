// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

namespace ARSTD
{

  template <class Type>
  const Matrix44<Type> Matrix44<Type>::IDENTITY = Matrix44<Type>(true);


  template <class Type>
  Matrix44<Type> Matrix44<Type>::inverse(const Matrix44<Type>& mat)
  {
    return boost::qvm::inverse(mat);
  }


  template <class Type>
  Matrix44<Type> Matrix44<Type>::slerp(const Matrix44<Type>& mat1, const Matrix44<Type>& mat2, const double& t)
  {
    return Matrix44<Type>(Quaternion<Type>::slerp(mat1.getRotationAsQuaternion(), mat2.getRotationAsQuaternion(), t),
                          Vector3<Type>::lerp(mat1.getTranslation(), mat2.getTranslation(), t));
  }


  template <class Type>
  Matrix44<Type>::Matrix44(bool bInitAsIdentity)
  {
    if (bInitAsIdentity)
      setIdentity();
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Vector3<Type>& vecTranslation)
  {
    setIdentity();
    setTranslation(vecTranslation);
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Matrix<Type>& mat)
  {
    assert(mat.getColumns() == 4 && mat.getRows() == 4);
    memcpy(this->pData, mat.getData(), 16*sizeof(Type));
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Matrix33<Type>& rotation, const Vector3<Type>& vecTranslation)
  {
    setIdentity();
    setRotation(rotation);
    setTranslation(vecTranslation);
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Quaternion<Type>& rotation, const Vector3<Type>& vecTranslation)
  {
    setIdentity();
    setRotation(rotation);
    setTranslation(vecTranslation);
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Vector3<Type>& vecXAxis, const Vector3<Type>& vecYAxis, 
                           const Vector3<Type>& vecZAxis, const Vector3<Type>& vecTranslation)
  : xx(vecXAxis.x),
    yx(vecXAxis.y),
    zx(vecXAxis.z),
    wx(0),
    xy(vecYAxis.x),
    yy(vecYAxis.y),
    zy(vecYAxis.z),
    wy(0),
    xz(vecZAxis.x),
    yz(vecZAxis.y),
    zz(vecZAxis.z),
    wz(0),
    xw(vecTranslation.x),
    yw(vecTranslation.y),
    zw(vecTranslation.z),
    ww(1)
  {
  }


  template <class Type>
  Matrix44<Type>::Matrix44(const Vector4<Type>& vecColumn0, const Vector4<Type>& vecColumn1,
                           const Vector4<Type>& vecColumn2, const Vector4<Type>& vecColumn3)
  : xx(vecColumn0.x),
    yx(vecColumn0.y),
    zx(vecColumn0.z),
    wx(vecColumn0.w),
    xy(vecColumn1.x),
    yy(vecColumn1.y),
    zy(vecColumn1.z),
    wy(vecColumn1.w),
    xz(vecColumn2.x),
    yz(vecColumn2.y),
    zz(vecColumn2.z),
    wz(vecColumn2.w),
    xw(vecColumn3.x),
    yw(vecColumn3.y),
    zw(vecColumn3.z),
    ww(vecColumn3.w)
  {
  }


  template <class Type>
  Matrix44<Type>& Matrix44<Type>::operator *= (const Matrix44<Type>& rhs)
  {
    return boost::qvm::operator*=(*this, rhs);
  }


  template <class Type>
  Vector3<Type>& Matrix44<Type>::getXAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xx);
  }


  template <class Type>
  const Vector3<Type>& Matrix44<Type>::getXAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xx);
  }


  template <class Type>
  void Matrix44<Type>::setXAxis(const Vector3<Type>& vecXAxis)
  {
    getXAxis() = vecXAxis;
  }


  template <class Type>
  Vector3<Type>& Matrix44<Type>::getYAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xy);
  }


  template <class Type>
  const Vector3<Type>& Matrix44<Type>::getYAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xy);
  }


  template <class Type>
  void Matrix44<Type>::setYAxis(const Vector3<Type>& vecYAxis)
  {
    getYAxis() = vecYAxis;
  }


  template <class Type>
  Vector3<Type>& Matrix44<Type>::getZAxis()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xz);
  }


  template <class Type>
  const Vector3<Type>& Matrix44<Type>::getZAxis() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xz);
  }


  template <class Type>
  void Matrix44<Type>::setZAxis(const Vector3<Type>& vecZAxis)
  {
    getZAxis() = vecZAxis;
  }


  template <class Type>
  Vector3<Type>& Matrix44<Type>::getTranslation()
  {
    return *reinterpret_cast<Vector3<Type>*>(&this->xw);
  }


  template <class Type>
  const Vector3<Type>& Matrix44<Type>::getTranslation() const
  {
    return *reinterpret_cast<const Vector3<Type>*>(&this->xw);
  }


  template <class Type>
  void Matrix44<Type>::setTranslation(const Vector3<Type>& vecTranslation)
  {
    getTranslation() = vecTranslation;
  }


  template <class Type>
  void Matrix44<Type>::setIdentity()
  {
    boost::qvm::set_identity(*this);
  }


  template <class Type>
  bool Matrix44<Type>::isIdentity(const Type& epsilon) const
  {
    return equals(IDENTITY, *this, epsilon);
  }


  template <class Type>
  const Matrix33<Type> Matrix44<Type>::getRotationAsMatrix33() const
  {
    return Matrix33<Type>(getXAxis(), getYAxis(), getZAxis());
  }


  template <class Type>
  const Quaternion<Type> Matrix44<Type>::getRotationAsQuaternion() const
  {
    return Quaternion<Type>(getRotationAsMatrix33());
  }


  template <class Type>
  void Matrix44<Type>::setRotation(const Vector3<Type>& vecAxis, const Type& angle)
  {
    setRotation(Quaternion<Type>(vecAxis, angle));
  }


  template <class Type>
  void Matrix44<Type>::setRotation(const Matrix33<Type>& mat)
  {
    setXAxis(mat.getXAxis());
    setYAxis(mat.getYAxis());
    setZAxis(mat.getZAxis());
  }


  template <class Type>
  void Matrix44<Type>::setRotation(const Quaternion<Type>& quat)
  {
    assert(quat.length() - static_cast<Type>(1) <= EPSILON);
    setRotation(quat.getAsMatrix33());
  }


  template <class Type>
  void Matrix44<Type>::transpose()
  {
    std::swap(this->yx, this->xy);
    std::swap(this->zx, this->xz);
    std::swap(this->wx, this->xw);
    std::swap(this->zy, this->yz);
    std::swap(this->wy, this->yw);
    std::swap(this->wz, this->zw);
  }


  template <class Type>
  void Matrix44<Type>::transposeRot()
  {
    std::swap(this->yx, this->xy);
    std::swap(this->zx, this->xz);
    std::swap(this->zy, this->yz);
  }


  template <class Type>
  void Matrix44<Type>::invert()
  {
    *this = boost::qvm::inverse(*this);
  }  


  template <class Type>
  void Matrix44<Type>::makeHomogeneous(const EAxis& axisRemoved)
  {
    switch (axisRemoved)
    {
      case XAXIS:
        getZAxis().normalize();
        setXAxis((getYAxis() % getZAxis()).getNormalized());
        setYAxis(getZAxis() % getXAxis());
        break;

      case YAXIS:
        getXAxis().normalize();
        setYAxis((getZAxis() % getXAxis()).getNormalized());
        setZAxis(getXAxis() % getYAxis());
        break;

      case ZAXIS:
        getYAxis().normalize();
        setZAxis((getXAxis() % getYAxis()).getNormalized());
        setXAxis(getYAxis() % getZAxis());
        break;
    }

    this->wx = 0.0;
    this->wy = 0.0;
    this->wz = 0.0;
    this->ww = 1.0;
  }


  template <class Type>
  const Matrix44<Type> operator * (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs)
  {
    return Matrix44<Type>(lhs) *= rhs;
  }


  template <class Type>
  const Vector3<Type> operator * (const Matrix44<Type>& lhs, const Vector3<Type>& rhs)
  {
    return boost::qvm::transform_point(lhs, rhs);
  }


  template <class Type>
  const Vector4<Type> operator * (const Matrix44<Type>& lhs, const Vector4<Type>& rhs)
  {
    return boost::qvm::operator*(lhs, rhs);
  }


  template <class Type>
  bool equals(const Matrix44<Type>& lhs, const Matrix44<Type>& rhs, double fEpsilon)
  {
    if (&lhs == &rhs)
      return true;
    return boost::qvm::cmp(lhs, rhs, [fEpsilon](const Type& lc, const Type& rc) { return fabs(lc - rc) < fEpsilon; });
  }


  template <class Type>
  inline bool operator == (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs)
  {
    return equals(lhs, rhs, EPSILON);
  }

  
  template <class Type>
  inline bool operator != (const Matrix44<Type>& lhs, const Matrix44<Type>& rhs)
  {
    return !(lhs == rhs);
  }


  template <class Type>
  std::ostream& operator << (std::ostream& stream, const Matrix44<Type>& mat)
  {
    return stream << mat.xx << " " << mat.xy << " " << mat.xz << " " << mat.xw << " "
                  << mat.yx << " " << mat.yy << " " << mat.yz << " " << mat.yw << " "
                  << mat.zx << " " << mat.zy << " " << mat.zz << " " << mat.zw << " "
                  << mat.wx << " " << mat.wy << " " << mat.wz << " " << mat.ww;
  }

  
  template <class Type>
  std::istream& operator >> (std::istream& stream, Matrix44<Type>& mat)
  {
    return stream >> mat.xx >> mat.xy >> mat.xz >> mat.xw
                  >> mat.yx >> mat.yy >> mat.yz >> mat.yw
                  >> mat.zx >> mat.zy >> mat.zz >> mat.zw
                  >> mat.wx >> mat.wy >> mat.wz >> mat.ww;
  }


}  // namespace ARSTD
