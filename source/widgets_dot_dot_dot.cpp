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
void draw_led(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, 
              int Intensity, int low_level_indicator)
{
  if (Intensity > 0)
  {
    float size = (float)(Intensity / 10);

    if (size < 1.0f)
    {
      size = 1.0f;
    }

    Draw_List->AddNgonFilled(Screen_Position, size, (ImU32)Color, 4.0f);

    // Low level 
    // 0 - off
    // 1 - top
    // 2 - right
    // 3 - bot
    // 4 - left

    if (Intensity <= 10 && low_level_indicator > 0)
    {
      if (low_level_indicator == 1)
      {
        Draw_List->AddNgonFilled(ImVec2(Screen_Position.x, Screen_Position.y - 4.0f), 1.0f, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 3.0f);
      }
      if (low_level_indicator == 2)
      {
        Draw_List->AddNgonFilled(ImVec2(Screen_Position.x + 4.0f, Screen_Position.y), 1.0f, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 3.0f);
      }
      if (low_level_indicator == 3)
      {
        Draw_List->AddNgonFilled(ImVec2(Screen_Position.x, Screen_Position.y + 4.0f), 1.0f, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 3.0f);
      }
      if (low_level_indicator == 4)
      {
        Draw_List->AddNgonFilled(ImVec2(Screen_Position.x - 4.0f, Screen_Position.y), 1.0f, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 3.0f);
      }
    }
  }
}

// -------------------------------------------------------------------------------------

int DOT_DOT_DOT_SCREEN::intensity(CRGB Color)
{
  int largest_value = Color.r;

  if (Color.g > largest_value)
  {
    largest_value = Color.g;
  }

  if (Color.b > largest_value)
  {
    largest_value = Color.b;
  }

  return largest_value;
}

ImColor DOT_DOT_DOT_SCREEN::color_multiplier(CRGB Color, int Intensity)
{
  if (Intensity == 0)
  {
    return ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  }
  else
  {
    return ImColor((float)Color.r / (float)Intensity, 
                    (float)Color.g / (float)Intensity, 
                    (float)Color.b / (float)Intensity, 
                    1.0f);
  }
}

