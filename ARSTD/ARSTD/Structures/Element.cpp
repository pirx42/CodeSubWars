// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <string>

#include "Element.h"
#include "Node.h"


namespace ARSTD 
{

  Element::Element(const std::string& strName)
  : m_strName(strName),
    m_pParent(NULL)
  {
  }


  Element::~Element()
  {
  }


  void Element::setName(const std::string& strName)
  {
    m_strName = strName;
  }


  const std::string& Element::getName() const
  {
    return m_strName;
  }


  void Element::rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings)
  {
    if (strOldName == strNewName)
      return;

    if (bReplaceSubstrings)
    {
      size_t nIdx = m_strName.find(strOldName);
      if (nIdx != std::string::npos)
      {
        m_strName.erase(nIdx, strOldName.size());
        m_strName.insert(nIdx, strNewName);
      }
    }
    else
      setName(strNewName);
  }


  void Element::setParent(Element* pElement)
  {
    m_pParent = pElement;
  }


  Element* Element::getParent() const
  {
    return m_pParent;
  }


  void Element::resetParent()
  {
    m_pParent = NULL;
  }


  Element::PtrType Element::findElement(Element::PtrType pElement, const std::string& strName)
  {
    if (pElement->getName() == strName)
      return pElement;

    Node::PtrType pNode = std::dynamic_pointer_cast<Node>(pElement);
    if (!pNode)
      return Element::PtrType();

    Element::PtrType pFoundElement;
    Node::ChildConstRange range = pNode->getChildRange();
    for (Node::ChildConstIterator it = range.first; it != range.second; ++it)
    {
      pFoundElement = findElement(*it, strName);
      if (pFoundElement)
        break;
    }

    return pFoundElement;
  }



} //namespace ARSTD