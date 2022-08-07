// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "CSWSystemMessage.h"


namespace CodeSubWars
{

  CSWSystemMessage::PtrType CSWSystemMessage::create(const std::string& strMessage)
  {
    return PtrType(new CSWSystemMessage(strMessage));
  }


  CSWSystemMessage::~CSWSystemMessage()
  {
  }


  CSWIMessage::PtrType CSWSystemMessage::copy() const
  {
    return PtrType(new CSWSystemMessage(*this));
  }


  std::string CSWSystemMessage::getInformation() const
  {
    std::stringstream ss;
    ss.precision(3);
    ss.setf(std::ios::fixed);
    ss << "SystemMessage: " << m_strMessage;
    return ss.str();
  }


  const std::string& CSWSystemMessage::getText() const
  {
    return m_strMessage;
  }


  CSWSystemMessage::CSWSystemMessage(const std::string& strMessage)
  : m_strMessage(strMessage)
  {
  }


  CSWSystemMessage::CSWSystemMessage(const CSWSystemMessage& other)
  : m_strMessage(other.m_strMessage)
  {
  }

}