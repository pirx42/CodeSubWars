// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWIMessage.h"

namespace CodeSubWars
{

  /**
   * This is the specialized class for system specific messages.
   */
  class CSWSystemMessage : public CSWIMessage
  {
    public:
      typedef std::shared_ptr<CSWSystemMessage> PtrType;
  
      static PtrType create(const std::string& strMessage);

      virtual ~CSWSystemMessage();

      virtual CSWIMessage::PtrType copy() const;
      virtual std::string getInformation() const;
    
      /**
       * Returns the text containing the message.
       */
      const std::string& getText() const;

    protected:
      CSWSystemMessage(const std::string& strMessage);
      CSWSystemMessage(const CSWSystemMessage& other);

      std::string m_strMessage;
  };

}