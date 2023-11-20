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

using namespace std;

// -------------------------------------------------------------------------------------
//  Alert Class

class ALERT_2_TYPE_GENERIC
{
  private:

  bool ACKNOWLEGED = false;

  public:

  bool ACTIVE = false;
  bool DISPLAY = true;

  int ID = 0;

  string TEXT = "";

  void acknowlege();

  bool is_clear();
};

class ALERT_SYSTEM_2
{
  private:

  int LATEST_ID = 0;

  bool CHANGED = false;
  
  public:
  
  deque<ALERT_2_TYPE_GENERIC> GENERIC_ALERTS;

  bool changed();
  // 

  void add_generic_alert(string Text);
  //

  void alert_list_clean();

};



#endif