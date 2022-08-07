// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWController.h"

namespace CodeSubWars
{

  class CSWControlCenter;
  class CSWEngine;

  /**
   * 
   */
  class CSWAbsRotController : public CSWController
  {
    public:
      typedef std::shared_ptr<CSWAbsRotController> PtrType;
      typedef std::vector<std::shared_ptr<CSWEngine> > EngineContainer;
    
      enum Property
      {
        //what to control
        CONTROL_DIRECTION = 1,        //has the most priority; try to reach the given orientation.
        CONTROL_VELOCITY = 2,         //has always the second priority when given with DIRECTION

        //how to control      
        CONTROL_VELOCITY_RELATIVE = 8, //the given velocity is relative to the initial velocity
      
        //input parameter
        INTERPRET_AS_DIRECTION = 32,
        INTERPRET_AS_DIRECTION_ANGLES = 64,
        INTERPRET_AS_POSITION = 128,
        INTERPRET_AS_ABSOLUTE = 256,
        INTERPRET_AS_RELATIVE = 512
      };
    
    
      //direction stuff is in degree; positiv means cw
      //velocity stuff is in degree/s; positiv means ccw
      static PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter,
                            const EngineContainer& engines, 
                            const Vector3D& vecGPSTCenterOfMass,
                            const Vector3D& vecParameter, double fDirectionTolerance, //tolerance in degree
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperties = INTERPRET_AS_DIRECTION | INTERPRET_AS_ABSOLUTE | CONTROL_DIRECTION | CONTROL_VELOCITY,
                            double fMaxIntensity = 1.0);

      virtual ~CSWAbsRotController();

      virtual void initialize();

      virtual void update();
    
      virtual void finalize();

      virtual bool hasFinished();

      virtual const std::string& getName() const;
    
      virtual std::string getDetails() const;
    
      virtual double getProgress() const;

    protected:
      //assumes that the gps does not change to submarines center of mass over time
      //its not recommend to use a direction tolerance lower than 0.5 degree
      CSWAbsRotController(std::shared_ptr<CSWControlCenter> pControlCenter,
                          const EngineContainer& engines, 
                          const Vector3D& vecGPSTCenterOfMass,
                          const Vector3D& vecParameter, double fDirectionTolerance,
                          double fEndVelocity, double fVelocityTolerance, 
                          int nProperties, 
                          double fMaxIntensity);

      void determineRotationDestDirection(bool bInitialize);
                           
      std::string m_strName;
      int m_nProperties;
      bool m_bConditionReached;
  
      ARSTD::PIDController m_PIDController;
      std::shared_ptr<CSWControlCenter> m_pControlCenter;

      double m_fDirectionTolerance;
      double m_fInitialVelocity;    
      double m_fEndVelocity;        //end angular velocity of the front in m/s 
      double m_fVelocityTolerance;

      double m_fMaxIntensity;
      double m_fProgress;
    
      EngineContainer m_Engines;
      Vector3D m_vecGPSTCenterOfMass;
      Vector3D m_vecWorldTRotationAxis;
      Vector3D m_vecDir;
      Vector3D m_vecParameter;
  };

}