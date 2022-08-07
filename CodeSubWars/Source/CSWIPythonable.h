// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class PythonContext;

  /**
   * Objects with this property can be specialized by programming language python.
   */
  class CSWIPythonable
  {
    public:
      typedef std::shared_ptr<CSWIPythonable> PtrType;

      virtual ~CSWIPythonable() {}

      //initializes the python environment (new interpreter, thread state)
      //virtual void setThreadState(PyThreadState* pThreadState) = 0;
      //virtual PyThreadState* getThreadState() = 0;

      ////finalizes the python enviroment
      //virtual void finalizePython() = 0;

      virtual std::shared_ptr<PythonContext> getPythonContext() = 0;
      virtual void setPythonContext(std::shared_ptr<PythonContext> context) = 0;
  };

}