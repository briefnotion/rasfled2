// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_camera.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_CAMERA_H
#define WIDGETS_CAMERA_H

#include <stdio.h>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <cmath>
//#include <utility>

#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include "../../imgui/imgui.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"

// Rasfled Includes
#include "system.h"
#include "widgets.h"

// IMGui Includes

using namespace std;

// ---------------------------------------------------------------------------------------

void add_line_mod_v(ImDrawList *Draw_List, ImVec2 &Point1, ImVec2 &Point2, ImColor &Color, float Thickness);
void add_line_mod_h(ImDrawList *Draw_List, ImVec2 &Point1, ImVec2 &Point2, ImColor &Color, float Thickness);

// ---------------------------------------------------------------------------------------

class CAMERA_WIDGET
{
  private:

  bool SHOW_BUTTONS = false;
  TIMED_IS_READY SHOW_BUTTONS_TIMER;

  bool DISPLAY_CAMERA_SETTINGS = false;
  bool DISPLAY_ADJUSTMENTS = false;
  bool DISPLAY_ENHANCEMENTS = false;

  BUTTON_COLOR        BC_SNAPSHOT;
  
  BUTTON_TOGGLE_COLOR BC_DISPLAY_SETTINGS;
  BUTTON_TOGGLE_COLOR BC_DISPLAY_ADJUSTMENTS;
  BUTTON_TOGGLE_COLOR BC_DISPLAY_ENHANCEMENTS;

  // ---
  // Camera Settings
  BUTTON_COLOR        BC_CONNECT;
  BUTTON_COLOR        BC_DISCONNECT;
  BUTTON_COLOR        BC_DEFAULT;
  BUTTON_COLOR        BC_APPLY;

  // Enhancements
  BUTTON_TOGGLE_COLOR        BC_SHOW_PATH;
  BUTTON_TOGGLE_COLOR        BC_ENH_LOW_LIGHT;
  //BUTTON_TOGGLE_COLOR        BC_ENH_OVERLAY_LINES;
  BUTTON_TOGGLE_COLOR        BC_ENH_GLARE_MASK;
  BUTTON_TOGGLE_COLOR        BC_ENH_CANNY;
  BUTTON_TOGGLE_COLOR        BC_ENH_COLOR;
  BUTTON_TOGGLE_COLOR        BC_ENH_HOUGH;
  BUTTON_TOGGLE_COLOR        BC_ENH_CAR_DETECTION;
  BUTTON_TOGGLE_COLOR        BC_ENH_FAKE_FRAME;
  BUTTON_TOGGLE_COLOR        BC_ENH_DOUBLE_MASK;

  //---

  // Define normalized path coordinates (0.0=top/left, 1.0=bottom/right).
  // These coordinates create the three connected trapezoids.

  // Line Thickness
  float THICKNESS_1_A = 45.0f;
  float THICKNESS_1_B = 10.0f;
  float THICKNESS_2_A = 30.0f;
  float THICKNESS_2_B = 7.0f;
  float THICKNESS_3_A = 10.0f;
  float THICKNESS_3_B = 5.0f;

  ImColor COLOR_BLK = ImColor(0, 0, 0, 64);   // 5ft

  // Some working class variables
  // Variables for image size and centering, declared here to be accessible in class scopes.
  float ANGLE = 0.0f;
  ImVec4 WORKING_AREA;
  ImVec2 FINAL_SIZE = {0, 0};
  ImVec2 PADDING;

  // TESTING
  bool FULL_FRAME_STYLE = 1;
  // 0 - Full Frame Center
  // 1 - Full Horizontal, Center

  ImVec2 norm_to_screen(float &Nx, float &Ny, float &Slide, 
                        ImVec2 &Image_Start_Position, 
                        ImVec2 &Final_Size);

  void display_path(system_data &sdSysData);

  void display_camera_settings_window(system_data &sdSysData);
  void display_camera_adjustments_window(system_data &sdSysData);
  void display_camera_enhancements_window(system_data &sdSysData);

  void display_camera_stats_enhancements(system_data &sdSysData);
  void display_camera_stats_times(system_data &sdSysData);

  void display_camera_frame(system_data &sdSysData);
  
  public:

  void display(system_data &sdSysData, float Angle);
};


// ---------------------------------------------------------------------------------------

#endif
