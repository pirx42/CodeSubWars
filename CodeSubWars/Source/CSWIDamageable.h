// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * Objects with this property can be damaged by collisions or explosions. The initial health is 1.0 (100%).
   * If the health is below zero the object is dead and will be removed from the world. 
   */
  class CSWIDamageable
  {
    public:
      typedef std::shared_ptr<CSWIDamageable> PtrType;
  
      virtual ~CSWIDamageable() {}

      virtual void addDamage(const double fDamage) = 0;
      virtual void addHealth(const double fHealth) = 0;

      virtual void setDead(bool bValue) = 0;

      //Returns the current health of the object.
      virtual const double& getHealth() const = 0;

      virtual bool isAlive() const = 0;
      virtual bool isDisabled() const = 0;
      virtual bool isZombie() const = 0;
      virtual bool isDead() const = 0;

  };


}