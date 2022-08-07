// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <cassert>

namespace ARSTD
{

  class Utilities
  {
    public:
      template <typename Type>
      static std::string toString(const Type& value)
      {
        std::stringstream ss;
        ss << value;
        return ss.str();
      }
  };

} //namespace ARSTD