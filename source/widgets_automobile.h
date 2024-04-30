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
//  Nova Widget
void draw_bit(ImDrawList* Draw_List, system_data &sdSysData, 
              ImVec2 Start_Pos, bool Value, bool Ping, bool Details);

void nova_draw(ImDrawList *Draw_List, system_data &sdSysData,  
                NOVA_BITS_VALUE &Value);

void nova_draw_byte(ImDrawList *Draw_List, system_data &sdSysData,  
                NOVA_BITS_VALUE &Value, int Byte_Position);

void draw_nova_detail(system_data &sdSysData, NOVA_BITS_VALUE &Nova_Value, int &Item_Location);

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

  int WHEEL_FRAME_SIZE = 6;     // Number of draw frames spanned over value changes.
  
  bool VERY_LARGE = false;

  NEW_COLOR_SCALE COLOR_SCALE;  //  pros COLOR.

  bool DISPLAY_MARKER = false;
  int DISPLAY_MARKER_COLOR;
};

class T_LARGE_NUMBER_DISPLAY
{
  private:

  bool ACTIVE_WITHIN = false;
  bool IS_TEXT = false;

  IMPACT_RESISTANCE_FLOAT_FRAME_COUNT VALUE_WHEEL;
  BOOLEAN_GRADIANT WHEEL_COLOR;

  float VALUE = 0.0f;
  float VALUE_COMPARE = 0.0f;

  string VALUE_TEXT = ""; 

  MIN_MAX_TIME MIN_MAX;

  MARKER_GADGET MARKER;

  // ---
  // Positions and values
  ImVec2 FRAME_START_POS = ImVec2(0.0f, 0.0f);
  float PREV_Y_HEIGHT = 0.0f;

  ImVec2 DISPLAY_SIZE;

  ImVec2 ZERO_SIZE = ImVec2(0.0f, 0.0f);

  ImVec2 POS_GRAD_TOP_1 = ImVec2(0.0f, 0.0f);
  ImVec2 POS_GRAD_TOP_2 = ImVec2(0.0f, 0.0f);
  ImVec2 POS_GRAD_BOT_1 = ImVec2(0.0f, 0.0f);
  ImVec2 POS_GRAD_BOT_2 = ImVec2(0.0f, 0.0f);
  // ---

  bool CHANGED = false;

  void draw_scroll_num(ImDrawList *Draw_List, float Value, float Y_Height, ImVec2 Start_Pos, ImVec2 Zero_Font_Size);
  // only for 2 digit numbers

  public:

  T_LARGE_NUMBER_DISPLAY_PROPERTIES PROPS;

  void create();

  void update_value(system_data &sdSysData, float Value, float Compare_Value, bool Is_Within);
  void update_value(system_data &sdSysData, float Value);
  void update_value(string Text);

