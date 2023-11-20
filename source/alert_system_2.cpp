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

void ALERT_2_TYPE_MONITOR::alert_no_condition(int Id, string Alert_Text)
{
  ID = Id;
  ALERT_TEXT = Alert_Text;
}

bool ALERT_2_TYPE_MONITOR::alert_condition(int Id, bool Raise_Alert, bool Clear_Alert)
{
  bool ret_description_request = false;

  if (Clear_Alert)
  {
    if (ACTIVE)
    {
      ID = Id;
      ACTIVE = false;
      ret_description_request = true;
    }  
  }

  if (Raise_Alert)
  {
    if (ACTIVE == false)
    {
      ID = Id;
      ACTIVE = true;
      ACKNOWLEGED = false;
      DISPLAY = true;
      ret_description_request = true;
    }
    else
    {
      ret_description_request = true;
    }
  }

  return ret_description_request;
}

bool ALERT_2_TYPE_MONITOR::active()
{
  return ACTIVE;
}

bool ALERT_2_TYPE_MONITOR::display()
{
  return DISPLAY;
}

int ALERT_2_TYPE_MONITOR::id()
{
  return ID;
}

string ALERT_2_TYPE_MONITOR::alert_text()
{
  return ALERT_TEXT;
}

void ALERT_2_TYPE_MONITOR::update_alert_text(string Text)
{
  ALERT_TEXT = Text;
}

void ALERT_2_TYPE_MONITOR::acknowlege()
{
  ACKNOWLEGED = true;
  DISPLAY = false;
}

bool ALERT_2_TYPE_MONITOR::is_clear()
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

void ALERT_2_TYPE_MONITOR::clear()
{
  ACTIVE = false;
}

bool ALERT_SYSTEM_2::changed()
{
  return CHANGED;
}

void ALERT_SYSTEM_2::add_generic_alert(string Text)
{
  ALERT_2_TYPE_MONITOR tmp_alert;

  tmp_alert.alert_no_condition(LATEST_ID, Text);

  ALERTS.push_back(tmp_alert);

  LATEST_ID++;

  CHANGED = true;
}

void ALERT_SYSTEM_2::alert_list_clean()
{
  if (ALERTS.size() > 0)
  {
    int alert_num = 0;
    while (alert_num < ALERTS.size())
    {
      if (ALERTS[alert_num].is_clear())
      {
        ALERTS.erase(ALERTS.begin() + alert_num);
      }
      else
      {
        alert_num++;
      }
    }
  }
}

#endif