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

// Rotate Text

//Initial code for this comes from: https://gist.github.com/carasuca/e72aacadcf6cf8139de46f97158f790f
//  It doesn't seem to have a license statement, so I'm assuming it's public domain

//internal bounding boxes
// BB = outermost 'true' bounding box (including bounding box for handles)
// DR = bounding box for drawing
// IN = inner (ie inside bufferzone for boundaries handles
// CE = centron (prob central 10%)
// TL...B = bb for handles for grabbing the various edges/corners

void Text_Rotate(std::string textToRotate, float angleToRotate, bbEnum rotationCentre)
{
  // Calculate rotation angle in radians
  float rad = angleToRotate* float_PI / 180.0f;

  ImVec2 textStartPosition = ImGui::GetWindowPos() + ImGui::GetCursorPos();
  // ImRotateEnd uses GetWindowDrawList which is based on position of the screen

  // Calculate text size to determine bounding box
  ImVec2 textSize = ImGui::CalcTextSize(textToRotate.c_str());

  ImVec2 rotationCenter;

  // Calculate rotation center based on bbEnum value
  switch (rotationCentre)
  {
  case BB_CE:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x * 0.5f, textStartPosition.y + textSize.y * 0.5f);
      break;
  case BB_TL:
      rotationCenter = textStartPosition;
      break;
  case BB_TR:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x, textStartPosition.y);
      break;
  case BB_BL:
      rotationCenter = ImVec2(textStartPosition.x, textStartPosition.y + textSize.y);
      break;
  case BB_BR:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x, textStartPosition.y + textSize.y);
      break;
  case BB_L:
      rotationCenter = ImVec2(textStartPosition.x, textStartPosition.y + textSize.y / 2);
      break;
  case BB_R:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x, textStartPosition.y + textSize.y / 2);
      break;
  case BB_T:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x / 2, textStartPosition.y);
      break;
  case BB_B:
      rotationCenter = ImVec2(textStartPosition.x + textSize.x / 2, textStartPosition.y + textSize.y);
      break;
  default:
      std::cout << "ERROR: invalid rotationCentre in drawRotatedText\n";
      return;
  }

  // Start rotation
  int rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;

  // Render the text
  ImGui::Text(textToRotate.c_str());

  // Apply the rotation

  // Adjust the angle to match the standard horizontal orientation
  rad -= float_PI / 2.0f; // Subtract 90 degrees (PI/2 radians)

  float s = sin(rad), c = cos(rad);
  rotationCenter = ImRotate(rotationCenter, s, c) - rotationCenter;

  auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
  for (int i = rotation_start_index; i < buf.Size; i++)
  {
    buf[i].pos = ImRotate(buf[i].pos, s, c) - rotationCenter;
  }
}

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box(string Text, ImDrawList *Draw_List, system_data &sdSysData)
{
  ImGui::Text(Text.c_str());
  Draw_List->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), sdSysData.COLOR_SELECT.neo_color_ACTIVE(RAS_YELLOW));
}

//void simple_black_back_text_box(string Text, ImDrawList *Draw_List, system_data &sdSysData)
//{
  // isnt going to work
//  ImGui::Text(Text.c_str());
//  Draw_List->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), sdSysData.COLOR_SELECT.neo_c_black().ACTIVE, 3.0f);
//}

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

void Graphical_Number(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Position, float Width, int Number)
{
  for (int number = 0; number < Number; number++)
  {
    ImVec2 pos_1 = ImVec2(Position.x + Width - ((float)((number * 2) + 0)  * 12.0f), Position.y + 1.0f);
    ImVec2 pos_2 = ImVec2(Position.x + Width - ((float)((number * 2) + 1)  * 12.0f), Position.y + 1.0f);
    Draw_List->AddLine(pos_1, pos_2, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(RAS_WHITE), 3.0f);
  }
}

// ---------------------------------------------------------------------------------------

