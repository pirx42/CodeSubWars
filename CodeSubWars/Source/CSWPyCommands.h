// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include "CSWLog.h"

#include "CSWCommands.h"
#include "CSWWaitCommand.h"
#include "CSWVariousCommands.h"


namespace CodeSubWars
{

  template <class Type>
  boost::python::object get_owner(Type* me)
  {
    // Use secret interface to get the Python object
    // that owns *this.
    return boost::python::object(boost::python::handle<>(boost::python::borrowed(boost::python::detail::wrapper_base_::get_owner(*me))));
  }



  /**
   * The base class of Commands that are defined in python.
   */
  class CSWPyCommand : public ARSTD::Command, public std::enable_shared_from_this<CSWPyCommand>
  {
    public:
      typedef std::shared_ptr<CSWPyCommand> PtrType;
    
      CSWPyCommand() {}
      CSWPyCommand(const CSWPyCommand& other) : ARSTD::Command(other) {}
      virtual ~CSWPyCommand() {}

  //    /**
  //     * If you want store a reference to this object you _must_ use this method. Otherwise it could be happen that
  //     * this object is never destructed.
  //     * @return Returns a shared pointer to this.
  //     */
  //    std::shared_ptr<CSWPyCommand> getSharedThis() { return shared_from_this(); }

      /**
       * This is called directly before execution (before the first step() is called).
       */
      virtual void initialize() = 0;

      /**
       * This is called periodically about every 10ms while executing. If the command is done
       * finished() must be called. If finished() is never called the command never stops executing.
       */
      virtual void step() = 0;

      /**
       * This is called directly after finishing of execution or on breaking.
       */
      virtual void cleanup() = 0;

      /**
       * Returns some interesting stuff about the current command state.
       * @return The detailed state depend command description.
       */
      virtual std::string getDetails() const { return ARSTD::Command::getDetails(); }

      /**
       * Sets the progress of command execution.
       * @param fProgress The new progress of the execution in range [0, 1].
       */
      void setProgress(double fProgress) { ARSTD::Command::setProgress(fProgress); }

      /**
       * This indicates the execution environment that the command is done.
       * @warning This method is only valid to call inside step().
       */
      void finished() { ARSTD::Command::finished(); }

  };


  struct CSWPyCommandWrapper : public CSWPyCommand, boost::python::wrapper<CSWPyCommand>
  {
    CSWPyCommandWrapper() {}
    CSWPyCommandWrapper(const CSWPyCommand& other) : CSWPyCommand(other) {}

    virtual ~CSWPyCommandWrapper() 
    {
      PyErr_Print();
    }

    ARSTD::Command::PtrType copy() const
    {
      std::shared_ptr<CSWPyCommandWrapper> result;
      try
      {
        boost::python::object self = get_owner(this);

        // Find its most-derived Python class
        boost::python::object my_class = self.attr("__class__");

        // call the copy ctor through Python.
        boost::python::object py_result = my_class(self);
        result = boost::python::extract<std::shared_ptr<CSWPyCommandWrapper> >(py_result);
      }
      catch (...)
      {
        CSWLog::getInstance()->log(getName() + "::copy() throws an exception");
        throw;
      }
      return result;
    }

    void initialize() 
    { 
      try
      {
        this->get_override("initialize")(); 
      }
      catch (...)
      {
        CSWLog::getInstance()->log(getName() + "::initialize() throws an exception");
        throw;
      }
    }
  
    void step()
    { 
      try
      {
         this->get_override("step")();
      }
      catch (...)
      {
        CSWLog::getInstance()->log(getName() + "::step() throws an exception");
        throw;
      }
    }

    void cleanup()
    { 
      try
      {
         this->get_override("cleanup")();
      }
      catch (...)
      {
        CSWLog::getInstance()->log(getName() + "::cleanup() throws an exception");
        throw;
      }
    }

    std::string getName() const 
    { 
      try
      {
        return this->get_override("getName")(); 
      }
      catch (...)
      {
        CSWLog::getInstance()->log("command::getName() throws an exception (using default name)");
        PyErr_Print();
        return "command";
      }
    }
  
    unsigned __int64 getRessourceID() const
    {
      if (boost::python::override getRessourceID = this->get_override("getRessourceID"))
      {
        try
        {
          return getRessourceID();
        }
        catch (...)
        {
          CSWLog::getInstance()->log(getName() + "::getRessourceID() throws an exception (use default ressource id)");
          PyErr_Print();
        }
      }
      return CSWPyCommand::getRessourceID();
    }
    unsigned __int64 default_getRessourceID() const { return this->CSWPyCommand::getRessourceID(); }

    std::string getDetails() const
    {
      if (boost::python::override getDetails = this->get_override("getDetails"))
      {
        try
        {
          return getDetails();
        }
        catch (...)
        {
          CSWLog::getInstance()->log(getName() + "::getDetails() throws an exception (use default detail description)");
          PyErr_Print();
        }
      }
      return CSWPyCommand::getDetails();
    }
    std::string default_getDetails() const { return this->CSWPyCommand::getDetails(); }
  };


