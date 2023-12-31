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
#include <vector>
#include <deque>
#include <cmath>
#include <cmath>
//#include <utility>

// Rasfled Includes
#include "stringthings.h"
#include "system.h"
#include "helper.h"
#include "fled_time.h"
#include "nmea.h"
#include "aircraft_coordinator.h"
#include "screen4_helper.h"
#include "widgets.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;


// ---------------------------------------------------------------------------------------
// Map Tools

ImVec2 point_position_center(ImVec4 Working_Area);

ImVec2 point_position_lat_lon(ImVec4 Working_Area, ImVec2 Scale, 
                                ImVec2 Lat_Lon_Center, ImVec2 Lat_Lon, bool &Drawn);

ImVec2 point_position(ImVec4 Working_Area, ImVec2 Position);

// ---------------------------------------------------------------------------------------
// Markers

void draw_moving_marker(system_data &sdSysData, ImVec2 Screen_Position, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2);

void draw_marker(system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_airport_marker(system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_point_marker(ImVec2 Screen_Position, ImColor Color, float Size);

void draw_track(system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                NEW_COLOR_SCALE &Color_Scale, ImVec2 Center_Lat_Lon, DETAILED_TRACK &Track);

// ---------------------------------------------------------------------------------------

class MAP_MARKER
{
  public:

  ImVec2 LAT_LON;
  string DISPLAY_NAME = "";
  string LONG_NAME = "";
  int TYPE = 0;

  // Type 1
  vector<float> AIRPORT_LANDING_VECTORS;
  vector<ImVec2> REGION_GPS_COORDS;

  // Types:
  //  0 - Generic
  //  1 - Airport
  //  2 - Region
  //  3 - Interstate

  void clear();

  void draw(system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, ImVec2 Center_Lat_Lon, float Range);
};

// ---------------------------------------------------------------------------------------

class ADSB_WIDGET_Properties
// Properties (duh)
{
  public: 

  int ID = 0;
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

  int EXPIRATION_TIME = 5 * 60000;
  TIMED_PING EXPIREED;
  bool WIDGET_ACTIVE = false;

  // Directional varables
  MIN_MAX_TIME ALTITUDE_DIRECION;
  MIN_MAX_TIME SPEED_DIRECION;
  MIN_MAX_TIME SIG_STR_DIRECION;

  // Last known good cood data
  float LATITUDE = 0.0f;
  float LONGITUDE = 0.0f;

  DETAILED_TRACK TRACK;
  NEW_COLOR_SCALE ALTITUDE_COLOR_SCALE;

  public:

  AIRCRAFT AIRCRAFT_DATA;

  ADSB_WIDGET_Properties PROP;

  void create(system_data &sdSysData);

  void clear();
  // Clear values

  bool is_expired(system_data &sdSysData);

  bool changed();
  //  Return true is screen will be redrawn on next draw.
  //  Return false if no changes made.

  void update_aircraft(AIRCRAFT Aircraft, unsigned long tmeCurrentMillis);
  // Update values of gadget
  //  Gadget will be redrawn if values did changed or animations scheduled. 

  bool draw(system_data &sdSysData);
  // Draw all changes to Panel.
  //  Set Refresh to true to force redraw.
  //  Animations will be ignored without time reference.
  // Returns true if panel was redrawn.

  bool active();

  void draw_aircraft_map_marker(system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, ImVec2 Center_Lat_Lon);
};

// ---------------------------------------------------------------------------------------

class DISPLAY_DATA_ADSB
{
  public:

  bool ADSB_ACTIVE = false;

  AIRCRAFT_DATA AIRCRAFT_LIST;

  int ADSB_WIDGET_Count = 0;

  string TIME_OF_SIGNAL = "";
  string POSITIONED_COUNT = "";
  string POSITIONED_AIRCRAFT = "";
};

class ADSB_RANGE_Properties
{
  public:

  int COLOR;
};

class ADSB_RANGE
{
  private:
  
  int ZOOM_LEVEL = -1;
  float RANGE = 25.0f;
  float RADIUS_CIRCLE_POINT_SIZE = 0.0f;
  ImVec2 LAT_LON_TO_POINT_SCALE;

  ImVec4 PREV_WORKING_AREA;

  ImVec2 CENTER_LAT_LON;

  ImVec2 GPS_POS_LAT_LON;
  bool GPS_DISPLAY_CURRENT_LOCATION = false;

  void calculate_lat_lon_to_point_scale();

  void set_zoom_level();

  public:

  ADSB_RANGE_Properties PROPS;

  ImVec2 ll_2_pt_scale();

  ImVec2 center_lat_lon();

  void gps_display_current_location_toggle();

  bool gps_display_current_location();

  void set_gps_pos_lat_lon(ImVec2 Lat_Lon);

  ImVec2 gps_pos_lat_lon();

  float range();

  void set_range(float Range_Miles);

  void zoom_in();

  void zoom_out();

  void set_current_center_position(ImVec2 Lat_Lon);
  
  void draw(system_data &sdSysData, ImVec4 Working_Area);

  void draw_info();
};

// ---------------------------------------------------------------------------------------

class ADSB_MAP_Properties
{
  

};

class ADSB_MAP
{
  private:
  
  ADSB_RANGE RANGE_INDICATOR;
  deque<MAP_MARKER> LANDMARKS;

  NEW_COLOR_SCALE GPS_ALTITUDE_COLOR_SCALE;

  bool DISPLAY_LOCATION = false;

  void add_landmark(ImVec2 Lat_Lon, string Display_Name, int Type);

  public:

  void create(system_data &sdSysData);

  void draw(system_data &sdSysData,   DISPLAY_DATA_ADSB &SDATA, deque<ADSB_WIDGET> &ADSB_Widgets);

};

// ---------------------------------------------------------------------------------------

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
  ADSB_MAP ADSB_MAP_DISPLAY;

  int find_HEX(string Hex);
  // Gadget Internal:
  //  returns gadget position of aircraft with Hex ID

  int find_expired();
  // Gadget Internal:
  //  returns gadget position of aircraft with time expired.

  public:

  void create(system_data &sdSysData);
  
  void update(system_data &sdSysData);

  //void display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms,
  //              const char *name, bool *p_open, ImGuiWindowFlags flags);
  void display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms);
};

// ---------------------------------------------------------------------------------------
#endif