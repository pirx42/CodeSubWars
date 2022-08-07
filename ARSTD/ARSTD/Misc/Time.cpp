// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <windows.h>
#include <mmsystem.h>
#include <stdexcept>

#include "Time.h"

namespace ARSTD
{

  Time::PtrType Time::m_pInstance;


  void Time::setTimeRatio(double fRatio)
  {
    initialize();

    if (m_pInstance->m_Mode == MANUAL)
      return;

    LARGE_INTEGER currentCount;
    QueryPerformanceCounter(&currentCount);
    m_pInstance->m_fElapsedTimeBeforeChangeTimeRatio += (currentCount.QuadPart - m_pInstance->m_StartCount.QuadPart)*m_pInstance->m_fCurrentTimeRatio/
                                                        m_pInstance->m_PerformanceFrequency.QuadPart;
    m_pInstance->m_StartCount = currentCount;
    m_pInstance->m_fCurrentTimeRatio = fRatio;
  }


  double Time::getTimeRatio()
  {
    initialize();
    return m_pInstance->m_fCurrentTimeRatio;
  }

  
  double Time::getTime()
  {
    initialize();

    if (m_pInstance->m_Mode == MANUAL)
      return m_pInstance->m_fManualTime;

    LARGE_INTEGER currentCount;
    QueryPerformanceCounter(&currentCount);
    return m_pInstance->m_fElapsedTimeBeforeChangeTimeRatio + (currentCount.QuadPart - m_pInstance->m_StartCount.QuadPart)*m_pInstance->m_fCurrentTimeRatio/
           m_pInstance->m_PerformanceFrequency.QuadPart;
  }


  double Time::getRealTime()
  {
    initialize();

    LARGE_INTEGER currentCount;
    QueryPerformanceCounter(&currentCount);
    return static_cast<double>(currentCount.QuadPart - m_pInstance->m_RealStartCount.QuadPart)/m_pInstance->m_PerformanceFrequency.QuadPart;
  }

  
  void Time::step(double fDeltaTime)
  {
    initialize();

    if (m_pInstance->m_Mode != MANUAL)
      return;

    m_pInstance->m_fManualTime += fDeltaTime;
  }


  void Time::setTime(double fTime)
  {
    initialize();

    if (m_pInstance->m_Mode == MANUAL)
      return;

    m_pInstance->m_fElapsedTimeBeforeChangeTimeRatio = fTime;
  }


  void Time::reset(Mode mode, double fTime)
  {
    initialize();
    
    m_pInstance->resetLocal(mode, fTime);
  }


  Time::Time(double fRatio) 
  : m_fTimeRatio(fRatio),
    m_Mode(AUTOMATIC)
  {
    bool bResult = static_cast<bool>(QueryPerformanceCounter(&m_RealStartCount));
    bResult &= static_cast<bool>(QueryPerformanceFrequency(&m_PerformanceFrequency));
    if (!bResult)
      throw std::runtime_error("High performance counter in not supported!");

    resetLocal(AUTOMATIC, 0);
  }


  void Time::initialize()
  {
    if (!m_pInstance)
      m_pInstance = Time::PtrType(new Time());
  }


  void Time::resetLocal(Mode mode, double fTime)
  {
    m_Mode = mode;
    m_fManualTime = 0;
    m_fCurrentTimeRatio = m_fTimeRatio;
    m_fElapsedTimeBeforeChangeTimeRatio = fTime;

    QueryPerformanceCounter(&m_StartCount);
  }


} //namespace ARSTD