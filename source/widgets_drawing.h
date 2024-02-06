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
#include <vector>
#include<cmath>

// IMGui Includes
#include "../../imgui/imgui.h"
//#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

#include "imgui_internal.h"

// STB Includes
#include "../../stb/stb_image.h"

#include "system.h"
#include "fled_time.h"
#include "helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------

// Rotate Text

enum bbEnum { BB_BB, BB_DR, BB_IN, BB_CE, BB_TL, BB_TR, BB_BL, BB_BR, BB_L, BB_R, BB_T, BB_B };

void Text_Rotate(std::string textToRotate, float angleToRotate, bbEnum rotationCentre);

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box(ImDrawList *Draw_List, system_data &sdSysData);

// ---------------------------------------------------------------------------------------

bool Load_Texture_From_File(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
// Image Loading and Displaying Examples
// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

// ---------------------------------------------------------------------------------------

class MARKER_GADGET_PROPERTIES
{
  public:

  bool ENABLED = false;
  int COLOR;
  float SIZE = 3.0f;
  bool LEFT_SIDE = true;
  bool HORIZONTAL = false;
};

class MARKER_GADGET
{
  private:

  public:

  MARKER_GADGET_PROPERTIES PROPS;

  void draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Pos, ImVec2 Size, bool Display);
};

// ---------------------------------------------------------------------------------------

class TEXTURE_IMAGE
{
  private:

  int IMAGE_WIDTH = 0;
  int IMAGE_HEIGHT = 0;
  GLuint IMAGE_TEXTURE = 0;

  public:

  bool create(string Path, string Filename);

  ImVec2 size();

  int widtht();

  int height();

  void draw();

};

// ---------------------------------------------------------------------------------------

class ONE_CHAR_LINE_GRAPH_PROPERTIES
{
  public:

  int COLOR;
};

class ONE_CHAR_LINE_GRAPH
{
  private:

  float VALUE;

  public:
  
  ONE_CHAR_LINE_GRAPH_PROPERTIES PROPS;

  void update_value(float Value, float Max_Value);
  // Value 0.0 - 1.0

  void draw(ImDrawList *Draw_List, system_data &sdSysData);
};

// ---------------------------------------------------------------------------------------

class DRAW_RULER_PROPERTIES
{
  public:
  
  int COLOR;
  float POINT_SIZE = 1.0f;

  float MAX_VALUE = 100.0f;

  int MAX_TICK_LEVEL = 4;

  bool HORIZONTAL = true;
};

class DRAW_RULER
{
  private:

  ImVec2 PREV_START_POS;
  ImVec2 PREV_END_POS;

  bool draw_level_1 = false;
  bool draw_level_2 = false;
  bool draw_level_3 = false;
  bool draw_level_4 = false;

  float x_size = 0.0f;
  float y_size = 0.0f;

  float div_1 = 0.0f;
  float div_2 = 0.0f;
  float div_3 = 0.0f;
  float div_4 = 0.0f;

  float l1_size = 0.0f;
  float l2_size = 0.0f;
  float l3_size = 0.0f;
  float l4_size = 0.0f;

  float new_max_value = 0.0f;

  public:

  DRAW_RULER_PROPERTIES PROPS;

  void draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position);

};

// ---------------------------------------------------------------------------------------

class DRAW_GRID_PROPERTIES
{
  public:
  
  int COLOR;
  float POINT_SIZE = 2.0f;

  int SEPERATOR_COUNT_HORIZONTAL = 5;
  int SEPERATOR_COUNT_VERTICAL = 5;
};

class DRAW_GRID
{
  private:

  ImVec2 PREV_START_POS;
  ImVec2 PREV_END_POS;

  float X_SIZE = 0.0f;
  float Y_SIZE = 0.0f;

  public:

  DRAW_GRID_PROPERTIES PROPS;

  void draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position);
};

// ---------------------------------------------------------------------------------------
class D2_PLOT_LINE
{
  public:
  int RESERVE_SIZE = 0;
  int RESERVE_SIZE_CUTOFF = 0;
  int RESERVE_SIZE_TRIM_AMOUNT = 0;

  int LINE_COLOR;
  float POINT_SIZE = 2.0f;

  bool DISPLAY_MEAN = true;
  bool DISPLAY_MIN_MAX = false;
  float MIN_MAX_OVERLAP_FACTOR = 1.0f;

