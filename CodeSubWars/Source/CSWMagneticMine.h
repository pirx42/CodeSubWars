// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWMine.h"
#include "CSWIForceEmitter.h"


namespace CodeSubWars
{

  /**
   * This class specializes an ordinary mine to moving mine caused by magnetic forces. Ones the mine has been released and is armed, it moves to near
   * objects (below 100m) and will explode if something collides with it.
   */
  class CSWMagneticMine : public CSWMine,
                          public CSWIForceEmitter
  {
    public:
      typedef std::shared_ptr<CSWMagneticMine> PtrType;

      virtual ~CSWMagneticMine();

      //defined methods for forceemitter
      virtual bool isForceEmitterActive() const;
      virtual void initializeForceEmission();
      virtual void emitForce(std::shared_ptr<CSWObject> pObject);
      virtual void finalizeForceEmission();

    protected:
      CSWMagneticMine(const std::string& strName, const Matrix44D& matBaseTObject, double fEdgeLength);
  };

}