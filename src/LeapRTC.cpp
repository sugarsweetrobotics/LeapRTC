// -*- C++ -*-
/*!
 * @file  LeapRTC.cpp
 * @brief Leap Motion RTC
 * @date $Date$
 *
 * $Id$
 */

#include "LeapRTC.h"

#include "Leap.h"
// Module specification
// <rtc-template block="module_spec">
static const char* leaprtc_spec[] =
  {
    "implementation_id", "LeapRTC",
    "type_name",         "LeapRTC",
    "description",       "Leap Motion RTC",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "input",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
LeapRTC::LeapRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_frameOut("frame", m_frame)    

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
LeapRTC::~LeapRTC()
{
}



RTC::ReturnCode_t LeapRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("frame", m_frameOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t LeapRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t LeapRTC::onActivated(RTC::UniqueId ec_id)
{
  m_pLeapListener = new LeapListener();
  m_pLeapController = new Leap::Controller();
  m_pLeapController->addListener(*m_pLeapListener);

  return RTC::RTC_OK;
}



RTC::ReturnCode_t LeapRTC::onDeactivated(RTC::UniqueId ec_id)
{
  m_pLeapController->removeListener(*m_pLeapListener);
  delete m_pLeapController;
  delete m_pLeapListener;
  return RTC::RTC_OK;
}

inline void operator<<=(ssr::Vector &dst, const Leap::Vector &src) {
  dst.x = src.x;
  dst.y = src.y;
  dst.z = src.z;
}

inline void operator<<=(ssr::Orientation &dst, const Leap::Vector &src) {
  dst.pitch = src.pitch();
  dst.roll = src.roll();
  dst.yaw = src.yaw();
}

RTC::ReturnCode_t LeapRTC::onExecute(RTC::UniqueId ec_id)
{
  Leap::Frame &frame = m_pLeapListener->m_Frame;
  m_frame.id = frame.id();
  m_frame.timestamp = frame.timestamp();
  int nHands = frame.hands().count();
  m_frame.hands.length(nHands);
  for (int i = 0;i < nHands;i++) {
    m_frame.hands[i].palmPosition <<= frame.hands()[i].palmPosition();
    m_frame.hands[i].palmNormal   <<= frame.hands()[i].palmNormal();
    m_frame.hands[i].palmDirection <<= frame.hands()[i].direction();
    m_frame.hands[i].sphereCenter  <<= frame.hands()[i].sphereCenter();
    m_frame.hands[i].sphereRadius = frame.hands()[i].sphereRadius();
    m_frame.hands[i].palmOrientation <<= frame.hands()[i].direction();
    
    int nFinger = frame.hands()[i].fingers().count();
    m_frame.hands[i].fingers.length(nFinger);
    for (int j = 0;j < nFinger;j++) {
      m_frame.hands[i].fingers[j].tipPosition <<= frame.hands()[i].fingers()[j].tipPosition();
      m_frame.hands[i].fingers[j].touchZone = frame.hands()[i].fingers()[j].touchZone();
    }
  }

  const Leap::GestureList &gestures = m_pLeapListener->m_Frame.gestures();
  m_frame.gestures.length(gestures.count());
  for(int i = 0;i < gestures.count();i++) {
    const Leap::Gesture& gesture = gestures[0];
    m_frame.gestures[i].id = gesture.id();
    m_frame.gestures[i].type = gesture.type();
    switch(gesture.type()) {
    case Leap::Gesture::TYPE_CIRCLE: {
      const Leap::CircleGesture circle = gesture;
      m_frame.gestures[i].circle.state = gesture.state();
      m_frame.gestures[i].circle.progress = circle.progress();
      m_frame.gestures[i].circle.radius = circle.radius();
      m_frame.gestures[i].circle.center <<= circle.center();
      m_frame.gestures[i].circle.normal <<= circle.normal();
      break;
    }
    case Leap::Gesture::TYPE_SWIPE: {
      const Leap::SwipeGesture swipe = gesture;
      m_frame.gestures[i].swipe.state = gesture.state();
      m_frame.gestures[i].swipe.direction <<= swipe.direction();
      m_frame.gestures[i].swipe.position  <<= swipe.position();
      m_frame.gestures[i].swipe.startPosition  <<= swipe.startPosition();
      m_frame.gestures[i].swipe.speed = swipe.speed();
      break;
    }
    case Leap::Gesture::TYPE_KEY_TAP: {
      const Leap::KeyTapGesture key = gesture;
      m_frame.gestures[i].key.state = gesture.state();
      m_frame.gestures[i].key.direction <<= key.direction();
      m_frame.gestures[i].key.position  <<= key.position();
      break;
    }
    case Leap::Gesture::TYPE_SCREEN_TAP: {
      const Leap::ScreenTapGesture screen = gesture;
      m_frame.gestures[i].screen.state = gesture.state();
      m_frame.gestures[i].screen.direction <<= screen.direction();
      m_frame.gestures[i].screen.position  <<= screen.position();
      break;
    }
    case Leap::Gesture::TYPE_INVALID: {
      break;
    }
    }

  }



  m_frameOut.write();



  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t LeapRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t LeapRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void LeapRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(leaprtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<LeapRTC>,
                             RTC::Delete<LeapRTC>);
  }
  
};


