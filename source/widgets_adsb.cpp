// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_adsb.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_ADSB_CPP
#define WIDGETS_ADSB_CPP

#include "widgets_adsb.h"

// ---------------------------------------------------------------------------------------

/*
void BLANK::display(const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {

  }
  ImGui::End();
}
*/

// ---------------------------------------------------------------------------------------

void ADSB_SCREEN::display(unsigned long tmeFrame_Time, CONSOLE_COMMUNICATION &Screen_Comms, 
                          COLOR_COMBOS &Color_Select, 
                          const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    {
      ImGui::BeginGroup();
      if (button_simple_toggle_color("ADSB", "ADSB", ADSB_ACTIVE, Color_Select.COLOR_COMB_WHITE, Color_Select.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON))
      {
        if (ADSB_ACTIVE == true)
        {
          Screen_Comms.command_text_set(" adsboff");
        }
        else
        {
          Screen_Comms.command_text_set(" adsbon");
        }
      }

      if (button_simple_enabled("ADSB\nSnapshot", ADSB_ACTIVE, DEFAULTS.SIZE_BUTTON))
      {
        Screen_Comms.command_text_set(" adsbsnap");
      }
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    
    ImGui::Text("DATA: %d", 10);

  }
  ImGui::End();
}


void ADSB_SCREEN::set_adsb_active(bool Active)
{
  ADSB_ACTIVE = Active;
}

// ---------------------------------------------------------------------------------------
#endif