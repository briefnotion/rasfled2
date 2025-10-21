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

class CAMERA_WIDGET
{
  private:

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

  // ---
  BUTTON_COLOR        BC_AUTO_FOCUS_PLUS;
  BUTTON_COLOR        BC_AUTO_FOCUS_MINUS;

  BUTTON_COLOR        BC_ABSL_FOCUS_PLUS;
  BUTTON_COLOR        BC_ABSL_FOCUS_MINUS;

  BUTTON_COLOR        BC_AUTO_EXPOSURE_PLUS;
  BUTTON_COLOR        BC_AUTO_EXPOSURE_MINUS;

  BUTTON_COLOR        BC_BACKLIGHT_COMP_PLUS;
  BUTTON_COLOR        BC_BACKLIGHT_COMP_MINUS;

  BUTTON_COLOR        BC_HUE_PLUS;
  BUTTON_COLOR        BC_HUE_MINUS;

  BUTTON_COLOR        BC_GAMA_PLUS;
  BUTTON_COLOR        BC_GAMA_MINUS;

  BUTTON_COLOR        BC_GAIN_PLUS;
  BUTTON_COLOR        BC_GAIN_MINUS;

  BUTTON_COLOR        BC_SHARPMESS_PLUS;
  BUTTON_COLOR        BC_SHARPMESS_MINUS;

  // Enhancements
  BUTTON_TOGGLE_COLOR        BC_SHOW_PATH;
  BUTTON_TOGGLE_COLOR        BC_ENH_LOW_LIGHT;
  //BUTTON_TOGGLE_COLOR        BC_ENH_OVERLAY_LINES;
  BUTTON_TOGGLE_COLOR        BC_ENH_GLARE_MASK;
  BUTTON_TOGGLE_COLOR        BC_ENH_CANNY;
  BUTTON_TOGGLE_COLOR        BC_ENH_COLOR;
  BUTTON_TOGGLE_COLOR        BC_ENH_HOUGH;
  BUTTON_TOGGLE_COLOR        BC_ENH_CAR_DETECTION;

  //---

  // Define normalized path coordinates (0.0=top/left, 1.0=bottom/right).
  // These coordinates create the three connected trapezoids.

  // Line Thickness
  float THICKNESS_1_A = 16.0f;
  float THICKNESS_1_B = 10.0f;
  float THICKNESS_2_A = 13.0f;
  float THICKNESS_2_B = 7.0f;
  float THICKNESS_3_A = 11.0f;
  float THICKNESS_3_B = 5.0f;

  const ImU32 COLOR_BLK = IM_COL32(0, 0, 0, 255);   // 5ft

  // Some working class variables
  // Variables for image size and centering, declared here to be accessible in class scopes.
  float ANGLE = 0.0f;
  ImVec4 WORKING_AREA;
  ImVec2 FINAL_SIZE = {0, 0};
  ImVec2 PADDING;

  ImVec2 norm_to_screen(float &Nx, float &Ny, float &Slide, 
                        ImVec2 &Image_Start_Position, 
                        ImVec2 &Final_Size);

  void display_path(system_data &sdSysData);

  void display_camera_settings_window(system_data &sdSysData);
  void display_camera_adjustments_window(system_data &sdSysData);
  void display_camera_enhancements_window(system_data &sdSysData);

  public:

  void display(system_data &sdSysData, float Angle);
};


// ---------------------------------------------------------------------------------------

#endif
