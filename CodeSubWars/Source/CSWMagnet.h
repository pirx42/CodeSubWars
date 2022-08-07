// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWDamDynSolCol.h"
#include "CSWIForceEmitter.h"


namespace CodeSubWars
{

  class CSWMagnet : public CSWDamDynSolCol,
                    public CSWIForceEmitter
  {
    public:
      typedef std::shared_ptr<CSWMagnet> PtrType;

      virtual ~CSWMagnet();

      //defined methods for forceemitter
      virtual bool isForceEmitterActive() const;
      virtual void initializeForceEmission();
      virtual void emitForce(std::shared_ptr<CSWObject> pObject);
      virtual void finalizeForceEmission();

    protected:
      CSWMagnet(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size);
    
      //other stuff
      double m_fPower;
  };

}