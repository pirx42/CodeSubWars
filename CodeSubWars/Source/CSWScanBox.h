// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWTrigger.h"

namespace CodeSubWars
{

  // detects whether an object hits the box on not. once it is hit it will never reset.
  class CSWScanBox : public CSWTrigger
  {
    public:
      typedef std::shared_ptr<CSWScanBox> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, 
                            const double& fEdgeLength);

      virtual ~CSWScanBox();
    
      //defined methods for trigger
      virtual void detectCollision(CSWObject::PtrType pObject, const Vector3D& vecObjectTPosition);

      std::shared_ptr<CSWObject> getIntersectingObject() const;
 
    protected:
      CSWScanBox(const std::string& strName, const Matrix44D& matBaseTObject, 
                 const double& fEdgeLength);

      std::weak_ptr<CSWObject> m_pIntersectingObject;
  };

}