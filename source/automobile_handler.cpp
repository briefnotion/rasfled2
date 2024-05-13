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

void AUTOMOBILE_HANDLER::alert(system_data &sdSysData, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime)
{
  if (ALERT_TIMER.ping_down(tmeCurrentTime) == false)
  {
    Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "FLASH");
    ALERT_TIMER.ping_up(tmeCurrentTime, 10000);
  }
}

void AUTOMOBILE_HANDLER::update_events(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, ANIMATION_HANDLER &Animations, unsigned long tmeCurrentTime)
{
  // -------------------------------------------------------------------------------------
  // Automobile Data Switched to Not Available

  // Data Grab to reduce calc times.
  //float speed_trans_mph_impres = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph_impres(tmeCurrentTime);
  float speed_trans_mph_impres = sdSysData.CAR_INFO.CALCULATED.SPEED_ALL_TIRES_AVERAGE.val_mph();

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

      GEAR_PARK = false;
      GEAR_NEUTRAL = false;
      GEAR_REVERSE = false;
      GEAR_DRIVE = false;
      LIGHT_VELOCITY_ON = false;
      LIGHT_DOOR_HANDLE_ON = false;
    }
  }

  // -------------------------------------------------------------------------------------
  // Automobile Data Available
  if (sdSysData.CAR_INFO.active() == true)
  {
    // Check alerts

    // S-Temp alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_TEMP_S_TEMP, 
        sdSysData.CAR_INFO.CALCULATED.s_temp() >= 60.0f, sdSysData.CAR_INFO.CALCULATED.s_temp() < 50.0f))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_TEMP_S_TEMP, "S-Temp Value is " + to_string((int)sdSysData.CAR_INFO.CALCULATED.s_temp()));
    }

    // MIL alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_MIL, 
                                      sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light() == true , 
                                      sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light() == false))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_MIL, "Malfunction Indicator Lamp is " + to_string(sdSysData.CAR_INFO.STATUS.SYSTEM.malfunction_indicator_light()));
    }

    // Coolant alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_TEMP_COOLANT, 
                                      sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c() >= 100.0f, 
                                      sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c() < 80.0f))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_TEMP_COOLANT, "Coolant Temp Value is " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c());
    }

    // Air Intake Temp alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_TEMP_INTAKE, 
                                      sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c() >= sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 20.0f, 
                                      sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c() < sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c() + 15.0f))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_TEMP_INTAKE, "Intake Temp Value is " + sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c());
    }

    // Voltage alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_ELEC_VOLTAGE, 
                                      sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v() < 11.5f, 
                                      sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v() >= 12.0f))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_ELEC_VOLTAGE, "Voltage Value is " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v());
    }

    // Fuel Level alert
    if (sdSysData.ALERTS_2.res_alert_condition(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, RESERVE_ALERT_FUEL_LEVEL, 
                                      sdSysData.CAR_INFO.STATUS.FUEL.val_level() < 1.0f, sdSysData.CAR_INFO.STATUS.FUEL.val_level() > 2.0f))
    {
      sdSysData.ALERTS_2.res_update_alert_text(RESERVE_ALERT_FUEL_LEVEL, "Fuel Level is " + sdSysData.CAR_INFO.STATUS.FUEL.level());
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

    // Shutdown Waring
    if (IGNITION == false && IGNITION_WARNING_TIMER.enabled() && IGNITION_WARNING_TIMER.ping_down(tmeCurrentTime) == false)
    {
      sdSysData.ALERTS_2.add_generic_alert(sdSysData.COMMAND_THREADS, sdSysData.SOUND_SYSTEM, 
                                            "System shutting down in 1 minute.");
    }

    // Shutdown
    if (IGNITION == false && IGNITION_SHUTDOWN_TIMER.enabled() && IGNITION_SHUTDOWN_TIMER.ping_down(tmeCurrentTime) == false)
    {
      cons.command_text_set(" shutdown");
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

    const int activate_speed = 10;
    const int caution_speed = 5;

    // -------------------------------------------------------------------------------------
    // Alerts at non stops at low speed
    
    // Alert if door is open
    if (speed_trans_mph_impres > 0 && 
        speed_trans_mph_impres < 5)
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
    if (speed_trans_mph_impres > activate_speed + 6)
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
    else if (speed_trans_mph_impres < activate_speed + 4)
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

      if (set_bool_with_change_notify((GEAR_DRIVE == true || GEAR_NEUTRAL == true), LIGHT_VELOCITY_ON) == true)
      {
        if (LIGHT_VELOCITY_ON == true)
        {
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_On");
        }
        else
        {
          Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "Automobile - Velocity_Off");
        }
      }

      // Set colors if Velocity lights are on.  They could be off if in reverse or park.
      if (LIGHT_VELOCITY_ON == true)
      {
        // Multiplier
        if (speed_trans_mph_impres < activate_speed)
        {
          multiplier = 1 - speed_trans_mph_impres / activate_speed;
        }
        else 
        {
          multiplier = 0;
        }
        
        // Caution Multiplier
        if (speed_trans_mph_impres < caution_speed)
        {
          multiplier_caution = 1 - speed_trans_mph_impres / caution_speed;
        }
        else 
        {
          multiplier_caution = 0;
        }

        CRGB drive_color = CRGB(32, (int)(16 * multiplier_caution), 0);
        CRGB velocity_color = CRGB(8, 0, 0);
        CRGB drive_pulse_color = CRGB(30, 30, 30);

        // Adjust light colors and atttributes
        // Gear
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_O", drive_color.brightness(multiplier));
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_D", drive_color.brightness(multiplier));

        // Velocity
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_D", velocity_color.brightness(multiplier));
        Animations.mod_run_anim_color_dest_1("AUGEAR_VELOCITY_O", velocity_color.brightness(multiplier));

        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_D", speed_trans_mph_impres);
        Animations.mod_run_anim_velocity("AUGEAR_VELOCITY_O", speed_trans_mph_impres);

        // Keep?
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_PULSE", drive_pulse_color.brightness(multiplier));
        Animations.mod_run_anim_color_dest_1("AUGEAR_DRIVE_PULSE", drive_pulse_color.brightness(multiplier));
      }
    }
  }
}

// -------------------------------------------------------------------------------------

#endif
