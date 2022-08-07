// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWWaitCommand.h"


namespace CodeSubWars
{

  CSWWaitCommand::PtrType CSWWaitCommand::create(double fWaitTime)
  {
    return PtrType(new CSWWaitCommand(fWaitTime));
  }


  CSWWaitCommand::~CSWWaitCommand()
  {
  }


  ARSTD::Command::PtrType CSWWaitCommand::copy() const
  {
    return PtrType(new CSWWaitCommand(*this));
  }


  void CSWWaitCommand::initialize()
  {
    m_fBeginTime = ARSTD::Time::getTime();
  }


  void CSWWaitCommand::step()
  {
    double fCurrentTime = ARSTD::Time::getTime();
    if (m_fBeginTime + m_fWaitTime > fCurrentTime)
    {
      setProgress((fCurrentTime - m_fBeginTime)/m_fWaitTime);
    }
    else
    {
      setProgress(1);
      getCommandProcessor()->finished();
    }
  }


  void CSWWaitCommand::cleanup()
  {
  }


  uint64 CSWWaitCommand::getRessourceID() const
  {
    return 0;
  }


  std::string CSWWaitCommand::getName() const
  {
    return m_strName;
  }


  std::string CSWWaitCommand::getDetails() const
  {
    std::stringstream ss;
    ss.precision(2);
    ss.setf(std::ios::fixed);
    ss << "Waiting " << m_fWaitTime << " seconds";
    return ss.str();
  }


  CSWWaitCommand::CSWWaitCommand(double fWaitTime)
  : m_strName("Wait"),
    m_fWaitTime(fWaitTime),
    m_fBeginTime(ARSTD::Time::getTime())
  {
  }


  CSWWaitCommand::CSWWaitCommand(const CSWWaitCommand& other)
  : ARSTD::Command(other),
    m_strName(other.m_strName),
    m_fWaitTime(other.m_fWaitTime),
    m_fBeginTime(other.m_fBeginTime)
  {
  }

}