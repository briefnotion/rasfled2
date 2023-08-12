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

#define pi_adsb 3.14159265358979323846

#include <stdio.h>
#include <string>
#include <cmath>
#include <cmath>
//#include <utility>

// Rasfled Includes
#include "stringthings.h"
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
// ---------------------------------------------------------------------------------------

class W_TEXT_PROPERTIES
{
  public:

  COLOR_COMBO COLOR;
};

class W_TEXT
{
  private:

  string TEXT = "";

  public:

  W_TEXT_PROPERTIES PROPS;

  void update_text(unsigned long tmeCurrentMillis, string Text);

  void draw(unsigned long tmeCurrentMillis);

};
*/

// ---------------------------------------------------------------------------------------
// Map Tools

float calculate_distance(float lat1, float long1, float lat2, float long2);

ImVec2 get_miles_west_coords(float Latitude, float Longitude, float Distance_Miles);

// ---------------------------------------------------------------------------------------

class MAP_GLOBAL_POSITION
{
  public:
  STRING_FLOAT LATITUDE;
  STRING_FLOAT LONGITUDE;
};

// ---------------------------------------------------------------------------------------

class ADSB_RANGE_Properties
{
  public:

  COLOR_COMBO COLOR;
};

class ADSB_RANGE
{
  private:
  
  float RANGE = 25.0f;

  MAP_GLOBAL_POSITION CURRENT_POSITION;

  public:

  ADSB_RANGE_Properties PROPS;

  void set_range(float Range_Miles);

  void set_current_global_position(float Latitude, float Longitude);
  
  void draw(system_data &sdSysData, ImVec2 Available_Space);
};

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

  //bool CHANGED = true;
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

  //WIDGET_DEFAULTS DEFAULTS;

  // Was gadget redrawn during the previous draw cycle.
  //bool WAS_REDRAWN = false;

  int EXPIRATION_TIME = 15000;
  TIMED_PING EXPIREED;
  bool WIDGET_ACTIVE = false;

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
  //int Counter = 0;

  public:

  AIRCRAFT AIRCRAFT_DATA;

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

  bool draw(system_data &sdSysData);
  // Draw all changes to Panel.
  //  Set Refresh to true to force redraw.
  //  Animations will be ignored without time reference.
  // Returns true if panel was redrawn.

  bool active();
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

  // Screens
  bool DISPLAY_TABLE = true;
  bool DISPLAY_MAP = false;

  // Map Variables
  ADSB_RANGE RANGE_INDICATOR;

  int find_HEX(string Hex);
  // Gadget Internal:
  //  returns gadget position of aircraft with Hex ID

  int find_expired();
  // Gadget Internal:
  //  returns gadget position of aircraft with time expired.
  
  public:

  void create(system_data &sdSysData);
  
  void update(system_data &sdSysData);

  void display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms,
                const char *name, bool *p_open, ImGuiWindowFlags flags);
};

// ---------------------------------------------------------------------------------------
#endif