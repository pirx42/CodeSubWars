// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property have a chance for doing something periodically.
   */
  class CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWIUpdateable> PtrType;

      virtual ~CSWIUpdateable() {}

      //This method must be overriden in the specialized class. This methode is called periodically about every 10ms.
      //The main function should be checking sensors, seting up commands or something like that. The specialized class can implement everything 
      //here, but the execution time must not exceed 6ms in average. If it does the object will be kicked from the world!
      virtual void update() = 0;
  };

}