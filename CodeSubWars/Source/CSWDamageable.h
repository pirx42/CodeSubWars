// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWDamageable
  {
    public:
      typedef std::shared_ptr<CSWDamageable> PtrType;

      //if bSetAutomaticallyToDead is true, the object will set to dead, if health is below 0
      //if bSetAutomaticallyToDead is false, the object will become zombie, if health is below 0 (it is not
      //alive and not dead) 
      static PtrType create(bool bSetAutomaticallyToDead);
      ~CSWDamageable();

      void addDamage(const double fDamage);
      void addHealth(const double fHealth);

      void setDead(bool bValue);

      const double& getHealth() const;

      //equiv. to health > 0    
      bool isAlive() const;
      //equiv. to 0 < health < 0.1    
      bool isDisabled() const;
      //equiv. to health <= 0 and not m_bDead
      bool isZombie() const;
      //equiv. to m_bDead
      bool isDead() const;

    protected:
      CSWDamageable(bool bSetAutomaticallyToDead);

      bool m_bSetAutomaticallyToDead;
      double m_fHealth;   //1 max health
      bool m_bDead;
    
  };


}