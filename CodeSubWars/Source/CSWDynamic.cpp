// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"

#include "CSWDynamic.h"
#include "CSWWorld.h"
#include "CSWSettings.h"

namespace CodeSubWars
{

  const double CSWDynamic::OBJECT_DENSITY = 0.5; //average density of whole object in g/cm^3
  const double CSWDynamic::MAXIMAL_VELOCITY = 100; //max possible velocity in the world in m/s
  const double CSWDynamic::MAXIMAL_ANGULAR_VELOCITY = getInRad(720); //max possible angular velocity in the world in rad/s


  CSWDynamic::PtrType CSWDynamic::create(const Matrix44D& matBaseTObject, const Mesh::PtrType pMassMesh)
  {
    return PtrType(new CSWDynamic(matBaseTObject, pMassMesh));
  }


  CSWDynamic::~CSWDynamic()
  {
  }


  void CSWDynamic::setMassMesh(const Mesh::PtrType pMassMesh)
  {
    m_pMassMesh = pMassMesh;
    validate();
    initialize(m_vecWorldTPositionCM,
               m_vecWorldTVelocityCM,
               m_matWorldTOrientationCM,
               m_vecWorldTAngularMomentumCM);
  }


  const Mesh::PtrType CSWDynamic::getMassMesh() const
  {
    return m_pMassMesh;
  }


  void CSWDynamic::setNewTransform(const Matrix44D& matWorldTObject)
  {
    initialize(matWorldTObject.getTranslation(),
               m_vecWorldTVelocityCM,
               matWorldTObject.getRotationAsMatrix33(),
               m_vecWorldTAngularMomentumCM);
  }


  const Matrix44D CSWDynamic::getLastTransform() const
  {
    return Matrix44D(m_matWorldTOrientationCM, m_vecWorldTPositionCM);  
  }


  void CSWDynamic::setNewVelocity(const Vector3D& vecWorldTVelocityCM,
                                  const Vector3D& vecWorldTAngularMomentumCM)
  {
    initialize(m_vecWorldTPositionCM,
               vecWorldTVelocityCM,
               m_matWorldTOrientationCM,
               vecWorldTAngularMomentumCM);
  }


  CSWDynamic::MassPointContainerRange CSWDynamic::getMassPointRange()
  {
    return std::make_pair(m_MassPoints.begin(), m_MassPoints.end());
  }


  const Vector3D& CSWDynamic::getCenterOfMass() const
  {
    return m_vecObjectTCenterOfMass;
  }


  const double& CSWDynamic::getTotalMass() const
  {
    return m_fMass;
  }


