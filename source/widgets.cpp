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

  float power = (float)(sdSysData.tmeCURRENT_FRAME_TIME - Start_time) / (float)Duration;
  
  float r = (power * (float)End_Color.Value.x) + ((1 - power) * (float)Start_Color.Value.x);
  float g = (power * (float)End_Color.Value.y) + ((1 - power) * (float)Start_Color.Value.y);
  float b = (power * (float)End_Color.Value.z) + ((1 - power) * (float)Start_Color.Value.z);
  float a = (power * (float)End_Color.Value.w) + ((1 - power) * (float)Start_Color.Value.w);

  return ImColor(r, g, b);
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

void NEW_COLOR_SCALE::add_color_value_pair(float Value_Is_LT_or_EQ, COLOR_COMBO Return_Color)
{
  COLOR_VALUE_PAIR temp_color_pair;

  temp_color_pair.LT_or_EQ = Value_Is_LT_or_EQ;
  temp_color_pair.COLOR = Return_Color;

  COLOR_SCALE.push_back(temp_color_pair);
}

COLOR_COMBO NEW_COLOR_SCALE::get_color(float Value)
{
  COLOR_COMBO ret_color_combo;
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

void VERTICAL_BAR::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
}

void VERTICAL_BAR::draw(system_data &sdSysData, ImVec2 Size)
{
  float slider_pos = VALUE / PROPS.VALUE_MAX;

  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImU32(PROPS.COLOR.BACKGROUND));
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImU32(PROPS.COLOR.STANDARD));
  ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImU32(PROPS.COLOR.ACTIVE));
  ImGui::VSliderFloat("##v", Size, &slider_pos, 0.0f, 1.0f, "", sdSysData.SCREEN_DEFAULTS.flags_vs);
  ImGui::PopStyleColor(3);
}

// ---------------------------------------------------------------------------------------

void W_TEXT::update_text(system_data &sdSysData, string Text)
{
  if (TEXT != Text)
  {
    TEXT = Text;

    UPDATE_TIMED.ping_up(sdSysData.tmeCURRENT_FRAME_TIME, 500);
  }
}

void W_TEXT::draw(system_data &sdSysData)
{
  if (PROPS.CHANGE_NOTIFICATION == true && UPDATE_TIMED.ping_down(sdSysData.tmeCURRENT_FRAME_TIME))
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.ACTIVE, PROPS.COLOR.STANDARD)));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.ACTIVE, PROPS.COLOR.TEXT)));
    }
    
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(PROPS.COLOR.STANDARD));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(PROPS.COLOR.TEXT));
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
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));

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

void text_simple_bool(string Text, bool Indication, COLOR_COMBO COLOR)
{
  if (Indication == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(COLOR.TEXT));
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

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));

  if (Enabled == true)
  {
    if (ImGui::Button(Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }
  else
  {
    ImGui::BeginDisabled();
    ImGui::Button(Text.c_str(), ImVec2_Size);
    ImGui::EndDisabled();
  }

  ImGui::PopStyleColor();

  return ret_value;
}

bool button_simple_color(system_data &sdSysData, string Text, COLOR_COMBO Color, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));

  ImGui::PushStyleColor(ImGuiCol_Button, ImU32(Color.STANDARD));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(Color.HOVERED));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(Color.ACTIVE));
  if (ImGui::Button(Text.c_str(), ImVec2_Size))
  {
    ret_value = true;
  }
  ImGui::PopStyleColor(4);

  return ret_value;
}

bool button_simple_toggle_color(system_data &sdSysData, string True_Value_Text, string False_Value_Text, bool Toggle, 
                                COLOR_COMBO True_Color, COLOR_COMBO False_Color, ImVec2 ImVec2_Size)
{
  // Does not control toggle, just shows value.
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));

  if (Toggle == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(False_Color.STANDARD));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(False_Color.HOVERED));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(False_Color.ACTIVE));
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(4);
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(True_Color.STANDARD));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(True_Color.HOVERED));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(True_Color.ACTIVE));
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
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
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
      
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));
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

  DSP_MIN.PROPS.CHANGE_NOTIFICATION = true;
  DSP_MAX.PROPS.CHANGE_NOTIFICATION = true;
  DSP_VALUE.PROPS.CHANGE_NOTIFICATION = false;
}

void BAR_TECH::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
  if (PROPS.DRAW_MIN_MAX)
  {
    MIN_MAX.put_value(Value, sdSysData.tmeCURRENT_FRAME_TIME);
  }
}

void BAR_TECH::draw(system_data &sdSysData)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();

  // Min Max
  if (PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_TOP)
  {
    draw_min_max_val(sdSysData);
  }

  // Draw
  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

  // Draw Background
  if (VALUE >= 0)
  {
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), PROPS.COLOR_BACKGROUND.DIM, 5.0f, ImDrawFlags_None);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), PROPS.COLOR_BACKGROUND.BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
  }
  else
  {
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.COLOR_COMB_RED.DIM, 5.0f, ImDrawFlags_None);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), sdSysData.COLOR_SELECT.COLOR_COMB_RED.BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
  }

  // Draw Min Max Bar
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
                              PROPS.COLOR_MARKER.DIM, 5.0f, ImDrawFlags_None);

    draw_list->AddRect(ImVec2(pos.x + min_location, pos.y +2), 
                              ImVec2(pos.x + max_location, pos.y + PROPS.BAR_HEIGHT -2), 
                              PROPS.COLOR_MARKER.BACKGROUND, 5.0f, ImDrawFlags_None, 2.0f);
  }

  // Draw Value Marker
  
  //float marker_location = abs((VALUE / PROPS.MAX) * (size.x - PROPS.MARKER_SIZE *2));
  float marker_location = abs((VALUE / PROPS.MAX) * size.x +1);

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
                            PROPS.COLOR_MARKER.STANDARD, 5.0f, ImDrawFlags_None);

  // Move Cursor Pos to new position
  ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + PROPS.BAR_HEIGHT));

  // Min Max
  if (PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_BOTTOM)
  {
    draw_min_max_val(sdSysData);
  }
}

// ---------------------------------------------------------------------------------------
#endif