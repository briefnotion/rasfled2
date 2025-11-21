// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: alert_system_coordinator.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef ALERT_SYSTEM_COORDINATOR_CPP
#define ALERT_SYSTEM_COORDINATOR_CPP

#include "alert_system_coordinator.h"

using namespace std;

void ALERT_SYSTEM_COORDINATOR::create(ALERT_SYSTEM_2 &All_Alerts)
{
  // Initialize AUTO Alert_2 System - Generate Reserve Alerts
  All_Alerts.PROPS.ALERT_SYSTEM_NAME = "AUTO";
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_TEMP_S_TEMP, "SUPER TEMP");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_TEMP_COOLANT, "COOLANT TEMP");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_ELEC_VOLTAGE, "VOLTAGE");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_FUEL_LEVEL, "FUEL LEVEL");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_TEMP_INTAKE, "INTTAKE TEMP");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_UNKNOWN_MESSAGE, "UNKNOW COMMUNICATION");
  All_Alerts.generate_reserve_alert(AUTO_RESERVE_ALERT_MIL, "MALFUNTION INDICATOR LAMP");
}

void ALERT_SYSTEM_COORDINATOR::check_for_alerts(system_data &sdSysData, AUTOMOBILE_HANDLER &Automobile_Handler)
{
  if (Automobile_Handler.CHANGED)
  {
    Automobile_Handler.CHANGED = false;

    float s_temp = sdSysData.CAR_INFO.CALCULATED.s_temp();

    // S-Temp alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_S_TEMP, 
      s_temp, 70.0f, 55.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_S_TEMP, "S-Temp Value is " + to_string((int)s_temp));
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_S_TEMP);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_S_TEMP].set_show_value_bar(true);
    }


    // MIL alert
    if (sdSysData.ALL_ALERTS.res_alert_condition(AUTO_RESERVE_ALERT_MIL, 
      sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light() == true , 
      sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light() == false))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_MIL, "Malfunction Indicator Lamp is " + to_string(sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light()));
    }

    // Coolant alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_COOLANT, 
          sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c(), 110.0f, 100.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_COOLANT, "Coolant Temp Value is " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c());
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_COOLANT);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_COOLANT].set_show_value_bar(true);
    }

    // Air Intake Temp alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_INTAKE, 
          sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c(), 
          sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 25.0f, 
          sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 15.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_INTAKE, "Intake Temp Value is " + sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c());
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_INTAKE);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_INTAKE].set_show_value_bar(true);
    }

    // Voltage alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_less_than(AUTO_RESERVE_ALERT_ELEC_VOLTAGE, 
          sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v(), 11.5f, 12.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_ELEC_VOLTAGE, "Voltage Value is " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v());
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_ELEC_VOLTAGE);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_ELEC_VOLTAGE].set_show_value_bar(true);
    }

    // Fuel Level alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_less_than(AUTO_RESERVE_ALERT_FUEL_LEVEL, 
          sdSysData.CAR_INFO.CALCULATED.fuel_level_emperical_val(), 3.0f, 6.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_FUEL_LEVEL, "Fuel Level is " + sdSysData.CAR_INFO.CALCULATED.fuel_level_emperical());
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_FUEL_LEVEL);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_FUEL_LEVEL].set_show_value_bar(true);
    }

    // Autonomous

    // Camera
    if (sdSysData.CAMERA_BACKUP.camera_online())
    {
      // Camera Gear
      if (sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_neutral() ||
      sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_reverse())
      {
        sdSysData.PANEL_CONTROL.CAMERA_BACKUP.request(sdSysData.PROGRAM_TIME.current_frame_time(), 2000, "GEAR ");
      }

      // Camera Hazards
      if (sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
      {
        sdSysData.PANEL_CONTROL.CAMERA_BACKUP.request(sdSysData.PROGRAM_TIME.current_frame_time(), 2000, "HAZ ");
      }

      // Camera Door Open
      if (sdSysData.PANEL_CONTROL.AUTO_DOOR_OPEN_COUNT > 0)
      {
        sdSysData.PANEL_CONTROL.CAMERA_BACKUP.request(sdSysData.PROGRAM_TIME.current_frame_time(), 2000, "DOOR ");
      }

      // Camer Blinker
      if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left() || sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right())
      {
        sdSysData.PANEL_CONTROL.CAMERA_BACKUP.request(sdSysData.PROGRAM_TIME.current_frame_time(), 2000, "TURN ");
      }
    }

    // Temperature Screen
    if ((sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c() > sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 25.0f) ||
          (sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c() > 110.0f) ||
          (s_temp > 70.0f))
    {
      sdSysData.PANEL_CONTROL.AUTO_TEMPERATURE.request(sdSysData.PROGRAM_TIME.current_frame_time(), 5000, "TMP ");
    }

    // Malfunction Screen
    if ((sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light() == true) ||
          sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v() < 11.5f)
    {
      sdSysData.PANEL_CONTROL.AUTO_MALFUNCTION.request(sdSysData.PROGRAM_TIME.current_frame_time(), 60000, "MIL ");
    }

    // Pressure Screen
    if ((sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.val_kPa() > 900.0f))
    {
      sdSysData.PANEL_CONTROL.AUTO_PRESSURE.request(sdSysData.PROGRAM_TIME.current_frame_time(), 5000, "PRS ");
    }

    // Acceleration Screen
    if (sdSysData.PANEL_CONTROL.FLAG_AUTO_ACCELERATION_DECELERATION)
    {
      sdSysData.PANEL_CONTROL.AUTO_ACCELERATION.request(sdSysData.PROGRAM_TIME.current_frame_time(), 5000, "ACC ");
    }
  }

  // Camera Timer
    // Camera
  if (sdSysData.CAMERA_BACKUP.camera_online())
  {
    // Camera Timer
    if (sdSysData.cdTIMER.is_active())
    {
      sdSysData.PANEL_CONTROL.CAMERA_BACKUP.request(sdSysData.PROGRAM_TIME.current_frame_time(), 2000, "TIMER ");
    }
  }

  // Aircraft
  if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.PANEL_FLAG_CHANGED)
  {
    if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.PANEL_FLAG_PROXIMITY)
    {
      sdSysData.PANEL_CONTROL.ADSB_MAP_MIN_DISTANCE.request(sdSysData.PROGRAM_TIME.current_frame_time(), 5000, "ADS-B MIN ");
    }

    if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY || 
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.PANEL_FLAG_LONG_DISTANCE)
    {
      sdSysData.PANEL_CONTROL.ADSB_MAP_MAX_DISTANCE.request(sdSysData.PROGRAM_TIME.current_frame_time(), 30000, "ADS-B MAX ");
    }
    
    sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.clear_panel_flags();
  }

  sdSysData.PANEL_CONTROL.activate(sdSysData.PROGRAM_TIME.current_frame_time());

}

#endif