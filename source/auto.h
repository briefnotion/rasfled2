// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: auto.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTO_H
#define AUTO_H

// Standard Header Files
#include <stdio.h>
#include <string>
#include <deque>

#include "fled_time.h"
#include "stringthings.h"
#include "helper.h"
#include "globe_helper.h"
#include "screen4_helper.h"
#include "auto_nova.h"
#include "dnfwts.h"
#include "commands.h"

#include "comport.h"

using namespace std;

// -------------------------------------------------------------------------------------
//  Functions

int method_temp_1(char Byte_1);

int method_2_byte(char Byte_1, char Byte_2);

float method_2_byte_float(char Byte_1, char Byte_2);

float method_2_byte_div_1000(char Byte_1, char Byte_2);

// -------------------------------------------------------------------------------------
//  Classs

bool check_availability(bool Coded_Availability, bool Source_Availabilty);

class AUTOMOBILE_DATA_LINE
{
  public:

  string ORIG = "";

  int ID = -1;

  int DATA[8] = {0};

  bool CHANGED = true;

  unsigned long TIMESTAMP_MESSAGE_SENT = 0;
    // Measured in miliseconds
    // Time sent on sending unit
    // Not synced with RasFLED clock.
};

class AUTOMOBILE_DATA
{
  public:

  // AD_C0  - Fuel % hex 3A
  // 00 C0 20 2D AC 27 50 00 3A 04

  // Hex 41 Engine Temp (C)
  // 01 00 80 00 41 A9 82 04 03 46

  // (((256*A)+B)/3+22/3)*0.145=32. 

  AUTOMOBILE_DATA_LINE AD_0;
  AUTOMOBILE_DATA_LINE AD_10;   //* 16  - Steering Wheeal Rate and Torue
                                //  00 10 0F 37 00 31 00 10 80 C8
  AUTOMOBILE_DATA_LINE AD_30;
  AUTOMOBILE_DATA_LINE AD_32;
  AUTOMOBILE_DATA_LINE AD_4A;
  AUTOMOBILE_DATA_LINE AD_4B;
  AUTOMOBILE_DATA_LINE AD_50;
  AUTOMOBILE_DATA_LINE AD_60;
  AUTOMOBILE_DATA_LINE AD_64;
  AUTOMOBILE_DATA_LINE AD_6A;
  AUTOMOBILE_DATA_LINE AD_70;   //  112? - Tourqe at Trans
                                //  Torque at Trans bit 25. 1 byte
                                //  00 70 0A 8F 0A 24 6A 24 FA 24
  AUTOMOBILE_DATA_LINE AD_80;   //* 128 - Power or Torque
                                //  00 80 90 00 7D 00 00 80 21 F7

