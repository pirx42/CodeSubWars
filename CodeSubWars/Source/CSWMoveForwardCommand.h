// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "ARSTD/Command/Command.h"
#include "ARSTD/Misc/PIDController.h"

namespace CodeSubWars
{

  class CSWEngine;

  class CSWControlCenter;


  class CSWMoveForwardCommand : public ARSTD::Command
  {
    public:
      typedef std::shared_ptr<CSWMoveForwardCommand> PtrType;

      enum Property
      {
        //what to control
        POSITION = 1,
        VELOCITY = 2,
      };

      //position stuff is in m; positiv means forward
      //velocity stuff is in m/s; positiv means forward
      static PtrType create(std::shared_ptr<CSWControlCenter>& pControlCenter,
                            std::shared_ptr<CSWEngine>& pEngine, 
                            double fSetRelativePosition, double fPositionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperty = POSITION | VELOCITY,
                            double fMaxIntensity = 1.0);

      static PtrType create(std::shared_ptr<CSWControlCenter>& pControlCenter,
                            std::shared_ptr<CSWEngine>& pEngine, 
                            const Vector3D& vecPos, double fPositionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperty = POSITION | VELOCITY,
                            double fMaxIntensity = 1.0);

      virtual ~CSWMoveForwardCommand();

      virtual ARSTD::Command::PtrType copy() const;
      virtual void initialize();
      virtual void step();
      virtual void cleanup();
      virtual uint64 getRessourceID() const;
      virtual std::string getName() const;
      virtual std::string getDetails() const;

    protected:
      CSWMoveForwardCommand(std::shared_ptr<CSWControlCenter> pControlCenter,
                            std::shared_ptr<CSWEngine> pEngine, 
                            double fSetRelativePosition, double fPositionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperty, double fMaxIntensity);

      CSWMoveForwardCommand(std::shared_ptr<CSWControlCenter> pControlCenter,
                            std::shared_ptr<CSWEngine> pEngine, 
                            const Vector3D& vecPos, double fPositionTolerance,
                            double fEndVelocity, double fVelocityTolerance, 
                            int nProperty, double fMaxIntensity);

      CSWMoveForwardCommand(const CSWMoveForwardCommand& other);

      std::string m_strName;
      ARSTD::PIDController m_PIDController;
      std::weak_ptr<CSWControlCenter> m_pControlCenter;
      std::weak_ptr<CSWEngine> m_pEngine;
      double m_fSetRelativePosition;
      double m_fPositionTolerance;
      Vector3D m_vecAbsolutFromPosition;
      double m_fInitialVelocity;
      double m_fEndVelocity;
      double m_fVelocityTolerance;
      double m_fMaxIntensity;
      int m_nProperties;
      bool m_bConditionReached;
      bool m_bUsePosition;
      Vector3D m_vecPos;
  };

}