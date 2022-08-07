// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  /**
   * Objects with this property are visible in the world.
   */
  class CSWISolid 
  {
    public:
      typedef std::shared_ptr<CSWISolid> PtrType;

      virtual ~CSWISolid() {}

      virtual void draw() = 0;

      //given in locale coordsystem
      virtual const Mesh::PtrType getSurface() const = 0;
      virtual const BoundingBoxD& getSurfaceBoundingBox() const = 0;

      //Sets the color of the submarine.
      //@param vecColor The new rgba-color.
      virtual void setColor(const Vector4D& vecColor) = 0;

      //Returns the color of the object.
      //@return The rgba-color of the object.
      virtual const Vector4D& getColor() = 0;
  };

}