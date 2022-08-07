// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "CSWEquipment.h"
#include "CSWIRechargeable.h"

namespace CodeSubWars
{

  class CSWRechargeable;
  class CSWWeapon;
  class CSWWeaponBatteryVisualizer;


  //struct WeaponLess : public std::binary_function<std::shared_ptr<CSWWeapon>, std::shared_ptr<CSWWeapon>, bool>
  //{
  //  bool operator()(std::shared_ptr<CSWWeapon> lhs, std::shared_ptr<CSWWeapon> rhs) const;
  //};


  /**
   * This class provides functionality to store, launch and recharge weapons. Weapons can only launched via WeaponBatteries. At least 
   * one second should be waited between two weapon releases. Otherwise the release failed and no weapon is released.
   * When firing a weapon through a weapon battery the following things will happen:\n
   *   -# the weapon will be armed (only when used releaseNext()),\n
   *   -# the weapon will be launched/released and \n
   *   -# the weapon will explode on a defined trigger.\n
   */
  class CSWWeaponBattery : public CSWEquipment,
                           public CSWIRechargeable
  {
    public:
      typedef std::shared_ptr<CSWWeaponBattery> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, int nMaxSize, const unsigned long& nAcceptedResources);

      virtual ~CSWWeaponBattery();

      //defined methods from ARSTD::Element
      virtual void rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings = true);

      //defined methods from CSWEquipment
      virtual QWidget* getInformationWidget(QWidget* pParent);

      //defined methods from CSWIRechargeable

      /**
       * Signals if the recharging is currently running.
       * @return The recharging resource ids if recharging is currently running otherwise 0.
       */
      virtual const unsigned long& getRechargingIDs() const;

      /**
       * Returns the current fill level in range [0, 1].
       * @return The current fill level in range [0, 1].
       */
      virtual double getFillLevel() const;
      virtual bool recharge(const unsigned long& nResourceIDs);
      virtual const unsigned long& getAcceptedResourceIDs();

      /**
       * Tries to recharge itself by the given resource provider.
       * @param strResourceProvider The name of the resource provider on which the object should try to recharge.
       * @param nResourceIDs The bitwise ORed resources that should be recharged by the requested provider. This should be
       *                     a subset of the resources that this rechargeable object could contain.
       * @return Returns the resource ids that will be recharged. Not 0 is returned on successfully applying for recharging on the given 
       *         provider for at least one requested resource otherwise 0.
       */
      virtual const unsigned long& tryRecharging(const std::string& strResourceProvider, const unsigned long& nResourceIDs);
      virtual void finalizeRecharge();


      bool insert(std::shared_ptr<CSWWeapon> pWeapon);

      /**
       * Launches a given weapon. If no weapon is given nothing will be done.
       * @param pWeapon The weapon that should be launched.
       * @return True on success
       * @warning The weapon _must_ be arm before launch. Otherwise the weapon will _never_ fire.
       */
      bool release(std::shared_ptr<CSWWeapon> pWeapon);
    
      /**
       * Arms and launches the next available weapon. If no weapon is available nothing will be done.
       * @param fArmDelay The time in seconds when the weapon should be armed.
       * @return True on success
       */
      bool releaseNext(double fArmDelay = 5.0);

      /**
       * Returns the weapon that would be launched next. 
       * @return The weapon that would be released next time. If no weapon is available None will returned.
       */
      std::shared_ptr<CSWWeapon> getNext() const;

      /**
       * Returns the direction in world coordinate system in which the weapon are launched.
       * @return The direction in world coordinate systen in which the weapon are launched.
       */
      const Vector3D& getDirection() const;

      //in local coord system (relative to its parent)
      //The maximal angle for adjusting is 30 degree.
      //The maximal angular velocity for adjusting is 11.25 degree/second.
      void setDirection(const Vector3D& vecDirection);
    
      /**
       * Returns how many weapon can contain the weapon battery.
       * @return The capacity of the weapon battery.
       */
      int getSize() const; 

      /**
       * Returns the current number of available weapons.
       * @return The number of currently inserted weapons.
       */
      int getNumInserted() const;

      /**
       * Returns true if the battery is full.
       * @return True if full.
       */
      bool isFull() const;
    
      /**
       * Returns true if the battery is empty.
       * @return True if empty.
       */
      bool isEmpty() const;
    

      friend CSWWeaponBatteryVisualizer;
    protected:
      //due to problems when finding weapons that are given through python a self defined less operator is used.
      typedef std::set<std::shared_ptr<CSWWeapon>/*, WeaponLess*/> WeaponContainer;

      CSWWeaponBattery(const std::string& strName, const Matrix44D& matBaseTObject, int nMaxSize, const unsigned long& nAcceptedResources);

      //nothing can be attached to this object
      bool attach(ARSTD::Element::PtrType pElement);
      bool detach(ARSTD::Element::PtrType pElement);
  
      //property implementations
      std::shared_ptr<CSWRechargeable> m_pRechargeable;
  
      static const double MAX_ANGLE; //in rad
      static const double ANGLULAR_VELOCITY; //in rad/s
    
      WeaponContainer m_WeaponContainer;
      int m_nMaxSize;
      double m_fLeastWeaponLaunchDelay; //the minimum time that must be between two weapon launches
      double m_fLastWeaponLaunched;

      double m_fLastRechargeTime;
      int m_Cnt;
    
      CSWWeaponBatteryVisualizer* m_pVisualizer;
  };
}