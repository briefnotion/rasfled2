// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_automobile.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_AUTOMOBILE_CPP
#define WIDGETS_AUTOMOBILE_CPP

#include "widgets_automobile.h"

// ---------------------------------------------------------------------------------------

/*
void BLANK::display(const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {

  }
  ImGui::End();
}
*/

// ---------------------------------------------------------------------------------------

void AUTOMOBILE_SCREEN::update(system_data &sdSysData, unsigned long tmeFrame_Time)
{
  SDATA.MESSAGES = sdSysData.CAR_INFO.message_count;
  SDATA.SPEED = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph();
  SDATA.VOLTAGE = sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v();

  SDATA.LATEST_UNKNOWN_MESSAGE = sdSysData.CAR_INFO.DATA.AD_UNKNOWN.ORIG;

  //SDATA.CRUISE_CONTROL_SET = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control();
  SDATA.CRUISE_CONTROL_SET_SPEED = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed();

  SDATA.ACCELERATION = sdSysData.CAR_INFO.CALCULATED.acceleration();

  SDATA.RPM = sdSysData.CAR_INFO.STATUS.RPM.rpm();
  SDATA.TORQUE = sdSysData.CAR_INFO.STATUS.POWER.load();

  SDATA.STEERING_WHEEL_ANGLE = sdSysData.CAR_INFO.STATUS.STEERING.steering_wheel_angle();
  SDATA.STEERING_WHEEL_LEFT_OF_CENTER = sdSysData.CAR_INFO.STATUS.STEERING.left_of_center();
  SDATA.STEERING_WHEEL_TURNING_DIRECTION = sdSysData.CAR_INFO.STATUS.STEERING.turning_direction();

  SDATA.TEMP_AMBIANT = sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.c();
  SDATA.TEMP_INTAKE = sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c();
  SDATA.TEMP_COOLANT = sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c();
  SDATA.TEMP_CATALYST = sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.c();
  SDATA.TEMP_SUPER_TEMP = sdSysData.CAR_INFO.CALCULATED.s_temp();

  SDATA.FUEL_RAIL_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa();
  SDATA.EVAP_SYSTEM_VAP_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa();

  SDATA.BAROMETER = sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.inHg();

  /*
  //--

  MESSAGES.set_text(to_string(sdSysData.CAR_INFO.message_count));

  AD_UNKNOWN.set_text(sdSysData.CAR_INFO.DATA.AD_UNKNOWN.ORIG);

  //-----------
  // Large Velocity
  if (sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph() >= 10)
  {
    LARGE_SPEED_10.set_text(CHARACTERS_6X5.number(get_2_pos_of_int((int)sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph())), tmeFrame_Time);
  }
  else
  {
    LARGE_SPEED_10.set_text(CHARACTERS_6X5.space(), tmeFrame_Time);
  }

  LARGE_SPEED_1.set_text(CHARACTERS_6X5.number(get_1_pos_of_int((int)sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph())), tmeFrame_Time);

  if (sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control() == true)
  {
    if (is_within(sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph(), 
                  sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed() - .5,
                  sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed() + .5) == true)
    {
      LARGE_SPEED_1.set_color(COLOR_GREEN, COLOR_BLACK);
      LARGE_SPEED_10.set_color(COLOR_GREEN, COLOR_BLACK);
    }
    else
    {
      LARGE_SPEED_1.set_color(COLOR_YELLOW, COLOR_BLACK);
      LARGE_SPEED_10.set_color(COLOR_YELLOW, COLOR_BLACK);
    }
  }
  else
  {
    LARGE_SPEED_1.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_SPEED_10.set_color(COLOR_WHITE, COLOR_BLACK);
  }

  //-----------
  // Large ACCELERATION
  LARGE_ACCELERATION_10.set_text(CHARACTERS_6X5.number(get_2_pos_of_int(10 * abs(sdSysData.CAR_INFO.CALCULATED.acceleration())), true), tmeFrame_Time);
  LARGE_ACCELERATION_1.set_text(CHARACTERS_6X5.number(get_1_pos_of_int(10 * abs(sdSysData.CAR_INFO.CALCULATED.acceleration()))), tmeFrame_Time);

  if (sdSysData.CAR_INFO.CALCULATED.acceleration() >= -.1)
  {
    LARGE_ACCELERATION_DESC.set_color(COLOR_WHITE, COLOR_BLUE);
  }
  else
  {
    LARGE_ACCELERATION_DESC.set_color(COLOR_YELLOW, COLOR_BLACK);
  }

  if (sdSysData.CAR_INFO.CALCULATED.acceleration() >= -5 && sdSysData.CAR_INFO.CALCULATED.acceleration() <= 3)
  {
    LARGE_ACCELERATION_10.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_1.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_MIN.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_MAX.set_color(COLOR_WHITE, COLOR_BLACK);
  }
  else
  {
    LARGE_ACCELERATION_10.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_1.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_MIN.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_MAX.set_color(COLOR_YELLOW, COLOR_BLACK);
  }

  LARGE_ACCELERATION_MIN.set_text(to_string_round_to_nth(abs(sdSysData.CAR_INFO.CALCULATED.ACCELERATION_MIN_MAX_HISTORY.min_float()), 1));
  LARGE_ACCELERATION_MAX.set_text(to_string_round_to_nth(sdSysData.CAR_INFO.CALCULATED.ACCELERATION_MIN_MAX_HISTORY.max_float(), 1));
  
  //-----------
  // Large GEAR
  
  LARGE_GEAR_1.set_text(CHARACTERS_6X5.number(sdSysData.CAR_INFO.STATUS.GEAR.reported()), tmeFrame_Time);
  LARGE_GEAR_S.set_text(CHARACTERS_6X5.character(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_short_desc()[0]), tmeFrame_Time);

  // Large TACH
  LARGE_TACH_1.set_text(CHARACTERS_6X5.number(get_2_pos_of_int(sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100)), tmeFrame_Time);
  LARGE_TACH_10.set_text(CHARACTERS_6X5.number(get_1_pos_of_int(sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100)), tmeFrame_Time);

  TACHOMETER_MIN_MAX.put_value(sdSysData.CAR_INFO.STATUS.RPM.val_rpm(), tmeFrame_Time);
  TACHOMETER_VALUE.set_text(to_string(sdSysData.CAR_INFO.STATUS.RPM.val_rpm()), tmeFrame_Time);
  TACHOMETER_MAX.set_text(to_string(TACHOMETER_MIN_MAX.max()), tmeFrame_Time);

  //-----------

  ACCELERATION.set_text(to_string_round_to_nth(abs(sdSysData.CAR_INFO.CALCULATED.acceleration()), 2));
  
  //-----------
  // Steering Wheel

  if (sdSysData.CAR_INFO.STATUS.STEERING.val_left_of_center() == true)
  {
    STEERING_WHEEL.update( -1 * (sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle()) + 180, tmeFrame_Time);
  }
  else 
  {
    STEERING_WHEEL.update(sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle() + 180, tmeFrame_Time);
  }
  
  STEERING_WHEEL_ANGLE.set_text(sdSysData.CAR_INFO.STATUS.STEERING.left_of_center() + " " + 
                                  sdSysData.CAR_INFO.STATUS.STEERING.turning_direction() + " " + 
                                  sdSysData.CAR_INFO.STATUS.STEERING.steering_wheel_angle());

  //-----------
  // Tachometer
  TACHOMETER.update((float)sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100, tmeFrame_Time);
  TACHOMETER2.update((float)sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100, tmeFrame_Time);

  //-----------
  // Torque
  TORQUE.update(((float)sdSysData.CAR_INFO.STATUS.POWER.val_load()) / 3, tmeFrame_Time);
  
  TORQUE_VAL.set_text(to_string(sdSysData.CAR_INFO.STATUS.POWER.val_load() / 3), tmeFrame_Time);
  TORQUE_MIN.set_text(to_string(TORQUE.MIN_MAX_HISTORY.min()), tmeFrame_Time);
  TORQUE_MAX.set_text(to_string(TORQUE.MIN_MAX_HISTORY.max()), tmeFrame_Time);

  //-----------
  // Speed
  SPEEDO.update(sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph(), tmeFrame_Time);

  if (SPEEDO.jecto_ready() == true)
  {
    SPEEDO_WATERFALL.update_via_slice(SPEEDO.jecto(), tmeFrame_Time);
  }

  //-----------
  // Super Temp
  SUPER_TEMP.update(sdSysData.CAR_INFO.CALCULATED.s_temp(), tmeFrame_Time);

  // Handle slices in the waterfall.
  if (SUPER_TEMP.jecto_ready() == true)
  {
    SUPER_TEMP_WATERFALL.update_via_slice(SUPER_TEMP.jecto(), tmeFrame_Time);
  }

  //-----------
  // Col 2

  // Temperature   0, 32, 38, 43, 
  ST_1_0.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.c(), tmeFrame_Time);
  ST_1_0.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c(), 0, 32, 38, 43, 48), COLOR_BLACK);

  ST_1_1.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c(), tmeFrame_Time);
  int air_intake_ambiant_diff = sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c() - sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c();
  ST_1_1.set_color(color_range_reverse(air_intake_ambiant_diff, 0, 10, 15, 20, 25), COLOR_BLACK);

  // Coolant - 60, 90, 110, 130, 140 
  ST_1_2.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c(), tmeFrame_Time);
  ST_1_2.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c(), 60, 90, 110, 130, 140), COLOR_BLACK);

  // Coolant - 60, 90, 110, 130, 140 
  ST_1_3.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.c(), tmeFrame_Time);
  ST_1_3.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.val_c(), 150, 600, 800, 1200, 1400), COLOR_BLACK);

  //ST_1_4.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  //ST_1_5.set_text(sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa());

  //ST_1_6.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);

  //ST_1_7.set_text(sdSysData.CAR_INFO.STATUS.
  
  // Voltage
  //ST_1_8.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  //-----------
  // Col 4

  ST_4_0.set_text(sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa());
  
  ST_4_1.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);
 
  //ST_4_2.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c(), tmeFrame_Time);

  // Voltage
  ST_4_3.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);
  ST_4_3.set_color(color_range(sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v(), 0, 8, 10, 14, 16 ), COLOR_BLACK);

  //ST_4_4.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  ST_4_5.set_text(sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.inHg(), tmeFrame_Time);

  //ST_4_6.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);

  //ST_4_7.set_text(sdSysData.CAR_INFO.STATUS.
  
  //ST_4_8.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);
  
  //-----------

  /*
  DATA_SET_01.update(sdSysData.CAR_INFO.DATA.AD_130, sdSysData.CAR_INFO.DATA.AD_130.DATA[6], sdSysData.CAR_INFO.DATA.AD_130.DATA[7], "", tmeFrame_Time);

  DATA_SET_02.update(sdSysData.CAR_INFO.DATA.AD_D0, sdSysData.CAR_INFO.DATA.AD_D0.DATA[6], sdSysData.CAR_INFO.DATA.AD_D0.DATA[7], 
                      "[1] " + to_string_binary(sdSysData.CAR_INFO.DATA.AD_D0.DATA[1]), tmeFrame_Time);
  */

  //DATA_SET_03.PROP.DESCRIPTION = "07 E8";
  //DATA_SET_03.update(sdSysData.CAR_INFO.DATA.AD_7E8, sdSysData.CAR_INFO.DATA.AD_7E8.DATA[3], sdSysData.CAR_INFO.DATA.AD_7E8.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  //DATA_SET_04.PROP.DESCRIPTION = "07 E9";
  //DATA_SET_04.update(sdSysData.CAR_INFO.DATA.AD_7E9, sdSysData.CAR_INFO.DATA.AD_7E9.DATA[3], sdSysData.CAR_INFO.DATA.AD_7E9.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  /*
  DATA_SET_05.update(sdSysData.CAR_INFO.DATA.AD_7E8, sdSysData.CAR_INFO.DATA.AD_7E8.DATA[5], sdSysData.CAR_INFO.DATA.AD_7E8.DATA[6], "", tmeFrame_Time);

  DATA_SET_06.update(sdSysData.CAR_INFO.DATA.AD_7E9, sdSysData.CAR_INFO.DATA.AD_7E9.DATA[5], sdSysData.CAR_INFO.DATA.AD_7E9.DATA[6], "", tmeFrame_Time);
  */

  //DATA_SET_07.PROP.DESCRIPTION = "07 EA";
  //DATA_SET_07.update(sdSysData.CAR_INFO.DATA.AD_7EA, sdSysData.CAR_INFO.DATA.AD_7EA.DATA[3], sdSysData.CAR_INFO.DATA.AD_7EA.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  //DATA_SET_08.PROP.DESCRIPTION = "07 EB";
  //DATA_SET_08.update(sdSysData.CAR_INFO.DATA.AD_7EB, sdSysData.CAR_INFO.DATA.AD_7EB.DATA[3], sdSysData.CAR_INFO.DATA.AD_7EB.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

}

