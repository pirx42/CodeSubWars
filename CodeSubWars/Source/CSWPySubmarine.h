// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include "CSWLog.h"
#include "CSWWorld.h"
#include "CSWWorldGuard.h"

#include "PythonContext.h"
#include "CSWIPythonable.h"
#include "CSWPythonable.h"

#include "CSWSubmarine.h"


namespace CodeSubWars
{

  /**
   * This is the base class for submarines defined in python. Submarines are the main actors in the world. Specialized submarines
   * are defined in python. Therefore its behavior is very different. All submarines have the same equipment and the same types of weapons
   * on bord. They are identically except its length. The length must be given in the constructor and must be in range [30, 110] m. The power of
   * engines and the capacity of the weapon batteries is depend on the submarine length. Larger submarines have more powerful engines and 
   * a higher capacity of the weapon batteries. But larger submarines can be better hit and detected by sonar than smaller ones. Smaller 
   * ones are a bit faster in controlling rotation and position.\n
   * Submarines are equipped with:\n
   *   - Engines:\n
   *    - \link CSWEngine MainEngine\endlink: This engine is mounted at the back and is directed toward the front. Its function to let the submarin move forward or backward.
   *                 Its direction can be changed within a range. The power is (depend on length \f$l\f$): \f$F_{max} [N] \approx l^3 * 90\f$ \n
   *    - \link CSWEngine BuoyancyTank\endlink: This "engine" is mounted in the center and is directed up. Its function to let the submarin move up or down.
   *                 The power is (depend on length \f$l\f$): \f$F_{max} [N] \approx l^3 * 0.12\f$ \n
   *    - \link CSWEngine BowJetOar\endlink: This engine is mounted at the front and is directed to the right. Its function to let the submarin rotate leftside or rightside.
   *                 The power is (depend on length \f$l\f$): \f$F_{max} [N] \approx l^3 * 4.4\f$ \n         
   *    - \link CSWEngine InclinationJetOar\endlink: This engine is mounted at the front and is directed up. Its function to let the submarin rotate upward or downward.
   *                 The power is (depend on length \f$l\f$): \f$F_{max} [N] \approx l^3 * 4.4\f$ \n         
   *    - \link CSWEngine AxialInclinationJetOar\endlink: This engine is mounted at the middle top and is directed to the right. Its function to let the submarin rotate
   *                 about his main axis. The power is (depend on length \f$l\f$): \f$F_{max} [N] \approx l^3 * 1.9\f$ \n 
   *    .
   *   - Sensors:\n
   *    - \link CSWActiveSonar ActiveSonar\endlink: This specialized active sonar is mounted in the center. The default scanning behavior is rotate horizontally.
   *                 It scanning behavior can be defined.\n
   *    - \link CSWPassiveSonar PassiveSonar\endlink: This specialized passive sonar is mounted in the center. The default scanning behavior is rotate horizontally.
   *                 It scanning behavior can be defined.\n
   *    - \link CSWGPS GPS\endlink: This gps system is mounted in the center. \n
   *    - \link CSWGyroCompass GyroCompass\endlink: This compass is mounted in the center.\n
   *    - \link CSWMovingPropertiesSensor MovingPropertiesSensor\endlink: This sensor is mounted in the center.\n
   *    .
   *   - Weapons:\n
   *    - \link CSWWeaponBattery FrontLeftWeaponBattery\endlink: This weapon battery is mounted at the front left. 
   *                 It contains \link GreenTorpedo green torpedos\endlink (the main attack weapon). 
   *                 The capacity is (depend on length \f$l\f$): \f$ N_{max} = l/2.5\f$ \n
   *    - \link CSWWeaponBattery FrontRightWeaponBattery\endlink: This weapon battery is mounted at the front right. 
   *                 It contains 50% \link RedTorpedo red torpedos\endlink and 50% \link BlueTorpedo blue torpedos\endlink.
   *                 The capacity is (depend on length \f$l\f$): \f$ N_{max} = l/10\f$ \n
   *    - \link CSWWeaponBattery BackWeaponBattery\endlink: This weapon battery is mounted at the back. 
   *                 It contains 50% \link GreenMine green mines\endlink and 50% \link YellowMine yellow mines\endlink.
   *                 The capacity is (depend on length \f$l\f$): \f$ N_{max} = l/5\f$ \n
   *    .
   *   - Other:\n
   *    - \link CSWTransceiver Transceiver\endlink: This transceiver is mounted in the center.\n
   *    - \link CSWMap Map\endlink: This map is mounted in the center. It handles reported position from \link CSWActiveSonar active sonar\endlink.\n
   *    .
   *   .
   */
  class CSWPySubmarine : public CSWSubmarine,
                         public CSWIPythonable
  {
    public:
      /**
       * Constructs a new submarine.
       * @param strName The name of the new submarine.
       * @param fLength The length of the new submarine.
       * @warning At this point the sensors have not correct values!
       */
      CSWPySubmarine(std::string strName, double fLength)
      : CSWSubmarine(strName, Matrix44D(), fLength)
      //, m_pPythonable(CSWPythonable::create())
      {
      }
    
      virtual ~CSWPySubmarine() {}

      /**
       * This method should be overriden if initializations must be done. Here all sensors return correct values.
       * Its ensured that this method is called before any other method (except constructor) is called.
       */
      virtual void initialize() { CSWSubmarine::initialize(); }
    
  //    /**
  //     * If you want store a reference to this object you _must_ use this method. Otherwise it could be happen that
  //     * this object is never destructed.
  //     * @return Returns a shared pointer to this.
  //     */
  //    std::shared_ptr<CSWObject> getSharedThis() { return CSWSubmarine::getSharedThis(); }

      /**
       * This method must be overriden in the specialized class. This methode is called periodically about every 10ms.
       * The main function should be checking sensors, seting up commands or something like that. The specialized class can implement everything 
       * here, but the execution time must not exceed 6ms in average. If it does the object will be kicked from the world!
       */
      virtual void update() { CSWSubmarine::update(); }

      /**
       * This method must be overriden in the specialized class. This methode is called if an event has been received by the object that should be 
       * processed. Returning true reports the eventmanager that the event passed to the object has successfully processed and delete it.
       * When returning false the event will be delivered the next time again until it expires or processed successfully.
       * The main function should be reacting to events e.g. seting up commands or something like that. The specialized class can implement everything 
       * here, but the execution time must not exceed 6ms in average. If it does the object will be kicked from the world!
       * @param pEvent The incoming event that should be processed.
       * @return True if the incoming event has been successfully processed. When returning false the event will be delivered next time again.
       */
      virtual bool processEvent(std::shared_ptr<CSWEvent> pEvent) { return CSWSubmarine::processEvent(pEvent); }

      /**
       * This method should be overriden if finalizations must be done.
       * Its ensured that this method is called directly before destructing.
       */
      virtual void finalize() { CSWSubmarine::finalize(); }
    
      /**
       * Returns the command processor.
       * The behavior is like in real life your 1st commander. You (the captain) makes a list of commands that must be done. This 
       * list is given to the commander. He cares about the correct execution. Ones you give him the list of commands you are able 
       * to do other things while your commander do the execution. 
       * @return The command processor.
       */
      std::shared_ptr<ARSTD::CommandProcessor> getCommandProcessor() { return CSWSubmarine::getCommandProcessor(); }

      virtual void step()
      {
        auto lck = m_pPythonContext->makeCurrent();
        //PyThreadState* state = getThreadState();
        //PyEval_AcquireThread(state);
        CSWSubmarine::step();
        //PyEval_ReleaseThread(state);
      }

      /**
       * Returns the main engine.
       * @return The main engine.
       */
      std::shared_ptr<CSWEngine> getMainEngine() { return CSWSubmarine::getMainEngine(); }

      /**
       * Returns the bows jet oar.
       * @return The bows jet oar.
       */
      std::shared_ptr<CSWEngine> getBowsJetOar() { return CSWSubmarine::getBowsJetOar(); }

      /**
       * Returns the inclination jet oar.
       * @return The inclination jet oar.
       */
      std::shared_ptr<CSWEngine> getInclinationJetOar() { return CSWSubmarine::getInclinationJetOar(); }

      /**
       * Returns the axial inclination jet oar.
       * @return The axial inclination jet oar.
       */
      std::shared_ptr<CSWEngine> getAxialInclinationJetOar() { return CSWSubmarine::getAxialInclinationJetOar(); }

      /**
       * Returns the buoyancy tank.
       * @return The buoyancy tank.
       */
      std::shared_ptr<CSWEngine> getBuoyancyTank() { return CSWSubmarine::getBuoyancyTank(); }

      /**
       * Returns the control center.
       * @return The control center.
       */
      std::shared_ptr<CSWControlCenter> getControlCenter() { return CSWSubmarine::getControlCenter(); }

      /**
       * Returns the front left weapon battery.
       * @return The front left weapon battery.
       */
      std::shared_ptr<CSWWeaponBattery> getFrontLeftWeaponBattery() { return CSWSubmarine::getFrontLeftWeaponBattery(); }

      /**
       * Returns the front right weapon battery.
       * @return The front right weapon battery.
       */
      std::shared_ptr<CSWWeaponBattery> getFrontRightWeaponBattery() { return CSWSubmarine::getFrontRightWeaponBattery(); }

      /**
       * Returns the back weapon battery.
       * @return The back weapon battery.
       */
      std::shared_ptr<CSWWeaponBattery> getBackWeaponBattery() { return CSWSubmarine::getBackWeaponBattery(); }

      /**
       * Returns the current health of the submarine.
       * @return The current health in range [0, 1] where 0 is dead and 1 is maximum.
       */
      const double& getHealth() const { return CSWSubmarine::getHealth(); }

      /**
       * Sets the color of the submarine.
       * @param vecColor The new red-green-blue-alpha - color. each color component in range [0, 1].
       */
      void setColor(const Vector4D& vecColor)
      { 
        Vector4D col(vecColor);
        col.a = 1; 
        CSWSubmarine::setColor(col); 
      }
    
      /**
       * Returns the color of the object.
       * @return The red-green-blue-alpha - color of the object.
       */
      const Vector4D& getColor() { return CSWSubmarine::getColor(); }
    
    
      /**
       * Transforms a direction given in world coordinate system into the local system.
       * @param vecWorldTDirection The direction in world coordinate system.
       * @return The direction in object coordinate system.
       */
      Vector3D makeLocalDirection(const Vector3D& vecWorldTDirection) { return CSWSubmarine::makeLocalDirection(vecWorldTDirection); }

      /**
       * Transforms a direction given in local coordinate system into the world coordinate system.
       * @param vecObjectTDirection The direction in local coordinate system.
       * @return The direction in world coordinate system.
       */
      Vector3D makeGlobalDirection(const Vector3D& vecObjectTDirection) { return CSWSubmarine::makeGlobalDirection(vecObjectTDirection); }

      /**
       * Transforms a position given in world coordinate system into the local system.
       * @param vecWorldTPosition The position in world coordinate system.
       * @return The position in object coordinate system.
       */
      Vector3D makeLocalPosition(const Vector3D& vecWorldTPosition) { return CSWSubmarine::makeLocalPosition(vecWorldTPosition); }

      /**
       * Transforms a position given in local coordinate system into the world coordinate system.
       * @param vecObjectTPosition The position in local coordinate system.
       * @return The position in world coordinate system.
       */
      Vector3D makeGlobalPosition(const Vector3D& vecObjectTPosition) { return CSWSubmarine::makeGlobalPosition(vecObjectTPosition); }
    

      //defined methods for pythonable
      //virtual void setThreadState(PyThreadState* pThreadState)
      //{
      //  m_pPythonable->setThreadState(pThreadState);
      //}


      //virtual PyThreadState* getThreadState()
      //{
      //  return m_pPythonable->getThreadState();
      //}


      //virtual void finalizePython()
      //{
      //  m_pPythonable->finalizePython();
      //}


      virtual std::shared_ptr<PythonContext> getPythonContext()
      {
        return m_pPythonContext;
      }

      virtual void setPythonContext(std::shared_ptr<PythonContext> context)
      {
        m_pPythonContext = context;
      }

    protected:
      //special properties
      //std::shared_ptr<CSWPythonable> m_pPythonable;
      std::shared_ptr<PythonContext> m_pPythonContext;

  };


