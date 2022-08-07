// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

namespace CodeSubWars
{

  class CSWExplosionVisualizer
  {
    public:
      typedef std::shared_ptr<CSWExplosionVisualizer> PtrType;

      static PtrType create();

      ~CSWExplosionVisualizer();

      void addExplosion(const Vector3D& vecPosition, const double& fRadius);

      void clear();
        
      void draw();
    
    protected:
      struct ExplosionEntry
      {
        ExplosionEntry() : vecPosition(0, 0, 0), fRadius(0), fTimeToStop(-1), bDrawn(false) {}
      
        Vector3D vecPosition;
        double fRadius;
        double fTimeToStop;
        bool bDrawn;
      };
    
      typedef std::vector<ExplosionEntry> ExplosionContainer;
  
      CSWExplosionVisualizer();

      ExplosionContainer m_Explosions;
  };

}