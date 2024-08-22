// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_alerts.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef WIDGETS_ALERTS_H
#define WIDGETS_ALERTS_H

// Standard Header Files
//#include <fstream>
//#include <iostream>
#include <string>
#include <vector>

// Rasfled Includes
//#include "system.h"
//#include "rasapi.h"
//#include "fled_time.h"
#include "widgets.h"
#include "widgets_drawing.h"
//#include "screen4_helper.h"
#include "alert_system_2.h"

// -------------------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------------------------------

class ALERT_WIDGET_WIDGTS
{
  public:

  BAR_TECH VALUES;

  void create(system_data &sdSysData, string Name);

};

class ALERT_WIDGET_PROPERTIES_LIST
{
  public:

  vector<ALERT_WIDGET_WIDGTS> LIST;

  void check_properties_list(system_data &sdSysData, int Alert_Num, string Name);
};

class ALERT_WIDGET
{
  private:

  int BACKGROUND_COLOR_VALUE;
  ImColor BACKGROUND_COLOR;

  int TEXT_COLOR_VALUE;
  ImColor TEXT_COLOR;
  //NEO_COLOR TEXT_COLOR;

  // Simple list to keep track if gadget properties
  ALERT_WIDGET_PROPERTIES_LIST PROPERTIES_RESERVE_LIST;

  public:

  void draw(system_data &sdSysData, ALERT_SYSTEM_2 &Alerts_List);
};

// -------------------------------------------------------------------------------------

#endif