// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: auto.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTO_CPP
#define AUTO_CPP

#include "auto.h"

// -------------------------------------------------------------------------------------
//  Functions

int method_temp_1(char Byte_1)
{
  return Byte_1 - 40;
}

int method_2_byte(char Byte_1, char Byte_2)
{
  return ((Byte_1 * 256) + Byte_2);
}

float method_2_byte_float(char Byte_1, char Byte_2)
{
  return (((float)Byte_1 * 256) + (float)Byte_2);
}

float method_2_byte_div_1000(char Byte_1, char Byte_2)
{
  return (((float)Byte_1 * 256) + (float)Byte_2) / 1000;
}

// -------------------------------------------------------------------------------------
//  Classs

bool check_availability(bool Coded_Availability, bool Source_Availabilty)
{
  if (Coded_Availability == true && Source_Availabilty == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//-----------

void BYTE_VALUE_8::store_value(int Value_0, int Value_1, int Value_2, int Value_3, 
                                  int Value_4, int Value_5, int Value_6, int Value_7)
{
  VALUE_0 = Value_0;
  VALUE_1 = Value_1;
  VALUE_2 = Value_2;
  VALUE_3 = Value_3;
  VALUE_4 = Value_4;
  VALUE_5 = Value_5;
  VALUE_6 = Value_6;
  VALUE_7 = Value_7;
}

int BYTE_VALUE_8::val_0()
{
  return VALUE_0;
}

int BYTE_VALUE_8::val_1()
{
  return VALUE_1;
}

int BYTE_VALUE_8::val_2()
{
  return VALUE_2;
}

int BYTE_VALUE_8::val_3()
{
  return VALUE_3;
}

int BYTE_VALUE_8::val_4()
{
  return VALUE_4;
}

int BYTE_VALUE_8::val_5()
{
  return VALUE_5;
}

int BYTE_VALUE_8::val_6()
{
  return VALUE_6;
}

int BYTE_VALUE_8::val_7()
{
  return VALUE_7;
}

//-----------

void TEMPERATURE::store_c(int Celsius)
{
  C = (float)Celsius;
  F = (float)temperture_translate_celcius_to_fahrenheit(Celsius);
}

void TEMPERATURE::store_c(float Celsius)
{
  C = Celsius;
  F = temperture_translate_celcius_to_fahrenheit(Celsius);
}

float TEMPERATURE::val_c()
{
  return C;
}

float TEMPERATURE::val_f()
{
  return F;
}

string TEMPERATURE::c()
{
  return to_string((int)C) + "c";
}

string TEMPERATURE::f()
{
  return to_string((int)F) + "f";
}

//-----------

void TIME::store_seconds(int Seconds)
{
  SECONDS = Seconds;
}

int TIME::val_seconds()
{
  return SECONDS;
}

string TIME::seconds()
{
  return to_string(SECONDS) + " s";
}

//-----------

void PRESSURE::store_kPa(int kPa)
{
  PA  = 1000 * kPa;
}

void PRESSURE::store_kPa(float kPa)
{
  PA  = 1000 * kPa;
}

void PRESSURE::store_Pa(int Pa)
{
  PA  = Pa;
}

void PRESSURE::store_Pa(float Pa)
{
  PA  = (float)Pa;
}

float PRESSURE::val_kPa()
{
  return PA / 1000;
}

string PRESSURE::kPa()
{
  return to_string((int)(PA / 1000)) + " kPa";
}

float PRESSURE::val_Pa()
{
  return PA;
}

string PRESSURE::Pa()
{
  return to_string((int)PA) + " Pa";
}

float PRESSURE::val_inHg()
{
  return pressure_translate_kPa_to_inHg(PA / 1000);
}

string PRESSURE::inHg()
{
  return to_string_round_to_nth(pressure_translate_kPa_to_inHg(PA / 1000), 2) + " inHg";
}

//-----------

void VOLTAGE::store_v(float Voltage)
{
  V  = Voltage;
}

float VOLTAGE::val_v()
{
  return V;
}

string VOLTAGE::v()
{
  return to_string_round_to_nth(V, 1) + " v";
}

//-----------

void AUTOMOBILE_ACCELERATOR::store_value(int A)
{
  VAL_VALUE = A;
}

float AUTOMOBILE_ACCELERATOR::val_value()
{
  return (float) VAL_VALUE;
}

//-----------

void AUTOMOBILE_BRAKE::store_value(int A, int B)
{
  VAL_VALUE = (A * 256) + B;
}

float AUTOMOBILE_BRAKE::val_value()
{
  return (float) VAL_VALUE;
}

//-----------

void AUTOMOBILE_SYSTEM::store_malfunction_indicator_light(bool Value)
{
  MALFUNCTION_INDICATOR_LIGHT = Value;
}

bool AUTOMOBILE_SYSTEM::malfunction_indicator_light()
{
  return MALFUNCTION_INDICATOR_LIGHT;
}

//-----------

void AUTOMOBILE_DOORS::set_source_availability(bool Available)
{
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_DOORS::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

bool AUTOMOBILE_DOORS::store(int Data)
{
  // AD_360 also contains info on the engine running status  

  // RB - Door
  if (Data > 0)   // One of the other bytes in 03 60 is for validation, but not checking.
  {
    LF_DOOR_OPEN = !get_bit_value(Data, 1);
    RF_DOOR_OPEN = !get_bit_value(Data, 2);
    LB_DOOR_OPEN = !get_bit_value(Data, 4);
    RB_DOOR_OPEN = !get_bit_value(Data, 8);

    HATCHBACK_DOOR_OPEN = !get_bit_value(Data, 16);
    HOOD_DOOR_OPEN = !get_bit_value(Data, 32);

    return false;
  }
  else
  {
    // Insert Error Check Here
    //return true;
    return true;
  }

  //1[2]
  //MOONROOF_DOOR_OPEN = get_bit_value(Data, 8);
}

bool AUTOMOBILE_DOORS::store_from_alt_source(int Door, bool Value)
{
  TRUTH_CATCH ret_changed;
  if (Door == 0)
  {
    ret_changed.catch_truth(set_bool_with_change_notify(Value, LB_DOOR_OPEN));
  }
  else if (Door == 1)
  {
    ret_changed.catch_truth(set_bool_with_change_notify(Value, LF_DOOR_OPEN));
  }
  else if (Door == 2)
  {
    ret_changed.catch_truth(set_bool_with_change_notify(Value, RB_DOOR_OPEN));
  }
  else if (Door == 3)
  {
    ret_changed.catch_truth(set_bool_with_change_notify(Value, RF_DOOR_OPEN));
  }
  return ret_changed.has_truth();
}

bool AUTOMOBILE_DOORS::lf_door_open()
{
  return LF_DOOR_OPEN;
}

bool AUTOMOBILE_DOORS::rf_door_open()
{
  return RF_DOOR_OPEN;
}

bool AUTOMOBILE_DOORS::lb_door_open()
{
  return LB_DOOR_OPEN;
}

bool AUTOMOBILE_DOORS::rb_door_open()
{
  return RB_DOOR_OPEN;
}

bool AUTOMOBILE_DOORS::hatchback_door_open()
{
  return HATCHBACK_DOOR_OPEN;
}

/*
bool AUTOMOBILE_DOORS::moonroof_door_open()
{
  return MOONROOF_DOOR_OPEN;
}
*/

bool AUTOMOBILE_DOORS::hood_door_open()
{
  return HOOD_DOOR_OPEN;
}

//-----------

void AUTOMOBILE_GUAGES::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    COOLANT = -1;
    COOLANT_DISP = "X";
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_GUAGES::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_GUAGES::store_coolant(int Value)
{
  COOLANT = Value;
  COOLANT_DISP = to_string(COOLANT) + " c";
}

int AUTOMOBILE_GUAGES::val_coolant()
{
  return COOLANT;
}

string AUTOMOBILE_GUAGES::coolant()
{
  return COOLANT_DISP;
}

//-----------

void AUTOMOBILE_FUEL::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    CONSUMED = -1;
    CONSUMED_DISP = "X";

    PERCENTAGE = -1;
    PERCENTAGE_DISP = "X";

    LEVEL_RAW = -1;
    LEVEL_DISP = "X";
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_FUEL::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_FUEL::store_consumed(int Consumed)
{
  CONSUMED = ((float)Consumed) / 20;

  CONSUMED_DISP = to_string_round_to_nth(CONSUMED, 2) + " gal";
}

void AUTOMOBILE_FUEL::store_percentage(int Percentage)
{
  PERCENTAGE = ((float)Percentage * 100) / 256 ;

  PERCENTAGE_DISP = to_string_round_to_nth(PERCENTAGE, 1) + " %%";
}

void AUTOMOBILE_FUEL::store_level(int Level)
{
  LEVEL_RAW = ((float)Level) / 20;
  LEVEL_DISP = to_string_round_to_nth(LEVEL_RAW, 1) + " gal";
  LEVEL_RAW_CHANGED = true;
}

void AUTOMOBILE_FUEL::store_fuel_rail_pressure_23(int A, int B)
{
  //FUEL_RAIL_PRESSURE.store_kPa(10 * (method_2_byte(A, B)));
  FUEL_RAIL_PRESSURE.store_kPa((method_2_byte(A, B)));
}

void AUTOMOBILE_FUEL::store_evap_system_vap_pressure_32(int A, int B)
{
  //A and B is two complement signed
  EVAP_SYSTEM_VAP_PRESSURE.store_Pa((two_byte_complement_signed(A, B)) / 4);
}

float AUTOMOBILE_FUEL::val_consumed()
{
  return CONSUMED;
}

string AUTOMOBILE_FUEL::consumed()
{
  return CONSUMED_DISP;
}

float AUTOMOBILE_FUEL::val_percentage()
{
  return PERCENTAGE;
}

string AUTOMOBILE_FUEL::percentage()
{
  return PERCENTAGE_DISP;
}

float AUTOMOBILE_FUEL::val_level_raw()
{
  return LEVEL_RAW;
}

string AUTOMOBILE_FUEL::level_raw()
{
  return LEVEL_DISP;
}

//-----------

void AUTOMOBILE_INDICATORS::process()
{
  // Condition Headlights On
  if (LIGHTS_HIGH_BEAM_ON == true || LIGHTS_POS == 2 || (LIGHTS_POS == 3 && LIGHTS_ON == true))
  {
    LIGHTS_HEADLIGHTS_ON = true;
  }
  else
  {
    LIGHTS_HEADLIGHTS_ON = false;
  }

  // Condition Parking Lights On
  if (LIGHTS_POS == 1 || LIGHTS_POS == 2 || (LIGHTS_POS == 3 && LIGHTS_ON == true))
  {
    LIGHTS_PARKING_ON = true;
  }
  else
  {
    LIGHTS_PARKING_ON = false;
  }
}

void AUTOMOBILE_INDICATORS::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    LIGHTS_POS = -1;
    LIGHTS_DESC = "X";

    PARKING_BRAKE_DESC = "X";

    CRUISE_CONTROL_SPEED = -1;
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_INDICATORS::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_INDICATORS::store_lights(int Lights)
{
  // Parse Lights
  if (Lights == 0)
  {
    LIGHT_SWITCH = false;
    LIGHTS_POS = 0;
    LIGHTS_DESC = "OFF";
  }
  else if (Lights == 1)
  {
    LIGHT_SWITCH = true;
    LIGHTS_POS = 1;
    LIGHTS_DESC = "PARKING";
  }
  else if (Lights == 2)
  {
    LIGHT_SWITCH = true;
    LIGHTS_POS = 2;
    LIGHTS_DESC = "ON";
  }
  else if (Lights == 3)
  {
    LIGHT_SWITCH = true;
    LIGHTS_POS = 3;
    LIGHTS_DESC = "AUTO";
  }
  else
  {
    LIGHT_SWITCH = false;
    LIGHTS_POS = Lights;
    LIGHTS_DESC = "UNKNOWN";
  }
}

void AUTOMOBILE_INDICATORS::store_lights_high_beam(int Data_1)
{
  LIGHTS_HIGH_BEAM_ON = get_bit_value(Data_1, 32);
}

void AUTOMOBILE_INDICATORS::store_on(int Light_Bits)
{
  // Lights Actually On      1st bit
  //  c8 [3] 	b1 lights on	10110001
	//          b0 lights off 10110000
  LIGHTS_ON = get_bit_value(Light_Bits, 1);

  // Parse Parking Brake      7th bit
  // B0 or B1 - OFF 10110000
  // F0 or F1 - On  11110000
  // bit
  if (get_bit_value(Light_Bits, 64) == false)
  {
    PARKING_BRAKE = false;
    PARKING_BRAKE_DESC = "PARKING BRAKE OFF";
  }
  else
  {
    PARKING_BRAKE = true;
    PARKING_BRAKE_DESC = "PARKING BRAKE ON";
  }
}

void AUTOMOBILE_INDICATORS::store_ignition(int Ignition)
{
  // Parse Ignition
  if (bit_value(Ignition, 5))
  {
    IGNITION = true;
  }
  else
  {
    IGNITION = false;
  }
}

void AUTOMOBILE_INDICATORS::store_cruise_control(int Data_1, int Data_2, float Multiplier)
{
  if (Data_1 == 194 || Data_1 == 210)  // c2 (194) or d2 (210) on
  {
    CRUISE_CONTROL = true;
    CRUISE_CONTROL_SPEED = Data_2 * Multiplier;
  }
  else                //  c0 (192) or d0 (208) off
  {
    CRUISE_CONTROL = false;
    CRUISE_CONTROL_SPEED = Data_2 * Multiplier;
  }
}

void AUTOMOBILE_INDICATORS::store_signal(int Signal_Lights)
{
  if (bit_value(Signal_Lights, 4))
  {
    if (bit_value(Signal_Lights, 5))
    {
      SIGNAL_RIGHT = true;
      SIGNAL_LEFT = false;
    }
    else
    {
      SIGNAL_LEFT = true;
      SIGNAL_RIGHT = false;
    }
  }
  else
  {
    SIGNAL_LEFT = false;
    SIGNAL_RIGHT = false;
  }

}

// Datastream regarding this protocol:
// Scanned... No signs that hazard indicators, active or inactive, are 
// interfacing with the auto's comp. Hypothesis: the hazard indicator 
// circuitry is isolated, a standalone subsystem, designed to maintain 
// functionality even if the rest of the auto suffers damage or shorts.
// 
// However, the sys does seem to register the left and right 
// signal indicators independently. The data packets regarding these 
// indicators are glitchy. Sometimes, they're received instantly, other 
// times, they're erratic. Can't compute why the data isn't reliable. 
// Suspect the delayed packets are due to the cam bus filtering out low 
// priority data, or there's a glitch in my RasCAN causing the packets 
// to be lost or not received.
// 
// So, how are hazard indicators read? Scanned the two bit values, 
// 7 for right signal, 6 for left. If both are active simultaneously, 
// then hazard indicators must be engaged. Because the indicators 
// blink, (it's possible that it's just the bit values of the indicators 
// on the dashboard,) I'm not registering the hazard indicators as 
// disengaged until there's been a consistent 5 seconds where both signal 
// indicators were inactive simultaneously.
// 
// Initially, this timer was set to 2 seconds, which wasn't sufficient, 
// because the communication error consistently lasted for a longer 
// duration.

void AUTOMOBILE_INDICATORS::store_hazards(unsigned long current_time, int Hazard)
{
  if (bit_value(Hazard, 7) && bit_value(Hazard, 6))
  {
    HAZARDS = true;
    HAZARD_CHECK.ping_up(current_time, 5000);
  }
  else
  {
    if (HAZARD_CHECK.enabled() && HAZARD_CHECK.ping_down(current_time) == false)
    {
      HAZARDS = false;
    }
  }
}

void AUTOMOBILE_INDICATORS::store_ignition_switch(int Key_Switch)
{
  if (!bit_value(Key_Switch, 4) && bit_value(Key_Switch, 3))        // xxx
  {
    IGNITION_SWITCH = -1;  // No key in ignition
  }
  else if (bit_value(Key_Switch, 4) && !bit_value(Key_Switch, 3))   // xxx
  {
    IGNITION_SWITCH = 1; // Key in ignition, in off position
  }
  //else if (bit_value(Key_Switch, 5) && !bit_value(Key_Switch, 4))   // xxx
  //{
  //  IGNITION_SWITCH = 2; // Key in acc position
  //}
  else if (bit_value(Key_Switch, 4) && bit_value(Key_Switch, 3))    // xxx
  {
    IGNITION_SWITCH = 3; // Key in running position
  }
  else
  {
    IGNITION_SWITCH = 0; // Unknown
  }
}

bool AUTOMOBILE_INDICATORS::val_light_switch()
{
  return LIGHT_SWITCH;
}

int AUTOMOBILE_INDICATORS::val_lights_pos()
{
  return LIGHTS_POS;
}

string AUTOMOBILE_INDICATORS::lights_switch()
{
  return LIGHTS_DESC;
}

bool AUTOMOBILE_INDICATORS::val_lights_high_beam_on()
{
  return LIGHTS_HIGH_BEAM_ON;
}

bool AUTOMOBILE_INDICATORS::val_lights_headlights_on()
{
  return LIGHTS_HEADLIGHTS_ON;
}

bool AUTOMOBILE_INDICATORS::val_lights_parking_on()
{
  return LIGHTS_PARKING_ON;
}

bool AUTOMOBILE_INDICATORS::val_sinal_left()
{
  return SIGNAL_LEFT;
}

bool AUTOMOBILE_INDICATORS::val_sinal_right()
{
  return SIGNAL_RIGHT;
}

bool AUTOMOBILE_INDICATORS::val_hazards()
{
  return HAZARDS;
}

int AUTOMOBILE_INDICATORS::val_ignition_switch()
{
  return IGNITION_SWITCH;
}

bool AUTOMOBILE_INDICATORS::val_parking_brake()
{
  return PARKING_BRAKE;
}

string AUTOMOBILE_INDICATORS::parking_brake()
{
  return PARKING_BRAKE_DESC;
}

bool AUTOMOBILE_INDICATORS::val_ignition()
{
  return IGNITION;
}

string AUTOMOBILE_INDICATORS::ignition()
{
  if (IGNITION)
  {
    return "ENGINE RUNNING";
  }
  else
  {
    return "ENGINE OFF";
  }
  
}

bool AUTOMOBILE_INDICATORS::cruise_control()
{
  return CRUISE_CONTROL;
}

float AUTOMOBILE_INDICATORS::cruise_control_speed()
{
  return CRUISE_CONTROL_SPEED;
}

//-----------

void AUTOMOBILE_POWER::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    VAL_LOAD = -1;
    LOAD = "X";
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_POWER::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_POWER::store(int Load)
{
  // val - 8000 -- Negative value is reverse power request.
  VAL_LOAD = (Load - 32768);   // Value type unknown

  LOAD = to_string(VAL_LOAD);
}

int AUTOMOBILE_POWER::val_load()
{
  return VAL_LOAD;
}

string AUTOMOBILE_POWER::load()
{
  return LOAD;
}

//-----------

void AUTOMOBILE_RPM::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    VAL_RPM = -1;
    RPM = "X";

    VAL_RPM_2 = -1;
    RPM_2 = "X";
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_RPM::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

bool AUTOMOBILE_RPM::store(int Rpm)
{
  int val = (Rpm - 24576) * 2;
  if (val > 0 && val < 10000)
  {
    VAL_RPM = val;    // Wrong

    RPM = to_string(VAL_RPM);

    return false;
  }
  else
  {
    return true;
  }
}

int AUTOMOBILE_RPM::val_rpm()
{
  return VAL_RPM;
}

string AUTOMOBILE_RPM::rpm()
{
  return RPM;
}

bool AUTOMOBILE_RPM::store_2(int Rpm)
{
  int val = (Rpm - 49152) * 2;
  if (val > 0 && val < 10000)
  {
    VAL_RPM_2 = val;    // Wrong

    RPM_2 = to_string(VAL_RPM_2);

    return false;
  }
  else
  {
    return true;
  }
}

int AUTOMOBILE_RPM::val_rpm_2()
{
  return VAL_RPM_2;
}

string AUTOMOBILE_RPM::rpm_2()
{
  return RPM_2;
}

//-----------

void AUTOMOBILE_STEERING::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    PREVIOUS_STEERING_WHEEL_ANGLE = -1;
    REPORTED_STEERING_WHEEL_ANGLE = -1;
    VAL_STEERING_WHEEL_ANGLE = -1;
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_STEERING::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_STEERING::store_steering_wheel_angle(int Angle, int Direction)
{
  // x8000 at 0 deg. x8500 at 90 deg. x9000 at 180 deg.
  //VAL_STEERING_WHEEL_ANGLE = ((float)Angle - 32768);

  VAL_STEERING_WHEEL_ANGLE = (((float)Angle - (float)32768) / 4.0f ) * 0.18f;

  // MSB for Direction

  // Determine direction of wheel turn.
  if (VAL_STEERING_WHEEL_ANGLE == PREVIOUS_STEERING_WHEEL_ANGLE)
  {
    CLOCKWISE = 0;
  }
  else
  {
    if (Direction >= 128)
    {
      CLOCKWISE = 1;
    }
    else
    {
      CLOCKWISE = -1;
    }

    // Determine if wheel is left or right of center.
    if (CLOCKWISE == 1)
    {
      if (VAL_STEERING_WHEEL_ANGLE > PREVIOUS_STEERING_WHEEL_ANGLE)
      {
        LEFT_OF_CENTER = false;
      }
    }
    else if (CLOCKWISE == -1)
    {
      if (VAL_STEERING_WHEEL_ANGLE > PREVIOUS_STEERING_WHEEL_ANGLE)
      {
        LEFT_OF_CENTER = true;
      }
    }

    // Set Display Variables
    PREVIOUS_STEERING_WHEEL_ANGLE = VAL_STEERING_WHEEL_ANGLE;
  }
}

float AUTOMOBILE_STEERING::val_steering_wheel_angle()
{
  return VAL_STEERING_WHEEL_ANGLE;
}

string AUTOMOBILE_STEERING::steering_wheel_angle()
{
  return to_string_round_to_nth(VAL_STEERING_WHEEL_ANGLE, 1) + " deg";
}

string AUTOMOBILE_STEERING::turning_direction()
{
  if (CLOCKWISE == 0)
  {
    return "--";
  }
  if (CLOCKWISE == 1)
  {
    return "->";
  }
  if (CLOCKWISE == -1)
  {
    return "<-";
  }
  return "XX";
}

bool AUTOMOBILE_STEERING::clockwise()
{
  return CLOCKWISE;
}

bool AUTOMOBILE_STEERING::val_left_of_center()
{
  return LEFT_OF_CENTER;
}

string AUTOMOBILE_STEERING::left_of_center()
{
  if (LEFT_OF_CENTER)
  {
    return "<-";
  }
  else
  {
    return "->";
  }
}

//-----------

void AUTOMOBILE_VELOCITY::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    SPEED_TRANS.store_kmph(-1, 0, 0);
    SPEED_DASH.store_kmph(-1, 0, 0);
    SPEED_LF_TIRE.store_kmph(-1, 0, 0);
    SPEED_RF_TIRE.store_kmph(-1, 0, 0);
    SPEED_LB_TIRE.store_kmph(-1, 0, 0);
    SPEED_RB_TIRE.store_kmph(-1, 0, 0);
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_VELOCITY::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

// Multiplier source unknown.  not calculating correctly.
// Possibly wrong spec tire size.  How the car knows is beyond me.
void AUTOMOBILE_VELOCITY::store_trans(int kmph, float Multiplier, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent, 
                                      float Verify_kmph)
{
  // Possibly meters per second devided by 3  (Multiplier = 1.2 for kmph conversion )
  //                                          (3.6 kmph = 1 mps                     )

  float calculated_velocity = (kmph * Multiplier) / 10;

  float compared_velocity_diff = abs(calculated_velocity - Verify_kmph);
  
  if (compared_velocity_diff < 3.0)
  {
    SPEED_TRANS.store_kmph(calculated_velocity, tmeFrame_Time, tmeFrame_Time_Sent);
  }
}

void AUTOMOBILE_VELOCITY::store_dash(int Upper, int Lower, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  SPEED_DASH.store_kmph((float)Upper + ((float)Lower / 255), tmeFrame_Time, tmeFrame_Time_Sent);
}

void AUTOMOBILE_VELOCITY::store_LF(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  SPEED_LF_TIRE.store_kmph(((float)(mps - 32768) * TIRE_MULTIPLIER) / 100.0f, tmeFrame_Time, tmeFrame_Time_Sent);
}

void AUTOMOBILE_VELOCITY::store_RF(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  SPEED_RF_TIRE.store_kmph(((float)(mps - 32768) * TIRE_MULTIPLIER) / 100.0f, tmeFrame_Time, tmeFrame_Time_Sent);
}

void AUTOMOBILE_VELOCITY::store_LB(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  SPEED_LB_TIRE.store_kmph(((float)(mps - 32768) * TIRE_MULTIPLIER) / 100.0f, tmeFrame_Time, tmeFrame_Time_Sent);
}

void AUTOMOBILE_VELOCITY::store_RB(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent)
{
  SPEED_RB_TIRE.store_kmph(((float)(mps - 32768) * TIRE_MULTIPLIER) / 100.0f, tmeFrame_Time, tmeFrame_Time_Sent);
}

float AUTOMOBILE_VELOCITY::tire_multiplier()
{
  return TIRE_MULTIPLIER;
}

//-----------

void AUTOMOBILE_TEMPATURE::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    // clear fields?
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_TEMPATURE::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_TEMPATURE::store_coolant_05(int Sensor_Temp)
{
  COOLANT_05.store_c(method_temp_1(Sensor_Temp));
}

/*
void AUTOMOBILE_TEMPATURE::store_coolant_67(int Sensor_Temp_B, int Sensor_Temp_C)
{
  COOLANT_67_b.store_c(method_temp_1(Sensor_Temp_B));
  COOLANT_67_c.store_c(method_temp_1(Sensor_Temp_C));
}
*/

void AUTOMOBILE_TEMPATURE::store_air_intake_0f(int Sensor_Temp)
{
  AIR_INTAKE_0f.store_c(method_temp_1(Sensor_Temp));
}

/*
void AUTOMOBILE_TEMPATURE::store_air_intake_68(int Sensor_Temp_B, int Sensor_Temp_C)
{
  AIR_INTAKE_68_b.store_c(method_temp_1(Sensor_Temp_B));
  AIR_INTAKE_68_b.store_c(method_temp_1(Sensor_Temp_C));
}
*/

void AUTOMOBILE_TEMPATURE::store_ambiant_air_46(int Sensor_Temp)
{
  AMBIANT_AIR_46.store_c(method_temp_1(Sensor_Temp));
}

void AUTOMOBILE_TEMPATURE::store_oil_5c(int Sensor_Temp)
{
  OIL_5c.store_c(method_temp_1(Sensor_Temp));
}

void AUTOMOBILE_TEMPATURE::store_exhaust_gas_6b(int Sensor_Temp)
{
  EXHAUST_GAS_6b.store_c(method_temp_1(Sensor_Temp));
}

void AUTOMOBILE_TEMPATURE::store_manifold_surface_84(int Sensor_Temp)
{
  MANIFOLD_SURFACE_84.store_c(method_temp_1(Sensor_Temp));
}

void AUTOMOBILE_TEMPATURE::store_abs_baro_33(int Baro)
{
  BARO_33.store_kPa(Baro);
}

void AUTOMOBILE_TEMPATURE::store_catalyst_3c(int A, int B)
{
  CATALYST_3C.store_c((method_2_byte_float(A, B) /10) - 40);
}

//-----------

void AUTOMOBILE_ELECTRICAL::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    // clear fields?
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_ELECTRICAL::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_ELECTRICAL::store_control_voltage_42(int Sensor_B, int Sensor_C)
{
  CONTROL_UNIT_42.store_v(method_2_byte_div_1000(Sensor_B, Sensor_C));
}

void AUTOMOBILE_ELECTRICAL::store_run_time_since_start_1f(int Sensor_B, int Sensor_C)
{
  RUN_TIME_SINCE_START.store_seconds(method_2_byte_div_1000(Sensor_B, Sensor_C));
}

//-----------

void AUTOMOBILE_TRANSMISSION_GEAR::set_source_availability(bool Available)
{
  if (SOURCE_AVAILABILITY == true && Available == false)
  {
    REPORTED = -1;
    SHORT_DESC = "X";
    LONG_DESC = "X";

    GEAR_SELECTION_REPORTED = -1;
    GEAR_SELECTION_LONG_DESC = "X";
    GEAR_SELECTION_PARK = false;
    GEAR_SELECTION_REVERSE = false;
    GEAR_SELECTION_NEUTRAL = false;
    GEAR_SELECTION_DRIVE = false;
    GEAR_SELECTION_LOW = false;
  }
  
  SOURCE_AVAILABILITY = Available;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::available()
{
  return check_availability(CODED_AVAILABILITY, SOURCE_AVAILABILITY);
}

void AUTOMOBILE_TRANSMISSION_GEAR::store(int gear)
{
  REPORTED = gear;    // Wrong on park and neutral and reverse.

  if (gear == 19)
  {
    SHORT_DESC = "PN (19)";
    LONG_DESC = "NeuPrk";
  }
  else if (gear == 1)
  {
    SHORT_DESC = "1";
    LONG_DESC = "1st";
  }
  else if (gear == 2)
  {
    SHORT_DESC = "2";
    LONG_DESC = "2nd";
  }
  else if (gear == 3)
  {
    SHORT_DESC = "3";
    LONG_DESC = "3rd";
  }
  else if (gear == 4)
  {
    SHORT_DESC = "4";
    LONG_DESC = "4th";
  }
  else if (gear == 5)
  {
    SHORT_DESC = "5";
    LONG_DESC = "5th";
  }
  else if (gear == 6)
  {
    SHORT_DESC = "6";
    LONG_DESC = "6th";
  }
  //else if (gear == 19)
  //{
  //  SHORT_DESC = "P";
  //  LONG_DESC = "Park";
  //}
  else if (gear == 0)
  {
    SHORT_DESC = "R (0)";
    LONG_DESC = "Reverse";
  }
  else
  {
    SHORT_DESC = "X";
    LONG_DESC = "Unknown";
  }
}

int AUTOMOBILE_TRANSMISSION_GEAR::reported()
{
  return REPORTED;
}

string AUTOMOBILE_TRANSMISSION_GEAR::short_desc()
{
  return SHORT_DESC;
}

string AUTOMOBILE_TRANSMISSION_GEAR::long_desc()
{
  return LONG_DESC;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::store_gear_selection(int Gear, int Gear_Alt)
{
  bool ret_changed = false;

  GEAR_SELECTION_REPORTED = Gear;

  // Verifying gear selections with transmissin gear position before changing 
  //  Will drasticly increase latency of reported gear shift lever position.

  if (Gear_Alt == 0)
  {
    if (!GEAR_SELECTION_PARK)
    {
      // Park
      GEAR_SELECTION_SHORT_DESC = "P";
      GEAR_SELECTION_LONG_DESC = "Park";
      GEAR_SELECTION_PARK = true;
      GEAR_SELECTION_REVERSE = false;
      GEAR_SELECTION_NEUTRAL = false;
      GEAR_SELECTION_DRIVE = false;
      GEAR_SELECTION_LOW = false;

      ret_changed = true;
    }
  }
  else if (bit_value(Gear_Alt, 6) == false && bit_value(Gear_Alt, 5) == false && bit_value(Gear_Alt, 4) == true)
  {
    if (!GEAR_SELECTION_REVERSE)
    {
      // Reverse
      GEAR_SELECTION_SHORT_DESC = "R";
      GEAR_SELECTION_LONG_DESC = "Reverse";
      GEAR_SELECTION_PARK = false;
      GEAR_SELECTION_REVERSE = true;
      GEAR_SELECTION_NEUTRAL = false;
      GEAR_SELECTION_DRIVE = false;
      GEAR_SELECTION_LOW = false;

      ret_changed = true;
    }
  }
  else if (bit_value(Gear_Alt, 6) == false && bit_value(Gear_Alt, 5) == true && bit_value(Gear_Alt, 4) == false)
  {
    if (!GEAR_SELECTION_NEUTRAL)
    {
      // Neutral
      GEAR_SELECTION_SHORT_DESC = "N";
      GEAR_SELECTION_LONG_DESC = "Neutral";
      GEAR_SELECTION_PARK = false;
      GEAR_SELECTION_REVERSE = false;
      GEAR_SELECTION_NEUTRAL = true;
      GEAR_SELECTION_DRIVE = false;
      GEAR_SELECTION_LOW = false;

      ret_changed = true;
    }
  }
  else if (bit_value(Gear_Alt, 6) == false && bit_value(Gear_Alt, 5) == true && bit_value(Gear_Alt, 4) == true)
  {
    if (!GEAR_SELECTION_DRIVE)
    {
      // Drive
      GEAR_SELECTION_SHORT_DESC = "D";
      GEAR_SELECTION_LONG_DESC = "Drive";
      GEAR_SELECTION_PARK = false;
      GEAR_SELECTION_REVERSE = false;
      GEAR_SELECTION_NEUTRAL = false;
      GEAR_SELECTION_DRIVE = true;
      GEAR_SELECTION_LOW = false;

      ret_changed = true;
    }
  }
  else if (bit_value(Gear_Alt, 6) == true && bit_value(Gear_Alt, 5) == false && bit_value(Gear_Alt, 4) == true)
  {
    if (!GEAR_SELECTION_LOW)
    {
      // Low
      GEAR_SELECTION_SHORT_DESC = "L";
      GEAR_SELECTION_LONG_DESC = "Low";
      GEAR_SELECTION_PARK = false;
      GEAR_SELECTION_REVERSE = false;
      GEAR_SELECTION_NEUTRAL = false;
      GEAR_SELECTION_DRIVE = false;
      GEAR_SELECTION_LOW = true;

      ret_changed = true;
    }
  }
  else
  {
    // Unknown  - 00 D0 7F 50 35 00 0D 08 0D 08 010F3FCE
    GEAR_SELECTION_SHORT_DESC = "X";
    GEAR_SELECTION_LONG_DESC = "Unknown";
    GEAR_SELECTION_PARK = false;
    GEAR_SELECTION_REVERSE = false;
    GEAR_SELECTION_NEUTRAL = false;
    GEAR_SELECTION_DRIVE = false;
    GEAR_SELECTION_LOW = false;

    // Insert Error Check Here
    ret_changed = true;
  }

  return ret_changed;
}

int AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_reported()
{
  return GEAR_SELECTION_REPORTED;
}

string AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_short_desc()
{
  return GEAR_SELECTION_SHORT_DESC;
}

string AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_long_desc()
{
  return GEAR_SELECTION_LONG_DESC;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_park()
{
  return GEAR_SELECTION_PARK;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_reverse()
{
  return GEAR_SELECTION_REVERSE;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_neutral()
{
  return GEAR_SELECTION_NEUTRAL;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_drive()
{
  return GEAR_SELECTION_DRIVE;
}

bool AUTOMOBILE_TRANSMISSION_GEAR::gear_selection_low()
{
  return GEAR_SELECTION_LOW;
}

//-----------

void TIRE_TTL::first_run(float Low_Percentage, float Top_Percentage)
{
  LOW_PERCENTAGE = Low_Percentage;
  TOP_PERCENTAGE = Top_Percentage;

  WHEEL_SPEED_PECENTAGE_DIFF_MEAN.PROP.SLICES = 20;
  WHEEL_SPEED_PECENTAGE_DIFF_MEAN.PROP.SAMPLE_LIMIT = 100;
  WHEEL_SPEED_PECENTAGE_DIFF_MEAN.PROP.SAMPLE_LIMITED_SPANS = true;
}

void TIRE_TTL::calculate(VELOCITY Tire_Speed, VELOCITY Transmission_Speed, unsigned long tmeFrame_Time)
{
  if( Transmission_Speed.val_meters_per_second() != 0)
  {
    DIFFERANCE_PERCENTAGE = Tire_Speed.val_meters_per_second() / Transmission_Speed.val_meters_per_second();
    LIFE_PERCENTAGE = ((DIFFERANCE_PERCENTAGE *100) - LOW_PERCENTAGE) / (TOP_PERCENTAGE - LOW_PERCENTAGE);

    WHEEL_SPEED_PECENTAGE_DIFF_MEAN.put_value(LIFE_PERCENTAGE, tmeFrame_Time);
    WHEEL_SPEED_OFFSET.store_meters_per_second(Tire_Speed.val_meters_per_second() - Transmission_Speed.val_meters_per_second(), tmeFrame_Time, 0);
  }
  else
  {
    WHEEL_SPEED_OFFSET.store_meters_per_second(0, tmeFrame_Time, 0);
  }
}

int TIRE_TTL::slice_size()
{
  return WHEEL_SPEED_PECENTAGE_DIFF_MEAN.slice_size();
}

int TIRE_TTL::slice_size_max()
{
  return WHEEL_SPEED_PECENTAGE_DIFF_MEAN.slice_size_max();
}

float TIRE_TTL::val_instant_differance_percentage()
{
  return 100 - (DIFFERANCE_PERCENTAGE *100);
}

string TIRE_TTL::instant_differance_percentage()
{
  return to_string_round_to_nth(100 - (DIFFERANCE_PERCENTAGE *100), 2) + "%%";
}

float TIRE_TTL::val_life_percentage_mean()
{
  return (WHEEL_SPEED_PECENTAGE_DIFF_MEAN.mean_float() * 100);
}

string TIRE_TTL::life_percentage_mean()
{
  return to_string((int)(WHEEL_SPEED_PECENTAGE_DIFF_MEAN.mean_float() * 100)) + "%%";
}

VELOCITY TIRE_TTL::wheel_speed_offset()
{
  return WHEEL_SPEED_OFFSET;
}

//-----------

bool SIMPLE_ERRORS_DATA::triggered()
{
  if (TRIGGERED)
  {
    TRIGGERED = false;
    return true;
  }
  else
  {
    return false;
  }
}

void SIMPLE_ERRORS_DATA::set_error(bool Value)
{
  if (ERRORED != Value)
  {
    ERRORED = Value;

    if (Value)
    {
      TRIGGERED = true;
    }
  }
}

int SIMPLE_ERRORS::error_count()
{
  if (GEAR_SELECTION.triggered() || 
      DOORS.triggered() || 
      RPM1.triggered() || 
      RPM2.triggered())
  {
    ERROR_COUNT++;
  }

  return ERROR_COUNT;
}

//-----------

void AUTOMOBILE_CALCULATED::compute_low(DNFWTS_ &Dnfwts, AUTOMOBILE_TRANSLATED_DATA &Status, unsigned long tmeFrame_Time)
{
  // Set Lights
  Status.INDICATORS.process();

  // No init process. Set first run variables
  if (FIRST_RUN == true)
  { 
    // TTL
    UNFILTHERED_LF_TTL.first_run(TTL_LF_Low_Percentage, TTL_LF_Top_Percentage);
    UNFILTHERED_RF_TTL.first_run(TTL_RF_Low_Percentage, TTL_RF_Top_Percentage);
    UNFILTHERED_LB_TTL.first_run(TTL_LB_Low_Percentage, TTL_LB_Top_Percentage);
    UNFILTHERED_RB_TTL.first_run(TTL_RB_Low_Percentage, TTL_RB_Top_Percentage);

    LF_TTL.first_run(TTL_LF_Low_Percentage, TTL_LF_Top_Percentage);
    RF_TTL.first_run(TTL_RF_Low_Percentage, TTL_RF_Top_Percentage);
    LB_TTL.first_run(TTL_LB_Low_Percentage, TTL_LB_Top_Percentage);
    RB_TTL.first_run(TTL_RB_Low_Percentage, TTL_RB_Top_Percentage);

    // Acceleration
    ACCELERATION_MIN_MAX_HISTORY.PROP.SLICES = 6;
    ACCELERATION_MIN_MAX_HISTORY.PROP.TIME_SPAN = 30000;

    ACCELERATION_QUICK_MEAN_HISTORY.PROPS.SIZE = 200;
    ACCELERATION_QUICK_MEAN_HISTORY.PROPS.ALIVE_TIME = 1000;
    ACCELERATION_QUICK_MEAN_HISTORY.PROPS.EMPERICAL_RULE_ENABLE = true;

    FUEL_LEVEL_EMPERICAL.PROPS.DEVIATIONS = 1.0f;
    FUEL_LEVEL_EMPERICAL.PROPS.VALUE_SIZE = 100;
    
    FIRST_RUN = false;
  }
 
  // Calculate Acceleration and Wheel Speed offsets.

  unsigned long speed_timestamp = Status.SPEED.SPEED_LB_TIRE.time_stamp_time_sent();
  //unsigned long speed_timestamp = Status.SPEED.SPEED_TRANS.time_stamp_time_sent();

  if (speed_timestamp != PREVIOUS_TIME_FOR_ACC)
  {                                               
    float current_velocity =  Status.SPEED.SPEED_ALL_TIRES_AVERAGE.val_meters_per_second();

    //float current_velocity = Status.SPEED.SPEED_TRANS.val_meters_per_second();

    // Reading Acceleration from tire speed may be more accurate.
    float ACCELERATION = 1000 * (   (current_velocity - PREVIOUS_VELOCITY_FOR_ACC)  ) / 
                        (speed_timestamp - PREVIOUS_TIME_FOR_ACC);

    if (abs(ACCELERATION) < 10)
    {
      ACCELERATION_QUICK_MEAN_HISTORY.set_value(tmeFrame_Time, ACCELERATION);
      ACCELERATION_MIN_MAX_HISTORY.put_value(ACCELERATION_QUICK_MEAN_HISTORY.impact(tmeFrame_Time), tmeFrame_Time);
    }
    
    PREVIOUS_VELOCITY_FOR_ACC = current_velocity;
    PREVIOUS_TIME_FOR_ACC = speed_timestamp;

    ACCELERATION_TIMER.ping_up(tmeFrame_Time, 250);

    // Calculate Wheelspeed Offset or Differance.

    // Unfilthered
    if (Status.SPEED.SPEED_TRANS.time_stamp_time_sent() != PREVIOUS_VELOCITY.time_stamp_time_sent())
    {
      UNFILTHERED_LF_TTL.calculate(Status.SPEED.SPEED_LF_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
      UNFILTHERED_RF_TTL.calculate(Status.SPEED.SPEED_RF_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
      UNFILTHERED_LB_TTL.calculate(Status.SPEED.SPEED_LB_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
      UNFILTHERED_RB_TTL.calculate(Status.SPEED.SPEED_RB_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);

      if (Status.SPEED.SPEED_TRANS.val_mph() > 43 && Status.SPEED.SPEED_TRANS.val_mph() < 48 && 
          Status.STEERING.val_steering_wheel_angle() < 2 && 
          ACCELERATION_QUICK_MEAN_HISTORY.impact(tmeFrame_Time) > -.2 && 
          ACCELERATION_QUICK_MEAN_HISTORY.impact(tmeFrame_Time) < .2 ) 
          // Only get values at certain speeds and while drivng straight.
          // Centering speed at 45 because I can drive for hours and not get over 50mph 
          //  and Acceleration < +- .2.
      {
        LF_TTL.calculate(Status.SPEED.SPEED_LF_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
        RF_TTL.calculate(Status.SPEED.SPEED_RF_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
        LB_TTL.calculate(Status.SPEED.SPEED_LB_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
        RB_TTL.calculate(Status.SPEED.SPEED_RB_TIRE, Status.SPEED.SPEED_TRANS, tmeFrame_Time);
      }
    }

    // DNFWTS
    {
      if (Status.BRAKE.val_value() > 0.0f && Status.ACCELERATOR.val_value() > 0.0f)
      {
        Dnfwts.turn_on(tmeFrame_Time);
      }
    }
  }

  // Fuel Level
  
  if (Status.FUEL.LEVEL_RAW_CHANGED)
  {
    FUEL_LEVEL_EMPERICAL.add_value(Status.FUEL.val_level_raw());
    FUEL_LEVEL_EMPERICAL_DISP = to_string_round_to_nth(FUEL_LEVEL_EMPERICAL.mean(), 2) + " gal";
    Status.FUEL.LEVEL_RAW_CHANGED = false;
  }

  // S-Temp
  S_TEMP = (((1.0f * Status.TEMPS.AMBIANT_AIR_46.val_c() + 
              2.0f * Status.TEMPS.AIR_INTAKE_0f.val_c() + 
              2.0f * Status.TEMPS.COOLANT_05.val_c() + 
              2.0f * (Status.TEMPS.CATALYST_3C.val_c() / 20.0f))
              / 7.0f) - 30.0f) * 3.0f;
}

float AUTOMOBILE_CALCULATED::acceleration(unsigned long tmeFrame_Time)
{
  return ACCELERATION_QUICK_MEAN_HISTORY.impact(tmeFrame_Time);
}

float AUTOMOBILE_CALCULATED::s_temp()
{
  return S_TEMP;
}

string AUTOMOBILE_CALCULATED::fuel_level_emperical()
{
  return FUEL_LEVEL_EMPERICAL_DISP;
}

float AUTOMOBILE_CALCULATED::fuel_level_emperical_val()
{
  return FUEL_LEVEL_EMPERICAL.mean();
}

//-----------

bool AUTOMOBILE::parse(string Line, int &PID_Recieved, bool &Identified)
{
  PID_Recieved = 0;
  Identified = false;

  bool ret_message_recieved = false;
  
  string left = "";

  int upper = 0;
  int lower = 0;

  int time_byte_0 = 0;
  int time_byte_1 = 0;
  int time_byte_2 = 0;
  int time_byte_3 = 0;

  FALSE_CATCH valid;

  AUTOMOBILE_DATA_LINE data;

  data.ORIG = Line;

  //02 00 F2 00 B7 EC 00 00 C0 67 004205B1

  valid.catch_false(string_hex_to_int(Line.substr(0, 2), upper));
  valid.catch_false(string_hex_to_int(Line.substr(3, 2), lower));
   
  valid.catch_false(string_hex_to_int(Line.substr(6, 2), data.DATA[0]));
  valid.catch_false(string_hex_to_int(Line.substr(9, 2), data.DATA[1]));
  valid.catch_false(string_hex_to_int(Line.substr(12, 2), data.DATA[2]));
  valid.catch_false(string_hex_to_int(Line.substr(15, 2), data.DATA[3]));
  valid.catch_false(string_hex_to_int(Line.substr(18, 2), data.DATA[4]));
  valid.catch_false(string_hex_to_int(Line.substr(21, 2), data.DATA[5]));
  valid.catch_false(string_hex_to_int(Line.substr(24, 2), data.DATA[6]));
  valid.catch_false(string_hex_to_int(Line.substr(27, 2), data.DATA[7]));

  valid.catch_false(string_hex_to_int(Line.substr(30, 2), time_byte_0));
  valid.catch_false(string_hex_to_int(Line.substr(32, 2), time_byte_1));
  valid.catch_false(string_hex_to_int(Line.substr(34, 2), time_byte_2));
  valid.catch_false(string_hex_to_int(Line.substr(36, 2), time_byte_3));

  if (valid.has_false() == false)
  {
    // Compute ID
    data.ID = upper *256 + lower;
    data.TIMESTAMP_MESSAGE_SENT = (time_byte_0 * (256 ^3)) + (time_byte_1 * (256 ^2)) + 
                    (time_byte_2 * 256) + time_byte_3;

    // Store into latest values
    LATEST_PID = data.ID;
    LATEST_DATA[0] = data.DATA[0];
    LATEST_DATA[1] = data.DATA[1];
    LATEST_DATA[2] = data.DATA[2];
    LATEST_DATA[3] = data.DATA[3];
    LATEST_DATA[4] = data.DATA[4];
    LATEST_DATA[5] = data.DATA[5];
    LATEST_DATA[6] = data.DATA[6];
    LATEST_DATA[7] = data.DATA[7];

    // NOTE:
    //  Set PID_Recieved if message is to be computed immediatly.
    //    PID_Recieved = data.ID;

    // Put value in pos
    if(data.ID == 0)
    {
      DATA.AD_0 = data;
      DATA.AD_0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 16)
    {
      DATA.AD_10 = data;
      PID_Recieved = data.ID;
      DATA.AD_10.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 48)
    {
      DATA.AD_30 = data;
      DATA.AD_30.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 50)
    {
      DATA.AD_32 = data;
      DATA.AD_32.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 74)
    {
      DATA.AD_4A = data;
      DATA.AD_4A.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 75)
    {
      DATA.AD_4B = data;
      DATA.AD_4B.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 80)
    {
      DATA.AD_50 = data;
      DATA.AD_50.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 96)
    {
      DATA.AD_60 = data;
      DATA.AD_60.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 100)
    {
      DATA.AD_64 = data;
      DATA.AD_64.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 106)
    {
      DATA.AD_6A = data;
      DATA.AD_6A.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 112)
    {
      DATA.AD_70 = data;
      DATA.AD_70.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 128)
    {
      DATA.AD_80 = data;
      DATA.AD_80.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 144)
    {
      DATA.AD_90 = data;
      DATA.AD_90.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 160)
    {
      DATA.AD_A0 = data;
      DATA.AD_A0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 176)
    {
      DATA.AD_B0 = data;
      DATA.AD_B0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 192)
    {
      DATA.AD_C0 = data;
      DATA.AD_C0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 200)
    {
      DATA.AD_C8 = data;
      DATA.AD_C8.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 208)
    {
      DATA.AD_D0 = data;
      DATA.AD_D0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 224)
    {
      DATA.AD_E0 = data;
      DATA.AD_E0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 240)
    {
      DATA.AD_F0 = data;
      PID_Recieved = data.ID;
      DATA.AD_F0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 248)
    {
      DATA.AD_F8 = data;
      DATA.AD_F8.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 256)
    {
      DATA.AD_100 = data;
      DATA.AD_100.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 258)
    {
      DATA.AD_102 = data;
      DATA.AD_102.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 416)
    {
      DATA.AD_1A0 = data;
      DATA.AD_1A0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 448)
    {
      DATA.AD_1C0 = data;
      DATA.AD_1C0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 288)
    {
      DATA.AD_120 = data;
      DATA.AD_120.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 296)
    {
      DATA.AD_128 = data;
      DATA.AD_128.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 304)
    {
      DATA.AD_130 = data;
      DATA.AD_130.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 312)
    {
      DATA.AD_138 = data;
      DATA.AD_138.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 320)
    {
      DATA.AD_140 = data;
      DATA.AD_140.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 336)
    {
      DATA.AD_150 = data;
      DATA.AD_150.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 352)
    {
      DATA.AD_160 = data;
      DATA.AD_160.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 382)
    {
      DATA.AD_17E = data;
      DATA.AD_17E.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 384)
    {
      DATA.AD_180 = data;
      DATA.AD_180.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 400)
    {
      DATA.AD_190 = data;
      PID_Recieved = data.ID;
      DATA.AD_190.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 464)
    {
      DATA.AD_1D0 = data;
      DATA.AD_1D0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 480)
    {
      DATA.AD_1E0 = data;
      DATA.AD_1E0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 512)
    {
      DATA.AD_200 = data;
      DATA.AD_200.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 520)
    {
      DATA.AD_208 = data;
      DATA.AD_208.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 528)
    {
      DATA.AD_210 = data;
      DATA.AD_210.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 531)
    {
      DATA.AD_213 = data;
      DATA.AD_213.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 536)
    {
      DATA.AD_218 = data;
      DATA.AD_218.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 583)
    {
      DATA.AD_247 = data;
      DATA.AD_247.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 594)
    {
      DATA.AD_252 = data;
      DATA.AD_252.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 595)
    {
      DATA.AD_253 = data;
      DATA.AD_253.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 608)
    {
      DATA.AD_260 = data;
      DATA.AD_260.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 624)
    {
      DATA.AD_270 = data;
      DATA.AD_270.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 640)
    {
      DATA.AD_280 = data;
      DATA.AD_280.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 656)
    {
      DATA.AD_290 = data;
      DATA.AD_290.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 688)
    {
      DATA.AD_2B0 = data;
      DATA.AD_2B0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 704)
    {
      DATA.AD_2C0 = data;
      DATA.AD_2C0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 720)
    {
      DATA.AD_2D0 = data;
      DATA.AD_2D0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 724)
    {
      DATA.AD_2D4 = data;
      DATA.AD_2D4.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 728)
    {
      DATA.AD_2D8 = data;
      DATA.AD_2D8.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 752)
    {
      DATA.AD_2F0 = data;
      DATA.AD_2F0.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 784)
    {
      DATA.AD_310 = data;
      DATA.AD_310.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 792)
    {
      DATA.AD_318 = data;
      DATA.AD_318.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 832)
    {
      DATA.AD_340 = data;
      DATA.AD_340.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 864)
    {
      DATA.AD_360 = data;
      DATA.AD_360.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 896)
    {
      DATA.AD_380 = data;
      DATA.AD_380.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 904)
    {
      DATA.AD_388 = data;
      DATA.AD_388.CHANGED = true;
    }
    else if(data.ID == 912)
    {
      DATA.AD_390 = data;
      DATA.AD_390.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 920)
    {
      DATA.AD_398 = data;
      DATA.AD_398.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 948)
    {
      DATA.AD_3B4 = data;
      DATA.AD_3B4.CHANGED = true;
    }
    else if(data.ID == 1024)
    {
      DATA.AD_400 = data;
      DATA.AD_400.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1029)
    {
      DATA.AD_405 = data;
      DATA.AD_405.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1034)
    {
      DATA.AD_40A = data;
      DATA.AD_40A.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1077)
    {
      DATA.AD_435 = data;
      DATA.AD_435.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1109)
    {
      DATA.AD_455 = data;
      DATA.AD_455.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1409)
    {
      DATA.AD_581 = data;
      DATA.AD_581.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 1506)
    {
      DATA.AD_5E2 = data;
      DATA.AD_5E2.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 0x73f)
    {
      DATA.AD_73F = data;
      DATA.AD_73F.CHANGED = true;
      Identified = true;
    }

    // Message Recieved Lines
    else if(data.ID == 2024)
    {
      DATA.AD_7E8 = data;
      PID_Recieved = data.ID;
      DATA.AD_7E8.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 2025)
    {
      DATA.AD_7E9 = data;
      PID_Recieved = data.ID;
      DATA.AD_7E9.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 2026)
    {
      DATA.AD_7EA = data;
      PID_Recieved = data.ID;
      DATA.AD_7EA.CHANGED = true;
      Identified = true;
    }
    else if(data.ID == 2027)
    {
      DATA.AD_7EB = data;
      PID_Recieved = data.ID;
      DATA.AD_7EB.CHANGED = true;
      Identified = true;
    }
    
    // RasFLED CAN Messages
    else if(data.ID == 0xffff)
    {
      DATA.AD_FFFF = data;
      PID_Recieved = data.ID;
      DATA.AD_FFFF.CHANGED = true;
      Identified = true;
    }
    
    // Unknown
    else
    {
      DATA.AD_UNKNOWN = data;
      DATA.AD_UNKNOWN.CHANGED = true;
      Identified = false;
    }

    ret_message_recieved = true;
  }

  return ret_message_recieved;
}

void COMMUNICATION_STATISTICS::process_received(int SentA, int SentB, int SentC)
{
  CAN_SENT = (SentA * 256) + SentB;
  CAN_MAX_QUEUE = SentC;

  RAS_RECEIVED = RAS_RECIEVED_CURRENT_COUNT - 1;
  RAS_RECIEVED_CURRENT_COUNT = 0;

  CAN_SENT_TOTAL_COUNT = CAN_SENT_TOTAL_COUNT + CAN_SENT;
  RAS_RECIEVED_TOTAL_COUNT = RAS_RECIEVED_TOTAL_COUNT + RAS_RECEIVED;
  ERRORS = abs(RAS_RECEIVED - RAS_RECEIVED);
}

void COMMUNICATION_STATISTICS::RAS_RECIEVED_CURRENT_COUNT_INC()
{
  RAS_RECIEVED_CURRENT_COUNT++;
}

int COMMUNICATION_STATISTICS::can_sent()
{
  return CAN_SENT;
}

int COMMUNICATION_STATISTICS::can_max_queue()
{
  return CAN_MAX_QUEUE;
}

int COMMUNICATION_STATISTICS::ras_recieved()
{
  return RAS_RECEIVED;
}

int COMMUNICATION_STATISTICS::can_sent_total_count()
{
  return CAN_SENT_TOTAL_COUNT;
}

int COMMUNICATION_STATISTICS::ras_recieved_total_count()
{
  return RAS_RECIEVED_TOTAL_COUNT;
}

int COMMUNICATION_STATISTICS::errors()
{
  return ERRORS;
}

bool AUTOMOBILE_AVAILABILITY::check_for_live_data(unsigned long tmeFrame_Time)
{
  if (ACTIVITY_TIMER.ping_down(tmeFrame_Time) == false)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool AUTOMOBILE_AVAILABILITY::set_active(AUTOMOBILE_TRANSLATED_DATA &Status, bool Available, unsigned long tmeFrame_Time)
{
  bool ret_changed = false;

  if (Available == false)
  {
    // Data not available, but was before.
    if (ACTIVE == true)
    {
      // turn everything off
      Status.STEERING.set_source_availability(false);
      Status.GEAR.set_source_availability(false);
      Status.SPEED.set_source_availability(false);
      Status.TEMPS.set_source_availability(false);
      Status.RPM.set_source_availability(false);
      Status.POWER.set_source_availability(false);
      Status.INDICATORS.set_source_availability(false);
      Status.FUEL.set_source_availability(false);
      Status.DOORS.set_source_availability(false);
      Status.GUAGES.set_source_availability(false);

      ACTIVE = false;
      ret_changed = true;
    }
  }
  else
  {
    // Data available.
    
    // Restart the timeout timer

    ACTIVITY_TIMER.ping_up(tmeFrame_Time, 5000);

    // Data available, but wasnt before.
    if (ACTIVE == false)
    {
      // turn everything on
      Status.STEERING.set_source_availability(true);
      Status.GEAR.set_source_availability(true);
      Status.SPEED.set_source_availability(true);
      Status.TEMPS.set_source_availability(true);
      Status.RPM.set_source_availability(true);
      Status.POWER.set_source_availability(true);
      Status.INDICATORS.set_source_availability(true);
      Status.FUEL.set_source_availability(true);
      Status.DOORS.set_source_availability(true);
      Status.GUAGES.set_source_availability(true);

      ACTIVE = true;
      ret_changed = true;
    }    
  }
  
  return ret_changed;
}

bool AUTOMOBILE_AVAILABILITY::is_active()
{
  return ACTIVE;
}

void AUTOMOBILE::add_to_pid_send_list(string Requested_PID)
{
  REQUESTED_PID_SEND_LIST.push_back(Requested_PID);
}

void AUTOMOBILE::set_default_request_pid_list()
{
  // https://en.wikipedia.org/wiki/OBD-II_PIDs

  /*
  //add_to_pid_send_list("03"); //  PID_FUEL_STATUS                   0x03  //  * 07 E8 04 41 03 02 00 00 00 00 0001D846
  //  	Fuel system status

  //add_to_pid_send_list("04"); //  PID_CALC_ENGINE_LOAD              0x04  //  * 07 E9 03 41 04 80 00 00 00 00 0001DC1A
  //  	Calculated engine load
  */  

  //add_to_pid_send_list("05"); //  PID_COOLANT_TEMP                  0x05  //  * 07 E8 03 41 05 6F 00 00 00 00 0003E108
  //  	Engine coolant temperature
  
  /*
  //add_to_pid_send_list("06"); //  PID_SHORT_TERM_FUEL_TRIM_1        0x06  //  * 07 E8 03 41 06 7E 00 00 00 00 0002EF49
  //  Short term fuel trim—Bank 1
  
  //add_to_pid_send_list("07"); //  PID_LONG_TERM_FUEL_TRIM_1         0x07  //  * 07 E8 03 41 07 89 00 00 00 00 0003FE53
  //  	Long term fuel trim—Bank 1
  
  //add_to_pid_send_list("0C"); //  PID_ENGIN_PRM                     0x0C  //  * 07 E8 04 41 0C 14 6B 00 00 00 0007338C
  //  	Engine speed
  
  //add_to_pid_send_list("0D"); //  PID_VEHICLE_SPEED                 0x0D  //  * 07 E8 03 41 0D 36 00 00 00 00 000415C3
  //  Vehicle speed
  */
  
  //add_to_pid_send_list("0F"); //  PID_INTAKE_AIR_TEMP               0x0F  //  * 07 E8 03 41 0F 4F 00 00 00 00 000419BF
  //  Intake air temperature
  
  /*
  //add_to_pid_send_list("10"); //  PID_MASS_AIR_FLOW_RATE            0x10  //  * 07 E8 04 41 10 01 85 00 00 00 00030FBF
  //  Mass air flow sensor (MAF) air flow rate
  
  //add_to_pid_send_list("11"); //  PID_THROTTLE_POSITION             0x11  //  * 07 E8 03 41 11 2A 00 00 00 00 000529D8
  //  Throttle position
  
  //add_to_pid_send_list("1C"); //  PID_OBD_STANDARDS                 0x1C  //  * 07 E9 03 41 1C 09 00 00 00 00 00020C2C
  //  OBD standards this vehicle conforms to
  */
  
  //add_to_pid_send_list("1F"); //  PID_RUN_TIME_SINCE_START          0x1F  //  * 07 E8 04 41 1F 00 AA 00 00 00 0002140B
  //  Run time since engine start
  
  /*
  //add_to_pid_send_list("21"); //  PID_DISTANCE_TRAVELED_MIL_ON      0x21  //  * 07 E8 04 41 21 00 00 00 00 00 0003272F
  //  Distance traveled with malfunction indicator lamp (MIL) on
  */
  
  //add_to_pid_send_list("23"); //  PID_FUEL_RAIL_PRESSURE_GAUGE      0x23  //  * 07 E8 04 41 23 00 E1 00 00 00 00032EEB
  //  Fuel Rail Gauge Pressure (diesel, or gasoline direct injection)
  
  /*
  //add_to_pid_send_list("2F"); //  PID_FUEL_TANK_LEVEL               0x2F  //  * 07 E8 03 41 2F 75 00 00 00 00 00054D00
  //  Fuel Tank Level Input
  
  //add_to_pid_send_list("31"); //  PID_DISTANCE_SINCE_CODES_CLEARED  0x31  //  * 07 E8 04 41 31 08 9E 00 00 00 00033AA3
  //  Distance traveled since codes cleared
  */
  
  //add_to_pid_send_list("32"); //  PID_EVAP_SYSTEM_VAPOR_PRESSURE    0x32  //  * 07 E8 04 41 32 FF 77 00 00 00 000662D9
  //  Evap. System Vapor Pressure
  
  //add_to_pid_send_list("33"); //  PID_BARAMETRIC_PRESSURE           0x33  //  * 07 E9 03 41 33 64 00 00 00 00 0002371D
  //  Absolute Barometric Pressure
  
  //add_to_pid_send_list("3C"); //  PID_CATALYST_TEMP_BANK_1_SENSOR_1 0x3C  //  * 07 E8 04 41 3C 19 A3 00 00 00 00066801
  //  Catalyst Temperature: Bank 1, Sensor 1
  
  //add_to_pid_send_list("42"); //  PID_CONTROL_VOLTAGE               0x42  //  * 07 E8 04 41 42 36 17 00 00 00 00070902
  //  Control module voltage
  
  /*
  //add_to_pid_send_list("43"); //  PID_ABSOLUTE_LOAD                 0x43  //  * 07 E8 04 41 43 00 1C 00 00 00 00046644
  //  Absolute load value
  
  //add_to_pid_send_list("44"); //  PID_COMMANDED_AIR_FUEL_RATIO      0x44  //  * 07 E8 04 41 44 70 A3 00 00 00 00046A22
  //  Commanded Air-Fuel Equivalence Ratio (lambda,λ)
  */
  
  //  add_to_pid_send_list("46"); //  PID_AMBIENT_AIR_TEMPERATURE       0x46  //  * 07 E8 03 41 46 4C 00 00 00 00 00057D0B
  //  Ambient air temperature
  
  /*
  //add_to_pid_send_list("47"); //  PID_ABSOLOUTE_TROTTLE_POSITION_B  0x47  //  * 07 E8 03 41 47 2B 00 00 00 00 00068C53
  //  Absolute throttle position B
  
  //add_to_pid_send_list("49"); //  PID_ACCELERATORE_PEDAL_POSITION_D 0x49  //  * 07 E9 03 41 49 27 00 00 00 00 00026486
  //  Accelerator pedal position D
  
  //add_to_pid_send_list("4A"); //  PID_ACCELERATORE_PEDAL_POSITION_E 0x4A  //  * 07 E8 03 41 4A 14 00 00 00 00 0006980A
  //  Accelerator pedal position E
  
  //add_to_pid_send_list("4C"); //  PID_COMMANDED_THROTTLE_ACTUATOR_E 0x4C  //  * 07 E8 03 41 4C 14 00 00 00 00 00069FE5
  //  Commanded throttle actuator
  */

  // Supported Service Codes List
  // Mode 09 Pid 00
  // 00 - 07 e9 06 49 00 14 40 00 00 00 00102d4f
  //                      0001010001000000
  //  (dec) 04 06 10
  //  (hex) 04 06 0a

  // Mode 03 Pid 00
  //  xx - 07 e9 02 43 00 00 00 00 00 00 00115ac8 - not yet decyphered.

  
  // Supported PID list: (Those guys back then were genius, because this code is brilliant)
  //  00 - 07 e9 06 41 00 98 18 00 13 00 000da162
  //                      10011000000110000000000000010011

  //  20 - 07 e9 06 41 20 80 01 a0 01 00 000e16c5
  //                      10000000000000011010000000000001

  //  40 - 07 e8 06 41 40 fe d0 80 00 00 000ed695
  //                      11111110110100001000000000000000

  //  Supported PIDs:
  //  (dec) 01 04 05 12 13 28 31 32 
  //  (hex) 00 04 05 0c 0d 1c 1f 20
  
  //  (dec) 33 48 49 51 64
  //  (hex) 21 30 31 33 40

  //  (dec) 65 66 67 68 69 70 71 73 74 79
  //  (hex) 41 42 43 44 45 46 47 49 4a 4f

  // Differant queue
  {
    add_to_pid_send_list("23"); //  PID_FUEL_RAIL_PRESSURE_GAUGE      0x23  //  * 07 E8 04 41 23 00 E1 00 00 00 00032EEB
    add_to_pid_send_list("32"); //  PID_EVAP_SYSTEM_VAPOR_PRESSURE    0x32  //  * 07 E8 04 41 32 FF 77 00 00 00 000662D9
    add_to_pid_send_list("42"); //  PID_CONTROL_VOLTAGE               0x42  //  * 07 E8 04 41 42 36 17 00 00 00 00070902
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure

    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("0F"); //  PID_INTAKE_AIR_TEMP               0x0F  //  * 07 E8 03 41 0F 4F 00 00 00 00 000419BF
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("3C"); //  PID_CATALYST_TEMP_BANK_1_SENSOR_1 0x3C  //  * 07 E8 04 41 3C 19 A3 00 00 00 00066801
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("46"); //  PID_AMBIENT_AIR_TEMPERATURE       0x46  //  * 07 E8 03 41 46 4C 00 00 00 00 00057D0B
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
  }

  add_to_pid_send_list("05"); //  PID_COOLANT_TEMP                  0x05  //  * 07 E8 03 41 05 6F 00 00 00 00 0003E108
  add_to_pid_send_list("23"); // Fuel Rail Pressure
  add_to_pid_send_list("32"); // Evap Sys Pressure
  add_to_pid_send_list("33"); //  PID_BARAMETRIC_PRESSURE           0x33  //  * 07 E9 03 41 33 64 00 00 00 00 0002371D

  {
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("42"); //  PID_CONTROL_VOLTAGE               0x42  //  * 07 E8 04 41 42 36 17 00 00 00 00070902
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure

    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("0F"); //  PID_INTAKE_AIR_TEMP               0x0F  //  * 07 E8 03 41 0F 4F 00 00 00 00 000419BF
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("3C"); // Catalyst Temp
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
    add_to_pid_send_list("46"); // Ambiant Temp
    
    add_to_pid_send_list("23"); // Fuel Rail Pressure
    add_to_pid_send_list("32"); // Evap Sys Pressure
  }

  add_to_pid_send_list("01"); // Monitor status since DTCs cleared
  add_to_pid_send_list("23"); // Fuel Rail Pressure
  add_to_pid_send_list("32"); // Evap Sys Pressure
  add_to_pid_send_list("1F"); //  PID_RUN_TIME_SINCE_START          0x1F  //  * 07 E8 04 41 1F 00 AA 00 00 00 0002140B

}

string AUTOMOBILE::requested_pid()
{
  return REQUESTED_PID;
}

bool AUTOMOBILE::active()
{
  return AVAILABILITY.is_active();
}

void AUTOMOBILE::process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time)
{
  int pid_recieved = 0;
  bool identified = false;

  // Activity Checks for other parts of the program to reference.
  if (AVAILABILITY.check_for_live_data(tmeFrame_Time) == false)
  {
    if (AVAILABILITY.set_active(STATUS, false, tmeFrame_Time) == true)
    {
      CHANGED = true;
    }
  }

  if (Com_Port.recieve_size() > 0)
  {
    while (Com_Port.recieve_size() > 0)
    {
      string input = trim(Com_Port.recieve());

      if (Com_Port.PROPS.PRINT_RECEIVED_DATA)
      {
        cons.printw(input);
      }

      if(input.size() == 38)  // v3 and v4: 2b pid 8b data 4b time_elapse
      {
        message_count++;
        STATISTICS.RAS_RECIEVED_CURRENT_COUNT_INC();

        if (parse(input, pid_recieved, identified) == true)
        {
          AUTOMOBILE_DATA_LINE message;
          bool recieved_cam_message = false;

          // Check to see what was requested and put data in correct 
          //  spot.
          if (pid_recieved == 0x7E8)
          {
            message = DATA.AD_7E8;
            recieved_cam_message = true;
          }
          else if (pid_recieved == 0x7E9)
          {
            message = DATA.AD_7E9;
            recieved_cam_message = true;
          }
          else if (pid_recieved == 0x7EA)
          {
            message = DATA.AD_7EA;
            recieved_cam_message = true;
          }
          else if (pid_recieved == 0x7EB)
          {
            message = DATA.AD_7EB;
            recieved_cam_message = true;
          }

          // Check to see if the message received was directed to this device, or was passively read.
          if (recieved_cam_message == true)
          {
            bool message_processed = false;

            // Check message to make sure its in correct format
            if (message.DATA[1] == 0x41)
            {

              if (message.DATA[2] == 0x01)  // System (MIL)
              {
                // Dont send another request until wait delay is up
                STATUS.SYSTEM.store_malfunction_indicator_light(bit_value(message.DATA[3], 7));

                message_processed = true;
              }

              if (message.DATA[2] == 0x05)  // Engine coolant temperature - 07 E8 03 41 05 7A 00 00 00 00 00125F9C
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_coolant_05(message.DATA[3]);

                message_processed = true;
              }

              /*
              Unknown
              if (message.DATA[2] == 0x67)  // Engine coolant temperature
              {
                // Dont send another request until wait delay is up
                // REQUESTED_PID_TIMER_WAIT.ping_up(tmeFrame_Time, REQUESTED_PID_TIMER_WAIT_DELAY);
                STATUS.TEMPS.store_coolant_67(message.DATA[3], message.DATA[4]);
              }
              */

              else if (message.DATA[2] == 0x0F)  // Intake air temperature - 07 E8 03 41 0F 4D 00 00 00 00 00127FEE
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_air_intake_0f(message.DATA[3]);

                message_processed = true;
              }

              /*
              Unknown
              if (message.DATA[2] == 0x68)  // Intake air temperature
              {
                // Dont send another request until wait delay is up
                // REQUESTED_PID_TIMER_WAIT.ping_up(tmeFrame_Time, REQUESTED_PID_TIMER_WAIT_DELAY);
                STATUS.TEMPS.store_air_intake_68(message.DATA[3], message.DATA[4]);
              }
              */
              
              else if (message.DATA[2] == 0x46)  // Ambient air temperature - 07 E8 03 41 46 4A 00 00 00 00 001264BF
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_ambiant_air_46(message.DATA[3]);

                message_processed = true;
              }
              
              else if (message.DATA[2] == 0x5C)  // Engine oil temperature
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_oil_5c(message.DATA[3]);

                message_processed = true;
              }
              
              else if (message.DATA[2] == 0x6B)  // Exhaust gas recirculation temperature
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_exhaust_gas_6b(message.DATA[3]);

                message_processed = true;
              }
              
              else if (message.DATA[2] == 0x84)  // Manifold surface temperature
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_manifold_surface_84(message.DATA[3]);

                message_processed = true;
              }
              
              else if (message.DATA[2] == 0x33)  // * Absolute Barometric Pressure - 07 E8 03 41 33 64 00 00 00 00 0012CA5C
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_abs_baro_33(message.DATA[3]);

                message_processed = true;
              }
            
              else if (message.DATA[2] == 0x1F)  // Run time since engine start - 07 E8 04 41 1F 00 AA 00 00 00 0002140B
              {
                // Dont send another request until wait delay is up
                STATUS.ELECTRICAL.store_run_time_since_start_1f(message.DATA[3], message.DATA[4]);

                message_processed = true;
              }

              else if (message.DATA[2] == 0x23)  // Fuel Rail Gauge Pressure (diesel, or gasoline direct injection) - 07 E8 04 41 23 00 E1 00 00 00 00032EEB
              {
                // Dont send another request until wait delay is up
                STATUS.FUEL.store_fuel_rail_pressure_23(message.DATA[3], message.DATA[4]);

                message_processed = true;
              }

              else if (message.DATA[2] == 0x3C)  //  Catalyst Temperature: Bank 1, Sensor 1 - 07 E8 04 41 3C 19 A3 00 00 00 00066801
              {
                // Dont send another request until wait delay is up
                STATUS.TEMPS.store_catalyst_3c(message.DATA[3], message.DATA[4]);

                message_processed = true;
              }

              else if (message.DATA[2] == 0x32)  //  Evap. System Vapor Pressure - 07 E8 04 41 32 FF 77 00 00 00 000662D9
              {
                // Dont send another request until wait delay is up
                STATUS.FUEL.store_evap_system_vap_pressure_32(message.DATA[3], message.DATA[4]);

                message_processed = true;
              }

              else if (message.DATA[2] == 0x42)  // Control Unit Voltage - 07 E8 04 41 42 36 2E 00 00 00 0003E73E
              {
                // Dont send another request until wait delay is up
                STATUS.ELECTRICAL.store_control_voltage_42(message.DATA[3], message.DATA[4]);

                message_processed = true;
              }
            }

            // Unkown message
            if (message_processed == false) 
            {
              //ALERTS_2.res_alert_no_condition(Thread, RESERVE_ALERT_UNKNOWN_MESSAGE, "Unparsed Message Received.");
              //cons.printw("Unparsed Message From Can Bus: " + message.ORIG);

              // ADD ALERT HERE!!!
            }
          }
          
          else if (identified)  // Message was read passivly, perform immediate calculation on some.
                                // Only if message was identified.
          {
            // High level Compute requiring calculation on all data.
            //  Compute on all data but can be processor intensive.

            // Only works if parse returns true as PID_RECIEVED.
            
            // Statistics from RasCan
            if (pid_recieved == 0xFFFF)
            {
              STATISTICS.process_received(DATA.AD_FFFF.DATA[0], DATA.AD_FFFF.DATA[1], DATA.AD_FFFF.DATA[2]);
            }

            // Steering Wheel Angle
            else if (pid_recieved == 0x0010)
            {
              STATUS.STEERING.store_steering_wheel_angle((DATA.AD_10.DATA[6] *256) + DATA.AD_10.DATA[7], 
                                                          DATA.AD_10.DATA[2]);
            }
                  
            // --------------------------------------------------------------------------------------
            //  Fraggin' Stupid Pleb message trap / scrubber.

            // Transmission Reported Speed
            else if (pid_recieved == 0x00F0)
            {
              STATUS.SPEED.store_trans((DATA.AD_F0.DATA[0] *256) + DATA.AD_F0.DATA[1], 1.13, tmeFrame_Time, 
                                        DATA.AD_F0.TIMESTAMP_MESSAGE_SENT, 
                                        (STATUS.SPEED.SPEED_LB_TIRE.val_kmph() + STATUS.SPEED.SPEED_LB_TIRE.val_kmph() + 
                                        STATUS.SPEED.SPEED_LB_TIRE.val_kmph() + STATUS.SPEED.SPEED_LB_TIRE.val_kmph()) /4);
            }

            // Individual Tire Speed
            else if (pid_recieved == 0x0190)
            {
              STATUS.SPEED.store_LF((DATA.AD_190.DATA[0] *256) + DATA.AD_190.DATA[1], tmeFrame_Time, DATA.AD_190.TIMESTAMP_MESSAGE_SENT);
              STATUS.SPEED.store_RF((DATA.AD_190.DATA[2] *256) + DATA.AD_190.DATA[3], tmeFrame_Time, DATA.AD_190.TIMESTAMP_MESSAGE_SENT);
              STATUS.SPEED.store_LB((DATA.AD_190.DATA[4] *256) + DATA.AD_190.DATA[5], tmeFrame_Time, DATA.AD_190.TIMESTAMP_MESSAGE_SENT);
              STATUS.SPEED.store_RB((DATA.AD_190.DATA[6] *256) + DATA.AD_190.DATA[7], tmeFrame_Time, DATA.AD_190.TIMESTAMP_MESSAGE_SENT);

              // Average Tire Speed
              {
                STATUS.SPEED.SPEED_ALL_TIRES_AVERAGE.store_meters_per_second(((STATUS.SPEED.SPEED_LF_TIRE.val_meters_per_second() + 
                                                                  STATUS.SPEED.SPEED_RF_TIRE.val_meters_per_second() +
                                                                  STATUS.SPEED.SPEED_LB_TIRE.val_meters_per_second() + 
                                                                  STATUS.SPEED.SPEED_RB_TIRE.val_meters_per_second()) / 4), 
                                                                  tmeFrame_Time,
                                                                  STATUS.SPEED.SPEED_LB_TIRE.time_stamp_time_sent());    
              }

              // Find slowest tire
              {
                float lowest_tire_speed = min({STATUS.SPEED.SPEED_LF_TIRE.val_meters_per_second(), 
                                              STATUS.SPEED.SPEED_RF_TIRE.val_meters_per_second(), 
                                              STATUS.SPEED.SPEED_LB_TIRE.val_meters_per_second(), 
                                              STATUS.SPEED.SPEED_RB_TIRE.val_meters_per_second()});

                STATUS.SPEED.SPEED_ALL_TIRES_LOWEST.store_meters_per_second(lowest_tire_speed, STATUS.SPEED.SPEED_LB_TIRE.time_stamp(), 
                                                                STATUS.SPEED.SPEED_LB_TIRE.time_stamp_time_sent());
              }   
            }
          
            AVAILABILITY.set_active(STATUS, true, tmeFrame_Time);
            CHANGED = true;
          }

          else    // Message was not identified

          {
            message = DATA.AD_UNKNOWN;
            //ALERTS_2.res_alert_no_condition(Thread, RESERVE_ALERT_UNKNOWN_MESSAGE, "Unidentfied Message Received.");
            //cons.printw("Unidentfied Message From Can Bus: " + message.ORIG);
          
            // ADD ALERT HERE!!!
          }
        }

        // Process Nova Calculations
        NOVA.process(tmeFrame_Time, LATEST_PID, LATEST_DATA);

      }
      else  // Message was non standard size
      {
        // output it to the console.
        /*
        if (input.size() > 4)
        {
          if (input.substr(0, 4) == "CAN:")
          {
            cons.printw(input);
          }
          else
          {
            cons.printw("Errored Message: " + input);
          }
        }
        else
        {
          cons.printw("Errored Message: " + input);
        }
        */
        if (input.size() > 4)
        {
          if (input.substr(0, 4) == "CAN:")
          {
            cons.printw(input);
          }
        }
      }
    }
  }

  // Send pid to get values
  if (REQUESTED_PID_TIMER_WAIT.ping_down(tmeFrame_Time) == false && active() == true)
  {
    // Dont send another request until wait timeout is up.
    //REQUESTED_PID_TIMER_WAIT.ping_up(tmeFrame_Time, REQUESTED_PID_TIMER_TIMEOUT_DELAY);
    REQUESTED_PID_TIMER_WAIT.ping_up(tmeFrame_Time, REQUESTED_PID_TIMER_WAIT_DELAY);

    REQUESTED_PID_SEND_LIST_POSITION ++;

    if (REQUESTED_PID_SEND_LIST_POSITION < 0 || REQUESTED_PID_SEND_LIST_POSITION > (int)REQUESTED_PID_SEND_LIST.size() -1)
    {
      REQUESTED_PID_SEND_LIST_POSITION = 0;
    }
    
    REQUESTED_PID = REQUESTED_PID_SEND_LIST[REQUESTED_PID_SEND_LIST_POSITION];

    if (PROPS.DATA_REQUEST_ENABLE)
    {
      Com_Port.send(REQUESTED_PID);
    }
  }
}

void AUTOMOBILE::translate(DNFWTS_ &Dnfwts, unsigned long tmeFrame_Time)
{
  if (AVAILABILITY.is_active() == true)
  {
    if (DATA.AD_130.CHANGED)
    {
      STATUS.SPEED.store_dash(DATA.AD_130.DATA[6], DATA.AD_130.DATA[7], tmeFrame_Time, DATA.AD_190.TIMESTAMP_MESSAGE_SENT);
      DATA.AD_130.CHANGED = false;
    }

    // Transmission Gear Position
    if (DATA.AD_F0.CHANGED)
    {
      STATUS.GEAR.store(DATA.AD_F0.DATA[2]);
      DATA.AD_F0.CHANGED = false;
    }
    
    // BRAKE PRESSURE
    if (DATA.AD_252.CHANGED)
    {
      STATUS.BRAKE.store_value(DATA.AD_252.DATA[1], DATA.AD_252.DATA[2]);
      DATA.AD_252.CHANGED = false;
    }

    // LOAD
    // ACCELERATOR POSTION
    if (DATA.AD_80.CHANGED)
    {
      STATUS.POWER.store((DATA.AD_80.DATA[5] *256) + DATA.AD_80.DATA[6]);
      STATUS.ACCELERATOR.store_value(DATA.AD_80.DATA[1]);
      DATA.AD_80.CHANGED = false;
    }

    // INDICATORS int Lights, int Parking_Brake, int Ignition
    if (DATA.AD_360.CHANGED)
    {
      STATUS.INDICATORS.store_lights_high_beam(DATA.AD_360.DATA[0]);
      DATA.AD_360.CHANGED = false;
    }

    // LIGHTS, IGNITION SWITCH
    if (DATA.AD_C8.CHANGED)
    {
      STATUS.INDICATORS.store_on(DATA.AD_C8.DATA[3]);
      STATUS.INDICATORS.store_ignition(DATA.AD_C8.DATA[5]);
      STATUS.INDICATORS.store_ignition_switch(DATA.AD_C8.DATA[5]);
      STATUS.INDICATORS.store_lights(DATA.AD_C8.DATA[7]);
      DATA.AD_C8.CHANGED = false;
    }

    // CRUISE CONTROL
    if (DATA.AD_200.CHANGED)
    {
      STATUS.INDICATORS.store_cruise_control(DATA.AD_200.DATA[6], DATA.AD_200.DATA[7], .312);
      DATA.AD_200.CHANGED = false;
    }

    // SIGNAL LIGHTS
    if (DATA.AD_260.CHANGED)
    {
      STATUS.INDICATORS.store_signal(DATA.AD_260.DATA[3]);
      DATA.AD_260.CHANGED = false;
    }

    // HAZARD LIGHTS
    if (DATA.AD_310.CHANGED)
    {
      STATUS.INDICATORS.store_hazards(tmeFrame_Time, DATA.AD_310.DATA[4]);
      DATA.AD_310.CHANGED = false;
    }

    // Dashboard Screen
    if (DATA.AD_140.CHANGED)
    {
      STATUS.GUAGES.SCREEN.store_value(DATA.AD_140.DATA[0], DATA.AD_140.DATA[1], DATA.AD_140.DATA[2], DATA.AD_140.DATA[3], 
                                        DATA.AD_140.DATA[4], DATA.AD_140.DATA[5], DATA.AD_140.DATA[6], DATA.AD_140.DATA[7]);
      DATA.AD_140.CHANGED = false;
    }

    // FUEL
    //if (DATA.AD_200.CHANGED)
    //{
    //  STATUS.FUEL.store_consumed(DATA.AD_200.DATA[7]);
    //  DATA.AD_200.CHANGED = false;
    //}
    
    // FUEL LEVEL PERCENTAGE
    if (DATA.AD_C0.CHANGED)
    {
      STATUS.FUEL.store_percentage(DATA.AD_C0.DATA[7]);
      DATA.AD_C0.CHANGED = false;
    }
    
    // FUEL LEVEL GALLONS
    if (DATA.AD_380.CHANGED)
    {
      STATUS.FUEL.store_level(DATA.AD_380.DATA[7]);
      DATA.AD_380.CHANGED = false;
    }

    // Guages
    
    if (DATA.AD_100.CHANGED)
    {
      STATUS.GUAGES.store_coolant(DATA.AD_100.DATA[3]);
      DATA.AD_100.CHANGED = false;
    }

    // --------------------------------------------------------------------------------------
    //  Fraggin' Stupid Pleb message trap / scrubber.

    //  This fragging code zone is dedicated to filtering out OP Cell phone interference sourced
    //  from zoned-out roamers (noobs on a warpath or some equally moronic and destructive crap) with cell
    //  phones and all that drek they're lugging in their chrome-stuffed backpacks.
    //  Don't know exactly what the hell is gettin' pumped out (burst electro mag interference, or some
    //  dreary Chinese self-surveillance kit swiped off that shady ali express), but whatever the frag it is,
    //  it's skyrocketed to the magnitude of causing engine misfires, glitched-out gear shifts,
    //  door sensor meltdowns, speed miscalculations, and chombatta, even a crash of the Iphone
    //  music app (only connected to the ride via the fraggin' Bluetooth.) I ain't building no redundant system
    //  or some glitchy-ass data checker. +90%hitrte

    // filtering out an all zero field - street level stupidity:                      00 D0 00 00 00 00 00 00 00 00 006E91ED
    // filtering out partial zero field - college, and professional level stupidity:  00 D0 7F 00 00 00 00 00 00 00 00DD8BCC
    // The car should have never sent those messages while gear selection is in drive, much less while drive 50mph.
    // Indicates TCM Failure.

    // Gear Lever Selection
    STATUS.GEAR.store_gear_selection(DATA.AD_D0.DATA[1], DATA.AD_D0.DATA[2]);
    
    // Door Open or Closed
    //  03 60 C0 40 3F 07 00 37 B8 7B 01097D8C
    //  03 60 C0 40 00 00 00 00 00 00 01097E20 - ZEROED DATA
    //  03 60 C0 40 3F 07 00 37 B8 7B 01097F4C

    CALCULATED.CAM_COMM_ERRORS.DOORS.set_error(STATUS.DOORS.store(DATA.AD_360.DATA[2]));
      // Error Check Incomplete

    // RPM
    CALCULATED.CAM_COMM_ERRORS.RPM1.set_error(STATUS.RPM.store((DATA.AD_90.DATA[4] *256) + DATA.AD_90.DATA[5]));
    
    CALCULATED.CAM_COMM_ERRORS.RPM2.set_error(STATUS.RPM.store_2((DATA.AD_90.DATA[2] *256) + DATA.AD_90.DATA[3]));
    

    // --------------------------------------------------------------------------------------
    
    // Low level Compute not requiring calculation on all data.
    //  Fast but not fully acurate.
    //  Currently call just before the data is displayed.
    CALCULATED.compute_low(Dnfwts, STATUS, tmeFrame_Time);

    /*
    // Move **********************************************************************************
    //Capture errors
    if (STATUS.GEAR.gear_selection_park() == true && STATUS.SPEED.SPEED_TRANS.val_mph() > 10)
    {
      ERROR_LOG_MESSAGE.push_back("Velicity over 10 and Gear in Park:");
      ERROR_LOG_MESSAGE.push_back("  " + DATA.AD_D0.ORIG);
      ERROR_LOG_MESSAGE.push_back("  " + DATA.AD_F0.ORIG);
    }

    if (ERROR_LOG_MESSAGE.size() >0)
    {
      deque_string_to_file(ERROR_LOG_FILENAME, ERROR_LOG_MESSAGE, true);
    }
    // Move **********************************************************************************
    */

    // Log File
    //  Create a status log file every 5 minutes
    //  Convert to json in future.
    if (STATUS_HISTORY_TIMER.ping_down(tmeFrame_Time) == false && TEST_DATA_CAN_BUS == false)
    {
      STATUS_HISTORY_MESSAGE.push_back(
                  file_format_system_hour_minutes_seconds() + ", " +
                  "Voltage, " + STATUS.ELECTRICAL.CONTROL_UNIT_42.v() + ", " + 
                  "Amiant Air Temp, " + STATUS.TEMPS.AMBIANT_AIR_46.c() + ", " + 
                  "Air Intake Temp, " + STATUS.TEMPS.AIR_INTAKE_0f.c() + ", " + 
                  "Coolant Temp, " + STATUS.TEMPS.COOLANT_05.c() + ", " + 
                  "Catalyst Temp, " + STATUS.TEMPS.CATALYST_3C.c() + ", " + 
                  "S-Temp, " + to_string(CALCULATED.s_temp()) + ", " + 

                  "TTL LF, " + CALCULATED.LF_TTL.life_percentage_mean() + ", "
                  "TTL RF, " + CALCULATED.RF_TTL.life_percentage_mean() + ", "
                  "TTL LB, " + CALCULATED.LB_TTL.life_percentage_mean() + ", "
                  "TTL RB, " + CALCULATED.RB_TTL.life_percentage_mean()
      );

      deque_string_to_file(PROPS.STATUS_HISTORY_HISTORY_DIR + file_format_system_date() + ".txt", STATUS_HISTORY_MESSAGE, true);

      STATUS_HISTORY_MESSAGE.clear();

      STATUS_HISTORY_TIMER.ping_up(tmeFrame_Time, 60000 * 5);
    }
  }
}

#endif
