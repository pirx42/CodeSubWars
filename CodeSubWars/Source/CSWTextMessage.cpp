// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWTextMessage.h"


namespace CodeSubWars
{

  CSWTextMessage::PtrType CSWTextMessage::create(const std::string& strMessage)
  {
    return PtrType(new CSWTextMessage(strMessage));
  }


  CSWTextMessage::~CSWTextMessage()
  {
  }


  CSWIMessage::PtrType CSWTextMessage::copy() const
  {
    return PtrType(new CSWTextMessage(*this));
  }


  std::string CSWTextMessage::getInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "TextMessage: " << m_strMessage;
    return ss.str();
  }


  const std::string& CSWTextMessage::getText() const
  {
    return m_strMessage;
  }


  CSWTextMessage::CSWTextMessage(const std::string& strMessage)
  : m_strMessage(strMessage)
  {
  }


  CSWTextMessage::CSWTextMessage(const CSWTextMessage& other)
  : m_strMessage(other.m_strMessage)
  {
  }

}