// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWWeapon.h"
#include "GreenMine.h"
#include "YellowMine.h"
#include "GreenTorpedo.h"
#include "RedTorpedo.h"

#include "CSWWeaponSupply.h"

using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Weapons)
{
  
  boost::python::enum_<CSWWeaponSupply::WeaponResource>("WeaponResource")
    .value("GREEN_TORPEDO_ID", CSWWeaponSupply::GREEN_TORPEDO_ID)
    .value("RED_TORPEDO_ID", CSWWeaponSupply::RED_TORPEDO_ID)
    .value("BLUE_TORPEDO_ID", CSWWeaponSupply::BLUE_TORPEDO_ID)
    .value("GREEN_MINE_ID", CSWWeaponSupply::GREEN_MINE_ID)
    .value("YELLOW_MINE_ID", CSWWeaponSupply::YELLOW_MINE_ID)
  ;
  


  boost::python::class_<CSWWeapon, boost::python::bases<CSWObject>, boost::noncopyable>("CSWWeapon", boost::python::no_init)
    .def("arm", &CSWWeapon::arm)
    .def("fire", &CSWWeapon::fire)
  ;

  boost::python::class_<GreenMine, boost::python::bases<CSWWeapon>, boost::noncopyable>("GreenMine", boost::python::no_init)
  ;

  boost::python::class_<GreenTorpedo, boost::python::bases<CSWWeapon>, boost::noncopyable>("GreenTorpedo", boost::python::no_init)
  ;

  boost::python::class_<RedTorpedo, boost::python::bases<CSWWeapon>, boost::noncopyable>("RedTorpedo", boost::python::no_init)
  ;

  boost::python::register_ptr_to_python<std::shared_ptr<CSWWeapon> >();
  boost::python::register_ptr_to_python<std::shared_ptr<GreenMine> >();
  boost::python::register_ptr_to_python<std::shared_ptr<GreenTorpedo> >();
  boost::python::register_ptr_to_python<std::shared_ptr<RedTorpedo> >();
}
