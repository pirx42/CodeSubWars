// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once

#include "CSWObject.h"

#include "CSWDynamic.h"

#include "CSWIDynamic.h"
#include "CSWISolid.h"
#include "CSWICollideable.h"


namespace CodeSubWars
{

  class CSWSolid;
  class CSWCollideable;

  /** 
   * This is a base class for some common objects. It simply implements some often used combinations of property interfaces.
   */
  //@todo -Give it a better name.
  class CSWDynSolCol : public CSWObject,
                       public CSWIDynamic,
                       public CSWISolid,
                       public CSWICollideable
  {
    public:
      typedef std::shared_ptr<CSWDynSolCol> PtrType;

      virtual ~CSWDynSolCol();

      //defined methods for dynamic
      virtual void updatePosition();
      virtual void resetForce();
      virtual void addForceToCM(const Vector3D& vecWorldTForce);
      virtual void addForce(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTForce);
      virtual void addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls);
      virtual MassPointContainerRange getMassPointRange();
      virtual const Vector3D& getVelocity() const;
      virtual const Vector3D& getAcceleration() const;
      virtual const Vector3D& getAngularVelocity() const;
      virtual Vector3D getAngularVelocity(const Vector3D& vecObjectTPosition) const;
      virtual const Vector3D& getAngularAcceleration() const;
      virtual Vector3D getAngularAcceleration(const Vector3D& vecObjectTPosition) const;
      virtual const Vector3D& getAngularMomentum() const;
      virtual const Matrix33D& getInvertedMomentOfIntertia() const;
      virtual const double& getTotalMass() const;
      virtual void setNewTransform(const Matrix44D& matWorldTObject);
      virtual void setNewVelocity(const Vector3D& vecWorldTVelocityCM,
                                  const Vector3D& vecWorldTAngularMomentumCM);
      virtual const Vector3D& getCenterOfMass() const;
      void initializeDynamic(const Matrix44D& matWorldTPositionCM, 
                             const Vector3D& vecWorldTVelocityCM,
                             const Vector3D& vecWorldTAngularMomentumCM);

      //defined methods for solid
      virtual void draw();
      virtual const Mesh::PtrType getSurface() const;
      virtual const BoundingBoxD& getSurfaceBoundingBox() const;

      virtual void setColor(const Vector4D& vecColor);
      virtual const Vector4D& getColor();
    
      //defined methods for collideable
      virtual const Mesh::PtrType getCollisionMesh() const;
      virtual const BoundingBoxD& getCollisionBoundingBox() const;
      virtual void initializeEnvironment(bool bSimplified);
      virtual void prepare();
      virtual void setupTransform();
      virtual const Matrix44D& getObjectTCollisionShape();
      virtual void setHit(bool bHit);
      virtual bool deleteEnvironment();

    protected:
      CSWDynSolCol(const std::string& strName, const Matrix44D& matBaseTObject, 
                   Mesh::PtrType pSurface);
    
      //properties
      std::shared_ptr<CSWDynamic> m_pDynamic;
      std::shared_ptr<CSWSolid> m_pSolid;
      std::shared_ptr<CSWCollideable> m_pCollideable;

  Vector3D m_vecCollisionPoint;
  Vector3D m_vecCollisionNormal;
  };

}