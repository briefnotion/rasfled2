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
#include "sounds.h"

// -------------------------------------------------------------------------------------

// Reserved Alert List
#define RESERVE_ALERT_LIST_SIZE       7

#define RESERVE_ALERT_TEMP_S_TEMP     0
#define RESERVE_ALERT_TEMP_COOLANT    1
#define RESERVE_ALERT_ELEC_VOLTAGE    2
#define RESERVE_ALERT_FUEL_LEVEL      3
#define RESERVE_ALERT_TEMP_INTAKE     4
#define RESERVE_ALERT_UNKNOWN_MESSAGE 5
#define RESERVE_ALERT_MIL             6

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
  string ALERT_TEXT = "";   // Text of alert

  public:

  void alert_no_condition(int Id, string Alert_Text);

  bool alert_condition(int Id, bool Raise_Alert, bool Clear_Alert, int &Changes);
  // returns true if requesting a description.
  // Changes = 0  - no change
  // Changes = 1  - changed to waring
  // Changes = 2  - raised to alert active
  // Changes = -1 - cleared
  
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

  bool MUTE = false;

  int LATEST_ID = 10000;

  bool CHANGED = false;

  int ALERTS_RESERVE_COUNT = 0;

  SOUNDS SOUND_SYSTEM;  // Primitive sound system.
  
  deque<ALERT_2_TYPE_MONITOR> GENERIC_ALERTS;
  ALERT_2_TYPE_MONITOR ALERTS_RESERVE[RESERVE_ALERT_LIST_SIZE];

  public:

  void set_mute(bool Mute);

  bool changed();
  
  // reserved alerts
  void res_alert_no_condition(int Id, string Alert_Text);
  bool res_alert_condition(int Id, bool Raise_Alert, bool Clear_Alert);
  void res_update_alert_text(int Id, string Text);
  bool res_active(int Id);        // Returns ACTIVE value
  bool res_warning(int Id);       // Returns WARNING value
  bool res_display(int Id);       // Returns DISPLAY value
  string res_alert_text(int Id);  // Text value of alert
  void res_acknowlege(int Id);
  bool res_is_clear(int Id);
  void res_clear(int Id);

  // generic alerts
  int gen_size();
  bool gen_warning(int Id);       // Returns WARNING value
  bool gen_display(int Id);       // Returns DISPLAY value
  string gen_alert_text(int Id);  // Text value of alert
  int gen_alert_id(int Id);
  void gen_acknowlege(int Id);

  // all alerts
  int alert_count();
  void add_generic_alert(string Text);
  void alert_list_clean();
  void display_active_alerts();

  void sound_alert(int Value);

};



#endif