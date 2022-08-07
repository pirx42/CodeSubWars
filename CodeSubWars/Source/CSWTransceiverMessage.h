// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWIMessage.h"

namespace CodeSubWars
{

  /**
   * This is the specialized class for transceiver messages.
   */
  class CSWTransceiverMessage : public CSWIMessage
  {
    public:
      typedef std::shared_ptr<CSWTransceiverMessage> PtrType;
  
      static PtrType create(const std::string& strMessage);

      virtual ~CSWTransceiverMessage();

      virtual CSWIMessage::PtrType copy() const;
      virtual std::string getInformation() const;
    
      /**
       * Returns the text containing the message.
       */
      const std::string& getText() const;

    protected:
      CSWTransceiverMessage(const std::string& strMessage);
      CSWTransceiverMessage(const CSWTransceiverMessage& other);

      std::string m_strMessage;
  };

}