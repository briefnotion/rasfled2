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