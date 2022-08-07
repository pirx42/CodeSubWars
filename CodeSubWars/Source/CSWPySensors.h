// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include "CSWSensor.h"
#include "CSWGPS.h"
#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWActiveSonar.h"
#include "CSWPassiveSonar.h"

using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Sensors)
{

  boost::python::class_<CSWSensor, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWSensor", boost::python::no_init)
  ;


  boost::python::class_<CSWGPS, boost::python::bases<CSWSensor>, boost::noncopyable>("CSWGPS", boost::python::no_init)
    .def("getPosition", &CSWGPS::getPosition, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWGyroCompass, boost::python::bases<CSWSensor>, boost::noncopyable>("CSWGyroCompass", boost::python::no_init)
    .def("getDirection", &CSWGyroCompass::getDirection)
    .def("getInclination", &CSWGyroCompass::getInclination)
    .def("getAxialInclination", &CSWGyroCompass::getAxialInclination)
    .def("getUpDirection", &CSWGyroCompass::getUpDirection, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getForwardDirection", &CSWGyroCompass::getForwardDirection, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWMovingPropertiesSensor, boost::python::bases<CSWSensor>, boost::noncopyable>("CSWMovingPropertiesSensor", boost::python::no_init)
    .def("getVelocity", &CSWMovingPropertiesSensor::getVelocity, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getAngularVelocity", &CSWMovingPropertiesSensor::getAngularVelocity, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getFrontAngularVelocity", &CSWMovingPropertiesSensor::getFrontAngularVelocity, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getAcceleration", &CSWMovingPropertiesSensor::getAcceleration, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getAngularAcceleration", &CSWMovingPropertiesSensor::getAngularAcceleration, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getFrontAngularAcceleration", &CSWMovingPropertiesSensor::getFrontAngularAcceleration, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getForwardVelocity", &CSWMovingPropertiesSensor::getForwardVelocity)
    .def("getUpVelocity", &CSWMovingPropertiesSensor::getUpVelocity)
    .def("getLeftVelocity", &CSWMovingPropertiesSensor::getLeftVelocity)
    .def("getForwardAcceleration", &CSWMovingPropertiesSensor::getForwardAcceleration)
    .def("getUpAcceleration", &CSWMovingPropertiesSensor::getUpAcceleration)
    .def("getLeftAcceleration", &CSWMovingPropertiesSensor::getLeftAcceleration)
    .def("getUpAngularVelocity", &CSWMovingPropertiesSensor::getUpAngularVelocity)
    .def("getLeftAngularVelocity", &CSWMovingPropertiesSensor::getLeftAngularVelocity)
    .def("getAxialAngularVelocity", &CSWMovingPropertiesSensor::getAxialAngularVelocity)
    .def("getUpAngularAcceleration", &CSWMovingPropertiesSensor::getUpAngularAcceleration)
    .def("getLeftAngularAcceleration", &CSWMovingPropertiesSensor::getLeftAngularAcceleration)
    .def("getAxialAngularAcceleration", &CSWMovingPropertiesSensor::getAxialAngularAcceleration)
  ;

  boost::python::class_<CSWSonar, boost::python::bases<CSWSensor>, boost::noncopyable>("CSWSonar", boost::python::no_init)
    .def("setEnableAutomaticRotation", &CSWSonar::setEnableAutomaticRotation)
    .def("isAutomaticRotationEnabled", &CSWSonar::isAutomaticRotationEnabled)
    .def("setScanDirectionMode", &CSWSonar::setScanDirectionMode)
    .def("setScanVelocityMode", &CSWSonar::setScanVelocityMode)
    .def("setScanRangeMode", &CSWSonar::setScanRangeMode)
    .def("getScanDirectionMode", &CSWSonar::getScanDirectionMode)
    .def("getScanVelocityMode", &CSWSonar::getScanVelocityMode)
    .def("getScanRangeMode", &CSWSonar::getScanRangeMode)
    .def("setScanDirection", &CSWSonar::setScanDirection)
    .def("getScanDirection", &CSWSonar::getScanDirection, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("pointToGlobalPosition", &CSWSonar::pointToGlobalPosition)
    .def("getDirection", &CSWSonar::getDirection, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::enum_<CSWSonar::ScanDirectionMode>("ScanDirectionMode")
    .value("FULL", CSWSonar::FULL)
    .value("FRONT", CSWSonar::FRONT)
    .value("BACK", CSWSonar::BACK)
    .value("LOCAL_DIRECTION", CSWSonar::LOCAL_DIRECTION)
    .value("GLOBAL_POSITION", CSWSonar::GLOBAL_POSITION)
  ;

  boost::python::enum_<CSWSonar::ScanVelocityMode>("ScanVelocityMode")
    .value("FAST", CSWSonar::FAST)
    .value("SLOW", CSWSonar::SLOW)
  ;

  boost::python::enum_<CSWSonar::ScanRangeMode>("ScanRangeMode")
    .value("NEAR_RANGE", CSWSonar::NEAR_RANGE)
    .value("FAR_RANGE", CSWSonar::FAR_RANGE)
  ;

  boost::python::class_<CSWActiveSonar, boost::python::bases<CSWSonar>, boost::noncopyable>("CSWActiveSonar", boost::python::no_init)
//    .def("getAngleOfBeam", &CSWActiveSonar::getAngleOfBeam, boost::python::return_value_policy<boost::python::copy_const_reference>())
//    .def("setAngleOfBeam", &CSWActiveSonar::setAngleOfBeam)
    .def("getDistance", &CSWActiveSonar::getDistance, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getMaximalDistance", &CSWActiveSonar::getMaximalDistance, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("hasTargetDetected", &CSWActiveSonar::hasTargetDetected)
    .def("isTargetDynamic", &CSWActiveSonar::isTargetDynamic)
    .def("isTargetBorder", &CSWActiveSonar::isTargetBorder)
    .def("isTargetSubmarine", &CSWActiveSonar::isTargetSubmarine)
    .def("isTargetWeapon", &CSWActiveSonar::isTargetWeapon)
    .def("getTargetPosition", &CSWActiveSonar::getTargetPosition, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWPassiveSonar, boost::python::bases<CSWSonar>, boost::noncopyable>("CSWPassiveSonar", boost::python::no_init)
    .def("getAngleOfBeam", &CSWPassiveSonar::getAngleOfBeam, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("setAngleOfBeam", &CSWPassiveSonar::setAngleOfBeam)
    .def("getLevel", &CSWPassiveSonar::getLevel, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("adjustDirectionToMaximum", &CSWPassiveSonar::adjustDirectionToMaximum)
    .def("isAdjusting", &CSWPassiveSonar::isAdjusting)
    .def("hasAdjusted", &CSWPassiveSonar::hasAdjusted)
  ;

  boost::python::register_ptr_to_python<std::shared_ptr<CSWSensor> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWGPS> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWGyroCompass> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWMovingPropertiesSensor> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSonar> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWActiveSonar> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWPassiveSonar> >();
}
