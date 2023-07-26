// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_H
#define SCREEN4_H

#include <stdio.h>
#include <string>

// Rasfled Includes
#include "system.h"
#include "screen4_helper.h"
#include "widgets.h"
#include "widgets_automobile.h"
#include "widgets_adsb.h"


// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

using namespace std;

// ---------------------------------------------------------------------------------------

static void glfw_error_callback(int error, const char* description);

// ---------------------------------------------------------------------------------------

class SCREEN4
{
  private:

  COLOR_COMBOS COLOR_SELECT;

  WIDGET_DEFAULTS DEFAULTS;

  bool use_work_area = true;

  bool show_another_window = true;
  bool show_test_window = false;

  ImVec4 clear_color;
  GLFWwindow *  window = nullptr;

  GLFWmonitor * monitor = nullptr;
  const GLFWvidmode * mode = nullptr;
  //ImGuiIO& io = ImGui::GetIO(); //(void)io;
  
  int width = 1280;
  int height = 720;

  // Windows Screen Variables

  //bool DISPLAY_MAIN = true;
  bool DISPLAY_DEBUG = false;
  bool DISPLAY_TIMER = false;
  bool DISPLAY_OVERHEAD_COLOR = false;

  int DISPLAY_MENU = 0;
  int DISPLAY_SCREEN = 0;

  bool RESET_KEYBOARD_FOCUS = true;

  bool CONSOLE_SCROLL_TO_BOTTOM = false;
  
  // ---
  // Screen Vars

  string COMMAND_TEXT_TEMP = "";
  string COMMAND_TEXT = "";
  string CONSOLE_TEXT = "";

  // ---
  // Screen Widgets
  TEXT_CONSOLE CONSOLE;
  AUTOMOBILE_SCREEN AUTOMOBILE;
  ADSB_SCREEN ADSB;
  TEXT_CONSOLE DAEMON_LOG;

  // --- 
  // Widgets
  W_TEXT_TF LIGHTS;
  W_TEXT_TF DEBUG;
  W_TEXT_TF OVERHEAD;
  W_TEXT_TF HAZARD;
  W_TEXT_TF DAY_NIGHT;
  W_TEXT_TF TIMER;
  W_TEXT_TF DOOR1;
  W_TEXT_TF DOOR2;
  W_TEXT_TF DOOR3;
  W_TEXT_TF DOOR4;
  W_TEXT_TF AUTO;
  W_TEXT_TF ADSB_IND;

  W_TEXT VERSION;
  W_TEXT TEMP;

  public:
  CONSOLE_COMMUNICATION SCREEN_COMMS;

  // Screen Return Variables
  bool WINDOW_CLOSE = false;

  int create();
  // Create Screen and set variables
  // Return 0 OK
  // Return 2 error

  void draw(system_data &sdSysData);
  // Render window to terminal

  void shutdown();
  // Proper close of imgui

  // ----
  // Data Update Paths
  void update_daemon_log(string Text);

  void update_automobile_gadgets(unsigned long &tmeCurrentMillis, system_data &sdSysData);

  void update_ADS_B_gadgets(unsigned long &tmeCurrentMillis, system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------
#endif