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


class ONE_CHAR_LINE_GRAPH_PROPERTIES
{
  public:

  COLOR_COMBO COLOR;  // not yet working,  
                      // needs hsv to rgb+f conversion
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