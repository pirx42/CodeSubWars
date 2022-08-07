// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

class CSWForceEmitter
{
  public:
    typedef boost::shared_ptr<CSWForceEmitter> PtrType;

    static PtrType create();
    ~CSWForceEmitter();

    //returns the emited force to the given world position
    ARSTD::Vector3D emitForce(const ARSTD::Vector3D& vecPosition);

  private:
    CSWForceEmitter();
    
};