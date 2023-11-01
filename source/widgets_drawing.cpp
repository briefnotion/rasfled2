// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_drawing.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_DRAWING_CPP
#define WIDGETS_DRAWING_CPP

#define STB_IMAGE_IMPLEMENTATION

#include "widgets_drawing.h"

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box()
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 128));
}

// ---------------------------------------------------------------------------------------

// Simple helper function to load an image into a OpenGL texture with common settings
bool Load_Texture_From_File(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
  // Load from file
  int image_width = 0;
  int image_height = 0;
  unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
      return false;

  // Create a OpenGL texture identifier
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
  #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  #endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;

  return true;
}

// ---------------------------------------------------------------------------------------

bool TEXTURE_IMAGE::create(string Path, string Filename)
{
  string qr_code_image_file = Path + Filename;

  bool ret = Load_Texture_From_File(qr_code_image_file.c_str(), &IMAGE_TEXTURE, &IMAGE_WIDTH, &IMAGE_HEIGHT);

  IM_ASSERT(ret);
  return ret;
}

ImVec2 TEXTURE_IMAGE::size()
{
  return ImVec2(IMAGE_WIDTH, IMAGE_HEIGHT);
}

int TEXTURE_IMAGE::widtht()
{
  return IMAGE_WIDTH;
}

int TEXTURE_IMAGE::height()
{
  return IMAGE_HEIGHT;
}

void TEXTURE_IMAGE::draw()
{
  //ImGui::Text("pointer = %p", IMAGE_TEXTURE);
  //ImGui::Text("size = %d x %d", IMAGE_WIDTH, IMAGE_HEIGHT);
  ImGui::Image((void*)(intptr_t)IMAGE_TEXTURE, ImVec2(IMAGE_WIDTH, IMAGE_HEIGHT));
}

// ---------------------------------------------------------------------------------------

void ONE_CHAR_LINE_GRAPH::update_value(system_data &sdSysData, float Value, float Max_Value)
{
  if (Max_Value > 0)
  {
    VALUE = Value / Max_Value;
  }
  else
  {
    VALUE = -1;
  }
}

void ONE_CHAR_LINE_GRAPH::draw(system_data &sdSysData)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImU32 col = PROPS.COLOR.TEXT;

  ImVec2 position = ImGui::GetCursorScreenPos();

  float x = position.x;
  float y = position.y + 3.0f;
  
  const float spacing = 10.0f;
  static float size = 12.0f;

  if (VALUE == 0)
  {
    draw_list->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 4, 1.5f);
  }

  if (VALUE > 0.0f && VALUE < 0.6f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.1f && VALUE < 0.7f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.2f && VALUE < 0.8f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.3f && VALUE < 0.9f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.5f && VALUE < 1.0f)
  {
    draw_list->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8, 1.5f);
  }

  if (VALUE >= 1.0f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8);
  }
  
  ImGui::Dummy(ImVec2((spacing), (spacing)));

  position.x = position.x + spacing;
  ImGui::SetCursorScreenPos(position);
}

// ---------------------------------------------------------------------------------------

