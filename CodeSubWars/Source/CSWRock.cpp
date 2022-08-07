// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWRock.h"


namespace CodeSubWars
{

  CSWRock::~CSWRock()
  {
  }


  CSWRock::CSWRock(const std::string& strName, const Matrix44D& matBaseTObject, const Size3D& size)
  : CSWDamDynSolCol(strName, matBaseTObject, 
                    Mesh::createBox(Matrix44D(Vector3D(-size.getWidth()/2.0, 
                                                       -size.getHeight()/2.0, 
                                                       -size.getDepth()/2.0)),
                                    size),
                    true)
  {
    assert(size.getWidth() > 0 && size.getHeight() > 0 && size.getDepth() > 0);
    setColor(Vector4D(0.7, 0.7, 0.7, 1));
  }

}