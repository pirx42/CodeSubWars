// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "arstd/Misc/PIDController.h"
#include "arstd/Misc/Time.h"

namespace ARSTD
{

  PIDController::PIDController(double fKp, double fKi, double fKd)
  {
    initialize(fKp, fKi, fKd);
  }


  PIDController::~PIDController()
  {
  }


  void PIDController::setup(double fSetPoint)
  {
    initialize(m_fKp, m_fKi, m_fKd);
    m_fSetPoint = fSetPoint;
  }


  void PIDController::setNewProcessValue(double fPV)
  {
    m_fProcessValue = fPV;
    validate();
  }


  double PIDController::getControlValue() const
  {
    return m_fCurrentControlValue;
  }


  void PIDController::initialize(double fKp, double fKi, double fKd)
  {
    m_fKp = fKp;
    m_fKi = fKi;
    m_fKd = fKd;

    m_fSetPoint = 0;
    m_fProcessValue = 0;
    m_fCurrentControlValue = 0;
    m_fEk_1 = 0;
    m_fEk_2 = 0;

    m_fOldTime = Time::getTime();
    m_nCalculations = 0;
  }

  
  void PIDController::validate()
  {
    double fCurrentTime = Time::getTime();
    double fElapsedTime = fCurrentTime - m_fOldTime;
    m_fOldTime = fCurrentTime;

    double fEk = m_fSetPoint - m_fProcessValue;

    double fPk = m_fKp*(fEk - m_fEk_1);
    double fIk = m_fKi*fEk*fElapsedTime;
    double fDk = 0;
    if (m_nCalculations < 2)
      ++m_nCalculations;
    else
      fDk = m_fKd*(fEk - 2*m_fEk_1 + m_fEk_2)/fElapsedTime;

    m_fCurrentControlValue += fPk + fIk + fDk;

    m_fEk_2 = m_fEk_1;
    m_fEk_1 = fEk;
  }

} //namespace ARSTD
