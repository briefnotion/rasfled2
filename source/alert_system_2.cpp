// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: alert_system_2.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef ALERT_SYSTEM_2_CPP
#define ALERT_SYSTEM_2_CPP

#include "alert_system_2.h"

using namespace std;

// -------------------------------------------------------------------------------------
//  Alert Class

void ALERT_2_TYPE_GENERIC::acknowlege()
{
  ACKNOWLEGED = true;
  DISPLAY = false;
}

bool ALERT_2_TYPE_GENERIC::is_clear()
{
  if (ACKNOWLEGED == true && ACTIVE == false && DISPLAY == false)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool ALERT_SYSTEM_2::changed()
{
  return CHANGED;
}

void ALERT_SYSTEM_2::add_generic_alert(string Text)
{
  ALERT_2_TYPE_GENERIC tmp_alert;

  tmp_alert.TEXT = Text;
  tmp_alert.ID = LATEST_ID;

  GENERIC_ALERTS.push_back(tmp_alert);

  LATEST_ID++;

  CHANGED = true;
}

void ALERT_SYSTEM_2::alert_list_clean()
{
  if (GENERIC_ALERTS.size() > 0)
  {
    int alert_num = 0;
    while (alert_num < GENERIC_ALERTS.size())
    {
      if (GENERIC_ALERTS[alert_num].is_clear())
      {
        GENERIC_ALERTS.erase(GENERIC_ALERTS.begin() + alert_num);
      }
      else
      {
        alert_num++;
      }
    }
  }
}

#endif