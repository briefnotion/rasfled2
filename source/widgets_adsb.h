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
#include "map.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;


// ---------------------------------------------------------------------------------------
// Map Tools

ImVec2 point_position(ImVec4 Working_Area, ImVec2 Position);
// Returns Screen coordinates of center of working area offset by Position.

// ---------------------------------------------------------------------------------------
// Markers
void draw_line(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size);

void draw_box(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size);

void draw_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_marker_filled(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_airport_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color);

void draw_point_marker(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, float Size);

void draw_track(ImDrawList *Draw_List, system_data &sdSysData, 
                ImVec4 &Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                float Initial_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK &Track);

void draw_track_2(ImDrawList *Draw_List, system_data &sdSysData, 
                  ImVec4 &Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                  float Initial_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                  DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK &Track);

AIRCRAFT draw_aircraft_map_marker(AIRCRAFT_MAP_DETAILS Aircraft, ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                                  int Draw_Level_Of_Detail, DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, NEW_COLOR_SCALE &Altitude_Color_Scale);

// ---------------------------------------------------------------------------------------

class ADSB_RANGE_Properties
{
  public:

  int COLOR;
};

class ADSB_RANGE
{
  private:
  
  //int ZOOM_LEVEL = -1;
  
  ImVec4 WORKING_AREA;
  ImVec2 CENTER;

  float RANGE = 25.0f;                    // Miles for first circle
  IMPACT_RESISTANCE_FLOAT RANGE_IMP;
  float RANGE_IMP_LATEST = 0.0f;          // Miles for first circle
  float RANGE_BLOCK_CURRENT = 0.0f;       // Current range block size determined by RANGE_IMP_LATEST
  float RANGE_POINT_DISTANCE = 0.0f;

  ImVec2 LAT_LON_TO_POINT_SCALE;          // Required for calculating map draw routines
                                          // in point_position_lat_lon
                                          
  DOUBLE_VEC2 NO_POS_LAT_LON;       // Stores focus point when map focus set to 0
  DOUBLE_VEC2 GPS_POS_LAT_LON;      // Stores focus point when map focus set to 1
  DOUBLE_VEC2 AIRCRAFT_POS_LAT_LON; // Stores focus point when map focus set to 2

  ImVec2 CENTER_POS_LAT_LON;

  void calculate_lat_lon_to_point_scale(int &Map_Location_Focus);

  private:

  void set_zoom_level(int Zoom_Level);

  public:

  ADSB_RANGE_Properties PROPS;

  // Center of map at location
  //int CENTER_ON_LOCATION = 1;
    // -9 - No Location

    // -2 - Center on aircraft not set
    // -1 - Center on current location not set
    //  0 - No Location
    //  1 - Center on current location
    //  2 - Center on aircraft

  float RADIUS_CIRCLE_POINT_SIZE = 0.0f;

  bool AIRCRAFT_COUNT_ZERO_HOLD_OFF = false;

  ImVec2 ll_2_pt_scale();

  void gps_display_current_location_toggle(int &Map_Location_Focus);

  bool gps_display_current_location(int &Map_Location_Focus);

  void set_no_pos_lat_lon(DOUBLE_VEC2 Lat_Lon);
  void set_gps_pos_lat_lon(DOUBLE_VEC2 Lat_Lon, int &Map_Location_Focus);
  void set_aircraft_pos_lat_lon(DOUBLE_VEC2 Lat_Lon, int &Map_Location_Focus);

  DOUBLE_VEC2 get_center_lat_lon(int &Map_Location_Focus);
  DOUBLE_VEC2 get_gps_lat_lon();

  void create();
  // Prep the color, range size, and size of the impact variables

  void update_range_block_size();
  // Get size of current range block after RANGE_IMP_LATEST is updated.

  void range_update(unsigned long Frame_Time, int &Map_Location_Focus);
  // Update the impact returned range value for futer request. 
  //  Run once at begining of every frame for range.

  float range();
  // Distance in miles, the range is set at for viewing.

  void set_range(float Range_Miles);
  // Set the distance in miles, the range is set at for viewing.