  struct CSWPySubmarineWrapper : public CSWPySubmarine, boost::python::wrapper<CSWPySubmarine>
  {
    CSWPySubmarineWrapper(std::string name, double fLength) 
    : CSWPySubmarine(name, fLength),
      m_UpdateTimes(NUM_CYCLES),
      m_ProcessEventTimes(NUM_CYCLES)
    {
      for (int i = 0; i < NUM_CYCLES; ++i)
      {
        m_UpdateTimes.push_back(0);
        m_ProcessEventTimes.push_back(0);
      }
    }


    virtual void initialize()
    {
      bool hasInitialize = false;
    {
      auto lck = getPythonContext()->makeCurrent();
      //PyThreadState* state = CSWPySubmarine::getThreadState();
      //PyEval_AcquireThread(state);
      if (boost::python::override initialize = this->get_override("initialize"))
      {
        hasInitialize = true;
        try
        {
          initialize();
        }
        catch (...)
        {
          CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
          CSWLog::getInstance()->log(getName() + "::initialize() throws an exception");
          PyErr_Print();
        }
      }
      //PyEval_ReleaseThread(state);
    }

      if (!hasInitialize)
        CSWPySubmarine::initialize();
    }

    void default_initialize() 
    { 
      this->CSWPySubmarine::initialize(); 
    }

  
    virtual ~CSWPySubmarineWrapper() 
    {
      PyErr_Print();
    }


