// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#pragma warning (disable : 4786)

#include <string>
#include <set>

#include <memory>

#include "Element.h"


namespace ARSTD 
{

  /**
   * This class is a specialized object in a tree that can have childs.
   */
  class Node : public Element
  {
    public:
      typedef std::shared_ptr<Node> PtrType;

      typedef std::set<Element::PtrType> ChildContainer;
      typedef ChildContainer::iterator ChildIterator;
      typedef ChildContainer::const_iterator ChildConstIterator;
      typedef std::pair<ChildIterator, ChildIterator> ChildRange;
      typedef std::pair<ChildConstIterator, ChildConstIterator> ChildConstRange;

      enum Modification
      {
        Nothing       = 0,
        ChildAdded    = 1,
        ChildRemoved  = 2
      };

      virtual ~Node();

      bool attach(Element::PtrType pElement);
      bool detach(Element::PtrType pElement);

      bool hasChilds() const;
      
      template <class Type>
      int getChildNumber() const;
      int getTotalChildNumber() const;
      
      virtual void rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings = true);

      int getModifications(bool bResetModification = true);
      void resetModification();

      ChildRange getChildRange();
      ChildConstRange getChildRange() const;

    protected:
      Node(const std::string& strName = "noname");

      ChildContainer m_ChildContainer;

      int m_nModifications;
  };


  template <class Type>
  int Node::getChildNumber() const
  {
    int nResult = 0;
    ChildContainer::const_iterator it = m_ChildContainer.begin();
    for (; it != m_ChildContainer.end(); ++it)
    {
      if (std::dynamic_pointer_cast<Type>(*it)) ++nResult;
    }
    return nResult;
  }



} //namespace ARSTD