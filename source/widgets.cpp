// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_CPP
#define WIDGETS_CPP

#include "widgets.h"

// ---------------------------------------------------------------------------------------

ImColor gradiant_color(system_data &sdSysData, unsigned long Start_time, unsigned long Duration, 
                      ImColor Start_Color, ImColor End_Color)
{
  ImColor ret_color;

  float power = (float)(sdSysData.PROGRAM_TIME.current_frame_time() - Start_time) / (float)Duration;
  
  float r = (power * (float)End_Color.Value.x) + ((1 - power) * (float)Start_Color.Value.x);
  float g = (power * (float)End_Color.Value.y) + ((1 - power) * (float)Start_Color.Value.y);
  float b = (power * (float)End_Color.Value.z) + ((1 - power) * (float)Start_Color.Value.z);
  float a = (power * (float)End_Color.Value.w) + ((1 - power) * (float)Start_Color.Value.w);

  return ImColor(r, g, b);
}

// ---------------------------------------------------------------------------------------

ImColor BOOLEAN_GRADIANT::boolean_color(system_data &sdSysData, bool Value, ImColor True_Color, ImColor False_Color)
{
  if (VALUE != Value)
  {
    VALUE = Value;

    if (VALUE)
    {
      COLOR_START = False_Color;
      COLOR_DESTINATION = True_Color;
    }
    else
    {
      COLOR_START = True_Color;
      COLOR_DESTINATION = False_Color;
    }

    if (ACTIVE.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
    { 
      unsigned long time_elapsed = sdSysData.PROGRAM_TIME.current_frame_time() - ACTIVE.start_time();
      unsigned long time_remaining = DURATION - (time_elapsed);

      ACTIVE.ping_up(sdSysData.PROGRAM_TIME.current_frame_time() - time_remaining, DURATION);
    }
    else
    {
      ACTIVE.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), DURATION);
    }

  }

  if (ACTIVE.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
  {
    return gradiant_color(sdSysData, ACTIVE.start_time(), DURATION, COLOR_START, COLOR_DESTINATION);
  }
  else
  {
    if (VALUE)
    {
      return True_Color;
    }
    else
    {
      return False_Color;
    }
  }
}

// ---------------------------------------------------------------------------------------

void IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::set_size(int Size)
{
  SIZE = Size;
  FIRST_RUN = true;
}

void IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::set_value(float Value)
{
  if (FIRST_RUN)
  {
    VALUE_COLLECTION.reserve(SIZE);

    for (int count = VALUE_COLLECTION.size(); count < SIZE; count++)
    {
      VALUE_COLLECTION.push_back( 0.0f );
    }

    READ_WRITE_POS = 0;

    FIRST_RUN = false;
  }
  
  LATEST_VALUE = Value;
}

float IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::value()
{
  READ_WRITE_POS++;
  if (READ_WRITE_POS >= SIZE)
  {
    READ_WRITE_POS = 0;
  }

  SUMMATION = SUMMATION + LATEST_VALUE - VALUE_COLLECTION[READ_WRITE_POS];
  
  VALUE_COLLECTION[READ_WRITE_POS] = LATEST_VALUE;

  if (VALUE_COLLECTION.size() > 0)
  {
    return (SUMMATION / (float)SIZE);
  }
  else
  {
    return LATEST_VALUE;
  }
}

// ---------------------------------------------------------------------------------------

bool NEW_COLOR_SCALE::active()
{
  if (COLOR_SCALE.size() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void NEW_COLOR_SCALE::add_color_value_pair(float Value_Is_LT_or_EQ, int Return_Color)
{
  COLOR_VALUE_PAIR temp_color_pair;

  temp_color_pair.LT_or_EQ = Value_Is_LT_or_EQ;
  temp_color_pair.COLOR = Return_Color;

  COLOR_SCALE.push_back(temp_color_pair);
}

int NEW_COLOR_SCALE::get_color(float Value)
{
  int ret_color_combo;
  bool found = false;
  
  if (COLOR_SCALE.size() > 0)
  {
    for (int pos = 0; pos < COLOR_SCALE.size() && found == false; pos++)
    {
      if (Value <= COLOR_SCALE[pos].LT_or_EQ)
      {
        ret_color_combo = COLOR_SCALE[pos].COLOR;
        found = true;
      }
    }
  }
  
  return ret_color_combo;
}

// ---------------------------------------------------------------------------------------

void W_TEXT::update_text(system_data &sdSysData, string Text)
{
  if (TEXT != Text)
  {
    TEXT = Text;

    UPDATE_TIMED.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 500);
  }
}

void W_TEXT::draw(system_data &sdSysData)
{
  if (PROPS.CHANGE_NOTIFICATION == true && UPDATE_TIMED.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.c_orange().ACTIVE, sdSysData.COLOR_SELECT.color(PROPS.COLOR).STANDARD)));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.c_orange().ACTIVE, sdSysData.COLOR_SELECT.color(PROPS.COLOR).TEXT)));
    }
    
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.color(PROPS.COLOR).STANDARD));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.color(PROPS.COLOR).TEXT));
    }
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
}

