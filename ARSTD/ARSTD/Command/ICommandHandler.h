// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <deque>
#include <memory>

namespace ARSTD
{

  class Command;

  class ICommandHandler
  {
    public:
      typedef std::shared_ptr<ICommandHandler> PtrType;
      typedef std::deque<std::shared_ptr<Command> > CommandContainer;
      typedef CommandContainer::iterator CommandIterator;
      typedef CommandContainer::const_iterator CommandConstIterator;
      typedef std::pair<CommandIterator, CommandIterator> CommandRange;
      typedef std::pair<CommandConstIterator, CommandConstIterator> CommandConstRange;

      virtual ~ICommandHandler() {}

      //callback method that will be executed by the executing command to report finishing of itself
      virtual void finished() = 0;

      //Repeates all commands that were previously executed.
      virtual void repeat() = 0;

      //these methods are routed through to the "real" command processor if call in macrocommand
      virtual bool push() = 0;
      virtual bool pop() = 0;
      virtual void cleanupHistory() = 0;

  };
}