    virtual void update() 
    { 
      if (!isAlive())
        return;
    
      auto lck = getPythonContext()->makeCurrent();
      //PyThreadState* state = CSWPySubmarine::getThreadState();
      //PyEval_AcquireThread(state);
      try
      {
        double fStart = ARSTD::Time::getRealTime();
      
        this->get_override("update")(); 
      
        m_UpdateTimes.push_back(ARSTD::Time::getRealTime() - fStart);
        double fAvgTime = std::accumulate(m_UpdateTimes.begin(), m_UpdateTimes.end(), 0.0) / m_UpdateTimes.capacity();
        if (fAvgTime > Constants::CRITICAL_CONSUMING_TIME)
        {
          CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
          std::stringstream ss;
          ss << getName() << "::update() causes critical processor load (" 
             << fAvgTime*1000.0 << "ms)";
          CSWLog::getInstance()->log(ss.str());
        }
      }
      catch (...)
      {
        CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
        CSWLog::getInstance()->log(getName() + "::update() throws an exception");
        PyErr_Print();
      }
      //PyEval_ReleaseThread(state);
    }

  
    virtual bool processEvent(std::shared_ptr<CSWEvent> pEvent)
    {
      if (!isAlive())
        return true;

      auto lck = getPythonContext()->makeCurrent();
      //PyThreadState* state = CSWPySubmarine::getThreadState();
      //PyEval_AcquireThread(state);
      bool bResult = true;
      try
      {
        double fStart = ARSTD::Time::getRealTime();
      
        bResult = this->get_override("processEvent")(pEvent);
      
        m_ProcessEventTimes.push_back(ARSTD::Time::getRealTime() - fStart);
        double fAvgTime = std::accumulate(m_ProcessEventTimes.begin(), m_ProcessEventTimes.end(), 0.0) / m_UpdateTimes.capacity();
        if (fAvgTime > Constants::CRITICAL_CONSUMING_TIME)
        {
          CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
          std::stringstream ss;
          ss << getName() << "::processEvent() causes critical processor load (" 
              << fAvgTime*1000.0 << "ms)";
          CSWLog::getInstance()->log(ss.str());
        }
      }
      catch (...)
      {
        CSWWorld::getInstance()->getWorldGuard()->addErroneousObject(getSharedThis(), CSWWorldGuard::FATAL);
        CSWLog::getInstance()->log(getName() + "::processEvent() throws an exception");
        PyErr_Print();
      }
      //PyEval_ReleaseThread(state);
      return bResult;
    }


