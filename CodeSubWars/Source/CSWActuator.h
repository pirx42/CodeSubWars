// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWEquipment.h"

namespace CodeSubWars
{

  /**
   * This is the base class for all actuators.
   */
  class CSWActuator : public CSWEquipment
  {
    public:
      typedef std::shared_ptr<CSWActuator> PtrType;

      virtual ~CSWActuator();

    protected:
      CSWActuator(const std::string& strName, const Matrix44D& matBaseTObject,
                  const double& fMaxAngle, const double& fAngularVelocity);
  };

}