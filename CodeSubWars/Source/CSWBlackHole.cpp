// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWBlackHole.h"
#include "CSWIDynamic.h"


namespace CodeSubWars
{

  CSWBlackHole::~CSWBlackHole()
  {
  }


  bool CSWBlackHole::isForceEmitterActive() const
  {
    return true;
  }


  void CSWBlackHole::initializeForceEmission()
  {
  }


  void CSWBlackHole::emitForce(CSWObject::PtrType pObject)
  {
    CSWIDynamic::PtrType pDynamic = std::dynamic_pointer_cast<CSWIDynamic>(pObject);
    if (!pDynamic)
      return;

    Vector3D vecDist;
    double fDist;  
    Vector3D vecWorldTMassPoint;
    CSWIDynamic::MassPointContainerRange range = pDynamic->getMassPointRange();
    CSWIDynamic::MassPointContainer::iterator itMassPoint = range.first;
    for (; itMassPoint != range.second; ++itMassPoint)
    {
      vecWorldTMassPoint = pObject->getWorldTransform()*itMassPoint->first;
      vecDist = getWorldTransform().getTranslation() - vecWorldTMassPoint;
      fDist = vecDist.getLength();
      if (fDist < 50)
        fDist = 50;
      pDynamic->addForce(vecWorldTMassPoint, vecDist.getNormalized()*m_fPower/fDist);
    }  
  }


  void CSWBlackHole::finalizeForceEmission()
  {
  }


  void CSWBlackHole::update()
  {
  }


  CSWBlackHole::CSWBlackHole(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWObject(strName, matBaseTObject),
    m_fPower(size.getVolume())
  {
  }

}