// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMagneticMine.h"


namespace CodeSubWars
{

  CSWMagneticMine::~CSWMagneticMine()
  {
  }


  bool CSWMagneticMine::isForceEmitterActive() const
  {
    return isLaunched() && isArmed();
  }


  void CSWMagneticMine::initializeForceEmission()
  {
  }


  void CSWMagneticMine::emitForce(const CSWObject::PtrType pObject)
  {
    CSWIDynamic::PtrType pDynamic = std::dynamic_pointer_cast<CSWIDynamic>(pObject);
    if (!pDynamic)
      return;

    Vector3D vecDist = pObject->getWorldTransform()*pDynamic->getCenterOfMass() - getWorldTransform()*getCenterOfMass();
    double fDist = vecDist.getLength();
    if (fDist > 100)
      return;
  
    Vector3D vecDistNorm = vecDist/fDist;
    if (fDist < 30)
      fDist = 30;
    addForceToCM(vecDistNorm*m_fPower*pDynamic->getTotalMass()/((fDist + 600)*6e+7));
  }


  void CSWMagneticMine::finalizeForceEmission()
  {
  }


  CSWMagneticMine::CSWMagneticMine(const std::string& strName, const Matrix44D& matBaseTObject, double fEdgeLength)
  : CSWMine(strName, matBaseTObject, fEdgeLength)
  {
    setColor(Vector4D(1, 1, 0, 1));
  }

}