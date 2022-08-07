// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWTrigger.h"

namespace CodeSubWars
{

  // detects colliding objects nearest to local origin. before collision detection cycle values are reset.
  class CSWScanRay : public CSWTrigger
  {
    public:
      typedef std::shared_ptr<CSWScanRay> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject, 
                            const double& fLength);

      virtual ~CSWScanRay();
    
      //defined methods for collideable
      virtual void prepare();
    
      //defined methods for trigger
      virtual void detectCollision(CSWObject::PtrType pObject, const Vector3D& vecObjectTPosition);

      double getDetectedDistance() const;
      std::shared_ptr<CSWObject> getIntersectingObject() const;
 
    protected:
      CSWScanRay(const std::string& strName, const Matrix44D& matBaseTObject, 
                 const double& fLength);

      double m_fLength;
      double m_fDetectedDistanceSquared;

      std::weak_ptr<CSWObject> m_pIntersectingObject;
  };

}