// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWForceEmitter.h"


CSWForceEmitter::PtrType CSWForceEmitter::create()
{
  return PtrType(new CSWForceEmitter());
}


CSWForceEmitter::~CSWForceEmitter()
{
}


ARSTD::Vector3D CSWForceEmitter::emitForce(const ARSTD::Vector3D& vecPosition)
{
  return ARSTD::Vector3D(0, 0, 0);
}


CSWForceEmitter::CSWForceEmitter()
{
}
