// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "CSWObject.h"
#include "Constants.h"

using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Common)
{

  boost::python::class_<std::vector<Vector3D> >("Vector3DContainer")
    .def(boost::python::vector_indexing_suite<std::vector<Vector3D> >())
  ;


  boost::python::class_<CSWObject, boost::noncopyable>("CSWObject", boost::python::no_init)
    .def("getName", &CSWObject::getName, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;


  boost::python::class_<Version>("Version", boost::python::init<std::string>())
    .def(str(boost::python::self))
    .def(boost::python::self < boost::python::self)
    .def(boost::python::self > boost::python::self)
    .def(boost::python::self == boost::python::self)
    .def_readonly("nMainVersion", &Version::nMainVersion)
    .def_readonly("nSubVersion", &Version::nSubVersion)
    .def_readonly("nSubSubVersion", &Version::nSubSubVersion)
  ;  

  boost::python::def("getVersion", &Constants::getVersion, boost::python::return_value_policy<boost::python::copy_const_reference>());


  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::Element> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWObject> >();
}
