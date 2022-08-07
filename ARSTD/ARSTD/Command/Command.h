// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <string>
#include <memory>
#include "arstd/Common/Types.h"
#include "arstd/Command/ICommandHandler.h"


namespace ARSTD
{
  class CommandProcessor;
  class MacroCommand;

  /**
   * The base class of all Commands that can be executed by the CommandProcessor.
   * @warning An instance of a command must _not_ used more than ones. To execute the same command again copy it first.
   */
  class Command
  {
    public:
      typedef std::shared_ptr<Command> PtrType;

      virtual ~Command() {}

      /**
       * Returns a copy of the command.
       * @return The copied command.
       */
      virtual PtrType copy() const = 0;
      
      //This is called directly before execution (before the first step() is called).
      virtual void initialize() = 0;

      //This is called periodically about every 10ms while executing. If the command is done
      //finished() must be called. If finished() is never called the command never stops executing.
      virtual void step() = 0;

      //This is called directly after finishing of execution or on breaking.
      virtual void cleanup() = 0;
      
      //returns the or-combined ids of the used resources
      virtual ARSTD::uint64 getRessourceID() const { return 0; }

      /**
       * Returns the name of the command.
       * @return The name of the command.
       */
      virtual std::string getName() const = 0;

      //Returns some interesting stuff about the current command state.
      //@return The detailed state depend command description.
      virtual std::string getDetails() const { return "n.a."; }

      /**
       * Returns whether the command was already executed.
       * @return Returns true if the command was successfully executed otherwise false.
       *         If the progress has not reached 1 (or 100%) false is returned.
       */
      virtual bool wasExecuted() const { return m_fProgress >= 1; }
      
      //is called if repeating will be initialized
      virtual void resetProgress() { setProgress(0); }

      /**
       * Returns the progress of command execution.
       * @return The progress of the execution in range [0, 1].
       */
      virtual double getProgress() const { return m_fProgress; }
      
      friend CommandProcessor;
      friend MacroCommand;

    protected:
      Command() {}
      Command(const Command& other)
      : m_pCommandprocessor(other.m_pCommandprocessor),
        m_fProgress(other.m_fProgress)
      {
      }

      ICommandHandler* getCommandProcessor() const { return m_pCommandprocessor; }

      //Sets the progress of command execution.
      //@param fProgress The new progress of the execution in range [0, 1].
      void setProgress(double fProgress) 
      { 
        m_fProgress = fProgress; 
        if (m_fProgress < 0) m_fProgress = 0;
        if (m_fProgress > 1) m_fProgress = 1;
      }

      //This indicates the execution environment that the command is done.
      //@warning This method is only valid to call inside step().
      void finished() { m_pCommandprocessor->finished(); }


      ICommandHandler* m_pCommandprocessor;
      double m_fProgress;
  };

} //namespace ARSTD
