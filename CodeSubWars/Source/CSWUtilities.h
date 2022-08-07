// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWUtilities
  {
    public:
      typedef std::map<std::string, std::string> SubmarineFileContainer;  //submarine name, abs file name

      ~CSWUtilities() {}
  
      //given in world coord system; returned in world coord system in degree
      static Vector3D determineAngles(const Vector3D& vecXDirection,
                                      const Vector3D& vecYDirection,
                                      const Vector3D& vecZDirection);
      static Vector3D determineAngles(const Matrix44D& mat);
      static Vector3D determineAngles(const Matrix33D& mat);

      static Vector3D getRandomPosition();
      static Vector3D getRandomDirection();
      static Matrix44D getRandomOrientation();

      static QString getUniqueFilename(const QString strPath);
    
      static Vector4D interpolateComponentsLinear(const Vector4D& vec1, const Vector4D& vec2, double fFactor);

      static std::pair<bool, Vector3D> getWindowCoords(const Vector3D& vecWorldTPos);

      static std::vector<Vector3D> getWindowCoords(const std::vector<Vector3D> positions);
    
      static Vector3D calcColorR2G2B(double fValue);


      static bool isValidSubmarineFile(const std::string& strAbsPath, const std::string& strSubmarineName);
      static SubmarineFileContainer determineAvailableSubmarines(const std::string& strAbsPath);
  };

}