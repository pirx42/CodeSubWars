// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once
#pragma warning (disable: 4786)
#pragma warning (disable: 4267)

// c++ and c standard lib
#include <conio.h>
#include <algorithm>
#include <functional>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <strstream>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <memory>


// boost
#define BOOST_ALL_NO_LIB

#include <boost/utility.hpp>
#include <boost/functional.hpp>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

#include <boost/iostreams/device/file.hpp> 
#include <boost/iostreams/filtering_stream.hpp> 
#include <boost/iostreams/filter/zlib.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/qvm.hpp>


// qt
#include <QtCore/QtCore>
#include <QtGUI/QtGUI>
#include <QtOpenGL/QtOpenGL>
#undef foreach


// windows
#include "shellapi.h"


//SOLID
#include <solid.h>


// ARSTD
#include <arstd/Command/Command.h>
#include <arstd/Command/CommandProcessor.h>
#include <arstd/Command/MacroCommand.h>
#include <arstd/Command/BasicCommands.h>

#include <arstd/Math/Math.h>
#include <arstd/Geometry/Line3.h>
#include <arstd/Geometry/BoundingBox.h>
#include <arstd/Geometry/Plane.h>
#include <arstd/Geometry/Mesh.h>

#include <arstd/Misc/Time.h>
#include <arstd/Misc/Utilities.h>
#include <arstd/Misc/PIDController.h>
#include <arstd/Misc/Version.h>

#include <arstd/Structures/Element.h>
#include <arstd/Structures/Node.h>

#include <arstd/OpenGL/OpenGLTools.h>


using ARSTD::PI;
using ARSTD::PI_HALF;
using ARSTD::EPSILON;
using ARSTD::EPSILON_SQUARED;
using ARSTD::getInRad;

using ARSTD::uint8;
using ARSTD::uint64;
using ARSTD::Version;
using ARSTD::Size3D;
using ARSTD::Size3F;

using ARSTD::Vector3D;
using ARSTD::Vector3F;
using ARSTD::Vector4D;
using ARSTD::Vector4F;
using ARSTD::MatrixD;
using ARSTD::Matrix33D;
using ARSTD::Matrix44D;
using ARSTD::Matrix44F;
using ARSTD::QuaternionD;
using ARSTD::Segment3D;

using ARSTD::Ray3D;
using ARSTD::Line3D;
using ARSTD::PlaneD;
using ARSTD::BoundingBoxD;
using ARSTD::AxisAlignedBoundingBoxD;
using ARSTD::Mesh;

#undef NO_DATA
