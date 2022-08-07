// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWDummy : public CSWObject
  {
    public:
      typedef std::shared_ptr<CSWDummy> PtrType;

      static PtrType create(const std::string& strName);

      virtual ~CSWDummy();

    protected:
      CSWDummy(const std::string& strName);
  };

}