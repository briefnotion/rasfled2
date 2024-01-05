// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: globe_helper.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef GLOBE_HELPER_CPP
#define GLOBE_HELPER_CPP

#include "globe_helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------
// Map Tools

float calculate_distance(float lat1, float lon1, float lat2, float lon2)
{
  // Convert latitude and longitude from degrees to radians
  lat1 = lat1 * float_PI / 180.0f;
  lon1 = lon1 * float_PI / 180.0f;
  lat2 = lat2 * float_PI / 180.0f;
  lon2 = lon2 * float_PI / 180.0f;

  // Calculate the differences between the coordinates
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  // Calculate the great circle distance using the haversine formula
  double a = pow(sin(dlat/2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon/2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  double R = 3963.191;  // Radius of the Earth in miles
  double distance = R * c;

  return (float)distance;
}


ImVec2 get_coords_x_miles_from_coords(float Latitude, float Longitude, float Distance_Miles, float Bearing_Degrees) 
{
  ImVec2 ret_coords;

  float lat1Rad = Latitude * float_PI / 180.0f;
  float lon1Rad = Longitude * float_PI / 180.0f;
  float angularDistance = Distance_Miles / float_EARTH_RADIUS;

  float lat2Rad = asin(sin(lat1Rad) * cos(angularDistance) + cos(lat1Rad) * sin(angularDistance) * cos(Bearing_Degrees * float_PI / 180.0f));
  float lon2Rad = lon1Rad + atan2(sin(Bearing_Degrees * float_PI / 180.0f) * sin(angularDistance) * cos(lat1Rad),
                                    cos(angularDistance) - sin(lat1Rad) * sin(lat2Rad));

  ret_coords.x = (lat2Rad * 180.0f / float_PI);
  ret_coords.y = (lon2Rad * 180.0f / float_PI);

  return ret_coords;
}

// -------------------------------------------------------------------------------------

void DISTANCE::store_meters(float Meters)
{
  METERS = Meters;
}

float DISTANCE::meters_val()
{
  return METERS;
}

string DISTANCE::meters()
{
  return to_string(METERS) + "m";
}

float DISTANCE::feet_val()
{
  return (METERS * 3.2808399f);
}

// -------------------------------------------------------------------------------------

void VELOCITY::store_kmph(float kmph, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  KMPH.set_value(tmeFrame_Time, kmph);  
  TIME_STAMP_TIME_SENT = tmeFrame_Time_Sent;
  TIME_STAMP = tmeFrame_Time;
}

void VELOCITY::store_meters_per_second(float mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  KMPH.set_value(tmeFrame_Time, velocity_translate_meters_per_second_to_kmph(mps));
  TIME_STAMP_TIME_SENT = tmeFrame_Time_Sent;
  TIME_STAMP = tmeFrame_Time;
}

void VELOCITY::store_knots(float knots, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  KMPH.set_value(tmeFrame_Time, velocity_translate_knots_to_kmph(knots));
  TIME_STAMP_TIME_SENT = tmeFrame_Time_Sent;
  TIME_STAMP = tmeFrame_Time;
}

float VELOCITY::val_kmph()
{
  return KMPH.latest();
}

float VELOCITY::val_meters_per_second()
{
  return velocity_translate_kmph_to_meters_per_second(KMPH.latest());
}

float VELOCITY::val_mph()
{
  return velocity_translate_kmph_to_mph(KMPH.latest());
}

float VELOCITY::val_mph_impres(unsigned long tmeFrame_Time)
{
  return velocity_translate_kmph_to_mph(KMPH.impact(tmeFrame_Time));
}

string VELOCITY::kmph()
{
  return to_string_round_to_nth(KMPH.latest(), 2);
}

string VELOCITY::mph()
{
  return to_string_round_to_nth(val_mph(), 2);
}

unsigned long VELOCITY::time_stamp()
{
  return TIME_STAMP;
}

unsigned long VELOCITY::time_stamp_time_sent()
{
  return TIME_STAMP_TIME_SENT;
}

//-----------


#endif