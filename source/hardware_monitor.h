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

#ifndef HARDWARE_MONITOR_H
#define HARDWARE_MONITOR_H

#include <string>

#include "definitions.h"

using namespace std;

// ---------------------------------------------------------------------------------------
// Hardware Monitor Class
class switch_map
// A simple switch to pin reference map
{
  // Referenced only by position in list.
  public:

  int     pin = 0;          // Hardware PIN Number
  string  SWITCH_NAME = ""; // Switch Name
  bool    value = false;    // 
};

class hardware_monitor
// Create a hardware switch or button varable type.
{
  public:
  // External
  bool booVALUE = false;
  unsigned long tmeTOGGLEDTIME = 0;
  bool ISHARDWARE = false;

  private:
  // Internal
  unsigned long tmeCHANGEDETECTEDTIME = 0;
  bool booPREVCHANGEDETECTED = false;
  unsigned int tmeLEEWAY = 0;
  bool booFIRSTRUN = true;

  public:
  void set(bool booValue, unsigned long tmeCheckTime, int tmeLeeWay, bool isHardware);

  bool changed(bool booValue, unsigned long tmeCheckTime);

  void read(bool booValue, unsigned long tmeCheckTime);
};

#endif // HARDWARE_MONITOR_H