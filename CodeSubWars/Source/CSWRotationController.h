// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWController.h"

namespace CodeSubWars
{

  class CSWControlCenter;
  class CSWEngine;

  //relative rotation: controls the orientation in local coordinates with given local destination orientation or velocity.
  class CSWRotationController : public CSWController
  {
    public:
      typedef std::shared_ptr<CSWRotationController> PtrType;
    
      enum Property
      {
        //what to control
        DIRECTION = 1,        //has the most priority; try to reach the given orientation.
        VELOCITY = 2,         //has always the second priority when given with DIRECTION

        //how to control      
        VELOCITY_RELATIVE = 8 //the given velocity is relative to the initial velocity
      };
    
    
      //direction stuff is in degree; positiv means cw
      //velocity stuff is in degree/s; positiv means ccw
      static PtrType create(std::shared_ptr<CSWControlCenter> pControlCenter,
                            std::shared_ptr<CSWEngine> pEngine, 
                            double fEndDirection, double fDirectionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperties = DIRECTION | VELOCITY,
                            double fMaxIntensity = 1.0);

      virtual ~CSWRotationController();

      virtual void initialize();

      virtual void update();
    
      virtual void finalize();

      virtual bool hasFinished();

      virtual const std::string& getName() const;
    
      virtual std::string getDetails() const;
    
      virtual double getProgress() const;

    protected:
      CSWRotationController(std::shared_ptr<CSWControlCenter> pControlCenter,
                            std::shared_ptr<CSWEngine> pEngine, 
                            double fEndDirection, double fDirectionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperties, 
                            double fMaxIntensity);
                          
      Vector3D calcCurrentAngles();                          

      std::string m_strName;
      int m_nProperties;
      bool m_bConditionReached;
  
      ARSTD::PIDController m_PIDController;
      std::shared_ptr<CSWControlCenter> m_pControlCenter;
      std::shared_ptr<CSWEngine> m_pEngine;

      Matrix33D m_matInitialOrientationTWorld;

      double m_fEndDirection;       //in degree
      double m_fAbsDestDirection;
      double m_fDirectionTolerance;
      double m_fInitialVelocity;    //in degree per second
      double m_fEndVelocity;        
      double m_fVelocityTolerance;
    
      double m_fMaxIntensity;
      double m_fProgress;
    
      int m_nNumRotationOffset;
  };

}