  CSWSetEngineDirectionCommand::PtrType (*CSWSetEngineDirectionCommand_create1)(CSWEngine::PtrType&, const double&, const double&) 
    = &CSWSetEngineDirectionCommand::create; 
  
  CSWSetEngineDirectionCommand::PtrType (*CSWSetEngineDirectionCommand_create2)(CSWEngine::PtrType&, const Vector3D&) 
    = &CSWSetEngineDirectionCommand::create; 


  ARSTD::Command::PtrType (*CSWCommands_setMainEngineDirection1)(CSWSubmarine*, const double&, const double&) 
    = &CSWCommands::setMainEngineDirection; 
  
  ARSTD::Command::PtrType (*CSWCommands_setMainEngineDirection2)(CSWSubmarine*, const Vector3D&) 
    = &CSWCommands::setMainEngineDirection; 


  CSWFireCommand::PtrType CSWFireCommand_create1(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery)
  {
    return CSWFireCommand::create(pWeaponBattery);
  }

  CSWFireCommand::PtrType CSWFireCommand_create2(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery, 
                                                 double fArmDelay)
  {
    return CSWFireCommand::create(pWeaponBattery, fArmDelay);
  }

  CSWFireCommand::PtrType CSWFireCommand_create3(std::shared_ptr<CSWWeaponBattery>& pWeaponBattery, 
                                                 double fArmDelay,
                                                 std::shared_ptr<CSWWeapon>& pWeapon)
  {
    return CSWFireCommand::create(pWeaponBattery, fArmDelay, pWeapon);
  }

}


using namespace CodeSubWars;


BOOST_PYTHON_MODULE(CodeSubWars_Commands)
{
  boost::python::class_<ARSTD::CommandProcessor, boost::noncopyable>("CommandProcessor", boost::python::no_init)
    .def("execute", &ARSTD::CommandProcessor::execute)
    .def("cleanup", &ARSTD::CommandProcessor::cleanup)
    .def("cleanupHistory", &ARSTD::CommandProcessor::cleanupHistory)
    .def("skipCurrent", &ARSTD::CommandProcessor::skipCurrent)
    .def("push", &ARSTD::CommandProcessor::push)
    .def("pop", &ARSTD::CommandProcessor::pop)
    .def("repeat", &ARSTD::CommandProcessor::repeat)
    .def("isBusy", &ARSTD::CommandProcessor::isBusy)
    .def("getCurrentCommandProgress", &ARSTD::CommandProcessor::getCurrentCommandProgress)
    .def("getCurrentCommandName", &ARSTD::CommandProcessor::getCurrentCommandName)
    .def("getNumWaitingCommands", &ARSTD::CommandProcessor::getNumWaitingCommands)
  ;


  boost::python::class_<ARSTD::Command, std::shared_ptr<ARSTD::Command>, boost::noncopyable>("Command", boost::python::no_init)
    .def("copy", &ARSTD::Command::copy)
    .def("getName", &ARSTD::Command::getName)
    .def("wasExecuted", &ARSTD::Command::wasExecuted)
    .def("getProgress", &ARSTD::Command::getProgress)
  ;


  boost::python::class_<CSWPyCommandWrapper, std::shared_ptr<CSWPyCommandWrapper>, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWPyCommand", 
          boost::python::init<>())
    .def(boost::python::init<const CSWPyCommand&>())