  Matrix44D CSWDynamic::updatePosition()
  {
    //calc forces applied to body
    m_vecWorldTForceTotal += m_vecWorldTForceCM;

    double fElapsedTime = getElapsedTime();

    if (m_vecWorldTForceTotal == Vector3D(0, 0, 0) &&
        m_vecWorldTTorqueTotal == Vector3D(0, 0, 0) &&
        m_vecWorldTVelocityCM == Vector3D(0, 0, 0) &&
        m_vecWorldTAngularVelocity == Vector3D(0, 0, 0))
    {
      resetForce();
      //object is sleeping. that means no forces are applied and it does not moving or rotating.
      return Matrix44D(m_matWorldTOrientationCM, m_vecWorldTPositionCM)*m_matCenterOfMassTObject;
    }

  //   double fWToHD = m_MassMeshBoundingBox.getWidth()*m_MassMeshBoundingBox.getWidth()/(m_MassMeshBoundingBox.getHeight()*m_MassMeshBoundingBox.getDepth());
  //   double fHToWD = m_MassMeshBoundingBox.getHeight()*m_MassMeshBoundingBox.getHeight()/(m_MassMeshBoundingBox.getWidth()*m_MassMeshBoundingBox.getDepth());
  //   double fDToWH = m_MassMeshBoundingBox.getDepth()*m_MassMeshBoundingBox.getDepth()/(m_MassMeshBoundingBox.getWidth()*m_MassMeshBoundingBox.getHeight());

    Vector3D vecObjectTDampingFactor(1, 1, 1);//fWToHD, fHToWD, fDToWH);
  //  Matrix33D matWorldTObjectRot = m_matWorldTOrientationCM*m_matCenterOfMassTObject.getRotationAsMatrix33();
    Vector3D vecWorldTDampingFactor = /*matWorldTObjectRot**/vecObjectTDampingFactor;
  //   vecWorldTDampingFactor.x = fabs(vecWorldTDampingFactor.x);
  //   vecWorldTDampingFactor.y = fabs(vecWorldTDampingFactor.y);
  //   vecWorldTDampingFactor.z = fabs(vecWorldTDampingFactor.z);




    //calc new position
    m_vecWorldTPositionCM += fElapsedTime*m_vecWorldTVelocityCM;



    //calc new velocity
    double fWorldTVelocityCM = m_vecWorldTVelocityCM.getLength();
    //determine the velocity damping. lies in range [0, v] 
    Vector3D vecWorldTVelocityDampingFactor = Vector3D(1e+8*1.0/vecWorldTDampingFactor.x, 
                                                       1e+8*1.0/vecWorldTDampingFactor.y,
                                                       1e+8*1.0/vecWorldTDampingFactor.z);
    vecWorldTVelocityDampingFactor *= fWorldTVelocityCM/m_fMass;
    Vector3D vecWorldTVelocityDamping(m_vecWorldTVelocityCM.x*(1 - 1/(1 + vecWorldTVelocityDampingFactor.x)),
                                      m_vecWorldTVelocityCM.y*(1 - 1/(1 + vecWorldTVelocityDampingFactor.y)),
                                      m_vecWorldTVelocityCM.z*(1 - 1/(1 + vecWorldTVelocityDampingFactor.z)));

    Vector3D vecWorldTLastVelocityCM(m_vecWorldTVelocityCM);
    m_vecWorldTVelocityCM += fElapsedTime*(m_vecWorldTForceTotal/m_fMass - vecWorldTVelocityDamping);
    //restrict velocity
    fWorldTVelocityCM = m_vecWorldTVelocityCM.getLength();
    if (fWorldTVelocityCM > MAXIMAL_VELOCITY)
      m_vecWorldTVelocityCM *= MAXIMAL_VELOCITY/fWorldTVelocityCM;



    //calc new acceleration
    m_vecWorldTAccelerationCM = (m_vecWorldTVelocityCM - vecWorldTLastVelocityCM)/fElapsedTime;



    //calc new orientation
    m_matWorldTOrientationCM += (tildeOperator(m_vecWorldTAngularVelocity)*m_matWorldTOrientationCM)*fElapsedTime;
    m_matWorldTOrientationCM.makeRotationMatrix(Matrix33D::COL0);



    //calc new angular momentum
    m_vecWorldTAngularMomentumCM += fElapsedTime*m_vecWorldTTorqueTotal;



    //calc new moment of inertia
    Matrix33D tmp(m_matWorldTOrientationCM);
    tmp.transpose();
    m_matMomentOfInertiaInvertWorldCM = m_matWorldTOrientationCM*m_matMomentOfInertiaInvertLocalCM*tmp;
    m_matMomentOfInertiaWorldCM = m_matWorldTOrientationCM*m_matMomentOfInertiaLocalCM*tmp;



    //calc new angular velocity
    Vector3D vecWorldTLastAngularVelocity(m_vecWorldTAngularVelocity);
    m_vecWorldTAngularVelocity = m_matMomentOfInertiaInvertWorldCM*m_vecWorldTAngularMomentumCM;
  
    //apply damping to angular velocity
    double fWorldTAngularVelocity = m_vecWorldTAngularVelocity.getLength();
    //determine the angular velocity damping. lies in range [0, w] 
    Vector3D vecWorldTAngularVelocityDampingFactor = Vector3D(1e+9*1.0/vecWorldTDampingFactor.x, 
                                                              1e+9*1.0/vecWorldTDampingFactor.y,
                                                              1e+9*1.0/vecWorldTDampingFactor.z);
    vecWorldTAngularVelocityDampingFactor *= fWorldTAngularVelocity/m_fMass;
    Vector3D vecWorldTAngularVelocityDamping(m_vecWorldTAngularVelocity.x*(1 - 1/(1 + vecWorldTAngularVelocityDampingFactor.x)),
                                             m_vecWorldTAngularVelocity.y*(1 - 1/(1 + vecWorldTAngularVelocityDampingFactor.y)),
                                             m_vecWorldTAngularVelocity.z*(1 - 1/(1 + vecWorldTAngularVelocityDampingFactor.z)));
    m_vecWorldTAngularVelocity += -fElapsedTime*vecWorldTAngularVelocityDamping;
  
    //restrict angular velocity
    fWorldTAngularVelocity = m_vecWorldTAngularVelocity.getLength();
    if (fWorldTAngularVelocity > MAXIMAL_ANGULAR_VELOCITY)
    {
      m_vecWorldTAngularVelocity *= MAXIMAL_ANGULAR_VELOCITY/fWorldTAngularVelocity;
    }
  
    //recalculate angular momentum from modified angular velocity
    m_vecWorldTAngularMomentumCM = m_matMomentOfInertiaWorldCM*m_vecWorldTAngularVelocity;


  
    //calc new angular acceleration
    m_vecWorldTAngularAcceleration = (m_vecWorldTAngularVelocity - vecWorldTLastAngularVelocity)/fElapsedTime;



    //reset forces
    resetForce();

    return Matrix44D(m_matWorldTOrientationCM, m_vecWorldTPositionCM)*m_matCenterOfMassTObject;
  }


