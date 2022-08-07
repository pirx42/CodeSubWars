// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWPythonable
  {
    public:
      typedef std::shared_ptr<CSWPythonable> PtrType;

      static PtrType create();
      ~CSWPythonable();

      void setThreadState(PyThreadState* pThreadState);
      PyThreadState* getThreadState();

      void finalizePython();

    protected:
      CSWPythonable();

      PyThreadState* m_pThreadState;
    
  };

}