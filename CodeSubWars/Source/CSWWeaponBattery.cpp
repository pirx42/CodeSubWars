// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWeaponBattery.h"
#include "CSWRechargeable.h"
#include "CSWWorld.h"
#include "CSWWeapon.h"
#include "CSWSubmarine.h"
#include "CSWMessageCollisionObjects.h"
#include "CSWIResourceProvider.h"
#include "CSWWeaponBatteryVisualizer.h"
#include "CSWWeaponSupply.h"
#include "CSWBattleStatistics.h"
//#include "Constants.h"
//#include "CSWEngine.h"
//#include "CSWPySubmarine.h"
#include "CSWSubmarine.h"

#include "GreenTorpedo.h"
#include "RedTorpedo.h"
#include "BlueTorpedo.h"
#include "GreenMine.h"
#include "YellowMine.h"

namespace CodeSubWars
{

  const double CSWWeaponBattery::MAX_ANGLE = getInRad(30);
  const double CSWWeaponBattery::ANGLULAR_VELOCITY = getInRad(11.25);


  //bool WeaponLess::operator()(std::shared_ptr<CSWWeapon> lhs, std::shared_ptr<CSWWeapon> rhs) const
  //{
  //  return lhs.get() < rhs.get();
  //}


  CSWWeaponBattery::PtrType CSWWeaponBattery::create(const std::string& strName, const Matrix44D& matBaseTObject, 
                                                     int nMaxSize, const unsigned long& nAcceptedResources)
  {
    return PtrType(new CSWWeaponBattery(strName, matBaseTObject, nMaxSize, nAcceptedResources));
  }


  CSWWeaponBattery::~CSWWeaponBattery()
  {
  }