void MARKER_GADGET::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Pos, ImVec2 Size, bool Display)
{
  if (PROPS.ENABLED && Display)
  {
    if (PROPS.HORIZONTAL)
    {
      Draw_List->AddRectFilled(ImVec2(Start_Pos.x, Start_Pos.y), 
                    ImVec2(Start_Pos.x + Size.x, Start_Pos.y + PROPS.SIZE), sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
      // No left side, relabel for bottom is not necessary.
    }
    else
    {
      if (PROPS.LEFT_SIDE)
      {
        Draw_List->AddRectFilled(ImVec2(Start_Pos.x, Start_Pos.y), 
                      ImVec2(Start_Pos.x + PROPS.SIZE, Start_Pos.y + Size.y), sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
      }
      else
      {
        Draw_List->AddRectFilled(ImVec2(Start_Pos.x + Size.x - PROPS.SIZE , Start_Pos.y), 
                      ImVec2(Start_Pos.x + Size.x, Start_Pos.y + Size.y), sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
      }
    }
  }
}

// ---------------------------------------------------------------------------------------

// ... your code to create and use the texture

// When you're done with the texture and want to unload it:
//glDeleteTextures(1, &texture);
void TEXTURE_IMAGE::clear()
{
  // UNTESTEST!
  glDeleteTextures(1, &IMAGE_TEXTURE);
}

bool TEXTURE_IMAGE::create(string Full_Filename, float Scale_Factor)
{
  int width = 0;
  int height = 0;
  
  bool ret = Load_Texture_From_File(Full_Filename.c_str(), &IMAGE_TEXTURE, &width, &height);

  IMAGE_SIZE = ImVec2(width * Scale_Factor, height * Scale_Factor);
  FILENAME = Full_Filename;

  IM_ASSERT(ret);
  return ret;
}

bool TEXTURE_IMAGE::create(string Full_Filename, ImVec2 Max_Size)
{
  int width = 0;
  int height = 0;

  float scale_factor = 1.0f;
  
  bool ret = Load_Texture_From_File(Full_Filename.c_str(), &IMAGE_TEXTURE, &width, &height);

  if (width > Max_Size.x || height > Max_Size.y)
  {
    float x_scale = Max_Size.x / width;
    float y_scale = Max_Size.y / height;

    if (x_scale < y_scale)
    {
      scale_factor = x_scale;
    }
    else
    {
      scale_factor = y_scale;
    }
  }

  IMAGE_SIZE = ImVec2(width * scale_factor, height * scale_factor);
  FILENAME = Full_Filename;

  IM_ASSERT(ret);
  return ret;
}

bool TEXTURE_IMAGE::create(string Path, string Filename, float Scale_Factor)
{
  string qr_code_image_file = Path + Filename;
  return create(qr_code_image_file, Scale_Factor);
}

ImVec2 TEXTURE_IMAGE::size()
{
  return IMAGE_SIZE;
}

ImVec2 TEXTURE_IMAGE::get_should_be_window_size()
{
  ImGuiStyle& style = ImGui::GetStyle();
  return ImVec2(IMAGE_SIZE.x + style.WindowPadding.x * 2.0f, 
                IMAGE_SIZE.y + 22.0f + style.WindowPadding.y * 2.0f);
}

void TEXTURE_IMAGE::draw()
{
  ImGui::Image((void*)(intptr_t)IMAGE_TEXTURE, IMAGE_SIZE);
}

// ---------------------------------------------------------------------------------------

void ONE_CHAR_LINE_GRAPH::update_value(float Value, float Max_Value)
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

void ONE_CHAR_LINE_GRAPH::draw(ImDrawList *Draw_List, system_data &sdSysData)
{
  ImU32 col = sdSysData.COLOR_SELECT.neo_color_TEXT(PROPS.COLOR);

  ImVec2 position = ImGui::GetCursorScreenPos();

  float x = position.x;
  float y = position.y + 3.0f;
  
  const float spacing = 10.0f;
  static float size = 12.0f;

  if (VALUE == 0)
  {
    Draw_List->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 4, 1.5f);
  }

  if (VALUE > 0.0f && VALUE < 0.6f)
  {
    Draw_List->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.1f && VALUE < 0.7f)
  {
    Draw_List->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.2f && VALUE < 0.8f)
  {
    Draw_List->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.3f && VALUE < 0.9f)
  {
    Draw_List->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.5f && VALUE < 1.0f)
  {
    Draw_List->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8, 1.5f);
  }

  if (VALUE >= 1.0f)
  {
    Draw_List->AddNgonFilled(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8);
  }
  
  ImGui::Dummy(ImVec2((spacing), (spacing)));

  position.x = position.x + spacing;
  ImGui::SetCursorScreenPos(position);
}

// ---------------------------------------------------------------------------------------

void DRAW_RULER::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  // Check Size Changes
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

    if (PROPS.HORIZONTAL)
    {
      div_1 = fmod(new_max_value / 2.0f, 10.0f);
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
    }
    else
    {
      div_1 = fmod(new_max_value / 2.0f, 10.0f);
      if (div_1 == 0.0f)
      {
        l1_size = y_size / 2.0f;
        draw_level_1 = true;
      }

      div_2 = fmod(new_max_value, 10.0f);
      if (div_2 == 0.0f)
      {
        l2_size = y_size / (new_max_value / 10.0f);
        draw_level_2 = true;
      }

      div_3 = fmod(new_max_value, 5.0f);
      if (div_3 == 0.0f)
      {
        l3_size = y_size / (new_max_value / 5.0f);
        draw_level_3 = true;
      }

      //div_4 = fmod(new_max_value, 1.0f);
      //if (div_4 == 0.0f)
      {
        l4_size = y_size / (new_max_value / 1.0f);
        draw_level_4 = true;
      }
    }
    
    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;
  }

  if (PROPS.HORIZONTAL)  // Vetical is dupe of horizontal, not small but works.
  {
    // 50% tick
    if (PROPS.MAX_TICK_LEVEL >= 1 && draw_level_1)
    {
      Draw_List->AddLine(ImVec2(Start_Position.x + l1_size, Start_Position.y), 
                          ImVec2(Start_Position.x + l1_size, Start_Position.y + (y_size * 0.75f)), 
                          sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
    }

    // 10 ticks
    if (PROPS.MAX_TICK_LEVEL >= 2 && draw_level_2)
    {
      for (int tenths = 1; tenths < (new_max_value / 10.0f); tenths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y + (y_size * 0.5f)), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 10 halves ticks
    if (PROPS.MAX_TICK_LEVEL >= 3 && draw_level_3)
    {
      for (int twentyiths = 1; twentyiths < (new_max_value / 5.0f); twentyiths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y + (y_size * 0.25f)), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 1 ticks
    if (PROPS.MAX_TICK_LEVEL >= 4 && draw_level_4)
    {
      for (int hundreds = 1; hundreds < (new_max_value / 1.0f); hundreds++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y + 1.0f), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }
  }
  else
  {
    // 50% tick
    if (PROPS.MAX_TICK_LEVEL >= 1 && draw_level_1)
    {
      Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + l1_size), 
                          ImVec2(Start_Position.x + (x_size * 0.75f), Start_Position.y + l1_size), 
                          sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
    }

    // 10 ticks
    if (PROPS.MAX_TICK_LEVEL >= 2 && draw_level_2)
    {
      for (int tenths = 1; tenths < (new_max_value / 10.0f); tenths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (tenths * l2_size)), 
                      ImVec2(Start_Position.x + (x_size * 0.5f), Start_Position.y + (tenths * l2_size)), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 10 halves ticks
    if (PROPS.MAX_TICK_LEVEL >= 3 && draw_level_3)
    {
      for (int twentyiths = 1; twentyiths < (new_max_value / 5.0f); twentyiths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (twentyiths * l3_size)), 
                      ImVec2(Start_Position.x + (x_size * 0.25f), Start_Position.y + (twentyiths * l3_size)), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 1 ticks
    if (PROPS.MAX_TICK_LEVEL >= 4 && draw_level_4)
    {
      for (int hundreds = 1; hundreds < (new_max_value / 1.0f); hundreds++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (hundreds * l4_size)), 
                      ImVec2(Start_Position.x + 1.0f, Start_Position.y  + (hundreds * l4_size)), 
                      sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }
  }
}

// ---------------------------------------------------------------------------------------

void DRAW_GRID::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
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
    Draw_List->AddLine(ImVec2(Start_Position.x + (vert_count * X_SIZE), Start_Position.y), 
                  ImVec2(Start_Position.x + (vert_count * X_SIZE), End_Position.y), 
                  sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR), PROPS.POINT_SIZE);
  }

  // Horizontal Lines
  for (int horz_count = 0; horz_count <= PROPS.SEPERATOR_COUNT_HORIZONTAL; horz_count++)
  {
    Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (horz_count * Y_SIZE)), 
                  ImVec2((End_Position.x), Start_Position.y + (horz_count * Y_SIZE)), 
                  sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR), PROPS.POINT_SIZE);
  }
}

