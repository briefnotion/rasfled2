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

  public:

  void display(system_data &sdSysData);
};


// ---------------------------------------------------------------------------------------

#endif
