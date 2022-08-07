// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWIMessage.h"

namespace CodeSubWars
{

  /**
   * This is the specialized class for text messages.
   */
  class CSWTextMessage : public CSWIMessage
  {
    public:
      typedef std::shared_ptr<CSWTextMessage> PtrType;
  
      /**
       * Creates a new text message.
       * @param strMessage The text which should contain the message.
       */
      static PtrType create(const std::string& strMessage);

      virtual ~CSWTextMessage();

      virtual CSWIMessage::PtrType copy() const;
      virtual std::string getInformation() const;
    
      /**
       * Returns the text containing the message.
       */
      const std::string& getText() const;

    protected:
      CSWTextMessage(const std::string& strMessage);
      CSWTextMessage(const CSWTextMessage& other);

      std::string m_strMessage;
  };

}