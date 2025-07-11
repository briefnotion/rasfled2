// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: fled_time.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef FLED_TIME_H
#define FLED_TIME_H

// Standard Header Files
#include <chrono>
#include <ctime>
#include <cmath>
#include <unistd.h>

#include "helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------

class EFFICIANTCY_TIMER_2
// Measures time passed between calls. 
{
  private:
  double LAST_ASKED_TIME = 0;
  double TIMER_STARTED   = 0;

  double START_TO_START_TIME = 0;

  public:

  double start_to_start_time();
  // Measure full round trip time.

  void start_timer(double dblCurrent_Time);
  // Start the timer (stopwatch) by setting its the stopwatch time.
  //  The timer is a simple and can be considered always active. 

  double elapsed_timer_time(double dblCurrent_Time);
  //  Returns the amount of time passed since the reset. 

  double elapsed_time(double dblCurrent_Time);
  // Measures the amount of time elasped since the previous time the function was 
  //  called, then returns the value, then resets for next time. 

  double simple_elapsed_time(double dblCurrent_Time);
  // Only returns the calc of current time - start timer time.
};

// ---------------------------------------------------------------------------------------
class STAT_DATA_DOUBLE
// Provide minor min max stats over time of resets.
// If real stats ever necessary, then weighted average 
//  routine needed.
{
  private:

  EFFICIANTCY_TIMER_2 TIMER;

  bool set = true;

  double DATA  = 0;
  double MIN   = 0;
  double MAX   = 0;

  void set_data(double data);

  public:

  double get_data();

  double get_min();

  double get_max();

  void reset_minmax();

  void start_timer(double dblCurrent_Time);

  void end_timer(double dblCurrent_Time);

  double start_to_start_time();
};

// -------------------------------------------------------------------------------------
// Time Variable
class FLED_TIME_VAR
{
  private:
  // time starts at jan 1 1970
  unsigned long SECONDS = 0;
  int MICRO_SECONDS = 0;   // 1*10^-6 Seconds

  time_t THE_TIME;
  tm *PTM;
  bool TIME_UPDATED = false;

  void update_time();

  public:
  void put_seconds(unsigned long Seconds);
  void put_deciseconds(int Deciseconds);
  void put_miliseconds(int Miliseconds);

  unsigned long get_seconds();

  int get_deciseconds();

  int get_miliseconds();

  int get_year();

  int get_month();

  int get_day();

  int get_hour();

  int get_minute();
  
  int get_second();
};


// -------------------------------------------------------------------------------------
// Keeps track of timing variables
//  1ms accuracy is good enough for anything outside of the sciences.
//  However, watching my blinkers get out of sync after 20 seconds threads 
//  on the side of irritating.

class FLED_TIME
{
  private:
  std::chrono::time_point<std::chrono::steady_clock> TIME_START;

  double ERROR_TOLERANCE = 1.0;
  double ERROR = 0.0;
  double OLD_NOW_TIME = 0.0;
  bool ERROR_EXIST = false;

  double CURRENT_FRAME_TIME = 0.0;

  bool test = false;

  // How long sleep will occure
  int SLEEP_TIME = 1000;

  public:
  STAT_DATA_DOUBLE COMPUTETIME;   // Loop time spent while only proceessing.
  STAT_DATA_DOUBLE CYCLETIME;     // Amount of time to complete an entire cycle.
  STAT_DATA_DOUBLE PREVSLEEPTIME; // Stored value returned on prev sleep cycle.

  public:
  void request_ready_time(unsigned long Ready_Time);
 
  double error();

  void clear_error();

  unsigned long now();

  void create();

  bool setframetime();

  double tmeFrameElapse();

  unsigned long current_frame_time();

  void sleep_till_next_frame();
};

// ---------------------------------------------------------------------------------------
class TIMED_IS_READY
// Class to manage conditions of when something needs to be ran.

