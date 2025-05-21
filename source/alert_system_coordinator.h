// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: alert_system_coordinator.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef ALERT_SYSTEM_COORDINATOR_H
#define ALERT_SYSTEM_COORDINATOR_H

#include "definitions.h"
#include "auto.h"
#include "alert_system_2.h"

class ALERT_SYSTEM_COORDINATOR
{
  private:
  
  public:

  void create(ALERT_SYSTEM_2 &All_Alerts);

  void check_for_alerts(ALERT_SYSTEM_2 &All_Alerts, AUTOMOBILE &Car_Info, bool Auto_Changed);
};

#endif