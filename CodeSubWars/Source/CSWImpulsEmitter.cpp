// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWImpulsEmitter.h"


CSWImpulsEmitter::PtrType CSWImpulsEmitter::create()
{
  return PtrType(new CSWImpulsEmitter());
}


CSWImpulsEmitter::~CSWImpulsEmitter()
{
}


ARSTD::Vector3D CSWImpulsEmitter::emitImpuls(const ARSTD::Vector3D& vecPosition)
{
  return ARSTD::Vector3D(0, 0, 0);
}


CSWImpulsEmitter::CSWImpulsEmitter()
{
}
