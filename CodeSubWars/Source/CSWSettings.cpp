// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSettings.h"
#include "CSWObject.h"
#include "CSWISolid.h"


namespace CodeSubWars
{

  CSWSettings::PtrType CSWSettings::create()
  {
    return PtrType(new CSWSettings());
  }


  CSWSettings::~CSWSettings()
  {
    m_PropertyTree.put("Options.Collision", m_nCollidableProperties);
    m_PropertyTree.put("Options.Damageable", m_nDamageableProperties);
    m_PropertyTree.put("Options.Dynamic", m_nDynamicProperties);
    m_PropertyTree.put("Options.Various", m_nVariousProperties);
    write_xml("preferences.cfg", m_PropertyTree);
  }


  CSWObject::PtrType CSWSettings::getSelectedObject()
  {
    return m_pSelectedObject.lock();
  }


  void CSWSettings::setSelectedObject(CSWObject::PtrType pObject)
  {
    m_pSelectedObject = pObject;
    m_pSelectedSolidObject = pObject->findFirstParent<CSWISolid>();
  }


  std::shared_ptr<CSWObject> CSWSettings::getSelectedSolidObject()
  {
    return m_pSelectedSolidObject.lock();
  }


  void CSWSettings::setCollidableProperties(int nProperties)
  {
    m_nCollidableProperties = nProperties;
  }


  int CSWSettings::getCollidableProperties() const
  {
    return m_nCollidableProperties;
  }


  void CSWSettings::setDamageableProperties(int nProperties)
  {
    m_nDamageableProperties = nProperties;
  }


  int CSWSettings::getDamageableProperties() const
  {
    return m_nDamageableProperties;
  }


  void CSWSettings::setDynamicProperties(int nProperties)
  {
    m_nDynamicProperties = nProperties;
  }


  int CSWSettings::getDynamicProperties() const
  {
    return m_nDynamicProperties;
  }


  void CSWSettings::setVariousProperties(int nProperties)
  {
    m_nVariousProperties = nProperties;
  }


  int CSWSettings::getVariousProperties() const
  {
    return m_nVariousProperties;
  }


  CSWSettings::CSWSettings()
  {
    try
    {
      read_xml("preferences.cfg", m_PropertyTree);
    }
    catch (...)
    {
    }

    m_nCollidableProperties = m_PropertyTree.get("Options.Collision", 2);
    m_nDamageableProperties = m_PropertyTree.get("Options.Damageable", 1);
    m_nDynamicProperties = m_PropertyTree.get("Options.Dynamic", 0);
    m_nVariousProperties = m_PropertyTree.get("Options.Various", 311);
  }


}