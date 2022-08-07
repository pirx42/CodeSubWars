// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSensor.h"

namespace CodeSubWars
{

  class CSWEvent;

  /**
   * This class provides Compass like functionality.
   */
  class CSWGyroCompass : public CSWSensor
  {
    public:
      typedef std::shared_ptr<CSWGyroCompass> PtrType;

      static PtrType create(const std::string& strName, std::shared_ptr<CSWEvent> pEvent = std::shared_ptr<CSWEvent>());

      virtual ~CSWGyroCompass();

      //defined methods from IUpdateable
      virtual void update();

    
      void setEvent(std::shared_ptr<CSWEvent> pEvent);

      /**
       * Returns the forward direction in world coordinates.
       */
      const Vector3D& getForwardDirection() const;

      /**
       * Returns the up direction in world coordinates.
       */
      const Vector3D& getUpDirection() const;
      //in world coords
      Matrix33D getOrientation() const;

      /**
       * Returns the absolute inclination of the sensor in degree [-90, 90]. (nose from down to up)
       * (rotation about x)
       */
      double getInclination(); 
    
      /**
       * Returns the absolute rotation of the sensor in degree [0, 360]. (nose from left to right)
       * (rotation about y)
       */
      double getDirection(); 
    
      /**
       * Returns the absolute axial inclination of the sensor in degree [-180, 180]. (turret from left to right)
       * (rotation about z)
       */
      double getAxialInclination(); 

    protected:
      CSWGyroCompass(const std::string& strName, std::shared_ptr<CSWEvent> pEvent);

      void validate();

      bool m_bUpToDate;

      double m_fDirection;
      double m_fInclination;
      double m_fAxialInclination;
      std::shared_ptr<CSWEvent> m_pEvent;
  };

}