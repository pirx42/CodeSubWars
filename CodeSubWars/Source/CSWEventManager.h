// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/smart_ptr.hpp>

namespace CodeSubWars
{

  class CSWEvent;

  /**
   * This class manages sending and delivering of events. An event is repeatly delivered to the receiver until the receiver processed the event
   * successfully by returning true in the processEvent method or the time to life has expired.
   */
  class CSWEventManager
  {
    public:
      typedef std::shared_ptr<CSWEventManager> PtrType;

      static PtrType getInstance();

      /**
       * Sends an event.
       * @param pEvent The event to sent.
       */
      void send(std::shared_ptr<CSWEvent> pEvent);

      //deliver all queued events
      void deliverAllEvents();

    protected:
      typedef std::list<std::shared_ptr<CSWEvent> > EventContainer;

      CSWEventManager();

      EventContainer m_EventContainer;
  };

}