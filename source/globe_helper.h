// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: globe_helper.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef GLOBE_HELPER_H
#define GLOBE_HELPER_H

// Standard Header Files
//#include <stdio.h>
//#include <string>
#include <vector>

// RASFled related header files
//#include "system.h"
//#include "commands.h"

#include "stringthings.h"
#include "screen4_helper.h"
#include "comport.h"

// -------------------------------------------------------------------------------------

class DISTANCE
{
  private:

  float METERS = 0;

  public:

  void store_meters(float Meters);

  float meters_val();
  string meters();

};

class GLOBAL_POSITION_DETAILED
{
  public:

  unsigned long SYSTTEM_UPDATE_TIME = 0;

  bool LIVE = true;

  float LATITUDE = 0;
  float LONGITUDE = 0;
  DISTANCE ALTITUDE;     // M
  DISTANCE GEOID_HEIGHT; // M (mean sea level above WGS84 ellipsoid)

  float TIME = 0;
  
  float HEADING = 0;
  float SPEED = 0;

  float DILUTION_OF_POSITION = 0;

  bool VALID_COORDS = false;
  bool CHANGED = false;
};

class GLOBAL_POSITION
{
  public:
  STRING_FLOAT LATITUDE;
  STRING_FLOAT LONGITUDE;

  bool GLOBAL_POSITION_FOUND = false;
};

// -------------------------------------------------------------------------------------


#endif