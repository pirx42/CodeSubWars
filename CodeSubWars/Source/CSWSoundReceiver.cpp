// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWObject.h"
#include "CSWISoundReceiver.h"
#include "CSWSoundReceiver.h"


namespace CodeSubWars
{

  CSWSoundReceiver::PtrType CSWSoundReceiver::create()
  {
    return PtrType(new CSWSoundReceiver());
  }


  CSWSoundReceiver::~CSWSoundReceiver()
  {
  }
    

  const double& CSWSoundReceiver::getAngleOfBeam() const
  {
    return m_fAngleOfBeam;
  }


  void CSWSoundReceiver::setAngleOfBeam(const double& fAngle)
  {
    if (fAngle < 0)
      m_fAngleOfBeam = 0;
    else if (fAngle > 180)
      m_fAngleOfBeam = 180;
    else
      m_fAngleOfBeam = fAngle;
  }


  void CSWSoundReceiver::addSound(const Vector3D& vecWorldTPosEmitter, const Vector3D& vecWorldTPosReceiver, 
                                  const Vector3D& vecWorldTDirReceiver, const double& fLevel)
  {
    if (fLevel <= 0)
      return;

    double fDistance = (vecWorldTPosEmitter - vecWorldTPosReceiver).getLength();
    if (fDistance < EPSILON)
    {
      m_fLevel += fLevel;
      m_bUpToDate = false;
      return;
    }
    Vector3D vecWorldTReceiverToEmitterDir = (vecWorldTPosEmitter - vecWorldTPosReceiver)/fDistance;
    double fAngle = ARSTD::getInDegree(vecWorldTDirReceiver.getAngleBetween(vecWorldTReceiverToEmitterDir));
    if (fAngle > m_fAngleOfBeam)
      return;
    double fCos = cos(ARSTD::PI_HALF*fAngle/m_fAngleOfBeam);
    assert(fCos >= 0);

    if (fDistance < 1)
      fDistance = 1;
    m_fLevel += fCos*fCos*fCos*fLevel/fDistance; //sound pressure is proportional to 1/r !!!!
    m_bUpToDate = false;
  }


  const double& CSWSoundReceiver::getLevel() const
  {
    if (!m_bUpToDate) 
    {
      m_fLevelIndB = 20*ARSTD::log10(m_fLevel);
      m_bUpToDate = true;
    }
    return m_fLevelIndB;
  }


  void CSWSoundReceiver::reset()
  {
    m_fLevel = 0;
    m_fLevelIndB = 0;
    m_bUpToDate = false;
  }


  CSWSoundReceiver::CSWSoundReceiver()
  : m_fAngleOfBeam(45),
    m_fLevel(0),
    m_fLevelIndB(0),
    m_bUpToDate(false)
  {
  }


}