// ---------------------------------------------------------------------------------------

void W_TEXT_TF::update_text(system_data &sdSysData, string True_Text, string False_Text)
{
  TEXT_TRUE = True_Text;
  TEXT_FALSE = False_Text;
}

bool W_TEXT_TF::update_tf(system_data &sdSysData, bool True_False)
{
  if (TRUE_FALSE == True_False)
  {
    return false;
  }
  else
  {
    TRUE_FALSE = True_False;
    return true;
  }
}

void W_TEXT_TF::draw(system_data &sdSysData)
{
  if (TRUE_FALSE == true)
  {
    TEXT_TRUE_FALSE.PROPS.STANDARD_COLOR = false;
    TEXT_TRUE_FALSE.PROPS.COLOR = PROPS.COLOR_TRUE;
    TEXT_TRUE_FALSE.update_text(sdSysData, TEXT_TRUE);
  }
  else
  {
    TEXT_TRUE_FALSE.PROPS.STANDARD_COLOR = true;
    TEXT_TRUE_FALSE.PROPS.COLOR = PROPS.COLOR_FALSE;
    TEXT_TRUE_FALSE.update_text(sdSysData, TEXT_FALSE);
  }

  TEXT_TRUE_FALSE.draw(sdSysData);
}

// ---------------------------------------------------------------------------------------

void TEXT_CONSOLE::add_line(string Text)
{
  CONSOLE_TEXT = CONSOLE_TEXT + "\n" + Text;
  CONSOLE_SCROLL_TO_BOTTOM = true;

  if (CONSOLE_TEXT.size() > 1024 * 10)
  {
    CONSOLE_TEXT.erase(0, CONSOLE_TEXT.find_first_of("\n", 512 ) + 1);
  } 
}

void TEXT_CONSOLE::display(system_data &sdSysData, const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));

    ImGui::TextUnformatted(CONSOLE_TEXT.c_str());
    if (CONSOLE_SCROLL_TO_BOTTOM == true && ImGui::GetScrollMaxY() > 0)
    {
      ImGui::SetScrollHereY(1.0f);
      CONSOLE_SCROLL_TO_BOTTOM = false;
    }
    
    ImGui::PopStyleColor();
  }
  ImGui::End();
}

// ---------------------------------------------------------------------------------------

void text_simple_bool(system_data &sdSysData, string Text, bool Indication, int COLOR)
{
  if (Indication == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.color(COLOR).TEXT));
    ImGui::Text(Text.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(Text.c_str());
  }
}

bool button_simple_enabled(system_data &sdSysData, string Text, bool Enabled, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));

  if (Enabled == true)
  {
    if (ImGui::Button(Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(sdSysData.COLOR_SELECT.c_blue().BACKGROUND)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(sdSysData.COLOR_SELECT.c_blue().BACKGROUND));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(sdSysData.COLOR_SELECT.c_blue().BACKGROUND));
    ImGui::Button(Text.c_str(), ImVec2_Size);
    ImGui::PopStyleColor(3);
  }

  ImGui::PopStyleColor();

  return ret_value;
}

bool button_simple_color(system_data &sdSysData, string Text, int Color, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));

  ImGui::PushStyleColor(ImGuiCol_Button, ImU32(sdSysData.COLOR_SELECT.color(Color).STANDARD));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(sdSysData.COLOR_SELECT.color(Color).HOVERED));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(sdSysData.COLOR_SELECT.color(Color).ACTIVE));
  if (ImGui::Button(Text.c_str(), ImVec2_Size))
  {
    ret_value = true;
  }
  ImGui::PopStyleColor(4);

  return ret_value;
}

