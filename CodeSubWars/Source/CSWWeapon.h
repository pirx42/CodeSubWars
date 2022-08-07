// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDamDynSolCol.h"
#include "CSWIImpulsEmitter.h"
#include "CSWISoundEmitter.h"
#include "CSWIPythonable.h"

namespace CodeSubWars
{

  class CSWEvent;
  class PythonContext;

  /**
   * This is the base class for all weapons within the world. A weapon is a thing that can causes damage to other damagable objects.
   * The damage height to other objects is depend on its size. Larger weapon causes higher damage.
   * This has the property to emit sound on its explosion that is depend on the explosion power. 
   * @warning Weapons can only be launched through a weaponbattery! Otherwise the object will not inserted into the object tree.
   */
  class CSWWeapon : public CSWDamDynSolCol,
                    public CSWIImpulsEmitter,
                    public CSWISoundEmitter,
                    public CSWIPythonable
  {
    public:
      typedef std::shared_ptr<CSWWeapon> PtrType;

      virtual ~CSWWeapon();

      //defined methods from IImpulsEmitter
      virtual bool isImpulsEmitterActive() const;
      virtual void initializeImpulsEmission();
      virtual void emitImpuls(std::shared_ptr<CSWObject> pObject);
      virtual void finalizeImpulsEmission();

      //defined methods from ISoundEmitter
      virtual bool isSoundEmitterActive() const;
      virtual void initializeSoundEmission();
      virtual void emitSound(std::shared_ptr<CSWObject> pObject);
      virtual void finalizeSoundEmission();

      //defined methods for collideable
      virtual void initializeEnvironment(bool bSimplified);

      //relative delay in s
      virtual bool launch(double fLaunchDelay);
    
      virtual std::shared_ptr<PythonContext> getPythonContext()
      {
        return m_pPythonContext;
      }

      virtual void setPythonContext(std::shared_ptr<PythonContext> context)
      {
        m_pPythonContext = context;
      }

      /**
       * Sets the relative time from now when the weapon will be armed. Only if a weapon is armed 
       * it could explode. This is only available if the weapon was not launched.
       * @param fArmDelay The time from now in seconds when the weapon is armed.
       * @return True on success.
       * @warning If the absolute time for fire is earlier than the absolute arm time, the fire time 
       * will be set to arm time.
       */
      virtual bool arm(double fArmDelay);

      /**
       * Sets the relative time from now when the weapon will be fired. This time should be larger 
       * than the arm delay. This is only available if the weapon was not launched.
       * @param fFireDelay The time from now in seconds when the weapon is fired. 
       * @return True on success.
       * @warning If the absolute time for fire is earlier than the absolute arm time, the arm time 
       * will be set to fire time.
       */
      virtual bool fire(double fFireDelay);

      virtual bool isLaunched() const;
      virtual bool isArmed() const;
      virtual bool isFired() const;

      void setShooterName(const std::string& strName);
      const std::string& getShooterName() const;
    
      bool isShooterSubmarine() const;
      void setShooterSubmarine(bool bVal);

    protected:
      CSWWeapon(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      //let the weapon firing immediately.
      void fireImmediately(bool bIgnoreArmTime = false);

      double getAbsLaunchTime() const;
      double getAbsArmTime() const;
      double getAbsFireTime() const;
    
      //other stuff
      std::string m_strShooterName;    
      bool m_bShooterSubmarine;
      //the absolut time from which the weapon is launched
      double m_fAbsLaunchTime;
      //the absolut time from which the weapon is armed
      double m_fAbsArmTime;
      //the absolut time from which the weapon is ready to be fired.
      double m_fAbsFireTime;
      double m_fRadius;
      double m_fPower;
      double m_fMaxDamage;

      std::shared_ptr<PythonContext> m_pPythonContext;
  };

}