//    .def("getSharedThis", &CSWPyCommand::getSharedThis)
    .def("initialize", boost::python::pure_virtual(&CSWPyCommand::initialize))
    .def("step", boost::python::pure_virtual(&CSWPyCommand::step))
    .def("cleanup", boost::python::pure_virtual(&CSWPyCommand::cleanup))
    .def("getName", boost::python::pure_virtual(&CSWPyCommand::getName))
    .def("setProgress", &CSWPyCommand::setProgress)
    .def("finished", &CSWPyCommand::finished)
    .def("getRessourceID", &CSWPyCommand::getRessourceID, &CSWPyCommandWrapper::default_getRessourceID)
    .def("getDetails", &CSWPyCommand::getDetails, &CSWPyCommandWrapper::default_getDetails)
  ;


  boost::python::class_<ARSTD::MacroCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("MacroCommand", boost::python::no_init)
    .def("create", &ARSTD::MacroCommand::create)
    .staticmethod("create")
    .def("attach", &ARSTD::MacroCommand::attach)
    .def("detach", &ARSTD::MacroCommand::detach)
  ;

  boost::python::class_<ARSTD::PushCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("PushCommand", boost::python::no_init)
    .def("create", &ARSTD::PushCommand::create)
    .staticmethod("create")
  ;

  boost::python::class_<ARSTD::PopCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("PopCommand", boost::python::no_init)
    .def("create", &ARSTD::PopCommand::create)
    .staticmethod("create")
  ;

  boost::python::class_<ARSTD::RepeatCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("RepeatCommand", boost::python::no_init)
    .def("create", &ARSTD::RepeatCommand::create)
    .staticmethod("create")
  ;

  boost::python::class_<ARSTD::CleanupHistoryCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CleanupHistoryCommand", boost::python::no_init)
    .def("create", &ARSTD::CleanupHistoryCommand::create)
    .staticmethod("create")
  ;

  
  boost::python::class_<CSWSendEventCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWSendEventCommand", boost::python::no_init)
    .def("create", &CSWSendEventCommand::create)
    .staticmethod("create")
  ;
  
  boost::python::class_<CSWWaitCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWWaitCommand", boost::python::no_init)
    .def("create", &CSWWaitCommand::create)
    .staticmethod("create")
  ;

  boost::python::class_<CSWSetEngineIntensityCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWSetEngineIntensityCommand", boost::python::no_init)
    .def("create", &CSWSetEngineIntensityCommand::create)
    .staticmethod("create")
  ;

  boost::python::class_<CSWSetEngineDirectionCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWSetEngineDirectionCommand", boost::python::no_init)
    .def("create", CSWSetEngineDirectionCommand_create1)
    .def("create", CSWSetEngineDirectionCommand_create2)
    .staticmethod("create")
  ;

  boost::python::class_<CSWFireCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWFireCommand", boost::python::no_init)
    .def("create", CSWFireCommand_create1)
    .def("create", CSWFireCommand_create2)
    .def("create", CSWFireCommand_create3)
    .staticmethod("create")
  ;

  boost::python::class_<CSWRechargeWeaponBatteryCommand, boost::python::bases<ARSTD::Command>, boost::noncopyable>("CSWRechargeWeaponBatteryCommand", boost::python::no_init)
    .def("create", &CSWRechargeWeaponBatteryCommand::create)
    .staticmethod("create")
  ;


  boost::python::class_<CSWCommands, boost::noncopyable>("CSWCommands", boost::python::no_init)
    .def("setMainEngineIntensity", &CSWCommands::setMainEngineIntensity)
    .staticmethod("setMainEngineIntensity")
    .def("setMainEngineDirection", CSWCommands_setMainEngineDirection1)
    .def("setMainEngineDirection", CSWCommands_setMainEngineDirection2)
    .staticmethod("setMainEngineDirection")
    .def("disableMainEngine", &CSWCommands::disableMainEngine)
    .staticmethod("disableMainEngine")
    .def("setBuoyancyTank", &CSWCommands::setBuoyancyTank)
    .staticmethod("setBuoyancyTank")
    .def("disableBuoyancyTank", &CSWCommands::disableBuoyancyTank)
    .staticmethod("disableBuoyancyTank")
    .def("setAxialInclinationJetOar", &CSWCommands::setAxialInclinationJetOar)
    .staticmethod("setAxialInclinationJetOar")
    .def("disableAxialInclinationJetOar", &CSWCommands::disableAxialInclinationJetOar)
    .staticmethod("disableAxialInclinationJetOar")
    .def("disableAllJetOars", &CSWCommands::disableAllJetOars)
    .staticmethod("disableAllJetOars")
    .def("disableAllEngines", &CSWCommands::disableAllEngines)
    .staticmethod("disableAllEngines")
    .def("stopRotation", &CSWCommands::stopRotation)
    .staticmethod("stopRotation")
//     .def("setOrientation", &CSWCommands::setOrientation)
//     .staticmethod("setOrientation")
    .def("setOrientationAbsoluteTowardPosition", &CSWCommands::setOrientationAbsoluteTowardPosition)
    .staticmethod("setOrientationAbsoluteTowardPosition")
    .def("setOrientationAbsoluteTowardDirection", &CSWCommands::setOrientationAbsoluteTowardDirection)
    .staticmethod("setOrientationAbsoluteTowardDirection")
//     .def("setNormalOrientation", &CSWCommands::setNormalOrientation)
//     .staticmethod("setNormalOrientation")
    .def("stopMoving", &CSWCommands::stopMoving)
    .staticmethod("stopMoving")
//     .def("moveAbsolute", &CSWCommands::moveAbsolute)
//     .staticmethod("moveAbsolute")
    .def("moveForward", &CSWCommands::moveForward)
    .staticmethod("moveForward")
    .def("moveUpward", &CSWCommands::moveUpward)
    .staticmethod("moveUpward")
    .def("stopAll", &CSWCommands::stopAll)
    .staticmethod("stopAll")
    .def("fireSalvo", &CSWCommands::fireSalvo)
    .staticmethod("fireSalvo")
  ;


  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::MacroCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::PushCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::PopCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::RepeatCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::CleanupHistoryCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWFireCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSendEventCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWWaitCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSetEngineIntensityCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSetEngineDirectionCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWRechargeWeaponBatteryCommand> >();
  boost::python::register_ptr_to_python<std::shared_ptr<ARSTD::CommandProcessor> >();
}
