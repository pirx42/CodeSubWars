// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property are able to be recharged by a resource provider. For recharge at a 
   * resource provider objects must be request recharging. If the object is currently recharging 
   * a new request of recharge to the same provider will fail until the recharging ends.
   */ 
  class CSWIRechargeable 
  {
    public:
      typedef std::shared_ptr<CSWIRechargeable> PtrType;

      virtual ~CSWIRechargeable() {}
    
      //Signals if the recharging is currently running
      //@return Recharging ids if recharging is currently running otherwise 0.
      virtual const unsigned long& getRechargingIDs() const = 0;

      //Returns the current fill level in range [0, 1].
      //@return The current fill level in range [0, 1].
      virtual double getFillLevel() const = 0;

      //This method is called periodically to recharge the object with the given resources. Returns true on 
      //fully recharged otherwise false.
      virtual bool recharge(const unsigned long& nResourceIDs) = 0;
    
      //bitwise ored combination of accepted resources
      virtual const unsigned long& getAcceptedResourceIDs() = 0;

      //Tries to recharge itself by the given resource provider.
      //@param strResourceProvider The name of the resource provider on which the object should try to recharge.
      //@param nResourceIDs The bitwise ORed resources that should be recharged by the requested provider. This should be
      //                    a subset of the resources that this rechargeable object could contain.
      //@return Returns the resource ids that will be recharged. Not 0 is returned on successfully applying for recharging on the given 
      //        provider for at least one requested resource otherwise 0.
      virtual const unsigned long& tryRecharging(const std::string& strResourceProvider, const unsigned long& nResourceIDs) = 0;
    
      //reset recharge process
      virtual void finalizeRecharge() = 0;
  };

}