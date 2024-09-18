// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: alert_system_2.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef ALERT_SYSTEM_2_H
#define ALERT_SYSTEM_2_H

// Standard Header Files
#include <stdio.h>
#include <string>
#include <deque>

// RASFled related header files
#include "stringthings.h"
#include "commands.h"
#include "sounds.h"

// -------------------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------------------------------
//  Alert Class

class ALERT_2_TYPE_MONITOR
{
  private:

  int ID = 0;

  bool ACTIVE = false;
  // Indicates alert was raised.
  // Will stay in true condition until alert is cleared.
  //  or value in condition is clear

  bool WARNING = false;
  // Indicates alert was raised and clear of the raise alert condition 
  //  but has not yet passed the clear alert condition.
  //  aka, in between active and clear.
  
  bool ACKNOWLEGED = false; // Indicates the alert was acknowedeged.
  bool DISPLAY = false;      // Determine if alert is to be displayed.
  
  string ALERT_TITLE       = "";   // Display Title of Alert
  string ALERT_TEXT_LINE_1 = "";   // Text of alert
  string ALERT_TEXT_LINE_2 = "";   // Text of alert
  bool SHOW_VALUE_BAR = false;

  public:

  float VALUE = 0;
  float ALERT_VALUE = 0;
  float CLEAR_VALUE = 0;

  void alert_no_condition(int Id, string Title, string Line_1, string Line_2);
  //void alert_no_condition(int Id, string Alert_Text_Line_1);
  
  bool alert_condition(int Id, bool Raise_Alert, bool Clear_Alert, int &Changes);
  // returns true if requesting a description.
  // Changes = 0  - no change
  // Changes = 1  - changed to warning
  // Changes = 2  - raised to alert active
  // Changes = -1 - cleared
  
  bool active();        // Returns ACTIVE value
  bool warning();       // Returns WARNING value
  bool display();       // Returns DISPLAY value
  int id();             // ID of alert

  void set_display_on();
  // Will set display to true

  void set_show_value_bar(bool Value);

  bool show_value_bar();

  string alert_title();  // Text value of alert
  string alert_text_line_1();  // Text value of alert
  string alert_text_line_2();  // Text value of alert

  void update_alert_title(string Text);
  // Change the title value of the alert;

  void update_alert_text_line_1(string Text);
  // Change the text value of the alert;

  void update_alert_text_line_2(string Text);
  // Change the text value of the alert;

  void acknowlege();
  // Indicates the alert was acknowedeged.
  //  Also, turns display off.

  bool is_clear();
  // Returns true if alert is: acknowledged, not active, and not to be 
  //  displayed.

  void clear();
  // Sets active to false;

  void full_reset();
  // Sets all values to initial clear state.  Full Reset
};

class ALERT_SYSTEM_2_PROPERTIES
{
  public:

  string ALERT_SYSTEM_NAME = "X";

  int RESERVE_ALERT_SIZE = 0;
};

class ALERT_SYSTEM_2
{
  private:

  int LATEST_ID = 10000;

  bool CHANGED = false;

  int ALERTS_RESERVE_COUNT = 0;

  vector<string> PASSING_NOTE_QUEUE;
  
  public:

  deque<ALERT_2_TYPE_MONITOR> GENERIC_ALERTS;
  deque<ALERT_2_TYPE_MONITOR> ALERTS_RESERVE;

  private:

  void play_alert_level_1();
  void play_alert_level_2();
  void play_alert_level_3();

  int search_for_alert(deque<ALERT_2_TYPE_MONITOR> &Alert_deque, string Title);
  // returns position of alert with title in list.
  // returns -1 if not found.

  public:

  ALERT_SYSTEM_2_PROPERTIES PROPS;

  bool changed();
  
  // reserved alerts
  void res_alert_no_condition(int Id, string Title, string Line_1, string Line_2);

  bool res_alert_condition(int Id, bool Raise_Alert, bool Clear_Alert);
  bool res_alert_condition_greater_than(int Id, float Value, float Alert_Condition, float Clear_Condition);
  bool res_alert_condition_less_than(int Id, float Value, float Alert_Condition, float Clear_Condition);
  
  void res_update_alert_title(int Id, string Title);
  void res_update_alert_text_line_1(int Id, string Text_Line_1);
  void res_update_line_2_with_conditions(int Id);

  bool res_active(int Id);        // Returns ACTIVE value
  bool res_warning(int Id);       // Returns WARNING value
  bool res_display(int Id);       // Returns DISPLAY value

  string res_alert_title(int Id);  // Text value of alert
  string res_alert_text_line_1(int Id);  // Text value of alert
  string res_alert_text_line_2(int Id);  // Text value of alert

  void generate_reserve_alert(int Id, string Title);
  // Assigne Titles to Generate reserve alerts.
  
  void res_acknowlege(int Id);
  bool res_is_clear(int Id);
  void res_clear(int Id);

  // generic alerts
  //void add_generic_alert(string Text);
  void add_generic_alert(string Title, string Line_1, string Line_2);

  int gen_size();
  bool gen_warning(int Id);       // Returns WARNING value
  bool gen_display(int Id);       // Returns DISPLAY value
  string gen_alert_title(int Id);  // Text value of alert
  string gen_alert_text_line_1(int Id);  // Text value of alert
  string gen_alert_text_line_2(int Id);  // Text value of alert
  int gen_alert_id(int Id);
  void gen_acknowlege(int Id);

  // all alerts
  int alert_count();
  void alert_list_clean(COMMAND_THREAD &Thread, SOUNDS &Sound_System);
  void display_active_alerts();

  void clear_all();
  // Sets all alerts to cleared values as a full reset.
  //  Except, does not change the Reserved Alert List.

  void sound_alert(int Value);
  //  1 - green
  //  2 - yellow
  //  3 - red
};



#endif