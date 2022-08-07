// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSensor.h"

namespace CodeSubWars
{

  class CSWEvent;

  /**
   * This class provides functionality about moving.
   */
  class CSWMovingPropertiesSensor : public CSWSensor
  {
    public:
      typedef std::shared_ptr<CSWMovingPropertiesSensor> PtrType;

      static PtrType create(const std::string& strName, std::shared_ptr<CSWEvent> pEvent = std::shared_ptr<CSWEvent>());

      virtual ~CSWMovingPropertiesSensor();

      //defined methods from IUpdateable
      virtual void update();
    
      void setEvent(std::shared_ptr<CSWEvent> pEvent);

      /**
       * Returns the velocity in world coordinates in meter per second.
       */
      const Vector3D& getVelocity() const;
      /**
       * Returns the angular velocity in world coordinates in degree per second.
       */
      const Vector3D& getAngularVelocity() const;
      /**
       * Returns the angular velocity of the nose of the submarine in world coordinates in meter per second.
       */
      const Vector3D& getFrontAngularVelocity() const;
      /**
       * Returns the acceleration in world coordinates in meter per second^2.
       */
      const Vector3D& getAcceleration() const;
      /**
       * Returns the angular acceleration in world coordinates in degree per second^2.
       */
      const Vector3D& getAngularAcceleration() const;
      /**
       * Returns the angular acceleration of the nose of the submarine in world coordinates in meter per second^2.
       */
      const Vector3D& getFrontAngularAcceleration() const;


      /**
       * Returns the relative forward velocity in meter per second.
       */
      double getForwardVelocity() const;
      /**
       * Returns the relative upward velocity in meter per second.
       */
      double getUpVelocity() const;
      /**
       * Returns the local coordinates leftward velocity in local coordinates in meter per second.
       */
      double getLeftVelocity() const;
    
      /**
       * Returns the relative upward angular velocity in degree per second.
       */
      double getUpAngularVelocity() const;
      /**
       * Returns the relative leftward angular velocity in degree per second.
       */
      double getLeftAngularVelocity() const;
      /**
       * Returns the relative axial angular velocity in degree per second.
       */
      double getAxialAngularVelocity() const;

      /**
       * Returns the relative forward acceleration in meter per second^2.
       */
      double getForwardAcceleration() const;
      /**
       * Returns the relative upward acceleration in meter per second^2.
       */
      double getUpAcceleration() const;
      /**
       * Returns the relative leftward acceleration in meter per second^2.
       */
      double getLeftAcceleration() const;
      /**
       * Returns the relative upward angular acceleration in degree per second^2.
       */
      double getUpAngularAcceleration() const;
      /**
       * Returns the relative leftward angular acceleration in degree per second^2.
       */
      double getLeftAngularAcceleration() const;
      /**
       * Returns the relative axial angular acceleration in degree per second^2.
       */
      double getAxialAngularAcceleration() const;

    protected:
      CSWMovingPropertiesSensor(const std::string& strName, std::shared_ptr<CSWEvent> pEvent);

      void validate();

      bool m_bUpToDate;
  
      Vector3D m_vecWorldTVelocity;
      Vector3D m_vecWorldTAngularVelocity;
      Vector3D m_vecWorldTFrontAngularVelocity;
      Vector3D m_vecWorldTAcceleration;
      Vector3D m_vecWorldTAngularAcceleration;
      Vector3D m_vecWorldTFrontAngularAcceleration;
    
      Vector3D m_vecObjectTVelocity; 
      Vector3D m_vecObjectTAngularVelocity;
      Vector3D m_vecObjectTAcceleration;
      Vector3D m_vecObjectTAngularAcceleration;

      std::shared_ptr<CSWEvent> m_pEvent;
  };

}