// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWObject.h"


namespace CodeSubWars
{

  bool ObjectLess::operator()(std::weak_ptr<CSWObject> lhs, std::weak_ptr<CSWObject> rhs) const
  {
    auto lptr = lhs.lock(), rptr = rhs.lock();
    if (!rptr)
      return false; // nothing after expired pointer 
    if (!lptr)
      return true;  // every not expired after expired pointer
    return lptr.get() < rptr.get();
  }


  CSWObject::~CSWObject()
  {
  }


  CSWObject::PtrType CSWObject::getSharedThis()
  {
    return shared_from_this();
  }


  Matrix44D& CSWObject::getTransform()
  {
    return m_matBaseTObject;
  }


  const Matrix44D& CSWObject::getTransform() const
  {
    return m_matBaseTObject;
  }


  void CSWObject::setTransform(const Matrix44D& matBaseTObject)
  {
    m_matBaseTObject = matBaseTObject;
  }


  const Matrix44D& CSWObject::getWorldTransform() const
  {
    return m_matWorldTObject;
  }


  void CSWObject::setWorldTransform(const Matrix44D& matWorldTObject)
  {
    m_matWorldTObject = matWorldTObject;
    m_matObjectTWorld = Matrix44D::inverse(matWorldTObject);
  }


  const Matrix44D& CSWObject::getInvertWorldTransform() const
  {
    return m_matObjectTWorld;
  }


  Matrix44D CSWObject::calcRootTObject() const
  {
    CSWObject* pParent = dynamic_cast<CSWObject*>(getParent());
    if (!pParent)
      return m_matBaseTObject;

    return pParent->calcRootTObject()*m_matBaseTObject;
  }


  CSWObject::PtrType CSWObject::getRoot()
  {
    CSWObject* pParent = dynamic_cast<CSWObject*>(getParent());
    if (!pParent)
      return getSharedThis();

    return pParent->getRoot();
  }


  void CSWObject::calcWorldTransforms(const Matrix44D& matWorldTParent)
  {
    Matrix44D matWorldTObject(matWorldTParent*getTransform());

    setWorldTransform(matWorldTObject);

    CSWObject::ChildRange range = getChildRange();
    for (CSWObject::ChildIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pChild = std::dynamic_pointer_cast<CSWObject>(*it);
      if (pChild)
        pChild->calcWorldTransforms(matWorldTObject);
    }
  }


  CSWObject::CSWObject(const std::string& strName, const Matrix44D& matBaseTObject)
  : ARSTD::Node(strName),
    m_matBaseTObject(matBaseTObject),
    m_matWorldTObject(true)
  {
  }


}