bool button_simple_toggle_color(system_data &sdSysData, string True_Value_Text, string False_Value_Text, bool Toggle, 
                                int True_Color, int False_Color, ImVec2 ImVec2_Size)
{
  // Does not control toggle, just shows value.
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));

  if (Toggle == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(sdSysData.COLOR_SELECT.color(False_Color).STANDARD));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(sdSysData.COLOR_SELECT.color(False_Color).HOVERED));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(sdSysData.COLOR_SELECT.color(False_Color).ACTIVE));
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(4);
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(sdSysData.COLOR_SELECT.color(True_Color).STANDARD));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(sdSysData.COLOR_SELECT.color(True_Color).HOVERED));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(sdSysData.COLOR_SELECT.color(True_Color).ACTIVE));
    if (ImGui::Button(True_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(4);
  }

  return ret_value;
}

// ---------------------------------------------------------------------------------------

void BAR_TECH::draw_min_max_val(system_data &sdSysData)
{
  if ((int)MIN_MAX.min() < 0)
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }
  else
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.yellow();
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.yellow();
  }
  else
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }

  // Display Values
  if (PROPS.DISPLAY_SINGLE_POINT_FLOAT)
  {
    DSP_MIN.update_text(sdSysData, to_string_round_to_nth(MIN_MAX.min_float(), 1));
    DSP_VALUE.update_text(sdSysData, to_string_round_to_nth(VALUE, 1));
    DSP_MAX.update_text(sdSysData, to_string_round_to_nth(MIN_MAX.max_float(), 1));
  }
  else
  {
    DSP_MIN.update_text(sdSysData, to_string((int)(MIN_MAX.min())));
    DSP_VALUE.update_text(sdSysData, to_string((int)(VALUE)));
    DSP_MAX.update_text(sdSysData, to_string((int)(MIN_MAX.max())));  
  }

  if (ImGui::BeginTable("Automobile Guage Val Min Max", 4, 0))
  {
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));
      ImGui::Text(PROPS.LABEL.c_str());
      ImGui::PopStyleColor();

      ImGui::TableNextColumn();
      DSP_MIN.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_VALUE.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_MAX.draw(sdSysData);
    }
    ImGui::EndTable();
  }
}

void BAR_TECH::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;

  RULER.PROPS.COLOR = PROPS.COLOR_RULER;
  RULER.PROPS.MAX_VALUE = PROPS.MAX;
  RULER.PROPS.MAX_TICK_LEVEL = PROPS.MAX_TICK_LEVEL;
  RULER.PROPS.HORIZONTAL = PROPS.HORIZONTAL;

  DSP_MIN.PROPS.CHANGE_NOTIFICATION = true;
  DSP_MAX.PROPS.CHANGE_NOTIFICATION = true;
  DSP_VALUE.PROPS.CHANGE_NOTIFICATION = false;
}

void BAR_TECH::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
  VALUE_MARKER.set_value(Value);

  if (PROPS.DRAW_MIN_MAX)
  {
    MIN_MAX.put_value(Value, sdSysData.PROGRAM_TIME.current_frame_time());
  }
}

