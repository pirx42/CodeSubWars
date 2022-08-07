// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

#include "Constants.h"
#include "CSWISolid.h"


namespace CodeSubWars
{

  template<typename ObjectType>
  class CSWMessageStoreObjects : public ARSTD::Message<ObjectType>
  {
    public:
      CSWMessageStoreObjects(boost::iostreams::filtering_ostream& os);
      virtual ~CSWMessageStoreObjects();

      void evaluateObject(typename ObjectType::PtrType pObject);

    protected:
      CSWMessageStoreObjects(const CSWMessageStoreObjects& other);
    
      boost::iostreams::filtering_ostream& m_OutputStream;
  };



  //implementation

  template<typename ObjectType>
  CSWMessageStoreObjects<ObjectType>::CSWMessageStoreObjects(boost::iostreams::filtering_ostream& os)
  : m_OutputStream(os)
  {
  }


  template<typename ObjectType>
  CSWMessageStoreObjects<ObjectType>::~CSWMessageStoreObjects()
  {
  }


  template<typename ObjectType>
  void CSWMessageStoreObjects<ObjectType>::evaluateObject(typename ObjectType::PtrType pObject)
  {
    CSWISolid::PtrType pSolid = std::dynamic_pointer_cast<CSWISolid>(pObject);
    //store only visible objects
    if (!pSolid)
      return;

    std::string strName = pObject->getName();
    ARSTD::uint16 nNameSize = strName.size();
    m_OutputStream.write(reinterpret_cast<const char*>(&nNameSize), 2);
    m_OutputStream.write(strName.c_str(), nNameSize);
  
    Matrix44F mat = static_cast<Matrix44F>(pObject->getWorldTransform());
    m_OutputStream.write(reinterpret_cast<const char*>(mat.pData), 16*4);
  
    Size3F size = static_cast<Size3F>(pSolid->getSurfaceBoundingBox().getSize());
    m_OutputStream.write(reinterpret_cast<const char*>(&size), 3*4);
  
    Vector4F color = static_cast<Vector4F>(pSolid->getColor());
    m_OutputStream.write(reinterpret_cast<const char*>(&color), 4*4);
  }

}