  bool draw(system_data &sdSysData, bool Draw, float Y_Height);
  bool draw(system_data &sdSysData, bool Draw);
  bool draw(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class W_GUAGE_PROPERTIES
{
  public:

  string LABEL = " ";

  float VALUE_MIN = 0.0f;
  float VALUE_MAX = 75.0f;

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

  float VALUE = 0.0f;
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

  int COLOR;            // If Color Scale not active then Color defaults to 
  NEW_COLOR_SCALE COLOR_SCALE;  //  pros COLOR.

  bool DISPLAY_MARKER = false;
  int DISPLAY_MARKER_COLOR;
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
  float VALUE_FLOAT = 0.0f;

  MARKER_GADGET MARKER;

  public:

  T_DATA_DISPLAY_PROPERTIES PROPS;

  void create(system_data &sdSysData);

  void update_value(system_data &sdSysData, string String_Value, float Float_Value);
  void update_value(system_data &sdSysData, string String_Value);

  void draw(ImDrawList *Draw_List, system_data &sdSysData, bool Draw_Marker);
  void draw(ImDrawList *Draw_List, system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class DISPLAY_DATA_AUTOMOBILE
{
  public:

  // Data

  string LATEST_UNKNOWN_MESSAGE = "";

  int MESSAGES = -1;
  
  float SPEED = 0.0f;
  float SPEED_IMPRES = 0.0f;
  float ACCELERATION = 0.0f;

  float CRUISE_CONTROL_SPEED = 0.0f;
  bool CRUISE_CONTROL_SET = false;

  int GEAR_VAL = 0;
  string GEAR = "X";
  string GEAR_SELECTION = "X";

  int RPM = 0;
  int TORQUE_DEMANDED = 0;

  int BRAKE_POWER = 0;
  int ACCELERATOR_POWER = 0;

  // Pressure

  string FUEL_RAIL_PRESSURE = "X";
  float FUEL_RAIL_PRESSURE_VAL = 0.0f;
  string EVAP_SYSTEM_VAP_PRESSURE = "X";
  float EVAP_SYSTEM_VAP_PRESSURE_VAL = 0.0f;

  string VOLTAGE = "X";
  float VOLTAGE_VAL = 0.0f;

  string BAROMETER = "X";
  float BAROMETER_VAL = 0.0f;

  int CAM_COMM_ERR = 0;
  int CAM_STAT_ERR = 0;

  // Steering

  float STEERING_WHEEL_ANGLE_VAL = 0.0f;
  bool STEERING_WHEEL_LEFT_OF_CENTER_VAL = false;
  string STEERING_WHEEL_ANGLE = "X";
  string STEERING_WHEEL_LEFT_OF_CENTER = "X";
  string STEERING_WHEEL_TURNING_DIRECTION = "X";

  // Temp

  string TEMP_AMBIANT_STRING = "";
  float TEMP_AMBIANT = 0.0f;
  
  string TEMP_AIR_INTAKE_STRING = "";
  float TEMP_AIR_INTAKE = 0.0f;
  
  string TEMP_COOLANT_STRING = "";
  float TEMP_COOLANT = 0.0f;

  string TEMP_CATALYST_STRING = "";
  float TEMP_CATALYST = 0.0f;

  string TEMP_S_TEMP_STRING = "";
  float TEMP_S_TEMP = 0.0f;

  bool LIGHTS_HEDLIGHTS_ON = false;
  bool LIGHTS_PARKING_ON = false;
  bool LIGHTS_BEAM_ON = false;
  string LIGHTS_SWITCH = "";

  bool PARKING_BRAKE = false;

  string FUEL_LEVEL = "";
  float FUEL_LEVEL_VAL = 0.0f;

  // TIRE TTL 

  float LF_TTL_BUFFER_FILL = 0.0f;
  float RF_TTL_BUFFER_FILL = 0.0f;
  float LB_TTL_BUFFER_FILL = 0.0f;
  float RB_TTL_BUFFER_FILL = 0.0f;

  float LF_TTL_BUFFER_FILL_MAX = 0.0f;
  float RF_TTL_BUFFER_FILL_MAX = 0.0f;
  float LB_TTL_BUFFER_FILL_MAX = 0.0f;
  float RB_TTL_BUFFER_FILL_MAX = 0.0f;

  string LF_TTL = "";
  float LF_TTL_VAL = 0.0f;
  string RF_TTL = "";
  float RF_TTL_VAL = 0.0f;
  string LB_TTL = "";
  float LB_TTL_VAL = 0.0f;
  string RB_TTL = "";
  float RB_TTL_VAL = 0.0f;

  // Errors
  int PREV_D_CAM_COMM_ERROR = 0;
  int PREV_D_CAM_STAT_ERROR = 0;

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

  // ------------------------------------------
  // Outside
  bool L_SPEED_OUTSIDE_DRAW = false;
  T_LARGE_NUMBER_DISPLAY L_SPEED_OUTSIDE;

  // ------------------------------------------
  // Mid Top
  DRAW_D2_PLOT PLOT_SLOW;

  BAR_TECH VB_SPEED;
  //BAR_TECH VB_TACH;
  BAR_TECH VB_S_TEMP;
  BAR_TECH VB_S_FUEL;
  BAR_TECH VB_S_VOLTAGE;

  // ------------------------------------------
  // Mid Bottom
  BAR_TECH TB_STEERING;
  BAR_TECH TB_SPEED;
  BAR_TECH TB_ACCELERATION;
  BAR_TECH TB_RPM;
  BAR_TECH TB_TORQUE;

  BAR_TECH VB_DECELERATOR;
  BAR_TECH VB_BRAKE;
  BAR_TECH VB_ACCELERATOR;
  
  TIMED_PING GEAR_SWITCH_DELAY;

  /*
  // Voltage
  DRAW_D2_PLOT PLOT_VOLTAGE;

  BAR_TECH VB_VOLTAGE;
  */

  // Temperature
  DRAW_D2_PLOT PLOT_TEMPERATURE;

  BAR_TECH VB_TEMPERATURE_COOLANT;
  BAR_TECH VB_TEMPERATURE_INTAKE;
  BAR_TECH VB_TEMPERATURE_AMBIANT;
  BAR_TECH VB_TEMPERATURE_CATALYST;
  BAR_TECH VB_TEMPERATURE_S_TEMP;

  // Power
  DRAW_D2_PLOT PLOT_POWER;

  BAR_TECH VB_POWER_TACH;
  BAR_TECH VB_POWER_TORQE;
  BAR_TECH VB_POWER_ACCELERATION;
  BAR_TECH VB_POWER_FUEL_RAIL_P;
  BAR_TECH VB_POWER_SYSTEM_VAPER_P;

  // ------------------------------------------
  // Large Displays on Main Sidebar Screen
  T_LARGE_NUMBER_DISPLAY L_SPEED_SB;
  T_LARGE_NUMBER_DISPLAY L_ACCELERATION_SB;
  T_LARGE_NUMBER_DISPLAY L_GEAR_SB;
  T_LARGE_NUMBER_DISPLAY L_TACH_SB;
  T_LARGE_NUMBER_DISPLAY L_VOLTAGE_SB;
  T_LARGE_NUMBER_DISPLAY L_S_TEMP_SB;
};

class AUTOMOBILE_SCREEN
{
  private:

  DISPLAY_DATA_AUTOMOBILE SDATA;
  
  WIDGET_DEFAULTS DEFAULTS;

  bool DISPLAY_DATA = true;

  bool DISPLAY_CONFIRM = false;
  
  bool DISPLAY_NOVA = false;

  bool NOVA_CLIP_DETAIL = false;

  int DISPLAY_MID_BOTTOM = 0; // 0 - Large Horizontal Bars
                              // 1 - Unused
                              // 2 - Temp Graph
                              // 3 - Power Graph

  void nova(system_data &sdSysData);

  public:
  
  void create(system_data &sdSysData);

  void update(system_data &sdSysData);

  void display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms, bool &Display_Confirm);

  void display_sidebar(system_data &sdSysData, bool Automobile_Screen_Selected, bool Restack_Windows);

};

// ---------------------------------------------------------------------------------------
#endif