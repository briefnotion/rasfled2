// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_automobile.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_AUTOMOBILE_H
#define WIDGETS_AUTOMOBILE_H

#include <stdio.h>
#include <string>

// Rasfled Includes
#include "system.h"
#include "auto.h"
#include "screen4_helper.h"
#include "widgets.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

/*
class BLANK
{
  private:

  public:

  void display( );
};
*/

// ---------------------------------------------------------------------------------------

class DISPLAY_DATA_AUTOMOBILE
{
  public:

  int MESSAGES = -1;
  string SPEED = "X";
  
  string VOLTAGE = "X";

  string LATEST_UNKNOWN_MESSAGE = "";

  bool CRUISE_CONTROL_SET = false;
  float CRUISE_CONTROL_SET_SPEED = -1;

  float ACCELERATION = 0;

  string RPM = "X";
  string TORQUE = "X";

  string STEERING_WHEEL_ANGLE = "X";
  string STEERING_WHEEL_LEFT_OF_CENTER = "X";
  string STEERING_WHEEL_TURNING_DIRECTION = "X";

  string TEMP_AMBIANT = "X";
  string TEMP_INTAKE = "X";
  string TEMP_COOLANT = "X";
  string TEMP_CATALYST = "X";
  float TEMP_SUPER_TEMP = 0;

  string FUEL_RAIL_PRESSURE = "X";
  string EVAP_SYSTEM_VAP_PRESSURE = "X";

  string BAROMETER = "X";
};

class AUTOMOBILE_SCREEN
{
  private:

  DISPLAY_DATA_AUTOMOBILE SDATA;
  
  WIDGET_DEFAULTS DEFAULTS;

  public:

  void update(system_data &sdSysData, unsigned long tmeFrame_Time);

  void display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                const char *name, bool *p_open, ImGuiWindowFlags flags);

  
};

// ---------------------------------------------------------------------------------------
#endif