/*
Example Usage:

void PANEL::request(unsigned long Time)
{
  REQUESTED = true;

  TIMER.set(Time, 500);
}

bool PANEL::requested(unsigned long Time)
{
  if  (REQUESTED == false)
  {
    return false;
  }
  else
  {
    if (TIMER.is_ready(Time))
    {
      REQUESTED = false;
      return false;
    }
    else
    {
      return true;
    }
  }
}

or a similer iteration

  if (CALIBRATION_TIMER.is_ready(tmeFrame_Time))
  {
    CALIBRATION_TIMER.set(tmeFrame_Time, CALIBRATION_DELAY);
    ...
  }

*/

{
  private:
  unsigned long TRIGGERED_TIME  = 0;  //  Most recent time the ready was activated
  unsigned long LAST_ASKED_TIME = 0;  //  Most recent time the variable was asked if was ready.
  unsigned long READY_TIME      = 0;  //  Calculated time of when variable will be ready.
  int           INTREVAL        = 0;  //  Time in miliseconds between ready.

  public:

  bool is_set();

  void set(unsigned long current_time, int delay);
  // Prep the TIMED_IS_READY varable
  // delay is measured in ms.

  void set(int delay);
  // Prep the TIMED_IS_READY varable
  //  If current time isn't available
  // delay is measured in ms.

  unsigned long get_ready_time();
  // Return the time value of when the variable will be ready. 

  bool is_ready(unsigned long current_time);
  // Check to see if enough time has passed.
  //  Returns true if interval time has passed.
  //    Resets timer if returned true.
  //  Returns false if time has not elapsed.
  //    Stores last asked time.
  
  bool is_ready_no_reset(unsigned long current_time);
  // Check to see if enough time has passed.
  //  Returns true if interval time has passed.
  //    Does not resets timer if returned true.
  //  Returns false if time has not elapsed.
  //    Stores last asked time.
  
  void set_earliest_ready_time(unsigned long current_time);
  // Manually set the ready time of the variable. 
  // Does not change time if time to set is later than the already set time.
};

// ---------------------------------------------------------------------------------------

//  PHASE OUT
//  (with what?  Did I write a replacement?)
//    Maybe TIMED_IS_READY?

class TIMED_PING
// Class to manage conditions of when something needs to be ran.
//  Simplified version of is_ready_timer.

// Possible (untested) example:
//  if (test.ping_down((unsigned long)(Time / 1000)) == false)
//  {
//    test.ping_up((unsigned long)(Time / 1000), 10000);
//    ... code ...
//  }

{
  private:
  unsigned long START_TIME      = 0;  //  Calculated time of when variable will be ready.
  unsigned long READY_TIME      = 0;  //  Calculated time of when variable will be ready.
  bool          ENABLED         = false;  // Determine if the timer is active.
  bool          BLIP_POS        = false;
  
  public:

  bool enabled();
  // Retruns enabled;
  // DO NOT CALL UNLESS PASSIVELY PROBING, PING DOWN RESULTS THE SAME.

  unsigned long start_time();
  // Returns Start Time.

  void ping_up(unsigned long current_time, int delay);
  // Like, sending a ping signal out.
  // Start the timer for the event that needs to be triggered.
  // If already running, resets the trigger time.

  bool ping_down(unsigned long current_time);
  // Like, did the sent signal get back yet.
  // Check for the event triggered time.
  // Returns true if interval time has not passed, and ping is enabled.
  //  Disables after returning false.
  // Returns false on all other conditions.

  bool blip_visible(unsigned long current_time);
  // Check for the event triggered time.
  // Returns true if interval time has not passed, and ping is enabled.
  //  Enabled remains true.
  // Returns false on all other conditions.
  
  bool blip_moved(unsigned long current_time);
  // Check to see if return ping has changed from false to true or true to false.
  // Returns true if changed.
  // Returns false if not changed or not enabled.
};

// -------------------------------------------------------------------------------------
// Functions

#endif