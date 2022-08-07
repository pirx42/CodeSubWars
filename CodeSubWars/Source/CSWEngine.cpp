// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CSWEngine.h"
#include "CSWWorld.h"
#include "CSWEngineVisualizer.h"
#include "CSWSoundVisualizer.h"
#include "CSWISoundReceiver.h"

namespace CodeSubWars
{

  const double CSWEngine::MAX_OAR_ANGLE = getInRad(45);
  const double CSWEngine::ANGLULAR_VELOCITY = getInRad(22.5);


  CSWEngine::PtrType CSWEngine::create(const std::string& strName, const Matrix44D& matBaseTObject,
                                       const Size3D& size, bool bAllowDirectionChanges)
  {
    return PtrType(new CSWEngine(strName, matBaseTObject, size, bAllowDirectionChanges));
  }


  CSWEngine::~CSWEngine()
  {
  }


  QWidget* CSWEngine::getInformationWidget(QWidget* pParent)
  {
    if (!m_pVisualizer)
      m_pVisualizer = new CSWEngineVisualizer(pParent, std::dynamic_pointer_cast<CSWEngine>(getSharedThis()));
    return m_pVisualizer;
  }


  bool CSWEngine::isSoundEmitterActive() const
  {
    return fabs(m_fIntensity) > EPSILON;
  }


  void CSWEngine::initializeSoundEmission()
  {
    CSWWorld::getInstance()->getSoundVisualizer()->addSound(getWorldTransform().getTranslation(), fabs(m_fIntensity*m_fMaxForce), reinterpret_cast<unsigned long long>(this));
  }


  void CSWEngine::emitSound(CSWObject::PtrType pObject)
  {
    if (CSWISoundReceiver::PtrType pSoundReceiver = std::dynamic_pointer_cast<CSWISoundReceiver>(pObject))
    {
      pSoundReceiver->addSound(getWorldTransform().getTranslation(), fabs(m_fIntensity*m_fMaxForce));
    }
  }


  void CSWEngine::finalizeSoundEmission()
  {
  }


  void CSWEngine::setDirection(const double& fHAngle, const double& fVAngle)
  {
    Vector3D vecNewDirection = m_vecObjectTInitialDirection.getRotated(m_vecObjectTInitialUp, -ARSTD::getInRad(fHAngle));
    vecNewDirection = vecNewDirection.getRotated(m_vecObjectTInitialUp % vecNewDirection, -ARSTD::getInRad(fVAngle));
    setDirection(vecNewDirection);
  }


  void CSWEngine::setDirection(const Vector3D& vecDirection)
  {
    if (!m_bAllowDirectionChanges)
      return;

    setRelativeDirection(vecDirection);
  }


  const Vector3D& CSWEngine::getDirection() const
  {
    return getWorldTransform().getZAxis();
  }


  void CSWEngine::setIntensity(const double fIntensity)
  {
    if (fIntensity > 1.0)
      m_fIntensity = 1.0;
    else if (fIntensity < -1.0)
      m_fIntensity = -1.0;
    else
      m_fIntensity = fIntensity;
  }


  const double& CSWEngine::getIntensity()
  {
    return m_fIntensity;
  }


  const Vector3D CSWEngine::getAppliedForce() const
  {
    return getDirection()*(m_fMaxForce*m_fIntensity);
  }


  CSWEngine::CSWEngine(const std::string& strName, const Matrix44D& matBaseTObject,
                       const Size3D& size, bool bAllowDirectionChanges)
  : CSWActuator(strName, matBaseTObject, MAX_OAR_ANGLE, ANGLULAR_VELOCITY),
    m_fMaxForce(size.getVolume()*1e+6),
    m_fIntensity(0),
    m_bAllowDirectionChanges(bAllowDirectionChanges),
    m_pVisualizer(NULL)
  {
  }

}