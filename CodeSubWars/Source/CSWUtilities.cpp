// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWUtilities.h"
#include "glut.h"


namespace CodeSubWars
{

  Vector3D CSWUtilities::determineAngles(const Vector3D& vecXDirection,
                                         const Vector3D& vecYDirection,
                                         const Vector3D& vecZDirection)
  {
    assert(fabs(vecXDirection.getLength() - 1.0) < EPSILON);
    assert(fabs(vecYDirection.getLength() - 1.0) < EPSILON);
    assert(fabs(vecZDirection.getLength() - 1.0) < EPSILON);
    Vector3D angle;

    //direction (rotation about y axis)
    if (vecZDirection.x < 0)
    {
      //Quadrant I and IV
      angle.y = -atan2(vecZDirection.x, vecZDirection.z)*180/PI;
    }
    else if (vecZDirection.x > 0)
    {
      //Quadrant II and III
      angle.y = fabs(atan2(vecZDirection.x, vecZDirection.z)*180/PI - 360);
    }
    else //vecZDirection.x == 0
    {
      angle.y = vecZDirection.z > 0 ? 0 : 180;
    }
    if (angle.y >= 360) angle.y = fmod(angle.y, 360.0);

    //inclination (rotation about x axis)
    angle.x = asin(vecZDirection.y)*180.0/PI;

    //axial inclination (rotation about z axis)
    Matrix44D mat;
    mat.getYAxis() = Vector3D(0, 1, 0); 
    if (mat.getYAxis() % vecZDirection != Vector3D(0, 0, 0))
    {
      mat.getXAxis() = mat.getYAxis() % vecZDirection;
      mat.getZAxis() = mat.getXAxis() % mat.getYAxis();
      mat.makeHomogeneous(Matrix44D::XAXIS);
    
      Vector3D vecNewX = Matrix44D::inverse(mat)*vecXDirection;
      angle.z = atan2(vecNewX.y, vecNewX.x)*180.0/PI;
    }
    else
    {
      angle.z = 0;
    }

    assert(angle.x >= -90 && angle.x <= 90);
    assert(angle.y >= 0 && angle.y < 360);
    assert(angle.z >= -180 && angle.z <= 180);
  
    return angle;
  }


  Vector3D CSWUtilities::determineAngles(const Matrix44D& mat)
  {
    return determineAngles(mat.getXAxis(), mat.getYAxis(), mat.getZAxis());
  }


  Vector3D CSWUtilities::determineAngles(const Matrix33D& mat)
  {
    return determineAngles(mat.getXAxis(), mat.getYAxis(), mat.getZAxis());
  }


  Vector3D CSWUtilities::getRandomPosition()
  {
    return Vector3D((rand() - RAND_MAX/2)/static_cast<double>(RAND_MAX), 
                    (rand() - RAND_MAX/2)/static_cast<double>(RAND_MAX), 
                    (rand() - RAND_MAX/2)/static_cast<double>(RAND_MAX));
  }


  Vector3D CSWUtilities::getRandomDirection()
  {
    return getRandomPosition().getNormalized();
  }


  Matrix44D CSWUtilities::getRandomOrientation()
  {
    Matrix44D mat;
    mat.getXAxis() = getRandomDirection();
    mat.getYAxis() = getRandomDirection();
    mat.getZAxis() = mat.getXAxis() % mat.getYAxis();
    mat.getYAxis() = mat.getZAxis() % mat.getXAxis();
    mat.makeHomogeneous(Matrix44D::XAXIS);
    return mat;
  }


  QString CSWUtilities::getUniqueFilename(const QString strPath)
  {
    if (!QDir().exists(strPath))
      throw std::runtime_error("could not determine unique file");

    QString strFileName;
    int nCntTries(50);
    bool bSuccess(false);
    while (!bSuccess && --nCntTries > 0)
    {
      strFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd__hh-mm-ss-zzz");
      std::stringstream path;
      path << strPath.toStdString() << "/" << strFileName.toStdString();

      bSuccess = !QFile(path.str().c_str()).exists();
    }
  
    if (!bSuccess)
      throw std::runtime_error("could not determine unique file");

    return strFileName;
  }


  Vector4D CSWUtilities::interpolateComponentsLinear(const Vector4D& vec1, const Vector4D& vec2, double fFactor)
  {
    if (fFactor <= 0)
      return vec1;
    if (fFactor >= 1)
      return vec2;

    return Vector4D(vec1.x + (vec2.x - vec1.x)*fFactor,
                    vec1.y + (vec2.y - vec1.y)*fFactor,
                    vec1.z + (vec2.z - vec1.z)*fFactor,
                    vec1.w + (vec2.w - vec1.w)*fFactor);
  }


