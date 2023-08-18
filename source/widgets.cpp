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

ImU32 gradiant_color(system_data &sdSysData, unsigned long Start_time, unsigned long Duration, 
                      ImU32 Start_Color, ImU32 End_Color)
{
  ImColor ret_color;

  //return (fltElapsed / FltDuration);
  float power = (float)(sdSysData.tmeCURRENT_FRAME_TIME - Start_time) / (float)Duration;

  //ImColor color = ImColor(255, 0, 0);
  //float hue, saturation, value;
  //ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);
  //ImU32 alt;

  //ret_color.Value.

  ImU32 tmpcolor = ImColor(255, 255, 255, 255);

  int start_color_r = (Start_Color >> IM_COL32_R_SHIFT) & 0xFF;
  int start_color_g = (Start_Color >> IM_COL32_G_SHIFT) & 0xFF;
  int start_color_b = (Start_Color >> IM_COL32_B_SHIFT) & 0xFF;
  //int start_color_a = (Start_Color >> IM_COL32_A_SHIFT) & 0xFF;

  /*
  int end_color_r = (End_Color >> IM_COL32_R_SHIFT) & 0xFF;
  int end_color_g = (End_Color >> IM_COL32_G_SHIFT) & 0xFF;
  int end_color_b = (End_Color >> IM_COL32_B_SHIFT) & 0xFF;
  int end_color_a = (End_Color >> IM_COL32_A_SHIFT) & 0xFF;
  */

  int end_color_r = (tmpcolor >> IM_COL32_R_SHIFT) & 0xFF;
  int end_color_g = (tmpcolor >> IM_COL32_G_SHIFT) & 0xFF;
  int end_color_b = (tmpcolor >> IM_COL32_B_SHIFT) & 0xFF;
  //int end_color_a = (tmpcolor >> IM_COL32_A_SHIFT) & 0xFF;
  
  int r = (power * end_color_r) + ((1 - power) * start_color_r);
  int g = (power * end_color_g) + ((1 - power) * start_color_g);
  int b = (power * end_color_b) + ((1 - power) * start_color_b);
  //int a = (power * end_color_a) + ((1 - power) * start_color_a);

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
  
  return (COLOR_COMBO) ret_color_combo;
}

// ---------------------------------------------------------------------------------------

void VERTICAL_BAR::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
}

void VERTICAL_BAR::draw(system_data &sdSysData, ImVec2 Size)
{
  float slider_pos = VALUE / PROPS.VALUE_MAX;

  ImGui::PushStyleColor(ImGuiCol_FrameBg, PROPS.COLOR.BACKGROUND);
  //ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, PROPS.COLOR.STANDARD);
  ImGui::PushStyleColor(ImGuiCol_SliderGrab, PROPS.COLOR.ACTIVE);
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
    //ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.ACTIVE);
    ImGui::PushStyleColor(ImGuiCol_Text, gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.ACTIVE, PROPS.COLOR.TEXT));
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else if (PROPS.COLOR.DEFAULT == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, PROPS.COLOR.TEXT);
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(TEXT.c_str());
  }
}

// ---------------------------------------------------------------------------------------

void W_TEXT_TF::update_text(system_data &sdSysData, string True_Text, string False_Text)
{
  TEXT_TRUE = True_Text;
  TEXT_FALSE = False_Text;
}

void W_TEXT_TF::update_tf(system_data &sdSysData, bool True_False)
{
  TRUE_FALSE = True_False;
}

void W_TEXT_TF::draw(system_data &sdSysData)
{
  if (TRUE_FALSE == true)
  {
    TEXT_TRUE_FALSE.PROPS.COLOR = PROPS.COLOR_TRUE;
    TEXT_TRUE_FALSE.update_text(sdSysData, TEXT_TRUE);
  }
  else
  {
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

void TEXT_CONSOLE::display(const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    ImGui::TextUnformatted(CONSOLE_TEXT.c_str());
    if (CONSOLE_SCROLL_TO_BOTTOM == true && ImGui::GetScrollMaxY() > 0)
    {
      ImGui::SetScrollHereY(1.0f);
      CONSOLE_SCROLL_TO_BOTTOM = false;
    }
  }
  ImGui::End();
}

// ---------------------------------------------------------------------------------------

void text_simple_bool(string Text, bool Indication, COLOR_COMBO COLOR)
{
  if (Indication == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, COLOR.TEXT);
    ImGui::Text(Text.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(Text.c_str());
  }
}\

bool button_simple_enabled(string Text, bool Enabled, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

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

  return ret_value;
}

bool button_simple_color(string Text, COLOR_COMBO Color, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  if (Color.DEFAULT == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, Color.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Color.HOVERED);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Color.ACTIVE);
    if (ImGui::Button(Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(3);
  }
  else
  {
    if (ImGui::Button(Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }

  return ret_value;
}

bool button_simple_toggle_color(string True_Value_Text, string False_Value_Text, bool Toggle, 
                                COLOR_COMBO True_Color, COLOR_COMBO False_Color, ImVec2 ImVec2_Size)
{
  // Does not control toggle, just shows value.
  bool ret_value = false;

  if (False_Color.DEFAULT == false && Toggle == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, False_Color.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, False_Color.HOVERED);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, False_Color.ACTIVE);
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(3);
  }
  else if (False_Color.DEFAULT == true && Toggle == false)
  {
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, True_Color.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, True_Color.HOVERED);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, True_Color.ACTIVE);
    if (ImGui::Button(True_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(3);
  }

  return ret_value;
}

// ---------------------------------------------------------------------------------------
#endif