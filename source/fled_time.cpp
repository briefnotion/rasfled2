// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: fled_time.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef FLED_TIME_CPP
#define FLED_TIME_CPP

#include "fled_time.h"

using namespace std;

// ---------------------------------------------------------------------------------------


void STAT_DATA_DOUBLE::set_data(double data)
// Provide minor min max stats over time of resets.
// I real stats ever necessary, then weighted average 
//  routine needed.
{
  DATA = data;
  
  if (set == false)
  {
    set = true;
    MIN = DATA;
    MAX = DATA;
  }
  else if (data < MIN)
  {
    MIN = data;
  }
  else if (data > MAX)
  {
    MAX = data;
  }
}

double STAT_DATA_DOUBLE::get_data()
{
  return DATA;
}

  double STAT_DATA_DOUBLE::get_min()
{
  return MIN;
}

  double STAT_DATA_DOUBLE::get_max()
{
  return MAX;
}

void STAT_DATA_DOUBLE::reset_minmax()
{
  set = false;
  MIN = DATA;
  MAX = DATA;
}

// ---------------------------------------------------------------------------------------

void EFFICIANTCY_TIMER::start_timer(double dblCurrent_Time)
{
  TIMER_STARTED = dblCurrent_Time;
}

double EFFICIANTCY_TIMER::elapsed_timer_time(double dblCurrent_Time)
{
  return dblCurrent_Time - TIMER_STARTED;
}

double EFFICIANTCY_TIMER::elapsed_time(double dblCurrent_Time)
{
  double time_elapsed = dblCurrent_Time - LAST_ASKED_TIME;
  LAST_ASKED_TIME = dblCurrent_Time;
  return time_elapsed;
}

double EFFICIANTCY_TIMER::simple_elapsed_time(double dblCurrent_Time)
{
  return dblCurrent_Time - TIMER_STARTED;
}

// -------------------------------------------------------------------------------------
// Time Variable

void FLED_TIME_VAR::update_time()
{
  THE_TIME = (time_t)SECONDS;
  PTM = localtime(&THE_TIME);
  TIME_UPDATED = true;
}

void FLED_TIME_VAR::put_seconds(unsigned long Seconds)
{
  SECONDS = Seconds;
  TIME_UPDATED = false;
}

void FLED_TIME_VAR::put_deciseconds(int Deciseconds)
{
  MICRO_SECONDS = Deciseconds *100000;
  TIME_UPDATED = false;
}

void FLED_TIME_VAR::put_miliseconds(int Miliseconds)
{
  MICRO_SECONDS = Miliseconds *1000;
  TIME_UPDATED = false;
}

unsigned long FLED_TIME_VAR::get_seconds()
{
  return SECONDS;
}

int FLED_TIME_VAR::get_deciseconds()
{
  return MICRO_SECONDS / 100000;
}

int FLED_TIME_VAR::get_miliseconds()
{
  return MICRO_SECONDS / 1000;
}

int FLED_TIME_VAR::get_year()
{

  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (1900 + PTM->tm_year);
}

int FLED_TIME_VAR::get_month()
{

  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (PTM->tm_mon +1);
}

int FLED_TIME_VAR::get_day()
{
  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (PTM->tm_mday);
}

int FLED_TIME_VAR::get_hour()
{
  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (PTM->tm_hour);
}

int FLED_TIME_VAR::get_minute()
{
  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (PTM->tm_min);
}

int FLED_TIME_VAR::get_second()
{
  if (TIME_UPDATED == false)
  {
    update_time();
  }

  return (PTM->tm_sec);
}

// -------------------------------------------------------------------------------------
// Keeps track of timing variables
double FLED_TIME::store_sleep_time(double tmeSleep)
// Pass through variable
// Stores the Sleep time to be displayed in diag, then returns the same value.
{
  PREVSLEEPTIME.set_data(tmeSleep);
  return tmeSleep;
}

double FLED_TIME::get_sleep_time(double Current_Time, unsigned long Wake_Time)
{
  // Return, in microseconds, the amount of time required to sleep.
  
  double sleeptime = 0;

  if(Current_Time < Wake_Time)
  {
    sleeptime = (unsigned long)Wake_Time - Current_Time;
  }

  return sleeptime;
}

void FLED_TIME::request_ready_time(unsigned long Ready_Time)
{
  if (Ready_Time < SLEEP_WAKE_TIME)
  {
    SLEEP_WAKE_TIME = Ready_Time;
  }
}

double FLED_TIME::error()
{
  return ERROR;
}

void FLED_TIME::clear_error()
{
  ERROR_EXIST = false;
}

unsigned long FLED_TIME::now()
{
  // Returns now time in milliseconds.
  // Should be Unsigned Long.
  std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
  std::chrono::duration<double>  dur = tmeNow - TIME_START;

  double nowtime = dur.count();

  // This error check routine and offset by error should prevent the program from locking up 
  //  while waiting on timing triggers, if the internet decides the system clock needs to change
  //  its time by a significant amount.  The problem is that there are things, such as the graphs,
  //  that, somehow, aren't fooled by this little hack.  Not sure why yet.

  // Check for error
  if ((abs(nowtime - OLD_NOW_TIME)) > ERROR_TOLERANCE)
  {
    ERROR = nowtime - OLD_NOW_TIME;
    ERROR_EXIST = true;
  }
  
  OLD_NOW_TIME = nowtime;
  nowtime = (nowtime - ERROR) * 1000.0 ;
  
  return (unsigned long)nowtime;
}

