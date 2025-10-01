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

  BUTTON_COLOR        BC_DISPLAY_SETTINGS;

  // ---

  BUTTON_COLOR        BC_CONNECT;
  BUTTON_COLOR        BC_DISCONNECT;

  BUTTON_COLOR        BC_DISPLAY_PATH;
  BUTTON_COLOR        BC_DISPLAY_PATH_SETTINGS;

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

  // Path Values
  bool                DISPLAY_PATH = false;
  bool                DISPLAY_PATH_SETTINGS = false;
  // Define normalized path coordinates (0.0=top/left, 1.0=bottom/right).
  // These coordinates create the three connected trapezoids.
  
  // Level 0: Closest to the car (Bottom of Green)
  float Y0 = 1.0f;
  float XL0 = -0.100f;
  float XR0 = 1.0f;
  
  // Level 1: Green/Yellow boundary
  float Y1 = 0.8f;
  float XL1 = 0.0f;
  float XR1 = 0.90f;
  
  // Level 2: Yellow/Red boundary
  float Y2 = 0.33f;
  float XL2 = 0.229f;
  float XR2 = 0.640f;

  // Level 3: Farthest point (Top of Red)
  float Y3 = 0.2f;
  float XL3 = 0.29f;
  float XR3 = 0.570f;

  void display_camera_settings_window(system_data &sdSysData);

  public:

  void display(system_data &sdSysData);
};


// ---------------------------------------------------------------------------------------

#endif
