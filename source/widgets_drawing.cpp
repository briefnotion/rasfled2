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
  Draw_List->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_ACTIVE(RAS_YELLOW));
}

//void simple_black_back_text_box(string Text, ImDrawList *Draw_List, system_data &sdSysData)
//{
  // isnt going to work
//  ImGui::Text(Text.c_str());
//  Draw_List->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_c_black().ACTIVE, 3.0f);
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
    ImVec2 pos_1 = ImVec2(Position.x + Width - ((float)((number * 2) + 0)  * 12.0f), Position.y - 3.0f);
    ImVec2 pos_2 = ImVec2(Position.x + Width - ((float)((number * 2) + 1)  * 12.0f), Position.y - 3.0f);
    Draw_List->AddLine(pos_1, pos_2, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_WHITE), 3.0f);
  }
}

// ---------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------

/*
ImVec2 position_on_plot(PROPERTY_ORIENTATION &Orientation, ImVec2 &Point, ImVec2 &Resize_Multiplier)
{
  ImVec2 ret_vec;

  // Calc
  if (Orientation.LEFT_TO_RIGHT)
  {
    ret_vec.x = START_POS.x + (Point.x  *  Resize_Multiplier.x);
  }
  else
  {
    ret_vec.x = END_POS.x - (Point.x  *  Resize_Multiplier.x);
  }

  if (Orientation.BOTTOM_TO_TOP)
  {
    ret_vec.y = END_POS.y - (Point.y *  Resize_Multiplier.y);
  }
  else
  {
    ret_vec.y = START_POS.y + (Point.y *  Resize_Multiplier.y);
  }

  // Return
  return ret_vec;
}
*/

// ---------------------------------------------------------------------------------------

