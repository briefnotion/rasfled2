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
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include <ctime>

// RASFled related header files
#include "helper.h"
#include "stringthings.h"
#include "screen4_helper.h"
#include "comport.h"

// ---------------------------------------------------------------------------------------
// Map Tools

double degToRad(double deg);
double radToDeg(double rad);
double julianDay(int year, int month, int day, double hourUTC);

double calculate_distance(double lat1, double long1, double lat2, double long2);
// Returns distance in miles from first and second lat lon coords.

float calculate_angle(float Distance, float Height);
// Returns angle in degrees. Distance and Height need to be same unit type. 

DOUBLE_VEC2 get_coords_x_miles_from_coords(double Latitude, double Longitude, double Distance_Miles, float Bearing_Degrees);
// Returns latitude and longitude coordinates at Distance_Miles on Bearing.

float signed_angular_error(float Compass_Bearing, float GPS_Bearing);
// Clockwise offset is positive and counterclockwise is 
//  negative—perfect for compass error tracking or smooth directional logic.
// For calculating the signed angular difference between two 
//  bearings—ensuring the result is in the range of –180° to +180°.

float wrap_degrees(float angle);
// Ensures that any input angle is converted into the standard compass range of 
//  0° to less than 360°, regardless of whether it's positive, 
//  negative, or beyond a full rotation.

// -------------------------------------------------------------------------------------

double getSunAltitude(double lat, double lon, int year, int month, int day, double hourUTC);
float daylightFactor(double altitude);
float getCurrentDaylightFactor(double lat, double lon);

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

// -------------------------------------------------------------------------------------

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
  //float val_mph_impres(unsigned long tmeFrame_Time);
  string kmph();
  string mph();

  unsigned long time_stamp();
  unsigned long time_stamp_time_sent();
};

// -------------------------------------------------------------------------------------

class GLOBAL_POSITION_DETAILED
{
  private:
  
  public:

  unsigned long SYSTEM_UPDATE_TIME = 0;

  double LATITUDE = 0;
  double LONGITUDE = 0;
  DISTANCE ALTITUDE;     // M
  DISTANCE GEOID_HEIGHT; // M (mean sea level above WGS84 ellipsoid)
  
  FLOAT_VALID TRUE_HEADING;
  VELOCITY SPEED;

  float DILUTION_OF_POSITION = 0;

  // Post Processing
  bool VALID_GPS_FIX = false;
  bool VALID_COORDS = false;
  bool VALID_TRACK = false;

  bool CHANGED = false;
};

class GLOBAL_POSITION
{
  public:
  STRING_DOUBLE LATITUDE;
  STRING_DOUBLE LONGITUDE;

  bool GLOBAL_POSITION_FOUND = false;
};

// -------------------------------------------------------------------------------------

class SIMPLE_TRACK_POINT
{
  public:
  
  double LATITUDE = 0.0f;
  double LONGITUDE = 0.0f;
};

class DETAILED_TRACK_POINT
{
  public:
  
  double LATITUDE = 0.0f;
  double LONGITUDE = 0.0f;

  float TRUE_HEADING = 0;

  float TIME = 0.0f;            // Time im ms:
                                //  Represeting either
                                //  frame time of message received 
                                //  or from nmea source gga time.
  float RSSI_INTENSITY = 0.0f;
  float ALTITUDE = 0.0f;
};

class DETAILED_TRACK
{
  private:
  
  int MAX_SIZE_DETAILED = 1024;
  int MAX_SISE_SIMPLE = 1024;
  
  public:

  vector<DETAILED_TRACK_POINT> TRACK_POINTS_DETAILED;
  vector<SIMPLE_TRACK_POINT> TRACK_POINTS_SIMPLE;
  
  void clear();

  void store(DETAILED_TRACK_POINT New_Track_Point);
  // Points are stored in TRACK_POINTS_DETAILED.
  // When TRACK_POINTS_DETAILED gets too big, points are 
  //  transfed to TRACK_POINTS_SIMPLE.
};

// -------------------------------------------------------------------------------------


#endif