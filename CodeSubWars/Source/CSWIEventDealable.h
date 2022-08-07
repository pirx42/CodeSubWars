// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWEvent;

  /**
   * Objects with this property can send or receive events.
   */
  class CSWIEventDealable
  {
    public:
      typedef std::shared_ptr<CSWIEventDealable> PtrType;
      typedef std::list<std::shared_ptr<CSWEvent> > EventContainer;
      typedef EventContainer::const_iterator EventConstIterator;    
      typedef std::pair<EventConstIterator, EventConstIterator> EventConstRange;
  
      virtual ~CSWIEventDealable() {}

      virtual void receiveEvent(std::shared_ptr<CSWEvent> pEvent) = 0;
      virtual void processReceivedQueuedEvents() = 0;

      //This method must be overriden in the specialized class. This methode is called if an event has been received by the object that should be 
      //processed. Returning true reports the eventmanager that the event passed to the object has successfully processed and delete it.
      //When returning false the event will be delivered the next time again until it expires or processed successfully.
      //@return True if the incoming event has been successfully processed. When returning false the event will be delivered next time again.
      virtual bool processEvent(std::shared_ptr<CSWEvent> pEvent) { return true; }

      virtual std::string getCurrentInformation() const = 0;
      virtual std::string getCurrentMessage() const = 0;

  };


}