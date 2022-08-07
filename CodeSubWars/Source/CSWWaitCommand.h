// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * This is a specialization of a command. It provides the functionality to let the execution waits a given period.
   */
  class CSWWaitCommand : public ARSTD::Command
  {
    public:
      typedef std::shared_ptr<CSWWaitCommand> PtrType;

      /**
       * Creates a new wait command.
       * @param fWaitTime The time in seconds that should be waited.
       * @return Returns the new constructed command.
       */
      static PtrType create(double fWaitTime);

      virtual ~CSWWaitCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      CSWWaitCommand(double fWaitTime);
      CSWWaitCommand(const CSWWaitCommand& other);

      std::string m_strName;
      double m_fWaitTime;
      double m_fBeginTime;
  };

}