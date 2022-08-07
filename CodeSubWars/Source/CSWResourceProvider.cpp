// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "CSWResourceProvider.h"
#include "CSWIRechargeable.h"


namespace CodeSubWars
{

  CSWResourceProvider::PtrType CSWResourceProvider::create(double fMaxDistance, const unsigned long& nProvidedResourceIDs)
  {
    return PtrType(new CSWResourceProvider(fMaxDistance, nProvidedResourceIDs));
  }


  CSWResourceProvider::~CSWResourceProvider()
  {
  }


  unsigned long CSWResourceProvider::requestRecharge(std::shared_ptr<CSWObject> pObjectToRecharge, const unsigned long& nResourceIDs,
                                                     const Vector3D& vecWorldTPosition)
  {
    CSWIRechargeable::PtrType pRechargeObj = std::dynamic_pointer_cast<CSWIRechargeable>(pObjectToRecharge);
    //the requested resource must be provided
    unsigned long nRequestedResourcesThatAreProvided = nResourceIDs & m_nProvidedResourceIDs;
    //the requested resource must be accepted by recharging object
    unsigned long nRequestedResourcesThatAreAcceptedByRechargeable = nResourceIDs & pRechargeObj->getAcceptedResourceIDs();
    unsigned long nRequestedResourcesThatWillRecharged = nRequestedResourcesThatAreProvided & nRequestedResourcesThatAreAcceptedByRechargeable;
  
    if (pObjectToRecharge &&
        pRechargeObj &&
        nRequestedResourcesThatWillRecharged &&
        (vecWorldTPosition - pObjectToRecharge->getWorldTransform().getTranslation()).getSquaredLength() < m_fMaxDistance*m_fMaxDistance)
    {
      bool bResult = m_RegisteredObjects.insert(std::make_pair(pObjectToRecharge, nRequestedResourcesThatWillRecharged)).second;
      return bResult ? nRequestedResourcesThatWillRecharged : 0;
    }
    return 0;
  }


  void CSWResourceProvider::updateRecharging(const Vector3D& vecWorldTPosition)
  {
    if (m_RegisteredObjects.empty())
      return;

    double fSquaredDistance = m_fMaxDistance*m_fMaxDistance;
    RegisteredObjectsContainer newContainer;
    RegisteredObjectsContainer::iterator it = m_RegisteredObjects.begin();
    for (; it != m_RegisteredObjects.end(); ++it)
    {
      CSWObject::PtrType pObj = it->first.lock();
      CSWIRechargeable::PtrType pRechargeableObj = std::dynamic_pointer_cast<CSWIRechargeable>(pObj);
      if (pObj && pRechargeableObj)
      { 
        if ((vecWorldTPosition - pObj->getWorldTransform().getTranslation()).getSquaredLength() < fSquaredDistance)
        {
          if (!pRechargeableObj->recharge(it->second))
          {
            newContainer.insert(*it);
          }
          else pRechargeableObj->finalizeRecharge();
        }
        else
        {
          //if outside valid range stop recharging
          pRechargeableObj->finalizeRecharge();
        }
      }
    }
  
    m_RegisteredObjects.swap(newContainer);
  
  //  RegisteredObjectsContainer::iterator itNewEnd = std::remove_if(m_RegisteredObjects.begin(), 
  //                                                                 m_RegisteredObjects.end(),
  //                                                                 boost::mem_fn(&CSWIRechargeable::isFullRecharged));
  //  m_RegisteredObjects.erase(itNewEnd, m_RegisteredObjects.end());
  }


  std::string CSWResourceProvider::getCurrentInfo(std::string (*pTranslatFunc)(int)) const
  {
    std::stringstream ss;
    ss.setf(std::ios::fixed);
    ss.precision(1);

    int nNumRechargeables = 0;
    RegisteredObjectsContainer::const_iterator it = m_RegisteredObjects.begin();
    for (; it != m_RegisteredObjects.end(); ++it)
    {
      CSWObject::PtrType pObj = it->first.lock();
      CSWIRechargeable::PtrType pRechargeableObj = std::dynamic_pointer_cast<CSWIRechargeable>(pObj);
      if (pObj && pRechargeableObj)
      { 
        ++nNumRechargeables;
        ss << pObj->getName() << " recharging " << pTranslatFunc(it->second) << " (" << pRechargeableObj->getFillLevel()*100 << "%)\n";
      }
    }

    std::stringstream ss2;
    ss2 << "Provided resources: " << pTranslatFunc(getProvidedResourceIDs()) << "\n";
    ss2 << "Recharging objects (" << nNumRechargeables << "):\n";
    ss2 << ss.str();
    return ss2.str();
  }


  const unsigned long& CSWResourceProvider::getProvidedResourceIDs() const
  {
    return m_nProvidedResourceIDs;
  } 
 
    
  double CSWResourceProvider::getMaxDistance() const
  {
    return m_fMaxDistance;
  }


  CSWResourceProvider::CSWResourceProvider(double fMaxDistance, const unsigned long& nProvidedResourceIDs)
  : m_fMaxDistance(fMaxDistance),
    m_nProvidedResourceIDs(nProvidedResourceIDs)
  {
  }

}