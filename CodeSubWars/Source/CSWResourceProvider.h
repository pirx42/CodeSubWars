// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWObject.h"

namespace CodeSubWars
{

  //class CSWObject;

  class CSWResourceProvider
  {
    public:
      typedef std::shared_ptr<CSWResourceProvider> PtrType;

      static PtrType create(double fMaxDistance, const unsigned long& nProvidedResourceIDs);
      ~CSWResourceProvider();

      unsigned long requestRecharge(std::shared_ptr<CSWObject> pObjectToRecharge, const unsigned long& nResourceIDs,
                                    const Vector3D& vecWorldTPosition);
    
      void updateRecharging(const Vector3D& vecWorldTPosition);

      std::string getCurrentInfo(std::string (*pTranslatFunc)(int)) const;

      const unsigned long& getProvidedResourceIDs() const;
    
      double getMaxDistance() const;

    protected:
      //distance to which recharging is possible. bitwise ored resourceids that were provided
      CSWResourceProvider(double fMaxDistance, const unsigned long& nProvidedResourceIDs);

      typedef std::map<std::weak_ptr<CSWObject>, unsigned long, ObjectLess> RegisteredObjectsContainer; //object to recharge + resources that will recharged

      RegisteredObjectsContainer m_RegisteredObjects;
      double m_fMaxDistance;
      const unsigned long m_nProvidedResourceIDs;
  };

}