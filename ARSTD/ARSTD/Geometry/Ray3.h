// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "arstd/Math/Math.h"


namespace ARSTD
{

  template <class Type>
  class Ray3
  {
    public:
      Ray3(const Vector3<Type>& vecPos, const Vector3<Type>& vecDir);

      template<class Type2>
      explicit Ray3(const Ray3<Type2>& other)
      : m_vecPos(static_cast<Vector3<Type>>(other.getPos())),
        m_vecDir(static_cast<Vector3<Type>>(other.getDir()()))
      {
      }

      const Vector3<Type>& getPos() const;
      void setPos(const Vector3<Type>& vecPos);

      const Vector3<Type>& getDir() const;
      void setDir(const Vector3<Type>& vecDir);

      void set(const Vector3<Type>& vecPos, const Vector3<Type>& vecDir);

    protected:
      Vector3<Type> m_vecPos;
      Vector3<Type> m_vecDir;
  };


  typedef Ray3<double> Ray3D;


  template <class Type>
  Ray3<Type>::Ray3(const Vector3<Type>& vecPos, const Vector3<Type>& vecDir)
  : m_vecPos(vecPos),
    m_vecDir(vecDir)
  {
  }


  template <class Type>
  inline const Vector3<Type>& Ray3<Type>::getPos() const
  {
    return m_vecPos;
  }

  
  template <class Type>
  inline const Vector3<Type>& Ray3<Type>::getDir() const
  {
    return m_vecDir;
  }


  template <class Type>
  inline void Ray3<Type>::setPos(const Vector3<Type>& vecPos)
  {
    m_vecPos = vecPos;
  }


  template <class Type>
  inline void Ray3<Type>::setDir(const Vector3<Type>& vecDir)
  {
    m_vecDir = vecDir;
  }


  template <class Type>
  inline void Ray3<Type>::set(const Vector3<Type>& vecPos, const Vector3<Type>& vecDir)
  {
    m_vecPos = vecPos;
    m_vecDir = vecDir;
  }


}  // namespace ARSTD
