// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#include "PrecompiledHeader.h"
#include "BuildDetails.h"
#include "Constants.h"

namespace CodeSubWars
{

  const double Constants::CRITICAL_CONSUMING_TIME = 0.006;

  const QString Constants::WEBSITE = "www.codesubwars.org";
  const QString Constants::AUTHOR = "Andreas Rose";
  const QString Constants::EMAIL = "andreas@rose.st";

  const Version& Constants::getVersion()
  {
    static Version v(0, 4, 7, BuildDetails::COMMIT_SHORT);
    return v;
  }

}