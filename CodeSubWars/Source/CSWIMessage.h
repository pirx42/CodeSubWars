// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * This is the base class for all messages.
   */
  class CSWIMessage
  {
    public:
      typedef std::shared_ptr<CSWIMessage> PtrType;

      virtual ~CSWIMessage() {}
    
      virtual PtrType copy() const = 0;
    
      virtual std::string getInformation() const = 0;
  };

}