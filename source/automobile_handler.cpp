// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: automobile_handler.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTOMOBILE_HANDLER_CPP
#define AUTOMOBILE_HANDLER_CPP

#include "automobile_handler.h"

using namespace std;

// -------------------------------------------------------------------------------------

void AUTOMOBILE_HANDLER::running_temperature_color_set(system_data &sdSysData, float S_Temp)
{
  // Simple temp color set.
  int rank = 0;

  if (S_Temp < 0.0f)
  {
    rank = 0;
  }
  else if (S_Temp < 10.0f)
  {
    rank = 1;
  }
  else if (S_Temp < 15.0f)
  {
    rank = 2;
  }
  else if (S_Temp < 20.0f)
  {
    rank = 3;
  }
  else if (S_Temp < 25.0f)
  {
    rank = 4;
  }
  else if (S_Temp < 30.0f)
  {
    rank = 5;
  }
  else if (S_Temp < 35.0f)
  {
    rank = 6;
  }
  else if (S_Temp < 40.0f)
  {
    rank = 7;
  }
  else if (S_Temp < 45.0f)
  {
    rank = 8;
  }
  else if (S_Temp < 50.0f)
  {
    rank = 9;
  }
  else if (S_Temp < 55.0f)
  {
    rank = 10;
  }
  else if (S_Temp < 60.0f)
  {
    rank = 11;
  }
  else if (S_Temp < 65.0f)
  {
    rank = 12;
  }
  else if (S_Temp < 70.0f)
  {
    rank = 13;
  }
  else if (S_Temp < 75.0f)
  {
    rank = 14;
  }
  else
  {
    rank = 15;
  }

  if (rank != running_temperature_color_rank)
  {
    running_temperature_color_rank = rank;
    
    sdSysData.PANEL_CONTROL.COLOR_SELECT.set_neo_duration(10000.0f);
    VELOCITY_COLOR.PROPS.DURATION = 10000.0f;

    switch (running_temperature_color_rank)
    {
      case 0:     // < 0
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), RAS_GREY);
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_GREY).SIMPLE_RGB);
        break;
      }
      case 1:     // 0 - 5
      { // Cyan
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.0f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));
        break;
      }
      case 2:     // 5 - 10
      { // White
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
        break;
      }
      case 3:     // 10 - 15
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.66f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.66f, 1.0f, 1.0f, 1.0f)));
        break;
      }
      case 4:     // 15 - 20
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.33f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity()); //
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.33f, 1.0f, 1.0f, 1.0f))); //
        break;
      }
      case 5:     // 20 - 25
      { // Cyan
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.0f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity()); //
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.0f, 1.0f, 1.0f, 1.0f))); //
        break;
      }
      case 6:     // 25 - 30
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.0f, 1.0f, 0.66f, sdSysData.PANEL_CONTROL.color_current_intensity()); //
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.0f, 1.0f, 0.66f, 1.0f))); //
        break;
      }
      case 7:     // 30 - 35
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.0f, 1.0f, 0.33f, sdSysData.PANEL_CONTROL.color_current_intensity()); // 
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.0f, 1.0f, 0.33f, 1.0f))); //
        break;
      }
      case 8:     // 35 - 40
      { // Green
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.0f, 1.0f, 0.0f, sdSysData.PANEL_CONTROL.color_current_intensity()); // G
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.0f, 1.0f, 0.0f, 1.0f))); // G
        break;
      }
      case 9:     // 40 - 45
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.33f, 1.0f, 0.0f, sdSysData.PANEL_CONTROL.color_current_intensity()); //
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.33f, 1.0f, 0.0f, 1.0f))); //
        break;
      }
      case 10:     // 45 - 50
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 0.66f, 1.0f, 0.0f, sdSysData.PANEL_CONTROL.color_current_intensity()); //
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(0.66f, 1.0f, 0.0f, 1.0f))); // 
        break;
      }
      case 11:     // 50 - 55
      {// Yellow
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 1.0f, 0.0f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 1.0f, 0.0f, 1.0f)));
        break;
      }
      case 12:     // 55 - 60
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 1.0f, 0.33f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 1.0f, 0.33f, 1.0f)));
        break;
      }
      case 13:     // 60 - 65
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 1.0f, 0.66f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 1.0f, 0.66f, 1.0f)));
        break;
      }
      case 14:     // 65 - 70
      { // White
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 1.0f, 1.0f, sdSysData.PANEL_CONTROL.color_current_intensity());
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
        break;
      }
      case 15:     // 70 +
      {
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f, 0.0f, 1.0f, 1.0f);
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), convert_imvec4_to_crgb(ImVec4(1.0f, 0.0f, 1.0f, 0.5f)));
        break;
      }
      default:
      { // Purple
        sdSysData.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSysData.PROGRAM_TIME.current_frame_time(), RAS_MONOCHROME);
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_MONOCHROME).SIMPLE_RGB);
        break;
      }
    }
  }
}

