// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

//todo: redo with partial specialization


namespace CodeSubWars
{

  template<typename ObjectType, typename Type0ToCollect>
  class CSWMessageCollectObjects1 : public ARSTD::Message<ObjectType>
  {
    public:
      typedef std::vector<typename ObjectType::PtrType> ObjectContainer;
      typedef std::vector<ObjectContainer> ObjectContainers;
  
      CSWMessageCollectObjects1()
      {
        m_ObjectContainers.resize(1);
      }

      virtual ~CSWMessageCollectObjects1() {}

      void evaluateObject(typename ObjectType::PtrType pObject)
      {
        assert(m_ObjectContainers.size() == 1);
        if (std::dynamic_pointer_cast<Type0ToCollect>(pObject))
        {
          m_ObjectContainers[0].push_back(pObject);
        }
      }

      const ObjectContainers& getCollectedObjects() const { return m_ObjectContainers; }

    protected:
      CSWMessageCollectObjects1(const CSWMessageCollectObjects1& other);
    
      ObjectContainers m_ObjectContainers;
  };









  template<typename ObjectType, typename Type0ToCollect, typename Type1ToCollect>
  class CSWMessageCollectObjects2 : public ARSTD::Message<ObjectType>
  {
    public:
      typedef std::vector<typename ObjectType::PtrType> ObjectContainer;
      typedef std::vector<ObjectContainer> ObjectContainers;
  
      CSWMessageCollectObjects2()
      {
        m_ObjectContainers.resize(2);
      }

      virtual ~CSWMessageCollectObjects2() {}

      void evaluateObject(typename ObjectType::PtrType pObject)
      {
        assert(m_ObjectContainers.size() == 2);
        if (std::dynamic_pointer_cast<Type0ToCollect>(pObject))
        {
          m_ObjectContainers[0].push_back(pObject);
        }
        else if (std::dynamic_pointer_cast<Type1ToCollect>(pObject))
        {
          m_ObjectContainers[1].push_back(pObject);
        }
      }

      const ObjectContainers& getCollectedObjects() const { return m_ObjectContainers; }

    protected:
      CSWMessageCollectObjects2(const CSWMessageCollectObjects2& other);
    
      ObjectContainers m_ObjectContainers;
  };

}