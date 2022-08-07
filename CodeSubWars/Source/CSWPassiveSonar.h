// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSonar.h"
#include "CSWISoundReceiver.h"

namespace CodeSubWars
{

  class CSWSoundReceiver;
  class CSWPassiveSonar3DView;
  class CSWPassiveSonarVisualizer;

  /**
   * This class specializes a sonar to a passive sonar. It detects environmental sounds of sound emitting objects.
   */
  class CSWPassiveSonar : public CSWSonar,
                          public CSWISoundReceiver
  {
    public:
      typedef std::shared_ptr<CSWPassiveSonar> PtrType;

      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject);

      virtual ~CSWPassiveSonar();

      //defined methods from CSWEquipment
      virtual QWidget* getInformationWidget(QWidget* pParent);

      //defined methods from IUpdateable
      virtual void update();

      //defined methods from CSWSonar
      virtual void draw();
      virtual Vector3D getAdditionalDirection(const Vector3D& vecCurrentDir, const Vector3D& vecUp) const;

      //defined methods from ISoundReceiver
      virtual const Vector3D& getDirection() const;
    
      /**
       * Returns the current set angle of beam.
       * @returns The current set angle of beam.
       */
      virtual const double& getAngleOfBeam() const;

      /**
       * Sets the current angle of beam within emitted sound are detected. If setting a larger angle 
       * the resolution will be decreased. Initially the angle is 15 degree.
       * @param fAngle The angle of beam that should be used in degree. This is clamped to [0, 180].
       */
      virtual void setAngleOfBeam(const double& fAngle);

      virtual void addSound(const Vector3D& vecWorldTPosEmitter, const double& fLevel);

      /**
       * Returns the currently detected sound pressure level (SPL) in decibel (dB).
       * Typical values are (depend on distance):\n
       *   Main engine of large submarine on max intensity:         about 100dB\n
       *   Jet oars of large submarine on max intensity:            about  80dB\n
       *   Explosion of torpedo/mine:                               about 150dB\n
       *   Engine of torpedo:                                       about  30dB\n
       * @return The currently detected sound pressure level in dB.
       */
      virtual const double& getLevel() const;
    
      virtual void reset();
    
      /**
       * Adjusts the currently set local direction to point to a maximum level. By doing this 
       * the automatic rotation will be disabled and the direction mode is set to LOCAL_DIRECTION.
       * @param fTimeToFind The maximal relative time within the maximum level must be found. 
       *                    If this time has reached the adjusting will stop at the currently found 
       *                    maximum. This time will never be exceeded.
       */
      void adjustDirectionToMaximum(const double& fTimeToFind);
    
      /**
       * Returns true if the sonar is currently adjusting.
       * @returns True if the sonar is currently adjusting otherwise false.
       */
      bool isAdjusting() const;

      /**
       * Returns true if a previously started adjusting has been finished.
       * @returns True if a previously started adjusting has been finished otherwise false.
       */
      bool hasAdjusted() const;

      friend CSWPassiveSonar3DView;
    protected:  
      CSWPassiveSonar(const std::string& strName, const Matrix44D& matBaseTObject);

      void findMaximum();
  
  
      //property implementations
      std::shared_ptr<CSWSoundReceiver> m_pSoundReceiver;

      boost::circular_buffer<std::pair<Vector3D, double> > m_LastScanValues; //direction in world coordinates, level
      CSWPassiveSonarVisualizer* m_pVisualizer;
    
      double m_fAdjustingEndTime;
      bool m_bFoundMaximum;
      bool m_bFindingMaximum;
      double m_fMaximumLevel;
      double m_fLastLevel;
      Vector3D m_vecMaximumDirection;
      double m_fStepSize;
      std::vector<std::pair<double, double> > m_Directions;
      int m_nCurrentDirectionIdx;
      int m_nMaximumDirectionIdx;
      int m_nNumberDirectionStepsWithoutNewMaximum;
      int m_nNumberBadDirections;
      int m_nNumberRestarts;
      int m_nDirectionChangeOrientation;
    
      std::vector<Vector3D> m_Vertices;
  };

}