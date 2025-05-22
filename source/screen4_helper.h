// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4_helper.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_HELPER_H
#define SCREEN4_HELPER_H

#include <stdio.h>
#include <string>
#include <vector>

// Rasfled Includes
#include "definitions.h"
#include "fled_time.h"
#include "LEDstuff.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"
// -------------------------------------------------------------------------------------

// COLOR_LIST
#define RAS_CUSTOM     -1
#define RAS_BLACK       0
#define RAS_WHITE       1
#define RAS_GREY        2
#define RAS_RED         3
#define RAS_ORANGE      4
#define RAS_YELLOW      5
#define RAS_GREEN       6
#define RAS_CYAN        7
#define RAS_BLUE        8
#define RAS_PURPLE      9
#define RAS_PINK        10
#define RAS_MONOCHROME  11

// ---------------------------------------------------------------------------------------

using namespace std;

// ---------------------------------------------------------------------------------------

ImVec2 operator+(ImVec2 V1, ImVec2 V2);
ImVec2 operator-(ImVec2 V1, ImVec2 V2);
ImVec2 operator*(ImVec2 V1, ImVec2 V2);
ImVec2 operator*(ImVec2 V1, float Number);

bool vector4_is_same(ImVec4 V1, ImVec4 V2);

// ---------------------------------------------------------------------------------------

ImVec2 text_size(string Text);

ImVec4 get_working_area();
// must be called before other drawing is done.
// X = x starting pos (position of left most window, if no write)
// Y = y starting pos (position of left most window, if no write)
// Z = x size
// W = y size

bool button_area(ImVec4 Working_Area);

// ---------------------------------------------------------------------------------------

class WIDGET_DEFAULTS
{
  public:

  ImVec2 SIZE_BUTTON_TAB = ImVec2(100.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, 43.0f * DEF_SCREEN_SIZE_X_MULTIPLIER);
  ImVec2 SIZE_BUTTON_TAB_MEDIUM = ImVec2(60.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, 43.0f * DEF_SCREEN_SIZE_X_MULTIPLIER);
  ImVec2 SIZE_BUTTON = ImVec2(75.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, 75.0f * DEF_SCREEN_SIZE_X_MULTIPLIER);
  ImVec2 SIZE_BUTTON_MEDIUM = ImVec2(60.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, 60.0f * DEF_SCREEN_SIZE_X_MULTIPLIER);
  ImVec2 SIZE_BUTTON_SMALL = ImVec2(43.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, 43.0f * DEF_SCREEN_SIZE_X_MULTIPLIER);
  
  // Main Window
  ImGuiWindowFlags flags =        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
  
  //ImGuiWindowFlags flags_no_title = ImGuiWindowFlags_NoTitleBar;

  ImGuiWindowFlags flags_b_c =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGuiWindowFlags flags_b_w =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;

  // Child Windows
  ImGuiWindowFlags flags_c =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

  //    No Transparancy
  ImGuiWindowFlags flags_c_nt =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | 
                                    ImGuiWindowFlags_NoBackground;

  ImGuiWindowFlags flags_w =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

  // For Popup Windows Like Color Chooser.
  ImGuiWindowFlags flags_w_pop =  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                    ImGuiWindowFlags_NoScrollbar;

  ImGuiWindowFlags flags_w_pop_scroll =  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

  ImGuiWindowFlags flags_t = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | 
                              ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH |
                              ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | 
                              ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner |
                              ImGuiTableFlags_SizingFixedFit;

  ImGuiWindowFlags flags_t_no_lines = ImGuiTableFlags_SizingFixedFit;

  ImGuiWindowFlags flags_vs = ImGuiSliderFlags_NoInput;
};

// ---------------------------------------------------------------------------------------

CRGB convert_imvec4_to_crgb(ImVec4 Vec4);

// ---------------------------------------------------------------------------------------

class META_SCREEN
{
  public:

  float TYPICAL_PIXEL_LINE_HEIGHT = 10.0f;
};

// ---------------------------------------------------------------------------------------

class NEO_COLOR_PROPERTIES
{
  public:

  float DURATION = 2000.0f;
};

class NEO_COLOR
{
  private:
  ImVec4 CURRENT_COLOR =  ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
  ImVec4 NEW_COLOR =      ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  ImVec4 PREV_COLOR =     ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  
  float CURRENT_TIME_PREV = 0.0f;
  float CURRENT_TIME = 0.00001f;
  float START_TIME = 0.00001f;

  bool NEEDS_CALC = true;

  void set_frame_time(unsigned long Time);
  ImColor calc_transition();
  bool reset_to_new_color(unsigned long Time, ImVec4 Color);

  public:
  NEO_COLOR_PROPERTIES PROPS;

