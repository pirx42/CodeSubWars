// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWObject;

  class CSWPyObjectLoader
  {
    public:

      ~CSWPyObjectLoader();
    
      static std::shared_ptr<CSWObject> createFromPyScript(const std::string& strAbsFileName, const std::string& strSubmarineName);

    protected:
      CSWPyObjectLoader();
  };

}