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
  DNFWTS_TEXT_1.set_value("DO \nNOT FUCK \nWITH THE SYSTEM");

  //string line_1 = "ABCD EFGH IJKL MNOP QRST UVWX YZCD EFGH I\n";
  string line_1 = "WARNING: Due to malfuction or override, \n";
  //string line_2 = "    ABCD EFGH IJKL MNOP QRST UVWX YZC\n";
  string line_2 = "    visual protection service has been\n";
  //string line_3 = "        ABCD EFGH IJKL MNOP QRSTu\n";
  string line_3 = "               disabled.\n";
  //string line_4 = "            ABCD EFGH IJKL MN\n";
  string line_4 = "\n";
  //string line_5 = "                ABCD EFGH\n";
  string line_5 = "\n";
  //string line_6 = "                    A\n\n\n\n\n\n\n";
  string line_6 = "\n\n\n\n\n\n\n";
  //string line_7 = "                ABCD EFGH\n";
  string line_7 = "                Please\n";
  //string line_8 = "            ABCD EFGH IJKL MN\n";
  string line_8 = "            be careful when\n";
  //string line_9 = "        ABCD EFGH IJKL MNOP QRSTu\n";
  string line_9 = "        entering and exiting the\n";
  //string line_10 = "    ABCD EFGH IJKL MNOP QRST UVWX YZC\n";
  string line_10 = "               vehicle.\n";
  //string line_11 = "ABCD EFGH IJKL MNOP QRST UVWX YZCD EFGH I\n";
  //string line_11 = "ABCD EFGH IJKL MNOP QRST UVWX YZCD EFGH I\n";

  string lines = line_1 + line_2 + line_3 + line_4 + line_5 + line_6 + line_7 + line_8 + line_9 + line_10;

  DNFWTS_TEXT_2.set_value(lines);
}

void DNFWTS_::turn_off()
{
  CHANGED = true;
  ACTIVE = false;
  ON = false;
  DNFWTS_TEXT_1.set_value("");
  DNFWTS_TEXT_2.set_value("");
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