// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_automobile.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_AUTOMOBILE_H
#define WIDGETS_AUTOMOBILE_H

#include <stdio.h>
#include <string>
#include <vector>

// Rasfled Includes
#include "system.h"
#include "auto.h"
#include "screen4_helper.h"
#include "widgets.h"
#include "widgets_drawing.h"

#include "stringthings.h"
#include "fled_time.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

/*
class BLANK_PROPERTIES
{
  public:

  COLOR_COMBO COLOR;
};

class BLANK
{
  private:

  string TEXT = "";

  public:

  W_TEXT_PROPERTIES PROPS;

  void update_text(unsigned long tmeCurrentMillis, string Text);

  void draw(unsigned long tmeCurrentMillis);

};
*/

// ---------------------------------------------------------------------------------------

class T_LARGE_NUMBER_DISPLAY_PROPERTIES
{
  public:

  string LABEL = " ";
  bool LABEL_ON_LEFT = true;

  float WITHIN_VALUE = .5;

  bool DISPLAY_MIN_MAX = false;

  int MIN_MAX_TIME_SPAN = 60000;
  int MIN_MAX_TIME_SLICES = 20;

  int WHEEL_FRAME_SIZE = 6;

  NEW_COLOR_SCALE COLOR_SCALE;  //  pros COLOR.
};

class T_LARGE_NUMBER_DISPLAY
{
  private:

  bool ACTIVE_WITHIN = false;
  bool IS_TEXT = false;

  IMPACT_RESISTANCE_FLOAT_FRAME_COUNT VALUE_WHEEL;
  BOOLEAN_GRADIANT WHEEL_COLOR;

  float VALUE = 0;
  float VALUE_COMPARE = 0;

  string VALUE_TEXT = ""; 

  MIN_MAX_TIME MIN_MAX;

  void draw_scroll_num(float Value);
  // only for 2 digit numbers

  public:

  T_LARGE_NUMBER_DISPLAY_PROPERTIES PROPS;

  void create();

  void update_value(system_data &sdSysData, float Value, float Compare_Value, bool Is_Within);
  void update_value(system_data &sdSysData, float Value);
  void update_value(system_data &sdSysData, string Text);

