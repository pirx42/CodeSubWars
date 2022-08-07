// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


//#include <boost/enable_shared_from_this.hpp>

#pragma once

namespace CodeSubWars
{

  class CSWObject;

  struct ObjectLess : public std::binary_function<std::shared_ptr<CSWObject>, std::shared_ptr<CSWObject>, bool>
  {
    bool operator()(std::weak_ptr<CSWObject> lhs, std::weak_ptr<CSWObject> rhs) const;
  };

  /**
   * This is the base class for all objects within a world.
   */
  class CSWObject : public ARSTD::Node, public std::enable_shared_from_this<CSWObject>
  {
    public:
      typedef std::shared_ptr<CSWObject> PtrType;

      template <class Type>
      static PtrType findObject(PtrType pObject, const Vector3D& vecWorldTPos, const double& fDistance);    

      virtual ~CSWObject();

      //If you want store a reference to this object you _must_ use this method. Otherwise it could be happen that
      //this object is never destructed.
      //@return Returns a shared pointer to this.
      PtrType getSharedThis();

      /**
       * Return the name of the object.
       * @return The current name of the object.
       */
      const std::string& getName() const { return ARSTD::Node::getName(); }

      Matrix44D& getTransform();
      const Matrix44D& getTransform() const;
      void setTransform(const Matrix44D& matBaseTObject);

      const Matrix44D& getWorldTransform() const;
      void setWorldTransform(const Matrix44D& matWorldTObject);

      const Matrix44D& getInvertWorldTransform() const;

      Matrix44D calcRootTObject() const;

      template <class Type>
      PtrType findFirstParent();

      PtrType getRoot();

      void calcWorldTransforms(const Matrix44D& matWorldTParent = Matrix44D::IDENTITY);

      //prepare itself.
      virtual void initialize() {}

      //cleans up itself.
      virtual void finalize() {}

    protected:    
      CSWObject(const std::string& strName, const Matrix44D& matBaseTObject = Matrix44D());

      Matrix44D m_matBaseTObject;   //position in meter
      Matrix44D m_matWorldTObject;  //position in meter
      Matrix44D m_matObjectTWorld;  //position in meter
  };



  template <class Type>
  CSWObject::PtrType CSWObject::findFirstParent()
  {
    if (dynamic_cast<const Type*>(this)) return getSharedThis();

    CSWObject* pParent = dynamic_cast<CSWObject*>(getParent());
    if (!pParent) return CSWObject::PtrType();

    return pParent->findFirstParent<Type>();
  }


  template <class Type>
  CSWObject::PtrType CSWObject::findObject(PtrType pObject, const Vector3D& vecWorldTPos, const double& fDistance)
  {
    if (std::dynamic_pointer_cast<Type>(pObject) &&
        (pObject->getTransform().getTranslation() - vecWorldTPos).getLength() < fDistance) return pObject;

    PtrType pFoundObject;
    ARSTD::Node::ChildConstRange range = pObject->getChildRange();
    for (ARSTD::Node::ChildConstIterator it = range.first; it != range.second; ++it)
    {
      CSWObject::PtrType pObj = std::dynamic_pointer_cast<CSWObject>(*it);
      if (!pObj) throw std::runtime_error("tree contains non CSWObjects");
      pFoundObject = findObject<Type>(pObj, vecWorldTPos, fDistance);
      if (pFoundObject) break;
    }

    return pFoundObject;
  }


}