// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWRock.h"
#include "CSWIUpdateable.h"

namespace CodeSubWars
{

  class CSWPassiveSonar;
  class CSWWeaponBattery;

  /** 
   * This class represents a rock (or asteroid), that fires \link GreenTorpedo green torpedos\endlink against detected enemies. The detection 
   * bases on \link CSWPassiveSonar PassiveSonar\endlink.
   */
  class CSWActiveRock : public CSWRock,
                        public CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWActiveRock> PtrType;

      virtual ~CSWActiveRock();

      //defined methods for object
      virtual void initialize();

      //defined methods for updateable
      virtual void update();
  
      //for debugging stuff
      virtual void draw();
    
    protected:
      CSWActiveRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      std::shared_ptr<CSWPassiveSonar> getPassiveSonar();
      std::shared_ptr<CSWWeaponBattery> getWeaponBattery();


      enum State
      {
        INITIALIZING = 1,
        SONAR_IDLE,
        SONAR_ADJUSTING,
        BATTERY_IDLE,
        BATTERY_ADJUSTING
      };
  
      std::shared_ptr<CSWPassiveSonar> m_pPassiveSonar;

      std::shared_ptr<CSWWeaponBattery> m_pWeaponBattery;

      //other stuff
      State m_CurrentState;
      double m_fLastWeaponLaunched;
  };

}