  AUTOMOBILE_DATA_LINE AD_90;   //* 144  - RPM
  AUTOMOBILE_DATA_LINE AD_A0;
  AUTOMOBILE_DATA_LINE AD_B0;
  AUTOMOBILE_DATA_LINE AD_C0;   //? 192 - Guages? Fuel % hex 3A
  AUTOMOBILE_DATA_LINE AD_C8;   //* 200 - Headlamp status, Ignition Status, Parking Brake Status
                                //        Key In          - [5] 4 on, 3 off
                                //        Key Out         - [5] 4 off, 3 on
                                //        Engine Running  - [5] 5 on
  AUTOMOBILE_DATA_LINE AD_D0;   // 208? - Gear Lever Status, Transmission Gear Position
  AUTOMOBILE_DATA_LINE AD_E0;
  AUTOMOBILE_DATA_LINE AD_F0;   //* 240 - Velocity (1,2) and Transmission Gear (3)
  AUTOMOBILE_DATA_LINE AD_F8;
  AUTOMOBILE_DATA_LINE AD_100;  //  256?  - Guages Hex 41 Engine Temp (C)
  AUTOMOBILE_DATA_LINE AD_102;
  AUTOMOBILE_DATA_LINE AD_120;
  AUTOMOBILE_DATA_LINE AD_128;
  AUTOMOBILE_DATA_LINE AD_130;  //* 304 - Speed, Data 6 kph, Data 7 Decimal
  AUTOMOBILE_DATA_LINE AD_138;
  AUTOMOBILE_DATA_LINE AD_140;  // - Steering Torque.
  AUTOMOBILE_DATA_LINE AD_150;
  AUTOMOBILE_DATA_LINE AD_160;
  AUTOMOBILE_DATA_LINE AD_17E;
  AUTOMOBILE_DATA_LINE AD_180;
  AUTOMOBILE_DATA_LINE AD_190;  //* - Tire Speed
  AUTOMOBILE_DATA_LINE AD_1A0;
  AUTOMOBILE_DATA_LINE AD_1C0;
  AUTOMOBILE_DATA_LINE AD_1D0;
  AUTOMOBILE_DATA_LINE AD_1E0;
  AUTOMOBILE_DATA_LINE AD_200;  //* 512 - Fuel Consumed From Start
  AUTOMOBILE_DATA_LINE AD_208;  //  02 08 7E F4 29 E6 40 84 FF 18
  AUTOMOBILE_DATA_LINE AD_210;  //  02 10 00 00 00 00 F2 9B 00 00
  AUTOMOBILE_DATA_LINE AD_213;  //? 531 - Break Pressure?
  AUTOMOBILE_DATA_LINE AD_218;  //? 536 - Odometer
  AUTOMOBILE_DATA_LINE AD_247;
  AUTOMOBILE_DATA_LINE AD_252;  //  02 52 00 00 00 0F FF 20 00 60 *
  AUTOMOBILE_DATA_LINE AD_253;  //  02 10 00 00 00 00 FA 8B 00 00
  AUTOMOBILE_DATA_LINE AD_260;  // Signal Lights D5 D4
                                //? 608 - Auto Start Stop Button?
                                //  02 60 88 82 00 0E 25 61 90 00
  AUTOMOBILE_DATA_LINE AD_270;  //  02 70 5E 4A 04 21 00 00 00 00
  AUTOMOBILE_DATA_LINE AD_280;  //  02 80 46 30 9E 72 05 12 C0 00
  AUTOMOBILE_DATA_LINE AD_290;  //  02 90 06 00 00 00 00 02 D2 0A *
                                //  02 90 06 00 00 00 00 02 D4 0A  
                                //  02 90 06 00 00 00 00 02 C1 0A
  AUTOMOBILE_DATA_LINE AD_2B0;  //  02 B0 2D FE 7B 46 80 00 22 1F
  AUTOMOBILE_DATA_LINE AD_2C0;  //? 704 - RPM delta or Request timing??
  AUTOMOBILE_DATA_LINE AD_2D0;  //  02 D0 00 00 01 FB 73 D0 5A 85
  AUTOMOBILE_DATA_LINE AD_2D4;  //  02 D4 00 00 00 00 BD FE FF FE
  AUTOMOBILE_DATA_LINE AD_2D8;  //  02 D8 04 00 00 80 00 00 00 00
  AUTOMOBILE_DATA_LINE AD_2F0;  //  02 F0 34 30 00 56 79 8D 92 60
  AUTOMOBILE_DATA_LINE AD_310;  //  03 10 D1 40 30 60 1D 42 00 6B
  AUTOMOBILE_DATA_LINE AD_318;  //  
  AUTOMOBILE_DATA_LINE AD_340;  //  03 40 00 00 00 00 03 0A 66 4A
  AUTOMOBILE_DATA_LINE AD_360;  //* 864 -High Beam Status and doors?
  AUTOMOBILE_DATA_LINE AD_380;  //* 896 - Fuel Level
  AUTOMOBILE_DATA_LINE AD_388;
  AUTOMOBILE_DATA_LINE AD_390;  //  03 90 00 00 00 00 0D 52 00 00
  AUTOMOBILE_DATA_LINE AD_398;  //  03 98 11 40 50 32 41 53 54 36
  AUTOMOBILE_DATA_LINE AD_3B4;  //  03 88 00 00 47 45 46 87 28 50
  AUTOMOBILE_DATA_LINE AD_400;  //  temp according to chat
                                //  04 00 24 00 00 00 00 00 00 00
  AUTOMOBILE_DATA_LINE AD_405;  //  04 05 01 67 67 13 ED 45 08 9C - Hazard Lights Blink on [5]7&6
  AUTOMOBILE_DATA_LINE AD_40A;  //  04 0A FF 16 21 00 21 01 10 00 - Partial VIN and other static ids
  AUTOMOBILE_DATA_LINE AD_435;  //  04 35 B2 28 91 DC FF D7 C3 E4
  AUTOMOBILE_DATA_LINE AD_455;  //  04 55 00 00 00 00 00 00 00 00
  AUTOMOBILE_DATA_LINE AD_581;  //  05 81 81 00 FF FF FF FF FF FF
  AUTOMOBILE_DATA_LINE AD_5E2;  //  05 E2 62 00 FF FF FF FF FF FF