  void CSWDynamic::resetForce()
  {
    m_vecWorldTForceCM = Vector3D(0, 0, 0);
    m_vecWorldTForceTotal = Vector3D(0, 0, 0);
    m_vecWorldTTorqueTotal = Vector3D(0, 0, 0);

    m_OldAppliedForces.clear();
    m_OldAppliedForces.swap(m_AppliedForces);
    assert(m_AppliedForces.empty());

    m_OldAppliedImpulses.clear();
    m_OldAppliedImpulses.swap(m_AppliedImpulses);
    assert(m_AppliedImpulses.empty());
  }


  void CSWDynamic::addForceToCM(const Vector3D& vecForce)
  {
    m_vecWorldTForceCM += vecForce;

    //for displaying only: store the position and the force
    m_AppliedForces.push_back(std::make_pair(m_vecWorldTPositionCM, vecForce));
  }


  void CSWDynamic::addForce(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTForce)
  {
    if (vecWorldTForce == Vector3D::ZERO)
      return;
  
    m_vecWorldTForceTotal += vecWorldTForce;
    if (vecWorldTPosition != m_vecWorldTPositionCM)
    {
      m_vecWorldTTorqueTotal += (vecWorldTPosition - m_vecWorldTPositionCM) % vecWorldTForce;
    }
  
    //for displaying only: store the position and the force
    m_AppliedForces.push_back(std::make_pair(vecWorldTPosition, vecWorldTForce));
  }


  void CSWDynamic::addImpuls(const Vector3D& vecWorldTPosition, const Vector3D& vecWorldTImpuls)
  {
    if (vecWorldTImpuls == Vector3D::ZERO)
      return;
  
    m_vecWorldTVelocityCM += vecWorldTImpuls/m_fMass; 
  
    double fWorldTVelocityCM = m_vecWorldTVelocityCM.getLength();
    if (fWorldTVelocityCM > MAXIMAL_VELOCITY)
      m_vecWorldTVelocityCM *= MAXIMAL_VELOCITY/fWorldTVelocityCM;
  
    if (vecWorldTPosition != m_vecWorldTPositionCM)
    {
      m_vecWorldTAngularMomentumCM += (vecWorldTPosition - m_vecWorldTPositionCM) % vecWorldTImpuls;
    }

    //for displaying only: store the position and the force
    m_AppliedImpulses.push_back(std::make_pair(vecWorldTPosition, vecWorldTImpuls));
  }


  const Vector3D& CSWDynamic::getVelocity() const
  {
    return m_vecWorldTVelocityCM;
  }


  const Vector3D& CSWDynamic::getAcceleration() const
  {
    return m_vecWorldTAccelerationCM;
  }


  const Vector3D& CSWDynamic::getAngularVelocity() const
  {
    return m_vecWorldTAngularVelocity;
  }


  Vector3D CSWDynamic::getAngularVelocity(const Vector3D& vecObjectTPosition) const
  {
    return m_vecWorldTAngularVelocity % (m_matWorldTOrientationCM*vecObjectTPosition);
  }


  const Vector3D& CSWDynamic::getAngularAcceleration() const
  {
    return m_vecWorldTAngularAcceleration;
  }


  Vector3D CSWDynamic::getAngularAcceleration(const Vector3D& vecObjectTPosition) const
  {
    return m_vecWorldTAngularAcceleration % (m_matWorldTOrientationCM*vecObjectTPosition);
  }


  const Vector3D& CSWDynamic::getAngularMomentum() const
  {
    return m_vecWorldTAngularMomentumCM;
  }


  const Matrix33D& CSWDynamic::getInvertedMomentOfIntertia() const
  {
    return m_matMomentOfInertiaInvertWorldCM;
  }


