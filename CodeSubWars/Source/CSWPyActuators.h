// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include "CSWActuator.h"
#include "CSWEngine.h"

using namespace CodeSubWars;


void (CSWEngine::*setDirection1)(const double&, const double&) = &CSWEngine::setDirection; 
void (CSWEngine::*setDirection2)(const Vector3D&) = &CSWEngine::setDirection; 


BOOST_PYTHON_MODULE(CodeSubWars_Actuators)
{
  boost::python::class_<CSWActuator, boost::python::bases<CSWEquipment>, boost::noncopyable>("CSWActuator", boost::python::no_init)
  ;

  boost::python::class_<CSWEngine, boost::python::bases<CSWActuator>, boost::noncopyable>("CSWEngine", boost::python::no_init)
    .def("setIntensity", &CSWEngine::setIntensity)
    .def("getIntensity", &CSWEngine::getIntensity, 
         boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("setDirection", setDirection1)
    .def("setDirection", setDirection2)
    .def("getDirection", &CSWEngine::getDirection,
         boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::register_ptr_to_python<std::shared_ptr<CSWActuator> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWEngine> >();
}
