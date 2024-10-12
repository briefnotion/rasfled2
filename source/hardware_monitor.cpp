// ****************************************HELPER_CPP***********************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: helper.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef HARDWARE_MONITOR_CPP
#define HARDWARE_MONITOR_CPP

#include "hardware_monitor.h"

using namespace std;

// ---------------------------------------------------------------------------------------
// Hardware Monitor Class

void hardware_monitor::set(bool booValue, unsigned long tmeCheckTime, int tmeLeeWay, bool isHardware)
// Prepare the switch.
{
  tmeCHANGEDETECTEDTIME = tmeCheckTime;
  tmeLEEWAY = tmeLeeWay;
  booVALUE = booValue;
  booPREVCHANGEDETECTED = false;
  ISHARDWARE = isHardware;
}

bool hardware_monitor::changed(bool booValue, unsigned long tmeCheckTime)
// Return true if the switch state change from on to off or off to on.
{
  unsigned long tmeTme = tmeCheckTime;

  // If the switch was just activated then run any set up its initial state and run
  //  any special routines.
  if (booFIRSTRUN == true)
  {
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
    tmeCHANGEDETECTEDTIME = tmeTme;
    booFIRSTRUN = false;

    if (BOOTEST == false)
    {
      return booValue;  // Comment this line out when testing
      //return false;   // Comment this line out when not testing
    }
    else
    {
      //return booValue;// Comment this line out when testing
      return false;     // Comment this line out when not testing
    }
  }
  else if (booVALUE == booValue)
  {
    booPREVCHANGEDETECTED = false;
    return false;
  }
  else if (booPREVCHANGEDETECTED == false)
  {
    tmeCHANGEDETECTEDTIME = tmeTme;
    booPREVCHANGEDETECTED = true;
    return  false;
  }
  // Only report change of status when Leeway time is passed.  This is a essentially a
  //  debouncer.
  else if (tmeTme < (tmeCHANGEDETECTEDTIME + tmeLEEWAY))
  {
    return false;
  }
  else
  {
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
    tmeCHANGEDETECTEDTIME = tmeTme;
    tmeTOGGLEDTIME = tmeTme;
    return true;
  }
}

void hardware_monitor::read(bool booValue, unsigned long tmeCheckTime)
{
  changed(booValue, tmeCheckTime);
}

#endif // HARDWARE_MONITOR_H