// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Math)
{
  /**
   * This class provides mathematical operations on three dimensional vectors.
   */
  boost::python::class_<Vector3D>("Vector3", boost::python::init<>())
    /**
     * Constructs a vector from 3 values.
     */
    .def(boost::python::init<double, double, double>())
    /**
     * The plus operator.
     */
    .def(boost::python::self + boost::python::self)
    /**
     * The minus operator.
     */
    .def(boost::python::self - boost::python::self)
    /**
     * The dot product operator.
     */
    .def(boost::python::self * boost::python::self)
    /**
     * The cross product operator.
     */
    .def(boost::python::self % boost::python::self)
    /**
     * The multiplication by scalar operator.
     */
    .def(boost::python::self * boost::python::other<double>())
    /**
     * The multiplication by scalar operator.
     */
    .def(boost::python::other<double>() * boost::python::self)
    /**
     * The str() operator.
     */
    .def(str(boost::python::self))
    /**
     * The == operator.
     */
    .def(boost::python::self == boost::python::self)
    /**
     * Returns the length.
     */
    .def("getLength", &Vector3D::getLength)
    /**
     * Returns the squared length.
     */
    .def("getSquaredLength", &Vector3D::getSquaredLength)
    /**
     * Normalizes the vector.
     */
    .def("normalize", &Vector3D::normalize, boost::python::return_value_policy<boost::python::copy_const_reference>())
    /**
     * Returns the normalized version of the vector without changing the vector.
     */
    .def("getNormalized", &Vector3D::getNormalized)
    /**
     * Returns the angle to the given vector in degree.
     */
    .def("getAngleBetween", &Vector3D::getAngleBetweenInDegree)
    /**
     * Returns a perpendicular vector.
     */
    .def("getPerpendicular", &Vector3D::getPerpendicular)

    /**
     * Enables access to members.
     */
    .def_readwrite("x", &Vector3D::x)
    .def_readwrite("y", &Vector3D::y)
    .def_readwrite("z", &Vector3D::z)

    .def_readwrite("r", &Vector3D::x)
    .def_readwrite("g", &Vector3D::y)
    .def_readwrite("b", &Vector3D::z)
  ;


  /**
   * This class provides mathematical operations on four dimensional vectors.
   */
  boost::python::class_<Vector4D>("Vector4", boost::python::init<>())
    /**
     * Constructs a vector from 4 values.
     */
    .def(boost::python::init<double, double, double, double>())
//    /**
//     * The plus operator.
//     */
//    .def(boost::python::self + boost::python::self)
//    /**
//     * The minus operator.
//     */
//    .def(boost::python::self - boost::python::self)
//    /**
//     * The dot product operator.
//     */
//    .def(boost::python::self * boost::python::self)
//    /**
//     * The cross product operator.
//     */
//    .def(boost::python::self % boost::python::self)
//    /**
//     * The multiplication by scalar operator.
//     */
//    .def(boost::python::self * boost::python::other<double>())
//    /**
//     * The multiplication by scalar operator.
//     */
//    .def(boost::python::other<double>() * boost::python::self)
    /**
     * The str() operator.
     */
    .def(str(boost::python::self))
    /**
     * Returns the length.
     */
    .def("getLength", &Vector4D::getLength)
    /**
     * Returns the squared length.
     */
    .def("getSquaredLength", &Vector4D::getSquaredLength)
    /**
     * Normalizes the vector.
     */
    .def("normalize", &Vector4D::normalize, boost::python::return_value_policy<boost::python::copy_const_reference>())
    /**
     * Returns the normalized version of the vector without changing the vector.
     */
    .def("getNormalized", &Vector4D::getNormalized)

    /**
     * Enables access to members.
     */
    .def_readwrite("x", &Vector4D::x)
    .def_readwrite("y", &Vector4D::y)
    .def_readwrite("z", &Vector4D::z)
    .def_readwrite("w", &Vector4D::w)

    .def_readwrite("r", &Vector4D::x)
    .def_readwrite("g", &Vector4D::y)
    .def_readwrite("b", &Vector4D::z)
    .def_readwrite("a", &Vector4D::w)
  ;


  
  /**
   * This class provides mathematical operations on homogenous 4x4 matrices.
   */
  boost::python::class_<Matrix44D>("Matrix44", boost::python::init<>())
    /**
     * Constructs a matrix from 4 vectors (x, y, z axis and translation).
     * The given vectors must build a homogeneous matrix.
     */
    .def(boost::python::init<Vector3D, Vector3D, Vector3D, Vector3D>())
    /**
     * The matrix*vector operator.
     */
    .def(boost::python::self * boost::python::other<Vector3D>())
    /**
     * The matrix*matrix operator.
     */
    .def(boost::python::self * boost::python::self)
    /**
     * The str() operator.
     */
    .def(str(boost::python::self))
    /**
     * Inverts the matrix.
     */
    .def("invert", &Matrix44D::invert)
  ;
}
