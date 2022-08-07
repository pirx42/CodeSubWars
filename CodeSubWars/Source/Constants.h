// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class Constants
  {
    public:
      static const double CRITICAL_CONSUMING_TIME;

      static const QString WEBSITE;
      static const QString AUTHOR;
      static const QString EMAIL;

      static const Version& getVersion();
  };

}