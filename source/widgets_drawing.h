// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_drawing.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_DRAWING_H
#define WIDGETS_DRAWING_H

#include <stdio.h>
//#include <string>
//#include <vector>

// IMGui Includes
#include "../../imgui/imgui.h"
//#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

// STB Includes
#include "../../stb/stb_image.h"

#include "system.h"
//#include "screen4_helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------

/*
class THING
{
  private:

  public:

  void draw(system_data &sdSysData);
};

*/

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box();

// ---------------------------------------------------------------------------------------

bool Load_Texture_From_File(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
// Image Loading and Displaying Examples
// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

// ---------------------------------------------------------------------------------------

class TEXTURE_IMAGE
{
  private:

  int IMAGE_WIDTH = 0;
  int IMAGE_HEIGHT = 0;
  GLuint IMAGE_TEXTURE = 0;

  public:

  bool create(string Path, string Filename);

  void draw();

};

class ONE_CHAR_LINE_GRAPH_PROPERTIES
{
  public:

  COLOR_COMBO COLOR;
};

class ONE_CHAR_LINE_GRAPH
{
  private:

  float VALUE;

  public:
  
  ONE_CHAR_LINE_GRAPH_PROPERTIES PROPS;

  void update_value(system_data &sdSysData, float Value, float Max_Value);
  // Value 0.0 - 1.0

  void draw(system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------
#endif