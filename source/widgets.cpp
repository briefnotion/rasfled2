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

bool button_simple_color(string Text, float H_Color, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  if (H_Color >= 0)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(H_Color, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(H_Color, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(H_Color, 0.8f, 0.8f));
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

bool button_simple_toggle_color(string True_Value_Text, string False_Value_Text, bool Toggle, float H_Color, ImVec2 ImVec2_Size)
{
  // Does not control toggle, just shows value.
  bool ret_value = false;

  if (H_Color >= 0 && Toggle == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(H_Color, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(H_Color, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(H_Color, 0.8f, 0.8f));
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(3);
  }
  else if (H_Color < 0 && Toggle == false)
  {
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }
  else if (H_Color >= 0 && Toggle == true)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(H_Color, 0.0, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(H_Color, 0.0f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(H_Color, 0.0f, 0.8f));
    if (ImGui::Button(True_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(3);
  }  
  else if (H_Color < 0 && Toggle == true)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.00, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.0f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.0f, 0.8f));
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