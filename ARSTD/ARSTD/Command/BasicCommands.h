// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <memory>
#include "Command.h"


namespace ARSTD
{
  
  /**
   * This command makes commands executing again until the given number of cycles is reached.
   * Its behavior is depend on how the command is executed.\n
   * 1. The command is executed directly by the command processor: All previously _executed_ commands
   *    are executed again.\n
   * 2. The command is part of macro command: All previously _attached_ commands are executed again.
   *    That means the repeat command attached to a macro command repeats only commands of this command
   *    also if this macro is attached to another macro.\n
   *
   *    Example: two loops in one command plus a seperate repeat. 2 times 2 times 2 (8) torpedos with  
   *             litte pauses are fired. (in python)
   \code
   *     innerLoop = MacroCommand.create('InnerLoop')
   *     innerLoop.attach(CSWFireCommand.create(self.getFrontLeftWeaponBattery(), None))
   *     innerLoop.attach(CSWWaitCommand.create(2.5))
   *     innerLoop.attach(RepeatCommand.create(1))
   *
   *     outerLoop = MacroCommand.create('OuterLoop')
   *     outerLoop.attach(CSWWaitCommand.create(5))
   *     outerLoop.attach(innerLoop)
   *     outerLoop.attach(RepeatCommand.create(1))
   *
   *     self.getCommandProcessor().execute(outerLoop)
   *     self.getCommandProcessor().execute(RepeatCommand.create(1))
   \endcode
   */
  class RepeatCommand : public Command
  {
    public:
      typedef std::shared_ptr<RepeatCommand> PtrType;

      /**
       * Creates a new repeat command.
       * @param nCycles The number of cycles that all previous commands should repeat. If a negativ value is given
       *                the repeating does not terminate automatically.
       * @return Returns the new constructed command.
       */
      static PtrType create(int nCycles = -1);

      virtual ~RepeatCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      RepeatCommand(int nCycles);
      RepeatCommand(const RepeatCommand& other);
      
      int m_nCycles;
      int m_nCurrentCycles;
  };



  /**
   * This command makes the current command processor context beeing copied and stored to the top of a stack.
   */
  class PushCommand : public Command
  {
    public:
      typedef std::shared_ptr<PushCommand> PtrType;

      /**
       * Creates a new PushCommand command.
       * @return Returns the new constructed command.
       */
      static PtrType create();

      virtual ~PushCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      PushCommand();
      PushCommand(const PushCommand& other);
  };



  /**
   * This command makes the topmost stored command processor context from a stack beeing restored and used as current context.
   * If the stack is empty nothing will be done.
   * @warning All commands that are in queue after this command are not executed.
   */
  class PopCommand : public Command
  {
    public:
      typedef std::shared_ptr<PopCommand> PtrType;

      /**
       * Creates a new PopCommand command.
       * @return Returns the new constructed command.
       */
      static PtrType create();

      virtual ~PopCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      PopCommand();
      PopCommand(const PopCommand& other);
  };



  /**
   * This command makes the history (the list of executed commands) from the current command processor context beeing cleared.
   */
  class CleanupHistoryCommand : public Command
  {
    public:
      typedef std::shared_ptr<CleanupHistoryCommand> PtrType;

      /**
       * Creates a new CleanupHistoryCommand command.
       * @return Returns the new constructed command.
       */
      static PtrType create();

      virtual ~CleanupHistoryCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      CleanupHistoryCommand();
      CleanupHistoryCommand(const CleanupHistoryCommand& other);
  };

} //namespace ARSTD