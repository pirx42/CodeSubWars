// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDynSolCol.h"
#include "CSWIDamageable.h"

namespace CodeSubWars
{

  class CSWDamageable;

  /** 
   * This is a base class for some common objects. It simple implements some often used combinations of property interfaces.
   */
  //@todo -Give it a better name.
  class CSWDamDynSolCol : public CSWDynSolCol,
                          public CSWIDamageable
  {
    public:
      typedef std::shared_ptr<CSWDamDynSolCol> PtrType;

      virtual ~CSWDamDynSolCol();

      //defined methods for damageable
      virtual void addDamage(const double fDamage);
      virtual void addHealth(const double fHealth);
      virtual void setDead(bool bValue);

      virtual const double& getHealth() const;
      virtual bool isAlive() const;
      virtual bool isDisabled() const;
      virtual bool isZombie() const;
      virtual bool isDead() const;

    protected:
      CSWDamDynSolCol(const std::string& strName, const Matrix44D& matBaseTObject, 
                      Mesh::PtrType pSurface, bool bSetAutomaticallyToDead);
    
      //properties
      std::shared_ptr<CSWDamageable> m_pDamageable;
  };

}