  AUTOMOBILE_DATA_LINE AD_73F;  //  07 3f 03 7f 01 22 00 00 00 00

  // Message Recieved Lines
  AUTOMOBILE_DATA_LINE AD_7E8;  //* 2025 - 
  AUTOMOBILE_DATA_LINE AD_7E9;  //* 2025 - 07 E9 03 41 0D 00 00 00 00 00
  AUTOMOBILE_DATA_LINE AD_7EA;  //* 2025 - 
  AUTOMOBILE_DATA_LINE AD_7EB;  //* 2025 - 

  AUTOMOBILE_DATA_LINE AD_FFFF; // RasFLED CAN Message - Statistics
                                //  A and B - 2 byte unsigned int
                                //  C Max number of messages stored in send queue
                                //    Number of messages sent via serial over 1 second
                                //    interval. message not sent if 0.

  AUTOMOBILE_DATA_LINE AD_UNKNOWN;
};

class BYTE_VALUE_8
{
  private:

  int VALUE_0 = 0;
  int VALUE_1 = 0;
  int VALUE_2 = 0;
  int VALUE_3 = 0;
  int VALUE_4 = 0;
  int VALUE_5 = 0;
  int VALUE_6 = 0;
  int VALUE_7 = 0;

  public:

  void store_value(int Value_0, int Value_1, int Value_2, int Value_3, 
                      int Value_4, int Value_5, int Value_6, int Value_7);

  int val_0();
  int val_1();
  int val_2();
  int val_3();
  int val_4();
  int val_5();
  int val_6();
  int val_7();
};

class TEMPERATURE
{
  private:

  float C = -1;
  float F = -1;

  unsigned long TIME_STAMP = -1; // Miliseconds.  Fairly loose timings.

  public:

  void store_c(int Celsius);
  void store_c(float Celsius);
  float val_c();
  float val_f();
  string c();
  string f();

  unsigned long time_stamp();
};

class TIME
{
  private:

  int SECONDS = -1;

  public:

  void store_seconds(int Seconds);
  int val_seconds();
  string seconds();
};

class PRESSURE
{
  private:

  float PA = -1;

  unsigned long TIME_STAMP = -1; // Miliseconds.  Fairly loose timings.

  public:

  void store_kPa(int kPa);
  void store_kPa(float kPa);
  void store_Pa(int Pa);
  void store_Pa(float Pa);

  float val_kPa();
  string kPa();
  float val_Pa();
  string Pa();
  float val_inHg();
  string inHg();


  unsigned long time_stamp();
};

class VOLTAGE
{
  private:

  float V = -1;

  unsigned long TIME_STAMP = -1; // Miliseconds.  Fairly loose timings.

  public:

  void store_v(float Voltage);
  float val_v();
  string v();

  unsigned long time_stamp();
};

class AUTOMOBILE_SYSTEM
{
  private:

  bool MALFUNCTION_INDICATOR_LIGHT = false;

  public:

  void store_malfunction_indicator_light(bool Value);

  bool malfunction_indicator_light();

};

class AUTOMOBILE_DOORS
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  bool LF_DOOR_OPEN = false;
  bool RF_DOOR_OPEN = false;
  bool LB_DOOR_OPEN = false;
  bool RB_DOOR_OPEN = false;

  bool HATCHBACK_DOOR_OPEN = false;

  //bool MOONROOF_DOOR_OPEN = false;  // Not Implemented

  bool HOOD_DOOR_OPEN = false;

  public:

  void set_source_availability(bool Available);
  bool available();

  bool store(int Data);
  // return true if errored.

  bool store_from_alt_source(int Door, bool Value);

  bool lf_door_open();
  bool rf_door_open();
  bool lb_door_open();
  bool rb_door_open();
  bool hatchback_door_open();
  //bool moonroof_door_open();
  bool hood_door_open();
};

