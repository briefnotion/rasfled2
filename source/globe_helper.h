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

// ---------------------------------------------------------------------------------------
// Map Tools

float calculate_distance(float lat1, float long1, float lat2, float long2);

ImVec2 get_coords_x_miles_from_coords(float Latitude, float Longitude, float Distance_Miles, float Bearing_Degrees);

// -------------------------------------------------------------------------------------

class DISTANCE
{
  private:

  float METERS = 0;

  public:

  void store_meters(float Meters);

  float meters_val();
  string meters();

  float feet_val();

};

class VELOCITY
{
  private:

  IMPACT_RESISTANCE_FLOAT KMPH;      // Base

  unsigned long TIME_STAMP = -1;            // Miliseconds.  Fairly loose timings.
  unsigned long TIME_STAMP_TIME_SENT = -1;  // Miliseconds.  Should be a more 
                                            //  accurate timing because set on sending 
                                            //  device. Latency of serial port send and 
                                            //  receive and processing is not factored.
                                            // May also be likely the latency is the same.
                                            //  May only matter when Serial Comm is congested.

  public:

  void store_kmph(float kmph, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_meters_per_second(float mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_knots(float knots, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);

  float val_kmph();
  float val_meters_per_second();
  float val_mph();
  float val_mph_impres(unsigned long tmeFrame_Time);
  string kmph();
  string mph();

  unsigned long time_stamp();
  unsigned long time_stamp_time_sent();
};

class GLOBAL_POSITION_DETAILED
{
  private:
  
  public:

  unsigned long SYSTTEM_UPDATE_TIME = 0;

  bool LIVE = true;

  float LATITUDE = 0;
  float LONGITUDE = 0;
  DISTANCE ALTITUDE;     // M
  DISTANCE GEOID_HEIGHT; // M (mean sea level above WGS84 ellipsoid)
  
  float TRUE_HEADING = 0;
  VELOCITY SPEED;

  float DILUTION_OF_POSITION = 0;

  bool VALID_COORDS = false;
  bool VALID_TRACK = false;

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