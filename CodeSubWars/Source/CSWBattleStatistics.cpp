// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWLog.h"
#include "CSWWorld.h"
#include "CSWBattleStatistics.h"
#include "CSWObject.h"
#include "CSWSubmarine.h"
#include "CSWWeapon.h"
#include "CSWMessageCollectObjects.h"


namespace CodeSubWars
{

  CSWBattleStatistics::PtrType CSWBattleStatistics::create()
  {
    return PtrType(new CSWBattleStatistics());
  }


  CSWBattleStatistics::~CSWBattleStatistics()
  {
  }


  void CSWBattleStatistics::initialize()
  {
    m_SubmarineTable.clear();
    m_KillTable.clear();
  }


  void CSWBattleStatistics::reportWeaponFired(CSWObject::PtrType pObject)
  {
    //count only fired weapons from submarines
    if (!std::dynamic_pointer_cast<CSWSubmarine>(pObject))
      return;
  
    SubmarineTable::iterator itFound = m_SubmarineTable.find(pObject->getName());
    if (itFound == m_SubmarineTable.end())
    {
      SubmarineEntry e;
      e.nNumFiredWeapons = 1;
      m_SubmarineTable[pObject->getName()] = e;
    }
    else
    {
      ++(itFound->second.nNumFiredWeapons);
    }
  }


  void CSWBattleStatistics::reportKill(CSWObject::PtrType pKiller, CSWObject::PtrType pKilledObject)
  {
    //count only kills to submarines
    if (!std::dynamic_pointer_cast<CSWSubmarine>(pKilledObject))
      return;
  
    bool bWasShooterASubmarine = true;
    std::string strKiller = pKiller->getName();
    if (CSWWeapon::PtrType pWeapon = std::dynamic_pointer_cast<CSWWeapon>(pKiller))
    {
      bWasShooterASubmarine = pWeapon->isShooterSubmarine();
      strKiller = pWeapon->getShooterName();
    }
    //count only kills caused by other submarines
    if (!bWasShooterASubmarine)
      return;
  
    SubmarineTable::iterator itFound = m_SubmarineTable.find(pKilledObject->getName());
    if (itFound == m_SubmarineTable.end())
    {
      SubmarineEntry e;
      e.fTimeKilled = ARSTD::Time::getTime();
      m_SubmarineTable[pKilledObject->getName()] = e;
    }
    else
    {
      itFound->second.fTimeKilled = ARSTD::Time::getTime();
    }

    KillEntry e;
    e.fTime = ARSTD::Time::getTime();
    e.strKilled = strKiller == pKilledObject->getName() ? "self" : pKilledObject->getName();
    m_KillTable.insert(std::make_pair(strKiller, e));
  
    std::stringstream ss;
    ss.precision(2);
    ss.setf(std::ios::fixed);
    ss << "\t" << e.fTime << " " << strKiller << " killed " << e.strKilled;
    CSWLog::getInstance()->log(ss.str());
  }


  void CSWBattleStatistics::reportHit(CSWObject::PtrType pObject, CSWObject::PtrType pHitObject)
  {
    //count only hits to submarines
    if (!std::dynamic_pointer_cast<CSWSubmarine>(pHitObject))
      return;
  
    bool bWasShooterASubmarine = true;
    std::string strShooterName = pObject->getName();
    if (CSWWeapon::PtrType pWeapon = std::dynamic_pointer_cast<CSWWeapon>(pObject))
    {
      bWasShooterASubmarine = pWeapon->isShooterSubmarine();
      strShooterName = pWeapon->getShooterName();
    }
    //count only hits caused by other submarines
    if (!bWasShooterASubmarine)
      return;
  
    //count only hits which are not to itself
    if (strShooterName == pHitObject->getName())
      return;

    SubmarineTable::iterator itFound = m_SubmarineTable.find(strShooterName);
    if (itFound == m_SubmarineTable.end())
    {
      SubmarineEntry e;
      e.nNumCausedHits = 1;
      m_SubmarineTable[strShooterName] = e;
    }
    else
    {
      ++(itFound->second.nNumCausedHits);
    }

    itFound = m_SubmarineTable.find(pHitObject->getName());
    if (itFound == m_SubmarineTable.end())
    {
      SubmarineEntry e;
      e.nNumHits = 1;
      m_SubmarineTable[pHitObject->getName()] = e;
    }
    else
    {
      ++(itFound->second.nNumHits);
    }
  }


  void CSWBattleStatistics::reportCollision(CSWObject::PtrType pObjectA, CSWObject::PtrType pObjectB)
  {
    if (std::dynamic_pointer_cast<CSWSubmarine>(pObjectA))
    {
      SubmarineTable::iterator itFound = m_SubmarineTable.find(pObjectA->getName());
      if (itFound == m_SubmarineTable.end())
      {
        SubmarineEntry e;
        e.nNumCollisions = 1;
        m_SubmarineTable[pObjectA->getName()] = e;
      }
      else
      {
        ++(itFound->second.nNumCollisions);
      }
    }

    if (std::dynamic_pointer_cast<CSWSubmarine>(pObjectB))
    {
      SubmarineTable::iterator itFound = m_SubmarineTable.find(pObjectB->getName());
      if (itFound == m_SubmarineTable.end())
      {
        SubmarineEntry e;
        e.nNumCollisions = 1;
        m_SubmarineTable[pObjectB->getName()] = e;
      }
      else
      {
        ++(itFound->second.nNumCollisions);
      }
    }
  }