void MARKER_GADGET::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Start_Pos, ImVec2 Size, bool Display)
{
  if (PROPS.ENABLED && Display)
  {
    if (PROPS.HORIZONTAL)
    {
      Draw_List->AddRectFilled(ImVec2(Start_Pos.x, Start_Pos.y), 
                    ImVec2(Start_Pos.x + Size.x, Start_Pos.y + PROPS.SIZE), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
      // No left side, relabel for bottom is not necessary.
    }
    else
    {
      if (PROPS.LEFT_SIDE)
      {
        Draw_List->AddRectFilled(ImVec2(Start_Pos.x, Start_Pos.y), 
                      ImVec2(Start_Pos.x + PROPS.SIZE, Start_Pos.y + Size.y), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
      }
      else
      {
        Draw_List->AddRectFilled(ImVec2(Start_Pos.x + Size.x - PROPS.SIZE , Start_Pos.y), 
                      ImVec2(Start_Pos.x + Size.x, Start_Pos.y + Size.y), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR));
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
  //ImGui::Image((void*)(intptr_t)IMAGE_TEXTURE, IMAGE_SIZE);
  ImGui::Image((ImTextureID)(intptr_t)IMAGE_TEXTURE, IMAGE_SIZE);
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
  ImU32 col = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(PROPS.COLOR);

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
                          sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
    }

    // 10 ticks
    if (PROPS.MAX_TICK_LEVEL >= 2 && draw_level_2)
    {
      for (int tenths = 1; tenths < (new_max_value / 10.0f); tenths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (tenths * l2_size), Start_Position.y + (y_size * 0.5f)), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 10 halves ticks
    if (PROPS.MAX_TICK_LEVEL >= 3 && draw_level_3)
    {
      for (int twentyiths = 1; twentyiths < (new_max_value / 5.0f); twentyiths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (twentyiths * l3_size), Start_Position.y + (y_size * 0.25f)), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 1 ticks
    if (PROPS.MAX_TICK_LEVEL >= 4 && draw_level_4)
    {
      for (int hundreds = 1; hundreds < (new_max_value / 1.0f); hundreds++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y), 
                      ImVec2(Start_Position.x + (hundreds * l4_size), Start_Position.y + 1.0f), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
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
                          sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
    }

    // 10 ticks
    if (PROPS.MAX_TICK_LEVEL >= 2 && draw_level_2)
    {
      for (int tenths = 1; tenths < (new_max_value / 10.0f); tenths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (tenths * l2_size)), 
                      ImVec2(Start_Position.x + (x_size * 0.5f), Start_Position.y + (tenths * l2_size)), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 10 halves ticks
    if (PROPS.MAX_TICK_LEVEL >= 3 && draw_level_3)
    {
      for (int twentyiths = 1; twentyiths < (new_max_value / 5.0f); twentyiths++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (twentyiths * l3_size)), 
                      ImVec2(Start_Position.x + (x_size * 0.25f), Start_Position.y + (twentyiths * l3_size)), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
      }
    }

    // 1 ticks
    if (PROPS.MAX_TICK_LEVEL >= 4 && draw_level_4)
    {
      for (int hundreds = 1; hundreds < (new_max_value / 1.0f); hundreds++)
      {
        Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (hundreds * l4_size)), 
                      ImVec2(Start_Position.x + 1.0f, Start_Position.y  + (hundreds * l4_size)), 
                      sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR), PROPS.POINT_SIZE);
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
                  sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(PROPS.COLOR), PROPS.POINT_SIZE);
  }

  // Horizontal Lines
  for (int horz_count = 0; horz_count <= PROPS.SEPERATOR_COUNT_HORIZONTAL; horz_count++)
  {
    Draw_List->AddLine(ImVec2(Start_Position.x, Start_Position.y + (horz_count * Y_SIZE)), 
                  ImVec2((End_Position.x), Start_Position.y + (horz_count * Y_SIZE)), 
                  sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(PROPS.COLOR), PROPS.POINT_SIZE);
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

void DRAW_D2_PLOT::create_subgraph(unsigned long Duration_Span_ms, string Label)
{
  DRAW_D2_PLOT_SUB_GRAPH_PROPERTIES tmp_sub_graph;
  
  tmp_sub_graph.LABEL = Label;

  if (SUB_GRAPHS.size() > 0)
  { 
    //  (I dont feel like thinking about the details of full time span of previous graphs and 
    //      how often it updates.  Maybe later if memory is an issue.  After all, its only max size.)
    tmp_sub_graph.DATA_POINTS_COUNT_MAX = SUB_GRAPHS.back().DATA_POINTS_COUNT_MAX;
  }
  else
  {
    // Max amount of points calculated by frames per second * duration.
    float fps = (float)(1000 / DEF_FRAME_RATE_DELAY);
    float duration = (float)Duration_Span_ms;
    int max_count = (int)(duration * fps / 1000.0f);
    tmp_sub_graph.DATA_POINTS_COUNT_MAX = max_count;
  }

  tmp_sub_graph.DURATION_SPAN = Duration_Span_ms;

  SUB_GRAPHS.push_back(tmp_sub_graph);

  // Adjust max count to be graph size first divided by amount of sub graphs.
  // Recalculate at each create subgraph.  Done at each subgraph create beyond the first.

  //  (I dont feel like thinking about the details of full time span of previous graphs and 
  //      how often it updates.  Maybe later if memory is an issue.  After all, its only max size.
  //      Theoreticlly, this should be based on the amount of time in the subgraph time span relative 
  //      to all the previous graphs variable time spans.  Also, size of graph in pixels should be considered.)
  if (SUB_GRAPHS.size() > 0)
  {
    for (int graph = 1; graph < (int)SUB_GRAPHS.size(); graph++)
    {
      SUB_GRAPHS[graph].DATA_POINTS_COUNT_MAX = SUB_GRAPHS[0].DATA_POINTS_COUNT_MAX / (int)SUB_GRAPHS.size();
    }
  }
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
    SUB_GRAPHS[sub_graph].TIME_PER_POINT_F = float(SUB_GRAPHS[sub_graph].DURATION_SPAN) / (float)(SUB_GRAPHS[sub_graph].DATA_POINTS_COUNT_MAX);
    SUB_GRAPHS[sub_graph].TIME_PER_POINT_UL = (SUB_GRAPHS[sub_graph].DURATION_SPAN / (unsigned long)(SUB_GRAPHS[sub_graph].DATA_POINTS_COUNT_MAX));
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
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE)));
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
              Draw_List->AddLine(top, bottom, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
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
            Draw_List->AddLine(min, max, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
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
            Draw_List->AddLine(mean_start, mean_end, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].LINE[line].POINT_SIZE);
          }
        }
      }

      // Draw holdover values, if exist
      if ((SUB_GRAPHS[graph].LINE[line].DISPLAY_MIN_MAX) && 
          (graph > 0) &&  
          (SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_COUNT > 0))
      {

        min = position_on_plot_at_start(SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_MIN, SUB_GRAPHS[graph]);

        max = position_on_plot_at_start(SUB_GRAPHS[graph - 1].LINE[line].HOLDOVER_MAX, SUB_GRAPHS[graph]);

        if (min_max_out_of_bounds_x == false)
        {
          Draw_List->AddLine(min, max, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(SUB_GRAPHS[graph].LINE[line].LINE_COLOR), SUB_GRAPHS[graph].X_FACTOR * SUB_GRAPHS[graph].LINE[line].MIN_MAX_OVERLAP_FACTOR);
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

int D2_PLOT_LINE_DEGENERATE::position_in_reference_vector_from_time_span(double Time_Span)
{
  int ret_found_postion = -1;

  for (int pos = 0; pos < (int)DATA_POINT.size() -1; pos++)
  {
    if (Time_Span > POSITION_INFO[pos].GRAPH_END_TIME - POSITION_INFO[pos].GRAPH_START_TIME)
    {
      ret_found_postion = pos;
    }
  }

  return ret_found_postion;
}

void D2_PLOT_LINE_DEGENERATE::reorganize_line_data(double Time)
{
  int working_size = 0;
  
  //working_size = (int)POSITION_INFO.size() -1;
  working_size = 0;

  if (Time > LEVEL_3_TIME_TO_REORGANIZE)
  {
    LEVEL_3_TIME_TO_REORGANIZE = Time + LEVEL_3_TIME;
    working_size = (int)POSITION_INFO.size() -1;
  }
  else if (Time > LEVEL_2_TIME_TO_REORGANIZE)
  {
    LEVEL_2_TIME_TO_REORGANIZE = Time + LEVEL_2_TIME;
    working_size = LEVEL_3_WORKING_SIZE;
  }
  else if (Time > LEVEL_1_TIME_TO_REORGANIZE)
  {
    LEVEL_1_TIME_TO_REORGANIZE = Time + LEVEL_1_TIME;
    working_size = LEVEL_2_WORKING_SIZE;
  }
  else
  {
    working_size = LEVEL_1_WORKING_SIZE;
  }

  if (POSITION_INFO.size() > 0 && DATA_POINT.size() > 0 && POSITION_INFO.size() ==  DATA_POINT.size())
  {
    for (int position = 0; position < working_size; position++)
    {
      if (DATA_POINT[position].samples() > 0 && DATA_POINT[position].is_placeholder() == false)
      {
        if ((Time - DATA_POINT[position].time_created()) > POSITION_INFO[position].GRAPH_END_TIME)
        {
          POSITION_INFO[position].ARBITRARY_FLAG = true;
        }
      }
    }

    for (int position = 0; position < working_size; position++) 
    {
      if (POSITION_INFO[position].ARBITRARY_FLAG)
      {
        DATA_POINT[position + 1].merge_into(DATA_POINT[position]);
        POSITION_INFO[position].ARBITRARY_FLAG = false;
      }
    }
  }
}


/*
float DRAW_D2_PLOT_DEGENERATE::triangular_number(float X)
{
  //  y = (x * (x + 1)) / 2
  return (X * (X + 1.0f)) / 2.0f;
}

float DRAW_D2_PLOT_DEGENERATE::triangular_number_accum(float F_of_y)
{
  //  x = (-1 + sqrt(1 + 8y)) / 2)
  return (-1.0f + sqrt(1.0f + 8.0f * F_of_y));
}

float DRAW_D2_PLOT_DEGENERATE::linear_number(float X)
{
  //  y = x
  return X;
}

float DRAW_D2_PLOT_DEGENERATE::linear_accum(float F_of_y)
{
  //  x = (-1.0f + std::sqrt(1.0f + 8.0f * targetSum)) / 2.0f;
  return (-1.0f + std::sqrt(1.0f + 8.0f * F_of_y)) / 2.0f;
}

float DRAW_D2_PLOT_DEGENERATE::exp_growth_number(float X)
{
  return std::pow(2.0f, X) - 1.0f;
}

float DRAW_D2_PLOT_DEGENERATE::exp_growth_accum(float F_of_y)
{
  return std::log2(F_of_y + 1.0f);
}
*/

float DRAW_D2_PLOT_DEGENERATE::exp_growth_accum_scale_seconds(double F_of_y)
{
  return log2(F_of_y * PROPS.TIME_SCALE + 1.0f);
}

/*
float DRAW_D2_PLOT_DEGENERATE::exp_growth_accum_scale_miliseconds(double F_of_y)
{
  const double TIME_SCALE = 0.0012f;; // makes 10s ≈ 1/4 of full time
  return log2(F_of_y * TIME_SCALE + 1.0f);
}
*/

double DRAW_D2_PLOT_DEGENERATE::exp_growth_number_scale_seconds(float X)
{
  return  (pow(2.0f, (double)X) - 1.0f) / PROPS.TIME_SCALE;
}

/*
double DRAW_D2_PLOT_DEGENERATE::exp_growth_number_scale_miliseconds(float X)
{
  const double TIME_SCALE = 0.0012f;
  return  (pow(2.0f, (double)X) - 1.0f) / TIME_SCALE;
}
*/

// ---

float DRAW_D2_PLOT_DEGENERATE::value_of_accumulated_time(double Time)
{
  return exp_growth_accum_scale_seconds(Time);
}

/*
float DRAW_D2_PLOT_DEGENERATE::accum_to_x_miliseconds(double Time)
{
  return exp_growth_accum_scale_miliseconds(Time);
}
*/

float DRAW_D2_PLOT_DEGENERATE::position_in_time_to_x_coord(double Time)
{
  return value_of_accumulated_time(Time) * DEGEN_POINT_TO_SIZE_RATIO;
}

/*
float DRAW_D2_PLOT_DEGENERATE::degen_time_scale_to_x_miliseconds(double Time)
{
  return accum_to_x_miliseconds(Time) * DEGEN_POINT_TO_SIZE_RATIO;
}
*/

double DRAW_D2_PLOT_DEGENERATE::value_at_position_in_time(float X)
{
  return exp_growth_number_scale_seconds(X);
}

/*
double DRAW_D2_PLOT_DEGENERATE::time_at_position_miliseconds(float X)
{
  return exp_growth_number_scale_miliseconds(X);
}
*/

double DRAW_D2_PLOT_DEGENERATE::x_coord_to_position_in_time(int X)
{
  return value_at_position_in_time((float)X / DEGEN_POINT_TO_SIZE_RATIO);
}

bool DRAW_D2_PLOT_DEGENERATE::position_on_plot(ImVec2 &Point, ImVec2 &Position_Point)
{
  // Calc
  if (PROPS.LEFT_TO_RIGHT)
  {
    Position_Point.x = START_POS.x + (Point.x  *  RESIZE_MULTI.x);
  }
  else
  {
    Position_Point.x = END_POS.x - (Point.x  *  RESIZE_MULTI.x);
  }

  if (PROPS.BOTTOM_TO_TOP)
  {
    Position_Point.y = END_POS.y - (Point.y *  RESIZE_MULTI.y);
  }
  else
  {
    Position_Point.y = START_POS.y + (Point.y *  RESIZE_MULTI.y);
  }

  // Return
  if (Position_Point.x < START_POS.x || Position_Point.x > END_POS.x ||
      Position_Point.y < START_POS.y || Position_Point.y > END_POS.y)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/*
float DRAW_D2_PLOT_DEGENERATE::time_scale_to_x(float Time)
{
  return Time * PROPS.VECTOR_SIZE / PROPS.TIME_SPAN_MS;
}
*/

float DRAW_D2_PLOT_DEGENERATE::value_to_y(float Value)
{
  return Value * FULL_SIZE.y / PROPS.DATA_POINTS_VALUE_MAX;
}

void DRAW_D2_PLOT_DEGENERATE::draw_grid(ImDrawList *Draw_List, system_data &sdSysData)
{
  // Assign Colors
  ImColor color_border = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(RAS_BLUE);
  ImColor color_grid   = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(RAS_WHITE);

  ImVec2 point_start;
  ImVec2 point_end;

  // Draw Frame around graph
  Draw_List->AddRect(START_POS, END_POS, color_border, 8.0f , 0, 1.0f);

  // Horizontal Lines
  if (PROPS.GRID_SEPERATOR_SEGMENTS > 1)
  {
    ImVec2 screen_position_start;
    ImVec2 screen_position_end;

    for (int horz_count = 1; horz_count < PROPS.GRID_SEPERATOR_SEGMENTS; horz_count++)
    {
      point_start.x = 0.0f;
      point_start.y = value_to_y(PROPS.DATA_POINTS_VALUE_MAX * ((float)horz_count / (float)PROPS.GRID_SEPERATOR_SEGMENTS));

      point_end.x   = position_in_time_to_x_coord(PROPS.TIME_SPAN_MS);
      point_end.y = point_start.y;

      if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
      {
        Draw_List->AddLine(screen_position_start, screen_position_end, 
                            color_grid, 1.0f);
      }
    }
  }

  // Vertical Lines
  if (GRID_PROPERTIES.size() > 0)
  {
    float mid_y = PROPS.DATA_POINTS_VALUE_MAX / 2.0f;
    
    for (int property = 0; property < (int)GRID_PROPERTIES.size(); property++)
    {
      ImVec2 screen_position_start;
      ImVec2 screen_position_end;

      for (int timepoint = 1; timepoint < GRID_PROPERTIES[property].DIVIDEND_COUNT; timepoint++)
      {
        float f = (float)timepoint / (float)GRID_PROPERTIES[property].DIVIDEND_COUNT;
        float x = GRID_PROPERTIES[property].TIME_END_LOCATION * f;

        point_start.y = value_to_y(mid_y - (mid_y * f));
        point_end.y =   value_to_y(mid_y + (mid_y * f));

        point_start.x = position_in_time_to_x_coord(x);
        point_end.x = point_start.x;

        if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
        {
          Draw_List->AddLine(screen_position_start, screen_position_end, color_grid, 1.0f);
        }
      }
      
      // Draw Last Line
      // Draw Labels
      {
        ImVec2 screen_position_start;
        ImVec2 screen_position_end;

        point_start.y = value_to_y(0.0f);
        point_end.y =   value_to_y(PROPS.DATA_POINTS_VALUE_MAX);
        point_start.x = position_in_time_to_x_coord(GRID_PROPERTIES[property].TIME_END_LOCATION);
        point_end.x  = point_start.x;

        if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
        {
          Draw_List->AddLine(screen_position_start, screen_position_end, color_grid, 2.0f);
        }
        
        point_end.x -= 2.0f;

        if (position_on_plot(point_end, screen_position_end))
        {
          ImGui::SetCursorScreenPos(screen_position_end);
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE)));
          ImGui::Text("%s", GRID_PROPERTIES[property].LABEL.c_str());
          ImGui::PopStyleColor();
        }
      }
    }
  }

  /*
  ImGui::Text("16 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.016f));
  ImGui::Text("30 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.030f));
  ImGui::Text("52 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.045f));  //60
  ImGui::Text("112 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.112f)); //120
  ImGui::Text("240 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.240f));
  ImGui::Text("472 ms: %d", LINE[0].position_in_reference_vector_from_time_span(0.472f)); //480
  ImGui::Text("1 s: %d", LINE[0].position_in_reference_vector_from_time_span(0.992f));       //1s
  ImGui::Text("10 s: %d", LINE[0].position_in_reference_vector_from_time_span(09.992f));     //10s
  ImGui::Text("1 m: %d", LINE[0].position_in_reference_vector_from_time_span(59.992f));      //1m
  ImGui::Text("1 h: %d", LINE[0].position_in_reference_vector_from_time_span((60.0f * 60.0f) - .008));      //1m
  ImGui::Text("TIME_FRAME_TIME: %f", LINE[0].TIME_FRAME_TIME);
  ImGui::Text("TIME_HALF_FRAME_TIME: %f", LINE[0].TIME_HALF_FRAME_TIME);
  ImGui::Text("LEVEL_1_TIME_TO_REORGANIZE: %f", LINE[0].LEVEL_1_TIME_TO_REORGANIZE);
  ImGui::Text("LEVEL_2_TIME_TO_REORGANIZE: %f", LINE[0].LEVEL_2_TIME_TO_REORGANIZE);
  ImGui::Text("LEVEL_3_TIME_TO_REORGANIZE: %f", LINE[0].LEVEL_3_TIME_TO_REORGANIZE);
  */
 
  /*
  // Test Point poition for debugging and accuracy
  {
    ImVec2 point_start;
    ImVec2 point_end;

    vector<float> position;

    ImColor color_line_dim = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(1);

    float tmp_position;
    for (int xpos = 0; xpos < DEGEN_POINT_SIZE; xpos++)
    {
      if (x_coord_to_position_in_time(xpos) < 10.0)
      {
        tmp_position = 0.0;
      }
      else if (x_coord_to_position_in_time(xpos) < 60 * 10)
      {
        tmp_position = 20.0;
      }
      else if (x_coord_to_position_in_time(xpos) < 60 * 60)
      {
        tmp_position = 40.0;
      }
      else
      {
        tmp_position = 60.0;
      }

      position.push_back(tmp_position);
    }

    for (int xpos = 0; xpos < DEGEN_POINT_SIZE - 1; xpos++)
    {
      point_start = ImVec2(xpos, position[xpos]);
      point_end = ImVec2(xpos + 1, position[xpos + 1]);
      Draw_List->AddLine(position_on_plot(point_start), position_on_plot(point_end), color_line_dim, 1.0f);
    }
  }
  */
}

