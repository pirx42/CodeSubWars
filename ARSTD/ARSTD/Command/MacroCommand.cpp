// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <sstream>
#include <algorithm>
#include <memory>
#include <cassert>

#include <boost/next_prior.hpp>

#include "MacroCommand.h"


namespace ARSTD
{


  MacroCommand::PtrType MacroCommand::create(const std::string& name)
  {
    return PtrType(new MacroCommand(name));
  }


  Command::PtrType MacroCommand::copy() const
  {
    return PtrType(new MacroCommand(*this));
  }


  void MacroCommand::initialize()
  {
    if (m_CommandContainer.empty()) 
    {
      m_itCurrentExecutingCommand = m_CommandContainer.end();
    }
    else 
    {
      m_itCurrentExecutingCommand = m_CommandContainer.begin();
      assert(*m_itCurrentExecutingCommand);
      (*m_itCurrentExecutingCommand)->initialize();
    }
  }


  void MacroCommand::step()
  {
    if (m_itCurrentExecutingCommand != m_CommandContainer.end()) 
    {
      (*m_itCurrentExecutingCommand)->step();
    }
    else
    {
      finished();
    }
  }


  void MacroCommand::cleanup()
  {
    if (m_itCurrentExecutingCommand != m_CommandContainer.end()) 
    {
      (*m_itCurrentExecutingCommand)->cleanup();
    }
  }


  std::string MacroCommand::getName() const
  {
    return getName(0);
  }


  std::string MacroCommand::getDetails() const
  {
    return getDetails(0);
  }


  bool MacroCommand::wasExecuted() const
  {
    return getProgress() >= 1;
  }


  double MacroCommand::getProgress() const
  {
    double fProgress = 0;

    CommandContainer::const_iterator it = m_CommandContainer.begin();
    for (; it != m_CommandContainer.end(); ++it)
    {
      fProgress += (*it)->getProgress();
    }
    if (!m_CommandContainer.empty())
      fProgress /= m_CommandContainer.size();

    return fProgress;
  }


  void MacroCommand::resetProgress()
  {
    CommandContainer::iterator it = m_CommandContainer.begin();
    for (; it != m_CommandContainer.end(); ++it)
    {
      (*it)->resetProgress();
    }
  }


  ARSTD::uint64 MacroCommand::getRessourceID() const
  {
    ARSTD::uint64 id = 0;

    CommandContainer::const_iterator it = m_CommandContainer.begin();
    for (; it != m_CommandContainer.end(); ++it)
    {
      id += (*it)->getRessourceID();
    }

    return id;
  }


  void MacroCommand::finished()
  {
    if (m_itCurrentExecutingCommand != m_CommandContainer.end())
      (*m_itCurrentExecutingCommand)->cleanup();

    if (m_itCurrentExecutingCommand != m_CommandContainer.end() && 
        m_itCurrentExecutingCommand != boost::prior(m_CommandContainer.end())) 
    {
      ++m_itCurrentExecutingCommand;
      (*m_itCurrentExecutingCommand)->resetProgress();
      (*m_itCurrentExecutingCommand)->initialize();
    }
    else 
    {
      m_itCurrentExecutingCommand = m_CommandContainer.end();
      getCommandProcessor()->finished();
    }
  }


  void MacroCommand::repeat()
  {
    if (m_CommandContainer.empty())
    {
      m_itCurrentExecutingCommand = m_CommandContainer.end();
      return;
    }
    
    if (m_itCurrentExecutingCommand != m_CommandContainer.end())
      (*m_itCurrentExecutingCommand)->cleanup();

    m_itCurrentExecutingCommand = m_CommandContainer.begin();
    MacroCommand::resetProgress();
    (*m_itCurrentExecutingCommand)->initialize();
  }


  bool MacroCommand::push()
  {
    return getCommandProcessor()->push();
  }


  bool MacroCommand::pop()
  {
    return getCommandProcessor()->pop();
  }


  void MacroCommand::cleanupHistory()
  {
    getCommandProcessor()->cleanupHistory();
  }


  double MacroCommand::getCurrentCommandProgress()
  {
    if (m_itCurrentExecutingCommand == m_CommandContainer.end())
      return -1;
    return (*m_itCurrentExecutingCommand)->getProgress();
  }


  std::string MacroCommand::getCurrentCommandName()
  {
    if (m_itCurrentExecutingCommand == m_CommandContainer.end())
      return "";
    return (*m_itCurrentExecutingCommand)->getName();
  }


