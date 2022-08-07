// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWBRImporter
  {
    public:
      struct ObjectData
      {
        std::string strName;
        Matrix44D matWorldTObject;
        Size3D size;
        Vector4D color;
      };
      typedef std::map<std::string, ObjectData> TimeSliceData;
      typedef std::vector<std::pair<double, TimeSliceData> > TimeSliceDataContainer;


      static bool loadREC(const QString& strFileName, TimeSliceDataContainer& dataContainer);
    
      static double getProgress();
    
    protected:
      ~CSWBRImporter() {}
    
      //format description:
      // header: 
      //  CSW.BR                            (6 byte (characters))
      //  <nMainVersion>                    (1 byte (integral))
      //  <nSubVersion>                     (1 byte (integral))
      //  <nSubSubVersion>                  (1 byte (integral))
      // list of object data per time slice. 
      // one time slice data constists of:
      //  <time of slice>                   (4 byte (floating point))
      //  <number of objects of time slice> (2 byte (integral))
      //    <name length of object>         (2 byte (integral))
      //    <name of object>                (1 byte per character; variable length)
      //    <transform worldTobject>        (16*4 byte (floating point = row based 4x4 matrix))
      //    <size of object>                (3*4 byte (floating point = WxHxD of object)
      //    <color of object>               (4*4 byte (floating point = r, g, b, a of object; each in range [0, 1])
      static bool loadREC_v0_2_0__v0_4_6(boost::iostreams::filtering_istream& is, TimeSliceDataContainer& dataContainer);

      static double m_fProgress;

  };

}