void DRAW_RULER::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    x_size = End_Position.x - Start_Position.x;
    y_size = End_Position.y - Start_Position.y;

    new_max_value = PROPS.MAX_VALUE;
    
    while (new_max_value <= 10.0f)
    {
      new_max_value = new_max_value * 10.0f;
    }
    
    while (new_max_value > 500.0f)
    {
      new_max_value = new_max_value / 10.0f;
    }

    div_1 = fmod(new_max_value /2, 10.0f);
    if (div_1 == 0.0f)
    {
      l1_size = x_size / 2.0f;
      draw_level_1 = true;
    }

    div_2 = fmod(new_max_value, 10.0f);
    if (div_2 == 0.0f)
    {
      l2_size = x_size / (new_max_value / 10.0f);
      draw_level_2 = true;
    }

    div_3 = fmod(new_max_value, 5.0f);
    if (div_3 == 0.0f)
    {
      l3_size = x_size / (new_max_value / 5.0f);
      draw_level_3 = true;
    }

    //div_4 = fmod(new_max_value, 1.0f);
    //if (div_4 == 0.0f)
    {
      l4_size = x_size / (new_max_value / 1.0f);
      draw_level_4 = true;
    }

    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;
  }


  // 50% tick
  if (PROPS.MAX_TICK_LEVEL >= 1 && draw_level_1)
  {
    draw_list->AddLine(ImVec2(Start_Position.x + l1_size, Start_Position.y), 
                        ImVec2(Start_Position.x + l1_size, Start_Position.y + (y_size * 0.75f)), 
                        PROPS.COLOR.STANDARD, PROPS.POINT_SIZE);
  }

  // 10 ticks
  if (PROPS.MAX_TICK_LEVEL >= 2 && draw_level_2)
  {
    for (int tenths = 1; tenths < (new_max_value / 10.0f); tenths++)
    {
      draw_list->AddLine(ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y), 
                    ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y + (y_size * 0.5f)), 
                    PROPS.COLOR.STANDARD, PROPS.POINT_SIZE);
    }
  }

  // 10 halves ticks
  if (PROPS.MAX_TICK_LEVEL >= 3 && draw_level_3)
  {
    for (int twentyiths = 1; twentyiths < (new_max_value / 5.0f); twentyiths++)
    {
      draw_list->AddLine(ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y), 
                    ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y + (y_size * 0.25f)), 
                    PROPS.COLOR.STANDARD, PROPS.POINT_SIZE);
    }
  }

  // 1 ticks
  if (PROPS.MAX_TICK_LEVEL >= 4 && draw_level_4)
  {
    for (int hundreds = 1; hundreds < (new_max_value / 1.0f); hundreds++)
    {
      draw_list->AddLine(ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y), 
                    ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y + 1.0f), 
                    PROPS.COLOR.STANDARD, PROPS.POINT_SIZE);
    }
  }
}

// ---------------------------------------------------------------------------------------

void DRAW_GRID::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    X_SIZE = ((End_Position.x - Start_Position.x) / (float)PROPS.SEPERATOR_COUNT_VERTICAL) - (1 / (float)PROPS.SEPERATOR_COUNT_VERTICAL);
    Y_SIZE = ((End_Position.y - Start_Position.y) / (float)PROPS.SEPERATOR_COUNT_HORIZONTAL) - (1 / (float)PROPS.SEPERATOR_COUNT_HORIZONTAL);

    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;
  }

  // Vertical Lines
  for (int vert_count = 0; vert_count <= PROPS.SEPERATOR_COUNT_VERTICAL; vert_count++)
  {
    draw_list->AddLine(ImVec2(Start_Position.x + (vert_count * X_SIZE), Start_Position.y), 
                  ImVec2(Start_Position.x + (vert_count * X_SIZE), End_Position.y), 
                  PROPS.COLOR.DIM, PROPS.POINT_SIZE);
  }

  // Horizontal Lines
  for (int horz_count = 0; horz_count <= PROPS.SEPERATOR_COUNT_HORIZONTAL; horz_count++)
  {
    draw_list->AddLine(ImVec2(Start_Position.x, Start_Position.y + (horz_count * Y_SIZE)), 
                  ImVec2((End_Position.x), Start_Position.y + (horz_count * Y_SIZE)), 
                  PROPS.COLOR.DIM, PROPS.POINT_SIZE);
  }
}

// ---------------------------------------------------------------------------------------

ImVec2 DRAW_D2_PLOT::position_on_plot(float X, float Y, bool &Out_Of_Bounds_X)
{
  ImVec2 ret_vec;

  // Calc
  if (PROPS.LEFT_TO_RIGHT)
  {
    ret_vec.x = PREV_START_POS.x + (X * X_FACTOR);
  }
  else
  {
    ret_vec.x = PREV_END_POS.x - (X * X_FACTOR);
  }

  if (PROPS.BOTTOM_TO_TOP)
  {
    ret_vec.y = PREV_END_POS.y - (Y * Y_FACTOR);
  }
  else
  {
    ret_vec.y = PREV_START_POS.y + (Y * Y_FACTOR);
  }

  // Check 
  if (ret_vec.x < PREV_START_POS.x || ret_vec.x > PREV_END_POS.x)
  {
    Out_Of_Bounds_X = true;
  }

  // Return
  return ret_vec;
}

