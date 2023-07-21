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

  // Some Colors HSV H
  const float H_RED = 0;
  const float H_YELLOW = .254;
  const float H_GREEN = .333;
  const float H_CYAN = .5;
  const float H_BLUE = .667;
  const float H_PURPLE = .768;

  ImVec2 SIZE_BUTTON_TAB = ImVec2(100, 43);
  ImVec2 SIZE_BUTTON = ImVec2(75, 75);
  ImVec2 SIZE_BUTTON_SMALL = ImVec2(43, 43);

  bool use_work_area = true;

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
  bool DISPLAY_TIMER = true;

  int DISPLAY_MENU = 0;
  int DISPLAY_SCREEN = 0;

  bool RESET_KEYBOARD_FOCUS = true;

  bool CONSOLE_SCROLL_TO_BOTTOM = false;
  // ---

  string COMMAND_TEXT_TEMP = "";
  string COMMAND_TEXT = "";
  string CONSOLE_TEXT = "";

  // ---

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

};

// ---------------------------------------------------------------------------------------
#endif