  void CSWDynamic::draw()
  {
    //draw force lines (only for debugging)
    if (!(CSWWorld::getInstance()->getSettings()->getDynamicProperties() & 
        (CSWSettings::DISPLAY_MASS_POINTS | CSWSettings::DISPLAY_DETAIL_FORCE | CSWSettings::DISPLAY_TOTAL_FORCE |
         CSWSettings::DISPLAY_DETAIL_IMPULS | CSWSettings::DISPLAY_TOTAL_IMPULS))) return;

    glPushAttrib(GL_ALL_ATTRIB_BITS);

      glEnable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glShadeModel(GL_FLAT);
      glDisable(GL_LIGHTING);
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glColor4d(0, 1, 1, 0.5);

      if (CSWWorld::getInstance()->getSettings()->getDynamicProperties() & CSWSettings::DISPLAY_MASS_POINTS)
      {
        glPointSize(4);
        m_pMassMesh->render();
      }

      glDepthMask(false);
    
      glPushMatrix();
        glLoadIdentity();

        Vector3D vecTotalForcePosition(0, 0, 0);
        Vector3D vecTotalForce(0, 0, 0);
        double fTotalForce = 0;

        glBegin(GL_LINES);
          std::vector<std::pair<Vector3D, Vector3D> >::const_iterator itForce = m_OldAppliedForces.begin();
          for (; itForce != m_OldAppliedForces.end(); ++itForce)
          {
            if (CSWWorld::getInstance()->getSettings()->getDynamicProperties() & CSWSettings::DISPLAY_DETAIL_FORCE)
            {
              glVertex3dv(itForce->first.pData);
              glVertex3dv((itForce->first + itForce->second/10000.0).pData);
            }
          
            double fForce = itForce->second.getLength();
            fTotalForce += fForce;
            vecTotalForcePosition += itForce->first*fForce;
            vecTotalForce += itForce->second;
          }
        glEnd();


        if (CSWWorld::getInstance()->getSettings()->getDynamicProperties() & CSWSettings::DISPLAY_TOTAL_FORCE)
        {
          if (fTotalForce > 0)
            vecTotalForcePosition /= fTotalForce;
          glLineWidth(4);
          glBegin(GL_LINES);
            glColor4d(0, 0.6, 1, 0.5);
            glVertex3dv(vecTotalForcePosition.pData);
            glVertex3dv((vecTotalForcePosition + vecTotalForce/10000.0).pData);
          glEnd();
        }


        Vector3D vecTotalImpulsPosition(0, 0, 0);
        Vector3D vecTotalImpuls(0, 0, 0);
        double fTotalImpuls = 0;
    
        glBegin(GL_LINES);
          std::vector<std::pair<Vector3D, Vector3D> >::const_iterator itImpuls = m_OldAppliedImpulses.begin();
          for (; itImpuls != m_OldAppliedImpulses.end(); ++itImpuls)
          {
            if (CSWWorld::getInstance()->getSettings()->getDynamicProperties() & CSWSettings::DISPLAY_DETAIL_IMPULS)
            {
              glVertex3dv(itImpuls->first.pData);
              glVertex3dv((itImpuls->first + itImpuls->second/10000.0).pData);
            }
          
            double fImpuls = itImpuls->second.getLength();
            fTotalImpuls += fImpuls;
            vecTotalImpulsPosition += itImpuls->first*fImpuls;
            vecTotalImpuls += itImpuls->second;
          }
        glEnd();


        if (CSWWorld::getInstance()->getSettings()->getDynamicProperties() & CSWSettings::DISPLAY_TOTAL_IMPULS)
        {
          if (fTotalImpuls > 0)
            vecTotalImpulsPosition /= fTotalImpuls;
          glLineWidth(4);
          glBegin(GL_LINES);
            glColor4d(0, 0.6, 1, 0.5);
            glVertex3dv(vecTotalImpulsPosition.pData);
            glVertex3dv((vecTotalImpulsPosition + vecTotalImpuls/10000.0).pData);
          glEnd();
        }

      glPopMatrix();

      glDepthMask(true);
    
    glPopAttrib();
  }


  CSWDynamic::CSWDynamic(const Matrix44D& matBaseTObject, const Mesh::PtrType pMassMesh)
  : m_pMassMesh(pMassMesh)
  {
    validate();
    initialize(matBaseTObject.getTranslation(),
               Vector3D(0, 0, 0),
               matBaseTObject.getRotationAsMatrix33(),
               Vector3D(0, 0, 0));
  }


