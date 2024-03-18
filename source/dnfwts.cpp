// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: dnfwts.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef DNFWTS_CPP
#define DNFWTS_CPP

#include "dnfwts.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

bool DNFWTS_::check_for_change()
{
  if (CHANGED)
  {
    CHANGED = false;
    return true;
  }
  else
  {
    return false;
  }
}

void DNFWTS_::turn_on(unsigned long Time)
{
  TIMER_DNFWTS.ping_up(Time, 5999);
  ON = true;
}

void DNFWTS_::trun_off()
{
  CHANGED = true;
  ACTIVE = false;
  ON = false;
}

bool DNFWTS_::active()
{
  return ACTIVE;
}

bool DNFWTS_::active_update(unsigned long Time)
{
  if (ACTIVE)
  {
    return true;
  }
  else
  {
    if (ON)
    {
      if (TIMER_DNFWTS.ping_down(Time) == false)
      {
        CHANGED = true;
        ACTIVE = true;
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
}

#endif