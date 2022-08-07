// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class CSWSoundReceiver
  {
    public:
      typedef std::shared_ptr<CSWSoundReceiver> PtrType;

      static PtrType create();
      ~CSWSoundReceiver();
    
      const double& getAngleOfBeam() const;
      void setAngleOfBeam(const double& fAngle);
      void addSound(const Vector3D& vecWorldTPosEmitter, const Vector3D& vecWorldTPosReceiver, 
                    const Vector3D& vecWorldTDirReceiver, const double& fLevel);
      const double& getLevel() const;    
      void reset();
    
    protected:
      CSWSoundReceiver();
    
      double m_fAngleOfBeam;
      double m_fLevel;
      mutable double m_fLevelIndB;
      mutable bool m_bUpToDate;
  };

}