// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once
#include <memory>

namespace ARSTD 
{

  class Element
  {
    public:
      typedef std::shared_ptr<Element> PtrType;

      virtual ~Element();

      void setName(const std::string& strName);
      const std::string& getName() const;

      virtual void rename(const std::string& strOldName, const std::string& strNewName, bool bReplaceSubstrings = true);

      void setParent(Element* pElement);
      Element* getParent() const;

      void resetParent();

      static PtrType findElement(PtrType pElement, const std::string& strName);

    protected:
      Element(const std::string& strName = "noname");

      std::string m_strName;
      Element* m_pParent;
  };

} //namespace ARSTD