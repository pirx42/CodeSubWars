// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <cassert>
#include "Node.h"
#include <iostream>

namespace ARSTD 
{

  Node::Node(const std::string& strName)
  : Element(strName)
  {
  }


  Node::~Node()
  {
  }


  bool Node::attach(Element::PtrType pElement)
  {
    if (!pElement)
      return false;

    bool bResult = m_ChildContainer.insert(pElement).second;
    if (bResult)
    {
      pElement->setParent(this);
      m_nModifications |= ChildAdded;
    }
    else std::cerr << "Node::attach(): could not attach element " << pElement->getName() << " onto element " << getName() << "\n";

    return bResult;
  }


  bool Node::detach(Element::PtrType pElement)
  {
    if (!pElement)
      return false;

    bool bResult = false;
    Element* pParent = pElement->getParent();
    if (pParent)
    {
      Node* pNode = dynamic_cast<Node*>(pParent);
      assert(pNode && "the parent of this element must be a node");
      bResult = pNode->m_ChildContainer.erase(pElement) != 0;
      if (bResult)
      {
        pElement->resetParent();
        m_nModifications |= ChildRemoved;
      }
    }
    
    if (!bResult)
      std::cerr << "Node::detach(): could not detach element " << pElement->getName() << " from element " << getName() << "\n";

    return bResult;
  }


  bool Node::hasChilds() const
  {
    return !m_ChildContainer.empty();
  }


  int Node::getTotalChildNumber() const
  {
    return static_cast<int>(m_ChildContainer.size());
  }


  void Node::rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings)
  {
    if (strOldName == strNewName)
      return;

    Element::rename(strOldName, strNewName, bReplaceSubstrings);

    ChildRange range = getChildRange();
    ChildIterator it = range.first;
    for (; it != range.second; ++it)
    {
      (*it)->rename(strOldName, strNewName, bReplaceSubstrings);
    }
  }


  int Node::getModifications(bool bResetModification)
  {
    int nTmp = m_nModifications;
    if (bResetModification)
      resetModification();
    
    ChildRange range = getChildRange();
    ChildIterator it = range.first;
    for (; it != range.second; ++it)
    {
      Node::PtrType pNode = std::dynamic_pointer_cast<Node>(*it);
      if (pNode)
        nTmp |= pNode->getModifications(bResetModification);
    }

    return nTmp;
  }


  void Node::resetModification()
  {
    m_nModifications = Nothing;
  }


  Node::ChildRange Node::getChildRange()
  {
    return std::make_pair(m_ChildContainer.begin(), m_ChildContainer.end());
  }


  Node::ChildConstRange Node::getChildRange() const
  {
    return std::make_pair(m_ChildContainer.begin(), m_ChildContainer.end());
  }


} //namespace ARSTD