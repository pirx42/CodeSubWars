// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWSubmarine.h"
#include "CSWIUpdateable.h"

#include "CSWFireCommand.h"
#include "CSWCollisionDetectedMessage.h"


class CSWIPySubmarine : public CSWSubmarine,
                        public CSWIUpdateable
{
  public:
    CSWIPySubmarine(std::string strName)
    : CSWSubmarine(strName, ARSTD::Matrix44D(), 300) {}
    
    virtual ~CSWIPySubmarine() {}

//dass muss unbedingt noch protected nach python gemappt werden
    CSWEngine::PtrType getMainEngine() { return CSWSubmarine::getMainEngine(); }

    CSWWeaponBattery::PtrType getWeaponBattery() { return CSWSubmarine::getWeaponBattery(); }

    ARSTD::CommandProcessor::PtrType getCommandProcessor() { return CSWSubmarine::getCommandProcessor(); }

};


struct CSWIPyWrapperSubmarine : public CSWIPySubmarine, boost::python::wrapper<CSWIPySubmarine>
{
  CSWIPyWrapperSubmarine(std::string name) : CSWIPySubmarine(name) {}
  
  virtual ~CSWIPyWrapperSubmarine() {}

  void update() { this->get_override("update")(); }
  
  bool processEvent(boost::shared_ptr<CSWEvent> pEvent)
  {
      if (boost::python::override processEvent = this->get_override("processEvent")) return processEvent(pEvent);
      return CSWIPySubmarine::processEvent(pEvent);
  }

  bool default_processEvent(boost::shared_ptr<CSWEvent> pEvent) { return this->CSWIPySubmarine::processEvent(pEvent); }
};


//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CSWFireCommand_create_stubs, CSWFireCommand::create, 1, 2)


BOOST_PYTHON_MODULE(CodeSubWars_Submarine)
{
  boost::python::class_<ARSTD::Matrix44D>("Matrix44", boost::python::init<>())
  ;
  


  boost::python::class_<CSWEngine, boost::noncopyable>("CSWEngine", boost::python::no_init)
    .def("setIntensity", &CSWEngine::setIntensity)
  ;



  boost::python::class_<CSWWeapon, boost::noncopyable>("CSWWeapon", boost::python::no_init)
  ;

  boost::python::class_<GreenTorpedo, boost::python::bases<CSWWeapon>, boost::noncopyable>("GreenTorpedo", boost::python::no_init)
    .def("create", &GreenTorpedo::create)
    .staticmethod("create")
  ;

  boost::python::class_<CSWWeaponBattery, boost::noncopyable>("CSWWeaponBattery", boost::python::no_init)
    .def("insert", &CSWWeaponBattery::insert)
  ;



  boost::python::class_<ARSTD::CommandProcessor, boost::noncopyable>("CommandProcessor", boost::python::no_init)
    .def("execute", &ARSTD::CommandProcessor::execute)
    .def("getNumWaitingCommands", &ARSTD::CommandProcessor::getNumWaitingCommands)
  ;

  boost::python::class_<ARSTD::Command, boost::noncopyable>("Command", boost::python::no_init)
  ;
  
  boost::python::class_<CSWFireCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWFireCommand", boost::python::no_init)
    .def("create", &CSWFireCommand::create)//, CSWFireCommand_create_stubs(args("a")))
    .staticmethod("create")
  ;
  


  boost::python::class_<CSWEvent, boost::noncopyable>("CSWEvent", boost::python::no_init)
    .def("getMessage", &CSWEvent::getMessage)
  ;

  boost::python::class_<CSWIMessage, boost::noncopyable>("CSWIMessage", boost::python::no_init)
  ;

  boost::python::class_<CSWCollisionDetectedMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWCollisionDetectedMessage", boost::python::no_init)
  ;


  boost::python::class_<CSWIPyWrapperSubmarine, boost::shared_ptr<CSWIPyWrapperSubmarine>, boost::noncopyable>("CSWIPySubmarine", 
          boost::python::init<std::string>())
    .def("getMainEngine", &CSWIPySubmarine::getMainEngine)
    .def("getWeaponBattery", &CSWIPySubmarine::getWeaponBattery)
    .def("getCommandProcessor", &CSWIPySubmarine::getCommandProcessor)
    .def("update", boost::python::pure_virtual(&CSWIPySubmarine::update))
    .def("processEvent", &CSWIPySubmarine::processEvent, &CSWIPyWrapperSubmarine::default_processEvent)
  ;


  boost::python::register_ptr_to_python<boost::shared_ptr<CSWEngine> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<GreenTorpedo> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<CSWWeaponBattery> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<CSWFireCommand> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<CSWEvent> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<CSWCollisionDetectedMessage> >();
  boost::python::register_ptr_to_python<boost::shared_ptr<ARSTD::CommandProcessor> >();
}
