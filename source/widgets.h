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
};

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
  COLOR_COMBO COLOR_COMB_RED;
  COLOR_COMBO COLOR_COMB_YELLOW;
  COLOR_COMBO COLOR_COMB_GREEN;
  COLOR_COMBO COLOR_COMB_CYAN;
  COLOR_COMBO COLOR_COMB_PURPLE;
  COLOR_COMBO COLOR_COMB_BLUE;

  void init();
};

// ---------------------------------------------------------------------------------------

class TEXT_CONSOLE
{
  private:

  string CONSOLE_TEXT = "";
  bool CONSOLE_SCROLL_TO_BOTTOM = false;

  public:

  void add_line(string Text);
  void display(const char *name, bool *p_open, ImGuiWindowFlags flags);
};

// ---------------------------------------------------------------------------------------

void text_simple_bool(string Text, bool Indication);

bool button_simple_enabled(string Text, bool Enabled, ImVec2 ImVec2_Size);
bool button_simple_color(string Text, COLOR_COMBO Color, ImVec2 ImVec2_Size);
bool button_simple_toggle_color(string True_Value_Text, string False_Value_Text, bool Toggle, 
                                COLOR_COMBO True_Color, COLOR_COMBO False_Color, ImVec2 ImVec2_Size);

// ---------------------------------------------------------------------------------------
#endif