// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "boost/weak_ptr.hpp"

namespace CodeSubWars
{

  class CSWObject;

  class CSWSettings
  {
    public:
      typedef std::shared_ptr<CSWSettings> PtrType;

      enum CollidableProperties
      {
        DISPLAY_COLLISION_MESH = 1,
        DISPLAY_OBJECT_OBJECT_COLLISION = 2,
        DISPLAY_OBJECT_TRIGGER_COLLISION = 4,
        DISPLAY_TRIGGER_TRIGGER_COLLISION = 8
      };

      enum DamageableProperties
      {
        DISPLAY_HEALTH = 1,
      };

      enum DynamicProperties
      {
        DISPLAY_TOTAL_FORCE = 1,
        DISPLAY_DETAIL_FORCE = 2,
        DISPLAY_TOTAL_IMPULS = 4,
        DISPLAY_DETAIL_IMPULS = 8,
        DISPLAY_MASS_POINTS = 16
      };
    
      enum VariousProperties
      {
        INITIALZE_SUBS_IN_PLANE = 1,
        DISPLAY_ACTIVE_SONAR_SCAN = 2,
        DISPLAY_PASSIVE_SONAR_SCAN = 4,
        DISPLAY_SOUND_EMITTING = 8,
        INCREASE_VISIBILITY_OF_SMALL_OBJECTS = 16,
        STORE_WORLD_PERIODICALLY = 32,
        DISPLAY_OBJECT_TREE = 64,
        DISPLAY_WEAPONS_IN_OBJECT_TREE = 128,
        AUTOMATIC_BATTLE_STOP = 256
      };



      static PtrType create();

      ~CSWSettings();

      std::shared_ptr<CSWObject> getSelectedObject();
      void setSelectedObject(std::shared_ptr<CSWObject> pObject);

      std::shared_ptr<CSWObject> getSelectedSolidObject();

      void setCollidableProperties(int nProperties);
      int getCollidableProperties() const;

      void setDamageableProperties(int nProperties);
      int getDamageableProperties() const;

      void setDynamicProperties(int nProperties);
      int getDynamicProperties() const;

      void setVariousProperties(int nProperties);
      int getVariousProperties() const;
    
    protected:
      CSWSettings();

      boost::property_tree::ptree m_PropertyTree;

      std::weak_ptr<CSWObject> m_pSelectedObject;
      std::weak_ptr<CSWObject> m_pSelectedSolidObject;
      int m_nCollidableProperties;
      int m_nDamageableProperties;
      int m_nDynamicProperties;
      int m_nVariousProperties;
  };

}