// ---------------------------------------------------------------------------------------

void D2_PLOT_LINE::holdover_clear()
{
  HOLDOVER_TOTAL = 0.0f;
  HOLDOVER_MAX = 0.0f;
  HOLDOVER_MIN = 0.0f;
  HOLDOVER_COUNT = 0.0f;
}

void D2_PLOT_LINE::holdover_update(MIN_MAX_TIME_SLICE_SIMPLE Slice_Value)
{
  if (HOLDOVER_COUNT == 0)
  {
      HOLDOVER_MAX = Slice_Value.MEAN_VALUE;
      HOLDOVER_MIN = Slice_Value.MEAN_VALUE;
  }

  HOLDOVER_TOTAL = HOLDOVER_TOTAL + Slice_Value.MEAN_VALUE;

  if (Slice_Value.MAX_VALUE > HOLDOVER_MAX)
  {
      HOLDOVER_MAX = Slice_Value.MAX_VALUE;
  }

  if (Slice_Value.MIN_VALUE < HOLDOVER_MIN)
  {
    HOLDOVER_MIN = Slice_Value.MIN_VALUE;
  }

  HOLDOVER_COUNT++;
}

void DRAW_D2_PLOT::merge(unsigned long Time, int Sub_Graph, int Line_Number)
{
  if (Sub_Graph +1 < (int)SUB_GRAPHS.size())
  {
    if (Line_Number < (int)SUB_GRAPHS[Sub_Graph].LINE.size())
    {
      if (SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.size() > 0)
      {
        if (SUB_GRAPHS[Sub_Graph].LINE[Line_Number].MERGE_TIMER.ping_down(Time) == false)
        {
          SUB_GRAPHS[Sub_Graph].LINE[Line_Number].MERGE_TIMER.ping_up(Time, SUB_GRAPHS[Sub_Graph +1].TIME_PER_POINT_UL);

          unsigned long clip_time = Time - (SUB_GRAPHS[Sub_Graph].DURATION_SPAN + SUB_GRAPHS[Sub_Graph +1].TIME_PER_POINT_UL);

          if (clip_time < Time)
          {
            int point = 0;

            while (SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT[point].TIME_CREATED < clip_time)
            {
              SUB_GRAPHS[Sub_Graph].LINE[Line_Number].holdover_update(SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT[point]);

              point++;
            }

            MIN_MAX_TIME_SLICE_SIMPLE tmp_slice;

            if (SUB_GRAPHS[Sub_Graph].LINE[Line_Number].SINGLE_VALUE)
            {
              tmp_slice.MEAN_VALUE = 1.0f;
              tmp_slice.TIME_CREATED = Time;
            }
            else
            {
              tmp_slice.MEAN_VALUE = SUB_GRAPHS[Sub_Graph].LINE[Line_Number].HOLDOVER_TOTAL / SUB_GRAPHS[Sub_Graph].LINE[Line_Number].HOLDOVER_COUNT;
              tmp_slice.MAX_VALUE = SUB_GRAPHS[Sub_Graph].LINE[Line_Number].HOLDOVER_MAX;
              tmp_slice.MIN_VALUE = SUB_GRAPHS[Sub_Graph].LINE[Line_Number].HOLDOVER_MIN;
              tmp_slice.TIME_CREATED = Time;
            }

            if (point > 0)
            {
              SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.erase(
                                SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.begin(), 
                                SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.begin() + point);
            }

            if (point > 0 || SUB_GRAPHS[Sub_Graph].LINE[Line_Number].HOLDOVER_COUNT > 0)
            {
              update(Time, Sub_Graph +1, Line_Number, tmp_slice);
            }

            SUB_GRAPHS[Sub_Graph].LINE[Line_Number].holdover_clear();
          }
        }
      }
    }
  }
}


