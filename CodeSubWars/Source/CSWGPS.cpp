// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWGPS.h"
#include "CSWWorld.h"
#include "CSWEvent.h"
#include "CSWEventManager.h"

namespace CodeSubWars
{

  CSWGPS::PtrType CSWGPS::create(const std::string& strName, CSWEvent::PtrType pEvent)
  {
    return PtrType(new CSWGPS(strName, pEvent));
  }


  CSWGPS::~CSWGPS()
  {
  }


  void CSWGPS::setEvent(CSWEvent::PtrType pEvent)
  {
    m_pEvent = pEvent;
  }


  const Vector3D& CSWGPS::getPosition() const
  {
    return getWorldTransform().getTranslation();
  }


  CSWGPS::CSWGPS(const std::string& strName, CSWEvent::PtrType pEvent)
  : CSWSensor(strName, Matrix44D(), 0, 0),
    m_pEvent(pEvent)
  {
  }

}