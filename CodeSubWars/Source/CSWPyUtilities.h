// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include "CSWUtilities.h"

using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Utilities)
{
  boost::python::class_<CSWUtilities, boost::noncopyable>("CSWUtilities", boost::python::no_init)
//    .def("determineAngles", &CSWUtilities::determineAngles)
  ;

  boost::python::class_<ARSTD::Time, boost::noncopyable>("CSWTime", boost::python::no_init)
    .def("getTime", &ARSTD::Time::getTime)
    .staticmethod("getTime")
  ;

}
