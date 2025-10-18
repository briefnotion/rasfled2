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
  double TRACK_HISTORY_CUTOFF = 8.0 * 60.0 * 60.0;   // 8 hours

};

class MAP
{
  private:
  TIMED_IS_READY  SAVE_TRACK_TIMER;
  TIMED_IS_READY  NEW_DISPLAY_TRACK_GENERATION_TIMER;

  double TIME_ERROR = 0.0;

  void build_kml_header(deque<string> &KML_Track, DETAILED_TRACK_POINT &First_Point);
  void build_kml_footer(deque<string> &KML_Track);

  void add_landmark(DOUBLE_VEC2 Lat_Lon, string Display_Name, int Type);
  // Adds landmark to vector list.

  bool map_load_json(string Filename);

  bool map_save();
  bool map_load();

  void track_distill( DETAILED_TRACK_ALTERNATIVE &Original_Track, 
                      DETAILED_TRACK_ALTERNATIVE &Recent_Track, 
                      DETAILED_TRACK_ALTERNATIVE &Old_Track,
                      deque<string> &KML_Track);
  // Reads Original Track. Splits and 
  // Returns Recent Track and Old track  

  void track_save_detailed_forgetable(DETAILED_TRACK_ALTERNATIVE &Track, string Filename);
  void track_save_kml_forgetable(deque<string> &KML_Track, string Filename);
  bool track_load_detailed( DETAILED_TRACK_ALTERNATIVE &Track, 
                            DETAILED_TRACK_ALTERNATIVE &Track_Discard, 
                            deque<string> &KML_Track, 
                            string Filename);

  void rebuild_track(CONSOLE_COMMUNICATION &cons);
  // Starts a track_distill on current track.
  // Track rebuild on large system time change.

  void generate_displayed_track(double Resolution);

  public:
  MAP_PROPERTIES PROPS;
  
  deque<MAP_INFO> LANDMARKS;
  DETAILED_TRACK_ALTERNATIVE  TRACK_2;
  DETAILED_TRACK_ALTERNATIVE  DISPLAYED_TRACK;

  PIXEL_SIZE_META_DATA LEVEL_OF_DETAIL_META;

  // This is a temporary solution for a problem that occures when 
  //  the track size displayed is more that 5000 points.
  // The program will not recover this value untill restarted.
  double MAX_TRACK_RESOLUTION_RESIZE_FACTOR = 1.0;

  string INFORMATION = "";

  // For Map Draw Routines, set in widgets_adsb
  float       RANGE_RADIUS_CIRCLE_POINT_SIZE = 136.800003f;
  double      CURRENT_RESOLUTION = -1.0;
  ImVec4      CURRENT_WORKING_AREA;
  DOUBLE_VEC2 CURRENT_CENTER_LAT_LON;
  float       CURRENT_MAP_BEARING;
  ImVec2      CURRENT_LAT_LON_SCALE;

  bool create();

  void load_track(CONSOLE_COMMUNICATION &cons, double Time_error);

  void update( CONSOLE_COMMUNICATION &cons, NMEA &GPS_System, unsigned long tmeFrame_Time);

  void close(CONSOLE_COMMUNICATION &cons);
};

// -------------------------------------------------------------------------------------

#endif