ImVec2 DRAW_D2_PLOT::position_on_plot_at_start(float Y, DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES &Graph)
{
  ImVec2 ret_vec;

  if (Y < 0.0f)
  {
    Y = 0.0f;
  }

  if (Y > PROPS.DATA_POINTS_VALUE_MAX)
  {
    Y = PROPS.DATA_POINTS_VALUE_MAX;
  }

  // Calc
  if (PROPS.LEFT_TO_RIGHT)
  {
    ret_vec.x = Graph.START_POS.x;
  }
  else
  {
    ret_vec.x = Graph.END_POS.x;
  }

  if (PROPS.BOTTOM_TO_TOP)
  {
    ret_vec.y = Graph.END_POS.y - (Y * Graph.Y_FACTOR);
  }
  else
  {
    ret_vec.y = Graph.START_POS.y + (Y * Graph.Y_FACTOR);
  }

  // Return
  return ret_vec;
}

ImVec2 DRAW_D2_PLOT::position_on_plot(float X, float Y, DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES &Graph, bool &Out_Of_Bounds_X)
{
  ImVec2 ret_vec;

  if (Y < 0.0f)
  {
    Y = 0.0f;
  }

  if (Y > PROPS.DATA_POINTS_VALUE_MAX)
  {
    Y = PROPS.DATA_POINTS_VALUE_MAX;
  }

  // Calc
  if (PROPS.LEFT_TO_RIGHT)
  {
    ret_vec.x = Graph.START_POS.x + (X * Graph.X_FACTOR);
  }
  else
  {
    ret_vec.x = Graph.END_POS.x - (X * Graph.X_FACTOR);
  }

  if (PROPS.BOTTOM_TO_TOP)
  {
    ret_vec.y = Graph.END_POS.y - (Y * Graph.Y_FACTOR);
  }
  else
  {
    ret_vec.y = Graph.START_POS.y + (Y * Graph.Y_FACTOR);
  }

  // Check 
  if (ret_vec.x < Graph.START_POS.x || ret_vec.x > Graph.END_POS.x)
  {
    Out_Of_Bounds_X = true;
  }

  // Return
  return ret_vec;
}

