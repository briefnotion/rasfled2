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

// -------------------------------------------------------------------------------------

// Reserved Alert List
#define RESERVE_ALERT_LIST_SIZE     5

#define RESERVE_ALERT_TEMP_S_TEMP   0
#define RESERVE_ALERT_TEMP_COOLANT  1
#define RESERVE_ALERT_ELEC_VOLTAGE  2
#define RESERVE_ALERT_FUEL_LEVEL    3
#define RESERVE_ALERT_TEMP_INTAKE   4

// -------------------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------------------------------
//  Alert Class

class ALERT_2_TYPE_MONITOR
{
  private:

  int ID = 0;

  bool ACTIVE = false;
  
  bool ACKNOWLEGED = false;
  bool DISPLAY = true;
  string ALERT_TEXT = "";

  public:

  void alert_no_condition(int Id, string Alert_Text);

  bool alert_condition(int Id, bool Raise_Alert, bool Clear_Alert);
  // returns true if requesting a description.
  
  bool active();
  bool display();
  int id();
  string alert_text();

  void set_display_on();

  void update_alert_text(string Text);
  void acknowlege();
  bool is_clear();
  void clear();
};

class ALERT_SYSTEM_2
{
  private:

  int LATEST_ID = 10000;

  bool CHANGED = false;
  
  public:
  
  deque<ALERT_2_TYPE_MONITOR> ALERTS;
  ALERT_2_TYPE_MONITOR ALERTS_RESERVE[RESERVE_ALERT_LIST_SIZE];

  bool changed();
  // 

  void add_generic_alert(string Text);
  //

  void alert_list_clean();

  void display_active_alerts();

};



#endif