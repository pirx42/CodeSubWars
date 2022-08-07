// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWScanBox.h"

namespace CodeSubWars
{

  CSWScanBox::PtrType CSWScanBox::create(const std::string& strName, const Matrix44D& matBaseTObject, 
                                         const double& fEdgeLength)
  {
    return PtrType(new CSWScanBox(strName, matBaseTObject, fEdgeLength));
  }


  CSWScanBox::~CSWScanBox()
  {
  }


  void CSWScanBox::detectCollision(CSWObject::PtrType pObject, const Vector3D& vecObjectTPosition)
  {
    m_pIntersectingObject = pObject;
  }


  CSWObject::PtrType CSWScanBox::getIntersectingObject() const
  {
    return m_pIntersectingObject.lock();
  }


  CSWScanBox::CSWScanBox(const std::string& strName, const Matrix44D& matBaseTObject, 
                         const double& fEdgeLength)
  : CSWTrigger(strName, matBaseTObject, Mesh::createBox(Matrix44D(Vector3D(-fEdgeLength*0.5, -fEdgeLength*0.5, -fEdgeLength*0.5)),
                                                        Size3D(fEdgeLength, fEdgeLength, fEdgeLength)))
  {
  }

}