  void CSWBattleStatistics::write()
  {
    CSWLog::getInstance()->log("total ranking:");
  
    typedef CSWMessageCollectObjects1<CSWObject, CSWSubmarine> MessageCollectSubmarines;
    MessageCollectSubmarines collectSubmarinesMessage;
    MessageCollectSubmarines::broadcastMessage(CSWWorld::getInstance()->getObjectTree(), collectSubmarinesMessage);

    //log alive objects
    int nCnt = 1;
    MessageCollectSubmarines::ObjectContainer::const_iterator itAliveSubmarines = collectSubmarinesMessage.getCollectedObjects()[0].begin();
    MessageCollectSubmarines::ObjectContainer::const_iterator itEnd = collectSubmarinesMessage.getCollectedObjects()[0].end();
    nCnt += std::distance(itAliveSubmarines, itEnd);
    for (; itAliveSubmarines != itEnd; ++itAliveSubmarines)
    {
      std::stringstream ss;
      ss << "\t1. " << (*itAliveSubmarines)->getName();
      CSWLog::getInstance()->log(ss.str());
    }
  
  
    if (!m_KillTable.empty())
    {
      std::map<double, std::string> tmp;
      std::multimap<std::string, KillEntry>::const_iterator itLogData = m_KillTable.begin();
      for (; itLogData != m_KillTable.end(); ++itLogData)
      {
        std::string strName;
        if (itLogData->second.strKilled == "self")
          strName = itLogData->first;
        else strName = itLogData->second.strKilled;
        tmp[itLogData->second.fTime] = strName;
      }
    
      std::map<double, std::string>::reverse_iterator rit = tmp.rbegin();
      for (; rit != tmp.rend(); ++rit)
      {
        assert(rit->second != "self");
        std::stringstream ss;
        ss << "\t" << nCnt << ". " << rit->second;
        CSWLog::getInstance()->log(ss.str());
        ++nCnt;
      }

    
    
      CSWLog::getInstance()->log("ranking by kills:");
    
      std::multimap<int, std::string> tmp2;
      std::pair<std::multimap<std::string, KillEntry>::const_iterator,
                std::multimap<std::string, KillEntry>::const_iterator> range;

      range.second = m_KillTable.begin();
      while (range.second != m_KillTable.end())
      {
        int nNumKills = 0;
        range = m_KillTable.equal_range(range.second->first);
        for (itLogData = range.first; itLogData != range.second; ++itLogData)
        { 
          if (itLogData->second.strKilled != "self")
            ++nNumKills;
        }
        if (nNumKills > 0)
          tmp2.insert(std::make_pair(nNumKills, range.first->first));
      }

      std::multimap<int, std::string>::reverse_iterator ritTmp = tmp2.rbegin();
      for (int nCnt = 1; ritTmp != tmp2.rend(); ++ritTmp, ++nCnt)
      {
        SubmarineTable::const_iterator itFound = m_SubmarineTable.find(ritTmp->second);
        assert(itFound != m_SubmarineTable.end());
        double fTime = itFound->second.fTimeKilled < 0 ? ARSTD::Time::getTime() : itFound->second.fTimeKilled;

        std::stringstream ss;
        ss.precision(2);
        ss.setf(std::ios::fixed);
        ss << "\t" << nCnt << ". " << ritTmp->second << " has " << ritTmp->first << " kills";
        if (ritTmp->first > 0)
          ss << " (" << ritTmp->first*60.0/fTime << " kills per minute)";
        CSWLog::getInstance()->log(ss.str());
      }
    }
  
  
    if (!m_SubmarineTable.empty())
    {
      CSWLog::getInstance()->log("ranking by hits:");
      std::multimap<int, std::string> tmp;
      SubmarineTable::const_iterator it = m_SubmarineTable.begin();
      for (; it != m_SubmarineTable.end(); ++it)
      {
        if (it->second.nNumCausedHits > 0)
          tmp.insert(std::make_pair(it->second.nNumCausedHits, it->first));
      }

      std::multimap<int, std::string>::reverse_iterator ritTmp = tmp.rbegin();
      for (int nCnt = 1; ritTmp != tmp.rend(); ++ritTmp, ++nCnt)
      {
        SubmarineTable::const_iterator itFound = m_SubmarineTable.find(ritTmp->second);
        assert(itFound != m_SubmarineTable.end());
        double fTime = itFound->second.fTimeKilled < 0 ? ARSTD::Time::getTime() : itFound->second.fTimeKilled;

        std::stringstream ss;
        ss.setf(std::ios::fixed);
        ss << "\t" << nCnt << ". " << ritTmp->second << " has " << itFound->second.nNumCausedHits << " hits";
        ss << " (" << itFound->second.nNumFiredWeapons << " fired, ";
        ss.precision(1);
        if (itFound->second.nNumFiredWeapons > 0)
          ss << itFound->second.nNumCausedHits*100.0/itFound->second.nNumFiredWeapons << "% hit rate, ";
        ss.precision(2);
        ss << itFound->second.nNumCausedHits*60.0/fTime << " hits per minute)";
        CSWLog::getInstance()->log(ss.str());
      }
    }
  }


  CSWBattleStatistics::CSWBattleStatistics()
  {
  }

}