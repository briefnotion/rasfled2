// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_H
#define WIDGETS_H

#include <stdio.h>
#include <string>
#include <vector>

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

#include "system.h"
#include "screen4_helper.h"
#include "widgets_drawing.h"

using namespace std;

// ---------------------------------------------------------------------------------------

ImColor gradiant_color(system_data &sdSysData, unsigned long Start_time, unsigned long Duration, 
                      ImColor Start_Color, ImColor End_Color);

// ---------------------------------------------------------------------------------------

class BOOLEAN_GRADIANT
{
  private:

  bool VALUE = false;

  TIMED_PING ACTIVE;
  
  unsigned long DURATION = 500; // miliseconds

  ImColor COLOR_START;
  ImColor COLOR_DESTINATION;

  public:

  ImColor boolean_color(system_data &sdSysData, bool Value, ImColor True_Color, ImColor False_Color);

};

// ---------------------------------------------------------------------------------------

class IMPACT_RESISTANCE_FLOAT_FRAME_COUNT
// Spits the changes of value over SIZE number of frames.
// Problem: Bound to FPS, If fps is halved then the value will take twice as long to 
//  change.
//  Unable to change without reconstructing the variable.
{
  private:

  int SIZE = 6;   // 6 - about 200ms at 30fps
  vector<float> VALUE_COLLECTION;

  bool FIRST_RUN = true;

  int READ_WRITE_POS = 0;

  float LATEST_VALUE = 0.0f;
  float SUMMATION = 0.0f;

  public:

  void set_size(int Size);

  void set_value(float Value);

  float value();

};

// ---------------------------------------------------------------------------------------

class COLOR_VALUE_PAIR
{
  public:

  float LT_or_EQ = 0.0f;
  int COLOR;
};

class NEW_COLOR_SCALE
{
  private:

  vector<COLOR_VALUE_PAIR> COLOR_SCALE;

  public:

  bool active();

  void add_color_value_pair(float Value_Is_LT_or_EQ, int Return_Color);

  int get_color(float Value);
};

// ---------------------------------------------------------------------------------------

class W_TEXT_PROPERTIES
{
  public:

  int COLOR;
  bool STANDARD_COLOR = false;

  bool CHANGE_NOTIFICATION = true;
};

class W_TEXT
// Simple Text Widget with Properties
{
  private:

  string TEXT = "";

  TIMED_PING UPDATE_TIMED;

  public:

  W_TEXT_PROPERTIES PROPS;

  void update_text(system_data &sdSysData, string Text);

  void draw(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------

class W_TEXT_TF_PROPERTIES
{
  public:

  int COLOR_TRUE;
  int COLOR_FALSE;
};

class W_TEXT_TF
// Text Widget for displaying True and False Indicators
{
  private:

  string TEXT_TRUE = "";
  string TEXT_FALSE = "";
  bool TRUE_FALSE = false;

  W_TEXT TEXT_TRUE_FALSE;
  
  public:

  W_TEXT_TF_PROPERTIES PROPS;

  //void update_text(system_data &sdSysData, string True_Text, string False_Text);
  void update_text(string True_Text, string False_Text);
  
  //bool update_tf(system_data &sdSysData, bool True_False);
  bool update_tf(bool True_False);
  // returns true if changed

  void draw(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------

class TEXT_CONSOLE
// Simple Console
{
  private:

  string CONSOLE_TEXT = "";
  bool CONSOLE_SCROLL_TO_BOTTOM = false;

  public:

  void add_line(string Text);
  void display(system_data &sdSysData, const char *name, bool *p_open, ImGuiWindowFlags flags);
};

// ---------------------------------------------------------------------------------------

void text_simple_bool(system_data &sdSysData, string Text, bool Indication, int COLOR);

bool button_simple_enabled(system_data &sdSysData, string Text, bool Enabled, ImVec2 ImVec2_Size);
bool button_simple_color(system_data &sdSysData, string Text, int Color, ImVec2 ImVec2_Size);
bool button_simple_toggle_color(system_data &sdSysData, string True_Value_Text, string False_Value_Text, bool Toggle, 
                                int True_Color, int False_Color, ImVec2 ImVec2_Size);

bool confirm_dialog(system_data &sdSysData, bool &Choice);
// Display confirmation dialog.
// Returns:
//  true if clicked
//  Choice returned when clicked is true or false

void draw_compass(system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2);

// ---------------------------------------------------------------------------------------

class BAR_TECH_PROPERTIES
{
  public:

  string LABEL = "";

  float BAR_HEIGHT = 20.0f;          // Bar Size Height, or Width
  float MARKER_SIZE = 5;          // Marker Height or Width.

  int COLOR_MARKER;       // Color of Marker
  int COLOR_BACKGROUND;   // Color of Marker

  float MAX = 0.0f;                  // Top Value of Graph

  bool HORIZONTAL = true;         // Bar Orientation

  // Min Max Related
  bool DRAW_MIN_MAX = false;                // Records and Displays Opaque Min Max Bar
  bool DRAW_MIN_MAX_ON_TOP = false;         // Display Min Max Values on above bar
  bool DRAW_MIN_MAX_ON_BOTTOM = false;      // Display Min Max Values on below bar
  bool DISPLAY_SINGLE_POINT_FLOAT = false;  // Min Max Numbers Displayed will be truncated to 1 decimal point
  int MIN_MAX_TIME_SPAN = 60000;            // Min Max Timespan in ms. Default 60 sec.
  int MIN_MAX_TIME_SLICES = 20;             // Default split 20, 3 seconds

  // Ruler
  bool DRAW_RULER = false;  // Draw ruler ticks
  int COLOR_RULER;  // Ruler color
  int MAX_TICK_LEVEL = 4;   // Tick level
};

class BAR_TECH
{
  private:

  float VALUE = 0.0f;
  IMPACT_RESISTANCE_FLOAT_FRAME_COUNT VALUE_MARKER;

  MIN_MAX_TIME MIN_MAX;

  DRAW_RULER RULER;

  W_TEXT DSP_MIN;
  W_TEXT DSP_MAX;
  W_TEXT DSP_VALUE;

  void draw_min_max_val(system_data &sdSysData);

  public:

  BAR_TECH_PROPERTIES PROPS;

  void create();

  void update_value(system_data &sdSysData, float Value);

  void draw(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------
#endif