void DRAW_D2_PLOT_DEGENERATE::build_vectors()
{
  if(LINE.size() > 0)
  {
    int number_of_points = PROPS.VECTOR_SIZE;

    // Build Data_Point Vectors
    MIN_MAX_TIME_SLICE_DOUBLE tmp_empty_time_slice;
    for(int line = 0; line < (int)LINE.size(); line++)
    {
      LINE[line].DATA_POINT.reserve(number_of_points);

      for (int point = 0; point < number_of_points; point++)
      {
        LINE[line].DATA_POINT.push_back(tmp_empty_time_slice);
      }
    }

    // Build Reference Vectors
    D2_VECTOR_POSITION_REFERENCE tmp_position_info;
    for(int line = 0; line < (int)LINE.size(); line++)
    {
      LINE[line].POSITION_INFO.reserve(number_of_points);

      for (int point = 0; point < number_of_points; point++)
      {
        tmp_position_info.GRAPH_START_TIME = x_coord_to_position_in_time((float)point);
        tmp_position_info.GRAPH_END_TIME = x_coord_to_position_in_time((float)point + 1.0f);
    
        LINE[line].POSITION_INFO.push_back(tmp_position_info);
      }
    }

    for(int line = 0; line < (int)LINE.size(); line++)
    {
      // Set Timing of reorganize.
      LINE[line].TIME_FRAME_TIME = DEF_FRAME_RATE_DELAY / 1000.0f;

      LINE[line].LEVEL_1_WORKING_SIZE = LINE[line].position_in_reference_vector_from_time_span(LINE[line].LEVEL_1_TIME);
      LINE[line].LEVEL_2_WORKING_SIZE = LINE[line].position_in_reference_vector_from_time_span(LINE[line].LEVEL_2_TIME);
      LINE[line].LEVEL_2_WORKING_SIZE = LINE[line].position_in_reference_vector_from_time_span(LINE[line].LEVEL_3_TIME);
    }
  }
}

