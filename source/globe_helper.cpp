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



double degToRad(double deg)
{ 
  return deg * float_PI / 180.0;
}

double radToDeg(double rad)
{
  return rad * 180.0 / float_PI;
}

// Julian Day calculation
double julianDay(int year, int month, int day, double hourUTC)
{
  if (month <= 2)
  {
    year--; month += 12;
  }
  int A = year / 100;
  int B = 2 - A + A / 4;
  return floor(365.25 * (year + 4716)) +
          floor(30.6001 * (month + 1)) +
          day + B - 1524.5 + hourUTC / 24.0;
}

double calculate_distance(double lat1, double lon1, double lat2, double lon2)
{
  // Convert latitude and longitude from degrees to radians
  lat1 = lat1 * double_PI / 180.0f;
  lon1 = lon1 * double_PI / 180.0f;
  lat2 = lat2 * double_PI / 180.0f;
  lon2 = lon2 * double_PI / 180.0f;

  // Calculate the differences between the coordinates
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  // Calculate the great circle distance using the haversine formula
  double a = pow(sin(dlat/2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon/2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  double R = 3963.191;  // Radius of the Earth in miles
  double distance = R * c;

  return distance;
}

float calculate_angle(float Distance, float Height) 
{
  return ((atan2(Height, Distance) * 180.0f) / float_PI);
}

DOUBLE_VEC2 get_coords_x_miles_from_coords(double Latitude, double Longitude, double Distance_Miles, float Bearing_Degrees) 
{
  DOUBLE_VEC2 ret_coords;

  double lat1Rad = Latitude * double_PI / 180.0f;
  double lon1Rad = Longitude * double_PI / 180.0f;
  double angularDistance = Distance_Miles / double_EARTH_RADIUS;

  double lat2Rad = asin(sin(lat1Rad) * cos(angularDistance) + cos(lat1Rad) * sin(angularDistance) * cos((double)Bearing_Degrees * double_PI / 180.0f));
  double lon2Rad = lon1Rad + atan2(sin((double)Bearing_Degrees * double_PI / 180.0f) * sin(angularDistance) * cos(lat1Rad),
                                    cos(angularDistance) - sin(lat1Rad) * sin(lat2Rad));

  ret_coords.x = (lat2Rad * 180.0f / double_PI);
  ret_coords.y = (lon2Rad * 180.0f / double_PI);

  return ret_coords;
}

float signed_angular_error(float Compass_Bearing, float GPS_Bearing)
{
  float diff = fmod(Compass_Bearing - GPS_Bearing + 540.0f, 360.0f) - 180.0f;
  return diff;
}

float wrap_degrees(float angle)
{
  float result = fmod(angle, 360.0f);
  if (result < 0.0f)
  {
    result += 360.0f;
  }
  return result;
}

// -------------------------------------------------------------------------------------

// Sun altitude calculation
double getSunAltitude(double lat, double lon, int year, int month, int day, double hourUTC) 
{
  double JD = julianDay(year, month, day, hourUTC);
  double T = (JD - 2451545.0) / 36525.0;

  // Sun's position
  double L0 = fmod(280.46646 + 36000.76983 * T, 360.0);
  double M  = 357.52911 + 35999.05029 * T;
  double e  = 0.016708634 - T * (0.000042037 + 0.0000001267 * T);

  double C = (1.914602 - T * (0.004817 + 0.000014 * T)) * sin(degToRad(M))
            + (0.019993 - 0.000101 * T) * sin(degToRad(2*M))
            + 0.000289 * sin(degToRad(3*M));
  double trueLong = L0 + C;
  double omega = 125.04 - 1934.136 * T;
  double lambda = trueLong - 0.00569 - 0.00478 * sin(degToRad(omega));

  double epsilon0 = 23.4392966666667 - T * (0.0130041666667 + 0.0000001638889 * T - 0.0000005036111 * T * T);
  double epsilon  = epsilon0 + 0.00256 * cos(degToRad(omega));

  // Sun declination
  double delta = radToDeg(asin(sin(degToRad(epsilon)) * sin(degToRad(lambda))));

  // Equation of time
  double y = tan(degToRad(epsilon/2)) * tan(degToRad(epsilon/2));
  double eqTime = 4 * radToDeg(y * sin(2*degToRad(L0))
                - 2*e*sin(degToRad(M))
                + 4*e*y*sin(degToRad(M))*cos(2*degToRad(L0))
                - 0.5*y*y*sin(4*degToRad(L0))
                - 1.25*e*e*sin(2*degToRad(M)));

  // True solar time
  double timeOffset = eqTime + 4 * lon; // in minutes
  double trueSolarTime = fmod(hourUTC * 60 + timeOffset + 1440, 1440);

  // Hour angle
  double hourAngle = (trueSolarTime / 4 < 0) ? trueSolarTime/4 + 180 : trueSolarTime/4 - 180;

  // Sun altitude
  double altitude = radToDeg(asin(sin(degToRad(lat)) * sin(degToRad(delta))
      + cos(degToRad(lat)) * cos(degToRad(delta)) * cos(degToRad(hourAngle))));
  return altitude;
}

// Map sun altitude to brightness factor 0.0–1.0
float daylightFactor(double altitude) 
{
  double minAlt = -6.0; // end of civil twilight
  double maxAlt = 6.0;  // fully bright
  
  if (altitude <= minAlt) return 0.0f;
  if (altitude >= maxAlt) return 1.0f;
  return static_cast<float>((altitude - minAlt) / (maxAlt - minAlt));
}

float getCurrentDaylightFactor(double lat, double lon) 
{
  // Get current time in UTC
  auto now = std::chrono::system_clock::now();
  std::time_t tt = std::chrono::system_clock::to_time_t(now);
  std::tm utc_tm = *std::gmtime(&tt);

  double hourUTC = utc_tm.tm_hour + utc_tm.tm_min / 60.0 + utc_tm.tm_sec / 3600.0;
  double alt = getSunAltitude(lat, lon, utc_tm.tm_year + 1900, utc_tm.tm_mon + 1, utc_tm.tm_mday, hourUTC);
  return daylightFactor(alt);
}

/*
// USAGE:
int main() {
    double lat = 34.0522;   // Example: Los Angeles
    double lon = -118.2437; // Negative for west

    float brightness = getCurrentDaylightFactor(lat, lon);
    std::cout << "Brightness factor: " << brightness << std::endl;
}
*/

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

/*
float VELOCITY::val_mph_impres(unsigned long tmeFrame_Time)
{
  return velocity_translate_kmph_to_mph(KMPH.impact(tmeFrame_Time));
}
*/

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
          if (TRACK_POINTS_DETAILED[pos +1].TIMESTAMP - TRACK_POINTS_DETAILED[pos].TIMESTAMP > 15000.0f)
          {
            temp_track_point_simple.TIMESTAMP = TRACK_POINTS_DETAILED[pos].TIMESTAMP;
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

/*
void ComputeArcRadians(float min_deg, float max_deg, float reference_deg,
                      float& out_start_rad, float& out_end_rad)
{
    if (min_deg < max_deg)
    {
        out_start_rad = (min_deg - 90.0f - reference_deg) * float_PI / 180.0f;
        out_end_rad   = (max_deg - 90.0f - reference_deg) * float_PI / 180.0f;
    }
    else
    {
        out_end_rad   = (min_deg - 90.0f - reference_deg) * float_PI / 180.0f;
        out_start_rad = (max_deg + 360.0f - 90.0f - reference_deg) * float_PI / 180.0f;
    }
}
*/

// ---------------------------------------------------------------------------------------

void DETAILED_TRACK_ALTERNATIVE::clear()
{
  TRACK_POINTS_DETAILED.clear();
}

void DETAILED_TRACK_ALTERNATIVE::store(DETAILED_TRACK_POINT New_Track_Point)
{
  if (TRACK_POINTS_DETAILED.size() == 0)
  {
    TRACK_POINTS_DETAILED.reserve(MAX_SIZE_DETAILED);
  }

  TRACK_POINTS_DETAILED.push_back(New_Track_Point);
}

#endif