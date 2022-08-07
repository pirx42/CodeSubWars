// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSensor.h"

namespace CodeSubWars
{

  class CSWEvent;

  /**
   * This class provides GPS like functionality.
   */
  class CSWGPS : public CSWSensor
  {
    public:
      typedef std::shared_ptr<CSWGPS> PtrType;

      static PtrType create(const std::string& strName, std::shared_ptr<CSWEvent> pEvent = std::shared_ptr<CSWEvent>());

      virtual ~CSWGPS();

      void setEvent(std::shared_ptr<CSWEvent> pEvent);
    
      /**
       * Returns the position of the sensor in world coordinates.
       */
      const Vector3D& getPosition() const;

    protected:
      CSWGPS(const std::string& strName, std::shared_ptr<CSWEvent> pEvent);

      std::shared_ptr<CSWEvent> m_pEvent;
  };

}