void DRAW_D2_PLOT_DEGENERATE::draw_lines(ImDrawList *Draw_List, system_data &sdSysData)
{
  if (LINE.size() > 0)
  {
    // Draw Frame and Grid
    draw_grid(Draw_List, sdSysData);

    //for every line
    for (int line_number = 0; line_number < (int)LINE.size(); line_number++)
    {
      // for every point
      //if (LINE[line_number].DATA_POINT_TILL_PIVOT.size() > 1)
      if (LINE[line_number].DATA_POINT.size() > 0)
      {
        // Screen Draw Points
        ImVec2 screen_position_start;
        ImVec2 screen_position_end;

        // Point to point start and end
        ImVec2 point_start;
        ImVec2 point_end;

        // Point Min Max
        ImVec2 point_min;
        ImVec2 point_max;

        // Values
        float value_mean = 0.0f;
        float value_min = 0.0f;
        float value_max = 0.0f;
        //double value_time_elapsed = 0.0f;

        bool start_drawing = false;

        ImColor color_line = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(LINE[line_number].LINE_COLOR);
        ImColor color_line_dim = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(LINE[line_number].LINE_COLOR);

        // Draw line
        for (int point = 0; point < (int)LINE[line_number].DATA_POINT.size(); point++)
        {
          if (LINE[line_number].DATA_POINT[point].samples() > 0)
          {
            // draw_min max
            value_min = LINE[line_number].DATA_POINT[point].min();
            value_max = LINE[line_number].DATA_POINT[point].max();
            point_min.x = point;
            point_min.y = value_to_y(value_min);
            point_max.x = point;
            point_max.y = value_to_y(value_max);
            if (position_on_plot(point_min, screen_position_start) && position_on_plot(point_max, screen_position_end))
            {
              Draw_List->AddLine(screen_position_start, screen_position_end, color_line_dim, 1.0f);
            }

            point_end.x = point;
            value_mean = LINE[line_number].DATA_POINT[point].mean();
            point_end.y = value_to_y(value_mean);

            if (start_drawing)
            {
              if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
              {
                Draw_List->AddLine(screen_position_start, screen_position_end, color_line, LINE[line_number].POINT_SIZE);
              }
            }

            start_drawing = true;
            point_start = point_end;
          }  
        }
      }
    }
  }
}

