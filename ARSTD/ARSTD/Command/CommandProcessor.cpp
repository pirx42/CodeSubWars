// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include <exception>
#include <stdexcept>

#include "Command.h"
#include "CommandProcessorContext.h"
#include "CommandProcessor.h"


namespace ARSTD
{

  CommandProcessor::PtrType CommandProcessor::create()
  {
    return PtrType(new CommandProcessor());
  }


  CommandProcessor::~CommandProcessor()
  {
  }


  void CommandProcessor::execute(Command::PtrType pCommand)
  {
    if (!m_pContext || !pCommand)
      return;
    
    pCommand->m_pCommandprocessor = this;
    pCommand->resetProgress();
    m_pContext->addCommand(pCommand);

    if (!m_pContext->getCurrentExecutingCommand())
      executeNext();
  }


  void CommandProcessor::finished()
  {
    executeNext();
  }


  void CommandProcessor::repeat()
  {
    if (!m_pContext)
      return;

    if (m_pContext->getCurrentExecutingCommand())
      m_pContext->getCurrentExecutingCommand()->cleanup();

    m_pContext->setAllCommandsToNotExecuted();
    ICommandHandler::CommandRange range = m_pContext->getWaitingCommands();
    ICommandHandler::CommandIterator it = range.first;
    for (; it != range.second; ++it)
    {
      (*it)->resetProgress();
    }
    executeNext();
  }


  bool CommandProcessor::push()
  {
    if (!m_pContext)
      return false;

    if (m_ContextStack.size() > 30)
      throw std::runtime_error("ARSTD::CommandProcessor::push(): maximum context stack depth exceeded");

    m_ContextStack.push_back(m_pContext->copy());
    return true;
  }


  bool CommandProcessor::pop()
  {
    if (m_ContextStack.empty())
      return false;

    if (m_pContext && m_pContext->getCurrentExecutingCommand())
      m_pContext->getCurrentExecutingCommand()->cleanup();
    
    m_pContext = m_ContextStack.back();
    m_ContextStack.pop_back();
    return true;
  }


  void CommandProcessor::step()
  {
    if (!m_pContext)
      return;

    if (m_pContext->getCurrentExecutingCommand())
      m_pContext->getCurrentExecutingCommand()->step();
    else
      executeNext();
  }


  void CommandProcessor::cleanup()
  {
    if (!m_pContext)
      return;

    if (m_pContext->getCurrentExecutingCommand())
      m_pContext->getCurrentExecutingCommand()->cleanup();

    m_pContext->cleanup();
  }


  void CommandProcessor::cleanupStack()
  {
    cleanup();
    m_ContextStack.clear();
  }


  void CommandProcessor::cleanupHistory()
  {
    if (!m_pContext)
      return;

    m_pContext->cleanupHistory();
  }


  void CommandProcessor::skipCurrent()
  {
    executeNext();
  }


  bool CommandProcessor::isBusy()
  {
    if (!m_pContext)
      return false;

    return m_pContext->getCurrentExecutingCommand() != nullptr;
  }


  double CommandProcessor::getCurrentCommandProgress() const
  {
    if (!m_pContext || !m_pContext->getCurrentExecutingCommand())
      return -1;
    return m_pContext->getCurrentExecutingCommand()->getProgress();
  }


  std::string CommandProcessor::getCurrentCommandName() const
  {
    if (!m_pContext || !m_pContext->getCurrentExecutingCommand())
      return "";
    return m_pContext->getCurrentExecutingCommand()->getName();  
  }


  std::string CommandProcessor::getCurrentCommandDetails() const
  {
    if (!m_pContext || !m_pContext->getCurrentExecutingCommand())
      return "";
    return m_pContext->getCurrentExecutingCommand()->getDetails();  
  }


  unsigned int CommandProcessor::getNumWaitingCommands() const
  {
    if (!m_pContext)
      return 0;
    return m_pContext->getNumWaitingCommands();
  }


  CommandProcessor::CommandConstRange CommandProcessor::getWaitingCommands() const
  {
    if (!m_pContext)
      return std::make_pair(CommandConstIterator(), CommandConstIterator());
    return m_pContext->getWaitingCommands();
  }


  unsigned int CommandProcessor::getCurrentStackDepth() const
  {
    return static_cast<unsigned int>(m_ContextStack.size());
  }


  unsigned int CommandProcessor::getNumExecutedCommands() const
  {
    if (!m_pContext)
      return 0;
    return m_pContext->getNumExecutedCommands();
  }


  CommandProcessor::CommandProcessor()
  : m_pContext(CommandProcessorContext::create())
  {
  }


  void CommandProcessor::executeNext()
  {
    if (!m_pContext)
      return;

    if (m_pContext->getCurrentExecutingCommand())
      m_pContext->getCurrentExecutingCommand()->cleanup();
        
    m_pContext->incrementCurrentExecutingCommand();

    if (m_pContext->getCurrentExecutingCommand()) 
    {
      m_pContext->getCurrentExecutingCommand()->resetProgress();
      m_pContext->getCurrentExecutingCommand()->initialize();
    }
  }

} //namespace ARSTD
