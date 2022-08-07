// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWDynamic
  {
    public:
      typedef std::shared_ptr<CSWDynamic> PtrType;

      typedef std::pair<Vector3D, double> MassPoint; //Position and mass
      typedef std::vector<MassPoint> MassPointContainer; 
      typedef std::pair<MassPointContainer::iterator, MassPointContainer::iterator> MassPointContainerRange;

      static PtrType create(const Matrix44D& matBaseTObject, const Mesh::PtrType pMassMesh);
      ~CSWDynamic();

      void setMassMesh(const Mesh::PtrType pMassMesh);
      const Mesh::PtrType getMassMesh() const;

      void setNewTransform(const Matrix44D& matWorldTObject);
      const Matrix44D getLastTransform() const;

      void setNewVelocity(const Vector3D& vecWorldTVelocityCM,
                          const Vector3D& vecWorldTAngularMomentumCM);

      MassPointContainerRange getMassPointRange();
      const Vector3D& getCenterOfMass() const;
      const double& getTotalMass() const;

      Matrix44D updatePosition();

      //in world coordsystem
      void resetForce();

      void addForceToCM(const Vector3D& vecWorldTForce);
      void addForce(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTForce);

      void addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls);

      const Vector3D& getVelocity() const;
      const Vector3D& getAcceleration() const;
      const Vector3D& getAngularVelocity() const; // rad/s
      Vector3D getAngularVelocity(const Vector3D& vecObjectTPosition) const; // m/s
      const Vector3D& getAngularAcceleration() const; // rad/s^2
      Vector3D getAngularAcceleration(const Vector3D& vecObjectTPosition) const; // m/s^2
      const Vector3D& getAngularMomentum() const;
      const Matrix33D& getInvertedMomentOfIntertia() const;
  
      void draw();

    protected:
      CSWDynamic(const Matrix44D& matBaseTObject, const Mesh::PtrType pMassMesh);

      void validate();

      void initialize(const Vector3D& vecWorldTPositionCM, 
                      const Vector3D& vecWorldTVelocityCM, 
                      const Matrix33D& matWorldTOrientationCM, 
                      const Vector3D& vecWorldTAngularMomentumCM);

      double getElapsedTime();

      static Matrix33D tildeOperator(const Vector3D& vec);

      static const double OBJECT_DENSITY;
      static const double MAXIMAL_VELOCITY;
      static const double MAXIMAL_ANGULAR_VELOCITY;
    


      double m_fStartTime;                          //in seconds 
    
      Vector3D m_vecObjectTCenterOfMass;            //CM
      double m_fMass;                               //M
      Matrix33D m_matMomentOfInertiaInvertLocalCM;  //I^(-1) dach
      Matrix33D m_matMomentOfInertiaLocalCM;        //I dach

      Vector3D m_vecWorldTPositionCM;               //r
      Vector3D m_vecWorldTVelocityCM;               //v
      Vector3D m_vecWorldTAccelerationCM;           //a
      Matrix33D m_matWorldTOrientationCM;           //A
      Vector3D m_vecWorldTAngularMomentumCM;        //L

      Matrix33D m_matMomentOfInertiaInvertWorldCM;  //I^(-1)
      Matrix33D m_matMomentOfInertiaWorldCM;        //I
      Vector3D m_vecWorldTAngularVelocity;          //w (omega)
      Vector3D m_vecWorldTAngularAcceleration;      //a (alpha)

      Vector3D m_vecWorldTForceTotal;               //F
      Vector3D m_vecWorldTTorqueTotal;              //t

      Mesh::PtrType m_pMassMesh;                    //in local coord system
      MassPointContainer m_MassPoints;              //mass in local system, force world coord system
      Matrix44D m_matCenterOfMassTObject;
      Vector3D m_vecWorldTForceCM;

      AxisAlignedBoundingBoxD m_MassMeshBoundingBox;  //in local coord system

      std::vector<std::pair<Vector3D, Vector3D> > m_AppliedForces; //in world coord system: position and force
      std::vector<std::pair<Vector3D, Vector3D> > m_OldAppliedForces; //in world coord system: position and force

      std::vector<std::pair<Vector3D, Vector3D> > m_AppliedImpulses; //in world coord system: position and impuls
      std::vector<std::pair<Vector3D, Vector3D> > m_OldAppliedImpulses; //in world coord system: position and impuls
  };

}