  std::string MacroCommand::getCurrentCommandDetails()
  {
    if (m_itCurrentExecutingCommand == m_CommandContainer.end())
      return "";
    return (*m_itCurrentExecutingCommand)->getDetails();
  }


  void MacroCommand::attach(Command::PtrType pCommand)
  {
    if (!pCommand)
      return;
    pCommand->m_pCommandprocessor = this;
    pCommand->resetProgress();
    m_CommandContainer.push_back(pCommand);
    m_itCurrentExecutingCommand = m_CommandContainer.end();
  }


  void MacroCommand::detach(Command::PtrType pCommand)
  {
    CommandContainer::iterator itFound = std::find(m_CommandContainer.begin(), m_CommandContainer.end(), pCommand);
    if (itFound != m_CommandContainer.end())
      m_CommandContainer.erase(itFound);
    m_itCurrentExecutingCommand = m_CommandContainer.end();
  }


  MacroCommand::MacroCommand(const std::string& name)
  : m_Name(name)
  {
    m_itCurrentExecutingCommand = m_CommandContainer.end();
  }


  MacroCommand::MacroCommand(const MacroCommand& other)
  : Command(other),
    m_Name(other.m_Name)
  {
    CommandContainer::const_iterator itOther = other.m_CommandContainer.begin();
    for (; itOther != other.m_CommandContainer.end(); ++itOther)
    {
      Command::PtrType pCopyCommand = (*itOther)->copy();
      pCopyCommand->m_pCommandprocessor = this;
      m_CommandContainer.push_back(pCopyCommand);      
    }
    
    assert(m_CommandContainer.size() == other.m_CommandContainer.size());
    itOther = other.m_CommandContainer.begin();
    CommandContainer::iterator it = m_CommandContainer.begin();
    for (; itOther != other.m_CommandContainer.end() && it != m_CommandContainer.end(); ++itOther, ++it)
    {
      if (other.m_itCurrentExecutingCommand == itOther)
      {
        m_itCurrentExecutingCommand = it;
      }
    }
  }


  std::string MacroCommand::getName(int nHiearchy) const
  {
    std::string strSingleSpace = "  ";
    std::string strSingleSpaceMinOne = " ";
    std::string strSpace = "";
    for (int i = 0; i < nHiearchy; ++i) strSpace += strSingleSpace;
    
    std::stringstream ss;
    ss << strSpace;
    if (nHiearchy != 0) ss << "+-";
    ss << m_Name << "\n";
    CommandContainer::const_iterator it = m_CommandContainer.begin();
    for (; it != m_CommandContainer.end(); ++it)
    {
      MacroCommand::PtrType pMC = std::dynamic_pointer_cast<MacroCommand>(*it);
      if (pMC)
        ss << pMC->getName(nHiearchy + 1);
      else 
      {
        if (it == m_itCurrentExecutingCommand)    
        {
          ss << ">" << strSpace << strSingleSpaceMinOne;
        }
        else
        {
          ss << strSpace << strSingleSpace;
        }
        ss << "+-" << (*it)->getName() << "\n";      
      }
    }

    return ss.str();
  }


  std::string MacroCommand::getDetails(int nHiearchy) const
  {
    std::string strSingleSpace = "  ";
    std::string strSingleSpaceMinOne = " ";
    std::string strSpace = "";
    for (int i = 0; i < nHiearchy; ++i) strSpace += strSingleSpace;
    
    std::stringstream ss;
    ss << strSpace;
    if (nHiearchy != 0) ss << "+-";
    ss << m_Name << "\n";
    CommandContainer::const_iterator it = m_CommandContainer.begin();
    for (; it != m_CommandContainer.end(); ++it)
    {
      MacroCommand::PtrType pMC = std::dynamic_pointer_cast<MacroCommand>(*it);
      if (pMC)
        ss << pMC->getDetails(nHiearchy + 1);
      else 
      {
        if (it == m_itCurrentExecutingCommand)    
        {
          ss << ">" << strSpace << strSingleSpaceMinOne;
        }
        else
        {
          ss << strSpace << strSingleSpace;
        }
        ss << "+-" << (*it)->getDetails() << "\n";      
      }
    }

    return ss.str();
  }


  MacroCommand::~MacroCommand()
  {
  }

} //namespace ARSTD