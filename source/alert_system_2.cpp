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

bool ALERT_2_TYPE_MONITOR::alert_condition(int Id, bool Raise_Alert, bool Clear_Alert, int &Changes)
{
  Changes = 0;

  if (Clear_Alert)
  {
    // Alert has cleared below waring levels
    if (ACTIVE)
    {
      // Alert was previously raised. 
      //  Clear all status and leave display as is.
      ID = Id;
      ACTIVE = false;
      WARNING = false;
      
      // Play clear sound
      Changes = -1;
    }  
  }
  else if (Raise_Alert)
  {
    // Alert is at active status
    //  Remove warnings status if set.
    if (ACTIVE == false || WARNING == true)
    {
      // Alert was previously at warning or cleared.
      // Set alert active status.
      ID = Id;
      ACTIVE = true;
      ACKNOWLEGED = false;
      DISPLAY = true;
      WARNING = false;
      Changes = 2;
    }
  }
  else if (ACTIVE)
  {
    // Alert is in between cleared and raised alert status
    // Set as warning.

    if (WARNING == false)
    {
      // Newly waring
      WARNING = true;
      
      Changes = 1;
    }
  }

  return DISPLAY;
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

// -------------------------------------------------------------------------------------

void ALERT_SYSTEM_2::set_mute(bool Mute)
{
  MUTE = Mute;
}

bool ALERT_SYSTEM_2::changed()
{
  return CHANGED;
}

// reserve alerts
void ALERT_SYSTEM_2::res_alert_no_condition(COMMAND_THREAD &Thread, unsigned long current_time_frame, int Id, string Alert_Text)
{
  ALERTS_RESERVE[Id].alert_no_condition(Id, Alert_Text);
  SOUND_SYSTEM.play_c6(Thread, current_time_frame, MUTE);
}

bool ALERT_SYSTEM_2::res_alert_condition(COMMAND_THREAD &Thread, unsigned long current_time_frame, int Id, bool Raise_Alert, bool Clear_Alert)
{
  int alert_change = 0;
  bool ret_description_request = ALERTS_RESERVE[Id].alert_condition(Id, Raise_Alert, Clear_Alert, alert_change);


  if (alert_change  == 2)       // Alert
  {
    SOUND_SYSTEM.play_alert_level_3(Thread, current_time_frame, MUTE);
  }
  else if (alert_change  == 1)  // Warning
  {
    SOUND_SYSTEM.play_alert_level_2(Thread, current_time_frame, MUTE);
  }
  else if (alert_change  == -1)  // Cleared
  {
    SOUND_SYSTEM.play_alert_level_1(Thread, current_time_frame, MUTE);
  }

  return ret_description_request;
}

void ALERT_SYSTEM_2::res_update_alert_text(int Id, string Text)
{
  ALERTS_RESERVE[Id].update_alert_text(Text);
}

bool ALERT_SYSTEM_2::res_active(int Id)
{
  return ALERTS_RESERVE[Id].active();
}

bool ALERT_SYSTEM_2::res_warning(int Id)
{
  return ALERTS_RESERVE[Id].warning();
}

bool ALERT_SYSTEM_2::res_display(int Id)
{
  return ALERTS_RESERVE[Id].display();
}

string ALERT_SYSTEM_2::res_alert_text(int Id)
{
  return ALERTS_RESERVE[Id].alert_text();
}

void ALERT_SYSTEM_2::res_acknowlege(int Id)
{
   ALERTS_RESERVE[Id].acknowlege();
}

bool ALERT_SYSTEM_2::res_is_clear(int Id)
{
  return ALERTS_RESERVE[Id].is_clear();
}

void ALERT_SYSTEM_2::res_clear(int Id)
{
  ALERTS_RESERVE[Id].clear();
}

// generic alerts
int ALERT_SYSTEM_2::gen_size()
{
  return GENERIC_ALERTS.size();
}

bool ALERT_SYSTEM_2::gen_warning(int Id)
{
  return GENERIC_ALERTS[Id].warning();
}

bool ALERT_SYSTEM_2::gen_display(int Id)
{
  return  GENERIC_ALERTS[Id].display();
}

string ALERT_SYSTEM_2::gen_alert_text(int Id)
{
  return  GENERIC_ALERTS[Id].alert_text();
}

int ALERT_SYSTEM_2::gen_alert_id(int Id)
{
  return  GENERIC_ALERTS[Id].id();
}

void ALERT_SYSTEM_2::gen_acknowlege(int Id)
{
  GENERIC_ALERTS[Id].acknowlege();
}

// all alerts

int ALERT_SYSTEM_2::alert_count()
{
  return (GENERIC_ALERTS.size() + ALERTS_RESERVE_COUNT);
}

void ALERT_SYSTEM_2::add_generic_alert(COMMAND_THREAD &Thread, unsigned long current_time_frame, string Text)
{
  ALERT_2_TYPE_MONITOR tmp_alert;

  tmp_alert.alert_no_condition(LATEST_ID, Text);

  GENERIC_ALERTS.push_back(tmp_alert);

  LATEST_ID++;

  SOUND_SYSTEM.play_a6(Thread, current_time_frame, MUTE);

  CHANGED = true;
}

void ALERT_SYSTEM_2::alert_list_clean()
{
  if (GENERIC_ALERTS.size() > 0)
  {
    int alert_num = 0;
    while (alert_num < (int)GENERIC_ALERTS.size())
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

  for (int alert_pos = 0; alert_pos < (int)GENERIC_ALERTS.size(); alert_pos++)
  {
    GENERIC_ALERTS[alert_pos].set_display_on();
  }
}

void ALERT_SYSTEM_2::sound_alert(COMMAND_THREAD &Thread, unsigned long current_time_frame, int Value)
{
  if (Value == 3)
  {
    SOUND_SYSTEM.play_alert_level_3(Thread, current_time_frame, MUTE);
  }
  else if (Value == 2)
  {
    SOUND_SYSTEM.play_alert_level_2(Thread, current_time_frame, MUTE);
  }
  else
  {
    SOUND_SYSTEM.play_alert_level_1(Thread, current_time_frame, MUTE);
  }
}

void ALERT_SYSTEM_2::sound_tone(COMMAND_THREAD &Thread, unsigned long current_time_frame, int Value)
{
  if (Value == 61)
  {
    SOUND_SYSTEM.play_c6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 62)
  {
    SOUND_SYSTEM.play_d6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 63)
  {
    SOUND_SYSTEM.play_e6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 64)
  {
    SOUND_SYSTEM.play_f6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 65)
  {
    SOUND_SYSTEM.play_g6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 66)
  {
    SOUND_SYSTEM.play_a6(Thread, current_time_frame, MUTE);
  }
  else if (Value == 67)
  {
    SOUND_SYSTEM.play_b6(Thread, current_time_frame, MUTE);
  }
  else // invalid
  {
    SOUND_SYSTEM.play_alert_level_1(Thread, current_time_frame, MUTE);
  }
}


#endif