void DRAW_D2_PLOT::create_subgraph(int Max_Data_Point_Count, unsigned long Duration_Span_ms, string Label)
{
  DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES tmp_sub_graph;
  
  tmp_sub_graph.LABEL = Label;
  tmp_sub_graph.DATA_POINTS_COUNT_MAX = Max_Data_Point_Count;
  tmp_sub_graph.DURATION_SPAN = Duration_Span_ms;

  SUB_GRAPHS.push_back(tmp_sub_graph);
}

void DRAW_D2_PLOT::create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor, bool Single_Value)
{
  for (int graph = 0; graph < (int)SUB_GRAPHS.size(); graph++)
  {
    D2_PLOT_LINE tmp_line;

    tmp_line.LINE_COLOR = Color;
    tmp_line.DISPLAY_MEAN = Display_Mean;
    tmp_line.DISPLAY_MIN_MAX = Display_Min_Max;
    tmp_line.POINT_SIZE = Point_Size;
    tmp_line.MIN_MAX_OVERLAP_FACTOR = Min_Max_Overlap_Factor;

    tmp_line.RESERVE_SIZE = (int)((float)SUB_GRAPHS[graph].DATA_POINTS_COUNT_MAX * (float)1.5);
    tmp_line.RESERVE_SIZE_CUTOFF = (int)((float)SUB_GRAPHS[graph].DATA_POINTS_COUNT_MAX * (float)1.4);
    tmp_line.RESERVE_SIZE_TRIM_AMOUNT = (int)((float)SUB_GRAPHS[graph].DATA_POINTS_COUNT_MAX * (float).3);

    tmp_line.SINGLE_VALUE = Single_Value;

    tmp_line.DATA_POINT.reserve(tmp_line.RESERVE_SIZE);

    SUB_GRAPHS[graph].LINE.push_back(tmp_line);
  }
}

void DRAW_D2_PLOT::create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size, float Min_Max_Overlap_Factor)
{
  create_line(Color, Display_Mean, Display_Min_Max, Point_Size, Min_Max_Overlap_Factor, false);
}

