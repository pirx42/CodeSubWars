// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWObject;

  class CSWWorldGuard
  {
    public:
      typedef std::shared_ptr<CSWWorldGuard> PtrType;

      enum ErrorType
      {
        CRITICAL = 1,   //object have some chances to avoid occured error. after x occured errors the object will be removed.
        FATAL = 2       //object must be removed imediately
      };

      static PtrType create();

      ~CSWWorldGuard();

      void update();
    
      void addErroneousObject(std::shared_ptr<CSWObject> pObject, ErrorType type);
      void addErroneousObject(const std::string& strName, ErrorType type);

      void removeErroneousObject(std::shared_ptr<CSWObject> pObject);

      bool isBattleRunning() const;

      void clear();    
    
    protected:
      typedef std::map<std::shared_ptr<CSWObject>, double> CriticalObjectContainer;  //object, error points
      typedef std::map<std::shared_ptr<CSWObject>, double> OutsideObjectContainer; //object, time inserted

      CSWWorldGuard();

      std::vector<std::shared_ptr<CSWObject> > getObjectsToKick(double fErrorThreshold);
  
      void determineBattleState();
    
      static double CRITICAL_OUTSIDE_TIME;
    
      CriticalObjectContainer m_CriticalObjects;
      OutsideObjectContainer m_ObjectsOutsideWorld; 
      bool m_bBattleRunning;
  };

}