void AUTOMOBILE_SCREEN::display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                                const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    ImGui::Text("Messages: %d", SDATA.MESSAGES);
    ImGui::Text("Speed: %s", SDATA.SPEED.c_str());
    ImGui::Text("Voltage: %s", SDATA.VOLTAGE.c_str());
    ImGui::Text("Unknown: %s", SDATA.LATEST_UNKNOWN_MESSAGE.c_str());
    ImGui::Text("Cruise Speed: %f", SDATA.CRUISE_CONTROL_SET_SPEED);
    ImGui::Text("Acceleration: %f", SDATA.ACCELERATION);
    ImGui::Text("Tach: %s", SDATA.RPM.c_str());
    ImGui::Text("Torque: %s", SDATA.TORQUE.c_str());
    ImGui::Text("Steering Angle : %s", SDATA.STEERING_WHEEL_ANGLE.c_str());
    ImGui::Text("Steering LOC: %s", SDATA.STEERING_WHEEL_LEFT_OF_CENTER.c_str());
    ImGui::Text("Steering Dir: %s", SDATA.STEERING_WHEEL_TURNING_DIRECTION.c_str());
    ImGui::Text("Temp Ambiant: %s", SDATA.TEMP_AMBIANT.c_str());
    ImGui::Text("Temp Intake: %s", SDATA.TEMP_INTAKE.c_str());
    ImGui::Text("Temp Coolant: %s", SDATA.TEMP_COOLANT.c_str());
    ImGui::Text("Temp Catalyst: %s", SDATA.TEMP_CATALYST.c_str());
    ImGui::Text("Temp Super: %f", SDATA.TEMP_SUPER_TEMP);
    ImGui::Text("Pressure Rail: %s", SDATA.FUEL_RAIL_PRESSURE.c_str());
    ImGui::Text("Pressure Vap: %s", SDATA.EVAP_SYSTEM_VAP_PRESSURE.c_str());
    ImGui::Text("Baro: %s", SDATA.BAROMETER.c_str());
  }
  ImGui::End();
}

// ---------------------------------------------------------------------------------------
#endif