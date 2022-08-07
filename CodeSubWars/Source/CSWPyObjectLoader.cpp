// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWPyObjectLoader.h"
#include "CSWLog.h"

#include "CSWObject.h"
#include "CSWIPythonable.h"
#include "PythonContext.h"


namespace CodeSubWars
{

  CSWObject::PtrType CSWPyObjectLoader::createFromPyScript(const std::string& strAbsFileName, const std::string& strSubmarineName)
  {
    CSWObject::PtrType pObject;

    PythonContext::PtrType pyContext = PythonContext::create();
    //PyGILState_Ensure();
    ////gil held

    //// It maintains a separate interp (sub-interpreter) for each object.
    ////gil must be held
    //PyThreadState* pThreadState = Py_NewInterpreter();
    ////gil held

    ////gil must be held
    //PyThreadState_Swap(pThreadState);
    ////gil held

    ////gil must be held
    //PyEval_ReleaseThread(pThreadState);
    ////gil not held

  {
    auto pyMakeCurrentLock = pyContext->makeCurrent();
    //---------------------------------
    ////gil must not be held
    //PyEval_AcquireThread(pThreadState);
    ////gil held


    wchar_t pBuff[] = L"CodeSubWars.exe";
    wchar_t* Argv[1];
    Argv[0] = pBuff;
    PySys_SetArgv(1, Argv);
  
    try
    {
      // Create new dictionary with python builtin functions. So each python object is executed in its own environment.
      // Therefor it is not possible to call a global function defined in another object code.
      PyObject* pDict = PyDict_New();
      PyDict_SetItemString(pDict, "__builtins__", PyEval_GetBuiltins());

      // Define the derived class in Python.
      std::string strExec("exec(open(\'" + strAbsFileName + "\').read())");
      boost::python::handle<> result(PyRun_String(strExec.c_str(), Py_file_input, pDict, pDict));

      // Result is not needed
      result.reset();

      // Extract the raw Python object representing the just defined derived class
      boost::python::handle<> class_ptr(PyRun_String((strSubmarineName + "\n").c_str(), Py_eval_input, pDict, pDict));

      // Wrap the raw Python object in a Boost.Python object
      boost::python::object PythonDerived(class_ptr);

      // But now creating and using instances of the Python class is almost
      // as easy!
      boost::python::object py_base = PythonDerived();
    
      pObject = boost::python::extract<std::shared_ptr<CSWObject> >(py_base);
      if (CSWIPythonable::PtrType pPyObj = std::dynamic_pointer_cast<CSWIPythonable>(pObject))
      {
        //pPyObj->setThreadState(pThreadState);
        pPyObj->setPythonContext(pyContext);
      }
      else
      {
        CSWLog::getInstance()->log(pObject->getName() + " is no pythonable object -> skipped");
        pObject.reset();
      }
    }
    catch (...)
    {
      if (PyErr_Occurred())
        PyErr_Print();
    }

    ////gil must be held
    //PyEval_ReleaseThread(pThreadState);
    ////gil not held
  }
    return pObject;
  }

}