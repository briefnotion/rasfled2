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

void W_TEXT::update_text(unsigned long tmeCurrentMillis, string Text)
{
  TEXT = Text;
}

void W_TEXT::draw(unsigned long tmeCurrentMillis)
{
  if (PROPS.COLOR.DEFAULT == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, PROPS.COLOR.ACTIVE);
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(TEXT.c_str());
  }
}

// ---------------------------------------------------------------------------------------

void W_TEXT_TF::update_text(unsigned long tmeCurrentMillis, string True_Text, string False_Text)
{
  TEXT_TRUE = True_Text;
  TEXT_FALSE = False_Text;
}

void W_TEXT_TF::update_tf(unsigned long tmeCurrentMillis, bool True_False)
{
  TRUE_FALSE = True_False;
}

void W_TEXT_TF::draw(unsigned long &tmeCurrentMillis)
{
  if (TRUE_FALSE == true && PROPS.COLOR_TRUE.DEFAULT == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, PROPS.COLOR_TRUE.ACTIVE);
    ImGui::Text(TEXT_TRUE.c_str());
    ImGui::PopStyleColor();
  }
  else if (TRUE_FALSE == true && PROPS.COLOR_TRUE.DEFAULT == true)
  {
    ImGui::Text(TEXT_TRUE.c_str());
  }
  else if (TRUE_FALSE == false && PROPS.COLOR_FALSE.DEFAULT == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, PROPS.COLOR_FALSE.ACTIVE);
    ImGui::Text(TEXT_FALSE.c_str());
    ImGui::PopStyleColor();
  }
  else // if (TRUE_FALSE == false && PROPS.COLOR_FALSE.DEFAULT == true)
  {
    ImGui::Text(TEXT_FALSE.c_str());
  }
}

// ---------------------------------------------------------------------------------------

void COLOR_COMBO::set(float H, float S, float V)
{
STANDARD = ImColor::HSV(H, S, V);
HOVERED = ImColor::HSV(H, S + .1, V + .1);
ACTIVE = ImColor::HSV(H, S + .2, V + .2);
DEFAULT = false;
}

void COLOR_COMBOS::init()
{
  COLOR_COMB_BLACK.set(0.0, 0.0f, 0.0f);
  COLOR_COMB_WHITE.set(0.0, 0.0f, .6f);
  COLOR_COMB_RED.set(0.0, 0.6f, 0.6f);
  COLOR_COMB_YELLOW.set(0.254f, 0.6f, 0.6f);
  COLOR_COMB_GREEN.set(0.333f, 0.6f, 0.6f);
  COLOR_COMB_CYAN.set(0.5f, 0.6f, 0.6f);
  COLOR_COMB_BLUE.set(0.667f, 0.6f, 0.6f);
  COLOR_COMB_PURPLE.set(0.768f, 0.6f, 0.6f);
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

void text_simple_bool(string Text, bool Indication)
{
  if (Indication == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
    ImGui::Text(Text.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(Text.c_str());
  }
}

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