void DRAW_D2_PLOT_DEGENERATE::reorganize_all_line_data(double Time)
{
  if (LINE.size() > 0)
  {
    for (int line = 0; line < (int)LINE.size(); line++)
    {
      LINE[line].reorganize_line_data(Time);
    }
  }

  reorganize_all_line_data_flag = false;
}

void DRAW_D2_PLOT_DEGENERATE::set_reorganize_data_flag_on()
{
  reorganize_all_line_data_flag = true;
}

void DRAW_D2_PLOT_DEGENERATE::create(double Time)
{
  // Max Point Size - Calculate full size points
  MAX_POINT_SIZE_ACCUM = value_of_accumulated_time(PROPS.TIME_SPAN_MS);

  // Calculate DEGEN_POINT_TO_SIZE_RATIO
  DEGEN_POINT_TO_SIZE_RATIO = PROPS.VECTOR_SIZE / MAX_POINT_SIZE_ACCUM;
  DEGEN_POINT_SIZE = (int)(DEGEN_POINT_TO_SIZE_RATIO * MAX_POINT_SIZE_ACCUM); // Could just set as FULL_SIZE.x, but verify ratio instead

  // Build Vectors for each line
  build_vectors();

  TIME_START = (Time);

}

void DRAW_D2_PLOT_DEGENERATE::create_grid_divider(float Time_End_Location, int Divider_Count, string Label)
{
  DRAW_D2_PLOT_DEGENERATE_GRID_PROPERTIES tmp_grid_prop;

  tmp_grid_prop.TIME_END_LOCATION = Time_End_Location;
  tmp_grid_prop.DIVIDEND_COUNT    = Divider_Count;
  tmp_grid_prop.LABEL             = Label;

  GRID_PROPERTIES.push_back(tmp_grid_prop);
}


void DRAW_D2_PLOT_DEGENERATE::create_line(int Color, bool Display_Mean, bool Display_Min_Max, float Point_Size)
{
  D2_PLOT_LINE_DEGENERATE tmp_line;
  
  tmp_line.LINE_COLOR = Color;
  tmp_line.DISPLAY_MEAN = Display_Mean;
  tmp_line.DISPLAY_MIN_MAX = Display_Min_Max;
  tmp_line.POINT_SIZE = Point_Size;

  LINE.push_back(tmp_line);
}

void DRAW_D2_PLOT_DEGENERATE::update(double Time, int Line_Number, float Value)
{
  if (reorganize_all_line_data_flag)
  {
    reorganize_all_line_data(Time);
  }

  LINE[Line_Number].DATA_POINT[Line_Number].store_value(Value, Time);
}

