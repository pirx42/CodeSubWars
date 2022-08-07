// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWObject;

  class CSWRechargeable
  {
    public:
      typedef std::shared_ptr<CSWRechargeable> PtrType;

      static PtrType create(const unsigned long& nAcceptedResourceIDs);

      ~CSWRechargeable();

      const unsigned long& getRechargingIDs() const;
      const unsigned long& getAcceptedResourceIDs() const;
      const unsigned long& tryRecharging(std::shared_ptr<CSWObject> pRechargeable, const unsigned long& nResourceIDs, 
                                         const std::string& strResourceProvider);
      void finalizeRecharge();

    protected:
      CSWRechargeable(const unsigned long& nAcceptedResourceIDs);
    
      const unsigned long m_nAcceptedResourceIDs;
      unsigned long m_nRechargingIDs;
  };

}