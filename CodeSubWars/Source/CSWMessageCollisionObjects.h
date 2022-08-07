// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include "arstd/Message/Message.h"

namespace CodeSubWars
{

  class CSWObject;


  class CSWMessageDeleteCollisionObjects : public ARSTD::Message<CSWObject>
  {
    public:
      CSWMessageDeleteCollisionObjects();
      virtual ~CSWMessageDeleteCollisionObjects();

      void evaluateObject(std::shared_ptr<CSWObject> pObject);

      static bool deleteCollision(std::shared_ptr<CSWObject> pObject);

    protected:
      static bool deleteObjectCollision(std::shared_ptr<CSWObject> pObject);

      CSWMessageDeleteCollisionObjects(const CSWMessageDeleteCollisionObjects& other);
  };




  class CSWMessageInitializeCollisionObjects : public ARSTD::Message<CSWObject>
  {
    public:
      CSWMessageInitializeCollisionObjects();
      virtual ~CSWMessageInitializeCollisionObjects();

      void evaluateObject(std::shared_ptr<CSWObject> pObject);

      static bool initializeCollision(std::shared_ptr<CSWObject> pObject);

      static DT_Bool collide(void* client_data, void* pObj1, void* pObj2,
                             const DT_CollData *coll_data);
 
    protected:
      static bool initializeObjectCollision(std::shared_ptr<CSWObject> pObject);

      CSWMessageInitializeCollisionObjects(const CSWMessageInitializeCollisionObjects& other);
  };

}