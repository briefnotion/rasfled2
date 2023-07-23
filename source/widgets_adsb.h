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
#include "system.h"
#include "helper.h"
#include "fled_time.h"
#include "aircraft_coordinator.h"
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

class ADSB_WIDGET_Properties
// Properties (duh)
{
  public: 

  int ID;
  string NAME = "";
  string LABEL = "";

  int COLOR = COLOR_WHITE;
  int BCOLOR = COLOR_BLACK;
  
  int POSY = 0;
  int POSX = 0;
  int SIZEY = 5;
  int SIZEX = 21;

  bool CHANGED = true;

  AIRCRAFT AIRCRAFT_DATA;
};

class ADSB_WIDGET
// Gadget display single aircraft information to panel with 
//  primary info.  Includes alt, ground speed, heading, 
//  some nav info, signal strength, and time to expire. 
// Set PROP (properties) before calling create routine.
// Warning:
//  Some properties can be changed directly but will not 
//    trigger the redraw or work correctly. 
//  To change properties properly, call the provided set 
//    routines or set routines of internal gadgets.
{
  
  private:

  // Gadget window
  //PANEL ADSB_PANEL;

  // Was gadget redrawn during the previous draw cycle.
  bool WAS_REDRAWN = false;

  int EXPIRATION_TIME = 15000;
  TIMED_PING EXPIREED;

  //string compass_mini_top(float Heading);

  //string compass_mini_bottom(float Heading);

  // Directional varables
  MIN_MAX_TIME ALTITUDE_DIRECION;
  MIN_MAX_TIME SPEED_DIRECION;
  MIN_MAX_TIME SIG_STR_DIRECION;

  // Last known good cood data
  float LATITUDE = 0;
  float LONGITUDE = 0;

  //Debug
  int Counter = 0;
  

  public:

  /*

  // Text Fields  
  Text_Field FLIGHT;
  Text_Field SQUAWK;

  Text_Field ALTITUDE;
  Text_Field_Multi_Line ALTITUDE_IND;
  Text_Field NAV_ALTITUDE_MCP; 

  Text_Field D_VERTICAL_RATE;
  Text_Field_Multi_Line D_VERTICAL_RATE_IND;

  Text_Field SPEED;
  Text_Field_Multi_Line SPEED_IND;

  Text_Field TRACK;
  Mini_Compass TRACK_MINI_COMPASS;
  
  Text_Field NAV_HEADING;
  Mini_Compass NAV_HEADING_MINI_COMPASS;

  Text_Field SIG_STR_IND;
  Text_Field COORD_TTL_IND;
  Text_Field DATA_TTL_IND;

  Text_Field MESSAGE;
  */  

  ADSB_WIDGET_Properties PROP;


  void create();
  // Prepare gadget to be drawn.  
  //  Define PROP (properties before calling this routine)
  //    Property Size and Position is necessary before calling create.

  void clear();
  // Clear values

  bool changed();
  //  Return true is screen will be redrawn on next draw.
  //  Return false if no changes made.

  void update_aircraft(AIRCRAFT Aircraft, unsigned long &tmeCurrentMillis);
  // Update values of gadget
  //  Gadget will be redrawn if values did changed or animations scheduled. 

  bool draw(bool Refresh, unsigned long tmeFrame_Time);
  // Draw all changes to Panel.
  //  Set Refresh to true to force redraw.
  //  Animations will be ignored without time reference.
  // Returns true if panel was redrawn.
};

class DISPLAY_DATA_ADSB
{
  public:

  bool ADSB_ACTIVE = false;

  AIRCRAFT_DATA AIRCRAFT_LIST;

  int ADSB_WIDGET_Count = 0;

  string TIME_OF_SIGNAL = "";
  string POSITIONED_COUNT = "";
  string POSITIONED_AIRCRAFT = "";

  /*
  int MESSAGES = 0;
  float SPEED = 0;
  float SUPER_TEMP = 0;
  string VOLTAGE = "X";
  */
};


class ADSB_SCREEN
{
  private:

  DISPLAY_DATA_ADSB SDATA;
  WIDGET_DEFAULTS DEFAULTS;

  deque<ADSB_WIDGET> ADSB_WIDGET_q;

  int find_HEX(string Hex);
  // Gadget Internal:
  //  returns gadget position of aircraft with Hex ID

  int find_expired();
  // Gadget Internal:
  //  returns gadget position of aircraft with time expired.
  
  public:
  
  void update(system_data &sdSysData, unsigned long tmeFrame_Time);

  void display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                COLOR_COMBOS &Color_Select, 
                const char *name, bool *p_open, ImGuiWindowFlags flags);
};

// ---------------------------------------------------------------------------------------
#endif