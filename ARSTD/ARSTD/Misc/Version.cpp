// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include <iosfwd>
#include <sstream>
#include <functional>
#include <algorithm>

#include "Version.h"

namespace ARSTD
{
  
  Version::Version(uint nMainV, uint nSubV, uint nSubSubV, const std::string& commitHash, int nVersionPartsToCompare)
  : nMainVersion(nMainV),
    nSubVersion(nSubV),
    nSubSubVersion(nSubSubV),
    strCommitHash(commitHash),
    m_nVersionPartsToCompare(nVersionPartsToCompare)
  {
  }


  Version::Version(const std::string& str, int nVersionPartsToCompare)
  : nMainVersion(0),
    nSubVersion(0),
    nSubSubVersion(0),
    m_nVersionPartsToCompare(nVersionPartsToCompare)
  {
    std::string strCopy(str);
    std::replace_if(strCopy.begin(), strCopy.end(), std::not1(std::ptr_fun(isdigit)), ' ');

    std::stringstream ss;
    ss << strCopy;
    ss >> this->nMainVersion >> this->nSubVersion >> this->nSubSubVersion;
  }


  std::string Version::getAsString() const
  {
    std::stringstream ss;
    ss << this->nMainVersion << "." << this->nSubVersion << "." << this->nSubSubVersion << " (" << this->strCommitHash << ")";
    return ss.str();
  }


  const Version::Compare Version::compare(const Version& lhs, const Version& rhs)
  {
    if (lhs.m_nVersionPartsToCompare & MAIN || rhs.m_nVersionPartsToCompare & MAIN)
    {
      if (lhs.nMainVersion < rhs.nMainVersion)
        return LESS;
      else if (lhs.nMainVersion > rhs.nMainVersion)
        return GREATER;
    }

    if (lhs.m_nVersionPartsToCompare & SUB || rhs.m_nVersionPartsToCompare & SUB)
    {
      if (lhs.nSubVersion < rhs.nSubVersion)
        return LESS;
      else if (lhs.nSubVersion > rhs.nSubVersion)
        return GREATER;
    }
    
    if (lhs.m_nVersionPartsToCompare & SUBSUB || rhs.m_nVersionPartsToCompare & SUBSUB)
    {
      if (lhs.nSubSubVersion < rhs.nSubSubVersion)
        return LESS;
      else if (lhs.nSubSubVersion > rhs.nSubSubVersion)
        return GREATER;
    }

    return EQUAL;
  }


  bool operator < (const Version& lhs, const Version& rhs)
  {
    return Version::compare(lhs, rhs) == Version::LESS;
  }


  bool operator > (const Version& lhs, const Version& rhs)
  {
    return Version::compare(lhs, rhs) == Version::GREATER;
  }


  bool operator == (const Version& lhs, const Version& rhs)
  {
    return Version::compare(lhs, rhs) == Version::EQUAL;
  }


  bool operator != (const Version& lhs, const Version& rhs)
  {
    return !(operator ==(lhs, rhs));
  }


  bool operator <= (const Version& lhs, const Version& rhs)
  {
    return operator < (lhs, rhs) || operator == (lhs, rhs);
  }


  bool operator >= (const Version& lhs, const Version& rhs)
  {
    return operator > (lhs, rhs) || operator == (lhs, rhs);
  }


  std::ostream& operator << (std::ostream& os, const Version& version)
  {
    os << version.nMainVersion << " " << version.nSubVersion << " " << version.nSubSubVersion << " " << version.strCommitHash;
    return os;
  }


  std::istream& operator >> (std::istream& is, Version& version)
  {
    is >> version.nMainVersion >> version.nSubVersion >> version.nSubSubVersion >> version.strCommitHash;
    return is;
  }

}