class AUTOMOBILE_GUAGES
{
  private:

  bool CODED_AVAILABILITY = false;
  bool SOURCE_AVAILABILITY = false;

  int COOLANT = -1;
  string COOLANT_DISP = "X";

  public:

  BYTE_VALUE_8 SCREEN;

  void set_source_availability(bool Available);
  bool available();

  void store_coolant(int Value);
  int val_coolant();
  string coolant();

};

class AUTOMOBILE_FUEL
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  float CONSUMED = -1.0f;
  string CONSUMED_DISP = "X";

  float PERCENTAGE = -1.0f;
  string PERCENTAGE_DISP = "X";

  float LEVEL_RAW = -1.0f;
  string LEVEL_DISP = "X";

  public:

  PRESSURE FUEL_RAIL_PRESSURE;
  PRESSURE EVAP_SYSTEM_VAP_PRESSURE;

  bool LEVEL_RAW_CHANGED = true;

  void set_source_availability(bool Available);
  bool available();

  void store_consumed(int Consumed);
  void store_percentage(int Percentage);
  void store_level(int Level);
  void store_fuel_rail_pressure_23(int A, int B);
  void store_evap_system_vap_pressure_32(int A, int B);

  float val_consumed();
  string consumed();

  float val_percentage();
  string percentage();

  float val_level_raw();
  string level_raw();
};

class AUTOMOBILE_INDICATORS
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  bool LIGHTS_ON = false;
  bool LIGHTS_HEADLIGHTS_ON = false;
  bool LIGHTS_PARKING_ON = false;
  bool LIGHTS_HIGH_BEAM_ON;
  int LIGHTS_POS = -1;
  bool LIGHT_SWITCH = false;
  string LIGHTS_DESC = "X";

  bool SIGNAL_LEFT = false;
  bool SIGNAL_RIGHT = false;

  bool HAZARDS = false;
  TIMED_PING HAZARD_CHECK;

  int IGNITION_SWITCH = 0;

  bool PARKING_BRAKE = false;
  string PARKING_BRAKE_DESC = "X";

  bool IGNITION = false;

  bool CRUISE_CONTROL = false;
  float CRUISE_CONTROL_SPEED = -1;

  public:

  void process();
  //  To be called at caculate to set headlight and paring status.

  void set_source_availability(bool Available);
  bool available();

  void store_lights(int Lights);
  void store_lights_high_beam(int Data_1);
  void store_on(int Parking_Brake);
  void store_ignition(int Ignition);
  void store_cruise_control(int Data_1, int Data_2, float Multiplier);

  void store_signal(int Signal_Lights);
  void store_hazards(unsigned long current_time, int Hazard);
  void store_ignition_switch(int Key_Switch);

  bool val_light_switch();
  int val_lights_pos();
  string lights_switch();
  bool val_lights_high_beam_on();
  bool val_lights_headlights_on();
  bool val_lights_parking_on();

  bool val_sinal_left();
  bool val_sinal_right();

  bool val_hazards();
  int val_ignition_switch();

  bool val_parking_brake();
  string parking_brake();

  bool val_ignition();
  string ignition();

  bool cruise_control();
  float cruise_control_speed();
};

class AUTOMOBILE_POWER
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  int VAL_LOAD;

  string LOAD;

  public:

  void set_source_availability(bool Available);
  bool available();

  void store(int Load);
  int val_load();
  string load();
};

class AUTOMOBILE_RPM
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  int VAL_RPM = -1;
  string RPM = "X";

  int VAL_RPM_2 = -1;  //  unknown
  string RPM_2 = "X";   //  unknown

  public:

  void set_source_availability(bool Available);
  bool available();

  bool store(int Rpm);
  // return true if errored

  int val_rpm();
  string rpm();

  bool store_2(int Rpm);
  // return true if errored
  
  int val_rpm_2();
  string rpm_2();
};

class AUTOMOBILE_STEERING
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  float PREVIOUS_STEERING_WHEEL_ANGLE = -1;

  int REPORTED_STEERING_WHEEL_ANGLE = -1;
  int CLOCKWISE = -1;
  bool LEFT_OF_CENTER = false;

  float VAL_STEERING_WHEEL_ANGLE = -1;

  public:

  void set_source_availability(bool Available);
  bool available();

  void store_steering_wheel_angle(int Angle, int Direction);
  float val_steering_wheel_angle();
  string steering_wheel_angle();
  string turning_direction();
  bool clockwise();
  bool val_left_of_center();
  string left_of_center();
};

