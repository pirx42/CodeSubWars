// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "CSWEquipment.h"

namespace CodeSubWars
{

  class CSWTransceiver;
  class CSWMap;
  class CSWGyroCompass;
  class CSWMovingPropertiesSensor;
  class CSWSonar;
  class CSWActiveSonar;
  class CSWPassiveSonar;
  class CSWGPS;


  /**
   * This class provides access to some equipment.
   */
  class CSWControlCenter : public CSWEquipment
  {
    public:
      typedef std::shared_ptr<CSWControlCenter> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject);

      virtual ~CSWControlCenter();

      /**
       * Returns the GPS.
       */
      std::shared_ptr<CSWGPS> getGPS();

      /**
       * Returns the GyroCompass.
       */
      std::shared_ptr<CSWGyroCompass> getGyroCompass();

      /**
       * Returns the MovingProptertiesSensor.
       */
      std::shared_ptr<CSWMovingPropertiesSensor> getMovingPropertiesSensor();

      /**
       * Returns the ActiveSonar.
       */
      std::shared_ptr<CSWActiveSonar> getActiveSonar();

      /**
       * Returns the PassiveSonar.
       */
      std::shared_ptr<CSWPassiveSonar> getPassiveSonar();

      /**
       * Returns the Transceiver.
       */
      std::shared_ptr<CSWTransceiver> getTransceiver();

      /**
       * Returns the Map.
       */
      std::shared_ptr<CSWMap> getMap();

    protected:
      CSWControlCenter(const std::string& strName, const Matrix44D& matBaseTObject);

      //sensors
      std::shared_ptr<CSWGyroCompass> m_pGyroCompass;
      std::shared_ptr<CSWMovingPropertiesSensor> m_pMovingPropertiesSensor;
      std::shared_ptr<CSWActiveSonar> m_pActiveSonar;  //intial autorotating sonar
      std::shared_ptr<CSWPassiveSonar> m_pPassiveSonar;
      std::shared_ptr<CSWGPS> m_pGPS;

      //other stuff
      std::shared_ptr<CSWTransceiver> m_pTransceiver;
      std::shared_ptr<CSWMap> m_pMap;
        
  };

}