  std::pair<bool, Vector3D> CSWUtilities::getWindowCoords(const Vector3D& vecWorldTPos)
  {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    Matrix44D matCurrentModelview;
    glGetDoublev(GL_MODELVIEW_MATRIX, matCurrentModelview.pData);

    Matrix44D matCurrentProjection;
    glGetDoublev(GL_PROJECTION_MATRIX, matCurrentProjection.pData);

    Vector3D vecWindowTPosition;
    int nResult = gluProject(vecWorldTPos.x, vecWorldTPos.y, vecWorldTPos.z,
                             matCurrentModelview.pData,
                             matCurrentProjection.pData,
                             viewport,
                             &vecWindowTPosition.x, &vecWindowTPosition.y, &vecWindowTPosition.z);
  
    return std::make_pair(nResult == GL_TRUE, vecWindowTPosition);
  }


  std::vector<Vector3D> CSWUtilities::getWindowCoords(const std::vector<Vector3D> positions)
  {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    Matrix44D matCurrentModelview;
    glGetDoublev(GL_MODELVIEW_MATRIX, matCurrentModelview.pData);

    Matrix44D matCurrentProjection;
    glGetDoublev(GL_PROJECTION_MATRIX, matCurrentProjection.pData);

    std::vector<Vector3D> windowPositions(positions.size());
    int nResult = GL_TRUE;
    std::vector<Vector3D>::const_iterator it = positions.begin();
    std::vector<Vector3D>::iterator itWndPos = windowPositions.begin();
    for (; it != positions.end() && nResult == GL_TRUE; ++it, ++itWndPos)
    {
      nResult &= gluProject(it->x, it->y, it->z,
                            matCurrentModelview.pData,
                            matCurrentProjection.pData,
                            viewport,
                            &itWndPos->x, &itWndPos->y, &itWndPos->z);
    }
  
    if (nResult != GL_TRUE)
      windowPositions.clear();
    return windowPositions;
  }


  Vector3D CSWUtilities::calcColorR2G2B(double fValue)
  {
    Vector3D col(0, 0, 0);
    if (fValue < 0)
      return col;
    if (fValue > 1)
      return Vector3D(0, 0, 1);

    if (fValue < 0.25)
      col.r = 1 ;
    else if (fValue < 0.5)
      col.r = -4*fValue + 2;

    if (fValue < 0.25)
      col.g = 4*fValue; 
    else if (fValue < 0.75)
      col.g = 1;
    else
      col.g = -4*fValue + 4;

    if (fValue > 0.75)
      col.b = 1;
    else if (fValue > 0.5)
      col.b = 4*fValue - 2;

    return col;
  }


  bool CSWUtilities::isValidSubmarineFile(const std::string& strAbsPath, const std::string& strSubmarineName)
  {
    QFile file(strAbsPath.c_str());
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
      QString strLine = stream.readLine();
      strLine.replace(" ", "");
      if (strLine.contains(QString("class") + strSubmarineName.c_str() + "(CSWPySubmarine):"))
      {
        file.close();
        return true;
      }
    }
    file.close();
    return false;
  }


  CSWUtilities::SubmarineFileContainer CSWUtilities::determineAvailableSubmarines(const std::string& strAbsPath)
  {
    SubmarineFileContainer submarines;
    QDir d(QString(strAbsPath.c_str()));
  
    d.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    d.setSorting(QDir::Name);

    QFileInfoList list = d.entryInfoList();
    // if directory does not exist do nothing
    if (list.isEmpty())
      return CSWUtilities::SubmarineFileContainer();

    // scan given directory
    for (int i = 0; i < list.size(); ++i) 
    {
      QFileInfo fi = list.at(i);
  
      if (fi.isFile() && fi.suffix() == "py")
      {
        if (CSWUtilities::isValidSubmarineFile(fi.absoluteFilePath().toStdString(), fi.baseName().toStdString()))
        {
          submarines[fi.baseName().toStdString()] = fi.absoluteFilePath().toStdString();
        }
      }
      else if (fi.isDir() && fi.fileName() != "." && fi.fileName() != ".." && !fi.isSymLink())
      {
        // subdir found -> scan
        SubmarineFileContainer subs = determineAvailableSubmarines(fi.absoluteFilePath().toStdString());
        submarines.insert(subs.begin(), subs.end());
      }
    }
    return submarines;
  }

}