class AUTOMOBILE_VELOCITY
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  float TIRE_MULTIPLIER = 1.126f;

  public:

  VELOCITY SPEED_TRANS;
  
  VELOCITY SPEED_DASH;
  
  VELOCITY SPEED_LF_TIRE;
  VELOCITY SPEED_RF_TIRE;
  VELOCITY SPEED_LB_TIRE;
  VELOCITY SPEED_RB_TIRE;

  VELOCITY SPEED_ALL_TIRES_AVERAGE;
  VELOCITY SPEED_ALL_TIRES_LOWEST;
  
  void set_source_availability(bool Available);
  bool available();

  void store_trans(int kmph, float Multiplier, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent, 
                    float Verify_kmph);
  void store_dash(int Upper, int Lower, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_LF(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_RF(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_LB(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);
  void store_RB(int mps, unsigned long tmeFrame_Time, unsigned long tmeFrame_Time_Sent);

  float tire_multiplier();
};

class AUTOMOBILE_TEMPATURE
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  public:

  TEMPERATURE COOLANT_05;
  //TEMPERATURE COOLANT_67_b;
  //TEMPERATURE COOLANT_67_c;
  
  TEMPERATURE AIR_INTAKE_0f;
  //TEMPERATURE AIR_INTAKE_68_a;
  //TEMPERATURE AIR_INTAKE_68_b;

  TEMPERATURE AMBIANT_AIR_46;

  TEMPERATURE OIL_5c;
  TEMPERATURE EXHAUST_GAS_6b;
  TEMPERATURE MANIFOLD_SURFACE_84;
  
  TEMPERATURE CATALYST_3C;

  PRESSURE BARO_33;

  void set_source_availability(bool Available);
  bool available();

  void store_coolant_05(int Sensor_Temp);
  //void store_coolant_67(int Sensor_Temp_B, int Sensor_Temp_C);

  void store_air_intake_0f(int Sensor_Temp);
  //void store_air_intake_68(int Sensor_Temp_B, int Sensor_Temp_C);

  void store_ambiant_air_46(int Sensor_Temp);
  void store_oil_5c(int Sensor_Temp);
  void store_exhaust_gas_6b(int Sensor_Temp);
  void store_manifold_surface_84(int Sensor_Temp);

  void store_abs_baro_33(int Baro);
  
  void store_catalyst_3c(int A, int B);
};

class AUTOMOBILE_ELECTRICAL
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;

  public:

  VOLTAGE CONTROL_UNIT_42;
  TIME RUN_TIME_SINCE_START;

  void set_source_availability(bool Available);
  bool available();

  void store_control_voltage_42(int Sensor_B, int Sensor_C);  
  void store_run_time_since_start_1f(int Sensor_B, int Sensor_C);
};

class AUTOMOBILE_TRANSMISSION_GEAR
{
  private:

  bool CODED_AVAILABILITY = true;
  bool SOURCE_AVAILABILITY = false;
  
  // Transmission Info
  int REPORTED = -1;
  string SHORT_DESC = "X";
  string LONG_DESC = "X";

  // Transmission Gear Selection
  int GEAR_SELECTION_REPORTED = -1;
  string GEAR_SELECTION_SHORT_DESC = "X";
  string GEAR_SELECTION_LONG_DESC = "X";
  bool GEAR_SELECTION_PARK = false;
  bool GEAR_SELECTION_REVERSE = false;
  bool GEAR_SELECTION_NEUTRAL = false;
  bool GEAR_SELECTION_DRIVE = false;
  bool GEAR_SELECTION_LOW = false;
  
  public:

  void set_source_availability(bool Available);
  bool available();

  void store(int gear);
  int reported();
  string short_desc();
  string long_desc();

  bool store_gear_selection(int Gear, int Gear_Alt);
  // return true if changed.

  int gear_selection_reported();
  string gear_selection_short_desc();
  string gear_selection_long_desc();
  bool gear_selection_park();
  bool gear_selection_reverse();
  bool gear_selection_neutral();
  bool gear_selection_drive();
  bool gear_selection_low();
};

