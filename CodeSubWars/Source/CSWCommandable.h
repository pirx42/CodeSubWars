// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWCommandable
  {
    public:
      typedef std::shared_ptr<CSWCommandable> PtrType;

      static PtrType create();
      ~CSWCommandable();

      void step();

      ARSTD::CommandProcessor::PtrType getCommandProcessor();

    protected:
      CSWCommandable();

      ARSTD::CommandProcessor::PtrType m_pCommandProcessor;
  };

}