void BAR_TECH::draw(system_data &sdSysData)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();

  // Min Max
  if (PROPS.HORIZONTAL && PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_TOP)
  {
    draw_min_max_val(sdSysData);
  }

  // Draw
  ImVec2 pos = ImGui::GetCursorScreenPos();
  
  ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

  // Draw Background - Red if negative, Normal color if positive.
  if (PROPS.HORIZONTAL)
  {
    if (VALUE >= 0)
    {
      draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.color(PROPS.COLOR_BACKGROUND).DIM, 5.0f, ImDrawFlags_None);
      draw_list->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.color(PROPS.COLOR_BACKGROUND).BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.c_red().DIM, 5.0f, ImDrawFlags_None);
      draw_list->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.c_red().BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
    }
  }
  else
  {
    if (VALUE >= 0)
    {
      draw_list->AddRectFilled(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), sdSysData.COLOR_SELECT.color(PROPS.COLOR_BACKGROUND).DIM, 5.0f, ImDrawFlags_None);
      draw_list->AddRect(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), sdSysData.COLOR_SELECT.color(PROPS.COLOR_BACKGROUND).BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      draw_list->AddRectFilled(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), sdSysData.COLOR_SELECT.c_red().DIM, 5.0f, ImDrawFlags_None);
      draw_list->AddRect(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), sdSysData.COLOR_SELECT.c_red().BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
    }
  }

  // Draw Min Max Bar
  if (PROPS.HORIZONTAL)
  {
    if (PROPS.DRAW_MIN_MAX)
    {
      float min_location = (MIN_MAX.min_float() / PROPS.MAX) * size.x;
      float max_location = (MIN_MAX.max_float() / PROPS.MAX) * size.x;

      if (min_location < 0)
      {
        min_location = 0;
      }
      else if (min_location > size.x)
      {
        min_location = size.x;
      }

      if (max_location < 0)
      {
        max_location = 0;
      }
      else if (max_location > size.x)
      {
        max_location = size.x;
      }

      draw_list->AddRectFilled(ImVec2(pos.x + min_location, pos.y), 
                                ImVec2(pos.x + max_location, pos.y + PROPS.BAR_HEIGHT), 
                                sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).DIM, 5.0f, ImDrawFlags_None);

      draw_list->AddRect(ImVec2(pos.x + min_location, pos.y +2), 
                                ImVec2(pos.x + max_location, pos.y + PROPS.BAR_HEIGHT -2), 
                                sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);

      if (PROPS.DRAW_RULER)
      {
        RULER.draw(sdSysData, pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT));
      }
    }
  }
  else
  {
    if (PROPS.DRAW_MIN_MAX)
    {
      float min_location = (MIN_MAX.min_float() / PROPS.MAX) * size.y;
      float max_location = (MIN_MAX.max_float() / PROPS.MAX) * size.y;

      if (min_location < 0)
      {
        min_location = 0;
      }
      else if (min_location > size.y)
      {
        min_location = size.y;
      }

      if (max_location < 0)
      {
        max_location = 0;
      }
      else if (max_location > size.y)
      {
        max_location = size.y;
      }

      draw_list->AddRectFilled(ImVec2(pos.x, pos.y + size.y - max_location), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - min_location), 
                                sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).DIM, 5.0f, ImDrawFlags_None);

      draw_list->AddRect(ImVec2(pos.x, pos.y + size.y - max_location -2), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - min_location +2), 
                                sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);

      if (PROPS.DRAW_RULER)
      {
        RULER.draw(sdSysData, pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y));
      }
    }
  }

  // Draw Value Marker
  if (PROPS.HORIZONTAL)
  {
    float marker_location = abs((VALUE_MARKER.value() / PROPS.MAX) * size.x +1);

    if (marker_location < 0)
    {
      marker_location = 0;
    }
    if (marker_location > size.x)
    {
      marker_location = size.x;
    }

    draw_list->AddRectFilled(ImVec2(pos.x + marker_location - PROPS.MARKER_SIZE/2, pos.y), 
                              ImVec2(pos.x + marker_location + PROPS.MARKER_SIZE/2 , pos.y + PROPS.BAR_HEIGHT), 
                              sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).STANDARD_V, 5.0f, ImDrawFlags_None);

    // Move Cursor Pos to new position
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + PROPS.BAR_HEIGHT));

    // Min Max
    if (PROPS.HORIZONTAL && PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_BOTTOM)
    {
      draw_min_max_val(sdSysData);
    }
  }
  else
  {
    float marker_location = abs((VALUE_MARKER.value() / PROPS.MAX) * size.y +1);

    if (marker_location < 0)
    {
      marker_location = 0;
    }
    if (marker_location > size.y)
    {
      marker_location = size.y;
    }

    draw_list->AddRectFilled(ImVec2(pos.x, pos.y + size.y - (marker_location + PROPS.MARKER_SIZE/2)), 
                              ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - (marker_location - PROPS.MARKER_SIZE/2)), 
                              sdSysData.COLOR_SELECT.color(PROPS.COLOR_MARKER).STANDARD_V, 5.0f, ImDrawFlags_None);

    // Move Cursor Pos to new position
    ImGui::SetCursorScreenPos(ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y));
  }
}

// ---------------------------------------------------------------------------------------
#endif