void AUTOMOBILE_HANDLER::alert(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime)
{
  if (ALERT_TIMER.ping_down(tmeCurrentTime) == false)
  {
    Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "FLASH");
    ALERT_TIMER.ping_up(tmeCurrentTime, 10000);
  }
}

void AUTOMOBILE_HANDLER::update_events(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime)
{
  // -------------------------------------------------------------------------------------
  // Automobile Data Switched to Not Available

  // Check for changes in the automobile availability
  if ((set_bool_with_change_notify(sdSysData.CAR_INFO.active(), AUTO_ACTIVE) == true))
  {
    if (sdSysData.CAR_INFO.active() == false)
    {
      // if automobile is no longer available, make sure all related lights are off.
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Park_Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Neutral_Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Reverse_Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Door Handle Running Off");
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Allways_On_Off");

      GEAR_PARK = false;
      GEAR_NEUTRAL = false;
      GEAR_REVERSE = false;
      GEAR_DRIVE = false;
      LIGHT_VELOCITY_ON = false;
      LIGHT_DOOR_HANDLE_ON = false;
    }
    else
    {
      // Turn on always on animations when automobile is available.
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Allways_On_LF", 1);
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Allways_On_RF", 3);
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Allways_On_LB", 0);
      Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Allways_On_RB", 2);
    }
  }

  // -------------------------------------------------------------------------------------
  // Automobile Data Available
  if (sdSysData.CAR_INFO.active() == true)
  {
    CHANGED = true;

    // Update neo_color
    VELOCITY_COLOR.set_current_frame_time(sdSysData.PROGRAM_TIME.current_frame_time());

    // Variable Reuse
    float speed_lowest_tire_speed = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_ALL_TIRES_LOWEST.val_mph();
    float speed_average_tire_speed = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_ALL_TIRES_AVERAGE.val_mph();

    float s_temp = sdSysData.CAR_INFO.CALCULATED.s_temp();

    // Velocity - Must always get.
    // Gather data for tire speed for velocity animations.
    float adjustment = 1.0f;  // Simple way to adjust the speed of the animations.

    float speed_tire_LF = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_LF_TIRE.val_mph() * adjustment;
    float speed_tire_RF = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_RF_TIRE.val_mph() * adjustment;
    float speed_tire_LB = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_LB_TIRE.val_mph() * adjustment;
    float speed_tire_RB = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_RB_TIRE.val_mph() * adjustment;

    // Speed Testing
    //speed_tire_LF = 1.0f;
    //speed_tire_RF = 1.0f;
    //speed_tire_LB = 1.0f;
    //speed_tire_RB = 1.0f;

    // Difererential
    // Create speed differentials for each tire.
    float speed_tire_LF_diff = speed_tire_LF - speed_lowest_tire_speed;
    float speed_tire_RF_diff = speed_tire_RF - speed_lowest_tire_speed;
    float speed_tire_LB_diff = speed_tire_RB - speed_lowest_tire_speed;
    float speed_tire_RB_diff = speed_tire_LB - speed_lowest_tire_speed;

    // Differential - Animations
    Animations.mod_run_anim_velocity("AUTO_DIFFERENTIAL_O_LF", speed_tire_LF_diff);
    Animations.mod_run_anim_velocity("AUTO_DIFFERENTIAL_O_RF", speed_tire_RF_diff);
    Animations.mod_run_anim_velocity("AUTO_DIFFERENTIAL_O_LB", speed_tire_LB_diff);
    Animations.mod_run_anim_velocity("AUTO_DIFFERENTIAL_O_RB", speed_tire_RB_diff);

    // Check alerts





/*
    // S-Temp alert
    if (sdSysData.ALL_ALERTS.res_alert_condition_greater_than(AUTO_RESERVE_ALERT_TEMP_S_TEMP, 
        s_temp, 65.0f, 50.0f))
    {
      sdSysData.ALL_ALERTS.res_update_alert_text_line(AUTO_RESERVE_ALERT_TEMP_S_TEMP, "S-Temp Value is " + to_string((int)s_temp));
      sdSysData.ALL_ALERTS.res_update_additional_line_with_conditions(AUTO_RESERVE_ALERT_TEMP_S_TEMP);
      sdSysData.ALL_ALERTS.ALERTS_RESERVE[AUTO_RESERVE_ALERT_TEMP_S_TEMP].set_show_value_bar(true);
    }
*/





    // S-Temp Color Set
    if (sdSysData.RUNNING_COLOR == -1)
    {
      running_temperature_color_set(sdSysData, s_temp);
    }
    else
    {
      sdSysData.PANEL_CONTROL.COLOR_SELECT.set_neo_duration(2000.0f);
      VELOCITY_COLOR.PROPS.DURATION = 2000.0f;

      if (sdSysData.RUNNING_COLOR == RAS_RED || sdSysData.RUNNING_COLOR == RAS_BLUE)
      {
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_BLACK).SIMPLE_RGB);
      }
      else
      {
        VELOCITY_COLOR.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(sdSysData.RUNNING_COLOR).SIMPLE_RGB);
      }

    }





    /*
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

    */

    
    // Headlights Changed On and Off
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_headlights_on(), LIGHTS_HEADLIGHTS) == true)
    {
      // If lights are on, set screen brightness
      if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_headlights_on())
      {

        if (sdSysData.GPS_SYSTEM.active(sdSysData.PROGRAM_TIME.current_frame_time()))
        {
          // Only change the brightness once per minute
          if (SCREEN_BRIGHTNESS_CHANGE.is_ready(tmeCurrentTime))
          {
            SCREEN_BRIGHTNESS_CHANGE.set(tmeCurrentTime, 60000);

            // Set Screen brightness (automaticlly) to level indicated by sun position and gps location.
            //    50% + (50% * brightnes) --> 100% to 50%
            sdSysData.PANEL_CONTROL.color_set_automatic_intensity(tmeCurrentTime, 0.50f + (0.50f * 
                                                    getCurrentDaylightFactor((float)sdSysData.GPS_SYSTEM.current_position().LATITUDE, 
                                                                              (float)sdSysData.GPS_SYSTEM.current_position().LONGITUDE))
                                                                );
          }
        }
        else
        {
          // if gps not available, set to 50% if lights are on.
          sdSysData.PANEL_CONTROL.color_set_automatic_intensity(tmeCurrentTime, 0.50f);
        }
      }
      else
      {
        // Set Screen brightness (automaticlly) to 100%.
        sdSysData.PANEL_CONTROL.color_set_automatic_intensity(tmeCurrentTime, 1.00f);
      }
    }

    // Cruese Control On Off
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control(), CUISE_CONTROL) == true)
    {
      if (sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control())
      {
        sdSysData.SOUND_SYSTEM.add_note_to_queue(sdSysData.COMMAND_THREADS, "g6");
      }
      else
      {
        sdSysData.SOUND_SYSTEM.add_note_to_queue(sdSysData.COMMAND_THREADS, "f6");
      }
    }

    // Gear Selection
    // Changing Gear to Park
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_park(), GEAR_PARK) == true)
    {
      if (GEAR_PARK == true)
      {
        sdSysData.SOUND_SYSTEM.play_gear_park(sdSysData.COMMAND_THREADS);
        
        // Call animation to turn on Park color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Park_On");
      }
      else
      {
        // Call animation to turn off Park color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Park_Off");        
      }
    }
    
    // Changing Gear to Neutral
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_neutral(), GEAR_NEUTRAL) == true)
    {
      if (GEAR_NEUTRAL == true)
      {
        sdSysData.SOUND_SYSTEM.play_gear_neutral(sdSysData.COMMAND_THREADS);

        // Call animation to turn on Neutral color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Neutral_On");
      }
      else
      {
        // Call animation to turn off Neutral color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Neutral_Off");        
      }
    }

    // Changing Gear to Reverse
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_reverse(), GEAR_REVERSE) == true)
    {
      if (GEAR_REVERSE == true)
      {
        sdSysData.SOUND_SYSTEM.play_gear_reverse(sdSysData.COMMAND_THREADS);

        // Call animation to turn on Reverse color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Reverse_On");
      }
      else
      {
        // Call animation to turn off Reverse color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Reverse_Off");        
      }
    }

    // Changing Gear to Drive
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_drive(), GEAR_DRIVE) == true)
    {
      if (GEAR_DRIVE == true)
      {
        sdSysData.SOUND_SYSTEM.play_gear_drive(sdSysData.COMMAND_THREADS);
        
        // Call animation to turn on Drive color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_On");
      }
      else
      {
        // Call animation to turn off Drive color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_Off");
      }
    }

    // Changing Gear to Low
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_low(), GEAR_LOW) == true)
    {
      if (GEAR_LOW == true)
      {
        sdSysData.SOUND_SYSTEM.play_gear_low(sdSysData.COMMAND_THREADS);
        
        // Call animation to turn on Drive color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_On");
      }
      else
      {
        // Call animation to turn off Drive color.
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_Off");
      }
    }

    // Signal Lights
    // Left
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left(), SIGNAL_LEFT))
    {
      if (SIGNAL_LEFT)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Left_On", 0);
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Left_On", 1);
      }
      else
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Left_Off");
        SIGNAL_OFF_REDUNDANCY.ping_up(tmeCurrentTime, 2000);
      }
    }

    // Right
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right(), SIGNAL_RIGHT))
    {
      if (SIGNAL_RIGHT)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Right_On", 2);
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Right_On", 3);
      }
      else
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Right_Off");
        SIGNAL_OFF_REDUNDANCY.ping_up(tmeCurrentTime, 2000);
      }
    }

    // Hazards
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards(), HAZARDS))
    {
      if (HAZARDS)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "HAZARD");
        sdSysData.booHazardRunning = true;
      }
      else
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "HAZARD STOP");
        sdSysData.booHazardRunning = false;
      }
    }

    // Ignition Shutdown
    if (set_bool_with_change_notify(sdSysData.CAR_INFO.STATUS.INDICATORS.val_ignition() , IGNITION))
    {
      if (IGNITION == false)
      {
        IGNITION_WARNING_TIMER.ping_up(tmeCurrentTime, 9 * 60000);
        IGNITION_SHUTDOWN_TIMER.ping_up(tmeCurrentTime, 10 * 60000);
      }
    }

    // Shutdown Warning
    if (IGNITION == false && IGNITION_WARNING_TIMER.enabled() && IGNITION_WARNING_TIMER.ping_down(tmeCurrentTime) == false)
    {
      sdSysData.ALL_ALERTS.add_generic_alert("POWER", "System shutting down in\n1 minute.");
    }

    // Shutdown
    if (IGNITION == false && IGNITION_SHUTDOWN_TIMER.enabled() && IGNITION_SHUTDOWN_TIMER.ping_down(tmeCurrentTime) == false)
    {
      sdSysData.SCREEN_COMMS.command_text_set(" shutdown");
    }

    // Signal Off Reduncancy - because saw a stuck signal in testing.
    // Sends Signal_XXXXX_Off andimation to all channels after 2 second delay if both signals are off.
    if (SIGNAL_OFF_REDUNDANCY.enabled()== true && SIGNAL_LEFT == false && SIGNAL_RIGHT == false)
    {
      if (SIGNAL_OFF_REDUNDANCY.ping_down(tmeCurrentTime) == false)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Left_Off");
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Signal_Right_Off");
      }
    }

    // -------------------------------------------------------------------------------------
    // While Driving
    float multiplier = 0;
    float multiplier_caution = 0;

    // Tire Values
    float brightness_LF       = 0;
    float brightness_LF_slow  = 0;
    float brightness_RF       = 0;
    float brightness_RF_slow  = 0;
    float brightness_LB       = 0;
    float brightness_RB       = 0;

    // -------------------------------------------------------------------------------------
    // Alerts at non stops at low speed
    
    // Alert if door is open
    if (speed_average_tire_speed > 0 && 
        speed_average_tire_speed < 5)
    {
      if (sdSysData.CAR_INFO.STATUS.DOORS.hatchback_door_open() == true ||
          sdSysData.CAR_INFO.STATUS.DOORS.hood_door_open() == true ||
          sdSysData.CAR_INFO.STATUS.DOORS.lb_door_open() == true ||
          sdSysData.CAR_INFO.STATUS.DOORS.lf_door_open() == true ||
          sdSysData.CAR_INFO.STATUS.DOORS.rb_door_open() == true ||
          sdSysData.CAR_INFO.STATUS.DOORS.rf_door_open() == true)
      {
        alert(sdSysData, Animations, tmeCurrentTime);
      }

      // Alert if only parking lights are on
      if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_pos() == 1)
      {
          alert(sdSysData, Animations, tmeCurrentTime);
      }
    }

    // -------------------------------------------------------------------------------------
    // Lights
    
    // Turn off lights if speed over set value.
    if (speed_lowest_tire_speed > ACTIVATE_SPEED + 6)
    {
      if (LIGHT_DOOR_HANDLE_ON == true)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Door Handle Running Off");
        LIGHT_DOOR_HANDLE_ON = false;
      }

      if (LIGHT_DRIVE_ON == true)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_Off");
        LIGHT_DRIVE_ON = false;
      }

      if (LIGHT_VELOCITY_ON == true)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_Off");
        LIGHT_VELOCITY_ON = false;
      }
    }

    // -------------------------------------------------------------------------------------
    // Turn on lights if speed under set value.
    else if (speed_lowest_tire_speed < ACTIVATE_SPEED + 4)
    {
      if (LIGHT_DOOR_HANDLE_ON == false)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Door Handle Running On");
        LIGHT_DOOR_HANDLE_ON = true;
      }

      if (LIGHT_DRIVE_ON == false)
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Gear Select_Drive_On");
        LIGHT_DRIVE_ON = true;
      }

      // Velocity Sweeping Lights
      if (set_bool_with_change_notify((GEAR_DRIVE == true || GEAR_NEUTRAL == true), LIGHT_VELOCITY_ON) == true)
      {
        if (LIGHT_VELOCITY_ON == true)
        {
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_On_LF", 1);
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_On_RF", 3);
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_On_LB", 0);
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_On_RB", 2);
        }
        else
        {
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_Off");
        }
      }

      // Set colors if Velocity lights are on.  They could be off if in reverse or park.
      if (LIGHT_VELOCITY_ON == true)
      {
        // ---------------------------------------------
        // Calculations
        // Keep velocity animations running unless all tires have stopped.
        bool all_tires_stopped = (speed_tire_LF == 0.0f && speed_tire_RF == 0.0f && 
                                  speed_tire_LB == 0.0f && speed_tire_RB == 0.0f);

        if (all_tires_stopped == false)
        {
          if (speed_tire_LF == 0.0f)
          {
            speed_tire_LF = 0.1f;
          }
          if (speed_tire_RF == 0.0f)
          {
            speed_tire_RF = 0.1f;
          }
          if (speed_tire_LB == 0.0f)
          {
            speed_tire_LB = 0.1f;
          }
          if (speed_tire_RB == 0.0f)
          {
            speed_tire_RB = 0.1f;
          }
        }

        // Multiplier
        if (speed_average_tire_speed < ACTIVATE_SPEED)
        {
          multiplier = 1 - speed_average_tire_speed / ACTIVATE_SPEED;
        }
        else 
        {
          multiplier = 0;
        }

        // Multiplier for Individual Tire Speeds
        
        // LF
        if (speed_tire_LF < ACTIVATE_SPEED)
        {
          brightness_LF = 1 - (speed_tire_LF / ACTIVATE_SPEED);
        }
        else 
        {
          brightness_LF = 0;
        }

        // LF Slow
        if (speed_tire_LF < 2.0f)
        {
          brightness_LF_slow = 1 - (speed_tire_LF / 2.0f);
        }
        else 
        {
          brightness_LF_slow = 0;
        }

        // RF
        if (speed_tire_RF < ACTIVATE_SPEED)
        {
          brightness_RF = 1 - (speed_tire_RF / ACTIVATE_SPEED);
        }
        else 
        {
          brightness_RF = 0;
        }

        // RF Slow
        if (speed_tire_RF < 2.0f)
        {
          brightness_RF_slow = 1 - (speed_tire_RF / 2.0f);
        }
        else 
        {
          brightness_RF_slow = 0;
        }

        // LB
        if (speed_tire_LB < ACTIVATE_SPEED)
        {
          brightness_LB = 1 - (speed_tire_LB / ACTIVATE_SPEED);
        }
        else 
        {
          brightness_LB = 0;
        }

        // RB
        if (speed_tire_RB < ACTIVATE_SPEED)
        {
          brightness_RB = 1 - (speed_tire_RB / ACTIVATE_SPEED);
        }
        else 
        {
          brightness_RB = 0;
        }
        
        // Caution Multiplier based on Trasmission Speed
        if (speed_average_tire_speed < CAUTION_SPEED)
        {
          multiplier_caution = 1 - speed_average_tire_speed / CAUTION_SPEED;
        }
        else 
        {
          multiplier_caution = 0;
        }
      
        // ---------------------------------------------
        // Calculations done, call anims

        CRGB drive_color = CRGB(32, (int)(16 * multiplier_caution), 0);
        // Adjust light colors and atttributes
        // Gear
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_O", drive_color.brightness(multiplier));
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_D", drive_color.brightness(multiplier));

        // Velocity
        // LF
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_D_LF", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_LF * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_LF",
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_LF * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_LF_SLOW",
                                              (CRGB(10,10,10))
                                              .brightness(brightness_LF_slow));

        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_D_LF", speed_tire_LF * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_LF", speed_tire_LF * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_LF_SLOW", speed_tire_LF * 10.0f * 0.5f);
        
        // RF
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_D_RF", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_RF * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_RF", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_RF * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_RF_SLOW", 
                                              (CRGB(10,10,10))
                                              .brightness(brightness_RF_slow));

        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_D_RF", speed_tire_RF * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_RF", speed_tire_RF * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_RF_SLOW", speed_tire_RF * 10.0f * 0.5f);
        
        // LB
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_D_LB", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_LB * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_LB", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_LB * 0.04f));

        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_D_LB", speed_tire_LB * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_LB", speed_tire_LB * 0.5f);
        
        // RB
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_D_RB", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_RB * 0.04f));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O_RB", 
                                              (VELOCITY_COLOR.color())
                                              .brightness(brightness_RB * 0.04f));

        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_D_RB", speed_tire_RB * 0.5f);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O_RB", speed_tire_RB * 0.5f);
        
        // Keep?
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_PULSE", DRIVE_PULSE_COLOR.brightness(multiplier));
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_PULSE", DRIVE_PULSE_COLOR.brightness(multiplier));
      }
    }
  }
}

// -------------------------------------------------------------------------------------

#endif
