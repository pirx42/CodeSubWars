// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWExecuteCommand.h"


namespace CodeSubWars
{

  CSWExecuteCommand::~CSWExecuteCommand()
  {
  }


  void CSWExecuteCommand::initialize()
  {
  }


  void CSWExecuteCommand::step()
  {
    if (wasExecuted())
      return;
  
    run();
    setProgress(1);
    getCommandProcessor()->finished();
  }


  void CSWExecuteCommand::cleanup()
  {
  }


  uint64 CSWExecuteCommand::getRessourceID() const
  {
    return 0;
  }


  std::string CSWExecuteCommand::getName() const
  {
    return m_strName;
  }


  std::string CSWExecuteCommand::getDetails() const
  {
    return "Execute code";
  }


  CSWExecuteCommand::CSWExecuteCommand(const std::string& strName)
  : m_strName(strName)
  {
  }


  CSWExecuteCommand::CSWExecuteCommand(const CSWExecuteCommand& other)
  : ARSTD::Command(other),
    m_strName(other.m_strName)
  {
  }

}