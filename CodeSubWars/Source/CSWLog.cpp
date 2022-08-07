// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWLog.h"
#include <arstd/misc/Time.h>


namespace CodeSubWars
{

  CSWLog::PtrType CSWLog::getInstance()
  {
    static PtrType pInstance = PtrType(new CSWLog());
    return pInstance;
  }


  CSWLog::~CSWLog()
  {
    QString s = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss.zzz")) + "   application closed\n"; 
    m_LogFile << s.toStdString();
  }


  void CSWLog::setLogOutput(QTextEdit* pTextEdit)
  {
    m_pTextEdit = pTextEdit;
  }


  void CSWLog::resetLogOutput()
  {
    m_pTextEdit = NULL;
  }


  void CSWLog::log(const std::string& str)
  {
    QString s = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss.zzz")) + QString("   ") + str.c_str(); 
    if (m_pTextEdit)
      m_pTextEdit->append(s);
    else
      std::cout << s.toStdString() << "\n";

    m_LogFile << s.toStdString() << "\n";
    
  }


  CSWLog::CSWLog()
  : m_pTextEdit(NULL)
  {
    QDir().mkdir("log");
    m_LogFile.open("log/events.log", std::ios::out | std::ios::app);
    assert(m_LogFile.is_open());
  
    QString s = QDateTime::currentDateTime().toString(QString("yyyy/MM/dd hh:mm:ss.zzz")) + "   application started\n"; 
    m_LogFile << s.toStdString();
  }

}