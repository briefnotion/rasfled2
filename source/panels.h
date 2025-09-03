// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: panels.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef PANELS_H
#define PANELS_H

#include "fled_time.h"
#include "stringthings.h"
#include "screen4_helper.h"

// ---------------------------------------------------------------------------------------

class FLOAT_ON_OFF_AUTOMATIC
{
  private:
  float VALUE_MANUAL    = 0.0f;
  float VALUE_AUTOMATIC = 0.0f;
  bool  AUTOMATIC_ON    = false;

  public:
  void set_initial_values(bool Automatic_On_or_Off, float Manual_Value, float Automatic_value);
  bool set_manual_value(float Value);
  bool set_automatic_value(float Value);
  bool set_automatic(bool On_or_Off);
  bool automatic_value();
  float value();
};

// ---------------------------------------------------------------------------------------

class SCREEN4_PANELS
{
  public:

  //sdSysData.PANEL_CONTROL.

  // Main Screen
  int MAIN_DISPLAY_SCREEN = 0;
  // Screen:
  //  0 - Console
  //  1 - Automobile
  //  2 - ADSB
  //  3 - not coded
  //  4 - Logs
  //  5 - Dot Dot Dot



  // Automobile
  bool AUTOMOBILE_DISPLAY_NOVA = false;
  bool AUTOMOBILE_DISPLAY_NOVA_SCREEN = true;
    // false - nova screen 1
    // true  - nova screen 2

  int  AUTOMOBILE_NOVA_2_SELECTION = 0;
  // 0 - Drive
  // 1 - P & T
  // 2 - Indicators
  // 3 - Engine
  // 4 - Other


  int AUTOMOBILE_DISPLAY_MID_TOP = 0;
  // 0 - Speed slow degen
  // 1 - Power Curve

  int AUTOMOBILE_DISPLAY_MID_BOTTOM = 0; 
  // 0 - Large Horizontal Bars
  // 1 - Unused
  // 2 - Temp Graph
  // 3 - Power Graph



  // ADSB
  bool ADSB_DISPLAY_TABLE = false;
  bool ADSB_DISPLAY_MAP = true;

  int ADSB_RANGE_INDICATOR_ZOOM_MIN_MAX = 0;
  // 0  - OFF
  // 1  - MIN (On)
  // 2  - MAX (On)
  // 3  - DRV (On)

  float RANGE_RADIUS_CIRCLE_POINT_SIZE = 136.800003f;
  double CURRENT_RESOLUTION = -1.0;
};


class PANEL
{
  private:
  bool REQUESTED = false;

  TIMED_IS_READY TIMER;

  public:

  SEARCH_STRING EXTRA;

  void request(unsigned long Time, int Linger_Time, string Description);
  void requested(unsigned long Time, bool &Requested);
  bool display();
};


class SCREEN4_PANEL_CONTROL
{
  private:

  // AKA: Screen Brightness
  //float INTENSITY = 1.0f; // Not sure if this is the best place for intensity.
                          //  Maybe it should just reside with the COLOR_SELECT.
                          //  Except, its a panel thing

  FLOAT_ON_OFF_AUTOMATIC INTENSITY;

  int ATONOMOUS = 1;
  // 0 - Off
  // 1 - On
  // 2 - Active

  // Panel backups
  SCREEN4_PANELS PANELS_ON;

  // Set Panels
  void set_auto_temperature();
  void set_auto_malfunction();
  void set_auto_pressure();
  void set_auto_acceleration();
  //void set_adsb_map();
  void set_adsb_map_min_distance();
  void set_adsb_map_max_distance();

  public:
  // Colors
  COLOR_COMBOS    COLOR_SELECT;     // All Working Colors

  // Main working panel
  SCREEN4_PANELS  PANELS;

  // Panel Control variables from alert system coordinator
  PANEL AUTO_TEMPERATURE;
  PANEL AUTO_MALFUNCTION;
  PANEL AUTO_PRESSURE;
  PANEL AUTO_ACCELERATION;
  //PANEL ADSB_MAP;
  PANEL ADSB_MAP_MIN_DISTANCE;
  PANEL ADSB_MAP_MAX_DISTANCE;

  // Notation 
  string EXTRA;

  // Color Intensity
  void color_start(unsigned long Time);
  void color_set_initial_values(bool Automatic_On_or_Off, float Manual_Value, float Automatic_value);
  
  void color_set_manual_intensity(unsigned long Time, float Intensity);
  void color_set_automatic_intensity(unsigned long Time, float Intensity);
  void color_set_automatic(unsigned long Time, bool On_or_Off);
  
  float color_current_intensity();
  bool color_automatic_value();

  // Flags initiated from points other than alert system coordinator
  bool FLAG_AUTO_ACCELERATION_DECELERATION = false;

  // Autonomous Control
  int autonomous_state();
  void autonomous_on();
  void autonomous_off();

  // Activate the panels from alert system coordinator
  void activate(unsigned long Time);
};

// ---------------------------------------------------------------------------------------

#endif