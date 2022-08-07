// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWUtilities.h"

namespace CodeSubWars
{

  class CSWObject;
  class CSWSettings;
  class CSWWorldGuard;
  class CSWBattleStatistics;
  class CSWExplosionVisualizer;
  class CSWSoundVisualizer;

  class CSWWorld
  {
    public:
      typedef std::shared_ptr<CSWWorld> PtrType;
    
      enum WorldType
      {
        DEFAULT_1 = 1,
        DEFAULT_2 = 2,
        DEFAULT_3 = 3,
        DEFAULT_4 = 4,
        DEFAULT_5 = 5,
        LOAD_WORLD
      };
    
      enum BattleType
      {
        SINGLE = 1,
        TEAM = 2
      };
    
      const static Size3D CUBE_SIZE;
      static const double CUBE_MARGIN;
      static const double CUBE_THICKNESS;

      static PtrType getInstance();

      virtual ~CSWWorld();

      DT_SceneHandle getScene();
      DT_RespTableHandle getResponseTable();
      DT_ResponseClass getResponseClass();

      void newWorld(WorldType type);
      void finalizeWorld();

      void newBattle(const CSWUtilities::SubmarineFileContainer& submarines, 
                     BattleType type, 
                     int nTeamSize,
                     ARSTD::Time::Mode timeMode = ARSTD::Time::AUTOMATIC);
      void finalizeBattle();

      PyThreadState* getPyMainState() const;

      std::shared_ptr<CSWObject> getObjectTree();
      const std::shared_ptr<CSWObject> getObjectTree() const;
      void setObjectTree(const std::shared_ptr<CSWObject> pObjectTree);

      bool isOutside(const std::shared_ptr<CSWObject> pObject) const;
      std::shared_ptr<CSWObject> getObject(const Vector3D& vecWorldTPosition, const double& fDistance) const;

      bool isBattleRunning() const;

      bool hasChanged(bool bResetModification = true);

      void draw();

      void recalculate();

      void store();
      
      std::shared_ptr<CSWSettings> getSettings();
      std::shared_ptr<CSWWorldGuard> getWorldGuard();
      std::shared_ptr<CSWBattleStatistics> getBattleStatistics();
      std::shared_ptr<CSWExplosionVisualizer> getExplosionVisualizer();
      std::shared_ptr<CSWSoundVisualizer> getSoundVisualizer();

      const std::vector<boost::tuples::tuple<std::string, double, double> >& getLoad() const;
      
    protected:
      CSWWorld();

      void setupEnvironment1();
      void setupEnvironment2();
      void setupEnvironment3();
      void setupEnvironment4();
      void setupEnvironment5();

      void attachObject(std::shared_ptr<CSWObject> pObject, bool bChangePosition = true);

      void calcWorldTransform();
    
      void loadSubmarines(const CSWUtilities::SubmarineFileContainer& submarines, int nTeamSize);

      void useNewRecordFileInPath(const std::string& strPath);
      void store(boost::iostreams::filtering_ostream& os);

      void rebuildReceiverContainer();
    
      template <typename ForwardIterator>
      std::pair<typename std::iterator_traits<ForwardIterator>::value_type, 
                typename std::iterator_traits<ForwardIterator>::value_type> 
      calcStatistics(ForwardIterator itBegin, ForwardIterator itEnd)
      {
        typedef std::iterator_traits<ForwardIterator>::value_type ValueType;
        ValueType mean = ValueType();
        ValueType max = ValueType();
        int nNumElements = 0;
        for (ForwardIterator it = itBegin; it != itEnd; ++it, ++nNumElements)
        {
          if (nNumElements < 25) mean += *it;
          if (max < *it) max = *it;
        }
        if (nNumElements) mean /= std::min(nNumElements, 25);
      
        return std::make_pair(mean, max);
      }



      DT_SceneHandle m_hDTScene;
      DT_RespTableHandle m_hDTRespTable;
      DT_ResponseClass m_DTResponseClass;
    
      bool m_bWorldInitialized;
      bool m_bBattleInitialized;
    
      QMutex m_mtxRecalc;
      QMutex m_mtxDraw;
    
      PyThreadState* m_pyMainState;
    
      std::shared_ptr<CSWObject> m_pObjectTree;
      std::vector<std::shared_ptr<CSWObject> > m_LoadedSubmarines;
      std::shared_ptr<CSWSettings> m_pSettings;
      std::shared_ptr<CSWWorldGuard> m_pWorldGuard;
      std::shared_ptr<CSWBattleStatistics> m_pBattleStatistics;
      std::shared_ptr<CSWExplosionVisualizer> m_pExplosionVisualizer;
      std::shared_ptr<CSWSoundVisualizer> m_pSoundVisualizer;
    
      std::vector<boost::tuples::tuple<std::string, double, double> > m_Load;
      BattleType m_BattleType;
    
      boost::iostreams::filtering_ostream m_RecordStream;
      double m_fLastStoredTime;
    
      std::vector<std::shared_ptr<CSWObject> > m_DynamicObjectContainer;
      std::vector<std::shared_ptr<CSWObject> > m_SoundReceiverObjectContainer;

      boost::circular_buffer<double> m_CalculateTimes;
      boost::circular_buffer<double> m_TransformCalculateTimes;
      boost::circular_buffer<double> m_RecalcTimes;
      boost::circular_buffer<double> m_UpdateProcessEventCalculateTimes;
      boost::circular_buffer<double> m_CollisionCalculateTimes;
  };

}
