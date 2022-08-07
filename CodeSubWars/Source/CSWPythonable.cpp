// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWPythonable.h"
#include "CSWWorld.h"


namespace CodeSubWars
{

  CSWPythonable::PtrType CSWPythonable::create()
  {
    return PtrType(new CSWPythonable());
  }


  CSWPythonable::~CSWPythonable()
  {
  }


  void CSWPythonable::setThreadState(PyThreadState* pThreadState)
  {
    m_pThreadState = pThreadState;
  }


  PyThreadState* CSWPythonable::getThreadState()
  {
    return m_pThreadState;
  }


  void CSWPythonable::finalizePython()
  {
    //if (m_pThreadState) 
    //{
    //  //gil must not be held
    //  PyEval_AcquireThread(m_pThreadState);
    //  //gil held

    //  //PyEval_AcquireThread(state);
    //  Py_EndInterpreter(m_pThreadState);
    //  //gil held

    //  PyThreadState_Swap(CSWWorld::getInstance()->getPyMainState());
    //  //gil must be held
    //  PyEval_ReleaseThread(CSWWorld::getInstance()->getPyMainState());
    //  //gil not held
    //  m_pThreadState = NULL;
    //}
  }


  CSWPythonable::CSWPythonable()
  : m_pThreadState(NULL)
  {
  }


}