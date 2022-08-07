// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWCommandable.h"


namespace CodeSubWars
{

  CSWCommandable::PtrType CSWCommandable::create()
  {
    return PtrType(new CSWCommandable());
  }


  CSWCommandable::~CSWCommandable()
  {
  }


  void CSWCommandable::step()
  {
    m_pCommandProcessor->step();
  }


  ARSTD::CommandProcessor::PtrType CSWCommandable::getCommandProcessor()
  {
    return m_pCommandProcessor;
  }


  CSWCommandable::CSWCommandable()
  : m_pCommandProcessor(ARSTD::CommandProcessor::create())
  {
  }


}