  bool is_changing();
  // Called after color or set color:
  //  Returns true if a color value returned will 
  //  be or is different from the previous time called.

  void set_current_frame_time(unsigned long Time);
  // Sets the current time of the color varible.
  //  Necessary if only .color() will be called and 
  //  advantagous if called multiple times in the same
  //  time frame.

  ImColor color_jump(unsigned long Time, ImColor Start_Color, ImColor End_Color);
  // Sets color immediatly to Start color, then 
  //  returns to End_Color entered.  Will 
  //  return transition color over time defined in props if 
  //  the colors are different.

  ImColor color(unsigned long Time, ImColor Color);
  // Returns Color entered in parameter.  Will 
  //  return transition color over time defined in props if 
  //  the colors are different.

  ImColor color(unsigned long Time);
  // Returns Color enter in previous set_color. Will 
  //  return transition color over time defined in props if 
  //  the colors are different.
  //  set_color will need to be called before.

  ImColor color();
  // Returns Color enter in previous set_color. Will 
  //  return transition color over time defined in props if 
  //  the colors are different.
  //  set_current_frame_time will need to be called before.
  //  set_color will need to be called before.

  void set_color_jump(unsigned long Time, ImColor Start_Color, ImColor End_Color);
  // Sets Current Color to Start Color. 
  //  Sets Transition to End_Color for next color call.

  void set_color(unsigned long Time, ImColor Color); 
  // Enters color for next color call.
};

// ---------------------------------------------------------------------------------------

class NEO_COLOR_CRGB
{
  private:
  ImVec4 CURRENT_COLOR =  ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
  ImVec4 NEW_COLOR =      ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  ImVec4 PREV_COLOR =     ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  
  float CURRENT_TIME_PREV = 0.0f;
  float CURRENT_TIME = 0.0f;
  float START_TIME = 0.0f;

  bool NEEDS_CALC = true;

  void set_frame_time(unsigned long Time);
  CRGB calc_transition();
  bool reset_to_new_color(unsigned long Time, ImVec4 Color);

  public:
  NEO_COLOR_PROPERTIES PROPS;

  bool is_changing();
  // Called after color or set color:
  //  Returns true if a color value returned will 
  //  be or is different from the previous time called.

  void set_current_frame_time(unsigned long Time);
  // Sets the current time of the color varible.
  //  Necessary if only .color() will be called and 
  //  advantagous if called multiple times in the same
  //  time frame.

  CRGB color(unsigned long Time, CRGB Color);
  // Returns Color entered in parameter.  Will 
  //  return transition color over time defined in props if 
  //  the colors are different.
  
  CRGB color(unsigned long Time);
  // Returns Color enter in previous set_color. Will 
  //  return transition color over time defined in props if 
  //  the colors are different.
  //  set_color will need to be called before.

  CRGB color();
  // Returns Color enter in previous set_color. Will 
  //  return transition color over time defined in props if 
  //  the colors are different.
  //  set_current_frame_time will need to be called before.
  //  set_color will need to be called before.

  void set_color(unsigned long Time, CRGB Color); 
  // Enters color for next color call.
};

// ---------------------------------------------------------------------------------------

class COLOR_COMBO
{
  public:

  ImColor         TEXT;
  ImColor         BACKGROUND;
  ImColor         DIM;
  ImColor         STANDARD;
  ImColor         STANDARD_V;
  ImColor         HOVERED;
  ImColor         ACTIVE;
  CRGB            SIMPLE_RGB;

  void set_rgb(float R, float G, float B, float A, float Intensity);
  void set_rgb_v(float R, float G, float B, float A, float Intensity);
  void set_rgb_black();
};

// ---------------------------------------------------------------------------------------

class NEO_COLOR_COMBO
{
  public:

  NEO_COLOR       TEXT;
  NEO_COLOR       BACKGROUND;
  NEO_COLOR       DIM;
  NEO_COLOR       STANDARD;
  NEO_COLOR       STANDARD_V;
  NEO_COLOR       HOVERED;
  NEO_COLOR       ACTIVE;
  NEO_COLOR_CRGB  SIMPLE_RGB;

  void set_neo_rgb(unsigned long Time, COLOR_COMBO &Color_Combo);
};

// ---------------------------------------------------------------------------------------

class COLOR_COMBOS
{
  private:

  vector<COLOR_COMBO>     COLOR_COMBINATIONS;     // Color palates for standard
  vector<COLOR_COMBO>     COLOR_COMBINATIONS_V;   // Color palates for corpo mode
  vector<NEO_COLOR_COMBO> COLOR_COMBINATIONS_NEO; // Color palates for corpo mode

  COLOR_COMBO             CUSTOM_COLOR_COMBO;

  bool ALREADY_INITIALIZED = false;

