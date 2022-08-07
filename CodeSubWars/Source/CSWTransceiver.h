// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "CSWIEventDealable.h"

#include "CSWEquipment.h"

namespace CodeSubWars
{

  class CSWEvent;
  class CSWEventDealable;

  /**
   * This class encapsulates transceiver functionality. With this you can send and receive broadcast messages. The last 100 received text messages are
   * queued until they fetched with getFirstReceived.
   */
  class CSWTransceiver : public CSWEquipment,
                         public CSWIEventDealable

  {
    public:
      typedef std::shared_ptr<CSWTransceiver> PtrType;

      static PtrType create(const std::string& strName);

      virtual ~CSWTransceiver();

      //defined methods for eventdealable
      virtual void receiveEvent(std::shared_ptr<CSWEvent> pEvent);
      virtual void processReceivedQueuedEvents();
      virtual bool processEvent(std::shared_ptr<CSWEvent> pEvent);
      virtual std::string getCurrentInformation() const;
      virtual std::string getCurrentMessage() const;

      void setEncryptKey(const std::string& strKey);

      /**
       * Returns the maximal number of messages that will be queued.
       */
      int getMaxNumQueuedMessages() const;

      /**
       * Returns the current queued number of messages.
       */
      int getNumQueuedMessages() const;

      /**
       * Sends a given text to all eventdealable objects within the world, but not to itself.
       * @param strMessage The text that should be sent.
       */
      void send(const std::string& strMessage);
    
      /**
       * Returns the first (the oldest in the queue) received text and removes it from internal queue. 
       * If no text is available "" is returned.
       */
      std::string getFirstReceived();
 
    protected:
      typedef boost::circular_buffer<std::string> MessageQueue;

      CSWTransceiver(const std::string& strName);

      static const int MAX_NUM_QUEUED_MESSAGES;

      std::shared_ptr<CSWEventDealable> m_pEventDealable;
      MessageQueue m_ReceivedMessages;
  };

}