// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: map.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef MAP_H
#define MAP_H

// Libraries
#include <stdio.h>
#include <string>
#include <vector>

// Rasfled libraries
#include "globe_helper.h"
#include "json_interface.h"
#include "screen4_helper.h"
#include "nmea.h"
#include "map_hardcode.h"
#include "threading.h"

using namespace std;

void track_save_detailed(const DETAILED_TRACK_ALTERNATIVE &Track, const string Filename);

// -------------------------------------------------------------------------------------

class PIXEL_SIZE_META_DATA
{
  private:
  float CURRENT_CIRCLE_SIZE_PIXELS = 0.0f;

  void create();

  public:
  vector<double> LOD;
  // Pixel size in miles per LOD
  vector<DOUBLE_VEC2> LOD_TRACK_LIST;

  void check_circle_size_pixels(float Circle_Size_Pixels);
};

class MAP_PROPERTIES
{
  public:

  string FILENAME_GENERICS_MAP = "";
  string FILENAME_AIRPORTS_MAP = "";
  string FILENAME_REGIONS_MAP = "";
  string FILENAME_ROADS_MAP = "";
  string CURRENT_TRACK_FILENAME = "";
  string TRACK_HISTORY_FOLDER = "";

  unsigned long SAVE_TRACK_TIMER =   9 * 60 * 1000;

};

class MAP
{
  private:
  TIMED_IS_READY  SAVE_TRACK_TIMER;

  void add_landmark(DOUBLE_VEC2 Lat_Lon, string Display_Name, int Type);
  // Adds landmark to vector list.

  bool map_load_json(string Filename);

  bool map_save();
  bool map_load();

  bool track_save(DETAILED_TRACK &Track, string Filename);
  bool track_load(DETAILED_TRACK &Track, string Filename);

  void track_save_detailed_forgetable(DETAILED_TRACK_ALTERNATIVE &Track, string Filename);
  bool track_load_detailed( DETAILED_TRACK_ALTERNATIVE &Track, 
                            DETAILED_TRACK_ALTERNATIVE &Track_Discard, 
                            string Filename);
  void generate_displayed_track(double Resolution);

  public:
  MAP_PROPERTIES PROPS;
  
  deque<MAP_INFO> LANDMARKS;
  DETAILED_TRACK_ALTERNATIVE  TRACK_2;
  DETAILED_TRACK_ALTERNATIVE  TRACK_2_DISCARD;
  DETAILED_TRACK_ALTERNATIVE  DISPLAYED_TRACK;

  PIXEL_SIZE_META_DATA LEVEL_OF_DETAIL_META;

  string INFORMATION = "";

  // For Map Draw Routines, set in widgets_adsb
  float       RANGE_RADIUS_CIRCLE_POINT_SIZE = 136.800003f;
  double      CURRENT_RESOLUTION = -1.0;
  ImVec4      CURRENT_WORKING_AREA;
  DOUBLE_VEC2 CURRENT_CENTER_LAT_LON;
  float       CURRENT_MAP_BEARING;
  ImVec2      CURRENT_LAT_LON_SCALE;

  bool create();

  void load_track(CONSOLE_COMMUNICATION &cons);

  void update( CONSOLE_COMMUNICATION &cons, NMEA &GPS_System, unsigned long tmeFrame_Time);

  void close(CONSOLE_COMMUNICATION &cons);
};

// -------------------------------------------------------------------------------------

#endif