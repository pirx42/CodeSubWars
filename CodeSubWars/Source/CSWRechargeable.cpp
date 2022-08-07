// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWRechargeable.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "CSWIRechargeable.h"
#include "CSWIResourceProvider.h"


namespace CodeSubWars
{

  CSWRechargeable::PtrType CSWRechargeable::create(const unsigned long& nAcceptedResourceIDs)
  {
    return PtrType(new CSWRechargeable(nAcceptedResourceIDs));
  }


  CSWRechargeable::~CSWRechargeable()
  {
  }


  const unsigned long& CSWRechargeable::getRechargingIDs() const
  {
    return m_nRechargingIDs;
  }


  const unsigned long& CSWRechargeable::getAcceptedResourceIDs() const
  {
    return m_nAcceptedResourceIDs;
  }


  const unsigned long& CSWRechargeable::tryRecharging(CSWObject::PtrType pRechargeable, const unsigned long& nResourceIDs,
                                                      const std::string& strResourceProvider)
  {
    if (getRechargingIDs())
      return getRechargingIDs();

    ARSTD::Element::PtrType pFoundElement = ARSTD::Element::findElement(CSWWorld::getInstance()->getObjectTree(), strResourceProvider);
    CSWIResourceProvider::PtrType pResourceProvider = std::dynamic_pointer_cast<CSWIResourceProvider>(pFoundElement);
    if (pResourceProvider && std::dynamic_pointer_cast<CSWIRechargeable>(pRechargeable))
    {
      m_nRechargingIDs = pResourceProvider->requestRecharge(pRechargeable, nResourceIDs);
    }
    else
    {
      m_nRechargingIDs = 0;
    }
    return m_nRechargingIDs;
  }


  void CSWRechargeable::finalizeRecharge()
  {
    m_nRechargingIDs = 0;
  }


  CSWRechargeable::CSWRechargeable(const unsigned long& nAcceptedResourceIDs)
  : m_nAcceptedResourceIDs(nAcceptedResourceIDs),
    m_nRechargingIDs(0)
  {
  }

}