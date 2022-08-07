// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <memory>
#include "ICommandHandler.h"


namespace ARSTD
{

  class Command;
  class CommandProcessor;

  //the context that is mainly managed through a command processor
  class CommandProcessorContext
  {
    public:
      typedef std::shared_ptr<CommandProcessorContext> PtrType;

      //creates a new empty context
      static PtrType create();

      ~CommandProcessorContext();

      //makes a deep copy
      PtrType copy();

      //adds a command to the end
      bool addCommand(std::shared_ptr<Command> pCommand);
      
      //returns the command the is currently executing. returns NULL if no command is currently executing.
      std::shared_ptr<Command> getCurrentExecutingCommand() const;

      //empties the whole context
      void cleanup();

      //empties only the executed command container
      void cleanupHistory();
      
      //sets the commandprocessor to all commands
      void setCommandProcessor(CommandProcessor* pCommandProcessor);

      //returns the number of executed commands
      int getNumExecutedCommands() const;
      
      //returns the number of waiting commands
      int getNumWaitingCommands() const;

      //returns a range of all waiting commands
      ICommandHandler::CommandRange getWaitingCommands();
      ICommandHandler::CommandConstRange getWaitingCommands() const;      

      //makes the current executing command to be the next available
      void incrementCurrentExecutingCommand();
      
      //marks all commands to not be executed
      void setAllCommandsToNotExecuted();
      
    protected:
      CommandProcessorContext();
      CommandProcessorContext(const CommandProcessorContext& other);
      
      ICommandHandler::CommandContainer m_ExecutedCommands;
      std::shared_ptr<Command> m_pCurrentExecutingCommand;
      ICommandHandler::CommandContainer m_WaitingCommands;
  };

} //namespace ARSTD