void DRAW_D2_PLOT::create_line(COLOR_COMBO Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor)
{
  D2_PLOT_LINE tmp_line;

  tmp_line.LINE_COLOR = Color;
  tmp_line.DISPLAY_MEAN = Display_Mean;
  tmp_line.DISPLAY_MIN_MAX = Display_Min_Max;
  tmp_line.POINT_SIZE = Point_Size;
  tmp_line.MIN_MAX_OVERLAP_FACTOR = Min_Max_Overlap_Factor;

  tmp_line.RESERVE_SIZE = (int)((float)PROPS.DATA_POINTS_COUNT_MAX * (float)1.5);
  tmp_line.RESERVE_SIZE_CUTOFF = (int)((float)PROPS.DATA_POINTS_COUNT_MAX * (float)1.4);
  tmp_line.RESERVE_SIZE_TRIM_AMOUNT = (int)((float)PROPS.DATA_POINTS_COUNT_MAX * (float).3);

  tmp_line.DATA_POINT.reserve(tmp_line.RESERVE_SIZE);

  LINE.push_back(tmp_line);
}

void DRAW_D2_PLOT::create(unsigned long Start_Plot_Time)
{
  GRID.PROPS.COLOR = PROPS.COLOR_GRID;
  GRID.PROPS.POINT_SIZE = PROPS.POINT_SIZE_GRID;
  GRID.PROPS.SEPERATOR_COUNT_HORIZONTAL = PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL;
  GRID.PROPS.SEPERATOR_COUNT_VERTICAL = PROPS.GRID_SEPERATOR_COUNT_VERTICAL;

  TIME_START = Start_Plot_Time;
}

void DRAW_D2_PLOT::update(unsigned long Time, int Line_Number, float Value)
{
  if (Line_Number >= 0 && Line_Number < LINE.size())
  {
    MIN_MAX_TIME_SLICE_SIMPLE tmp_value;

    tmp_value.MEAN_VALUE = Value;
    tmp_value.TIME_CREATED = Time;

    LINE[Line_Number].DATA_POINT.push_back(tmp_value);

    if (LINE[Line_Number].DATA_POINT.size() >= LINE[Line_Number].RESERVE_SIZE_CUTOFF)
    {
      LINE[Line_Number].DATA_POINT.erase(LINE[Line_Number].DATA_POINT.begin(), LINE[Line_Number].DATA_POINT.begin() + LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT);
    }
  }
}

void DRAW_D2_PLOT::update(int Line_Number, MIN_MAX_TIME_SLICE_SIMPLE Sample)
{
  if (Line_Number >= 0 && Line_Number < LINE.size())
  {
    LINE[Line_Number].DATA_POINT.push_back(Sample);

    if (TIME_START == 0)
    {
      TIME_START = Sample.TIME_CREATED;
    }
    
    if (LINE[Line_Number].DATA_POINT.size() >= LINE[Line_Number].RESERVE_SIZE_CUTOFF)
    {
      LINE[Line_Number].DATA_POINT.erase(LINE[Line_Number].DATA_POINT.begin(), LINE[Line_Number].DATA_POINT.begin() + LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT);
    }
  }
}

void DRAW_D2_PLOT::update_timed(unsigned long Time, int Line_Number, float Value)
{
  LINE[Line_Number].VALUES.store_value(Value);

  if (LINE[Line_Number].UPDATE_SIMPLE_VALUES.ping_down(Time) == false)
  {
    MIN_MAX_TIME_SLICE_SIMPLE tmp_value;

    tmp_value.MEAN_VALUE = LINE[Line_Number].VALUES.mean();
    tmp_value.MIN_VALUE = LINE[Line_Number].VALUES.min();
    tmp_value.MAX_VALUE = LINE[Line_Number].VALUES.max();
    tmp_value.TIME_CREATED = LINE[Line_Number].VALUES.time_created();

    update(Line_Number, tmp_value);

    LINE[Line_Number].VALUES.clear(Time);

    LINE[Line_Number].UPDATE_SIMPLE_VALUES.ping_up(Time, TIME_PER_POINT_UL);
  }
}

