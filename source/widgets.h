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

// Rasfled Includes
#include "helper.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

using namespace std;

// ---------------------------------------------------------------------------------------

void text_simple_bool(string Text, bool Indication);

bool button_simple_enabled(string Text, bool Enabled, ImVec2 ImVec2_Size);
bool button_simple_color(string Text, float H_Color, ImVec2 ImVec2_Size);
bool button_simple_toggle_color(string True_Value_Text, string False_Value_Text, bool Toggle, float H_Color, ImVec2 ImVec2_Size);

// ---------------------------------------------------------------------------------------
#endif