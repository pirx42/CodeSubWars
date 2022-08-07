// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWWorld.h"


namespace CodeSubWars
{

  class CSWSilentApplication
  {
    public:

      CSWSilentApplication(int argc, char** argv);
      ~CSWSilentApplication();
    
      void run();
    
    protected:
      enum ParameterType
      {
        UNKNOWN = 0,
        RUNNING_MODE = 1,
        WORLD_TYPE = 4,
        BATTLE_TYPE = 8,
        TEAMSIZE_TYPE = 16,
        TIMESTEP_TYPE = 32
      };

      void checkRequirements();    
      std::pair<ParameterType, std::string> determineParameterType(const std::string& value);
    
      void showSyntax();
    
      bool m_bParametersValid;
      CSWWorld::WorldType m_WorldType;
      CSWWorld::BattleType m_BattleType;
      int m_nTeamSize;
      double m_fTimeStep;
  };

}