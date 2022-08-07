// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWObject;
  class CSWWeapon;

  class CSWBattleStatistics
  {
    public:
      typedef std::shared_ptr<CSWBattleStatistics> PtrType;


      static PtrType create();

      ~CSWBattleStatistics();

      void initialize();
    
      void reportWeaponFired(std::shared_ptr<CSWObject> pObject);
      void reportKill(std::shared_ptr<CSWObject> pKiller, std::shared_ptr<CSWObject> pKilledObject);
      void reportHit(std::shared_ptr<CSWObject> pObject, std::shared_ptr<CSWObject> pHitObject);
      void reportCollision(std::shared_ptr<CSWObject> pObjectA, std::shared_ptr<CSWObject> pObjectB);
      
      void write();
    
    protected:
      struct SubmarineEntry
      {
        SubmarineEntry()
        : fTimeKilled(-1), nNumFiredWeapons(0), nNumCausedHits(0), nNumHits(0), nNumCollisions(0)
        {
        }
    
        double fTimeKilled;
        //number of fired weapons
        int nNumFiredWeapons;
        //number of hits caused by self
        int nNumCausedHits;
        //number of hits to self
        int nNumHits;
        //number of collision of self
        int nNumCollisions;
      };

      struct KillEntry
      {
        double fTime;
        std::string strKilled;
      };
    
      //submarine name with regarding information
      typedef std::map<std::string, SubmarineEntry> SubmarineTable; 
      //killer submarine with regarding information
      typedef std::multimap<std::string, KillEntry> KillTable;
    
      CSWBattleStatistics();

      SubmarineTable m_SubmarineTable;
      KillTable m_KillTable;
  };

}