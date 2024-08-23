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

// -------------------------------------------------------------------------------------

// Reserved Alert List
#define AUTO_RESERVE_ALERT_TEMP_S_TEMP     0
#define AUTO_RESERVE_ALERT_TEMP_COOLANT    1
#define AUTO_RESERVE_ALERT_ELEC_VOLTAGE    2
#define AUTO_RESERVE_ALERT_FUEL_LEVEL      3
#define AUTO_RESERVE_ALERT_TEMP_INTAKE     4
#define AUTO_RESERVE_ALERT_UNKNOWN_MESSAGE 5
#define AUTO_RESERVE_ALERT_MIL             6

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

  CRGB velocity_color = CRGB(6, 3, 0);
  CRGB drive_pulse_color = CRGB(30, 30, 30);

  NEO_COLOR_CRGB VELOCITY_COLOR;

  // control values
  const int ACTIVATE_SPEED = 10;
  const int CAUTION_SPEED = 5;

  void alert(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime);
  // Flash lights

  public:

  void update_events(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime);
};

// -------------------------------------------------------------------------------------

#endif