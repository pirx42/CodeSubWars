// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "CommandProcessorContext.h"
#include "Command.h"
#include "CommandProcessor.h"


namespace ARSTD
{

  CommandProcessorContext::PtrType CommandProcessorContext::create()
  {
    return PtrType(new CommandProcessorContext());
  }


  CommandProcessorContext::~CommandProcessorContext()
  {
  }


  CommandProcessorContext::PtrType CommandProcessorContext::copy()
  {
    return PtrType(new CommandProcessorContext(*this));
  }


  bool CommandProcessorContext::addCommand(Command::PtrType pCommand)
  {
    m_WaitingCommands.push_back(pCommand);
    return true;
  }


  Command::PtrType CommandProcessorContext::getCurrentExecutingCommand() const
  {
    return m_pCurrentExecutingCommand;
  }


  void CommandProcessorContext::cleanup()
  {
    m_ExecutedCommands.clear();
    m_pCurrentExecutingCommand.reset();
    m_WaitingCommands.clear();
  }
  

  void CommandProcessorContext::cleanupHistory()
  {
    m_ExecutedCommands.clear();
  }
  

  void CommandProcessorContext::setCommandProcessor(CommandProcessor* pCommandProcessor)
  {
//    if (m_pCurrentExecutingCommand) m_pCurrentExecutingCommand->setCommandProcessor(pCommandProcessor);
//
//    ICommandHandler::CommandContainer::const_iterator it = other.m_WaitingCommands.begin();
//    ICommandHandler::CommandContainer::const_iterator itEnd = other.m_WaitingCommands.end();
//    for (; it != itEnd; ++it)
//    {
//      (*it)->setCommandProcessor(pCommandProcessor);      
//    }
  }
  

  int CommandProcessorContext::getNumExecutedCommands() const
  {
    return static_cast<int>(m_ExecutedCommands.size());
  }


  int CommandProcessorContext::getNumWaitingCommands() const
  {
    return static_cast<int>(m_WaitingCommands.size());
  }


  ICommandHandler::CommandRange CommandProcessorContext::getWaitingCommands()
  {
    return std::make_pair(m_WaitingCommands.begin(), m_WaitingCommands.end());
  }
  
  
  ICommandHandler::CommandConstRange CommandProcessorContext::getWaitingCommands() const
  {
    return std::make_pair(m_WaitingCommands.begin(), m_WaitingCommands.end());
  }


  void CommandProcessorContext::incrementCurrentExecutingCommand()
  {
    if (m_pCurrentExecutingCommand)
      m_ExecutedCommands.push_back(m_pCurrentExecutingCommand);
    if (!m_WaitingCommands.empty())
    {
      m_pCurrentExecutingCommand = m_WaitingCommands.front();
      m_WaitingCommands.pop_front();
    }
    else
      m_pCurrentExecutingCommand.reset();
  }


  void CommandProcessorContext::setAllCommandsToNotExecuted()
  {
    ICommandHandler::CommandContainer newWaitingCommands;
    
    ICommandHandler::CommandConstIterator it = m_ExecutedCommands.begin();
    for (; it != m_ExecutedCommands.end(); ++it)
    {
      newWaitingCommands.push_back(*it);
    }
    
    if (m_pCurrentExecutingCommand)
      newWaitingCommands.push_back(m_pCurrentExecutingCommand);

    it = m_WaitingCommands.begin();
    for (; it != m_WaitingCommands.end(); ++it)
    {
      newWaitingCommands.push_back(*it);
    }
    
    cleanup();
    m_WaitingCommands.swap(newWaitingCommands);
  }
  

  CommandProcessorContext::CommandProcessorContext()
  {
  }


  CommandProcessorContext::CommandProcessorContext(const CommandProcessorContext& other)
  {
    ICommandHandler::CommandContainer::const_iterator it = other.m_ExecutedCommands.begin();
    ICommandHandler::CommandContainer::const_iterator itEnd = other.m_ExecutedCommands.end();
    for (; it != itEnd; ++it)
    {
      m_ExecutedCommands.push_back((*it)->copy());
    }

    if (other.m_pCurrentExecutingCommand) m_pCurrentExecutingCommand = other.m_pCurrentExecutingCommand->copy();
  
    it = other.m_WaitingCommands.begin();
    itEnd = other.m_WaitingCommands.end();
    for (; it != itEnd; ++it)
    {
      m_WaitingCommands.push_back((*it)->copy());
    }
  }



} //namespace ARSTD