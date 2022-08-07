// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWControlCenter.h"

#include "CSWTransceiver.h"
#include "CSWMap.h"

#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWActiveSonar.h"
#include "CSWPassiveSonar.h"
#include "CSWGPS.h"



namespace CodeSubWars
{

  CSWControlCenter::PtrType CSWControlCenter::create(const std::string& strName, const Matrix44D& matBaseTObject)
  {
    return PtrType(new CSWControlCenter(strName, matBaseTObject));
  }


  CSWControlCenter::~CSWControlCenter()
  {
  }


  CSWGPS::PtrType CSWControlCenter::getGPS()
  {
    return m_pGPS;
  }


  CSWGyroCompass::PtrType CSWControlCenter::getGyroCompass()
  {
    return m_pGyroCompass;
  }
 
   
  CSWMovingPropertiesSensor::PtrType CSWControlCenter::getMovingPropertiesSensor()
  {
    return m_pMovingPropertiesSensor;
  }


  CSWActiveSonar::PtrType CSWControlCenter::getActiveSonar()
  {
    return m_pActiveSonar;
  }


  CSWPassiveSonar::PtrType CSWControlCenter::getPassiveSonar()
  {
    return m_pPassiveSonar;
  }


  CSWTransceiver::PtrType CSWControlCenter::getTransceiver()
  {
    return m_pTransceiver;
  }


  CSWMap::PtrType CSWControlCenter::getMap()
  {
    return m_pMap;
  }


  CSWControlCenter::CSWControlCenter(const std::string& strName, const Matrix44D& matBaseTObject)
  : CSWEquipment(strName, matBaseTObject, 0, 0),
    m_pActiveSonar(CSWActiveSonar::create(strName + ".ActiveSonar", Matrix44D())),
    m_pPassiveSonar(CSWPassiveSonar::create(strName + ".PassiveSonar", Matrix44D())),
    m_pGyroCompass(CSWGyroCompass::create(strName + ".GyroCompass")),
    m_pMovingPropertiesSensor(CSWMovingPropertiesSensor::create(strName + ".MovingPropertiesSensor")),
    m_pGPS(CSWGPS::create(strName + ".GPS")),
    m_pTransceiver(CSWTransceiver::create(strName + ".Transceiver")),
    m_pMap(CSWMap::create(strName + ".Map"))
  {
    attach(m_pGyroCompass);
    attach(m_pMovingPropertiesSensor);
    attach(m_pActiveSonar);
    attach(m_pPassiveSonar);
    attach(m_pGPS);
    attach(m_pTransceiver);
    attach(m_pMap);
  
    m_pActiveSonar->setMap(m_pMap);
    m_pPassiveSonar->setMap(m_pMap);
  }

}