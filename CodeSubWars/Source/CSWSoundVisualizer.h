// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

namespace CodeSubWars
{

  class CSWSoundVisualizer
  {
    public:
      typedef std::shared_ptr<CSWSoundVisualizer> PtrType;

      static PtrType create();

      ~CSWSoundVisualizer();

      void addSound(const Vector3D& vecPosition, const double& fIntensity, long nEmitterID);

      void clear();
        
      void draw();
    
    protected:
      struct SoundEntry
      {
        SoundEntry() : vecPosition(0, 0, 0), fStartIntensity(0), fStartTime(-1) {}
      
        Vector3D vecPosition;
        double fStartIntensity;
        double fStartTime;
      };

      typedef std::map<int, SoundEntry> SoundEmits;    //normalized time to 0.5s -> sound entry
      typedef std::map<long, SoundEmits> SoundContainer;  //emitter id -> sound emits
  
      CSWSoundVisualizer();

      SoundContainer m_Sounds;
  };

}