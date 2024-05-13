// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: automobile_handler.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTOMOBILE_HANDLER_H
#define AUTOMOBILE_HANDLER_H

// Standard Header Files
#include <stdio.h>

#include "screen4_helper.h"
#include "auto.h"
#include "animations_handler.h"
#include "LEDstuff.h"
#include "fled_time.h"
#include "alert_system_2.h"

using namespace std;

// -------------------------------------------------------------------------------------
//  

class AUTOMOBILE_HANDLER
{
  private:
  // 

  bool AUTO_ACTIVE = false;

  bool GEAR_PARK = false;
  bool GEAR_NEUTRAL = false;
  bool GEAR_REVERSE = false;
  bool GEAR_DRIVE = false;
  bool GEAR_LOW = false;

  bool SIGNAL_LEFT = false;
  bool SIGNAL_RIGHT = false;
  TIMED_PING SIGNAL_OFF_REDUNDANCY;

  bool HAZARDS = false;

  bool IGNITION = false;
  TIMED_PING IGNITION_WARNING_TIMER;
  TIMED_PING IGNITION_SHUTDOWN_TIMER;

  bool LIGHT_DRIVE_ON = false;
  bool LIGHT_VELOCITY_ON = false;
  bool LIGHT_DOOR_HANDLE_ON = false;

  TIMED_PING ALERT_TIMER;   // Restrict multiple sequential alerts

  void alert(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime);
  // Flash lights

  public:

  void update_events(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime);
};

// -------------------------------------------------------------------------------------

#endif