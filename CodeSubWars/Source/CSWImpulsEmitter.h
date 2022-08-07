// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

class CSWImpulsEmitter
{
  public:
    typedef boost::shared_ptr<CSWImpulsEmitter> PtrType;

    static PtrType create();
    ~CSWImpulsEmitter();

    //returns the emited force to the given world position
    ARSTD::Vector3D emitImpuls(const ARSTD::Vector3D& vecPosition);

  private:
    CSWImpulsEmitter();
    
};