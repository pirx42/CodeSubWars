// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWControlCommand.h"
#include "CSWController.h"


namespace CodeSubWars
{

  CSWControlCommand::PtrType CSWControlCommand::create(CSWController::PtrType pController)
  {
    return PtrType(new CSWControlCommand(pController));
  }


  CSWControlCommand::~CSWControlCommand()
  {
  }


  ARSTD::Command::PtrType CSWControlCommand::copy() const
  {
    return PtrType(new CSWControlCommand(*this));
  }


  void CSWControlCommand::initialize()
  {
    m_pController->initialize();
  }


  void CSWControlCommand::step()
  {
    m_pController->update();
    setProgress(m_pController->getProgress());
    if (m_pController->hasFinished())
    {
      cleanup();
      setProgress(1);
      getCommandProcessor()->finished();
    }
  }


  void CSWControlCommand::cleanup()
  {
    m_pController->finalize();
  }


  uint64 CSWControlCommand::getRessourceID() const
  {
    return 0;
  }


  std::string CSWControlCommand::getName() const
  {
    return m_pController->getName();
  }


  std::string CSWControlCommand::getDetails() const
  {
    return m_pController->getDetails();
  }


  CSWControlCommand::CSWControlCommand(CSWController::PtrType pController)
  : m_strName("ControlCommand"),
    m_pController(pController)
  {
  }


  CSWControlCommand::CSWControlCommand(const CSWControlCommand& other)
  : ARSTD::Command(other),
    m_strName(other.m_strName),
    m_pController(other.m_pController)
  {
  }

}