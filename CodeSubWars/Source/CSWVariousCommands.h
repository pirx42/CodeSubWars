// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWExecuteCommand.h"

namespace CodeSubWars
{

  class CSWEngine;
  class CSWEvent;
  class CSWWeapon;
  class CSWWeaponBattery;


  /**
   * This is a specialization of a command. It provides the functionality to set the intensity of an engine.
   */
  class CSWSetEngineIntensityCommand : public CSWExecuteCommand
  {
    public:
      typedef std::shared_ptr<CSWSetEngineIntensityCommand> PtrType;

      /**
       * Creates a new command that let the given engine change its intensity.
       * @param pEngine The engine which should change its intensity.
       * @param fIntensity The new intensity in range [-1, 1].
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWEngine>& pEngine, const double& fIntensity);

      virtual ARSTD::Command::PtrType copy() const;

      virtual std::string getDetails() const;

      virtual ~CSWSetEngineIntensityCommand();

    protected:
      CSWSetEngineIntensityCommand(std::shared_ptr<CSWEngine> pEngine, const double& fIntensity);
      CSWSetEngineIntensityCommand(const CSWSetEngineIntensityCommand& other);

      virtual void run();
    
      std::weak_ptr<CSWEngine> m_pEngine;
      double m_fIntensity;
  };





  /**
   * This is a specialization of a command. It provides the functionality to set the direction of an engine.
   */
  class CSWSetEngineDirectionCommand : public CSWExecuteCommand
  {
    public:
      typedef std::shared_ptr<CSWSetEngineDirectionCommand> PtrType;

      /**
       * Creates a new command that let the given engine change its direction.
       * @param pEngine The engine which should change its direction.
       * @param fHAngle The new horizontal angle. Positiv means rightward (in degree).
       * @param fVAngle The new vertical angle. Positiv means upward (in degree).
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWEngine>& pEngine, const double& fHAngle, const double& fVAngle);

      /**
       * Creates a new command that let the given engine change its direction.
       * @param pEngine The engine which should change its direction.
       * @param vecDirection The new direction relative to its parent.
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWEngine>& pEngine, const Vector3D& vecDirection);

      virtual ARSTD::Command::PtrType copy() const;

      virtual std::string getDetails() const;

      virtual ~CSWSetEngineDirectionCommand();

    protected:
      CSWSetEngineDirectionCommand(std::shared_ptr<CSWEngine> pEngine, const double& fHAngle, const double& fVAngle);
      CSWSetEngineDirectionCommand(std::shared_ptr<CSWEngine> pEngine, const Vector3D& vecDirection);
      CSWSetEngineDirectionCommand(const CSWSetEngineDirectionCommand& other);

      virtual void run();
    
      std::weak_ptr<CSWEngine> m_pEngine;
      bool m_bUseAngles;
      double m_fHAngle;
      double m_fVAngle;
      Vector3D m_vecDirection;
  };





  /**
   * This is a specialization of a command. It provides the functionality to send an event.
   */
  class CSWSendEventCommand : public CSWExecuteCommand
  {
    public:
      typedef std::shared_ptr<CSWSendEventCommand> PtrType;

      /**
       * Creates a new send event command. If no valid event is given an exception is thrown.
       * @param pEvent The event which should be sent. This must be not None.
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWEvent> pEvent);

      virtual ARSTD::Command::PtrType copy() const;

      virtual std::string getDetails() const;

      virtual ~CSWSendEventCommand();

    protected:
      CSWSendEventCommand(std::shared_ptr<CSWEvent> pEvent);
      CSWSendEventCommand(const CSWSendEventCommand& other);

      virtual void run();
    
      std::shared_ptr<CSWEvent> m_pEvent;
  };





  /**
   * This is a specialization of a command. It provides the functionality to launch a Weapon from a WeaponBattery.
   */
  class CSWFireCommand : public CSWExecuteCommand
  {
    public:
      typedef std::shared_ptr<CSWFireCommand> PtrType;

      /**
       * Creates a new fire command. If no Weapon is loaded to the WeaponBattery nothing will be done.
       * @param pWeaponBattery The WeaponBattery from where the Weapon should be launched.
       * @param fArmDelay The time in seconds when the weapon should be armed.
       * @param pWeapon The Weapon that will be launched. If the given Weapon is not found nothing will be done.
       *                If None is given the next available Weapon will be launched. (by default None)
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery, 
                            double fArmDelay = 5.0,
                            std::shared_ptr<CSWWeapon>& pWeapon = std::shared_ptr<CSWWeapon>());

      virtual ARSTD::Command::PtrType copy() const;

      virtual std::string getDetails() const;

      virtual ~CSWFireCommand();

    protected:
      CSWFireCommand(std::shared_ptr<CSWWeaponBattery> pWeaponBattery, 
                     double fArmDelay,
                     std::shared_ptr<CSWWeapon> pWeapon);
      CSWFireCommand(const CSWFireCommand& other);

      virtual void run();
    
      std::weak_ptr<CSWWeaponBattery> m_pWeaponBattery;
      double m_fArmDelay;
      std::weak_ptr<CSWWeapon> m_pWeapon;
  };





  /**
   * This is a specialization of a command. It provides the functionality to request recharge of a weapon battery at
   * a resource provider.
   */
  class CSWRechargeWeaponBatteryCommand : public CSWExecuteCommand
  {
    public:
      typedef std::shared_ptr<CSWRechargeWeaponBatteryCommand> PtrType;

      /**
       * Creates a new command that let the given WeaponBattery tries to recharge at the given supply.
       * @param pWeaponBattery The weapon battery which should be recharged.
       * @param strSupplyName The name of the object where the battery should try to be recharged.
       * @param nResourceIDs The ORed combination of resourceIDs that should be recharged at that supply.
       * @return Returns the new constructed command.
       */
      static PtrType create(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery, 
                            const std::string& strSupplyName, 
                            unsigned long nResourceIDs);

      virtual ARSTD::Command::PtrType copy() const;

      virtual std::string getDetails() const;

      virtual ~CSWRechargeWeaponBatteryCommand();

    protected:
      CSWRechargeWeaponBatteryCommand(std::shared_ptr<CSWWeaponBattery> pWeaponBattery, 
                                      const std::string& strSupplyName,
                                      unsigned long nResourceIDs);
      CSWRechargeWeaponBatteryCommand(const CSWRechargeWeaponBatteryCommand& other);

      virtual void run();
    
      std::weak_ptr<CSWWeaponBattery> m_pRechargeableWeaponBattery;
      std::string m_strSupplyName;
      unsigned long m_nResourceIDs;
  };

}