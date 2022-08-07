// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDamDynSolCol.h"
#include "CSWIEventDealable.h"
#include "CSWICommandable.h"
#include "CSWIUpdateable.h"
#include "CSWIImpulsEmitter.h"
#include "CSWISoundEmitter.h"


namespace CodeSubWars
{

  class CSWEventDealable;
  class CSWCommandable;

  class CSWCommands;

  class CSWEngine;
  class CSWControlCenter;
  class CSWWeaponBattery;


  /**
   * This is the base class for all submarines within the world.
   */
  class CSWSubmarine : public CSWDamDynSolCol,
                       public CSWIEventDealable,
                       public CSWICommandable,
                       public CSWIUpdateable,
                       public CSWIImpulsEmitter,
                       public CSWISoundEmitter
  {
    public:
      typedef std::shared_ptr<CSWSubmarine> PtrType;

      virtual ~CSWSubmarine();

      //defined methods for dynamic
      virtual void updatePosition();

      //defined methods for eventdealable
      virtual void receiveEvent(std::shared_ptr<CSWEvent> pEvent);
      virtual void processReceivedQueuedEvents();
      virtual std::string getCurrentInformation() const;
      virtual std::string getCurrentMessage() const;

      //This method must be overriden in the specialized class. This methode is called if an event has been received by the object that should be 
      //processed. Returning true reports the eventmanager that the event passed to the object has successfully processed and delete it.
      //When returning false the event will be delivered the next time again until it expires or processed successfully.
      //The main function should be reacting to events e.g. seting up commands or something like that. The specialized class can implement everything 
      //here, but the execution time must not exceed 6ms in average. If it does the object will be kicked from the world!
      //@param pEvent The incoming event that should be processed.
      //@return True if the incoming event has been successfully processed. When returning false the event will be delivered next time again.
      virtual bool processEvent(std::shared_ptr<CSWEvent> pEvent) { return true; }


      //defined methods for commandable
      virtual void step();
      virtual std::shared_ptr<ARSTD::CommandProcessor> getCommandProcessor();


      //defined methods for updateable

      //This method must be overriden in the specialized class. This methode is called periodically about every 10ms.
      //The main function should be checking sensors, seting up commands or something like that. The specialized class can implement everything 
      //here, but the execution time must not exceed 6ms in average. If it does the object will be kicked from the world!
      virtual void update();


      //defined methods for collideable
      virtual void initializeEnvironment(bool bSimplified);


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


      //defined methods for solid (draw debug stuff)
      virtual void draw();

      Vector3D makeLocalDirection(const Vector3D& vecWorldTDirection);
      Vector3D makeGlobalDirection(const Vector3D& vecObjectTDirection);
      Vector3D makeLocalPosition(const Vector3D& vecWorldTPosition);
      Vector3D makeGlobalPosition(const Vector3D& vecObjectTPosition);

      int getTeamID() const;
      void setTeamID(int nID);

      friend CSWCommands;

    protected:
      CSWSubmarine(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);
    
      std::shared_ptr<CSWEngine> getMainEngine();
      std::shared_ptr<CSWEngine> getBuoyancyTank();
      std::shared_ptr<CSWEngine> getBowsJetOar();
      std::shared_ptr<CSWEngine> getInclinationJetOar();
      std::shared_ptr<CSWEngine> getAxialInclinationJetOar();

      std::shared_ptr<CSWControlCenter> getControlCenter();

      std::shared_ptr<CSWWeaponBattery> getFrontLeftWeaponBattery();
      std::shared_ptr<CSWWeaponBattery> getFrontRightWeaponBattery();
      std::shared_ptr<CSWWeaponBattery> getBackWeaponBattery();

      static const double MAX_OAR_ANGLE; //in Degree
      static const int NUM_CYCLES;  
    
      //special properties
      std::shared_ptr<CSWEventDealable> m_pEventDealable;
      std::shared_ptr<CSWCommandable> m_pCommandable;

      //equipment
      std::shared_ptr<CSWEngine> m_pMainEngine;
      std::shared_ptr<CSWEngine> m_pBowsJetOar;  //bugstrahlruder
      std::shared_ptr<CSWEngine> m_pBuoyancyTank; //auf/abtriebstank
      std::shared_ptr<CSWEngine> m_pInclinationJetOar; //neigungsruder
      std::shared_ptr<CSWEngine> m_pAxialInclinationJetOar; //axialneigungsruder

      std::shared_ptr<CSWControlCenter> m_pControlCenter;
    
      std::shared_ptr<CSWWeaponBattery> m_pFrontLeftWeaponBattery;
      std::shared_ptr<CSWWeaponBattery> m_pFrontRightWeaponBattery;
      std::shared_ptr<CSWWeaponBattery> m_pBackWeaponBattery;
    
      boost::circular_buffer<double> m_Times;
  
      double m_fExplosionPower;
      double m_fExplosionRadius;
      double m_fMaxExplosionDamage;
    
      int m_nTeamID;
  };

}