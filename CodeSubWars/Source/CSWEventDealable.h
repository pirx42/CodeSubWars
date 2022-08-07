// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWIEventDealable.h"

namespace CodeSubWars
{

  class CSWEvent;

  //@warning only the last 30 outgoing and incoming events are buffered to be displayed in the gui.
  class CSWEventDealable
  {
    public:
      typedef std::shared_ptr<CSWEventDealable> PtrType;
      typedef CSWIEventDealable::EventContainer EventContainer;
      typedef CSWIEventDealable::EventConstIterator EventConstIterator;    
      typedef CSWIEventDealable::EventConstRange EventConstRange;

      static PtrType create();
      ~CSWEventDealable();

      virtual void receiveEvent(std::shared_ptr<CSWEvent> pEvent);
      virtual void processReceivedQueuedEvents();

      void send(std::shared_ptr<CSWEvent> pEvent);

      std::string getCurrentInformation() const;
      std::string getCurrentMessage() const;

    protected:
      CSWEventDealable();

      EventContainer m_IncomingEventContainer;

  struct EventDetail
  {
    EventDetail() : time(0), type("NN") {}
  
    EventDetail(const std::string& tp, std::shared_ptr<CSWEvent> pE)
    : time(ARSTD::Time::getTime()), type(tp), pEvent(pE)
    {
    }

    double time;
    std::string type; //"I" or "O"
    std::shared_ptr<CSWEvent> pEvent;
  };

  mutable boost::circular_buffer<EventDetail> m_TmpEventContainer;
  mutable int m_nIncoming;
  mutable int m_nOutgoing;

  };


}