class AUTOMOBILE_ACCELERATOR
{
  private:

  int VAL_VALUE = 0;  // Units unknown
                  // Range?: 0 - (256)

  public:

  void store_value(int A);

  float val_value();
};

class AUTOMOBILE_BRAKE
{
  private:

  int VAL_VALUE = 0;  // Units unknown
                  // Range?: 0 - (256 * 256)

  public:

  void store_value(int A, int B);

  float val_value();
};

class AUTOMOBILE_TRANSLATED_DATA
{
  public:

  AUTOMOBILE_SYSTEM SYSTEM;
  AUTOMOBILE_STEERING STEERING;
  AUTOMOBILE_TRANSMISSION_GEAR GEAR;
  AUTOMOBILE_VELOCITY SPEED;
  AUTOMOBILE_TEMPATURE TEMPS;
  AUTOMOBILE_RPM RPM;
  AUTOMOBILE_POWER POWER;
  AUTOMOBILE_INDICATORS INDICATORS;
  AUTOMOBILE_FUEL FUEL;
  AUTOMOBILE_DOORS DOORS;
  AUTOMOBILE_GUAGES GUAGES;
  AUTOMOBILE_ELECTRICAL ELECTRICAL;
  AUTOMOBILE_ACCELERATOR ACCELERATOR;
  AUTOMOBILE_BRAKE BRAKE;
};

class TIRE_TTL
{  
  // Hardcoding Tire Life Span
  // +0.03 m/s to -0.15 mi/hr, difference of .18 mi/hr, 0 is transmission registered normal tire size.
  // +0.01341 m/s to -0.06705 mi/hr, difference of .18 mi/hr, 0 is transmission registered normal tire size.

  private:

  MIN_MAX_TIME WHEEL_SPEED_PECENTAGE_DIFF_MEAN;
  VELOCITY WHEEL_SPEED_OFFSET;

  float DIFFERANCE_PERCENTAGE = 0;
  float LIFE_PERCENTAGE = 0;

  float TOP_PERCENTAGE = 100.15;
  float LOW_PERCENTAGE = 99.7;

  public:

  void first_run(float Low_Percentage, float Top_Percentage);

  void calculate(VELOCITY Tire_Speed, VELOCITY Transmission_Speed, 
                  unsigned long tmeFrame_Time);

  int slice_size();

  int slice_size_max();

  float val_instant_differance_percentage();

  string instant_differance_percentage();

  float val_life_percentage_mean();

  string life_percentage_mean();

  VELOCITY wheel_speed_offset();
};

class SIMPLE_ERRORS_DATA
{
  private:
  bool TRIGGERED = false;
  bool ERRORED = false;

  public:
  
  bool triggered();

  void set_error(bool Value);
};

class SIMPLE_ERRORS
// Ive written this code before, in the unfinished alerts system.  
//  Reuse the old code or merge if this code stays.
{
  private:

  int ERROR_COUNT = 0;

  public:
  
  SIMPLE_ERRORS_DATA GEAR_SELECTION;
  SIMPLE_ERRORS_DATA DOORS;
  SIMPLE_ERRORS_DATA RPM1;
  SIMPLE_ERRORS_DATA RPM2;

  int error_count();
  // not keeping accurate numbers. one is enough.
};

class AUTOMOBILE_CALCULATED
{
  private:

  bool FIRST_RUN = true;

  TIMED_PING ACCELERATION_TIMER;

  float PREVIOUS_VELOCITY_FOR_ACC;
  unsigned long PREVIOUS_TIME_FOR_ACC;
  
  float S_TEMP = -1;

  VELOCITY PREVIOUS_VELOCITY;

  EMPERICAL FUEL_LEVEL_EMPERICAL;
  string FUEL_LEVEL_EMPERICAL_DISP = "X";
  
  MIN_MAX_TIME ACCELERATION_MIN_MAX_HISTORY;
  IMPACT_RESISTANCE_FLOAT ACCELERATION_QUICK_MEAN_HISTORY;

  public:

  TIRE_TTL UNFILTHERED_LF_TTL;
  TIRE_TTL UNFILTHERED_RF_TTL;
  TIRE_TTL UNFILTHERED_LB_TTL;
  TIRE_TTL UNFILTHERED_RB_TTL;

