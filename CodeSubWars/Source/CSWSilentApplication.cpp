// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSilentApplication.h"
#include "Constants.h"
#include "CSWWorld.h"
#include "CSWObject.h"
#include "CSWLog.h"
#include "CSWUtilities.h"


namespace CodeSubWars
{

  CSWSilentApplication::CSWSilentApplication(int argc, char** argv)
  : m_bParametersValid(true),
    m_WorldType(CSWWorld::DEFAULT_1),
    m_BattleType(CSWWorld::SINGLE),
    m_nTeamSize(3),
    m_fTimeStep(0.01)
  {
    checkRequirements();
  
    int nReadData = 0;
    for (int i = 0; i < argc - 1 && m_bParametersValid; ++i)
    {
      std::pair<ParameterType, std::string> result = determineParameterType(argv[i + 1]);
      switch (result.first)
      {
        case UNKNOWN:
        {
          m_bParametersValid = false;
          break;
        }
        case RUNNING_MODE:
        {
          m_bParametersValid &= result.second == "silent";        
          break;
        }
        case WORLD_TYPE:
        {
          int t = atoi(result.second.c_str());
          if (t >= 1 && t <= 5)
            m_WorldType = static_cast<CSWWorld::WorldType>(t);
          else
            m_bParametersValid = false;        
          break;
        }
        case BATTLE_TYPE:
        {
          if (result.second == "single")
            m_BattleType = CSWWorld::SINGLE;
          else if (result.second == "team")
            m_BattleType = CSWWorld::TEAM;
          else
            m_bParametersValid = false;
          break;
        }
        case TEAMSIZE_TYPE:
        {
          int t = atoi(result.second.c_str());
          if (t == 3 || t == 5 || t == 10)
            m_nTeamSize = t;
          else
            m_bParametersValid = false;        
          break;
        }
        case TIMESTEP_TYPE:
        {
          double t = atof(result.second.c_str());
          if (t >= 0.01 && t <= 0.1)
            m_fTimeStep = t;
          else
            m_bParametersValid = false;        
          break;
        }
      }
      nReadData |= result.first;
    }
  
    //at least running mode must given
    m_bParametersValid &= nReadData & RUNNING_MODE;
  }


  CSWSilentApplication::~CSWSilentApplication()
  {
  }


  void CSWSilentApplication::run()
  {
    std::cout << "CodeSubWars v" << Constants::getVersion().getAsString() << " beta\n";
    std::cout << "Copyright (c) 2005-2022 " << qPrintable(Constants::AUTHOR) << ". All rights reserved.\n";
    std::cout << "Released under the MIT license. (see license.txt)\n\n";

    if (!m_bParametersValid)
    {
      showSyntax();
      return;
    }

    std::cout.precision(2);
    std::cout.setf(std::ios::fixed);
  
    CSWLog::getInstance()->log("starting in silent mode ...");
  
    std::string strBattleType("unknown");
    if (m_BattleType == CSWWorld::SINGLE)
      strBattleType = "single";
    else if (m_BattleType == CSWWorld::TEAM)
      strBattleType = "team";
  
    std::stringstream ss;
    ss << "parameters: world = " << m_WorldType << " battle = " << strBattleType;
    if (m_BattleType == CSWWorld::TEAM)
      ss << " teamsize = " << m_nTeamSize;
    ss << " timestep = " << m_fTimeStep;
    CSWLog::getInstance()->log(ss.str());  
  
    //calculate average over last 5 seconds
    boost::circular_buffer<double> timeRatios(5.0/m_fTimeStep);
  
    CSWLog::getInstance()->log("initializing ...");
    CSWWorld::getInstance()->newWorld(m_WorldType);
  
    CSWWorld::getInstance()->newBattle(CSWUtilities::determineAvailableSubmarines("submarines"), 
                                       m_BattleType, m_nTeamSize, ARSTD::Time::MANUAL);
  
    double fOldRealTime = ARSTD::Time::getRealTime();
    double fStartRealTime = fOldRealTime;
  
    CSWLog::getInstance()->log("running ... (press ESC to stop)");
    std::cout << "\n";
    while (!_kbhit() && CSWWorld::getInstance()->isBattleRunning())
    {
      //recalculate the world
      CSWWorld::getInstance()->recalculate();

      //add time step to time
      ARSTD::Time::step(m_fTimeStep);
    
      //calculate average
      double fCurrentRealTime = ARSTD::Time::getRealTime();
      timeRatios.push_back(m_fTimeStep/(fCurrentRealTime - fOldRealTime));
      fOldRealTime = fCurrentRealTime;

      static int a = 0;
      ++a;
      if (a == 100) 
      {
        double fTimeRatio = std::accumulate(timeRatios.begin(), timeRatios.end(), 0) / timeRatios.capacity();
        std::cout << "\r Time: " << ARSTD::Time::getTime() 
                  << " TimeRatio: " << fTimeRatio 
                  << " Objects: " << CSWWorld::getInstance()->getObjectTree()->getTotalChildNumber() << "\t";
        a = 0;
      }
    }
    double fSimulatedTime = ARSTD::Time::getTime();
  
    if (CSWWorld::getInstance()->isBattleRunning())
    {
      std::cout << "\n\n";
      CSWLog::getInstance()->log("finalizing ...");
      CSWWorld::getInstance()->finalizeBattle();
      CSWWorld::getInstance()->finalizeWorld();
    }
    CSWLog::getInstance()->log(("total simulated time: " + 
                                QString::number(fSimulatedTime, 'f', 2) +
                                " ratio: " + QString::number(fSimulatedTime/(ARSTD::Time::getRealTime() - fStartRealTime), 'f', 2)).toStdString());
  }


