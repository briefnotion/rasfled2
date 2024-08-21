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

#include "fled_time.h"
#include "LEDstuff.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

ImVec2 operator+(ImVec2 V1, ImVec2 V2);
ImVec2 operator-(ImVec2 V1, ImVec2 V2);
ImVec2 operator*(ImVec2 V1, ImVec2 V2);
ImVec2 operator*(ImVec2 V1, float Number);

// ---------------------------------------------------------------------------------------

class WIDGET_DEFAULTS
{
  public:

  ImVec2 SIZE_BUTTON_TAB = ImVec2(100, 43);
  ImVec2 SIZE_BUTTON = ImVec2(75, 75);
  ImVec2 SIZE_BUTTON_MEDIUM = ImVec2(60, 60);
  ImVec2 SIZE_BUTTON_SMALL = ImVec2(43, 43);
  
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

class NEO_COLOR_PROPERTIES
{
  public:

  float DURATION = 2000.0f;
};

class NEO_COLOR
{
  private:
  ImColor CURRENT_COLOR = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  ImColor NEW_COLOR = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  ImColor PREV_COLOR = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  
  float CURRENT_TIME = 0.0f;
  float START_TIME = 0.0f;

  bool CHANGED = false;

  ImColor calc_transition();
  void reset_to_new_color(ImColor Color);

  public:
  NEO_COLOR_PROPERTIES PROPS;

  bool changed();

  void set_current_frame_time(unsigned long Time);

  ImColor color(unsigned long Time, ImColor Color);
  ImColor color(unsigned long Time);
  ImColor color();

  void set_color(unsigned long Time, ImColor Color); 
};

// ---------------------------------------------------------------------------------------

class COLOR_COMBO
{
  public:

  ImColor TEXT;
  ImColor BACKGROUND;
  ImColor DIM;
  ImColor STANDARD;
  ImColor STANDARD_V;
  ImColor HOVERED;
  ImColor ACTIVE;
  CRGB    SIMPLE_RGB;

  void set_rgb(float R, float G, float B, float A, float Intensity);
  void set_rgb_v(float R, float G, float B, float A, float Intensity);
  void set_rgb_black();
};

// ---------------------------------------------------------------------------------------

class NEO_COLOR_COMBO
{
  public:

  NEO_COLOR TEXT;
  NEO_COLOR BACKGROUND;
  NEO_COLOR DIM;
  NEO_COLOR STANDARD;
  NEO_COLOR STANDARD_V;
  NEO_COLOR HOVERED;
  NEO_COLOR ACTIVE;
  CRGB      SIMPLE_RGB;

  void set_neo_rgb(unsigned long Time, COLOR_COMBO Color_Combo);
};

// ---------------------------------------------------------------------------------------

class COLOR_COMBOS
{
  private:

  vector<COLOR_COMBO> COLOR_COMBINATIONS;   //Color palates for standard
  vector<COLOR_COMBO> COLOR_COMBINATIONS_V; // Color palates for corpo mode
  vector<NEO_COLOR_COMBO> COLOR_COMBINATIONS_NEO; // Color palates for corpo mode

  bool ALREADY_INITIALIZED = false;

  int void_color_value = 3;
  bool void_color = false;
  
  bool CHANGED = false;

  void set_neo_colors_with_color_change(unsigned long Time);

  public:

  void init(unsigned long Time, float Intensity);

  void set_frame_time(unsigned long);
  // For all colors, set the current frame time. 
  // Call first at start of new draw frame to eliminate the 
  //  need to transfer a new time stamp at every color request.

  COLOR_COMBO color(int Color);
  NEO_COLOR_COMBO neo_color(int Color);

  void toggle_void_color(unsigned long Time);

  COLOR_COMBO c_black();
  COLOR_COMBO c_white();
  COLOR_COMBO c_grey();
  COLOR_COMBO c_red();
  COLOR_COMBO c_orange();
  COLOR_COMBO c_yellow();
  COLOR_COMBO c_green();
  COLOR_COMBO c_cyan();
  COLOR_COMBO c_blue();
  COLOR_COMBO c_purple();
  COLOR_COMBO c_pink();

  NEO_COLOR_COMBO neo_c_black();
  NEO_COLOR_COMBO neo_c_white();
  NEO_COLOR_COMBO neo_c_grey();
  NEO_COLOR_COMBO neo_c_red();
  NEO_COLOR_COMBO neo_c_orange();
  NEO_COLOR_COMBO neo_c_yellow();
  NEO_COLOR_COMBO neo_c_green();
  NEO_COLOR_COMBO neo_c_cyan();
  NEO_COLOR_COMBO neo_c_blue();
  NEO_COLOR_COMBO neo_c_purple();
  NEO_COLOR_COMBO neo_c_pink();

  int black();
  int white();
  int grey();
  int red();
  int orange();
  int yellow();
  int green();
  int cyan();
  int blue();
  int purple();
  int pink();
  int void_colr();

  void void_color_set(unsigned long Time, int Color);

  bool changed();
  // tmp routine
  // resets to false after read
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
#endif