  void zoom_in(system_data &sdSysData);
  // Decrease the distance in miles, the range is set at for viewing, 
  //  by one.

  void zoom_out(system_data &sdSysData);
  // Increase the distance in miles, the range is set at for viewing, 
  //  by one.

  void zoom_return(int Zoom_Level);
  // Set the zoom scale back to level.

  double resolution();
  // Current pixel size in miles.
  
  void draw_scale(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area);
  // Draws range circle onto the screen.

  void draw_info(int &Map_Location_Focus);
  // Draws ledgend info onto the screen
  //  Text Lat Lon Range
};

// ---------------------------------------------------------------------------------------

class ADSB_MAP
{
  private:
  
  // BUTTON_TOGGLE_COLOR
  BUTTON_TOGGLE_COLOR BTC_NORTH_UP;
  BUTTON_COLOR        BC_CENT_LOCATION;
  BUTTON_TOGGLE_COLOR BTC_CENT;
  BUTTON_TOGGLE_COLOR BTC_CALI;
  BUTTON_TOGGLE_COLOR BTC_LOCK;
  
  // BUTTON_COLOR
  BUTTON_TOGGLE_COLOR BC_DRV;
  BUTTON_TOGGLE_COLOR BC_MAX;
  BUTTON_TOGGLE_COLOR BC_MIN;
  BUTTON_COLOR        BC_PLUS;
  //BUTTON_COLOR      BC_LOC;
  BUTTON_COLOR        BC_MINUS;
  BUTTON_COLOR        BC_BEAR_RESET;
  BUTTON_COLOR        BC_CALI_RESET;
  BUTTON_TOGGLE_COLOR BTC_GPS_ASSIST;

  ADSB_RANGE RANGE_INDICATOR;

  NEW_COLOR_SCALE GPS_ALTITUDE_COLOR_SCALE;
  NEW_COLOR_SCALE ALTITUDE_COLOR_SCALE;

  bool SHOW_BUTTONS = true;
  TIMED_PING SHOW_BUTTONS_TIMER;

  IMPACT_RESISTANCE_FLOAT_FRAME_COUNT MAP_HEADING_DEGREES;
  float MAP_HEADING_DEGREES_LATEST = 0.0f;

  bool NORTH_UP = false;

  bool ACTIVE_GPS = false;
  bool ACTIVE_COMPASS = false;
  bool ACTIVE_ADSB = false;
  bool ACTIVE_AUTOMOBILE = false;

  ImVec4 WORKING_AREA;

  COMPASS_WIDGET CURRENT_POSITION_COMPASS;

  // Adds landmark to vector list.
  void draw_landmark( ImDrawList *Draw_List, system_data &sdSysData, MAP_INFO &Landmark, 
                      ImVec4 &Working_Area, ImVec2 Scale, 
                      DOUBLE_VEC2 Center_Lat_Lon, float &Map_Bearing, float Range);

  void screen_buttons(system_data &sdSysData);
  // Draws buttons onto the screen

  void screen_draw_calibration(ImDrawList *Draw_List, system_data &sdSysData);
  // Draws compass calibration data onto the screen

  void screen_text(system_data &sdSysData);
  // Draws text onto the screen
  //  Warining
  //  ADSB info
  //  GPS info
  //  Compass info

  void screen_draw_map_tracks(ImDrawList *Draw_List, system_data &sdSysData);
  //  Draws Map Tracks

  void screen_draw_position_marker(ImDrawList *Draw_List, system_data &sdSysData);
  //  Draw map position marker onto the screen
  
  void screen_draw_compass_center(ImDrawList *Draw_List, system_data &sdSysData);

  void screen_draw_aircraft(ImDrawList *Draw_List, system_data &sdSysData);
  //  Draw aircraft markers onto the screen
  //    Draws Tracks

  public:

  void create();

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

  // Map Variables
  ADSB_MAP ADSB_MAP_DISPLAY;

  void adsb_table_draw(system_data &sdSysData);

  public:

  void create();

  void display(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------
#endif