// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <string>
#include "arstd/Common/Types.h"

namespace ARSTD
{

  struct Version
  {
    public :
      enum VersionParts
      {
        MAIN = 1,
        SUB = 2,
        SUBSUB = 4,

        MAIN_AND_SUB = 3,
        ALL = 7
      };

      explicit Version(uint nMainVersion = 0, uint nSubVersion = 0, uint nSubSubVersion = 0, const std::string& commitHash = "",
                       int nVersionPartsToCompare = MAIN_AND_SUB);
      explicit Version(const std::string& str, int nVersionPartsToCompare = MAIN_AND_SUB);

      std::string getAsString() const;

      uint nMainVersion;
      uint nSubVersion;
      uint nSubSubVersion;
      std::string strCommitHash;

      friend bool operator < (const Version& lhs, const Version& rhs);
      friend bool operator > (const Version& lhs, const Version& rhs);
      friend bool operator == (const Version& lhs, const Version& rhs);

    protected:
      enum Compare
      { 
        LESS = 0,
        EQUAL,
        GREATER
      };

      static const Compare compare(const Version& lhs, const Version& rhs);

      int m_nVersionPartsToCompare;
  };

  bool operator < (const Version& lhs, const Version& rhs);
  bool operator > (const Version& lhs, const Version& rhs);
  bool operator == (const Version& lhs, const Version& rhs);
  bool operator != (const Version& lhs, const Version& rhs);
  bool operator <= (const Version& lhs, const Version& rhs);
  bool operator >= (const Version& lhs, const Version& rhs);

  std::ostream& operator << (std::ostream& os, const Version& version);
  std::istream& operator >> (std::istream& is, Version& version);

}