    virtual void finalize()
    {
      bool hasFinalize = false;

    {
      auto lck = getPythonContext()->makeCurrent();
      //PyThreadState* state = CSWPySubmarine::getThreadState();
      ////gil must not be held
      //PyEval_AcquireThread(state);
      ////gil held

      if (boost::python::override finalize = this->get_override("finalize"))
      {
        hasFinalize = true;
        try
        {
          //due to the user is able to define command that holds references to itself circular dependancies could 
          //be possible. to avoid not destructing explicit cleaning up the command processor. 
          getCommandProcessor()->cleanupStack();

          finalize();
        }
        catch (...)
        {
          //on exception this object is not added to erroneous objects because finalize is only called before destruction 
          //of the object. that means the object is already in removing "process".
          CSWLog::getInstance()->log(getName() + "::finalize() throws an exception");
          PyErr_Print();
        }
      }
      ////gil must be held
      //PyEval_ReleaseThread(state);
      ////gil not held
    }
      if (!hasFinalize)
        CSWPySubmarine::finalize();
    }

    void default_finalize() 
    { 
      this->CSWPySubmarine::finalize(); 
    }


    static const int NUM_CYCLES = 100;  
    boost::circular_buffer<double> m_UpdateTimes;
    boost::circular_buffer<double> m_ProcessEventTimes;
  };

}

