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
#include <string>
#include <vector>
#include<cmath>

// IMGui Includes
#include "../../imgui/imgui.h"
//#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"

#include <GLFW/glfw3.h>

//#include "imgui_internal.h"
#include "../../imgui/imgui_internal.h"


// STB Includes
#include "../../stb/stb_image.h"

#include "system.h"
#include "fled_time.h"
#include "helper.h"

using namespace std;

// ---------------------------------------------------------------------------------------

// Rotate Text

enum bbEnum { BB_BB, BB_DR, BB_IN, BB_CE, BB_TL, BB_TR, BB_BL, BB_BR, BB_L, BB_R, BB_T, BB_B };
  //      internal bounding boxes
  // BB = outermost 'true' bounding box (including bounding box for handles)
  // DR = bounding box for drawing
  // IN = inner (ie inside bufferzone for boundaries handles
  // CE = centron (prob central 10%)
  // TL...B = bb for handles for grabbing the various edges/corners

  //  TOP LEFT BOTTOM RIGHT

void Text_Rotate(std::string textToRotate, float angleToRotate, bbEnum rotationCentre);

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box(string Text, ImDrawList *Draw_List, system_data &sdSysData);

//void simple_black_back_text_box(string Text, ImDrawList *Draw_List, system_data &sdSysData);

// ---------------------------------------------------------------------------------------

