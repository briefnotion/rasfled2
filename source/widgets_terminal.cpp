// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_terminal.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_TERMINAL_CPP
#define WIDGETS_TERMINAL_CPP

#include "widgets_terminal.h"

// ---------------------------------------------------------------------------------------

void TERMINAL_SCREEN::display(system_data &sdSysData)
{
  ImVec4 working_area = get_working_area();

  //bool move_focus = false;

  // ---

  float footer_height = ImGui::GetFrameHeightWithSpacing(); 

  ImGui::SetNextWindowSize(ImVec2(working_area.z, working_area.w - footer_height));
  ImGui::Begin("Terminal", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);

  {
    // Access bash
    {
      std::lock_guard<std::mutex> lock(sdSysData.TERMINAL_THREAD.BUF_MUTEX);

      // Display each line from the terminal buffer
      for (auto &line : sdSysData.TERMINAL_THREAD.TERM_BUF) 
      {
        ImGui::TextUnformatted(line.c_str());
      }
    }

    // Scroll
    {
      // Auto-scroll to bottom if already near bottom
      if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      {
        ImGui::SetScrollHereY(1.0f);
      }
    }

    // Handle focus if user clicks anywhere in terminal window
    //if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) &&
    //    ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    //{
    //  move_focus = true;
    //}

    ImGui::EndChild();
  }

  // ---

  // Not Working Yet
  //if (move_focus)
  //{
  //  ImGui::SetKeyboardFocusHere(); // focus the input below
  //}

  // Command input 
  { 
    ImGui::PushItemWidth(-FLT_MIN);  // make input take full width
    
    if (ImGui::InputText("##Command", input, sizeof(input),
                          ImGuiInputTextFlags_EnterReturnsTrue)) 
    {
      sdSysData.TERMINAL_THREAD.send_command(input);
      input[0] = '\0';
      ImGui::SetKeyboardFocusHere(-1); // focus this input again
    }
  }
}

// ---------------------------------------------------------------------------------------

#endif