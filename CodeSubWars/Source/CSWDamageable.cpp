// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWDamageable.h"
#include "CSWWorld.h"
#include "CSWSettings.h"


namespace CodeSubWars
{

  CSWDamageable::PtrType CSWDamageable::create(bool bSetAutomaticallyToDead)
  {
    return PtrType(new CSWDamageable(bSetAutomaticallyToDead));
  }


  CSWDamageable::~CSWDamageable()
  {
  }


  void CSWDamageable::addDamage(const double fDamage)
  {
    if (fDamage < 0)
      return;
    m_fHealth -= fDamage;
    if (m_bSetAutomaticallyToDead && !isAlive())
      m_bDead = true;
  }


  void CSWDamageable::addHealth(const double fHealth)
  {
    if (fHealth < 0)
      return;
    m_fHealth += fHealth;
  }


  void CSWDamageable::setDead(bool bValue)
  {
    m_bDead = bValue;
  }


  const double& CSWDamageable::getHealth() const
  {
    static double zero = 0;
    return m_bDead ? zero : m_fHealth;
  }


  bool CSWDamageable::isAlive() const
  {
    return getHealth() > 0;
  }


  bool CSWDamageable::isDisabled() const
  {
    return isAlive() && getHealth() < 0.1;
  }


  bool CSWDamageable::isZombie() const
  {
    return !isDead() && !isAlive();
  }


  bool CSWDamageable::isDead() const
  {
    return m_bDead;
  }


  CSWDamageable::CSWDamageable(bool bSetAutomaticallyToDead)
  : m_bSetAutomaticallyToDead(bSetAutomaticallyToDead),
    m_fHealth(1),
    m_bDead(false)
  {
  }

}