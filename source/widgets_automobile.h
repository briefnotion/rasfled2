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

class DISPLAY_DATA
{
  public:

  int MESSAGES = 0;
  float SPEED = 0;
  float SUPER_TEMP = 0;
  string VOLTAGE = "X";
};

class AUTOMOBILE_SCREEN
{
  private:

  DISPLAY_DATA DATA;

  public:

  void update(system_data &sdSysData, unsigned long tmeFrame_Time);

  void display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                const char *name, bool *p_open, ImGuiWindowFlags flags);

  
};

// ---------------------------------------------------------------------------------------
#endif