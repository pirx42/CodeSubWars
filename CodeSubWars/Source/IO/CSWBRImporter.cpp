// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWBRImporter.h"

namespace CodeSubWars
{

  double CSWBRImporter::m_fProgress = 0;

  bool CSWBRImporter::loadREC(const QString& strFileName, TimeSliceDataContainer& dataContainer)
  {
    m_fProgress = 0;
  
    boost::iostreams::file_source fs(strFileName.toStdString(), std::ios::in | std::ios::binary);
    if (!fs.is_open())
      return false;
  
    //check header
    char buff[6];
    fs.read(buff, 6);
    if (std::string(buff, 6) != "CSW.BR")
      return false;

    Version version(0, 0, 0, "", ARSTD::Version::MAIN_AND_SUB | ARSTD::Version::SUBSUB);
    fs.read(reinterpret_cast<char*>(&version.nMainVersion), 1);
    fs.read(reinterpret_cast<char*>(&version.nSubVersion), 1);
    fs.read(reinterpret_cast<char*>(&version.nSubSubVersion), 1);

    //determine data size to be read  
  //  int nOldPos = fs. .tell();
  //  fs.seekg(0, std::ios::end);
  //  int nSize = static_cast<int>(fs.tellg()) - nOldPos;
  //  fs.seek(nOldPos, std::ios::beg);

    //configure input stream depend on version
    boost::iostreams::filtering_istream is;
    //if (version >= Version(0, 4, 1, 0, ARSTD::Version::MAIN_AND_SUB | ARSTD::Version::SUBSUB) && 
    //    version <= Version(0, 4, 6, 0, ARSTD::Version::MAIN_AND_SUB | ARSTD::Version::SUBSUB))
    //{
    //  is.push(boost::iostreams::zlib_decompressor());
    //}  
    is.push(fs);
    if (!is.is_complete())
      return false;

    if (version >= Version(0, 2, 0, "", ARSTD::Version::MAIN_AND_SUB | ARSTD::Version::SUBSUB) && 
        version <= Version(0, 4, 6, "", ARSTD::Version::MAIN_AND_SUB | ARSTD::Version::SUBSUB))
    {
      return CSWBRImporter::loadREC_v0_2_0__v0_4_6(is, dataContainer);
    }
  
    return false;
  }


  double CSWBRImporter::getProgress()
  {
    return m_fProgress;
  }


  bool CSWBRImporter::loadREC_v0_2_0__v0_4_6(boost::iostreams::filtering_istream& is, TimeSliceDataContainer& dataContainer)
  {
    if (!is.is_complete())
      return false;
  
    m_fProgress = 0;
    float fMinTime = std::numeric_limits<float>::max();
    float fMaxTime = 0;  
    float fLastTime = 0;

    while (!is.eof() || !is.fail())
    {
      TimeSliceData data;
    
      float fTime;
      is.read(reinterpret_cast<char*>(&fTime), 4);
      if (is.eof() || is.fail())
        return true;

      if (fTime < fMinTime)
        fMinTime = fTime;
      if (fTime > fMaxTime)
        fMaxTime = fTime;
      if (fTime < fLastTime)
        return true;
      fLastTime = fTime;
    
      ARSTD::uint16 nNumObjects;
      is.read(reinterpret_cast<char*>(&nNumObjects), 2);
      if (is.eof() || is.fail())
        return true;

      ObjectData objData;
      for (int nCnt = 0; nCnt < nNumObjects; ++nCnt)
      {      
        ARSTD::uint16 nNameSize;
        is.read(reinterpret_cast<char*>(&nNameSize), 2);
        if (is.eof() || is.fail())
          return true;
        char* buff = new char[nNameSize];
        is.read(buff, nNameSize);
        objData.strName = std::string(buff, nNameSize);
        delete [] buff;
        if (is.eof() || is.fail())
          return true;

        Matrix44F mat;
        is.read(reinterpret_cast<char*>(mat.pData), 16*4);
        if (is.eof() || is.fail())
          return true;
        objData.matWorldTObject = static_cast<Matrix44D>(mat);
      
        Size3F size;
        is.read(reinterpret_cast<char*>(&size), 3*4);
        if (is.eof() || is.fail())
          return true;
        objData.size = static_cast<Size3D>(size);

        Vector4F color;
        is.read(reinterpret_cast<char*>(&color), 4*4);
        if (is.eof() || is.fail())
          return true;
        objData.color = static_cast<Vector4D>(color);

        data[objData.strName] = objData;
      }
    
      if (!data.empty())
        dataContainer.push_back(std::make_pair(fTime, data));

  //    m_fProgress = static_cast<double>(is.tellg())*100.0/nSize;
    }
    m_fProgress = 1.0;

    return true;
  }

}