void DOT_DOT_DOT_SCREEN::display(system_data &sdSysData, ANIMATION_HANDLER &Animations)
{
  ImGui::BeginChild("Dot Dot Dot", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    ImVec4 working_area = get_working_area();
    ImDrawList* draw_list_screen = ImGui::GetWindowDrawList();

    int pixel_step = 4 * DEF_SCREEN_SIZE_X_MULTIPLIER;

    int door_size = sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(0).vLED_STRIPS.at(0).led_count();
    int over_size = sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(1).vLED_STRIPS.at(0).led_count();

    ImVec2 start_pos = ImVec2(20 * DEF_SCREEN_SIZE_X_MULTIPLIER, 20 * DEF_SCREEN_SIZE_Y_MULTIPLIER);
    start_pos = start_pos + ImVec2(working_area.x,  working_area.y);

    // ---

    int left      = start_pos.x + (door_size * pixel_step);
    int left_left = left - (door_size * pixel_step) - (pixel_step * 2);

    int top       = start_pos.y;
    int botom     = start_pos.y + (over_size * pixel_step * 2);

    int right       = left + (20  * DEF_SCREEN_SIZE_X_MULTIPLIER);
    int right_right = right + (door_size * pixel_step);

    // LED Simulator
    {
      ImVec2 draw_position;

      // ---

      for(int group=0; group < sdSysData.CONFIG.LED_MAIN.at(0).g_size(); group++)
      {

        for(int strip=0; strip < sdSysData.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
        {

          int intense = 0;
          ImColor color;
          
          // 0 0  - LB Door
          if (group  == 0 && strip == 0)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(left_left + (pos * pixel_step), 
                                      botom);

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 3);
            }
          }

          // 0 1  - LB Overhead
          if (group  == 0 && strip == 1)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(left, 
                                      botom - (pos * pixel_step ));

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 2);
            }
          }

          // 1 0  - LF Overhead
          if (group  == 1 && strip == 0)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(left, 
                                      top + (over_size * pixel_step) - (pos * pixel_step));

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 2);
            }
          }

          // 1 1  - LF Door
          if (group  == 1 && strip == 1)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(left_left + (pos * pixel_step), 
                                      top);

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 1);
            }
          }

          // 2 0  - RB Door
          if (group  == 2 && strip == 0)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(right_right - (pos * pixel_step), 
                                      botom);

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 3);
            }
          }

          // 2 1  - RB Overhead
          if (group  == 2 && strip == 1)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(right, 
                                      botom - (pos * pixel_step ));

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 4);
            }
          }

          // 3 0  - LF Overhead
          if (group  == 3 && strip == 0)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(right, 
                                      top + (over_size * pixel_step) - (pos * pixel_step));

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 4);
            }
          }

          // 3 1  - LF Door
          if (group  == 3 && strip == 1)
          {
            for (int pos = 0; pos < sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); pos++)
            {
              draw_position = ImVec2(right_right - (pos * pixel_step), 
                                      top);

              intense = intensity(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos]);
              color = color_multiplier(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[pos], intense);

              draw_led(draw_list_screen, draw_position, color, intense, 1);
            }
          }
        }
      }
    }
    
    // Communications Button Controls
    {
      ImGui::SetNextWindowPos(ImVec2(left_left, top + 20.0f));
      
      ImGui::BeginChild("Communications", ImVec2((door_size * pixel_step) - (20.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), (2.0f * (over_size * pixel_step) - (40.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER))), true, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
      // Comms
        {
          if (START_AUTO.button_color(sdSysData, "Start\nAuto", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" startauto");
          }
          ImGui::SameLine();
          if (STOP_AUTO.button_color(sdSysData, "Stop\nAuto", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" stopauto");
          }

          // ---

          if (START_ADSB.button_color(sdSysData, "Start\nADS-B", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" startadsb");
          }
          ImGui::SameLine();
          if (STOP_ADSB.button_color(sdSysData, "Stop\nADS-B", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" stopadsb");
          }
          
          // ---

          if (START_GPS.button_color(sdSysData, "Start\nGPS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" startgps");
          }
          ImGui::SameLine();
          if (STOP_GPS.button_color(sdSysData, "Stop\nGPS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" stopgps");
          }
          
          // ---
          
          if (START_COMPASS.button_color(sdSysData, "Start\nCompass", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" startcomp");
          }
          ImGui::SameLine();
          if (STOP_COMPASS.button_color(sdSysData, "Stop\nCompass", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            sdSysData.SCREEN_COMMS.command_text_set(" stopcomp");
          }
        }

        ImGui::EndChild();

      }
    }
    
    // Event Controls
    {
      ImGui::SetNextWindowPos(ImVec2(right + 20.0f, top + 20.0f));
      
      ImGui::BeginChild("Events", ImVec2((door_size * pixel_step) - (20.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), (2.0f * (over_size * pixel_step) - (40.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER))), true, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
        int event_count_total = 0;

        for (int channel = 0; channel < sdSysData.CONFIG.iNUM_CHANNELS; channel++)
        {
          event_count_total =+ Animations.EVENTS[channel].teDATA.size();
        }

        ImGui::Text("Event Count: %d", event_count_total);

        // ---

        if (CLEAR_EVENTS.button_color(sdSysData, "CLEAR\nEVENTS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          for (int channel = 0; channel < sdSysData.CONFIG.iNUM_CHANNELS; channel++)
          {
            for (int event = 0; event < (int)Animations.EVENTS[channel].teDATA.size(); event++)
            {
              Animations.EvClear(channel, event);
            }
          }
        }

        //ImGui::SameLine();

        if (AUTO_TO_CONSOLE.button_color(sdSysData, "AUTO\nCONS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" autop");
        }
        ImGui::SameLine();
        if (GPS_TO_CONSOLE.button_color(sdSysData, "GPS\nCONS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" gpsp");
        }

        /*
        ImGui::SameLine();

        if (button_color(sdSysData, "ANIMB", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" stopauto");
        }
        */

        // ---

        ImGui::NewLine();
        ImGui::Text("Screen Brightness:");

        if (BRIGHTNESS_100.button_color(sdSysData, "BRT\n100", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" brt100");
        }
        ImGui::SameLine();
        if (BRIGHTNESS_50.button_color(sdSysData, "BRT\n50", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" brt50");
        }
        ImGui::SameLine();
        if (BRIGHTNESS_25.button_color(sdSysData, "BRT\n25", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          sdSysData.SCREEN_COMMS.command_text_set(" brt25");
        }
        ImGui::EndChild();
      }

      // Debug area
      {
        ImGui::SameLine();
        ImGui::BeginChild("Debug", ImVec2(ImGui::GetContentRegionAvail().x, (2.0f * (over_size * pixel_step) - (40.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER))), true, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          // Rasfled Stats
          ImGui::Text("         PRC RND");
          ImGui::Text("  Sleep: %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.PREVSLEEPTIME.get_data(), sdSysData.PROGRAM_TIME.PREVSLEEPTIME.start_to_start_time());
          ImGui::Text("Compute: %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.COMPUTETIME.get_data(), sdSysData.PROGRAM_TIME.COMPUTETIME.start_to_start_time());
          ImGui::Text("  Cycle: %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.CYCLETIME.get_data(), sdSysData.PROGRAM_TIME.CYCLETIME.start_to_start_time());
          ImGui::NewLine();
          ImGui::Text("   AUTO: %3.0lf %3.0lf ms", sdSysData.dblCOMMS_AUTO_TRANSFER_TIME.get_data(), sdSysData.dblCOMMS_AUTO_TRANSFER_TIME.start_to_start_time());
          ImGui::Text("    GPS: %3.0lf %3.0lf ms", sdSysData.dblCOMMS_GPS_TRANSFER_TIME.get_data(), sdSysData.dblCOMMS_GPS_TRANSFER_TIME.start_to_start_time());
          ImGui::Text("COMPASS: %3.0lf %3.0lf ms", sdSysData.dblCOMMS_COMPASS_DATA_READ_TIME.get_data(), sdSysData.dblCOMMS_COMPASS_DATA_READ_TIME.start_to_start_time());
          ImGui::Text(" Screen: %3.0lf %3.0lf ms", sdSysData.dblSCREEN_RENDER_TIME.get_data(), sdSysData.dblSCREEN_RENDER_TIME.start_to_start_time());
          ImGui::Text(" Render: %3.0lf %3.0lf ms", sdSysData.dblCOMMS_LED_RENDER_TIME.get_data(), sdSysData.dblCOMMS_LED_RENDER_TIME.start_to_start_time());

          /*
          ImGui::Text("NEO Changing:\n %d", 
          sdSysData.COLOR_SELECT.neo_color_is_changing());
          
          //ImGui::Text("Walker:\n %f", 
          //  walker_float(sdSysData.PROGRAM_TIME.current_frame_time(), 0.20f, 70.0f));

          ImGui::Text("NEO RGB:\n %d : %d : %d", 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).r, 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).g, 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).b);

          ImGui::Text("NEO RGB (brt 0.04):\n %d : %d : %d", 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).brightness(0.04f).r, 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).brightness(0.04f).g, 
            sdSysData.COLOR_SELECT.neo_color_SIMPLE_RGB(RAS_CUSTOM).brightness(0.04f).b);

          ImGui::Text("Void Color : Prev:\n %d : %d", 
            sdSysData.COLOR_SELECT.void_colr(), 
            sdSysData.COLOR_SELECT.void_colr_prev());

          ImGui::Text("Running Color : \n %d : %d", 
            sdSysData.RUNNING_COLOR, 
            sdSysData.RUNNING_COLOR_TEMPERATURE);

          ImGui::Text("CUSTOM_COLOR ():\n %d : %d : %d", 
            sdSysData.CUSTOM_COLOR.r, 
            sdSysData.CUSTOM_COLOR.g, 
            sdSysData.CUSTOM_COLOR.b);
          */

          ImGui::EndChild();
        }
      }
    }

    ImGui::EndChild();

  }
}


// ---------------------------------------------------------------------------------------
#endif