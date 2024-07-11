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

void ALERT_2_TYPE_MONITOR::alert_no_condition(int Id, string Title, string Line_1, string Line_2)
{
  ID = Id;
  DISPLAY = true;
  ALERT_TITLE = Title;
  ALERT_TEXT_LINE_1 = Line_1;
  ALERT_TEXT_LINE_2 = Line_2;
}

//void ALERT_2_TYPE_MONITOR::alert_no_condition(int Id, string Alert_Text_Line_1)
//{
//  alert_no_condition(Id, "", Alert_Text_Line_1, "");
//}

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

void ALERT_2_TYPE_MONITOR::set_show_value_bar(bool Value)
{
  SHOW_VALUE_BAR = Value;
}

bool ALERT_2_TYPE_MONITOR::show_value_bar()
{
  return SHOW_VALUE_BAR;
}

string ALERT_2_TYPE_MONITOR::alert_title()
{
  return ALERT_TITLE;
}

string ALERT_2_TYPE_MONITOR::alert_text_line_1()
{
  return ALERT_TEXT_LINE_1;
}

string ALERT_2_TYPE_MONITOR::alert_text_line_2()
{
  return ALERT_TEXT_LINE_2;
}

void ALERT_2_TYPE_MONITOR::update_alert_title(string Text)
{
  ALERT_TITLE = Text;
}

void ALERT_2_TYPE_MONITOR::update_alert_text_line_1(string Text)
{
  ALERT_TEXT_LINE_1 = Text;
  ALERT_TEXT_LINE_2 = "";
}

