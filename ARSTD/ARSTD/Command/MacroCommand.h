// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "Command.h"
#include "ICommandHandler.h"

namespace ARSTD
{

  /**
   * This class provides functionality for defining macro commands. Macro commands are a list of other commands. These commands
   * can also be MacroCommands. The order of the list is the same as the execution order.
   */
  class MacroCommand : public Command, public ICommandHandler
  {
    public:
      typedef std::shared_ptr<MacroCommand> PtrType;

      /**
      * Creates a new MacroCommand.
      * @param strName The name of the MacroCommand.
      */
      static PtrType create(const std::string& strName);

      virtual ~MacroCommand();

      virtual Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual ARSTD::uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

      virtual bool wasExecuted() const;
      virtual void resetProgress();
      virtual double getProgress() const;

      virtual void finished();
      virtual void repeat();
      virtual bool push();
      virtual bool pop();
      virtual void cleanupHistory();

      double getCurrentCommandProgress();
      std::string getCurrentCommandName();
      std::string getCurrentCommandDetails();

      /**
       * Attaches a command or MacroCommand.
       * @param pCommand The command that should be attached.
       */
      void attach(Command::PtrType pCommand);
      /**
       * Detaches a command or MacroCommand.
       * @param pCommand The command that should be detached.
       */
      void detach(Command::PtrType pCommand);

    protected:
      MacroCommand(const std::string& name);
      MacroCommand(const MacroCommand& other);

      CommandRange getAttachedCommands();

      std::string getName(int nHiearchy = 0) const;
      std::string getDetails(int nHiearchy = 0) const;

      CommandContainer m_CommandContainer;
      CommandIterator m_itCurrentExecutingCommand;
      std::string m_Name;
  };

} //namespace ARSTD