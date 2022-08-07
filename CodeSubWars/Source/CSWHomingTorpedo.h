// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWTorpedo.h"

#include "CSWIUpdateable.h"

namespace CodeSubWars
{

  class CSWActiveSonar;

  /**
   * This class specializes a torpedo to an homing torpedo. This torpedo has an active sonar. Ones the torpedo has been released 
   * it checks in front whether a dynamic object exists. If so it modifies its main engine direction a little bit to move to the 
   * detected object.
   */
  class CSWHomingTorpedo : public CSWTorpedo,
                           public CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWHomingTorpedo> PtrType;

      virtual ~CSWHomingTorpedo();

      //defined methods for updateable
      virtual void update();

      //for debugging stuff
      virtual void draw();

    protected:
      CSWHomingTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength);

      std::shared_ptr<CSWActiveSonar> getActiveSonar();

      //sensors
      std::shared_ptr<CSWActiveSonar> m_pActiveSonar; 
  };

}