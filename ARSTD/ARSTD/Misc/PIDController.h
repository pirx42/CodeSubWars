// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace ARSTD
{

  class PIDController
  {
    public:
      PIDController(double fKp, double fKi, double fKd);
      ~PIDController();

      void setup(double fSetPoint);

      void setNewProcessValue(double fPV);

      double getControlValue() const;

    protected:
      void initialize(double fKp, double fKi, double fKd);
      void validate();

      double m_fKp;
      double m_fKi;
      double m_fKd;

      double m_fSetPoint;
      double m_fProcessValue;
      double m_fCurrentControlValue;
      double m_fEk_1;
      double m_fEk_2;

      double m_fOldTime;
      int m_nCalculations;
  };

} //namespace ARSTD
