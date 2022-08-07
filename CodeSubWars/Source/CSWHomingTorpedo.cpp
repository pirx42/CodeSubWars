// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWHomingTorpedo.h"
#include "CSWWorld.h"
#include "CSWSettings.h"

#include "CSWEngine.h"
#include "CSWActiveSonar.h"


namespace CodeSubWars
{

  CSWHomingTorpedo::~CSWHomingTorpedo()
  {
  }


  void CSWHomingTorpedo::update()
  {
    if (getActiveSonar()->hasTargetDetected() && getActiveSonar()->isTargetDynamic())
    {
      Vector3D vecSonarDir = getInvertWorldTransform().getRotationAsMatrix33()*getActiveSonar()->getDirection();
      Vector3D vecSonarPlaneDir(-vecSonarDir.x/6, -vecSonarDir.y/6, 5);
      getMainEngine()->setDirection(vecSonarPlaneDir);
    }
  }


  void CSWHomingTorpedo::draw()
  {
    CSWWeapon::draw();

    if (CSWWorld::getInstance()->getSettings()->getSelectedObject() == getSharedThis())
    {
      getActiveSonar()->draw();
    }
  }


  CSWHomingTorpedo::CSWHomingTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWTorpedo(strName, matBaseTObject, fLength),
    m_pActiveSonar(CSWActiveSonar::create(strName + ".ActiveSonar", Matrix44D()))
  {
    attach(m_pActiveSonar);
    m_pActiveSonar->setScanDirectionMode(CSWActiveSonar::FRONT);
    m_pActiveSonar->setScanVelocityMode(CSWActiveSonar::FAST);
    m_pActiveSonar->setScanRangeMode(CSWActiveSonar::NEAR_RANGE);
  }


  CSWActiveSonar::PtrType CSWHomingTorpedo::getActiveSonar()
  {
    return m_pActiveSonar;
  }

}