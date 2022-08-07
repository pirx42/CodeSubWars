// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <memory>
#include <vector>
#include "ICommandHandler.h"


namespace ARSTD
{

  class Command;
  class CommandProcessorContext;

  /**
   * This class encapsulates the management of commands. If commands are executed by the commandprocessor, they will be stored
   * and executed one after another till all done. 
   */
  class CommandProcessor : public ICommandHandler
  {
    public:
      typedef std::shared_ptr<CommandProcessor> PtrType;

      static PtrType create();

      ~CommandProcessor();

      /**
       * Execute the given command. If other commands are currently executed this command will be stored and executed next.
       * @param pCommand The command that will be added to the execution queue.
       */
      void execute(std::shared_ptr<Command> pCommand);

      virtual void finished();

      /**
       * Repeates all commands that were previously executed.
       */
      virtual void repeat();

      /**
       * Copies the current context to a stack.
       * @return True on success.
       */
      virtual bool push();

      /**
       * Restores the topmost context from a stack and makes it current.
       * @return True on success.
       */
      virtual bool pop();

      /**
       * Removes only executed commands that are stored.
       */
      virtual void cleanupHistory();
      
      void step();

      /**
       * Removes all (executed and waiting) stored commands from the queue.
       * @post isBusy() is false
       */
      void cleanup();

      //cleans up the stack and removes all (executed and waiting) stored commands from the queue.
      //@post isBusy() is false
      void cleanupStack();

      /**
       * Removes the currently executing command and do the next (if available).
       */
      void skipCurrent();
    
      /**
       * Returns true if the processor is currently executing a command otherwise false.
       * @return True if currently executing a command otherwise false.
       */
      bool isBusy();

      /**
       * Returns the progress of the currently executing command.
       * @return The progress of the executing command in range [0, 1]. If the processor is not busy -1 is returned.
       */
      double getCurrentCommandProgress() const;
      
      /**
       * Returns the name of the currently executing command.
       * @return The current executing command name. If the processor is not busy "" is returned.
       */
      std::string getCurrentCommandName() const;
      std::string getCurrentCommandDetails() const;

      /**
       * Returns the number of commands that currently waits of its execution.
       * @return The number of currently waiting commands.
       */
      unsigned int getNumWaitingCommands() const;
      
      ICommandHandler::CommandConstRange getWaitingCommands() const;

      unsigned int getCurrentStackDepth() const;

      unsigned int getNumExecutedCommands() const;

    protected:
      CommandProcessor();

      void executeNext();

      std::shared_ptr<CommandProcessorContext> m_pContext;
      std::vector<std::shared_ptr<CommandProcessorContext> > m_ContextStack;
  };

} //namespace ARSTD