  MIN_MAX_TIME_SLICE VALUES;
  TIMED_PING UPDATE_SIMPLE_VALUES;
  
  TIMED_PING MERGE_TIMER;
  float HOLDOVER_TOTAL = 0.0f;
  float HOLDOVER_MAX = 0.0f;
  float HOLDOVER_MIN = 0.0f;
  float HOLDOVER_COUNT = 0.0f;

  bool SINGLE_VALUE = false;

  vector<MIN_MAX_TIME_SLICE_SIMPLE> DATA_POINT;

  void holdover_clear();

  void holdover_update(MIN_MAX_TIME_SLICE_SIMPLE Slice_Value);

};

class DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES
{
  private:

  public:

  // Calculated

  string LABEL = "";

  ImVec2 START_POS;
  ImVec2 END_POS;

  float X_SIZE = 0.0f;
  float Y_SIZE = 0.0f;

  float X_FACTOR = 1.0f;
  float Y_FACTOR = 1.0f;

  float TIME_PER_POINT_F = 1.0f;
  unsigned long TIME_PER_POINT_UL = 1;

  vector<D2_PLOT_LINE> LINE;

  // Definable

  int DATA_POINTS_COUNT_MAX = 100;          // Resolution of x point size
                                            //  Keep within a few thousand max
                                            //  Doubtful anything beyond screen.x size
                                            //  would be viewable.

  unsigned long DURATION_SPAN = 60 * 1000;  // Duration from start to end
                                            //  of graph (in miliseconds)
                                            //  Default is 1 minute.
};

class DRAW_D2_PLOT_PROPERTIES
{
  public:

  string LABEL = "Label";
  
  int COLOR_GRID;                   // Color of grid
  float POINT_SIZE_GRID = 1.0f;             // Size of grid lines
  int GRID_SEPERATOR_COUNT_HORIZONTAL = 5;  // Horizontal grid line count
  int GRID_SEPERATOR_COUNT_VERTICAL = 5;    // Vertical grid line count

  float DATA_POINTS_VALUE_MAX = 100;        // Max value, top of graph value.

  bool LEFT_TO_RIGHT = TRUE;                // Defalt - plot points start on left side
  bool BOTTOM_TO_TOP = TRUE;                // Defalt - 0 value points start on bottom
};

class DRAW_D2_PLOT
// Declaration
//  DRAW_D2_PLOT plot;
//  plot.PROPS.thing = things;
//
//  plot.create_subgraph(size of vec, duration start to end ms);
//  ...
//
//  plot.create_line(Color, dir, dir , sizes, sizes);
//  ...
//  
//  plot.create(plot_start_time);  

{
  private:

  DRAW_GRID GRID;

  ImVec2 PREV_START_POS;
  ImVec2 PREV_END_POS;

  float FULL_X_SIZE = 0.0f;
  float FULL_Y_SIZE = 0.0f;

  unsigned long TIME_START = 0;

  vector<DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES> SUB_GRAPHS;

  void merge(unsigned long Time, int Sub_Graph, int Line_Number);

  ImVec2 position_on_plot_at_start(float Y, DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES &Graph);
  // Internal

  ImVec2 position_on_plot(float X, float Y, DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES &Graph, bool &Out_Of_Bounds_X);
  // Internal

  public:

  DRAW_D2_PLOT_PROPERTIES PROPS;

  void create_subgraph(int Max_Data_Point_Count, unsigned long Duration_Span_ms, string Label);

  void create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor, bool Single_Value);
  // Prepare line for drawing
  //  Color of line
  //  Show Mean Value
  //  Show Min Max Values
  //  Point_Size - Mean value line point size
  //  Min_Max_Overlap_Factor - Size of min_max is calculated. Value of 2.0f will double
  //                            the size and cause min_max lines to overlap.
  //                            Overlapped areas should be brighter.

  void create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor);
  // Same as before, without single value option.

  void create(unsigned long Start_Plot_Time);

  void update(unsigned long Time, int Line_Number, float Value);

  void update(unsigned long Time, int Sub_Graph, int Line_Number, MIN_MAX_TIME_SLICE_SIMPLE Sample);

  void update(unsigned long Time, int Line_Number, MIN_MAX_TIME_SLICE_SIMPLE Sample);

  void update_timed(unsigned long Time, int Line_Number, float Value);

  void draw_graph(ImDrawList *Draw_List, system_data &sdSysData);

  bool draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position);
};

// ---------------------------------------------------------------------------------------
#endif