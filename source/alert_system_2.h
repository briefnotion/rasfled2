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
  // Indicates alert was raised.
  // Will stay in true condition until alert is cleared.
  //  or value in condition is clear

  bool WARNING = false;
  // Indicates alert was raised and clear of the raise alert condition 
  //  but has not yet passed the clear alert condition.
  //  aka, in between active and clear.
  
  bool ACKNOWLEGED = false; // Indicates the alert was acknowedeged.
  bool DISPLAY = true;      // Determine if alert is to be displayed.
  string ALERT_TEXT = "";   // Text of alert

  public:

  void alert_no_condition(int Id, string Alert_Text);

  bool alert_condition(int Id, bool Raise_Alert, bool Clear_Alert);
  // returns true if requesting a description.
  
  bool active();        // Returns ACTIVE value
  bool warning();       // Returns WARNING value
  bool display();       // Returns DISPLAY value
  int id();             // ID of alert
  string alert_text();  // Text value of alert

  void set_display_on();
  // Will set display to true

  void update_alert_text(string Text);
  // Change the text value of the alert;

  void acknowlege();
  // Indicates the alert was acknowedeged.
  //  Also, turns display off.

  bool is_clear();
  // Returns true if alert is: acknowledged, not active, and not to be 
  //  displayed.

  void clear();
  // Sets active to false;
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