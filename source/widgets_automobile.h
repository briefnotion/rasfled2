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

#include "stringthings.h"
#include "fled_time.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

// ImPlot Inncludes
// https://github.com/epezent/implot
#include "../../implot/implot.h"

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

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 1200) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
};

// ---------------------------------------------------------------------------------------

class W_GUAGE_PLOT_PROPERTIES
{
  public:

  //COLOR_COMBO COLOR;
  string LABEL = " ";

  float VALUE_MIN = 0;
  float VALUE_MAX = 70;

  //int MIN_MAX_TIME_SPAN = 60000;
  //int MIN_MAX_TIME_SLICES = 20;

};

class W_GUAGE_PLOT
{
  private:

  ScrollingBuffer DATA1;
  ScrollingBuffer DATA2;
  ScrollingBuffer DATA3;

  float IO_TIME = 0;

  TIMED_PING UPDATE_DATA;

  public: 

  W_GUAGE_PLOT_PROPERTIES PROPS;

  void create();

  void update_value(system_data &sdSysData, float Value1, float Value2, float Value3);

  void draw(system_data &sdSysData);
};

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
};

class T_LARGE_NUMBER_DISPLAY
{
  private:

  bool ACTIVE_WITHIN = false;
  bool IS_TEXT = false;

  float VALUE = 0;
  float VALUE_COMPARE = 0;

  string VALUE_TEXT = ""; 

  MIN_MAX_TIME MIN_MAX;

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

  //COLOR_COMBO COLOR;
  string LABEL = "";

  int DATA_OFFSET = 90;

  NEW_COLOR_SCALE COLOR_SCALE;
};

class T_DATA_DISPLAY
// Setup a color scale based on value
// Colors will not update on sys data color map update.
{
  private:

  WIDGET_DEFAULTS DEFAULTS;

  W_TEXT LABEL;
  W_TEXT DATA;
  
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
  float ACCELERATION = 0;

  float CRUISE_CONTROL_SPEED = 0;
  bool CRUISE_CONTROL_SET = false;

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

  // Steering

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

  // Large Indicators

  T_LARGE_NUMBER_DISPLAY L_SPEED;
  T_LARGE_NUMBER_DISPLAY L_ACCELERATION;
  T_LARGE_NUMBER_DISPLAY L_GEAR;
  T_LARGE_NUMBER_DISPLAY L_TACH;

  // Display Data

  T_DATA_DISPLAY D_FUEL_RAIL_PRESSURE;
  T_DATA_DISPLAY D_EVAP_SYSTEM_VAP_PRESSURE;
  T_DATA_DISPLAY D_VOLTAGE;
  T_DATA_DISPLAY D_BAROMETER;

  T_DATA_DISPLAY D_TEMP_AMBIANT;
  T_DATA_DISPLAY D_TEMP_INTAKE;
  T_DATA_DISPLAY D_TEMP_COOLANT;
  T_DATA_DISPLAY D_TEMP_CATALYST;
  T_DATA_DISPLAY D_TEMP_SUPER_TEMP;

  // Guages

  W_GUAGE G_SPEED;
  W_GUAGE G_ACCELERATION;

  W_GUAGE G_RPM;
  W_GUAGE G_TORQUE;

  W_GUAGE G_TEMP_AMBIANT;
  W_GUAGE G_TEMP_INTAKE;
  W_GUAGE G_TEMP_COOLANT;
  W_GUAGE G_TEMP_CATALYST;
  W_GUAGE G_TEMP_SUPER_TEMP;

  // Plot

  W_GUAGE_PLOT P_SPEED;
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

  
};

// ---------------------------------------------------------------------------------------
#endif