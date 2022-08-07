// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

template <typename ParamType>
class ConnectableObject
{
  public:
    typedef allocator::types<ParamType>::pointer ParamPtrType;

    virtual ~ConnectableObject() {}

    ParamPtrType getParameter() { return &m_Parameter; }

  protected:
    ConnectableObject();

  private:
    ParamType m_Parameter;
};