  void draw(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class W_GUAGE_PROPERTIES
{
  public:

  //COLOR_COMBO COLOR;
  string LABEL = " ";

  float VALUE_MIN = 0;
  float VALUE_MAX = 75;

  int MIN_MAX_TIME_SPAN = 60000;
  int MIN_MAX_TIME_SLICES = 20;

};

class W_GUAGE
{
  private:

  WIDGET_DEFAULTS DEFAULTS;

  W_TEXT DSP_MIN;
  W_TEXT DSP_MAX;
  W_TEXT DSP_VALUE;

  float VALUE = 0;
  MIN_MAX_TIME MIN_MAX;

  public:

  W_GUAGE_PROPERTIES PROPS;

  void create();

  void update_value(system_data &sdSysData, float value);

  void draw(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class T_DATA_DISPLAY_PROPERTIES
{
  public:

  string LABEL = "";
  int LABEL_TEXT_SIZE = -1;

  COLOR_COMBO COLOR;            // If Color Scale not active then Color defaults to 
  NEW_COLOR_SCALE COLOR_SCALE;  //  pros COLOR.
};

class T_DATA_DISPLAY
// Setup a color scale based on value
// Colors will not update on sys data color map update.
{
  private:

  WIDGET_DEFAULTS DEFAULTS;

  W_TEXT LABEL;
  W_TEXT DATA;

  ImVec2 LABEL_PIXEL_SIZE;
  
  string VALUE_STRING = "";
  float VALUE_FLOAT = 0;

  public:

  T_DATA_DISPLAY_PROPERTIES PROPS;

  void create(system_data &sdSysData);

  void update_value(system_data &sdSysData, string String_Value, float Float_Value);
  void update_value(system_data &sdSysData, string String_Value);

  void draw(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class DISPLAY_DATA_AUTOMOBILE
{
  public:

  // Data

  string LATEST_UNKNOWN_MESSAGE = "";

  int MESSAGES = -1;
  
  float SPEED = 0;
  float SPEED_IMPRES = 0;
  float ACCELERATION = 0;

  float CRUISE_CONTROL_SPEED = 0;
  bool CRUISE_CONTROL_SET = false;

  int GEAR_VAL = 0;
  string GEAR = "X";
  string GEAR_SELECTION = "X";

  int RPM = 0;
  int TORQUE_DEMANDED = 0;

  // Pressure

  string FUEL_RAIL_PRESSURE = "X";
  float FUEL_RAIL_PRESSURE_VAL = 0;
  string EVAP_SYSTEM_VAP_PRESSURE = "X";
  float EVAP_SYSTEM_VAP_PRESSURE_VAL = 0;

  string VOLTAGE = "X";
  float VOLTAGE_VAL = 0;

  string BAROMETER = "X";
  float BAROMETER_VAL = 0;

  int CAM_COMM_ERR = 0;

  // Steering

  float STEERING_WHEEL_ANGLE_VAL = 0;
  bool STEERING_WHEEL_LEFT_OF_CENTER_VAL = false;
  string STEERING_WHEEL_ANGLE = "X";
  string STEERING_WHEEL_LEFT_OF_CENTER = "X";
  string STEERING_WHEEL_TURNING_DIRECTION = "X";

  // Temp

  string TEMP_AMBIANT_STRING = "";
  float TEMP_AMBIANT = 0;
  
  string TEMP_AIR_INTAKE_STRING = "";
  float TEMP_AIR_INTAKE = 0;
  
  string TEMP_COOLANT_STRING = "";
  float TEMP_COOLANT = 0;

  string TEMP_CATALYST_STRING = "";
  float TEMP_CATALYST = 0;

  string TEMP_S_TEMP_STRING = "";
  float TEMP_S_TEMP = 0;

  bool LIGHTS_HEDLIGHTS_ON = false;
  bool LIGHTS_PARKING_ON = false;
  bool LIGHTS_BEAM_ON = false;
  string LIGHTS_SWITCH = "";

  bool PARKING_BRAKE = false;

  string FUEL_LEVEL = "";
  float FUEL_LEVEL_VAL = 0;

  // TIRE TTL 

  float LF_TTL_BUFFER_FILL = 0;
  float RF_TTL_BUFFER_FILL = 0;
  float LB_TTL_BUFFER_FILL = 0;
  float RB_TTL_BUFFER_FILL = 0;

  float LF_TTL_BUFFER_FILL_MAX = 0;
  float RF_TTL_BUFFER_FILL_MAX = 0;
  float LB_TTL_BUFFER_FILL_MAX = 0;
  float RB_TTL_BUFFER_FILL_MAX = 0;

  string LF_TTL = "";
  float LF_TTL_VAL = 0;
  string RF_TTL = "";
  float RF_TTL_VAL = 0;
  string LB_TTL = "";
  float LB_TTL_VAL = 0;
  string RB_TTL = "";
  float RB_TTL_VAL = 0;

  // Large Displays on Main Sidebar Screen
  /*
  T_LARGE_NUMBER_DISPLAY L_SPEED;
  T_LARGE_NUMBER_DISPLAY L_ACCELERATION;
  T_LARGE_NUMBER_DISPLAY L_GEAR;
  T_LARGE_NUMBER_DISPLAY L_TACH;
  */

  // Large Displays on Main Sidebar Screen
  T_LARGE_NUMBER_DISPLAY L_SPEED_SB;
  T_LARGE_NUMBER_DISPLAY L_ACCELERATION_SB;
  T_LARGE_NUMBER_DISPLAY L_GEAR_SB;
  T_LARGE_NUMBER_DISPLAY L_TACH_SB;
  T_LARGE_NUMBER_DISPLAY L_VOLTAGE_SB;
  T_LARGE_NUMBER_DISPLAY L_S_TEMP_SB;

  // Display Data

  T_DATA_DISPLAY D_FUEL_RAIL_PRESSURE;
  T_DATA_DISPLAY D_EVAP_SYSTEM_VAP_PRESSURE;
  T_DATA_DISPLAY D_VOLTAGE;
  T_DATA_DISPLAY D_BAROMETER;
  T_DATA_DISPLAY D_CAM_COMM_ERROR;

  T_DATA_DISPLAY D_TEMP_AMBIANT;
  T_DATA_DISPLAY D_TEMP_INTAKE;
  T_DATA_DISPLAY D_TEMP_COOLANT;
  T_DATA_DISPLAY D_TEMP_CATALYST;
  T_DATA_DISPLAY D_TEMP_SUPER_TEMP;

  ONE_CHAR_LINE_GRAPH OC_LF_TTL;
  ONE_CHAR_LINE_GRAPH OC_RF_TTL;
  ONE_CHAR_LINE_GRAPH OC_LB_TTL;
  ONE_CHAR_LINE_GRAPH OC_RB_TTL;

  T_DATA_DISPLAY D_LF_TTL;
  T_DATA_DISPLAY D_RF_TTL;
  T_DATA_DISPLAY D_LB_TTL;
  T_DATA_DISPLAY D_RB_TTL;

  T_DATA_DISPLAY D_LIGHTS;

  T_DATA_DISPLAY D_FUEL_LEVEL;

  T_DATA_DISPLAY D_PARKING_BRAKE;

  T_DATA_DISPLAY D_CRUISE_ON;
  T_DATA_DISPLAY D_CRUISE_SPEED;

  // Guages

  BAR_TECH TB_STEERING;
  BAR_TECH TB_SPEED;
  BAR_TECH TB_ACCELERATION;
  BAR_TECH TB_RPM;
  BAR_TECH TB_TORQUE;
  
  TIMED_PING GEAR_SWITCH_DELAY;
  //BAR_TECH TB_RPM_G1;
  //BAR_TECH TB_RPM_G2;
  //BAR_TECH TB_RPM_G3;
  //BAR_TECH TB_RPM_G4;
  //BAR_TECH TB_RPM_G5;

  /*
  W_GUAGE G_TEMP_AMBIANT;
  W_GUAGE G_TEMP_INTAKE;
  W_GUAGE G_TEMP_COOLANT;
  W_GUAGE G_TEMP_CATALYST;
  W_GUAGE G_TEMP_SUPER_TEMP;
  */

  // Plot

  VERTICAL_BAR VB_SPEED;
  VERTICAL_BAR VB_TACH;
  VERTICAL_BAR VB_S_TEMP;

  DRAW_D2_PLOT PLOT_SLOW;

};

class AUTOMOBILE_SCREEN
{
  private:

  DISPLAY_DATA_AUTOMOBILE SDATA;
  
  WIDGET_DEFAULTS DEFAULTS;

  public:
  
  void create(system_data &sdSysData);

  void update(system_data &sdSysData);

  void display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms, 
                                const char *name, bool *p_open, ImGuiWindowFlags flags);

  void display_sidebar(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms, 
                              const char *name, bool *p_open, ImGuiWindowFlags flags);

};

// ---------------------------------------------------------------------------------------
#endif