bool Load_Texture_From_File(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
// Image Loading and Displaying Examples
// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

void Graphical_Number(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Position, float Width, int Number);

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

  ImVec2 IMAGE_SIZE;
  GLuint IMAGE_TEXTURE = 0;

  public:

  string FILENAME = "";
  
  void clear();

  bool create(string Full_Filename, float Scale_Factor);
  bool create(string Full_Filename, ImVec2 Max_Size);
  bool create(string Path, string Filename, float Scale_Factor);

  ImVec2 size();

  ImVec2 get_should_be_window_size();

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

  void create_subgraph(unsigned long Duration_Span_ms, string Label);

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




class D2_VECTOR_POSITION_REFERENCE
{
  public:

  double GRAPH_START_TIME;
  double GRAPH_END_TIME;
};

class D2_PLOT_LINE_DEGENERATE
{
  public:

  int PIVOT_POINT = 0;
  //int RESERVE_SIZE = 0;
  //int RESERVE_SIZE_CUTOFF = 0;
  //int RESERVE_SIZE_TRIM_AMOUNT = 0;

  int LINE_COLOR;
  float POINT_SIZE = 2.0f;

  bool DISPLAY_MEAN = true;
  bool DISPLAY_MIN_MAX = false;
  //float MIN_MAX_OVERLAP_FACTOR = 1.0f;

  //MIN_MAX_TIME_SLICE VALUES;
  //TIMED_PING UPDATE_SIMPLE_VALUES;
  
  //TIMED_PING MERGE_TIMER;
  //float HOLDOVER_TOTAL = 0.0f;
  //float HOLDOVER_MAX = 0.0f;
  //float HOLDOVER_MIN = 0.0f;
  //float HOLDOVER_COUNT = 0.0f;

  //bool SINGLE_VALUE = false;

  vector<D2_VECTOR_POSITION_REFERENCE>  POSITION_INFO;
  deque<MIN_MAX_TIME_SLICE_DOUBLE>     DATA_POINT_TILL_PIVOT;
  vector<MIN_MAX_TIME_SLICE_DOUBLE>     DATA_POINT;

  //void holdover_clear();

  //void holdover_update(MIN_MAX_TIME_SLICE_SIMPLE Slice_Value);

  void reorganize_line_data(double Time);
};




class DRAW_D2_PLOT_DEGENERATE_PROPERTIES
{
  public:

  string LABEL = "Label";
  
  int COLOR_GRID;                   // Color of grid
  //float POINT_SIZE_GRID = 1.0f;             // Size of grid lines
  //int GRID_SEPERATOR_COUNT_HORIZONTAL = 5;  // Horizontal grid line count
  //int GRID_SEPERATOR_COUNT_VERTICAL = 5;    // Vertical grid line count

  float DATA_POINTS_VALUE_MAX = 100;        // Max value, top of graph value.

  //float TIME_SPAN_MS = 8.0f * 60.0f * 60.0f * 1000.0f;      // Time span in milliseconds
  double TIME_SPAN_MS = 8.0 * 60.0 * 60.0;

  bool LEFT_TO_RIGHT = TRUE;                // Defalt - plot points start on left side
  bool BOTTOM_TO_TOP = TRUE;                // Defalt - 0 value points start on bottom
};

class DRAW_D2_PLOT_DEGENERATE
{
  private:

  //DRAW_GRID GRID;
  
  ImVec2 START_POS;
  ImVec2 END_POS;

  ImVec2 PREV_START_POS;
  ImVec2 PREV_END_POS;

  float FULL_X_SIZE = 0.0f;
  float FULL_Y_SIZE = 0.0f;

  float MAX_POINT_SIZE_ACCUM = 0.0f;
  float DEGEN_POINT_TO_SIZE_RATIO = 0.0f;
  int   DEGEN_POINT_SIZE = 0;

  double TIME_START = 0;

  vector<D2_PLOT_LINE_DEGENERATE> LINE;
  
  // ---
  // Formulas:

  // Triangular Number and Inverse Triangular Number
  float triangular_number(float X);             //  y = (x * (x + 1)) / 2
  float triangular_number_accum(float F_of_y);  //  x = (-1 + sqrt(1 + 8y)) / 2)
  
  // Linear and Inverse Linear
  float linear_number(float X);                 //  y = x
  float linear_accum(float F_of_y);             //  x = (-1.0f + std::sqrt(1.0f + 8.0f * targetSum)) / 2.0f;

  // Exponential Growth and Inverse Exponential Growth
  float exp_growth_number(float X);       
  float exp_growth_accum(float F_of_y);
  
  // Exponential Growth and Inverse Exponential Growth with Scale
  float exp_growth_accum_scale_seconds(double F_of_y);
  //float exp_growth_accum_scale_miliseconds(double F_of_y);

  double exp_growth_number_scale_seconds(float X);
  //double exp_growth_number_scale_miliseconds(float X);

  // ---

  float value_of_accumulated_time(double Time);
  //float accum_to_x_miliseconds(double Time);

  float position_in_time_to_x_coord(double Time);
  //float degen_time_scale_to_x_miliseconds(double Time);

  double value_at_position_in_time(float X);
  //double time_at_position_miliseconds(float X);

  double x_coord_to_position_in_time(int X);

  // ---

  //vector<DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES> SUB_GRAPHS;
  //void merge(double Time, int Sub_Graph, int Line_Number);

  ImVec2 position_on_plot(ImVec2 &Point);
  // Internal

  float time_scale_to_x(float Time);
  // Looks at class vars TIME_START and PROPS.TIME_SPAN_MS to determine pos of x on graph.

  float value_to_y(float Value);
  // Looks at class vars to determine y on graph.

  //void draw_min_max_at_point();
  //void draw_point_to_point();
  void draw_lines(ImDrawList *Draw_List, system_data &sdSysData);

  void first_run();
  bool FIRST_RUN_COMPLETE = false;

  public:

  DRAW_D2_PLOT_DEGENERATE_PROPERTIES PROPS;

  void create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor, bool Single_Value);
  // Prepare line for drawing
  //  Color of line
  //  Show Mean Value
  //  Show Min Max Values
  //  Point_Size - Mean value line point size
  //  Min_Max_Overlap_Factor - Size of min_max is calculated. Value of 2.0f will double
  //                            the size and cause min_max lines to overlap.
  //                            Overlapped areas should be brighter.

  void create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size);
  // Same as before, without single value option.

  void create(double Start_Plot_Time);

  void update(double Time, int Line_Number, float Value);

  //void reorganize_graph_data(double Time);

  bool draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position);
};

// ---------------------------------------------------------------------------------------
#endif