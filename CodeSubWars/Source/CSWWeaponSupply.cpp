// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWeaponSupply.h"
#include "CSWIRechargeable.h"
#include "CSWResourceProvider.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"
#include "CSWTextMessage.h"


namespace CodeSubWars
{

  CSWWeaponSupply::~CSWWeaponSupply()
  {
  }


  void CSWWeaponSupply::update()
  {
    double fCurrentTime = ARSTD::Time::getTime();
    //if this object is an environment object it can be happen that the last time is lager than the current. this is 
    //caused by REstarting a new battle. The world time is zeroed but the internally stored lastSentTime is not zeroed.
    //to prevent this problem: send also if lastSendTime is smaller than current time.
    if (fCurrentTime > m_fLastSentTime && 
        fCurrentTime - m_fLastSentTime < 5)
      return;
    m_fLastSentTime = fCurrentTime;
  
    std::stringstream ss;
    ss.precision(0);
    ss.setf(std::ios::fixed);
    ss << getName() << " position: " << getWorldTransform().getTranslation() << 
                       " radius: " << m_pResourceProvider->getMaxDistance() << 
                       " resources: " << m_pResourceProvider->getProvidedResourceIDs();
    CSWEventManager::getInstance()->send(CSWEvent::create(getSharedThis(), CSWObject::PtrType(), CSWTextMessage::create(ss.str()), 1));
  }


  unsigned long CSWWeaponSupply::requestRecharge(CSWObject::PtrType pObjectToRecharge, const unsigned long& nResourceIDs)
  {
    return m_pResourceProvider->requestRecharge(pObjectToRecharge, nResourceIDs, getWorldTransform().getTranslation());
  }


  void CSWWeaponSupply::updateRecharging()
  {
    m_pResourceProvider->updateRecharging(getWorldTransform().getTranslation());
  }


  std::string CSWWeaponSupply::getCurrentInfo() const
  {
    return m_pResourceProvider->getCurrentInfo(&CSWWeaponSupply::translate);  
  }


  std::string CSWWeaponSupply::translate(int nResources)
  {
    if (!nResources)
      return "None";

    std::string strResult;
    if (nResources & GREEN_TORPEDO_ID)
    {
      strResult += "GreenTorpedo";
      nResources &= ~GREEN_TORPEDO_ID;
    }
    if (nResources & RED_TORPEDO_ID) 
    {
      if (!strResult.empty())
        strResult += ", ";
      strResult += "RedTorpedo";
      nResources &= ~RED_TORPEDO_ID;
    }
    if (nResources & BLUE_TORPEDO_ID) 
    {
      if (!strResult.empty())
        strResult += ", ";
      strResult += "BlueTorpedo";
      nResources &= ~BLUE_TORPEDO_ID;
    }
    if (nResources & GREEN_MINE_ID) 
    {
      if (!strResult.empty())
        strResult += ", ";
      strResult += "GreenMine";
      nResources &= ~GREEN_MINE_ID;
    }
    if (nResources & YELLOW_MINE_ID) 
    {
      if (!strResult.empty())
        strResult += ", ";
      strResult += "YellowMine";
      nResources &= ~YELLOW_MINE_ID;
    }
    return strResult;
  }



  CSWWeaponSupply::CSWWeaponSupply(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWDynSolCol(strName, matBaseTObject, 
                 Mesh::createBox(Matrix44D(Vector3D(-size.getWidth()/2.0, 
                                                    -size.getHeight()/2.0, 
                                                    -size.getDepth()/2.0)),
                                 size)),
    m_pResourceProvider(CSWResourceProvider::create(300, GREEN_TORPEDO_ID /*| RED_TORPEDO_ID | BLUE_TORPEDO_ID*/ | GREEN_MINE_ID /*| YELLOW_MINE_ID*/)),
    m_fLastSentTime(0)
  {
    assert(size.getWidth() > 0 && size.getHeight() > 0 && size.getDepth() > 0);
    setColor(Vector4D(0, 1, 0, 1));
  }

}