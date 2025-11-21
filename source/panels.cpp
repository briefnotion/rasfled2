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

void PANEL::request(unsigned long Time, int Linger_Time, string Description)
{
  REQUESTED = true;
  TIMER.set(Time, Linger_Time);
  EXTRA.set_value(Description);
}

void PANEL::requested(unsigned long Time, bool &Requested)
{
  if (REQUESTED == true)
  {
    if (TIMER.is_ready(Time))
    {
      REQUESTED = false;
      EXTRA.set_value("");
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

void SCREEN4_PANEL_CONTROL::set_backup_camera()
{
  PANELS.MAIN_DISPLAY_SCREEN = 7;
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
  //PANELS.ADSB_DISPLAY_TABLE = false;
  //PANELS.ADSB_DISPLAY_MAP = true;
  
  PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 1);
}

void SCREEN4_PANEL_CONTROL::set_adsb_map_max_distance()
{
  PANELS.MAIN_DISPLAY_SCREEN = 2;
  //PANELS.ADSB_DISPLAY_TABLE = false;
  //PANELS.ADSB_DISPLAY_MAP = true;
  
  PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 2);
}

// ---
void SCREEN4_PANEL_CONTROL::color_start(unsigned long Time)
{
  COLOR_SELECT.init_and_set_intensity(Time, INTENSITY.value());
}

void SCREEN4_PANEL_CONTROL::color_set_initial_values(bool Automatic_On_or_Off, float Manual_Value, float Automatic_value)
{
  INTENSITY.set_initial_values(Automatic_On_or_Off, Manual_Value, Automatic_value);
}

void SCREEN4_PANEL_CONTROL::color_set_manual_intensity(unsigned long Time, float Intensity)
{
  if (INTENSITY.set_manual_value(Intensity))
  {
    COLOR_SELECT.init_and_set_intensity(Time, Intensity);
  }
}

void SCREEN4_PANEL_CONTROL::color_set_automatic_intensity(unsigned long Time, float Intensity)
{
  if (INTENSITY.set_automatic_value(Intensity))
  {
    COLOR_SELECT.init_and_set_intensity(Time, Intensity);
  }
}

void SCREEN4_PANEL_CONTROL::color_set_automatic(unsigned long Time, bool On_or_Off)
{
  if (INTENSITY.set_automatic(On_or_Off))
  {
    COLOR_SELECT.init_and_set_intensity(Time, INTENSITY.value());
  }
}

float SCREEN4_PANEL_CONTROL::color_current_intensity()
{
  return INTENSITY.value();
}

bool SCREEN4_PANEL_CONTROL::color_automatic_value()
{
  return INTENSITY.automatic_value();
}

int SCREEN4_PANEL_CONTROL::autonomous_state()
{
  return ATONOMOUS;
}

void SCREEN4_PANEL_CONTROL::autonomous_on()
{
  PANELS_BACKUP = PANELS;
  ATONOMOUS = 1;
}

void SCREEN4_PANEL_CONTROL::autonomous_off()
{
  if (ATONOMOUS == 2)
  {
    PANELS = PANELS_BACKUP;
  }
  ATONOMOUS = 0;
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

    CAMERA_BACKUP.requested(Time, panel_requested);

    //ADSB_MAP.requested(Time, panel_requested);
    ADSB_MAP_MIN_DISTANCE.requested(Time, panel_requested);
    ADSB_MAP_MAX_DISTANCE.requested(Time, panel_requested);

    // If the panels are requested, set the autonomous state and remember the panels.
    if (panel_requested)
    {
      if (ATONOMOUS == 1)
      {
        ATONOMOUS = 2;
        PANELS_BACKUP = PANELS;
      }
    }
    else
    {
      if (ATONOMOUS == 2)
      {
        ATONOMOUS = 1;
        PANELS = PANELS_BACKUP;
      }
    }
    // If the panels are requested, call the panels.
    // Start with the least important first so that they can be overlaped and 
    //  the most important or least likely panels will be prominant.

    EXTRA = "";

    if (panel_requested)
    {

      if (AUTO_TEMPERATURE.display())
      {
        set_auto_temperature();
        EXTRA = AUTO_TEMPERATURE.EXTRA.value() + EXTRA;
      }

      if (AUTO_PRESSURE.display())
      {
        set_auto_pressure();
        EXTRA = AUTO_PRESSURE.EXTRA.value() + EXTRA;
      }

      if (AUTO_ACCELERATION.display())
      {
        set_auto_acceleration();
        EXTRA = AUTO_ACCELERATION.EXTRA.value() + EXTRA;
      }

      if (ADSB_MAP_MIN_DISTANCE.display())
      {
        set_adsb_map_min_distance();
        EXTRA = ADSB_MAP_MIN_DISTANCE.EXTRA.value() + EXTRA;
      }

      if (ADSB_MAP_MAX_DISTANCE.display())
      {
        set_adsb_map_max_distance();
        EXTRA = ADSB_MAP_MAX_DISTANCE.EXTRA.value() + EXTRA;
      }

      if (AUTO_MALFUNCTION.display())
      {
        set_auto_malfunction();
        EXTRA = AUTO_MALFUNCTION.EXTRA.value() + EXTRA;
      }

      if (CAMERA_BACKUP.display())
      {
        set_backup_camera();
        EXTRA = CAMERA_BACKUP.EXTRA.value() + EXTRA;
      }

    }

  }

  // clear all flags
  FLAG_AUTO_ACCELERATION_DECELERATION = false;
  
}

// ---------------------------------------------------------------------------------------


#endif