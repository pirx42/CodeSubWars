// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * Objects with this property have a command processor that can handle executing of commands.
   */
  class CSWICommandable
  {
    public:
      typedef std::shared_ptr<CSWICommandable> PtrType;

      virtual ~CSWICommandable() {}

      virtual void step() = 0;
      //Returns the command processor.
      virtual ARSTD::CommandProcessor::PtrType getCommandProcessor() = 0;

  };

}