void DRAW_D2_PLOT::create(unsigned long Start_Plot_Time)
{
  GRID.PROPS.COLOR = PROPS.COLOR_GRID;
  GRID.PROPS.POINT_SIZE = PROPS.POINT_SIZE_GRID;
  GRID.PROPS.SEPERATOR_COUNT_HORIZONTAL = PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL;
  GRID.PROPS.SEPERATOR_COUNT_VERTICAL = PROPS.GRID_SEPERATOR_COUNT_VERTICAL;

  TIME_START = Start_Plot_Time;

  for (int sub_graph = 0; sub_graph < (int)SUB_GRAPHS.size(); sub_graph++)
  {
    SUB_GRAPHS[sub_graph].TIME_PER_POINT_F = float(SUB_GRAPHS[sub_graph].DURATION_SPAN / SUB_GRAPHS[sub_graph].DATA_POINTS_COUNT_MAX);
    SUB_GRAPHS[sub_graph].TIME_PER_POINT_UL = (SUB_GRAPHS[sub_graph].DURATION_SPAN / SUB_GRAPHS[sub_graph].DATA_POINTS_COUNT_MAX);
  }
}

void DRAW_D2_PLOT::update(unsigned long Time, int Line_Number, float Value)
{
  if (SUB_GRAPHS.size() > 0)
  {
    if (Line_Number >= 0 && Line_Number < (int)SUB_GRAPHS[0].LINE.size())
    {
      MIN_MAX_TIME_SLICE_SIMPLE tmp_value;

      tmp_value.MEAN_VALUE = Value;
      tmp_value.MAX_VALUE = Value;
      tmp_value.MIN_VALUE = Value;
      tmp_value.TIME_CREATED = Time;

      SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT.push_back(tmp_value);

      merge(Time, 0, Line_Number);

      if ((int)SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT.size() >= SUB_GRAPHS[0].LINE[Line_Number].RESERVE_SIZE_CUTOFF)
      {
        if (1 < SUB_GRAPHS.size())
        {
          int point = 0;

          while(point < SUB_GRAPHS[0].LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT)
          {
            SUB_GRAPHS[0].LINE[Line_Number].holdover_update(SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT[point]);
            point++;
          }
        }

        SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT.erase(SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT.begin(), SUB_GRAPHS[0].LINE[Line_Number].DATA_POINT.begin() + SUB_GRAPHS[0].LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT);

      }
    }
  }
}

void DRAW_D2_PLOT::update(unsigned long Time, int Sub_Graph, int Line_Number, MIN_MAX_TIME_SLICE_SIMPLE Sample)
{  
  if (Sub_Graph < (int)SUB_GRAPHS.size())
  {
    if (Line_Number < (int)SUB_GRAPHS[Sub_Graph].LINE.size())
    {
      SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.push_back(Sample);

      if (TIME_START == 0)
      {
        TIME_START = Sample.TIME_CREATED;
      }
      
      merge(Time, Sub_Graph, Line_Number);

      if ((int)SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.size() >= SUB_GRAPHS[Sub_Graph].LINE[Line_Number].RESERVE_SIZE_CUTOFF)
      {
        if ((Sub_Graph) < (int)SUB_GRAPHS.size())
        {
          int point = 0;

          while(point < SUB_GRAPHS[Sub_Graph].LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT)
          {
            SUB_GRAPHS[Sub_Graph].LINE[Line_Number].holdover_update(SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT[point]);
            point++;
          }
        }

        SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.erase(SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.begin(), SUB_GRAPHS[Sub_Graph].LINE[Line_Number].DATA_POINT.begin() + SUB_GRAPHS[Sub_Graph].LINE[Line_Number].RESERVE_SIZE_TRIM_AMOUNT);

      }
    }
  }
}

void DRAW_D2_PLOT::update(unsigned long Time, int Line_Number, MIN_MAX_TIME_SLICE_SIMPLE Sample)
{
  update (Time, 0, Line_Number, Sample);
}

