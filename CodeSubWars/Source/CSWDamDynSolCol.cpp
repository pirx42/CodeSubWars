// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "CSWDamDynSolCol.h"
#include "CSWDamageable.h"


namespace CodeSubWars
{

  CSWDamDynSolCol::~CSWDamDynSolCol()
  {
  }


  void CSWDamDynSolCol::addDamage(const double fDamage)
  {
    m_pDamageable->addDamage(fDamage);
  }


  void CSWDamDynSolCol::addHealth(const double fHealth)
  {
    m_pDamageable->addHealth(fHealth);
  }


  void CSWDamDynSolCol::setDead(bool bValue)
  {
    m_pDamageable->setDead(bValue);
  }


  const double& CSWDamDynSolCol::getHealth() const
  {
    return m_pDamageable->getHealth();
  }


  bool CSWDamDynSolCol::isAlive() const
  {
    return m_pDamageable->isAlive();
  }


  bool CSWDamDynSolCol::isDisabled() const
  {
    return m_pDamageable->isDisabled();
  }


  bool CSWDamDynSolCol::isZombie() const
  {
    return m_pDamageable->isZombie();
  }


  bool CSWDamDynSolCol::isDead() const
  {
    return m_pDamageable->isDead();
  }


  CSWDamDynSolCol::CSWDamDynSolCol(const std::string& strName, const Matrix44D& matBaseTObject, 
                                   Mesh::PtrType pSurface, bool bSetAutomaticallyToDead)
  : CSWDynSolCol(strName, matBaseTObject, pSurface),
    m_pDamageable(CSWDamageable::create(bSetAutomaticallyToDead))
  {
  }

}