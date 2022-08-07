// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWIMessage.h"

namespace CodeSubWars
{

  /**
   * This is the specialized class for explosion detected messages.
   */
  class CSWExplosionDetectedMessage : public CSWIMessage
  {
    public:
      typedef std::shared_ptr<CSWExplosionDetectedMessage> PtrType;
  
      static PtrType create(const Vector3D& vecWorldTPoint, const double& fDamage);

      virtual ~CSWExplosionDetectedMessage();

      virtual CSWIMessage::PtrType copy() const;
      virtual std::string getInformation() const;

      /**
       * Returns the position in world coordinates where the explosion was detected.
       */
      const Vector3D& getPoint() const;

      /**
       * Returns the damage applied by the detected explosion in range [0, 1].
       */
      const double& getDamage() const;

    protected:
      CSWExplosionDetectedMessage(const Vector3D& vecWorldTPoint, const double& fDamage);
      CSWExplosionDetectedMessage(const CSWExplosionDetectedMessage& other);

      Vector3D m_vecWorldTPoint;
      double m_fDamage;
  };

}