void DRAW_D2_PLOT::update_timed(unsigned long Time, int Line_Number, float Value)
{  
  if (SUB_GRAPHS.size() > 0)
  {
    SUB_GRAPHS[0].LINE[Line_Number].VALUES.store_value(Value);

    if (SUB_GRAPHS[0].LINE[Line_Number].UPDATE_SIMPLE_VALUES.ping_down(Time) == false)
    {
      MIN_MAX_TIME_SLICE_SIMPLE tmp_value;

      tmp_value.MEAN_VALUE = SUB_GRAPHS[0].LINE[Line_Number].VALUES.mean();
      tmp_value.MIN_VALUE = SUB_GRAPHS[0].LINE[Line_Number].VALUES.min();
      tmp_value.MAX_VALUE = SUB_GRAPHS[0].LINE[Line_Number].VALUES.max();
      tmp_value.TIME_CREATED = SUB_GRAPHS[0].LINE[Line_Number].VALUES.time_created();

      update(Time, 0, Line_Number, tmp_value);

      SUB_GRAPHS[0].LINE[Line_Number].VALUES.clear(Time);

      SUB_GRAPHS[0].LINE[Line_Number].UPDATE_SIMPLE_VALUES.ping_up(Time, SUB_GRAPHS[0].TIME_PER_POINT_UL);
    }
  }
}

//void DRAW_D2_PLOT::draw_graph(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
void DRAW_D2_PLOT::draw_graph(ImDrawList *Draw_List, system_data &sdSysData)
{
  for (int graph = 0; graph < (int)SUB_GRAPHS.size(); graph++)
  {
    ImGui::SetCursorScreenPos(ImVec2(SUB_GRAPHS[graph].START_POS.x + 2.0f, SUB_GRAPHS[graph].START_POS.y + 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE)));
    ImGui::Text("%s", SUB_GRAPHS[graph].LABEL.c_str());
    ImGui::PopStyleColor();

    for (int line = 0; line < (int)SUB_GRAPHS[graph].LINE.size(); line++)
    {
      bool single_value_out_of_bounds_x = false;
      bool min_max_out_of_bounds_x = false;
      bool mean_out_of_bounds_x_start = false;
      bool mean_out_of_bounds_x_end = false;
      
      ImVec2 mean_start;
      ImVec2 mean_end;
      
      if (SUB_GRAPHS[graph].LINE[line].DATA_POINT.size() > 1)
      {
        mean_end = position_on_plot((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[0].TIME_CREATED), 
                                    SUB_GRAPHS[graph].LINE[line].DATA_POINT[0].MEAN_VALUE, SUB_GRAPHS[graph], mean_out_of_bounds_x_end);
      }

      ImVec2 min;
      ImVec2 max;
      ImVec2 top;
      ImVec2 bottom;

      for (int point = 0; point < (int)SUB_GRAPHS[graph].LINE[line].DATA_POINT.size(); point++)
      {
        // Draw Single Value Line
        if (SUB_GRAPHS[graph].LINE[line].SINGLE_VALUE)
        {
          single_value_out_of_bounds_x = false;

          if (SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].MEAN_VALUE > 0.0f)
          {
            top = position_on_plot(
                  ((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].TIME_CREATED) / SUB_GRAPHS[graph].TIME_PER_POINT_F), 
                  PROPS.DATA_POINTS_VALUE_MAX, SUB_GRAPHS[graph], single_value_out_of_bounds_x);
            bottom = position_on_plot(
                  ((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].TIME_CREATED) / SUB_GRAPHS[graph].TIME_PER_POINT_F), 
                  0.0f, SUB_GRAPHS[graph], single_value_out_of_bounds_x);

            if (single_value_out_of_bounds_x == false)
            {
              Draw_List->AddLine(top, bottom, sdSysData.COLOR_SELECT.neo_color_STANDARD(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
            }
          }
        }

        // Draw Min Max
        if (SUB_GRAPHS[graph].LINE[line].DISPLAY_MIN_MAX)
        {
          min_max_out_of_bounds_x = false;

          // New plot points are added to front, plot scrolls from front
          min = position_on_plot(
                ((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].TIME_CREATED) / SUB_GRAPHS[graph].TIME_PER_POINT_F), 
                SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].MIN_VALUE, SUB_GRAPHS[graph], min_max_out_of_bounds_x);
          max = position_on_plot(
                ((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].TIME_CREATED) / SUB_GRAPHS[graph].TIME_PER_POINT_F), 
                SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].MAX_VALUE, SUB_GRAPHS[graph], min_max_out_of_bounds_x);

          if (min_max_out_of_bounds_x == false)
          {
            Draw_List->AddLine(min, max, sdSysData.COLOR_SELECT.neo_color_DIM(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
          }
        }
        
        // Draw Line
        if ((point > 0) && SUB_GRAPHS[graph].LINE[line].DISPLAY_MEAN)
        {
          mean_start = mean_end;
          mean_out_of_bounds_x_start = mean_out_of_bounds_x_end;

          mean_out_of_bounds_x_end = false;

          // New plot points are added to front, plot scrolls from front
          mean_end = position_on_plot(
                ((float)(sdSysData.PROGRAM_TIME.current_frame_time() - SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].TIME_CREATED) / SUB_GRAPHS[graph].TIME_PER_POINT_F), 
                SUB_GRAPHS[graph].LINE[line].DATA_POINT[point].MEAN_VALUE, SUB_GRAPHS[graph], mean_out_of_bounds_x_end);

          if (mean_out_of_bounds_x_start == false && mean_out_of_bounds_x_end == false)
          {
            Draw_List->AddLine(mean_start, mean_end, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].LINE[line].POINT_SIZE);
          }
        }
      }

      // Draw holdover values, if exist
      if (SUB_GRAPHS[graph].LINE[line].DISPLAY_MIN_MAX && 
          graph > 0 &&  
          SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_COUNT > 0)
      {

        min = position_on_plot_at_start(SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_MIN, SUB_GRAPHS[graph]);

        max = position_on_plot_at_start(SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_MAX, SUB_GRAPHS[graph]);

        if (min_max_out_of_bounds_x == false)
        {
          Draw_List->AddLine(min, max, sdSysData.COLOR_SELECT.neo_color_DIM(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
        }
      }
    }
  }
}