void FLED_TIME::create()
{
  // Initialize as Start of Program Time.
  TIME_START = std::chrono::system_clock::now();

  EFFIC_TIMER.start_timer(now());
}

bool FLED_TIME::setframetime()
{
  CURRENT_FRAME_TIME = now();

  return ERROR_EXIST;
}

double FLED_TIME::tmeFrameElapse()
{
  // Returns, in milliseconds, the amount of time passed since frame time.
  double elapsed = (double)now()- CURRENT_FRAME_TIME;

  return elapsed;
}

unsigned long FLED_TIME::current_frame_time()
{
  return (unsigned long)CURRENT_FRAME_TIME;
}

void FLED_TIME::sleep_till_next_frame()
{
  // Measure how much time has passed since the previous time the program was at 
  //  this point and store that value to be displayed in diag.
  CYCLETIME.set_data(EFFIC_TIMER.elapsed_time(now()));

  // Reset the the compute timer (stopwatch) and store the value before the program sleeps. 
  COMPUTETIME.set_data(EFFIC_TIMER.elapsed_timer_time(now()));

  // Sleep until the next frame.
  usleep ((int)(1000 * store_sleep_time(get_sleep_time(now(), SLEEP_WAKE_TIME))));
  
  // Reset to 1 second
  SLEEP_WAKE_TIME = 1000;

  // Start the the compute timer (stopwatch) before the program as the program wakes to measure the amount of 
  //  time the compute cycle is.
  EFFIC_TIMER.start_timer(now());
}
  
// ---------------------------------------------------------------------------------------

bool TIMED_IS_READY::is_set()
{
  if (TRIGGERED_TIME == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void TIMED_IS_READY::set(unsigned long current_time, int delay)
// Prep the TIMED_IS_READY varable
// delay is measured in ms.
{
  TRIGGERED_TIME  = current_time;
  INTREVAL        = delay;
}

void TIMED_IS_READY::set(int delay)
// Prep the TIMED_IS_READY varable
//  If current time isn't available
// delay is measured in ms.
{
  TRIGGERED_TIME  = 0;
  INTREVAL        = delay;
}

unsigned long TIMED_IS_READY::get_ready_time()
// Return the time value of when the variable will be ready. 
{
  return READY_TIME;
}

bool TIMED_IS_READY::is_ready(unsigned long current_time)
{
  // Check to see if enough time has passed.
  //  Returns true if interval time has passed.
  //    Resets timer if returned true.
  //  Returns false if time has not elapsed.
  //    Stores last asked time.
  if(current_time >= READY_TIME)
  {
    TRIGGERED_TIME = current_time;
    READY_TIME = current_time + INTREVAL;
    return true;
  }
  else
  {
    LAST_ASKED_TIME = current_time;
    return false;
  }
}

bool TIMED_IS_READY::is_ready_no_reset(unsigned long current_time)
{
  // Check to see if enough time has passed.
  //  Returns true if interval time has passed.
  //    Does not resets timer if returned true.
  //  Returns false if time has not elapsed.
  //    Stores last asked time.
  if(current_time >= READY_TIME)
  {
    LAST_ASKED_TIME = current_time;
    return true;
  }
  else
  {
    LAST_ASKED_TIME = current_time;
    return false;
  }
}

void TIMED_IS_READY::set_earliest_ready_time(unsigned long current_time)
// Manually set the ready time of the variable. 
// Does not change time if time to set is later than the already set time.
{
  if (current_time < READY_TIME)
  {
    READY_TIME = current_time;
  }
}

// ---------------------------------------------------------------------------------------

bool TIMED_PING::enabled()
{
  return ENABLED;
}

unsigned long TIMED_PING::start_time()
{
  return START_TIME;
}

void TIMED_PING::ping_up(unsigned long current_time, int delay)
// Start the timer for the event that needs to be triggered.
{
  START_TIME      = current_time;
  READY_TIME      = current_time + delay;
  ENABLED         = true;
}

bool TIMED_PING::ping_down(unsigned long current_time)
{
  // Check for the event triggered time.
  // Returns true if interval time has not passed, and ping is enabled.
  //  Disables after returning false.
  // Returns false if not enabled or time is passed.
  if (ENABLED == true)
  {
    if (current_time < READY_TIME)
    {
      return true;
    }
    else
    {
      ENABLED = false;
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool TIMED_PING::blip_visible(unsigned long current_time)
{
  // Check for the event triggered time.
  // Returns true if interval time has not passed, and ping is enabled.
  // Returns false if not enabled or time is passed.
  if (ENABLED == true)
  {
    return (current_time < READY_TIME);
  }
  else
  {
    return false;
  }
}

bool TIMED_PING::blip_moved(unsigned long current_time)
// Check to see if return ping has changed from false to true or true to false.
// Returns true if changed.
// Returns false if not changed or not enabled.
{
  if (ENABLED == true)
  {
    bool new_blip_pos = blip_visible(current_time);

    if (new_blip_pos != BLIP_POS)
    {
      BLIP_POS = new_blip_pos;
      return true;
    }
    else
    {
      return false;
    }
  }
  else 
  {
    return false;
  }
}

// -------------------------------------------------------------------------------------
// Functions


#endif