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
#include "hmc5883l.h"
#include "aircraft_coordinator.h"
#include "screen4_helper.h"
#include "widgets.h"
#include "widgets_drawing.h"
#include "globe_helper.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;


// ---------------------------------------------------------------------------------------
// Map Tools

float degrees_to_radians(float Degrees);

void rotate_point(ImVec2 Center, float Angle_In_Rads, ImVec2 &Point);

ImVec2 point_position_center(ImVec4 Working_Area);

ImVec2 point_position_lat_lon(ImVec4 Working_Area, ImVec2 Scale, 
                                ImVec2 Lat_Lon_Center, ImVec2 Lat_Lon, 
                                float Degrees, bool &Drawn);

ImVec2 point_position(ImVec4 Working_Area, ImVec2 Position);

// ---------------------------------------------------------------------------------------
// Markers
void draw_line(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size);

void draw_box(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size);

void draw_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_marker_filled(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_airport_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_point_marker(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, float Size);

void draw_track(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                float Strength_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                ImVec2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK &Track);

AIRCRAFT draw_aircraft_map_marker(AIRCRAFT_MAP_DETAILS Aircraft, ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                                  int Draw_Level_Of_Detail, ImVec2 Center_Lat_Lon, float Map_Bearing, NEW_COLOR_SCALE &Altitude_Color_Scale);

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

  void draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
            ImVec2 Center_Lat_Lon, float Map_Bearing, float Range);
};

// ---------------------------------------------------------------------------------------

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

  ImVec2 NO_POS_LAT_LON;
  ImVec2 GPS_POS_LAT_LON;
  ImVec2 AIRCRAFT_POS_LAT_LON;

  ImVec2 CENTER_POS_LAT_LON;

  void calculate_lat_lon_to_point_scale();

  void set_zoom_level();

  public:

  ADSB_RANGE_Properties PROPS;

  // Center of map at location
  int CENTER_ON_LOCATION = 1;
    // 0 - No Location
    // 1 - Center on current location
    // 2 - Center on aircraft

  ImVec2 ll_2_pt_scale();

  void gps_display_current_location_toggle();

  bool gps_display_current_location();

  void set_no_pos_lat_lon(ImVec2 Lat_Lon);
  void set_gps_pos_lat_lon(ImVec2 Lat_Lon);
  void set_aircraft_pos_lat_lon(ImVec2 Lat_Lon);

  ImVec2 get_center_lat_lon();
  ImVec2 get_gps_lat_lon();

  float range();
  // Distance in miles, the range is set at for viewing.

  void set_range(float Range_Miles);
  // Set the distance in miles, the range is set at for viewing.

  void zoom_in();
  // Decrease the distance in miles, the range is set at for viewing, 
  //  by one.

  void zoom_out();
  // Increase the distance in miles, the range is set at for viewing, 
  //  by one.
  
  void draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area);

  void draw_info();
};

// ---------------------------------------------------------------------------------------

class ADSB_MAP
{
  private:
  
  // BUTTON_TOGGLE_COLOR
  BUTTON_TOGGLE_COLOR BTC_NORTH_UP;
  BUTTON_TOGGLE_COLOR BTC_CENT;
  BUTTON_TOGGLE_COLOR BTC_CALI;
  BUTTON_TOGGLE_COLOR BTC_LOCK;
  
  // BUTTON_COLOR
  BUTTON_COLOR BC_PLUS;
  //BUTTON_COLOR BC_LOC;
  BUTTON_COLOR BC_MINUS;
  BUTTON_COLOR BC_BEAR_RESET;
  BUTTON_COLOR BC_CALI_RESET;

  ADSB_RANGE RANGE_INDICATOR;
  deque<MAP_MARKER> LANDMARKS;

  NEW_COLOR_SCALE GPS_ALTITUDE_COLOR_SCALE;
  NEW_COLOR_SCALE ALTITUDE_COLOR_SCALE;

  bool SHOW_BUTTONS = true;
  TIMED_PING SHOW_BUTTONS_TIMER;

  IMPACT_RESISTANCE_FLOAT_FRAME_COUNT MAP_HEADING_DEGREES;
  bool NORTH_UP = false;

  COMPASS_WIDGET CURRENT_POSITION_COMPASS;

  void add_landmark(ImVec2 Lat_Lon, string Display_Name, int Type);

  public:

  void create(system_data &sdSysData);

  void draw(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------

class ADSB_SCREEN
{
  private:
  
  // BUTTON_TOGGLE_COLOR
  BUTTON_TOGGLE_COLOR BTC_ADSB;

  // BUTTON_COLOR
  BUTTON_COLOR BC_VIEW;

  // Screens
  bool DISPLAY_TABLE = true;
  bool DISPLAY_MAP = false;

  // Map Variables
  ADSB_MAP ADSB_MAP_DISPLAY;

  void adsb_table_draw(system_data &sdSysData);

  public:

  void create(system_data &sdSysData);

  void display(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------
#endif