void DRAW_D2_PLOT::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    X_SIZE = (End_Position.x - Start_Position.x);
    Y_SIZE = (End_Position.y - Start_Position.y);

    X_FACTOR = X_SIZE / (float)PROPS.DATA_POINTS_COUNT_MAX;
    Y_FACTOR = Y_SIZE / (float)PROPS.DATA_POINTS_VALUE_MAX;

    TIME_PER_POINT_F = float(PROPS.DURATION_SPAN / PROPS.DATA_POINTS_COUNT_MAX);
    TIME_PER_POINT_UL = (PROPS.DURATION_SPAN / PROPS.DATA_POINTS_COUNT_MAX);

    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;
  }

  ImGui::BeginChild(PROPS.LABEL.c_str(), ImVec2(X_SIZE, Y_SIZE), false, 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
  {
    GRID.draw(sdSysData, Start_Position, End_Position);

    for (int line = 0; line < LINE.size(); line++)
    {
      bool out_of_bounds_x = false;
      
      ImVec2 mean_start;
      ImVec2 mean_end = position_on_plot(0, LINE[line].DATA_POINT[0].MEAN_VALUE, out_of_bounds_x);

      ImVec2 min;
      ImVec2 max;

      for (int point = 1; point < LINE[line].DATA_POINT.size(); point++)
      {
        mean_start = mean_end;

        // Draw Min Max
        if (LINE[line].DISPLAY_MIN_MAX)
        {
          out_of_bounds_x = false;

          // New plot points continue from prev pos
          //min = position_on_plot(
          //     ((float)(LINE[line].DATA_POINT[point].TIME_CREATED - TIME_START) / TIME_PER_POINT_F), 
          //     LINE[line].DATA_POINT[point].MIN_VALUE);
          //max = position_on_plot(
          //     ((float)(LINE[line].DATA_POINT[point].TIME_CREATED - TIME_START) / TIME_PER_POINT_F), 
          //     LINE[line].DATA_POINT[point].MAX_VALUE);

          // New plot points are added to front, plot scrolls from front
          min = position_on_plot(
               ((float)(sdSysData.tmeCURRENT_FRAME_TIME - LINE[line].DATA_POINT[point].TIME_CREATED) / TIME_PER_POINT_F), 
               LINE[line].DATA_POINT[point].MIN_VALUE, out_of_bounds_x);
          max = position_on_plot(
               ((float)(sdSysData.tmeCURRENT_FRAME_TIME - LINE[line].DATA_POINT[point].TIME_CREATED) / TIME_PER_POINT_F), 
               LINE[line].DATA_POINT[point].MAX_VALUE, out_of_bounds_x);

          if (out_of_bounds_x == false)
          {
            draw_list->AddLine(min, max, LINE[line].LINE_COLOR.DIM, X_FACTOR * LINE[line].MIN_MAX_OVERLAP_FACTOR);
          }
        }
        
        // Draw Line
        if (LINE[line].DISPLAY_MEAN)
        {
          out_of_bounds_x = false;

          // New plot points continue from prev pos
          //mean_end = position_on_plot(
          //      ((float)(LINE[line].DATA_POINT[point].TIME_CREATED - TIME_START) / TIME_PER_POINT_F), 
          //      LINE[line].DATA_POINT[point].MEAN_VALUE);

          // New plot points are added to front, plot scrolls from front
          mean_end = position_on_plot(
                ((float)(sdSysData.tmeCURRENT_FRAME_TIME - LINE[line].DATA_POINT[point].TIME_CREATED) / TIME_PER_POINT_F), 
                LINE[line].DATA_POINT[point].MEAN_VALUE, out_of_bounds_x);
          
          if (out_of_bounds_x == false)
          {
            draw_list->AddLine(mean_start, mean_end, LINE[line].LINE_COLOR.STANDARD, LINE[line].POINT_SIZE);
          }
        }
      }
    }
  }
  ImGui::EndChild();

}

// ---------------------------------------------------------------------------------------

#endif