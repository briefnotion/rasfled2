// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_dot_dot_dot.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_DOT_DOT_DOT_H
#define WIDGETS_DOT_DOT_DOT_H

//#include <stdio.h>
//#include <string>
//#include <vector>

// Rasfled Includes
#include "system.h"
#include "LEDstuff.h"
#include "screen4_helper.h"
#include "widgets.h"
//#include "widgets_drawing.h"
#include "animations_handler.h"

//#include "stringthings.h"
//#include "fled_time.h"

// IMGui Includes
#include "../../imgui/imgui.h"
//#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

//void draw_marker_led(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);
void draw_led(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, 
              int Intensity, int low_level_indicator);

// ---------------------------------------------------------------------------------------

class DOT_DOT_DOT_SCREEN
{
  private:  
  
  // BUTTON_COLOR
  BUTTON_COLOR START_AUTO;
  BUTTON_COLOR STOP_AUTO;
  BUTTON_COLOR START_ADSB;
  BUTTON_COLOR STOP_ADSB;
  BUTTON_COLOR START_GPS;
  BUTTON_COLOR STOP_GPS;
  BUTTON_COLOR START_COMPASS;
  BUTTON_COLOR STOP_COMPASS;

  BUTTON_COLOR CLEAR_EVENTS;
  BUTTON_COLOR AUTO_TO_CONSOLE;
  BUTTON_COLOR GPS_TO_CONSOLE;

  BUTTON_TOGGLE_COLOR BRIGHTNESS_100;
  BUTTON_TOGGLE_COLOR BRIGHTNESS_50;
  BUTTON_TOGGLE_COLOR BRIGHTNESS_AUTO;

  int intensity(CRGB Color);

  ImColor color_multiplier(CRGB Color, int Intensity);

  public:

  void display(system_data &sdSysData, ANIMATION_HANDLER &Animations);
  // Draw all changes to Panel.
  // Returns true if panel was redrawn.

};

// ---------------------------------------------------------------------------------------
#endif