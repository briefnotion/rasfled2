// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_adsb.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_ADSB_H
#define WIDGETS_ADSB_H

#include <stdio.h>
#include <string>

// Rasfled Includes
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

  void display(const char *name, bool *p_open, ImGuiWindowFlags flags);
};
*/

// ---------------------------------------------------------------------------------------

class ADSB_SCREEN
{
  private:
  
  WIDGET_DEFAULTS DEFAULTS;

  bool ADSB_ACTIVE = false;

  public:

  void display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                COLOR_COMBOS &Color_Select, 
                const char *name, bool *p_open, ImGuiWindowFlags flags);

  void set_adsb_active(bool Active);
};

// ---------------------------------------------------------------------------------------
#endif