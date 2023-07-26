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
    ScrollingBuffer(int max_size = 2000) {
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
  float VALUE_MAX = 100;

  //int MIN_MAX_TIME_SPAN = 60000;
  //int MIN_MAX_TIME_SLICES = 20;

};

class W_GUAGE_PLOT
{
  private:

  float VALUE = 0;

  ScrollingBuffer DATA;

  float IO_TIME = 0;

  public: 

  W_GUAGE_PLOT_PROPERTIES PROPS;

  void create();

  void update_value(unsigned long tmeCurrentMillis, float value);

  void draw(unsigned long tmeCurrentMillis);
};

// ---------------------------------------------------------------------------------------

class W_GUAGE_PROPERTIES
{
  public:

  //COLOR_COMBO COLOR;
  string LABEL = " ";

  float VALUE_MIN = 0;
  float VALUE_MAX = 100;

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

  void update_value(unsigned long tmeCurrentMillis, float value);

  void draw(unsigned long tmeCurrentMillis);
};

// ---------------------------------------------------------------------------------------

class DISPLAY_DATA_AUTOMOBILE
{
  public:

  int MESSAGES = -1;
  string LATEST_UNKNOWN_MESSAGE = "";

  W_TEXT_TF T_CRUISE_CONTROL;
  W_GUAGE G_SPEED;

  W_GUAGE G_ACCELERATION;

  W_GUAGE G_RPM;
  W_GUAGE G_TORQUE;

  float CRUISE_CONTROL_SPEED = 0;
  bool CRUISE_CONTROL_SET = false;
  
  string STEERING_WHEEL_ANGLE = "X";
  string STEERING_WHEEL_LEFT_OF_CENTER = "X";
  string STEERING_WHEEL_TURNING_DIRECTION = "X";

  W_GUAGE TEMP_AMBIANT;
  W_GUAGE TEMP_INTAKE;
  W_GUAGE TEMP_COOLANT;
  W_GUAGE TEMP_CATALYST;
  W_GUAGE TEMP_SUPER_TEMP;

  string FUEL_RAIL_PRESSURE = "X";
  string EVAP_SYSTEM_VAP_PRESSURE = "X";

  string VOLTAGE = "X";
  string BAROMETER = "X";

  W_GUAGE_PLOT P_SPEED;
};

class AUTOMOBILE_SCREEN
{
  private:

  DISPLAY_DATA_AUTOMOBILE SDATA;
  
  WIDGET_DEFAULTS DEFAULTS;

  public:
  
  void create(COLOR_COMBOS &Color_Select);

  void update(system_data &sdSysData, unsigned long tmeFrame_Time);

  void display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                const char *name, bool *p_open, ImGuiWindowFlags flags);

  
};

// ---------------------------------------------------------------------------------------
#endif