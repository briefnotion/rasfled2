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

/**/
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


void ALERT_SYSTEM_COORDINATOR::check_for_alerts(SCREEN4_PANEL_CONTROL &Panel_Control, ALERT_SYSTEM_2 &All_Alerts, AUTOMOBILE &Car_Info, bool Auto_Changed)
{
  if (Auto_Changed)
  {
    float s_temp = Car_Info.CALCULATED.s_temp();

    // S-Temp alert
    if (All_Alerts.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_S_TEMP, 
      s_temp, 65.0f, 50.0f))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_S_TEMP, "S-Temp Value is " + to_string((int)s_temp));
      All_Alerts.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_S_TEMP);
      All_Alerts.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_S_TEMP].set_show_value_bar(true);
    }


    // MIL alert
    if (All_Alerts.res_alert_condition(AUTO_RESERVE_ALERT_MIL, 
      Car_Info.STATUS.SYSTEM.malfunction_indicator_light() == true , 
      Car_Info.STATUS.SYSTEM.malfunction_indicator_light() == false))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_MIL, "Malfunction Indicator Lamp is " + to_string(Car_Info.STATUS.SYSTEM.malfunction_indicator_light()));
    }

    // Coolant alert
    if (All_Alerts.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_COOLANT, 
          Car_Info.STATUS.TEMPS.COOLANT_05.val_c(), 110.0f, 100.0f))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_COOLANT, "Coolant Temp Value is " + Car_Info.STATUS.TEMPS.COOLANT_05.c());
      All_Alerts.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_COOLANT);
      All_Alerts.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_COOLANT].set_show_value_bar(true);
    }

    // Air Intake Temp alert
    if (All_Alerts.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_INTAKE, 
          Car_Info.STATUS.TEMPS.AIR_INTAKE_0f.val_c(), 
          Car_Info.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 25.0f, 
          Car_Info.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 15.0f))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_INTAKE, "Intake Temp Value is " + Car_Info.STATUS.TEMPS.AIR_INTAKE_0f.c());
      All_Alerts.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_INTAKE);
      All_Alerts.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_INTAKE].set_show_value_bar(true);
    }

    // Voltage alert
    if (All_Alerts.res_alert_condition_less_than(AUTO_RESERVE_ALERT_ELEC_VOLTAGE, 
          Car_Info.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v(), 11.5f, 12.0f))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_ELEC_VOLTAGE, "Voltage Value is " + Car_Info.STATUS.ELECTRICAL.CONTROL_UNIT_42.v());
      All_Alerts.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_ELEC_VOLTAGE);
      All_Alerts.ALERTS_RESERVE[AUTO_RESERVE_ALERT_ELEC_VOLTAGE].set_show_value_bar(true);
    }

    // Fuel Level alert
    if (All_Alerts.res_alert_condition_less_than(AUTO_RESERVE_ALERT_FUEL_LEVEL, 
          Car_Info.CALCULATED.fuel_level_emperical_val(), 3.0f, 6.0f))
    {
      All_Alerts.res_update_alert_text_line(AUTO_RESERVE_ALERT_FUEL_LEVEL, "Fuel Level is " + Car_Info.CALCULATED.fuel_level_emperical());
      All_Alerts.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_FUEL_LEVEL);
      All_Alerts.ALERTS_RESERVE[AUTO_RESERVE_ALERT_FUEL_LEVEL].set_show_value_bar(true);
    }

    // Autonomous

    // Temperature Screen
    if ((Car_Info.STATUS.TEMPS.AIR_INTAKE_0f.val_c() > Car_Info.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 25.0f) ||
          (Car_Info.STATUS.TEMPS.COOLANT_05.val_c() > 110.0f) ||
          (s_temp > 65.0f))
    {
      Panel_Control.AUTO_TEMPERATURE.request();
    }

    // Malfunction Screen
    if ((Car_Info.STATUS.SYSTEM.malfunction_indicator_light() == true) ||
          Car_Info.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v() < 11.5f)
    {
      Panel_Control.AUTO_MALFUNCTION.request();
    }
  }

  Panel_Control.activate();

}


#endif