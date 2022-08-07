// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWController;

  class CSWControlCommand : public ARSTD::Command
  {
    public:
      typedef std::shared_ptr<CSWControlCommand> PtrType;

      static PtrType create(std::shared_ptr<CSWController> pController);

      virtual ~CSWControlCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      CSWControlCommand(std::shared_ptr<CSWController> pController);
      CSWControlCommand(const CSWControlCommand& other);

      std::string m_strName;
      std::shared_ptr<CSWController> m_pController;
  };

}