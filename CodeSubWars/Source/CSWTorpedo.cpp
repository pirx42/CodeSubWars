// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWSolid.h"
#include "CSWTorpedo.h"
#include "CSWEvent.h"
#include "CSWCollisionDetectedMessage.h"
#include "CSWExplosionDetectedMessage.h"
#include "CSWEngine.h"



namespace CodeSubWars
{

  CSWTorpedo::~CSWTorpedo()
  {
  }


  void CSWTorpedo::addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls)
  {
    CSWWeapon::addImpuls(vecWorldTPosition, vecWorldTImpuls);
    fireImmediately();
  }


  bool CSWTorpedo::launch(double fLaunchDelay)
  {
    bool bResult = true;
    if (getAbsFireTime() > ARSTD::Time::getTime() + 60)
      bResult &= fire(60);
    if (bResult)
      return CSWWeapon::launch(fLaunchDelay);
    return bResult;
  }


  CSWTorpedo::CSWTorpedo(const std::string& strName, const Matrix44D& matBaseTObject, double fLength)
  : CSWWeapon(strName, matBaseTObject, Size3D(fLength/10, fLength/10, fLength))
  {
    Size3D sizeSurface = m_pSolid->getMesh()->getAxisAlignedBoundingBox().getSize();

    //create and add equipment
    m_pMainEngine = CSWEngine::create(strName + ".MainEngine", 
                                      Matrix44D(m_pDynamic->getCenterOfMass() - Vector3D(0, 0, sizeSurface.getDepth()/2)), 
                                      m_pDynamic->getMassMesh()->getAxisAlignedBoundingBox().getSize()/Size3D(2.5, 2.5, 9),
                                      true);
    attach(m_pMainEngine);
  }


  CSWEngine::PtrType CSWTorpedo::getMainEngine()
  {
    return m_pMainEngine;
  }

}