  void CSWSilentApplication::checkRequirements()
  {

  
  }


  std::pair<CSWSilentApplication::ParameterType, std::string> CSWSilentApplication::determineParameterType(const std::string& value)
  {
    std::pair<ParameterType, std::string> result(UNKNOWN, "");
    if (value.size() <= 2)
      return result;
    if (value[0] != '-')
      return result;
  
    const std::string RUNNING_MODE_KEY = "silent";
    const std::string WORLD_KEY = "world";
    const std::string BATTLE_KEY = "battle";
    const std::string TEAMSIZE_KEY = "teamsize";
    const std::string TIMESTEP_KEY = "timestep";
  
    if (value.substr(1, value.size() - 1) == RUNNING_MODE_KEY)
    {
      result.first = RUNNING_MODE;
      result.second = value.substr(1, value.size() - 1);
      return result;
    }

    size_t nIdx = value.find(WORLD_KEY + "=");
    if (nIdx != std::string::npos)
    {
      result.first = WORLD_TYPE;
      result.second = value.substr(nIdx + WORLD_KEY.size() + 1, value.size() - (nIdx + WORLD_KEY.size() + 1));
      return result;
    }

    nIdx = value.find(BATTLE_KEY + "=");
    if (nIdx != std::string::npos)
    {
      result.first = BATTLE_TYPE;
      result.second = value.substr(nIdx + BATTLE_KEY.size() + 1, value.size() - (nIdx + BATTLE_KEY.size() + 1));
      return result;
    }

    nIdx = value.find(TEAMSIZE_KEY + "=");
    if (nIdx != std::string::npos)
    {
      result.first = TEAMSIZE_TYPE;
      result.second = value.substr(nIdx + TEAMSIZE_KEY.size() + 1, value.size() - (nIdx + TEAMSIZE_KEY.size() + 1));
      return result;
    }

    nIdx = value.find(TIMESTEP_KEY + "=");
    if (nIdx != std::string::npos)
    {
      result.first = TIMESTEP_TYPE;
      result.second = value.substr(nIdx + TIMESTEP_KEY.size() + 1, value.size() - (nIdx + TIMESTEP_KEY.size() + 1));
      return result;
    }
  
    return result;
  }


  void CSWSilentApplication::showSyntax()
  {
    std::cout << "A physics based three dimensional programming game.\n";
    std::cout << "\n";
    std::cout << "Syntax: CodeSubWars [-silent] [-world=<1-5>] [-battle=<single|team>] [-teamsize=<3|5|10>] [-timestep=x]\n";
    std::cout << "\n";
    std::cout << "  -silent     Using this parameter makes the application starts without graphical output.\n";
    std::cout << "              The other parameters are only valid when this is set.\n";
    std::cout << "\n";
    std::cout << "  -world      With this parameter the predefined world environment can be choosen. Default is 1.\n";
    std::cout << "\n";
    std::cout << "  -battle     This parameter indicates which battle mode should be run. Default is single.\n";
    std::cout << "\n";
    std::cout << "  -teamsize   This parameter defines the number of team member in team mode. Default is 3.\n";
    std::cout << "\n";
    std::cout << "  -timestep   Here the time step in seconds for each simulation step can be set. The given\n";
    std::cout << "              value must be in range [0.01, 0.1]. Default is 0.01.\n";
  }

}