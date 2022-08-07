// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "PythonContext.h"
#include "CSWWorld.h"


namespace CodeSubWars
{

  PythonContext::MakeCurrentLock::MakeCurrentLock(PyThreadState* state)
  : m_State(state)
  {
    if (!m_State)
      return;

    //gil must not be held
    PyEval_AcquireThread(m_State);
    //gil held
    //std::cout << GetTickCount() << " " << (unsigned long long)m_State << " context acquired" << "\n";
  }

  PythonContext::MakeCurrentLock::~MakeCurrentLock()
  {
    if (!m_State)
      return;

    //gil must be held
    PyEval_ReleaseThread(m_State);
    //gil not held
    //std::cout << GetTickCount() << " " << (unsigned long long)m_State << " context released" << "\n";
  }

  std::vector<std::weak_ptr<PythonContext>> PythonContext::m_UsedPythonContexts;

  std::shared_ptr<PythonContext> PythonContext::create()
  {
    PtrType c(new PythonContext);
    c->initialize();
    m_UsedPythonContexts.push_back(c);
    return c;
  }

  std::shared_ptr<PythonContext> PythonContext::createEmpty()
  {
    PtrType c(new PythonContext);
    m_UsedPythonContexts.push_back(c);
    return c;
  }

  PythonContext::~PythonContext()
  {
    destroy();
  }

  void PythonContext::destroyAllRemaining()
  {
    for (auto& c : m_UsedPythonContexts)
    {
      if (PtrType pc = c.lock())
      {
        pc->destroy();
      }
    }
    m_UsedPythonContexts.clear();
  }

  PythonContext::MakeCurrentLock PythonContext::makeCurrent()
  {
    return MakeCurrentLock(m_State);
  }

  //void PythonContext::call(std::function<void(void)> f)
  //{
  //  if (m_State)
  //    return;

  //  //gil must not be held
  //  PyEval_AcquireThread(m_State);
  //  //gil held

  //  f();

  //  //gil must be held
  //  PyEval_ReleaseThread(m_State);
  //  //gil not held
  //}

  void PythonContext::destroy()
  {
    if (!m_State)
      return;

    //gil must not be held
    PyEval_AcquireThread(m_State);
    //gil held

    //std::cout << GetTickCount() << " " << std::hex << (unsigned long long)m_State << std::dec << " context destroyed\n";

    Py_EndInterpreter(m_State);
    m_State = nullptr;
    //gil held

    PyThreadState_Swap(CSWWorld::getInstance()->getPyMainState());
    //gil must be held
    PyEval_ReleaseThread(CSWWorld::getInstance()->getPyMainState());
    //gil not held
  }

  PythonContext::PythonContext()
  : m_State(nullptr)
  {
  }

  void PythonContext::initialize()
  {
    PyGILState_Ensure();
    //gil held

    // It maintains a separate interp (sub-interpreter) for each object.
    //gil must be held
    m_State = Py_NewInterpreter();
    //gil held

    //gil must be held
    PyThreadState_Swap(m_State);
    //gil held

    //gil must be held
    PyEval_ReleaseThread(m_State);
    //gil not held
    //std::cout << GetTickCount() << " " << std::hex << (unsigned long long)m_State << std::dec << " context created\n";
  }


}