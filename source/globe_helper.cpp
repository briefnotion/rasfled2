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

float calculate_angle(float Distance, float Height) 
{
  return ((atan2(Height, Distance) * 180.0f) / float_PI);
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

// ---------------------------------------------------------------------------------------

void DETAILED_TRACK::clear()
{
  TRACK_POINTS_DETAILED.clear();
  TRACK_POINTS_SIMPLE.clear();
}

void DETAILED_TRACK::store(DETAILED_TRACK_POINT New_Track_Point)
{
  if (TRACK_POINTS_DETAILED.size() == 0)
  {
    TRACK_POINTS_DETAILED.reserve(MAX_SIZE_DETAILED);
    TRACK_POINTS_SIMPLE.reserve(MAX_SISE_SIMPLE);
  }

  // TRACK_POINTS_DETAILED needs resizing if too large.
  if ((int)TRACK_POINTS_DETAILED.size() > (MAX_SIZE_DETAILED - 10))
  {
    vector<DETAILED_TRACK_POINT> temp_track_points;
    temp_track_points.reserve(MAX_SIZE_DETAILED);

    SIMPLE_TRACK_POINT temp_track_point_simple;

    // start by moving only large time values
    bool large_time = true;

    for (int pos = 0; pos < (int)TRACK_POINTS_DETAILED.size(); pos++)
    {
      // if point is within resize range
      if (pos <= ((float)MAX_SIZE_DETAILED * 0.95f))
      {
        if (large_time)
        {
          // if point time and next point time diff is more than 15 seconds, move it to the 
          //  simple vector.
          if (TRACK_POINTS_DETAILED[pos +1].TIME - TRACK_POINTS_DETAILED[pos].TIME > 15000.0f)
          {
            temp_track_point_simple.LATITUDE = TRACK_POINTS_DETAILED[pos].LATITUDE;
            temp_track_point_simple.LONGITUDE = TRACK_POINTS_DETAILED[pos].LONGITUDE;

            TRACK_POINTS_SIMPLE.push_back(temp_track_point_simple);
          }
          else 
          {
            // time iteration size getting small, divide from here till size limit reached.
            large_time = false;
          }
        }
        else
        // only copy it to the tmp new track if its an even position.
        {
          if (pos % 2 == 0)
          {
            temp_track_points.push_back(TRACK_POINTS_DETAILED[pos]);
          }
        }
      }
      else
      {
        // copy remaining points to the tmp new track.
        temp_track_points.push_back(TRACK_POINTS_DETAILED[pos]);
      }
    }

    // swap tmp new track to the old track position. 
    TRACK_POINTS_DETAILED.swap(temp_track_points);

    // should destruct itself, if noticed its not try
    //temp_track_points.clear();
    //temp_track_points.shrink_to_fit();
  }

  TRACK_POINTS_DETAILED.push_back(New_Track_Point);
}


#endif