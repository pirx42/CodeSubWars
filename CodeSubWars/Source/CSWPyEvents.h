// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include "CSWEventManager.h"
#include "CSWEvent.h"

#include "CSWIMessage.h"
#include "CSWSystemMessage.h"
#include "CSWTextMessage.h"
#include "CSWTransceiverMessage.h"
#include "CSWCollisionDetectedMessage.h"
#include "CSWExplosionDetectedMessage.h"

using namespace CodeSubWars;


//static void sendEvent(std::shared_ptr<CSWEvent> pEvent)
//{
//  CSWEventManager::getInstance()->send(pEvent);    
//}
//
//
struct CSWPyEvent : public CSWEvent, boost::python::wrapper<CSWEvent>
{
  virtual ~CSWPyEvent() {}
  
  static PtrType createA(CSWObject* pSender, CSWObject* pReceiver, 
                         const std::shared_ptr<CSWIMessage> pMessage, double fExpireTime = 1)
  {
    if (!pSender || !pReceiver)
      throw std::runtime_error("sender or receiver is not defined");
    return CSWEvent::create(pSender->getSharedThis(), pReceiver->getSharedThis(), pMessage, fExpireTime);
  }
};



BOOST_PYTHON_MODULE(CodeSubWars_Events)
{

  boost::python::class_<CSWEventManager, boost::noncopyable>("CSWEventManager", boost::python::no_init)
    .def("send", &CSWEventManager::send)
//    .staticmethod("send")
  ;


  boost::python::class_<CSWPyEvent, std::shared_ptr<CSWPyEvent>, boost::noncopyable>("CSWEvent", boost::python::no_init)
    .def("create", &CSWPyEvent::createA)
    .staticmethod("create")
    .def("getMessage", &CSWPyEvent::getMessage)
  ;

  boost::python::class_<CSWIMessage, boost::noncopyable>("CSWIMessage", boost::python::no_init)
  ;

  boost::python::class_<CSWSystemMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWSystemMessage", boost::python::no_init)
    .def("getText", &CSWSystemMessage::getText, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWCollisionDetectedMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWCollisionDetectedMessage", boost::python::no_init)
    .def("getPoint", &CSWCollisionDetectedMessage::getPoint, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getDamage", &CSWCollisionDetectedMessage::getDamage, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWExplosionDetectedMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWExplosionDetectedMessage", boost::python::no_init)
    .def("getPoint", &CSWExplosionDetectedMessage::getPoint, boost::python::return_value_policy<boost::python::copy_const_reference>())
    .def("getDamage", &CSWExplosionDetectedMessage::getDamage, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWTextMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWTextMessage", boost::python::no_init)
    .def("create", &CSWTextMessage::create)
    .staticmethod("create")
    .def("getText", &CSWTextMessage::getText, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;

  boost::python::class_<CSWTransceiverMessage, boost::python::bases<CSWIMessage>, boost::noncopyable>("CSWTransceiverMessage", boost::python::no_init)
    .def("getText", &CSWTransceiverMessage::getText, boost::python::return_value_policy<boost::python::copy_const_reference>())
  ;


  boost::python::register_ptr_to_python<std::shared_ptr<CSWEventManager> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWEvent> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWIMessage> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWSystemMessage> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWCollisionDetectedMessage> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWExplosionDetectedMessage> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWTextMessage> >();
  boost::python::register_ptr_to_python<std::shared_ptr<CSWTransceiverMessage> >();
}
