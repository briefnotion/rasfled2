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

#include "system.h"
#include "screen4_helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------

class W_TEXT_PROPERTIES
{
  public:

  COLOR_COMBO COLOR;
};

class W_TEXT
// Simple Text Widget with Properties
{
  private:

  string TEXT = "";

  public:

  W_TEXT_PROPERTIES PROPS;

  void update_text(system_data &sdSysData, string Text);

  void draw(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------

class W_TEXT_TF_PROPERTIES
{
  public:

  COLOR_COMBO COLOR_TRUE;
  COLOR_COMBO COLOR_FALSE;
};

class W_TEXT_TF
// Text Widget for displaying True and False Indicators
{
  private:

  string TEXT_TRUE = "";
  string TEXT_FALSE = "";
  bool TRUE_FALSE = false;
  
  public:

  W_TEXT_TF_PROPERTIES PROPS;

  void update_text(system_data &sdSysData, string True_Text, string False_Text);
  void update_tf(system_data &sdSysData, bool True_False);

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