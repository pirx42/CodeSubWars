// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWTransceiver.h"
#include "CSWEventDealable.h"

#include "CSWEvent.h"
#include "CSWTransceiverMessage.h"


namespace CodeSubWars
{

  const int CSWTransceiver::MAX_NUM_QUEUED_MESSAGES = 100;


  CSWTransceiver::PtrType CSWTransceiver::create(const std::string& strName)
  {
    return PtrType(new CSWTransceiver(strName));
  }


  CSWTransceiver::~CSWTransceiver()
  {
  }


  void CSWTransceiver::receiveEvent(CSWEvent::PtrType pEvent)
  {
    m_pEventDealable->receiveEvent(pEvent);
  }


  void CSWTransceiver::processReceivedQueuedEvents()
  {
    m_pEventDealable->processReceivedQueuedEvents();
  }


  bool CSWTransceiver::processEvent(CSWEvent::PtrType pEvent)
  {
    //get all messages an decrypt (front received first)
    CSWTransceiverMessage::PtrType pMessage = std::dynamic_pointer_cast<CSWTransceiverMessage>(pEvent->getMessage());
    if (!pMessage)
      return true; //discard all other messages

    m_ReceivedMessages.push_back(pMessage->getText());
  
    //processing transceiver messages is always successfully
    return true;
  }


  std::string CSWTransceiver::getCurrentInformation() const
  {
    return m_pEventDealable->getCurrentInformation();
  }


  std::string CSWTransceiver::getCurrentMessage() const
  {
    return m_pEventDealable->getCurrentMessage();
  }


  void CSWTransceiver::setEncryptKey(const std::string& strKey)
  {
  throw std::runtime_error("en/decryption is currently not supported!");
  //  m_Encryptor = ARSTD::Rijndael(ARSTD::Rijndael::ByteContainer(strKey.begin(), strKey.end()));
  }


  int CSWTransceiver::getMaxNumQueuedMessages() const
  {
    return MAX_NUM_QUEUED_MESSAGES;
  }


  int CSWTransceiver::getNumQueuedMessages() const
  {
    return static_cast<int>(m_ReceivedMessages.size());
  }


  void CSWTransceiver::send(const std::string& strMessage)
  {
    m_pEventDealable->send(CSWEvent::create(getSharedThis(), CSWObject::PtrType(), CSWTransceiverMessage::create(strMessage), 1));
  }


  std::string CSWTransceiver::getFirstReceived()
  {
    std::string strMessage;
    if (!m_ReceivedMessages.empty())
    {
      strMessage = m_ReceivedMessages.front();
      m_ReceivedMessages.pop_front();
    }
    return strMessage;
  }


  CSWTransceiver::CSWTransceiver(const std::string& strName)
  : CSWEquipment(strName, Matrix44D(), 0, 0),
    m_pEventDealable(CSWEventDealable::create()),
    m_ReceivedMessages(MAX_NUM_QUEUED_MESSAGES)
  {
  }

}