// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWEvent.h"
#include "CSWObject.h"
#include "CSWIMessage.h"


namespace CodeSubWars
{

  const double CSWEvent::MAX_EXPIRE_TIME = 20;


  CSWEvent::PtrType CSWEvent::create(CSWObject::PtrType pSender, CSWObject::PtrType pReceiver, 
                                     CSWIMessage::PtrType pMessage, double fExpireTime)
  {
    return PtrType(new CSWEvent(pSender, pReceiver, pMessage, fExpireTime));
  }


  CSWEvent::PtrType CSWEvent::createBroadcast(CSWObject::PtrType pSender, CSWIMessage::PtrType pMessage, double fExpireTime)
  {
    return PtrType(new CSWEvent(pSender, CSWObject::PtrType(), pMessage, fExpireTime));
  }


  CSWEvent::PtrType CSWEvent::createAnonymous(CSWObject::PtrType pReceiver, CSWIMessage::PtrType pMessage, double fExpireTime)
  {
    return PtrType(new CSWEvent(CSWObject::PtrType(), pReceiver, pMessage, fExpireTime));
  }


  CSWEvent::~CSWEvent()
  {
  }


  CSWEvent::PtrType CSWEvent::copy() const
  {
    return PtrType(new CSWEvent(*this));  
  }


  CSWObject::PtrType CSWEvent::getSender()
  {
    return m_pSender.lock();
  }


  CSWObject::PtrType CSWEvent::getReceiver()
  {
    return m_pReceiver.lock();
  }


  void CSWEvent::setReceiver(CSWObject::PtrType pObject)
  {
    m_pReceiver = pObject;
  }


  CSWIMessage::PtrType CSWEvent::getMessage()
  {
    return m_pMessage;
  }


  bool CSWEvent::hasExpired() const
  {
    return ARSTD::Time::getTime() > m_fSendTime + m_fExpireTime;
  }


  CSWEvent::CSWEvent(CSWObject::PtrType pSender, CSWObject::PtrType pReceiver, 
                     CSWIMessage::PtrType pMessage, double fExpireTime)
  : m_pSender(pSender),
    m_pReceiver(pReceiver),
    m_pMessage(pMessage),
    m_fExpireTime(fExpireTime),
    m_fSendTime(0)
  {
    if (m_fExpireTime < 0)
      m_fExpireTime = 0;
    if (m_fExpireTime > MAX_EXPIRE_TIME)
      m_fExpireTime = MAX_EXPIRE_TIME;
  }


  CSWEvent::CSWEvent(const CSWEvent& other)
  : m_pSender(other.m_pSender),
    m_pReceiver(other.m_pReceiver),
    m_pMessage(other.m_pMessage->copy()),
    m_fExpireTime(other.m_fExpireTime),
    m_fSendTime(other.m_fSendTime)
  {
  }


}