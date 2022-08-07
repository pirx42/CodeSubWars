// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSoundHomingTorpedo.h"
#include "CSWWorld.h"
#include "CSWSettings.h"

#include "CSWEngine.h"
#include "CSWPassiveSonar.h"


namespace CodeSubWars
{

  CSWSoundHomingTorpedo::~CSWSoundHomingTorpedo()
  {
  }


  void CSWSoundHomingTorpedo::update()
  {
    if (!getPassiveSonar()->isAdjusting())
    {
      if (getPassiveSonar()->hasAdjusted())
      {
        Vector3D vecSonarDir = getInvertWorldTransform().getRotationAsMatrix33()*getPassiveSonar()->getDirection();
        Vector3D vecSonarPlaneDir(-vecSonarDir.x/1.5, -vecSonarDir.y/1.5, 5);
        getMainEngine()->setDirection(vecSonarPlaneDir);    
      }
      getPassiveSonar()->setScanDirection(Vector3D(0, 0, 1));
      getPassiveSonar()->adjustDirectionToMaximum(0.7);
    }
  }


  void CSWSoundHomingTorpedo::draw()
  {
    CSWWeapon::draw();

    if (CSWWorld::getInstance()->getSettings()->getSelectedObject() == getSharedThis())
    {
      getPassiveSonar()->draw();
    }
  }


  CSWSoundHomingTorpedo::CSWSoundHomingTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWTorpedo(strName, matBaseTObject, fLength),
    m_pPassiveSonar(CSWPassiveSonar::create(strName + ".PassiveSonar", Matrix44D()))
  {
    attach(m_pPassiveSonar);
    m_pPassiveSonar->setEnableAutomaticRotation(false);
    m_pPassiveSonar->setScanDirectionMode(CSWPassiveSonar::LOCAL_DIRECTION);
    m_pPassiveSonar->setScanDirection(Vector3D(0, 0, 1));
    m_pPassiveSonar->setAngleOfBeam(30);
  }


  CSWPassiveSonar::PtrType CSWSoundHomingTorpedo::getPassiveSonar()
  {
    return m_pPassiveSonar;
  }

}