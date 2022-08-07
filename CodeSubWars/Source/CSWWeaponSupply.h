// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDynSolCol.h"
#include "CSWIUpdateable.h"
#include "CSWIResourceProvider.h"

namespace CodeSubWars
{

  class CSWResourceProvider;
  class CSWIRechargeable;

  /** 
   * This class represents a resource provider for weapons. Rechargeable objects can request recharging of specific weapons 
   * to this object. The instance of this class broadcasts text messages periodicaly every 5 seconds. These messages contain
   * the name, the position, the radius within objects must be for recharging and the bitwise ORed provided resource ids.\n
   * The message format is: "<WeaponSupplyName> position: <x> <y> <z> radius: <r> resources: <i>"
   */
  //@todo this behaves similar to a factory for weapon/ammo -> try to implement like a factory where supported weapons/ammo can be 
  //      registered. but this is no static factory. there can independently exists instances that support different weapons/ammo.
  class CSWWeaponSupply : public CSWDynSolCol,
                          public CSWIUpdateable,
                          public CSWIResourceProvider
  {
    public:
      typedef std::shared_ptr<CSWWeaponSupply> PtrType;

      /**
       * The resource of weapons.
       */
      enum WeaponResource
      {
        GREEN_TORPEDO_ID = 1,     ///< The resource id identifying green (ordinary) torpedos.
        RED_TORPEDO_ID = 2,       ///< The resource id identifying red (active sonar guided) torpedos.
        BLUE_TORPEDO_ID = 4,      ///< The resource id identifying blue (passive sonar guided) torpedos.
        GREEN_MINE_ID = 1024,     ///< The resource id identifying green (ordinary) mines.
        YELLOW_MINE_ID = 2048     ///< The resource id identifying yellow (magnetic) mines.
      };

      virtual ~CSWWeaponSupply();

      //defined methods from CSWIUpdateable
      virtual void update();

      //defined methods from CSWIResourceProvider
      virtual unsigned long requestRecharge(std::shared_ptr<CSWObject> pObjectToRecharge, const unsigned long& nResourceIDs);
      virtual void updateRecharging();

      virtual std::string getCurrentInfo() const;
    
      static std::string translate(int nResources);

    protected:
      CSWWeaponSupply(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
    
      //property implementations
      std::shared_ptr<CSWResourceProvider> m_pResourceProvider; 
  
      //other stuff
      double m_fLastSentTime;
  };

}