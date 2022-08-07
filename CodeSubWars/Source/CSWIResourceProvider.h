// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWObject;

  /**
   * Objects with this property are able to provide resources to rechargeable objects.
   */ 
  class CSWIResourceProvider 
  {
    public:
      typedef std::shared_ptr<CSWIResourceProvider> PtrType;

      virtual ~CSWIResourceProvider() {}

      //method is called by an CSWIRechargeable to register to the provider for recharging the given resources. 
      //returning resource ids that will be recharged. 0 means failure.
      virtual unsigned long requestRecharge(std::shared_ptr<CSWObject> pObjectToRecharge, const unsigned long& nResourceIDs) = 0;
    
      //makes the provider updating all currently registered rechargeable objects.
      virtual void updateRecharging() = 0;

      virtual std::string getCurrentInfo() const = 0;
    
  };

}