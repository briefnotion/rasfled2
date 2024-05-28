// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_dot_dot_dot.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_DOT_DOT_DOT_CPP
#define WIDGETS_DOT_DOT_DOT_CPP

#include "widgets_dot_dot_dot.h"

// ---------------------------------------------------------------------------------------

//void draw_marker_led(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
void draw_led(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, ImColor Color)
{
  if (sdSysData.PROGRAM_TIME.current_frame_time() > 0)
  {
    //does nothing.
  }

  Draw_List->AddNgonFilled(Screen_Position, 2.0f, (ImU32)Color, 6.0f);
}

// -------------------------------------------------------------------------------------

ImColor DOT_DOT_DOT_SCREEN::color_multiplier(CRGB Color)
{
  int max_color = Color.r + Color.g + Color.b;

  ImColor ret_color;

  if (max_color < 10)
  {
    float multiplier = (256.0f / 20.0f);

    ret_color = ImColor((float)Color.r / multiplier, 
                        (float)Color.g / multiplier, 
                        (float)Color.b / multiplier, 
                        1.0f);
  }
  else if (max_color < 100)
  {
    float multiplier = (256.0f / 5.0f);

    ret_color = ImColor((float)Color.r / multiplier, 
                        (float)Color.g / multiplier, 
                        (float)Color.b / multiplier, 
                        1.0f);
  }
  else if (max_color < 100)
  {
    float multiplier = (256.0f);

    ret_color = ImColor((float)Color.r / multiplier, 
                        (float)Color.g / multiplier, 
                        (float)Color.b / multiplier, 
                        1.0f);
  }

  return ret_color;

}

void DOT_DOT_DOT_SCREEN::display(system_data &sdSysData)
//void DOT_DOT_DOT_SCREEN::display()
{
  ImVec4 working_area = get_working_area();
  ImDrawList* draw_list_screen = ImGui::GetWindowDrawList();

  //ImColor led_color = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
  ImVec2 draw_position;

  for(int group=0; group < sdSysData.CONFIG.LED_MAIN.at(0).g_size(); group++)
  {
    for(int strip=0; strip < sdSysData.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
    {
      for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
      {
        draw_position = ImVec2(working_area.x + 50.0f + (pos * 4), working_area.y + 50.0f + (float)(group * 30) + (float)(strip * 10));

        draw_led(draw_list_screen, sdSysData, draw_position, 
                  color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]));
      }
    }
  }
}


// ---------------------------------------------------------------------------------------
#endif