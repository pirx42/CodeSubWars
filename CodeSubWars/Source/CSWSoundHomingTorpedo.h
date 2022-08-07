// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWTorpedo.h"

#include "CSWIUpdateable.h"

namespace CodeSubWars
{

  class CSWPassiveSonar;

  /**
   * This class specializes a torpedo to a sound based homing torpedo. This torpedo has a passive sonar. Ones the torpedo has been released 
   * it checks in front whether a sound is emitted. If so it modifies its main engine direction a little bit to move to the 
   * detected direction.
   */
  class CSWSoundHomingTorpedo : public CSWTorpedo,
                                public CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWSoundHomingTorpedo> PtrType;

      virtual ~CSWSoundHomingTorpedo();

      //defined methods for updateable
      virtual void update();

      //for debugging stuff
      virtual void draw();

    protected:
      CSWSoundHomingTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      std::shared_ptr<CSWPassiveSonar> getPassiveSonar();

      //sensors
      std::shared_ptr<CSWPassiveSonar> m_pPassiveSonar;
  };

}