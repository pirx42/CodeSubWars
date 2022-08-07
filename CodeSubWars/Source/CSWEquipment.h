// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWObject.h"
#include "CSWIUpdateable.h"

namespace CodeSubWars
{

  /**
   * This is the base class for all equipment stuff.
   */
  class CSWEquipment : public CSWObject,
                       public CSWIUpdateable
  {
    public:
      typedef std::shared_ptr<CSWEquipment> PtrType;

      virtual ~CSWEquipment();

      //defined methods from IUpdateable
      virtual void update();

      //Sets the direction relative to local initial set coordinate system. If a direction is given that
      //can not adjust, the maximal direction nearest to the given direction will be set. The
      //maximal excursion of the new direction relative to it initial direction is given by the second parameter.
      //@param vecDirection The new direction relative to its parent.
      virtual void setRelativeDirection(const Vector3D& vecDirection);
      virtual void setAbsoluteDirection(const Vector3D& vecWorldTDirection);

      void setMaxAngle(const double& fMaxAngle);

      void setVelocity(const double& fVelocity);
      void setAngularVelocity(const double& fAngularVelocity);
    
      /**
       * Returns whether the equipment is currently change its pose against its parent object.
       * @return True if direction/position changes are currently made, otherwise false.
       */
      bool isMoving() const;

      virtual QWidget* getInformationWidget(QWidget* pParent);

    protected:
      CSWEquipment(const std::string& strName, const Matrix44D& matBaseTObject,
                   const double& fMaxAngle, const double& fAngularVelocity);
    
      std::pair<bool, Matrix44D> calcNewTransform(const double& fElapsedTime);

      const Vector3D m_vecObjectTInitialDirection;
      const Vector3D m_vecObjectTInitialUp;
    
      Matrix44D m_matParentTObjectDestination;
      bool m_bMoving;
      double m_fLastTime;
      double m_fVelocity;                   //in m/s
      double m_fAngularVelocity;            //the angular velocity to change to the new direction in rad/s.
      double m_fMaxAngle;                   //the maximal excursion relative to the initial direction in rad.

  };

}