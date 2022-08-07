// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSonar.h"

namespace CodeSubWars
{

  class CSWScanRay;
  class CSWActiveSonar3DView;
  class CSWActiveSonarVisualizer;

  /**
   * This class specializes a sonar to an active sonar. The time difference between sent and received sonic waves gives
   * the distance to detected solid objects within a small region. 
   * The three modes for direction, velocity and range have now additional meanings:
   *
   * - ScanDirectionModes:\n
   *   - FULL             Scans additionally vertical about 30 degree. (Default)\n
   *   - FRONT            Scans additionally vertical about 60 degree in forward direction.\n
   *   - BACK             Scans additionally vertical about 60 degree in backward direction.\n 
   *   - LOCAL_DIRECTION  Scans additionally vertical about 30 degree in currently set custom local direction.\n
   *   - GLOBAL_POSITION  Scans additionally vertical about 30 degree in direction to the currently set global position.\n
   *   .
   * - ScanVelocityMode:\n
   *   - FAST             Scans additionally vertical with 1125 degree per second. (Default)\n
   *   - SLOW             Scans additionally vertical with 375 degree per second.\n
   *   .
   * - ScanRangeModes:\n
   *   - NEAR_RANGE       Scans like described above. (Default)\n
   *   - FAR_RANGE        Reduces the scaning area (vertical) and the velocity (vertical) by factor 4.\n
   *   .
   * .
   * Detected objects are reported to the map.
   * @warning Objects closer than 60m are not detected.
   */
  class CSWActiveSonar : public CSWSonar
  {
    public:
      typedef std::shared_ptr<CSWActiveSonar> PtrType;
    
      static PtrType create(const std::string& strName, const Matrix44D& matBaseTObject);

      virtual ~CSWActiveSonar();

      //defined methods from CSWEquipment
      virtual QWidget* getInformationWidget(QWidget* pParent);
    
      //defined methods from IUpdateable
      virtual void update();

      //defined methods from CSWSonar
      virtual void draw();
      virtual Vector3D getAdditionalDirection(const Vector3D& vecCurrentDir, const Vector3D& vecUp) const;

      /**
       * 
       */
      const double& getAngleOfBeam() const;

      /**
       * 
       */
      void setAngleOfBeam(const double& fAngle);

      /**
       * Returns the distance from the sensor to the detected object. 
       * @return The distance from the sensor to the currently detected object. If no object has been 
       *         detected getMaximalDistance() is returned.
       */
      const double& getDistance() const;

      /**
       * Returns the maximal distance within objects can be detected.
       * @returns The maximal distance within objects can be detected.
       */
      const double& getMaximalDistance() const;
    
      /**
       * Returns true if an object has been detected otherwise false
       * @return True if an object has been detected otherwise false.
       */
      bool hasTargetDetected() const;

      /**
       * Returns true if a dynamic object has been detected.
       * @returns True if a dynamic object has been detected.
       */
      bool isTargetDynamic() const;

      /**
       * Returns true if a world border has been detected.
       * @returns True if a world border has been detected.
       */
      bool isTargetBorder() const;

      /**
       * Returns true if a submarine has been detected.
       * @returns True if a submarine has been detected.
       */
      bool isTargetSubmarine() const;

      /**
       * Returns true if a weapon has been detected.
       * @returns True if a weapon has been detected.
       */
      bool isTargetWeapon() const;

      /**
       * Returns the position of the detection in world coordinates.
       * @returns The position of the detection in world coordinates. If no object has been detected Vector3(0, 0, 0) is returned.
       */
      const Vector3D& getTargetPosition() const;

      friend CSWActiveSonar3DView;
    protected:
      struct ScanLine
      {
        ScanLine() : segment(), vecColor(0, 0, 0)
        {
        }

        ScanLine(Segment3D seg, Vector4D col) : segment(seg), vecColor(col)
        {
        }
      
        Segment3D segment;
        Vector4D vecColor;
      };
  
      CSWActiveSonar(const std::string& strName, const Matrix44D& matBaseTObject);

      void validate();


      static const double VERTICAL_SCANNING_ANGLE; //the angle that is scanned verticaly
      static const double VERTICAL_ANGULAR_VELOCITY; //in rad per second;
  
      static const double MAX_DISTANCE;

      double m_fAngleOfBeam;
      double m_fDistance;
      bool m_bTargetDetected;
      Vector3D m_vecTargetPosition;
      std::weak_ptr<CSWObject> m_pDetectedObject;
    
      boost::circular_buffer<ScanLine> m_LastScans;
    
      CSWActiveSonarVisualizer* m_pVisualizer;
    
      std::shared_ptr<CSWScanRay> m_pScanRay;
  };    

}