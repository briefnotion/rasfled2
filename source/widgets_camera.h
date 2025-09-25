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

// IMGui Includes

using namespace std;

// ---------------------------------------------------------------------------------------

class CAMERA_WIDGET
{
  private:
  public:

  void display(system_data &sdSysData);
};


// ---------------------------------------------------------------------------------------

#endif
