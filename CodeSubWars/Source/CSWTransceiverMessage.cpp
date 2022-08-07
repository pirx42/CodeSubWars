// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWTransceiverMessage.h"


namespace CodeSubWars
{

  CSWTransceiverMessage::PtrType CSWTransceiverMessage::create(const std::string& strMessage)
  {
    return PtrType(new CSWTransceiverMessage(strMessage));
  }


  CSWTransceiverMessage::~CSWTransceiverMessage()
  {
  }


  CSWIMessage::PtrType CSWTransceiverMessage::copy() const
  {
    return PtrType(new CSWTransceiverMessage(*this));
  }


  std::string CSWTransceiverMessage::getInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "TransceiverMessage: " << m_strMessage;
    return ss.str();
  }


  const std::string& CSWTransceiverMessage::getText() const
  {
    return m_strMessage;
  }


  CSWTransceiverMessage::CSWTransceiverMessage(const std::string& strMessage)
  : m_strMessage(strMessage)
  {
  }


  CSWTransceiverMessage::CSWTransceiverMessage(const CSWTransceiverMessage& other)
  : m_strMessage(other.m_strMessage)
  {
  }

}