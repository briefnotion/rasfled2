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

  float footer_height = ImGui::GetFrameHeightWithSpacing(); 

  ImGui::SetNextWindowSize(ImVec2(working_area.z, working_area.w - footer_height));
  ImGui::Begin("Terminal", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);

  {
    {
      // Use the public access method from the class (or BUF_MUTEX if you made it public)
      std::lock_guard<std::mutex> lock(sdSysData.TERMINAL_THREAD.BUF_MUTEX);

      for (int row = 0; row < TERMINAL::ROWS; row++)
      {
        // 1. Construct the string directly from the char array slice.
        // This is much faster as it avoids the concatenation loop.
        std::string line(sdSysData.TERMINAL_THREAD.SCREEN[row], TERMINAL::COLS);

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

    ImGui::EndChild();
  }

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