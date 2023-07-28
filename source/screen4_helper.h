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

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

class WIDGET_DEFAULTS
{
  public:

  ImVec2 SIZE_BUTTON_TAB = ImVec2(100, 43);
  ImVec2 SIZE_BUTTON = ImVec2(75, 75);
  ImVec2 SIZE_BUTTON_MEDIUM = ImVec2(60, 60);
  ImVec2 SIZE_BUTTON_SMALL = ImVec2(43, 43);
  
  ImGuiWindowFlags flags =        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGuiWindowFlags flags_c =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;
  //ImGuiWindowFlags flags_c =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
  //                                  ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | 
  //                                  ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGuiWindowFlags flags_w =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
  //ImGuiWindowFlags flags_w =      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
  //                                  ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | 
  //                                  ImGuiWindowFlags_NoBringToFrontOnFocus;

  ImGuiWindowFlags flags_w_pop =  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                    ImGuiWindowFlags_NoScrollbar;

  ImGuiWindowFlags flags_t = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | 
                              ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH |
                              ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | 
                              ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner |
                              ImGuiTableFlags_SizingFixedFit;

  ImGuiWindowFlags flags_t_no_lines = ImGuiTableFlags_SizingFixedFit;
};

// ---------------------------------------------------------------------------------------

class COLOR_COMBO
{
  public:

  bool DEFAULT = true;

  ImVec4 STANDARD;
  ImVec4 HOVERED;
  ImVec4 ACTIVE;

  void set(float H, float S, float V);
};

class COLOR_COMBOS
{
  public: 
  COLOR_COMBO COLOR_COMB_DEFAULT;
  COLOR_COMBO COLOR_COMB_BLACK;
  COLOR_COMBO COLOR_COMB_WHITE;
  COLOR_COMBO COLOR_COMB_GREY;
  COLOR_COMBO COLOR_COMB_RED;
  COLOR_COMBO COLOR_COMB_ORANGE;
  COLOR_COMBO COLOR_COMB_YELLOW;
  COLOR_COMBO COLOR_COMB_GREEN;
  COLOR_COMBO COLOR_COMB_CYAN;
  COLOR_COMBO COLOR_COMB_BLUE;
  COLOR_COMBO COLOR_COMB_PURPLE;
  COLOR_COMBO COLOR_COMB_PINK;

  void init();
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

  public:

  DEBUG DEBUG_STATUS;

  bool WINDOW_CLOSE = false;

  void carrage_return_set();
  bool carrage_return_get();

  void command_text_set(string Text);
  bool command_text_changed();
  string command_text_get();

  void command_text_clear();
  bool command_text_clear_get();
  
  bool printw_q_avail();
  string printw_q_get();
  void printw(string Text);
  
};

// ---------------------------------------------------------------------------------------
#endif