  int void_color_value = 3;
  int void_color_prev_value = 3;
  bool void_color = false;

  void set_neo_colors_with_color_change(unsigned long Time);

  public:

  void init(unsigned long Time, float Intensity);

  void set_neo_duration(float Duration);

  void set_frame_time(unsigned long);
  // For all colors, set the current frame time. 
  // Call first at start of new draw frame to eliminate the 
  //  need to transfer a new time stamp at every color request.

  COLOR_COMBO pure_color(int Color);
  COLOR_COMBO color(int Color);

  ImColor neo_color_TEXT(int Color);
  ImColor neo_color_BACKGROUND(int Color);
  ImColor neo_color_DIM(int Color);
  ImColor neo_color_STANDARD(int Color);
  ImColor neo_color_STANDARD_V(int Color);
  ImColor neo_color_HOVERED(int Color);
  ImColor neo_color_ACTIVE(int Color);
  CRGB neo_color_SIMPLE_RGB(int Color);
  
  bool neo_color_is_changing();

  void toggle_void_color(unsigned long Time);
  void void_color_set(unsigned long Time, int Color);
  void void_color_set(unsigned long Time, float R, float G, float B, float Intensity);

  int void_colr();
  //int custom_colr();
  int void_colr_prev();
};

// ---------------------------------------------------------------------------------------

class DEBUG
{
  public:
  
  //Debug On Off
  bool DEBUG = false;
  
  // Doors
  vector<bool> DOOR;
};


class CONSOLE_COMMUNICATION
// A smaller variable to facilitate communication to the console.
{
  private: 

  bool COMMAND_TEXT_CLEAR = false;
  bool CARRAGE_RETURN = false;

  string COMMAND_TEXT = "";
  bool CHANGED = false;

  string PRINT_Q = "";
  bool PRINT_Q_AVAIL = false;

  string PENDING_CONFIRM_COMMAND = "";
  bool PENDING_CONFIRM_WITH_CR = false;

  public:

  DEBUG DEBUG_STATUS;

  bool WINDOW_CLOSE = false;

  void carrage_return_set();
  bool carrage_return_get();

  void command_text_set(string Text);
  bool command_text_changed();
  string command_text_get();

  void command_pending_set(string Text);
  void command_pending_set_cr(string Text);
  void command_pending_send();

  void command_text_clear();
  bool command_text_clear_get();
  
  bool printw_q_avail();
  string printw_q_get();
  void printw(string Text);
  
};


// ---------------------------------------------------------------------------------------

class SCREEN4_PANELS
{
  public:

  //sdSysData.PANEL_CONTROL.

  // Main Screen
  int MAIN_DISPLAY_SCREEN = 0;
  // Screen:
  //  0 - Console
  //  1 - Automobile
  //  2 - ADSB
  //  3 - not coded
  //  4 - Logs
  //  5 - Dot Dot Dot



  // Automobile
  bool AUTOMOBILE_DISPLAY_NOVA = false;
  bool AUTOMOBILE_DISPLAY_NOVA_SCREEN = true;
    // false - nova screen 1
    // true  - nova screen 2

  int  AUTOMOBILE_NOVA_2_SELECTION = 0;
  // 0 - Drive
  // 1 - P & T
  // 2 - Indicators
  // 3 - Engine
  // 4 - Other


  int AUTOMOBILE_DISPLAY_MID_TOP = 0;
  // 0 - Speed slow degen
  // 1 - Power Curve

  int AUTOMOBILE_DISPLAY_MID_BOTTOM = 0; 
  // 0 - Large Horizontal Bars
  // 1 - Unused
  // 2 - Temp Graph
  // 3 - Power Graph



  // ADSB
  bool ADSB_DISPLAY_TABLE = false;
  bool ADSB_DISPLAY_MAP = true;

  int ADSB_RANGE_INDICATOR_ZOOM_MIN_MAX = 0;
  // 0  - OFF
  // 1  - MIN (On)
  // 2  - MAX (On)
};


class PANEL
{
  private:
  bool REQUESTED = false;

  public:


  void request();
  bool requested();
};


class SCREEN4_PANEL_CONTROL
{
  private:

  int ATONOMOUS = 1;
  // 0 - Off
  // 1 - On
  // 2 - Active

  SCREEN4_PANELS PANELS_UDEF;

  //bool AUTONOMOUS_CHANGED = false;

  void set_auto_temperature();
  void set_auto_malfunction();

  public:

  SCREEN4_PANELS PANELS;

  PANEL AUTO_TEMPERATURE;
  PANEL AUTO_MALFUNCTION;

  int autonomous_state();
  void autonomous_on();
  void autonomous_off();

  void activate();
};

// ---------------------------------------------------------------------------------------

#endif
