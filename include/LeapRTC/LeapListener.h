#pragma once

#include "Leap.h"

class LeapListener : public Leap::Listener {
 private:

 public:
  Leap::Frame m_Frame;
 public:
  LeapListener();
  virtual ~LeapListener();

  virtual void onInit(const Leap::Controller&);
  virtual void onConnect(const Leap::Controller&);
  virtual void onDisconnect(const Leap::Controller&);
  virtual void onExit(const Leap::Controller&);
  virtual void onFrame(const Leap::Controller&);
  virtual void onFocusGained(const Leap::Controller&);
  virtual void onFocusLost(const Leap::Controller&);
};
