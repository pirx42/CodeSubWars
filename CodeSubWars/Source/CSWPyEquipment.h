// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "CSWEquipment.h"
#include "CSWMap.h"
#include "CSWTransceiver.h"
#include "CSWWeaponBattery.h"
#include "CSWControlCenter.h"
#include "CSWGPS.h"
#include "CSWGyroCompass.h"
#include "CSWMovingPropertiesSensor.h"
#include "CSWActiveSonar.h"
#include "CSWPassiveSonar.h"

using namespace CodeSubWars;


unsigned long (CSWMap::*insertElement1)(const Vector3D&, unsigned long, unsigned long) = &CSWMap::insertElement; 
unsigned long (CSWMap::*insertElement2)(const CSWMapElement&, unsigned long) = &CSWMap::insertElement;

int (CSWMap::*removeElements1)(unsigned long, unsigned long) = &CSWMap::removeElements;
int (CSWMap::*removeElements2)(const Vector3D&, double, unsigned long, unsigned long) = &CSWMap::removeElements;


BOOST_PYTHON_MODULE(CodeSubWars_Equipment)
{

  boost::python::class_<CSWEquipment, boost::python::bases<CSWObject>, boost::noncopyable>("CSWEquipment", boost::python::no_init)
    .def("isMoving", &CSWEquipment::isMoving)
  ;


  boost::python::class_<CSWMapElement>("CSWMapElement", boost::python::init<>())
    .def("isValid", &CSWMapElement::isValid)
    .def_readonly("nID", &CSWMapElement::nID)
    .def_readwrite("vecWorldTPosition", &CSWMapElement::vecWorldTPosition)
    .def_readwrite("vecWorldTVelocity", &CSWMapElement::vecWorldTVelocity)
    .def_readwrite("nLevel", &CSWMapElement::nLevel)
    .def_readwrite("nUserData", &CSWMapElement::nUserData)
  ;

  boost::python::class_<std::vector<CSWMapElement> >("MapElementContainer")
    .def(boost::python::vector_indexing_suite<std::vector<CSWMapElement> >())
  ;

  boost::python::enum_<CSWMap::DangerLevel>("DangerLevel")
    .value("UNKNOWN", CSWMap::UNKNOWN)
    .value("NONE", CSWMap::NONE)
    .value("LOW", CSWMap::LOW)
    .value("MEDIUM", CSWMap::MEDIUM)
    .value("HIGH", CSWMap::HIGH)
  ;

  boost::python::class_<CSWMap, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWMap", boost::python::no_init)
    .def("insertElement", insertElement1)
    .def("insertElement", insertElement2)
    .def("isEmpty", &CSWMap::isEmpty)
    .def("clear", &CSWMap::clear)
    .def("findElementByID", &CSWMap::findElementByID)
    .def("findNearestElement", &CSWMap::findNearestElement)
    .def("findElements", &CSWMap::findElements)
    .def("removeElements", removeElements1)
    .def("removeElements", removeElements2)
  ;



  boost::python::class_<CSWTransceiver, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWTransceiver", boost::python::no_init)
    .def("getMaxNumQueuedMessages", &CSWTransceiver::getMaxNumQueuedMessages)
    .def("getNumQueuedMessages", &CSWTransceiver::getNumQueuedMessages)
    .def("send", &CSWTransceiver::send)
    .def("getFirstReceived", &CSWTransceiver::getFirstReceived)
  ;


  boost::python::class_<CSWWeaponBattery, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWWeaponBattery", boost::python::no_init)
    .def("getRechargingIDs", &CSWWeaponBattery::getRechargingIDs, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getFillLevel", &CSWWeaponBattery::getFillLevel)
    .def("tryRecharging", &CSWWeaponBattery::tryRecharging, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("release", &CSWWeaponBattery::release)//, boost::python::args("CSWWeapon"))
    .def("releaseNext", &CSWWeaponBattery::releaseNext)
    .def("getNext", &CSWWeaponBattery::getNext)
    .def("getSize", &CSWWeaponBattery::getSize) 
    .def("getNumInserted", &CSWWeaponBattery::getNumInserted)
    .def("isFull", &CSWWeaponBattery::isFull)
    .def("isEmpty", &CSWWeaponBattery::isEmpty)
    .def("getDirection", &CSWWeaponBattery::getDirection, boost::python::return_value_policy<boost::python::copy_const_reference>())
//    .def("setDirection", &CSWWeaponBattery::setDirection)
  ;


  boost::python::class_<CSWControlCenter, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWControlCenter", boost::python::no_init)
    .def("getGPS", &CSWControlCenter::getGPS)
    .def("getGyroCompass", &CSWControlCenter::getGyroCompass)
    .def("getMovingPropertiesSensor", &CSWControlCenter::getMovingPropertiesSensor)
    .def("getActiveSonar", &CSWControlCenter::getActiveSonar)
    .def("getPassiveSonar", &CSWControlCenter::getPassiveSonar)
    .def("getTransceiver", &CSWControlCenter::getTransceiver)
    .def("getMap", &CSWControlCenter::getMap)
  ;

  
  boost::python::register_ptr_to_python<std::shared_ptr<CSWEquipment> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWMap> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWTransceiver> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWWeaponBattery> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWControlCenter> >();
}
