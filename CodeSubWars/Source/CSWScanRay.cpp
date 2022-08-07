// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWScanRay.h"

namespace CodeSubWars
{

  CSWScanRay::PtrType CSWScanRay::create(const std::string& strName, const Matrix44D& matBaseTObject, 
                                         const double& fLength)
  {
    return PtrType(new CSWScanRay(strName, matBaseTObject, fLength));
  }


  CSWScanRay::~CSWScanRay()
  {
  }


  void CSWScanRay::prepare()
  {
    CSWTrigger::prepare();
    //before doing collision test, reset variables
    m_pIntersectingObject.reset();
    m_fDetectedDistanceSquared = m_fLength*m_fLength;
  }


  void CSWScanRay::detectCollision(CSWObject::PtrType pObject, const Vector3D& vecObjectTPosition)
  {
  //  if (pObject == findFirstParent<CSWISolid>())
  //    return;
    double fCurrentDistanceSquared = vecObjectTPosition.getSquaredLength();
    if (fCurrentDistanceSquared < 70*70)
      return;

    if (fCurrentDistanceSquared < m_fDetectedDistanceSquared)
    {
      m_fDetectedDistanceSquared = fCurrentDistanceSquared;
      m_pIntersectingObject = pObject;
    }
  }


  double CSWScanRay::getDetectedDistance() const
  {
    return sqrt(m_fDetectedDistanceSquared);
  }


  CSWObject::PtrType CSWScanRay::getIntersectingObject() const
  {
    return m_pIntersectingObject.lock();
  }


  CSWScanRay::CSWScanRay(const std::string& strName, const Matrix44D& matBaseTObject, 
                         const double& fLength)
  : CSWTrigger(strName, matBaseTObject, Mesh::createBox(Matrix44D(Vector3D(-0.5, -0.5, 0)),
                                                        Size3D(1, 1, fLength))),
    m_fLength(fLength)
  {
  }

}