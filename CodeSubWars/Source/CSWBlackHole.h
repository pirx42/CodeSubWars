// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWObject.h"

#include "CSWIForceEmitter.h"
#include "CSWIUpdateable.h"


namespace CodeSubWars
{

  /** 
   * This class represents a black hole. This applies forces to move objects to its near. Objects should not move close to this.
   */
  class CSWBlackHole : public CSWObject,
                       public CSWIForceEmitter,
                       public CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWBlackHole> PtrType;

      virtual ~CSWBlackHole();
  
      //defined methods from forceemitter
      virtual bool isForceEmitterActive() const;
      virtual void initializeForceEmission();
      virtual void emitForce(CSWObject::PtrType pObject);
      virtual void finalizeForceEmission();

      //defined methods from updateable
      virtual void update();

    protected:
      CSWBlackHole(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);

      //properties

      //other stuff
      double m_fPower;
  };

}