  void CSWDynamic::validate()
  {
    assert(m_pMassMesh);

    m_MassMeshBoundingBox = m_pMassMesh->getAxisAlignedBoundingBox();

    //store mass points and regarding mass in seperate container
    m_MassPoints.clear();
    double fMassPerMassPoint = m_MassMeshBoundingBox.getSize().getVolume()*1000.0*OBJECT_DENSITY/m_pMassMesh->getNumVertices();
    Mesh::VectorConstIterator itVertex = m_pMassMesh->getVerticesBegin();
    for (; itVertex != m_pMassMesh->getVerticesEnd(); ++itVertex)
    {
      m_MassPoints.push_back(std::make_pair(*itVertex, fMassPerMassPoint));
    }


    //calc center of mass and mass
    m_vecObjectTCenterOfMass = Vector3D(0, 0, 0);
    m_fMass = 0;
    MassPointContainer::const_iterator itMassPoint = m_MassPoints.begin();
    for (; itMassPoint != m_MassPoints.end(); ++itMassPoint)
    {
      m_fMass += itMassPoint->second;
      m_vecObjectTCenterOfMass += itMassPoint->first*itMassPoint->second;
    }
    if (m_fMass != 0.0)
      m_vecObjectTCenterOfMass /= m_fMass;
    m_matCenterOfMassTObject = Matrix44D::inverse(Matrix44D(m_vecObjectTCenterOfMass));


    //calc moment of inertia invert local
    m_matMomentOfInertiaLocalCM = Matrix33D(true);
    Matrix33D tmp;
    itMassPoint = m_MassPoints.begin();
    for (; itMassPoint != m_MassPoints.end(); ++itMassPoint)
    {
      tmp = tildeOperator(itMassPoint->first);
      m_matMomentOfInertiaLocalCM -= tmp*tmp*itMassPoint->second;
    }
    MatrixD mat(m_matMomentOfInertiaLocalCM);
    bool bResult = mat.invert();
    if (!bResult)
      throw std::runtime_error("moment of inertia could not be calculated correctly!");
    m_matMomentOfInertiaInvertLocalCM = Matrix33D(mat);

    resetForce();
  }


  void CSWDynamic::initialize(const Vector3D& vecWorldTPositionCM, 
                              const Vector3D& vecWorldTVelocityCM, 
                              const Matrix33D& matWorldTOrientationCM, 
                              const Vector3D& vecWorldTAngularMomentumCM)
  {
    //set initial position, orientation, velocity, angular momentum
    m_fStartTime = ARSTD::Time::getTime();

    m_vecWorldTPositionCM = vecWorldTPositionCM;
    m_vecWorldTVelocityCM = vecWorldTVelocityCM;
    double fWorldTVelocityCM = m_vecWorldTVelocityCM.getLength();
    if (fWorldTVelocityCM > MAXIMAL_VELOCITY)
      m_vecWorldTVelocityCM *= MAXIMAL_VELOCITY/fWorldTVelocityCM;
     
    m_vecWorldTAccelerationCM = Vector3D(0, 0, 0);
    m_matWorldTOrientationCM = matWorldTOrientationCM;
    m_vecWorldTAngularMomentumCM = vecWorldTAngularMomentumCM;

    Matrix33D tmp(m_matWorldTOrientationCM);
    tmp.transpose();
    m_matMomentOfInertiaInvertWorldCM = m_matWorldTOrientationCM*m_matMomentOfInertiaInvertLocalCM*tmp;
    m_matMomentOfInertiaWorldCM = m_matWorldTOrientationCM*m_matMomentOfInertiaLocalCM*tmp;
    m_vecWorldTAngularVelocity = m_matMomentOfInertiaInvertWorldCM*m_vecWorldTAngularMomentumCM;

    double fWorldTAngularVelocity = m_vecWorldTAngularVelocity.getLength();
    if (fWorldTAngularVelocity > MAXIMAL_ANGULAR_VELOCITY) 
    {
      m_vecWorldTAngularVelocity *= MAXIMAL_ANGULAR_VELOCITY/fWorldTAngularVelocity;
      m_vecWorldTAngularMomentumCM = m_matMomentOfInertiaWorldCM*m_vecWorldTAngularVelocity;
    }

    m_vecWorldTAngularAcceleration = Vector3D(0, 0, 0);
  }


  double CSWDynamic::getElapsedTime()
  {
    double fCurrentTime = ARSTD::Time::getTime();
    double fElapsedTime = fCurrentTime - m_fStartTime;
    m_fStartTime = fCurrentTime;
    return fElapsedTime;
  }


  Matrix33D CSWDynamic::tildeOperator(const Vector3D& vec)
  {
    return Matrix33D(Vector3D(0,      vec.z,  -vec.y), 
                     Vector3D(-vec.z, 0,      vec.x ),
                     Vector3D(vec.y,  -vec.x, 0     ));
  }

}