bool DRAW_D2_PLOT_DEGENERATE::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  bool ret_clicked = false;
  
  // If graph is resized.
  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    START_POS = Start_Position;
    END_POS = End_Position;

    FULL_SIZE.x = (End_Position.x - Start_Position.x);
    FULL_SIZE.y = (End_Position.y - Start_Position.y);

    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;

    if (FIRST_RUN_COMPLETE == false)
    {
      ORIGINAL_SIZE = FULL_SIZE;

      RESIZE_MULTI.x = (FULL_SIZE.x / ORIGINAL_SIZE.x) * (ORIGINAL_SIZE.x / PROPS.VECTOR_SIZE);
      RESIZE_MULTI.y = FULL_SIZE.y / ORIGINAL_SIZE.y;

      FIRST_RUN_COMPLETE = true;
    }
    else
    {
      RESIZE_MULTI.x = (FULL_SIZE.x / ORIGINAL_SIZE.x) * (ORIGINAL_SIZE.x / PROPS.VECTOR_SIZE);
      RESIZE_MULTI.y = FULL_SIZE.y / ORIGINAL_SIZE.y;
    }

  }

  ImGui::BeginChild(PROPS.LABEL.c_str(), FULL_SIZE, false, 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
  {
    ImDrawList* draw_list_graph = ImGui::GetWindowDrawList();

    // Draw lines
    draw_lines(draw_list_graph, sdSysData);

    // Is gadget clicked
    ImGui::SetCursorScreenPos(Start_Position);
    if (ImGui::InvisibleButton("InvisibleButton", FULL_SIZE))
    {
      ret_clicked = true;
    }

  }
  ImGui::EndChild();

  return ret_clicked;
}

// ---------------------------------------------------------------------------------------

bool DRAW_D2_PLOT_POWER_CURVE::position_on_plot(ImVec2 &Point, ImVec2 &Position_Point)
{
  // Calc
  if (PROPS.ORIENTATION.LEFT_TO_RIGHT)
  {
    Position_Point.x = START_POS.x + (Point.x);
  }
  else
  {
    Position_Point.x = END_POS.x - (Point.x);
  }

  if (PROPS.ORIENTATION.BOTTOM_TO_TOP)
  {
    Position_Point.y = END_POS.y - (Point.y);
  }
  else
  {
    Position_Point.y = START_POS.y + (Point.y);
  }

  // Return
  if (Position_Point.x < START_POS.x || Position_Point.x > END_POS.x ||
      Position_Point.y < START_POS.y || Position_Point.y > END_POS.y)
  {
    return false;
  }
  else
  {
    return true;
  }
}

float DRAW_D2_PLOT_POWER_CURVE::value_to_x(float Speed)
{
  if (CURRENT_SPEED_MAX < PROPS.MAX_SPEED)
  {
    return Speed * (FULL_SIZE.x / CURRENT_SPEED_MAX);
  }
  else
  {
    return Speed * (FULL_SIZE.x / (float)(PROPS.MAX_SPEED));
  }

}

float DRAW_D2_PLOT_POWER_CURVE::value_to_y_accel(float Acceleration)
{
  return Acceleration * FULL_SIZE.y / (CURRENT_ACCELERATION_MAX * 1.2f);
}

float DRAW_D2_PLOT_POWER_CURVE::value_to_y_decel(float Deceleration)
{
  return Deceleration * FULL_SIZE.y / (CURRENT_DECELERATION_MAX * 1.2f);
}

