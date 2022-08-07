// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "Constants.h"
#include "CSWEventDealable.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"
#include "CSWObject.h"
#include "CSWIMessage.h"


namespace CodeSubWars
{

  CSWEventDealable::PtrType CSWEventDealable::create()
  {
    return PtrType(new CSWEventDealable());
  }


  CSWEventDealable::~CSWEventDealable()
  {
  }


  void CSWEventDealable::receiveEvent(CSWEvent::PtrType pEvent)
  {
    m_IncomingEventContainer.push_back(pEvent);
  m_TmpEventContainer.push_back(EventDetail("I", pEvent));
  ++m_nIncoming;
  }


  void CSWEventDealable::processReceivedQueuedEvents()
  {
    bool bRemoveFromQueue;
    EventContainer::iterator it = m_IncomingEventContainer.begin();
    for (; it != m_IncomingEventContainer.end();)
    { 
      bRemoveFromQueue = false;
      if (!(*it)->hasExpired())
      {
        CSWIEventDealable::PtrType pEventDealable = std::dynamic_pointer_cast<CSWIEventDealable>((*it)->getReceiver());
        if (pEventDealable)
        {
          bRemoveFromQueue = pEventDealable->processEvent(*it);
        }
      }
      else
      {
        bRemoveFromQueue = true;
      }

      //if event has expired or successfully processed erase event from queue
      EventContainer::iterator itTmp = it++;
      if (bRemoveFromQueue)
        m_IncomingEventContainer.erase(itTmp);
    }
  }


  void CSWEventDealable::send(std::shared_ptr<CSWEvent> pEvent)
  {
      CSWEventManager::getInstance()->send(pEvent);
  m_TmpEventContainer.push_back(EventDetail("O", pEvent));
  ++m_nOutgoing;
  }


  std::string CSWEventDealable::getCurrentInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "Incoming: " << m_nIncoming << "\n"
       << "Outgoing: " << m_nOutgoing << "\n";
    return ss.str();
  }


  std::string CSWEventDealable::getCurrentMessage() const
  {
    if (m_TmpEventContainer.empty())
      return std::string();
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
  
    boost::circular_buffer<EventDetail>::const_iterator it = m_TmpEventContainer.begin();
    for (; it != m_TmpEventContainer.end(); ++it)
    {
      ss << it->time << " " << it->type << "  " << it->pEvent->getMessage()->getInformation() << "\n";
    }
    m_TmpEventContainer.clear();
  
    return ss.str();
  }


  CSWEventDealable::CSWEventDealable()
  : m_nIncoming(0),
    m_nOutgoing(0),
    m_TmpEventContainer(30) //only the last 30 events will be buffered for getting detail information
  {
  }


}