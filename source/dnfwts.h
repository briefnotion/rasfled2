// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: dnfwts.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef DNFWTS_H
#define DNFWTS_H

//#include <stdio.h>
//#include <string>
//#include <deque>

// RASFled related header files
//#include "definitions.h"
//#include "helper.h"
//#include "commands.h"
//#include "hardware_status.h"
#include "fled_time.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

// -------------------------------------------------------------------------------------
//  DNFWTS Class
class DNFWTS_
{
  private:

  bool ACTIVE = false;
  bool ON = false;
  bool CHANGED = false;

  TIMED_PING TIMER_DNFWTS;

  public:

  bool check_for_change();
  void turn_on(unsigned long Time);
  void trun_off();
  bool active();
  bool active_update(unsigned long Time);

};



#endif