using namespace CodeSubWars;

BOOST_PYTHON_MODULE(CodeSubWars_Submarine)
{

  boost::python::class_<CSWSubmarine, boost::python::bases<CSWObject>, boost::noncopyable>("CSWSubmarine", boost::python::no_init)
  ;

  boost::python::class_<CSWPySubmarineWrapper, std::shared_ptr<CSWPySubmarineWrapper>, boost::python::bases<CSWSubmarine>, boost::noncopyable>("CSWPySubmarine", 
          boost::python::init<std::string, double>())
//    .def("getSharedThis", &CSWPySubmarine::getSharedThis)
    .def("initialize", &CSWPySubmarine::initialize, &CSWPySubmarineWrapper::default_initialize)
    .def("update", boost::python::pure_virtual(&CSWPySubmarine::update))
    .def("processEvent", boost::python::pure_virtual(&CSWPySubmarine::processEvent))
    .def("finalize", &CSWPySubmarine::finalize, &CSWPySubmarineWrapper::default_finalize)

    .def("getMainEngine", &CSWPySubmarine::getMainEngine)
    .def("getBuoyancyTank", &CSWPySubmarine::getBuoyancyTank)
    .def("getBowsJetOar", &CSWPySubmarine::getBowsJetOar)
    .def("getInclinationJetOar", &CSWPySubmarine::getInclinationJetOar)
    .def("getAxialInclinationJetOar", &CSWPySubmarine::getAxialInclinationJetOar)

    .def("getControlCenter", &CSWPySubmarine::getControlCenter)

    .def("getFrontLeftWeaponBattery", &CSWPySubmarine::getFrontLeftWeaponBattery)
    .def("getFrontRightWeaponBattery", &CSWPySubmarine::getFrontRightWeaponBattery)
    .def("getBackWeaponBattery", &CSWPySubmarine::getBackWeaponBattery)

    .def("getCommandProcessor", &CSWPySubmarine::getCommandProcessor)
    .def("getHealth", &CSWPySubmarine::getHealth, boost::python::return_value_policy<boost::python::copy_const_reference>())
    
    .def("setColor", &CSWPySubmarine::setColor)
    .def("getColor", &CSWPySubmarine::getColor, boost::python::return_value_policy<boost::python::copy_const_reference>())
    
    .def("makeLocalDirection", &CSWPySubmarine::makeLocalDirection)
    .def("makeGlobalDirection", &CSWPySubmarine::makeGlobalDirection)
    .def("makeLocalPosition", &CSWPySubmarine::makeLocalPosition)
    .def("makeGlobalPosition", &CSWPySubmarine::makeGlobalPosition)
  ;
  
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSubmarine> >();
  
}
