// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  /**
   * This is the base class for all messages.
   */
  class CSWIEquipmentVisualizer
  {
    public:
      virtual ~CSWIEquipmentVisualizer() {}
    
      virtual void updateContent() = 0;
  };

}