void DRAW_D2_PLOT_POWER_CURVE::draw_lines(ImDrawList *Draw_List, system_data &sdSysData)
{
  
  // Draw Frame and Grid
  draw_grid(Draw_List, sdSysData);

  // Simple Label
  ImGui::Text("                                MAX: %.2f      MIN: %.2f", CURRENT_ACCELERATION_MAX, CURRENT_DECELERATION_MAX);
  ImGui::Text("                                v %.2f : a %.2f", LAST_SPEED_ACCELERATION_READ.x, LAST_SPEED_ACCELERATION_READ.y);
  // Spaces are the temporary solution.  :)

  // Commonly used varables

  // Screen Draw Points
  ImVec2 screen_position_start;
  ImVec2 screen_position_end;

  // Point to point start and end
  ImVec2 point_start;
  ImVec2 point_end;

  // Values
  ImColor color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_YELLOW);
  ImColor color_line_decel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED);

  ImColor color_line_accel_history = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_WHITE);
  ImColor color_line_decel_history = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_PINK);

  ImColor color_marker = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_WHITE);
  ImColor color_marker_fill = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_YELLOW);
  ImColor color_marker_fill_decel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED);

  // Draw DECELERATION
  if (SPEED_VECTORS_DECELERATION.size() > 0)
  {
    point_end.x = value_to_x(0);
    point_end.y = value_to_y_decel(SPEED_VECTORS_DECELERATION[0].max());
    float speed = 0.0f;
    for (int speed_point = 1; speed_point < (int)SPEED_VECTORS_DECELERATION.size(); speed_point++)
    {
      speed = (float)speed_point / (float)PROPS.MPH_DIVISION;
      point_start = point_end;
      point_end.x = value_to_x(speed);
      point_end.y = value_to_y_decel(SPEED_VECTORS_DECELERATION[speed_point].max());
      if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
      {
        Draw_List->AddLine(screen_position_start, screen_position_end, color_line_decel, 2.0f);
      }
    }
  }

  // Draw Acceleration
  if (SPEED_VECTORS_ACCELERATION.size() > 0)
  {
    point_end.x = value_to_x(0);
    point_end.y = value_to_y_accel(SPEED_VECTORS_ACCELERATION[0].max());
    float speed = 0.0f;
    for (int speed_point = 1; speed_point < (int)SPEED_VECTORS_ACCELERATION.size(); speed_point++)
    {
      if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 0)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_GREY);
      }
      else if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 1)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_WHITE);
      }
      else if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 2)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_ORANGE);
      }
      else if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 3)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_BLUE);
      }
      else if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 4)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_GREEN);
      }
      else if (SPEED_VECTORS_ACCELERATION_GEAR[speed_point] == 5)
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_YELLOW);
      }
      else
      {
        color_line_accel = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_PURPLE);
      }

      speed = (float)speed_point / (float)PROPS.MPH_DIVISION;
      point_start = point_end;
      point_end.x = value_to_x(speed);
      point_end.y = value_to_y_accel(SPEED_VECTORS_ACCELERATION[speed_point].max());
      if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
      {
        Draw_List->AddLine(screen_position_start, screen_position_end, color_line_accel, 4.0f);
      }
    }
  }

  // Draw_history
  if (SPEED_VECTORS_HISTORY.size() > 0)
  {
    bool acceleration_start = true;
    bool acceleration_end   = true;

    for (int position = 0; position < (int)SPEED_VECTORS_HISTORY.size(); position++)
    {
      point_end.x = value_to_x(SPEED_VECTORS_HISTORY[position].x);
      point_end.y = SPEED_VECTORS_HISTORY[position].y;

      if (point_end.y >= 0.0f)
      {
        acceleration_end = true;
        point_end.y = value_to_y_accel(point_end.y);
      }
      else
      {
        acceleration_end = false;
        point_end.y = value_to_y_decel(abs(point_end.y));
      }

      if (acceleration_start == acceleration_end)
      {
        if (position > 0)
        {
          if (acceleration_start)
          {
            if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
            {
              Draw_List->AddLine(screen_position_start, screen_position_end, color_line_accel_history, 3.0f);
            }
          }
          else
          {
            if (position_on_plot(point_start, screen_position_start) && position_on_plot(point_end, screen_position_end))
            {
              Draw_List->AddLine(screen_position_start, screen_position_end, color_line_decel_history, 3.0f);
            }
          }
        }
      }

      point_start = point_end;
      acceleration_start = acceleration_end;
    }
  }


  // Draw Indicator
  if (LAST_SPEED_ACCELERATION_READ.y >= 0.0f)
  {    
    // Draw Current Acceleration pos indicator
    point_start.x = value_to_x(LAST_SPEED_ACCELERATION_READ.x);
    point_start.y = value_to_y_accel(LAST_SPEED_ACCELERATION_READ.y);

    if (position_on_plot(point_start, screen_position_start))
    {
      Draw_List->AddCircleFilled(screen_position_start, PROPS.INDICATOR_SIZE, color_marker_fill);
      Draw_List->AddCircle(screen_position_start, PROPS.INDICATOR_SIZE, color_marker, 0, 2.0f);
    }
  }
  else
  {         
    // Draw Current Deceleration pos indicator
    point_start.x = value_to_x(LAST_SPEED_ACCELERATION_READ.x);
    point_start.y = value_to_y_decel(abs(LAST_SPEED_ACCELERATION_READ.y)); 

    if (position_on_plot(point_start, screen_position_start))
    {
      Draw_List->AddCircleFilled(screen_position_start, PROPS.INDICATOR_SIZE, color_marker_fill_decel);
      Draw_List->AddCircle(screen_position_start, PROPS.INDICATOR_SIZE, color_marker);
    }
  }
}

void DRAW_D2_PLOT_POWER_CURVE::draw_grid(ImDrawList *Draw_List, system_data &sdSysData)
{
  // Assign Colors
  ImColor color_border        = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(RAS_BLUE);
  ImColor color_grid_speed    = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(RAS_WHITE);
  ImColor color_grid_accel    = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_YELLOW);
  ImColor color_grid_decel    = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED);

  ImVec2 point_start;
  ImVec2 point_end;
  
  ImVec2 screen_position_start;
  ImVec2 screen_position_end;
  
  // Draw Frame around graph
  Draw_List->AddRect(START_POS, END_POS, color_border, 8.0f , 0, 1.0f);

  // Draw Speed
  for (int speed_line = 1; speed_line <= PROPS.MAX_SPEED / 10; speed_line++)
  {
    point_start.x = value_to_x((float)speed_line * 10.0f);
    point_start.y = 0.0f;
    point_end.x = value_to_x((float)speed_line * 10.0f);
    point_end.y = 0.0f;

    position_on_plot(point_start, screen_position_start);
    position_on_plot(point_end, screen_position_end);

    screen_position_start.y = START_POS.y;
    screen_position_end.y   = END_POS.y;

    Draw_List->AddLine(screen_position_start, screen_position_end, 
                        color_grid_speed, 1.0f);

    ImGui::SetCursorScreenPos(screen_position_start);
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE)));
    ImGui::Text("%d", (int)(speed_line * 10.0f));
    ImGui::PopStyleColor();
  }

  // Draw Accel
  for (int accel_line = 1; accel_line <= (int)CURRENT_ACCELERATION_MAX; accel_line++)
  {
    point_start.x = 0.0f;
    point_start.y = value_to_y_accel((float)accel_line);
    point_end.x = 0.0f;
    point_end.y = value_to_y_accel((float)accel_line);

    position_on_plot(point_start, screen_position_start);
    position_on_plot(point_end, screen_position_end);

    screen_position_start.x = START_POS.x;
    screen_position_end.x   = END_POS.x;

    Draw_List->AddLine(screen_position_start, screen_position_end, 
                        color_grid_accel, 1.0f);
  
  }

  // Draw Decel
  for (int decel_line = 1; decel_line <= (int)(CURRENT_DECELERATION_MAX); decel_line++)
  {
    point_start.x = 0.0f;
    point_start.y = value_to_y_decel((float)decel_line);
    point_end.x = 0.0f;
    point_end.y = value_to_y_decel((float)decel_line);

    position_on_plot(point_start, screen_position_start);
    position_on_plot(point_end, screen_position_end);

    screen_position_start.x = START_POS.x;
    screen_position_end.x   = END_POS.x;

    Draw_List->AddLine(screen_position_start, screen_position_end, 
                        color_grid_decel, 1.0f);
  
  }
  
}

