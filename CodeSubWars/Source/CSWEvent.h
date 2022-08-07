// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWObject;
  class CSWIMessage;
  class CSWEventManager;

  /**
   * This class encapsulates an event. An event will delivered from sender to an receiver. It contains a message and
   * a time to life.
   */
  class CSWEvent
  {
    public:
      typedef std::shared_ptr<CSWEvent> PtrType;

      /**
       * Creates a new Event.
       * @param pSender The sender (should always be self, because you never know other submarines)
       * @param pReceiver The receiver (should always be self, because you never know other submarines)
       * @param pMessage The message that should be delivered.
       * @param fExpireTime The time to life in seconds. This is clamped to range [0, 20].
       */
      static PtrType create(std::shared_ptr<CSWObject> pSender, std::shared_ptr<CSWObject> pReceiver, 
                            std::shared_ptr<CSWIMessage> pMessage, double fExpireTime);

      static PtrType createBroadcast(std::shared_ptr<CSWObject> pSender, std::shared_ptr<CSWIMessage> pMessage, double fExpireTime);
      static PtrType createAnonymous(std::shared_ptr<CSWObject> pReceiver, std::shared_ptr<CSWIMessage> pMessage, double fExpireTime);

      virtual ~CSWEvent();

      PtrType copy() const;

      std::shared_ptr<CSWObject> getSender();
      std::shared_ptr<CSWObject> getReceiver();

      void setReceiver(std::shared_ptr<CSWObject> pObject);

      /**
       * Returns the containing message.
       * @return The containing message.
       */
      std::shared_ptr<CSWIMessage> getMessage();

      bool hasExpired() const;

      friend CSWEventManager;

    protected:
      CSWEvent(std::shared_ptr<CSWObject> pSender, std::shared_ptr<CSWObject> pReceiver, 
               std::shared_ptr<CSWIMessage> pMessage, double fExpireTime);

      CSWEvent();
      CSWEvent(const CSWEvent& other);

      static const double MAX_EXPIRE_TIME;

      std::weak_ptr<CSWObject> m_pSender;
      std::weak_ptr<CSWObject> m_pReceiver;
      std::shared_ptr<CSWIMessage> m_pMessage;
      //absolute time in seconds
      double m_fSendTime; 
      //relative time in seconds
      double m_fExpireTime; 
  };

}