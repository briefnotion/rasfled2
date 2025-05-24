// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: panels.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef PANELS_CPP
#define PANELS_CPP

#include "panels.h"

using namespace std;

// ---------------------------------------------------------------------------------------

void PANEL::request(unsigned long Time, int Linger_Time)
{
  REQUESTED = true;

  TIMER.set(Time, Linger_Time);
}

void PANEL::requested(unsigned long Time, bool &Requested)
{
  if (REQUESTED == true)
  {
    if (TIMER.is_ready(Time))
    {
      REQUESTED = false;
    }
    else
    {
      Requested = true;
      REQUESTED = true;
    }
  }
}

bool PANEL::display()
{
  return REQUESTED;
}

// ---------------------------------------------------------------------------------------

void SCREEN4_PANEL_CONTROL::set_auto_temperature()
{
  PANELS.MAIN_DISPLAY_SCREEN = 1;
  PANELS.AUTOMOBILE_DISPLAY_NOVA = false;
  PANELS.AUTOMOBILE_DISPLAY_MID_BOTTOM = 2;
}

void SCREEN4_PANEL_CONTROL::set_auto_malfunction()
{
  PANELS.MAIN_DISPLAY_SCREEN = 1;
  PANELS.AUTOMOBILE_DISPLAY_NOVA = false;
  PANELS.AUTOMOBILE_DISPLAY_MID_BOTTOM = 0;
}

void SCREEN4_PANEL_CONTROL::set_auto_pressure()
{
  PANELS.MAIN_DISPLAY_SCREEN = 1;
  PANELS.AUTOMOBILE_DISPLAY_NOVA = false;
  PANELS.AUTOMOBILE_DISPLAY_MID_BOTTOM = 3;
}

void SCREEN4_PANEL_CONTROL::set_auto_acceleration()
{
  PANELS.MAIN_DISPLAY_SCREEN = 1;
  PANELS.AUTOMOBILE_DISPLAY_NOVA = false;
  PANELS.AUTOMOBILE_DISPLAY_MID_TOP = 1;
}

/*
void SCREEN4_PANEL_CONTROL::set_adsb_map()
{
  PANELS.MAIN_DISPLAY_SCREEN = 2;
  PANELS.ADSB_DISPLAY_TABLE = false;
  PANELS.ADSB_DISPLAY_MAP = true;
}
*/

void SCREEN4_PANEL_CONTROL::set_adsb_map_min_distance()
{
  PANELS.MAIN_DISPLAY_SCREEN = 2;
  PANELS.ADSB_DISPLAY_TABLE = false;
  PANELS.ADSB_DISPLAY_MAP = true;
  
  PANELS.ADSB_RANGE_INDICATOR_ZOOM_MIN_MAX = 1;
}

void SCREEN4_PANEL_CONTROL::set_adsb_map_max_distance()
{
  PANELS.MAIN_DISPLAY_SCREEN = 2;
  PANELS.ADSB_DISPLAY_TABLE = false;
  PANELS.ADSB_DISPLAY_MAP = true;
  
  PANELS.ADSB_RANGE_INDICATOR_ZOOM_MIN_MAX = 2;
}

// --- 

int SCREEN4_PANEL_CONTROL::autonomous_state()
{
  return ATONOMOUS;
}

void SCREEN4_PANEL_CONTROL::autonomous_on()
{
  ATONOMOUS = 1;
  PANELS_OFF = PANELS;
}

void SCREEN4_PANEL_CONTROL::autonomous_off()
{
  ATONOMOUS = 0;
  PANELS = PANELS_ON;
}

void SCREEN4_PANEL_CONTROL::activate(unsigned long Time)
{
  bool panel_requested = false;

  // If Autonomous is on, check if the panels are requested.
  if (ATONOMOUS > 0)
  {
    // Check if the panels are requested.
    AUTO_TEMPERATURE.requested(Time, panel_requested);
    AUTO_MALFUNCTION.requested(Time, panel_requested);
    AUTO_PRESSURE.requested(Time, panel_requested);
    AUTO_ACCELERATION.requested(Time, panel_requested);

    //ADSB_MAP.requested(Time, panel_requested);
    ADSB_MAP_MIN_DISTANCE.requested(Time, panel_requested);
    ADSB_MAP_MAX_DISTANCE.requested(Time, panel_requested);

    // If the panels are requested, set the autonomous state and remember the panels.
    if (panel_requested)
    {
      if (ATONOMOUS == 1)
      {
        ATONOMOUS = 2;
        PANELS_OFF = PANELS;
      }
    }
    else
    {
      if (ATONOMOUS == 2)
      {
        ATONOMOUS = 1;
        PANELS = PANELS_OFF;
      }
    }

    // If the panels are requested, call the panels.
    // Start with the least important first so that they can overlap and 
    //  the most important one will be on prominant.
    if (panel_requested)
    {

      if (ADSB_MAP_MIN_DISTANCE.display())
      {
        set_adsb_map_min_distance();
      }

      if (ADSB_MAP_MAX_DISTANCE.display())
      {
        set_adsb_map_max_distance();
      }

      if (AUTO_PRESSURE.display())
      {
        set_auto_pressure();
      }

      if (AUTO_TEMPERATURE.display())
      {
        set_auto_temperature();
      }

      if (AUTO_ACCELERATION.display())
      {
        set_auto_acceleration();
      }

      if (AUTO_MALFUNCTION.display())
      {
        set_auto_malfunction();
      }

    }

  }

  // clear all flags
  FLAG_AUTO_ACCELERATION_DECELERATION = false;
  
}

// ---------------------------------------------------------------------------------------


#endif