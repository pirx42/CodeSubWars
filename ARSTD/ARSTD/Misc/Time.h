// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <windows.h>
#include <memory>

namespace ARSTD
{

  class Time
  {
    public:
      typedef std::shared_ptr<Time> PtrType;
      
      enum Mode
      {
        AUTOMATIC = 1,
        MANUAL = 2
      };

      static void setTimeRatio(double fRatio);
      static double getTimeRatio();
      
      static double getTime();
      static double getRealTime();

      static void setTime(double fTime);

      static void step(double fDeltaTime);

      static void reset(Mode mode = AUTOMATIC, double fTime = 0);
      
    protected:
      Time(double fRatio = 1.0);

      static void initialize();
      void resetLocal(Mode mode, double fTime);

      static PtrType m_pInstance;
      LARGE_INTEGER m_StartCount;
      LARGE_INTEGER m_RealStartCount;
      LARGE_INTEGER m_PerformanceFrequency;
      
      Mode m_Mode;
      double m_fManualTime;
      double m_fTimeRatio;
      double m_fCurrentTimeRatio;
      double m_fElapsedTimeBeforeChangeTimeRatio;
  };

} //namespace ARSTD