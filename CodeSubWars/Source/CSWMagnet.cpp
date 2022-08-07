// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMagnet.h"
#include "CSWEvent.h"
#include "CSWCollisionDetectedMessage.h"


namespace CodeSubWars
{

  CSWMagnet::~CSWMagnet()
  {
  }


  bool CSWMagnet::isForceEmitterActive() const
  {
    return true;
  }


  void CSWMagnet::initializeForceEmission()
  {
  }


  void CSWMagnet::emitForce(const CSWObject::PtrType pObject)
  {
    CSWIDynamic::PtrType pDynamic = std::dynamic_pointer_cast<CSWIDynamic>(pObject);
    if (!pDynamic)
      return;

    Vector3D vecDist;
    double fDist;  
    Vector3D vecWorldTMassPoint;
    CSWIDynamic::MassPointContainerRange range = getMassPointRange();
    CSWIDynamic::MassPointContainer::iterator itMassPoint = range.first;
    for (; itMassPoint != range.second; ++itMassPoint)
    {
      vecWorldTMassPoint = getWorldTransform()*itMassPoint->first;
      vecDist = pObject->getWorldTransform()*pDynamic->getCenterOfMass() - vecWorldTMassPoint;
      fDist = vecDist.getLength();
      if (fDist < 1)
        fDist = 1;
      addForce(vecWorldTMassPoint, vecDist.getNormalized()*m_fPower*1e+5/fDist);
    }  
  }


  void CSWMagnet::finalizeForceEmission()
  {
  }


  CSWMagnet::CSWMagnet(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWDamDynSolCol(strName, matBaseTObject, 
                    Mesh::createBox(Matrix44D(Vector3D(-size.getWidth()/2.0, 
                                                       -size.getHeight()/2.0, 
                                                       -size.getDepth()/2.0)),
                                    size),
                    true),
    m_fPower(size.getVolume())
  {
    setColor(Vector4D(1, 1, 0, 1));
  }

}