void ALERT_2_TYPE_MONITOR::update_alert_text_line_2(string Text)
{
  ALERT_TEXT_LINE_2 = Text;
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
void ALERT_SYSTEM_2::play_alert_level_1()
{
  PASSING_NOTE_QUEUE.push_back("d7");
  PASSING_NOTE_QUEUE.push_back( "c7");
}

void ALERT_SYSTEM_2::play_alert_level_2()
{
  PASSING_NOTE_QUEUE.push_back("e7");
  PASSING_NOTE_QUEUE.push_back("d7");
}

void ALERT_SYSTEM_2::play_alert_level_3()
{
  PASSING_NOTE_QUEUE.push_back("c7");
  PASSING_NOTE_QUEUE.push_back("d7");
  PASSING_NOTE_QUEUE.push_back("e7");
}

int ALERT_SYSTEM_2::search_for_alert(deque<ALERT_2_TYPE_MONITOR> &Alert_deque, string Title)
{
  int ret_pos = -1;

  if (Alert_deque.size() > 0)
  {
    for (int pos = 0; pos < (int)Alert_deque.size() && ret_pos == -1; pos++)
    {
      if (Title == Alert_deque[pos].alert_title())
      {
        ret_pos = pos;
      }
    }
  }

  return ret_pos;
}

bool ALERT_SYSTEM_2::changed()
{
  return CHANGED;
}

// reserve alerts
void ALERT_SYSTEM_2::res_alert_no_condition(int Id, string Title, string Line_1, string Line_2)
{
  ALERTS_RESERVE[Id].alert_no_condition(Id, Title, Line_1, Line_2);
  PASSING_NOTE_QUEUE.push_back("f7");
}

bool ALERT_SYSTEM_2::res_alert_condition(int Id, bool Raise_Alert, bool Clear_Alert)
{
  int alert_change = 0;
  bool was_active = ALERTS_RESERVE[Id].active();

  bool ret_description_request = ALERTS_RESERVE[Id].alert_condition(Id, Raise_Alert, Clear_Alert, alert_change);

  if (alert_change  == 2 && was_active == false)       // Alert
  {
    play_alert_level_3();
  }
  //else if (alert_change  == 1)  // Warning
  //{
  //  Sound_System.play_alert_level_2(Thread);
  //}
  else if (alert_change  == -1)                       // Cleared
  {
    play_alert_level_1();
  }

  return ret_description_request;
}

bool ALERT_SYSTEM_2::res_alert_condition_greater_than(int Id, float Value, float Alert_Condition, float Clear_Condition)
{
  ALERTS_RESERVE[Id].VALUE = Value;
  ALERTS_RESERVE[Id].ALERT_VALUE = Alert_Condition;
  ALERTS_RESERVE[Id].CLEAR_VALUE = Clear_Condition;

  return res_alert_condition(Id, Value >= Alert_Condition, Value < Clear_Condition);
}

bool ALERT_SYSTEM_2::res_alert_condition_less_than(int Id, float Value, float Alert_Condition, float Clear_Condition)
{
  ALERTS_RESERVE[Id].VALUE = Value;
  ALERTS_RESERVE[Id].ALERT_VALUE = Alert_Condition;
  ALERTS_RESERVE[Id].CLEAR_VALUE = Clear_Condition;

  return res_alert_condition(Id, Value <= Alert_Condition, Value > Clear_Condition);
}

void ALERT_SYSTEM_2::res_update_alert_title(int Id, string Title)
{
  ALERTS_RESERVE[Id].update_alert_title(Title);
}

void ALERT_SYSTEM_2::res_update_alert_text_line_1(int Id, string Text_Line_1)
{
  ALERTS_RESERVE[Id].update_alert_text_line_1(Text_Line_1);
}

void ALERT_SYSTEM_2::res_update_line_2_with_conditions(int Id)
{
  ALERTS_RESERVE[Id].update_alert_text_line_2("V: " + to_string_round_to_nth(ALERTS_RESERVE[Id].VALUE, 2) + "   " + 
                                              "C: " + to_string_round_to_nth(ALERTS_RESERVE[Id].CLEAR_VALUE, 2) + "   " + 
                                              "A: " + to_string_round_to_nth(ALERTS_RESERVE[Id].ALERT_VALUE, 2));
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

string ALERT_SYSTEM_2::res_alert_title(int Id)
{
  return ALERTS_RESERVE[Id].alert_title();
}

string ALERT_SYSTEM_2::res_alert_text_line_1(int Id)
{
  return ALERTS_RESERVE[Id].alert_text_line_1();
}

string ALERT_SYSTEM_2::res_alert_text_line_2(int Id)
{
  return ALERTS_RESERVE[Id].alert_text_line_2();
}

void ALERT_SYSTEM_2::generate_reserve_alert(int Id, string Title)
{
  while (Id >= (int)ALERTS_RESERVE.size())
  {
    ALERT_2_TYPE_MONITOR tmp_alert_monitor;

    ALERTS_RESERVE.push_back(tmp_alert_monitor);
  }

  ALERTS_RESERVE[Id].update_alert_title(Title);
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

//void ALERT_SYSTEM_2::add_generic_alert(string Text)
//{
//  ALERT_2_TYPE_MONITOR tmp_alert;
//
//  tmp_alert.alert_no_condition(LATEST_ID, Text);
//
//  GENERIC_ALERTS.push_back(tmp_alert);
//
//  LATEST_ID++;
//
//  PASSING_NOTE_QUEUE.push_back("f7");
//
//  CHANGED = true;
//}

void ALERT_SYSTEM_2::add_generic_alert(string Title, string Line_1, string Line_2)
{
  int find_id = search_for_alert(GENERIC_ALERTS, Title);

  if (find_id == -1)
  {
    ALERT_2_TYPE_MONITOR tmp_alert;

    tmp_alert.alert_no_condition(LATEST_ID, Title, Line_1, Line_2);
    GENERIC_ALERTS.push_back(tmp_alert);
    LATEST_ID++;
    
    PASSING_NOTE_QUEUE.push_back("f7");
  }
  else
  {
    GENERIC_ALERTS[find_id].update_alert_text_line_1(Line_1);
    GENERIC_ALERTS[find_id].update_alert_text_line_2(Line_2);
    GENERIC_ALERTS[find_id].set_display_on();   // tmp until clear method created.
  }

  CHANGED = true;
}

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

string ALERT_SYSTEM_2::gen_alert_title(int Id)
{
  return  GENERIC_ALERTS[Id].alert_title();
}

string ALERT_SYSTEM_2::gen_alert_text_line_1(int Id)
{
  return  GENERIC_ALERTS[Id].alert_text_line_1();
}

string ALERT_SYSTEM_2::gen_alert_text_line_2(int Id)
{
  return  GENERIC_ALERTS[Id].alert_text_line_2();
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

void ALERT_SYSTEM_2::alert_list_clean(COMMAND_THREAD &Thread, SOUNDS &Sound_System)
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
  for (int res_alert_pos = 0; res_alert_pos < (int)ALERTS_RESERVE.size(); res_alert_pos++)
  {
    if (ALERTS_RESERVE[res_alert_pos].active())
    {
      ALERTS_RESERVE_COUNT++;
    }
  }

  if (PASSING_NOTE_QUEUE.size() > 0)
  {
    for (int pos = 0; pos < (int)PASSING_NOTE_QUEUE.size(); pos++)
    {
      Sound_System.add_note_to_queue(Thread, PASSING_NOTE_QUEUE[pos]);
    }
    PASSING_NOTE_QUEUE.clear();
  }
}

void ALERT_SYSTEM_2::display_active_alerts()
{
  for (int alert_pos = 0; alert_pos < (int)ALERTS_RESERVE.size(); alert_pos++)
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

void ALERT_SYSTEM_2::sound_alert(int Value)
{
  if (Value == 3)
  {
    play_alert_level_3();
  }
  else if (Value == 2)
  {
    play_alert_level_2();
  }
  else
  {
    play_alert_level_1();
  }
}



#endif