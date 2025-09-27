// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: camera_coordinator.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef CAMERA_COORDINATOR_CPP
#define CAMERA_COORDINATOR_CPP

#include "camera_coordinator.h"

using namespace std;
// ---------------------------------------------------------------------------------------

void CAMERA_SYSTEM_COORDINATOR::check(system_data &sdSysData)
{
  if (sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_neutral() ||
      sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_reverse())
  {
    // If in reverse or neutral
    sdSysData.PANEL_CONTROL.CAMERA_BACKUP_ON_SYSTEM = true;
  }
  else if (sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
  {
    // If Hazards On
    sdSysData.PANEL_CONTROL.CAMERA_BACKUP_ON_SYSTEM = true;
  }
  else if (sdSysData.PANEL_CONTROL.AUTO_DOOR_OPEN_COUNT > 0)
  {
    // If a door is open
    sdSysData.PANEL_CONTROL.CAMERA_BACKUP_ON_SYSTEM = true;
  }
  else if (sdSysData.cdTIMER.is_active())
  {
    // Timer is on
    sdSysData.PANEL_CONTROL.CAMERA_BACKUP_ON_SYSTEM = true;
  }

  else
  {
    // Turn off Backup Camera
    sdSysData.PANEL_CONTROL.CAMERA_BACKUP_ON_SYSTEM = false;
  }
}

// ---------------------------------------------------------------------------------------

#endif