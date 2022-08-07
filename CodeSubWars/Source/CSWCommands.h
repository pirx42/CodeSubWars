// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

namespace CodeSubWars
{

  class CSWSubmarine;
  class CSWWeaponBattery;

  /**
   * This class provides useful predefined macro commands.
   */
  class CSWCommands
  {
    public:

      /**
       * Creates a new command that sets the intensity of the main engine.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fIntensity The new intensity.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setMainEngineIntensity(CSWSubmarine* pSubmarine, double fIntensity);

      /**
       * Creates a new command that sets the direction of the main engine.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fHAngle The new horizontal angle. Positiv means rightward (in degree).
       * @param fVAngle The new vertical angle. Positiv means upward (in degree).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setMainEngineDirection(CSWSubmarine* pSubmarine, const double& fHAngle, const double& fVAngle);

      /**
       * Creates a new command that sets the direction of the main engine.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param vecDirection The new direction relative to its parent.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setMainEngineDirection(CSWSubmarine* pSubmarine, const Vector3D& vecDirection);

      /**
       * Creates a new command that disables the main engine. (sets the intensity to zero)
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType disableMainEngine(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that sets the intensity of the buoyancy tank.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fIntensity The new intensity.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setBuoyancyTank(CSWSubmarine* pSubmarine, double fIntensity);

      /**
       * Creates a new command that disables the buoyancy tank. (sets the intensity to zero)
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType disableBuoyancyTank(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that sets the intensity of the axial inclination jet oar.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fIntensity The new intensity.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setAxialInclinationJetOar(CSWSubmarine* pSubmarine, double fIntensity);

      /**
       * Creates a new command that disables the axial inclination jet oar. (sets the intensity to zero)
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType disableAxialInclinationJetOar(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that disables the jet oars which are responsible for rotating a submarine. 
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType disableAllJetOars(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that disables all engines. 
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType disableAllEngines(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that controlls the rotation velocity by controlling the intensities of:
       * 1. the axial inclination jet oar, 2. the inclination jet oar and finally the axial inclination jet oar 
       * so that the rotation velocity of the submarine is nearly zero end velocity about all axis.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       * @post The angular velocity is near zero.
       */
      static ARSTD::Command::PtrType stopRotation(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that controlls the absolute orientation by controlling the intensities of:
       * 1. the axial inclination jet oar, 2. the inclination jet oar and finally the axial inclination jet oar 
       * so that the submarine is directed to the given absolut position with nearly zero end 
       * velocity about all axis.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param vecPosition The position in world coorinates to which the subarmine should direct to.
       * @param fRotationTolerance The destination rotation tolerance in degree. Should be larger or equal than 0.1.
       * @param fEndVelocityTolerance The angular end velocity tolerance in degree per second.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setOrientationAbsoluteTowardPosition(CSWSubmarine* pSubmarine, const Vector3D& vecPosition,
                                                                          double fRotationTolerance = 0.1, 
                                                                          double fEndVelocityTolerance = 0.3);

      /**
       * Creates a new command that controlls the absolute orientation by controlling the intensities of:
       * 1. the axial inclination jet oar, 2. the inclination jet oar and finally the axial inclination jet oar 
       * so that the submarine is directed to the given absolut direction with nearly zero end 
       * velocity about all axis.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param vecDirection The direction in world coorinates which the subarmine should direct.
       * @param fRotationTolerance The destination rotation tolerance in degree. Should be larger or equal than 0.1.
       * @param fEndVelocityTolerance The angular end velocity tolerance in degree per second.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType setOrientationAbsoluteTowardDirection(CSWSubmarine* pSubmarine, const Vector3D& vecDirection,
                                                                           double fRotationTolerance = 0.1, 
                                                                           double fEndVelocityTolerance = 0.3);

      /**
       * Creates a new command that controlls the translation by controlling the intensities of:
       * main engine and the buoyancy tank.
       * so that the submarine has nearly no end translation velocity.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @post The translation velocity is near zero.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType stopMoving(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that make the submarine moves forward by a given distance.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fDistance The distance that the submarine should move. (positive means forward, negative means backward)
       * @param fEndMoveVelocity The end translation velocity in meter per second.
       * @param fEndMoveVelocityTolerance The end translation velocity tolerance in degree per second.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType moveForward(CSWSubmarine* pSubmarine, double fDistance,
                                                 double fEndMoveVelocity = 0, double fEndMoveVelocityTolerance = 0.1);

      /**
       * Creates a new command that make the submarine moves upward by a given distance.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @param fDistance The distance that the submarine should move. (positive means upward, negative means downward)
       * @param fEndMoveVelocity The end translation velocity in meter per second.
       * @param fEndMoveVelocityTolerance The end translation velocity tolerance in degree per second.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType moveUpward(CSWSubmarine* pSubmarine, double fDistance,
                                                double fEndMoveVelocity = 0, double fEndMoveVelocityTolerance = 0.1);

      /**
       * Creates a new command that make the submarine does a full stop. The translation velocity and the rotation velocity
       * becomes nearly zero.
       * @param pSubmarine The submarine where the command should be executed (should always be self).
       * @return Returns the new constructed command.
       * @post The translation and angular velocity is near zero.
       */
      static ARSTD::Command::PtrType stopAll(CSWSubmarine* pSubmarine);

      /**
       * Creates a new command that make the submarine launches weapon from the specified WeaponBattery.
       * @param pWeaponBattery The weapon battery from which the weapons should be launched.
       * @param nNumTorpedos The number of torpedoes that should be launched.
       * @param fTimeBetweenLaunches The time between launches that should be wait in seconds.
       * @return Returns the new constructed command.
       */
      static ARSTD::Command::PtrType fireSalvo(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery, 
                                               int nNumTorpedos, double fTimeBetweenLaunches = 0);


    protected:
      CSWCommands() {}
      ~CSWCommands() {}
  };

}