  TIRE_TTL LF_TTL;
  TIRE_TTL RF_TTL;
  TIRE_TTL LB_TTL;
  TIRE_TTL RB_TTL;

  SIMPLE_ERRORS CAM_COMM_ERRORS;

  void compute_low(DNFWTS_ &Dnfwts, AUTOMOBILE_TRANSLATED_DATA &Status, unsigned long tmeFrame_Time);
  // Low level Compute not requiring calculation on all data.
  //  Fast but not fully acurate.
  //  Currently call just before the data is displayed.

  float acceleration(unsigned long tmeFrame_Time);
  // Acceleration computed from averageing 4 tire speeds and comparing the times
  //  the value was sent.
  //  Respons calculated over 1 second with 4 time slices

  float s_temp();
  // Non standand temperature score computed by 
  //  ambiant temp + intake temp + coolant temp + catalyst temp divided by twenty
  //  then all divided by 4 then subtracted by 30 then multiplied by 4. 
  // The results give a better single value about the amount of heat the system 
  //  is handling
  
  string fuel_level_emperical();
  float fuel_level_emperical_val();
  // Returns Emperical fuel level
};

class COMMUNICATION_STATISTICS
{
  private:

  int CAN_SENT = 0;
  int CAN_MAX_QUEUE = 0;
  int RAS_RECEIVED = 0;
  int RAS_RECIEVED_CURRENT_COUNT = 0;

  int CAN_SENT_TOTAL_COUNT = 0;
  int RAS_RECIEVED_TOTAL_COUNT = 0;
  int ERRORS = 0;

  public:

  void process_received(int SentA, int SentB, int SentC);
  void RAS_RECIEVED_CURRENT_COUNT_INC();

  int can_sent();
  int can_max_queue();

  int ras_recieved();
  
  int can_sent_total_count();
  int ras_recieved_total_count();
  int errors();
};

class AUTOMOBILE_AVAILABILITY
{
  private:

  TIMED_PING ACTIVITY_TIMER;
  bool ACTIVE = false;

  public:

  bool check_for_live_data(unsigned long tmeFrame_Time);
  // returns true if data live.

  bool set_active(AUTOMOBILE_TRANSLATED_DATA &Status, bool Available, unsigned long tmeFrame_Time);
  // returns true if active status changed

  bool is_active();

};

class AUTOMOBILE_PROPERTIES
// Panel border characters.
{
  public:
  
  bool SAVE_TO_LOG = false;
  string SAVE_LOG_FILENAME = "";
  string STATUS_HISTORY_HISTORY_DIR = "";

  bool DATA_REQUEST_ENABLE = true;
};

class AUTOMOBILE
{
  private:

  AUTOMOBILE_AVAILABILITY AVAILABILITY;

  string REQUESTED_PID = "";

  // Satus Log File
  deque<string> STATUS_HISTORY_MESSAGE;
  TIMED_PING STATUS_HISTORY_TIMER;

  // PID Send List
  deque<string> REQUESTED_PID_SEND_LIST;
  int REQUESTED_PID_SEND_LIST_POSITION = -1;

  TIMED_PING REQUESTED_PID_TIMER_WAIT;
  int REQUESTED_PID_TIMER_WAIT_DELAY = 50;
  //int REQUESTED_PID_TIMER_TIMEOUT_DELAY = 250;

  int LATEST_PID;
  int LATEST_DATA[8] = {0};
  // Hoping to return to this because its hacky.

  bool parse(string Line, int &PID_Recieved, bool &Identified);
  // Returns true if line is considered a message recieved.
  //  Message id will be in PID_Recieved

  public:

  bool STREET_LEGAL_MODE = true;

  bool CHANGED = false;

  AUTOMOBILE_DATA DATA;
  AUTOMOBILE_TRANSLATED_DATA STATUS;
  AUTOMOBILE_CALCULATED CALCULATED;
  AUTOMOBILE_PROPERTIES PROPS;
  AUTOMOBILE_NOVA NOVA;
  COMMUNICATION_STATISTICS STATISTICS;

  int message_count = 0;

  void add_to_pid_send_list(string Requested_PID);

  void set_default_request_pid_list();

  string requested_pid();

  bool active();

  void process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time);

  void translate(DNFWTS_ &Dnfwts, unsigned long tmeFrame_Time);
};





#endif