bool DRAW_D2_PLOT::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  bool ret_clicked = false;

  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    float x_size_per_subgraph = (End_Position.x - Start_Position.x) / (float)SUB_GRAPHS.size();

    for (int sub_graph = 0; sub_graph < (int)SUB_GRAPHS.size(); sub_graph++)
    {
      SUB_GRAPHS[sub_graph].X_SIZE = x_size_per_subgraph;
      SUB_GRAPHS[sub_graph].Y_SIZE = (End_Position.y - Start_Position.y);

      SUB_GRAPHS[sub_graph].X_FACTOR = SUB_GRAPHS[sub_graph].X_SIZE / (float)SUB_GRAPHS[sub_graph].DATA_POINTS_COUNT_MAX;
      SUB_GRAPHS[sub_graph].Y_FACTOR = SUB_GRAPHS[sub_graph].Y_SIZE / (float)PROPS.DATA_POINTS_VALUE_MAX;

      if (PROPS.LEFT_TO_RIGHT == true)
      {
        SUB_GRAPHS[sub_graph].START_POS = ImVec2(Start_Position.x + ((float)(sub_graph) * x_size_per_subgraph), Start_Position.y);
        SUB_GRAPHS[sub_graph].END_POS = ImVec2(Start_Position.x + ((float)(sub_graph + 1) * x_size_per_subgraph), End_Position.y);
      }
      else
      {
        SUB_GRAPHS[sub_graph].START_POS = ImVec2(End_Position.x - ((float)(sub_graph + 1) * x_size_per_subgraph), Start_Position.y);
        SUB_GRAPHS[sub_graph].END_POS = ImVec2(End_Position.x - ((float)(sub_graph) * x_size_per_subgraph), End_Position.y);
      }
    }

    FULL_X_SIZE = (End_Position.x - Start_Position.x);
    FULL_Y_SIZE = (End_Position.y - Start_Position.y);

    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;
  }

  ImGui::BeginChild(PROPS.LABEL.c_str(), ImVec2(FULL_X_SIZE, FULL_Y_SIZE), false, 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
  {
    ImDrawList* draw_list_graph = ImGui::GetWindowDrawList();

    GRID.draw(draw_list_graph, sdSysData, Start_Position, End_Position);

    draw_graph(draw_list_graph, sdSysData);

    ImGui::SetCursorScreenPos(Start_Position);
    if (ImGui::InvisibleButton("InvisibleButton", ImVec2(FULL_X_SIZE, FULL_Y_SIZE)))
    {
      ret_clicked = true;
    }

  }
  ImGui::EndChild();

  return ret_clicked;
}
// ---------------------------------------------------------------------------------------

#endif