  void CSWWeaponBattery::rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings)
  {
    CSWObject::rename(strOldName, strNewName, bReplaceSubstrings);

    WeaponContainer::const_iterator it = m_WeaponContainer.begin();
    for (; it != m_WeaponContainer.end(); ++it)
    {
      (*it)->rename(strOldName, strNewName, bReplaceSubstrings);
    }
  }


  QWidget* CSWWeaponBattery::getInformationWidget(QWidget* pParent)
  {
    if (!m_pVisualizer)
      m_pVisualizer = new CSWWeaponBatteryVisualizer(pParent, std::dynamic_pointer_cast<CSWWeaponBattery>(getSharedThis()));
    return m_pVisualizer;
  }


  const unsigned long& CSWWeaponBattery::getRechargingIDs() const
  {
    return m_pRechargeable->getRechargingIDs();
  }


  double CSWWeaponBattery::getFillLevel() const
  {
    return static_cast<double>(getNumInserted())/m_nMaxSize;
  }


  bool CSWWeaponBattery::recharge(const unsigned long& nResourceIDs)
  {
    double fCurrentTime = ARSTD::Time::getTime();
    if (fCurrentTime - m_fLastRechargeTime < 3)
      return isFull();
    m_fLastRechargeTime = fCurrentTime;
  
    if (nResourceIDs & CSWWeaponSupply::GREEN_TORPEDO_ID) 
    {
      insert(GreenTorpedo::create(getName() + ".GreenTorpedoR" + ARSTD::Utilities::toString(m_Cnt), Matrix44D(), 5));
      ++m_Cnt;
    }
    if (nResourceIDs & CSWWeaponSupply::RED_TORPEDO_ID)
    {
      insert(RedTorpedo::create(getName() + ".RedTorpedoR" + ARSTD::Utilities::toString(m_Cnt), Matrix44D(), 5));
      ++m_Cnt;
    }
    if (nResourceIDs & CSWWeaponSupply::BLUE_TORPEDO_ID)
    {
      insert(BlueTorpedo::create(getName() + ".BlueTorpedoR" + ARSTD::Utilities::toString(m_Cnt), Matrix44D(), 5));
      ++m_Cnt;
    }
    if (nResourceIDs & CSWWeaponSupply::GREEN_MINE_ID) 
    {
      insert(GreenMine::create(getName() + ".GreenMineR" + ARSTD::Utilities::toString(m_Cnt), Matrix44D(), 1));
      ++m_Cnt;
    }
    if (nResourceIDs & CSWWeaponSupply::YELLOW_MINE_ID) 
    {
      insert(YellowMine::create(getName() + ".YellowMineR" + ARSTD::Utilities::toString(m_Cnt), Matrix44D(), 1));
      ++m_Cnt;
    }
  
    return isFull();
  }


  const unsigned long& CSWWeaponBattery::getAcceptedResourceIDs()
  {
    return m_pRechargeable->getAcceptedResourceIDs();
  }


  const unsigned long& CSWWeaponBattery::tryRecharging(const std::string& strResourceProvider, const unsigned long& nResourceIDs)
  {
    if (isFull()) 
    {
      m_pRechargeable->finalizeRecharge();
      return m_pRechargeable->getRechargingIDs();
    }
    return m_pRechargeable->tryRecharging(getSharedThis(), nResourceIDs, strResourceProvider);
  }


  void CSWWeaponBattery::finalizeRecharge()
  {
    m_pRechargeable->finalizeRecharge();
  }


  bool CSWWeaponBattery::insert(CSWWeapon::PtrType pWeapon)
  {
    if (isFull())
      return false;
  
    return m_WeaponContainer.insert(pWeapon).second;
  }


  bool CSWWeaponBattery::release(CSWWeapon::PtrType pWeapon/*, double fLaunchDelay*/)
  {
    if (ARSTD::Time::getTime() - m_fLastWeaponLaunched < m_fLeastWeaponLaunchDelay)
      return false;

    WeaponContainer::iterator itFoundWeapon = m_WeaponContainer.find(pWeapon);
    if (itFoundWeapon == m_WeaponContainer.end())
      return false;
    m_WeaponContainer.erase(itFoundWeapon);

    Vector3D vecWorldTVelocity(0, 0, 0);
    Vector3D vecWorldTAngularMomentum(0, 0, 0);
    CSWIDynamic::PtrType pDynObj = std::dynamic_pointer_cast<CSWIDynamic>(findFirstParent<CSWIDynamic>());
    if (pDynObj)
    {
      //determine velocity at position of weapon battery
      vecWorldTVelocity = pDynObj->getVelocity() + pDynObj->getAngularVelocity(getTransform().getTranslation());
  //    vecWorldTAngularMomentum = pDynObj->getAngularMomentum();
    }

    pWeapon->initializeDynamic(getWorldTransform(), vecWorldTVelocity, vecWorldTAngularMomentum);

    CSWObject::PtrType pSolObj = findFirstParent<CSWISolid>();
    if (!pSolObj)
      throw std::runtime_error("weapon was released by a not solid object");
    pWeapon->setShooterName(pSolObj->getName());
    pWeapon->setShooterSubmarine(findFirstParent<CSWSubmarine>() != nullptr);
    CSWWorld::getInstance()->getBattleStatistics()->reportWeaponFired(pSolObj);

    if (auto pySub = std::dynamic_pointer_cast<CSWIPythonable>(findFirstParent<CSWIPythonable>()))
      pWeapon->setPythonContext(pySub->getPythonContext());

    bool bResult = true;
    bResult &= CSWMessageInitializeCollisionObjects::initializeCollision(pWeapon);
    if (!bResult)
      return bResult;
    bResult &= CSWWorld::getInstance()->getObjectTree()->attach(pWeapon);
    //bResult &= pSolObj->attach(pWeapon);
    if (!bResult)
      return bResult;
    bResult &= pWeapon->launch(0);//fLaunchDelay);
    if (!bResult)
      return bResult;

    m_fLastWeaponLaunched = ARSTD::Time::getTime();
    return bResult;
  }


  bool CSWWeaponBattery::releaseNext(double fArmDelay)
  {
    CSWWeapon::PtrType pWeapon = getNext();
    if (!pWeapon)
      return false;

    if (!pWeapon->arm(fArmDelay))
      return false;
    return release(pWeapon);
  }


  CSWWeapon::PtrType CSWWeaponBattery::getNext() const
  {
    if (isEmpty())
      return CSWWeapon::PtrType();
    //all elements in weapon container must be weapons!
    CSWWeapon::PtrType pNextWeapon = *(m_WeaponContainer.begin());
    if (!pNextWeapon)
      throw std::runtime_error("non empty weapon container should contain weapons");
    return pNextWeapon;
  }


  const Vector3D& CSWWeaponBattery::getDirection() const
  {
    return getWorldTransform().getZAxis();
  }


  void CSWWeaponBattery::setDirection(const Vector3D& vecDirection)
  {
    setRelativeDirection(vecDirection);
  }


  int CSWWeaponBattery::getSize() const
  {
    return m_nMaxSize;
  }


  int CSWWeaponBattery::getNumInserted() const
  {
    return static_cast<int>(m_WeaponContainer.size());
  }


  bool CSWWeaponBattery::isFull() const
  {
    return m_WeaponContainer.size() >= m_nMaxSize;
  }

    
  bool CSWWeaponBattery::isEmpty() const
  {
    return m_WeaponContainer.empty();
  }


  CSWWeaponBattery::CSWWeaponBattery(const std::string& strName, const Matrix44D& matBaseTObject, 
                                     int nMaxSize, const unsigned long& nAcceptedResources)
  : CSWEquipment(strName, matBaseTObject, MAX_ANGLE, ANGLULAR_VELOCITY),
    m_pRechargeable(CSWRechargeable::create(nAcceptedResources)),
    m_nMaxSize(nMaxSize),
    m_fLastWeaponLaunched(-1e+10),
    m_fLeastWeaponLaunchDelay(1.0),
    m_fLastRechargeTime(0),
    m_Cnt(0),
    m_pVisualizer(NULL)
  {
  }

}