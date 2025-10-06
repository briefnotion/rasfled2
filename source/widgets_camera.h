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
  BUTTON_TOGGLE_COLOR        BC_DISPLAY_PATH;
  BUTTON_TOGGLE_COLOR        BC_ENH_MEDIAN_BLUR;
  BUTTON_TOGGLE_COLOR        BC_ENH_LOW_LIGHT;
  BUTTON_TOGGLE_COLOR        BC_ENH_CARTOONIFY;
  BUTTON_TOGGLE_COLOR        BC_ENH_SHARPEN;
  BUTTON_TOGGLE_COLOR        BC_ENH_LINE_DETECTION;
  BUTTON_TOGGLE_COLOR        BC_ENH_ENH_CURVE_FIT;
  BUTTON_TOGGLE_COLOR        BC_ENH_ROAD_MASK;
  BUTTON_TOGGLE_COLOR        BC_ENH_CAR_DETECTION;

  // Path Values
  bool                DISPLAY_PATH = true;

  //---

  // Define normalized path coordinates (0.0=top/left, 1.0=bottom/right).
  // These coordinates create the three connected trapezoids.
  
  // Angle Muliplier
  float ANGLE_MULTIPLIER = 2.5f;

  // Line Thickness
  float THICKNESS_1 = 10.0f;
  float THICKNESS_2 = 7.0f;
  float THICKNESS_3 = 5.0f;

  // Level 0: Closest to the car (Bottom of Green)
  float Y0 = 1.000f;
  float XL0 = -0.100f;
  float XR0 = 1.160f;
  
  // Level 1: Green/Yellow boundary
  float Y1 = 0.800f;
  float XL1 = 0.00f;
  float XR1 = 1.020f;
  
  // Level 2: Yellow/Red boundary
  float Y2 = 0.330f;
  float XL2 = 0.227f;
  float XR2 = 0.700f;

  // Level 3: Farthest point (Top of Red)
  float Y3 = 0.200f;
  float XL3 = 0.295f;
  float XR3 = 0.610f;

  ImVec2 norm_to_screen(float &Nx, float &Ny, float &Slide, 
                        ImVec2 &Image_Start_Position, 
                        ImVec2 &Final_Size);

  void display_camera_settings_window(system_data &sdSysData);
  void display_camera_adjustments_window(system_data &sdSysData);
  void display_camera_enhancements_window(system_data &sdSysData);

  public:

  void display(system_data &sdSysData, float Angle);
};


// ---------------------------------------------------------------------------------------

#endif
