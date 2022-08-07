// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorld.h"
#include "CSWSettings.h"
#include "CSWActiveRock.h"
#include "CSWEvent.h"
#include "CSWCollisionDetectedMessage.h"

#include "CSWPassiveSonar.h"
#include "CSWWeaponBattery.h"
#include "CSWWeaponSupply.h"

#include "GreenTorpedo.h"


namespace CodeSubWars
{

  CSWActiveRock::~CSWActiveRock()
  {
  }


  void CSWActiveRock::initialize()
  {
    m_CurrentState = SONAR_IDLE;
  }


  void CSWActiveRock::update()
  {
    switch (m_CurrentState)
    {
      case INITIALIZING:
        //on itializing do nothing
        break;
      case SONAR_IDLE:
      {
        if (getPassiveSonar()->getLevel() > 70 && 
            m_fLastWeaponLaunched + 5 < ARSTD::Time::getTime())
        {
          getWeaponBattery()->setDirection(getInvertWorldTransform().getRotationAsMatrix33()*getPassiveSonar()->getDirection());
          m_CurrentState = BATTERY_ADJUSTING;
        }
        else
        {
          getPassiveSonar()->adjustDirectionToMaximum(3);
          m_CurrentState = SONAR_ADJUSTING;
        }  
        break;
      }
      case SONAR_ADJUSTING:
      {
        if (!getPassiveSonar()->isAdjusting())
          m_CurrentState = SONAR_IDLE;
        break;
      }
      case BATTERY_ADJUSTING:
      {
        if (!getWeaponBattery()->isMoving())
        { 
          bool bResult = getWeaponBattery()->releaseNext();
          if (bResult)
            m_fLastWeaponLaunched = ARSTD::Time::getTime();
          m_CurrentState = BATTERY_IDLE;
        }
        break;
      }
      case BATTERY_IDLE:
      {
        getPassiveSonar()->adjustDirectionToMaximum(3);
        m_CurrentState = SONAR_ADJUSTING;
        break;
      }
    }
  }


  void CSWActiveRock::draw()
  {
    CSWDamDynSolCol::draw();
    if (CSWWorld::getInstance()->getSettings()->getSelectedObject() == getSharedThis())
      getPassiveSonar()->draw();
  }


  CSWActiveRock::CSWActiveRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWRock(strName, matBaseTObject, size),
    m_pPassiveSonar(CSWPassiveSonar::create(strName + ".PassiveSonar", Matrix44D())),
    m_CurrentState(INITIALIZING),
    m_fLastWeaponLaunched(0)
  {
    attach(m_pPassiveSonar);
    m_pPassiveSonar->setMaxAngle(getInRad(90));
    m_pPassiveSonar->setEnableAutomaticRotation(false);
    m_pPassiveSonar->setAngleOfBeam(45);

    //add a little offset so that the battery is out of the collision surface. this simplifies the launching of weapons
    m_pWeaponBattery = CSWWeaponBattery::create(strName + ".WeaponBattery", Matrix44D(Vector3D(0, 0, size.getDepth()/2.0 + 8)), 
                                                size.getDepth(), CSWWeaponSupply::GREEN_TORPEDO_ID);
    m_pWeaponBattery->setMaxAngle(getInRad(90));
    m_pWeaponBattery->setAngularVelocity(getInRad(5));
    attach(m_pWeaponBattery);

    //load the weapons
    int nCnt = 0;
    while (!getWeaponBattery()->isFull())
    {
      std::stringstream ss;
      ss << ++nCnt;
      getWeaponBattery()->insert(GreenTorpedo::create(strName + "GreenTorpedo" + ss.str(), Matrix44D(), 5));
    }
  
    setColor(Vector4D(0.7, 0.7, 1.0, 1));
  }


  CSWPassiveSonar::PtrType CSWActiveRock::getPassiveSonar()
  {
    return m_pPassiveSonar;
  }


  CSWWeaponBattery::PtrType CSWActiveRock::getWeaponBattery()
  {
    return m_pWeaponBattery;
  }

}