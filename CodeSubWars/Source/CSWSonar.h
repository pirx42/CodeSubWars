// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSensor.h"

namespace CodeSubWars
{

  class CSWMap;

  /**
   * This is the base class for sonar like functionality.
   * To cover regions the sonar can be rotated periodically.
   * This can be controlled by the three different mode types (direction, velocity and range).
   * The maximal angular velocity for adjusting is 180 degree/second.
   */
  class CSWSonar : public CSWSensor
  {
    public:
      typedef std::shared_ptr<CSWSonar> PtrType;

      /**
       * Available scanning directions and areas.
       */
      enum ScanDirectionMode
      {
        FULL = 1,             ///< Scans horizontal about 360 degree. (Default)
        FRONT = 2,            ///< Scans horizontal about 90 degree in forward direction. 
        BACK = 3,             ///< Scans horizontal about 90 degree in backward direction. 
        LOCAL_DIRECTION = 4,  ///< Scans horizontal about 30 degree in currently set custom direction.
        GLOBAL_POSITION = 5   ///< Scans horizontal about 30 degree in direction to
                              ///< the currently set global position.
      };
    
      /**
       * Available scanning velocities.
       */
      enum ScanVelocityMode
      {
        FAST = 1,       ///< Scans horizontal with 90 degree per second. (Default)
        SLOW = 2        ///< Scans horizontal with 20 degree per second.
      };
    
      /**
       * Available scanning ranges.
       */
      enum ScanRangeMode
      {                  
        NEAR_RANGE = 1, ///< Scans like described above. (Default)
        FAR_RANGE = 2   ///< Reduces the scaning area (horizontal) and the velocity (horizontal) by factor 4.
      };


      virtual ~CSWSonar();

      //defined methods from IUpdateable
      virtual void update();

      virtual void draw() = 0;
    
      virtual Vector3D getAdditionalDirection(const Vector3D& vecCurrentDir, const Vector3D& vecUp) const = 0;

      //sets the map to which the detected position reported
      void setMap(std::shared_ptr<CSWMap> pMap);

    
      /**
       * Enable the automatic rotation. If this is set to false the sonar can be set manually to desired direction with 
       * setDirection().
       * @param bValue If true the automatic rotation is activated otherwise not.
       */    
      void setEnableAutomaticRotation(bool bValue);
    
      /**
       * Returns whether the automatic rotation is enabled or not.
       * @return True if the automatic rotation is activated otherwise false.
       */    
      bool isAutomaticRotationEnabled() const;
    
      /**
       * Sets the scanning direction.
       * @param directionMode The new scanning direction that should be used.
       * @warning Mode changes causes direction changes.
       */    
      void setScanDirectionMode(int directionMode);

      /**
       * Sets the scanning velocity.
       * @param velocityMode The new scanning velocity that should be used.
       * @warning Mode changes causes direction changes.
       */    
      void setScanVelocityMode(int velocityMode);

      /**
       * Sets the scanning range.
       * @param rangeMode The new scanning range that should be used.
       * @warning Mode changes causes direction changes.
       */    
      void setScanRangeMode(int rangeMode);
    
      /**
       * Gets the current scanning direction mode.
       * @return Returns the current scanning direction mode.
       */    
      ScanDirectionMode getScanDirectionMode() const;
    
      /**
       * Gets the current scanning velocity mode.
       * @return Returns the current scanning velocity mode.
       */    
      ScanVelocityMode getScanVelocityMode() const;

      /**
       * Gets the current scanning range mode.
       * @return Returns the current scanning range mode.
       */    
      ScanRangeMode getScanRangeMode() const;
    
      /**
       * Sets the new custom main scanning direction for scan direction mode LOCAL_DIRECTION.
       * @param vecDirection The new custom main scanning direction in local coordinate system that should be used.
       * @warning This makes only sense in mode LOCAL_DIRECTION.
       */    
      void setScanDirection(const Vector3D& vecDirection);

      /**
       * Gets the current custom main scanning direction for scan direction mode LOCAL_DIRECTION.
       * @return Returns the current custom main scanning direction in local coordinate system.
       * @warning This makes only sense in mode LOCAL_DIRECTION.
       */    
      const Vector3D& getScanDirection() const;
    
      /**
       * Lets the sonar always points to the given position in world coordinate system for scan direction mode GLOBAL_POSITION.
       * @param vecPosition The global position to which the sonar should be directed.
       * @warning This makes only sense in mode GLOBAL_POSITION.
       */
      void pointToGlobalPosition(const Vector3D& vecPosition);
    
      void setDirection(const Vector3D& vecDirection);

      /**
       * Gets the current scanning direction in world coordinate system.
       * @return Returns the current scanning direction in world coordinate system.
       */    
      const Vector3D& getDirection() const;

      void setWorldDirection(const Vector3D& vecWorldTDirection);


    protected:
      CSWSonar(const std::string& strName, const Matrix44D& matBaseTObject);

      std::shared_ptr<CSWMap> getMap();
    
      static const double ANGLULAR_VELOCITY; //in rad/s
    
      static const double HORIZONTAL_SCANNING_ANGLE; //the angle that is scanned horizontaly
      static const double HORIZONTAL_ANGULAR_VELOCITY; //in degree per second;

      bool m_bAutomaticRotationEnabled;
      ScanDirectionMode m_ScanDirectionMode;
      ScanVelocityMode m_ScanVelocityMode;
      ScanRangeMode m_ScanRangeMode;
      double m_fActiveAutomaticRotationTimePeriod; //the time period of automatic rotation mode was beeing enabled
      double m_fOldTime; 

      Vector3D m_vecMainScanDirection;
      Vector3D m_vecMainScanUp;
      Vector3D m_vecWorldTPointToPosition;

    protected:
      //here to prevent wrong accessing. use getMap() instead.
      std::weak_ptr<CSWMap> m_pMap;
  };

}