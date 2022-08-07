// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <sstream>
#include "BasicCommands.h"
#include "CommandProcessor.h"


namespace ARSTD
{

  RepeatCommand::PtrType RepeatCommand::create(int nCycles)
  {
    return PtrType(new RepeatCommand(nCycles));
  }


  RepeatCommand::~RepeatCommand()
  {
  }


  ARSTD::Command::PtrType RepeatCommand::copy() const
  {
    return PtrType(new RepeatCommand(*this));
  }


  void RepeatCommand::initialize()
  {
  }


  void RepeatCommand::step()
  {
    bool bRepeatSuccessful = false;
    if (m_nCurrentCycles != 0)
    {
      getCommandProcessor()->repeat();
      bRepeatSuccessful = true;

      if (m_nCurrentCycles > 0)
        --m_nCurrentCycles;
    }

    if (m_nCycles < 0)
      setProgress(0.5);
    else
      setProgress(static_cast<double>(m_nCycles - m_nCurrentCycles)/m_nCycles);

    //only if the repeating could not be "started" the command must finished directly otherwise this will done automaticaly
    //by the repeat method call
    if (!bRepeatSuccessful)
    {
      m_nCurrentCycles = m_nCycles;
      getCommandProcessor()->finished();
    }
  }


  void RepeatCommand::cleanup()
  {
  }


  uint64 RepeatCommand::getRessourceID() const
  {
    return 0;
  }


  std::string RepeatCommand::getName() const
  {
    return "Repeat";
  }


  std::string RepeatCommand::getDetails() const
  {
    std::stringstream ss;
    ss << "Repeats previous commands: cycle " << m_nCurrentCycles << " of " << m_nCycles;
    return ss.str();
  }


  RepeatCommand::RepeatCommand(int nCycles)
  : m_nCycles(nCycles)
  {
    if (m_nCycles < 0)
      m_nCycles = -1;
    m_nCurrentCycles = m_nCycles;
  }


  RepeatCommand::RepeatCommand(const RepeatCommand& other)
  : ARSTD::Command(other),
    m_nCycles(other.m_nCycles),
    m_nCurrentCycles(other.m_nCurrentCycles)
  {
  }





  PushCommand::PtrType PushCommand::create()
  {
    return PtrType(new PushCommand());
  }


  PushCommand::~PushCommand()
  {
  }


  ARSTD::Command::PtrType PushCommand::copy() const
  {
    return PtrType(new PushCommand(*this));
  }


  void PushCommand::initialize()
  {
  }


  void PushCommand::step()
  {
    getCommandProcessor()->push();
    setProgress(1);
    getCommandProcessor()->finished();
  }


  void PushCommand::cleanup()
  {
  }


  uint64 PushCommand::getRessourceID() const
  {
    return 0;
  }


  std::string PushCommand::getName() const
  {
    return "Push";
  }


  std::string PushCommand::getDetails() const
  {
    return "Pushs the current command processor context to a stack";
  }


  PushCommand::PushCommand()
  {
  }


  PushCommand::PushCommand(const PushCommand& other)
  : ARSTD::Command(other)
  {
  }





  PopCommand::PtrType PopCommand::create()
  {
    return PtrType(new PopCommand());
  }


  PopCommand::~PopCommand()
  {
  }


  ARSTD::Command::PtrType PopCommand::copy() const
  {
    return PtrType(new PopCommand(*this));
  }


  void PopCommand::initialize()
  {
  }


  void PopCommand::step()
  {
    setProgress(1);
    if (!getCommandProcessor()->pop())
      getCommandProcessor()->finished();
  }


  void PopCommand::cleanup()
  {
  }


  uint64 PopCommand::getRessourceID() const
  {
    return 0;
  }


  std::string PopCommand::getName() const
  {
    return "Pop";
  }


  std::string PopCommand::getDetails() const
  {
    return "Pops the command processor context from a stack and make it current";
  }


  PopCommand::PopCommand()
  {
  }


  PopCommand::PopCommand(const PopCommand& other)
  : ARSTD::Command(other)
  {
  }





  CleanupHistoryCommand::PtrType CleanupHistoryCommand::create()
  {
    return PtrType(new CleanupHistoryCommand());
  }


  CleanupHistoryCommand::~CleanupHistoryCommand()
  {
  }


  ARSTD::Command::PtrType CleanupHistoryCommand::copy() const
  {
    return PtrType(new CleanupHistoryCommand(*this));
  }


  void CleanupHistoryCommand::initialize()
  {
  }


  void CleanupHistoryCommand::step()
  {
    getCommandProcessor()->cleanupHistory();
    setProgress(1);
    getCommandProcessor()->finished();
  }


  void CleanupHistoryCommand::cleanup()
  {
  }


  uint64 CleanupHistoryCommand::getRessourceID() const
  {
    return 0;
  }


  std::string CleanupHistoryCommand::getName() const
  {
    return "CleanupHistory";
  }


  std::string CleanupHistoryCommand::getDetails() const
  {
    return "Cleans up the history of the command processor context";
  }


  CleanupHistoryCommand::CleanupHistoryCommand()
  {
  }


  CleanupHistoryCommand::CleanupHistoryCommand(const CleanupHistoryCommand& other)
  : ARSTD::Command(other)
  {
  }

} //namespace ARSTD
