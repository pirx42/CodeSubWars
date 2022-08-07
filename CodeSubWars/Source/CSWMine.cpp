// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWMine.h"
//#include "CSWScanBox.h"


namespace CodeSubWars
{

  CSWMine::~CSWMine()
  {
  }


  // void CSWMine::draw()
  // {
  //   CSWWeapon::draw();
  //   m_pScanBox->draw();
  // }


  void CSWMine::addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls)
  {
    CSWWeapon::addImpuls(vecWorldTPosition, vecWorldTImpuls);
    fireImmediately();
  }


  CSWMine::CSWMine(const std::string& strName, const Matrix44D& matBaseTObject, double fEdgeLength)
  : CSWWeapon(strName, matBaseTObject, Size3D(fEdgeLength, fEdgeLength, fEdgeLength))//,
  //  m_pScanBox(CSWScanBox::create(strName + ".ScanBox", Matrix44D(), 10))
  {
  //  attach(m_pScanBox);
  }

}