void DRAW_D2_PLOT_POWER_CURVE::build_speed_vectors()
{
  // Calculate the size of the speed vectors
  // 1 per tenth of a mph
  int size = PROPS.MAX_SPEED * PROPS.MPH_DIVISION;

  SPEED_VECTORS_ACCELERATION.reserve(size);
  SPEED_VECTORS_ACCELERATION_GEAR.reserve(size);
  SPEED_VECTORS_DECELERATION.reserve(size);

  MIN_MAX_TIME_SLICE_DOUBLE tmp_empty_slice;
  for (int pos = 0; pos < size; pos++)
  {
    SPEED_VECTORS_ACCELERATION.push_back(tmp_empty_slice);
    SPEED_VECTORS_ACCELERATION_GEAR.push_back(0);
    SPEED_VECTORS_DECELERATION.push_back(tmp_empty_slice);
  }
}

void DRAW_D2_PLOT_POWER_CURVE::create()
{
  build_speed_vectors();
}

bool DRAW_D2_PLOT_POWER_CURVE::update(double Time, float Speed, float Acceleration, int Gear)
{
  bool ret_trigger_flag = false;

  LAST_SPEED_ACCELERATION_READ.x = Speed;
  LAST_SPEED_ACCELERATION_READ.y = Acceleration;

  float storage_point = Speed * (float)PROPS.MPH_DIVISION;

  // Update Speed Range if needed
  if (Speed > CURRENT_SPEED_MAX)
  {
    CURRENT_SPEED_MAX = Speed;
  }

  if (Acceleration > 0.0f && Speed < PROPS.MAX_SPEED)
  {
    // Store Speed and Acceleration
    if ((int)storage_point < (int)SPEED_VECTORS_ACCELERATION.size())
    {
      SPEED_VECTORS_ACCELERATION[(int)storage_point].store_value(Acceleration, Time);
      SPEED_VECTORS_ACCELERATION_GEAR[(int)storage_point] = Gear;
      if (Acceleration > CURRENT_ACCELERATION_MAX)
      {
        CURRENT_ACCELERATION_MAX = Acceleration;
      }

      // Check for 15% flag
      if (Acceleration > SPEED_VECTORS_ACCELERATION[(int)storage_point].max() * PROPS.TRIGGER_LOWER_LIMIT_PROXIMITY &&
          Speed > PROPS.TRIGGER_LOWER_LIMIT_SPEED)
      {
        ret_trigger_flag = true;
      }

    }
  }
  else if (Acceleration < 0.0f && Speed < PROPS.MAX_SPEED)
  {
    // Store Speed and Deceleration
    if ((int)storage_point < (int)SPEED_VECTORS_DECELERATION.size())
    {
      SPEED_VECTORS_DECELERATION[(int)storage_point].store_value(abs(Acceleration), Time);
      if (abs(Acceleration) > CURRENT_DECELERATION_MAX)
      {
        CURRENT_DECELERATION_MAX = abs(Acceleration);
      }

      // Check for 15% flag
      if (abs(Acceleration) > SPEED_VECTORS_DECELERATION[(int)storage_point].max() * PROPS.TRIGGER_LOWER_LIMIT_PROXIMITY)
      {
        ret_trigger_flag = true;
      }

    }
  }

  // Update history line
  SPEED_VECTORS_HISTORY_UPDATE_COUNTER++;
  if (SPEED_VECTORS_HISTORY_UPDATE_COUNTER >= 20)
  {
    SPEED_VECTORS_HISTORY_UPDATE_COUNTER = 0;
    ImVec2 speed_acceleration = ImVec2(Speed, Acceleration);
    SPEED_VECTORS_HISTORY.push_back(speed_acceleration);
    while ((int)SPEED_VECTORS_HISTORY.size() > PROPS.HISTORY_SIZE)
    {
      SPEED_VECTORS_HISTORY.pop_front();
    }
  }

  return ret_trigger_flag;
}

bool DRAW_D2_PLOT_POWER_CURVE::draw(system_data &sdSysData, ImVec2 Start_Position, ImVec2 End_Position)
{
  bool ret_clicked = false;

  // If graph is resized.
  if (PREV_START_POS.x != Start_Position.x || PREV_START_POS.y != Start_Position.y || 
      PREV_END_POS.x != End_Position.x || PREV_END_POS.y != End_Position.y)
  {
    START_POS = Start_Position;
    END_POS = End_Position;
    FULL_SIZE.x = (End_Position.x - Start_Position.x);
    FULL_SIZE.y = (End_Position.y - Start_Position.y);
    PREV_START_POS = Start_Position;
    PREV_END_POS = End_Position;

    if (FIRST_RUN_COMPLETE == false)
    {
      FIRST_RUN_COMPLETE = true;
      ORIGINAL_SIZE = FULL_SIZE;
      TIME_START = (((double)sdSysData.PROGRAM_TIME.current_frame_time()) / 1000.0);
    }
  }
  
  ImGui::BeginChild(PROPS.LABEL.c_str(), FULL_SIZE, false, 
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
  {
    ImDrawList* draw_list_graph = ImGui::GetWindowDrawList();

    // Draw lines
    draw_lines(draw_list_graph, sdSysData);

    // Is gadget clicked
    ImGui::SetCursorScreenPos(Start_Position);
    if (ImGui::InvisibleButton("InvisibleButton", FULL_SIZE))
    {
      ret_clicked = true;
    }
  }
  ImGui::EndChild();

  return ret_clicked;
}

// ---------------------------------------------------------------------------------------

#endif