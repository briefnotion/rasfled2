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

// -------------------------------------------------------------------------------------


#endif