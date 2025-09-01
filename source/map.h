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

using namespace std;

#define MAP_POINT_TYPE_GENERIC  0
#define MAP_POINT_TYPE_AIRPORT  1
#define MAP_POINT_TYPE_REGION   2
#define MAP_POINT_TYPE_ROAD     3

// -------------------------------------------------------------------------------------

class PIXEL_SIZE_META_DATA
{
  private:
  float CURRENT_CIRCLE_SIZE_PIXELS = 0.0f;

  void create();

  public:
  vector<double> LOD;
  // Pixel size in miles per LOD

  void check_circle_size_pixels(float Circle_Size_Pixels);
};

class MAP_INFO
{
  public:

  DOUBLE_VEC2 LAT_LON;
  string DISPLAY_NAME = "";
  string LONG_NAME = "";
  int TYPE = 0;

  // Type 1
  vector<float> AIRPORT_LANDING_VECTORS;
  vector<DOUBLE_VEC2> REGION_GPS_COORDS;

  // Types:
  //  0 - Generic
  //  1 - Airport
  //  2 - Region
  //  3 - Interstate

  void clear();
};


class MAP_PROPERTIES
{
  public:

  string FILENAME_GENERICS_MAP = "";
  string FILENAME_AIRPORTS_MAP = "";
  string FILENAME_REGIONS_MAP = "";
  string FILENAME_ROADS_MAP = "";
  string CURRENT_TRACK_FILENAME = "";

  unsigned long SAVE_TRACK_TIMER =  9 * 60 * 1000;
  //unsigned long SAVE_TRACK_TIMER =  10 * 1000;

};

class MAP
{
  private:
  TIMED_IS_READY  SAVE_TRACK_TIMER;

  void add_landmark(DOUBLE_VEC2 Lat_Lon, string Display_Name, int Type);
  // Adds landmark to vector list.

  bool map_hardcoded_load();

  bool map_load_json(string Filename);

  bool map_save();
  bool map_load();

  bool track_save(DETAILED_TRACK &Track, string Filename);
  bool track_load(DETAILED_TRACK &Track, string Filename);

  bool track_save_detailed(DETAILED_TRACK_ALTERNATIVE &Track, string Filename);
  bool track_load_detailed(DETAILED_TRACK_ALTERNATIVE &Track, string Filename);
  void generate_displayed_track();

  vector<DOUBLE_VEC2> LOD_TRACK_LIST;

  bool TEST_ALTERNATIVE = false;

  public:
  MAP_PROPERTIES PROPS;
  
  deque<MAP_INFO> LANDMARKS;
  DETAILED_TRACK              TRACK;
  DETAILED_TRACK_ALTERNATIVE  TRACK_2;
  DETAILED_TRACK_ALTERNATIVE  DISPLAYED_TRACK;

  PIXEL_SIZE_META_DATA LEVEL_OF_DETAIL_META;

  string INFORMATION = "";

  bool create();

  void load_track(CONSOLE_COMMUNICATION &cons);

  void update( CONSOLE_COMMUNICATION &cons, NMEA &GPS_System, unsigned long tmeFrame_Time, 
                  float Radius_Circle_Point_Size, int Current_LOD, bool Test_Alternative);
};

// -------------------------------------------------------------------------------------

#endif