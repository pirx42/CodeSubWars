// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * This base class specializes commands to stateless commands. These command must not be initialized or finalized.
   */
  class CSWExecuteCommand : public ARSTD::Command
  {
    public:
      typedef std::shared_ptr<CSWExecuteCommand> PtrType;

      virtual ~CSWExecuteCommand();

      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      CSWExecuteCommand(const std::string& strName);
      CSWExecuteCommand(const CSWExecuteCommand& other);

      virtual void run() = 0;

      std::string m_strName;
      bool m_bExecuted;
  };

}