// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWActuator.h"
#include "CSWISoundEmitter.h"

namespace CodeSubWars
{

  class CSWEngineVisualizer;

  /**
   * This class encapsulates a machine that can "emit" a force in a direction. The maximal force is depend on the size of the 
   * engine (larger engines have more power than smaller ones). This has the property to emit sound that is depend on the 
   * currently emitting force. 
   */
  class CSWEngine : public CSWActuator,
                    public CSWISoundEmitter
  {
    public:
      typedef std::shared_ptr<CSWEngine> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject,
                            const Size3D& size, bool bAllowDirectionChanges = false);

      virtual ~CSWEngine();

      //defined methods from CSWEquipment
      virtual QWidget* getInformationWidget(QWidget* pParent);

      //defined methods from ISoundEmitter
      virtual bool isSoundEmitterActive() const;
      virtual void initializeSoundEmission();
      virtual void emitSound(std::shared_ptr<CSWObject> pObject);
      virtual void finalizeSoundEmission();

      //in world coord system
      const Vector3D getAppliedForce() const;
    
      /**
       * Sets the direction relative to local initial set coordinate system. If a direction is given that
       * the engine can not adjust, the maximal direction nearest to the given direction will be set. The
       * maximal excursion of the new direction relative to it initial direction is 45 degree. The angular
       * velocity for adjusting is 22.5 degree/second.
       * @param fHAngle The new horizontal angle, positiv means to the right. (in degree)
       * @param fVAngle The new vertical angle, positiv means to the up. (in degree)
       */
      virtual void setDirection(const double& fHAngle, const double& fVAngle);

      /**
       * Sets the direction relative to local initial set coordinate system. If a direction is given that
       * the engine can not adjust, the maximal direction nearest to the given direction will be set. The
       * maximal excursion of the new direction relative to it initial direction is 45 degree. The angular
       * velocity for adjusting is 22.5 degree/second.
       * @param vecDirection The new direction relative to its parent.
       */
      virtual void setDirection(const Vector3D& vecDirection);
    
      /**
       * Returns the normalized direction in the world coordinate system. This is the "forward" direction for positive
       * intensity values.
       * @return The current direction in woorld coordinate system.
       */
      const Vector3D& getDirection() const;

      /**
       * Sets the intensity of the engine relative to its parent. The valid range is [-1, 1], where positive means toward and
       * negative backward direction. Non-valid values are clipped against the valid range.
       * @param fIntensity The new intensity in range [-1, 1].
       */
      void setIntensity(const double fIntensity);

      /**
       * Returns the current set normalized intensity in range [-1, 1].
       */
      const double& getIntensity();
 
      friend CSWEngineVisualizer;
    protected:
      CSWEngine(const std::string& strName, const Matrix44D& matBaseTObject,
                const Size3D& size, bool bAllowDirectionChanges);

      static const double MAX_OAR_ANGLE; //in rad
      static const double ANGLULAR_VELOCITY; //in rad/s

      double m_fMaxForce;
      double m_fIntensity;
      bool m_bAllowDirectionChanges;
    
      CSWEngineVisualizer* m_pVisualizer;
  };

}