// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWLog
  {
    public:
      typedef std::shared_ptr<CSWLog> PtrType;
    
      static PtrType getInstance();

      virtual ~CSWLog();

      void setLogOutput(QTextEdit* pTextEdit);
      void resetLogOutput();
      void log(const std::string& str);
    
    protected:
      CSWLog();

      QTextEdit* m_pTextEdit;
      std::fstream m_LogFile;
  };

}