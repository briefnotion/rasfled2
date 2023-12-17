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
  DISPLAY = true;
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
      WARNING = false;
      ret_description_request = true;
    }  
  }
  else if (Raise_Alert)
  {
    WARNING = false;
    ret_description_request = true;

    if (ACTIVE == false)
    {
      ID = Id;
      ACTIVE = true;
      ACKNOWLEGED = false;
      DISPLAY = true;
    }
  }
  else if (ACTIVE)
  {
    WARNING = true;
    ret_description_request = true;
  }

  return ret_description_request;
}

bool ALERT_2_TYPE_MONITOR::warning()
{
  return WARNING;
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

void ALERT_2_TYPE_MONITOR::set_display_on()
{
  DISPLAY = true;
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

int ALERT_SYSTEM_2::alert_count()
{
  return (ALERTS.size() + ALERTS_RESERVE_COUNT);
}

void ALERT_SYSTEM_2::add_generic_alert(string Text)
{
  ALERT_2_TYPE_MONITOR tmp_alert;

  tmp_alert.alert_no_condition(LATEST_ID, Text);

  ALERTS.push_back(tmp_alert);

  LATEST_ID++;

  SOUND_SYSTEM.play_test();

  CHANGED = true;
}

void ALERT_SYSTEM_2::alert_list_clean()
{
  if (ALERTS.size() > 0)
  {
    int alert_num = 0;
    while (alert_num < (int)ALERTS.size())
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

  ALERTS_RESERVE_COUNT = 0;
  for (int res_alert_pos = 0; res_alert_pos < RESERVE_ALERT_LIST_SIZE; res_alert_pos++)
  {
    if (ALERTS_RESERVE[res_alert_pos].active())
    {
      ALERTS_RESERVE_COUNT++;
    }
  }
}

void ALERT_SYSTEM_2::display_active_alerts()
{
  for (int alert_pos = 0; alert_pos < RESERVE_ALERT_LIST_SIZE; alert_pos++)
  {
    if (ALERTS_RESERVE[alert_pos].active())
    {
      ALERTS_RESERVE[alert_pos].set_display_on();
    }
  }

  for (int alert_pos = 0; alert_pos < (int)ALERTS.size(); alert_pos++)
  {
    ALERTS_RESERVE[alert_pos].set_display_on();
  }
}



#endif