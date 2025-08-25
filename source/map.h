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

using namespace std;

// -------------------------------------------------------------------------------------

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

};

class MAP
{
  private:

  void add_landmark(DOUBLE_VEC2 Lat_Lon, string Display_Name, int Type);
  // Adds landmark to vector list.

  bool map_hardcoded_load();

  bool map_load_json(string Filename);

  bool map_save();
  bool map_load();

  public:
  MAP_PROPERTIES PROPS;
  
  deque<MAP_INFO> LANDMARKS;

  string INFORMATION = "";

  bool create();

};

// -------------------------------------------------------------------------------------

#endif