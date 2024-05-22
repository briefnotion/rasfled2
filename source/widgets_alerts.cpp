// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_alerts.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef WIDGETS_ALERTS_CPP
#define WIDGETS_ALERTS_CPP

#include "widgets_alerts.h"

using namespace std;

// -------------------------------------------------------------------------------------

void ALERT_WIDGET_WIDGTS::create(system_data &sdSysData, string Name)
{
  // Value Bar
  {
  VALUES.PROPS.LABEL = Name + " Value";
  VALUES.PROPS.BAR_HEIGHT = 20.0f;
  VALUES.PROPS.MARKER_SIZE = 15.0f;
  VALUES.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
  VALUES.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
  VALUES.PROPS.DRAW_MIN_MAX_ON_TOP = false;
  VALUES.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
  VALUES.PROPS.DRAW_MIN_MAX = true;
  VALUES.PROPS.MARKER_LIMIT_AT_RANGE = true;
  VALUES.PROPS.MIN = 0.0f;
  VALUES.PROPS.MAX = 1.0f;
  VALUES.PROPS.DRAW_RULER = true;
  VALUES.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
  VALUES.PROPS.MAX_TICK_LEVEL = 6;
  VALUES.PROPS.DRAW_FRAME_COUNT = 30;
  VALUES.create();
  }
}

void ALERT_WIDGET_PROPERTIES_LIST::check_properties_list(system_data &sdSysData, int Alert_Num, string Name)
{
  if (Alert_Num >= (int)LIST.size())
  {
    while (Alert_Num >= (int)LIST.size())
    {
      ALERT_WIDGET_WIDGTS tmp_properties;

      LIST.push_back(tmp_properties);

      LIST.back().create(sdSysData, Name + to_string(LIST.size() -1));
    }
  }
}

void ALERT_WIDGET::draw(system_data &sdSysData, ALERT_SYSTEM_2 &Alerts_List)
{
  // Go through and display all reserve
  for (int alert_num = 0; alert_num < RESERVE_ALERT_LIST_SIZE; alert_num++)
  {
      if (Alerts_List.res_display(alert_num))
      {
        // Create properties if needed
        PROPERTIES_RESERVE_LIST.check_properties_list(sdSysData, alert_num, "Reserve Alert ");

        ImGui::SetNextWindowSize(ImVec2(300, 100));

        if (Alerts_List.res_warning(alert_num))
        {
          ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
          ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
        }
        else if(Alerts_List.res_active(alert_num))
        {
          ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
        }
        
        if (ImGui::Begin(("ALERT " + to_string(alert_num)).c_str(), nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          ImVec2 screen_pos = ImGui::GetCursorScreenPos();
          ImGui::Text(Alerts_List.res_alert_text_line_1(alert_num).c_str());
          ImGui::Text(Alerts_List.res_alert_text_line_2(alert_num).c_str());

          if (Alerts_List.ALERTS_RESERVE[alert_num].show_value_bar())
          {
            ImDrawList* draw_list_alert = ImGui::GetWindowDrawList();

            if (Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE > Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE)
            {
              // Calculate Min Max values to display
              float min = Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE - 
                            (Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE - Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE);

              float max = Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE + 
                            (Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE - Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE);

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.PROPS.MIN = min;
              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.PROPS.MAX = max;

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.update_min_max_value(Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE, 
                                                                                  Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE);

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.update_value(sdSysData, Alerts_List.ALERTS_RESERVE[alert_num].VALUE);
            }
            else
            {
              // Calculate Min Max values to display
              float min = Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE - 
                (Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE - Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE);

              float max = Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE + 
                (Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE - Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE);

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.PROPS.MIN = min;
              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.PROPS.MAX = max;

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.update_min_max_value(Alerts_List.ALERTS_RESERVE[alert_num].ALERT_VALUE, 
                                                                                  Alerts_List.ALERTS_RESERVE[alert_num].CLEAR_VALUE);

              PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.update_value(sdSysData, Alerts_List.ALERTS_RESERVE[alert_num].VALUE);
            }
          
          PROPERTIES_RESERVE_LIST.LIST[alert_num].VALUES.draw(draw_list_alert, sdSysData);
        }

          ImGui::SetCursorScreenPos(screen_pos);
          if (ImGui::InvisibleButton(("Acknowlege Alert" + to_string(alert_num)).c_str(), ImGui::GetContentRegionAvail()))
          {
            Alerts_List.res_acknowlege(alert_num);
          }
        }
        ImGui::End();

        ImGui::PopStyleColor(3);
      }
    //}
  }

  // Go through and display all generic alerts
  if (Alerts_List.gen_size() > 0)
  {
    for (int alert_num = 0; alert_num < (int)Alerts_List.gen_size(); alert_num++)
    {
      ImGui::SetNextWindowSize(ImVec2(300, 100));
      
      if (Alerts_List.gen_display(alert_num))
      {
        // Create properties if needed
        //PROPERTIES_GENERAL_LIST.check_properties_list(sdSysData, alert_num, "General Alert ");

        if (Alerts_List.gen_warning(alert_num))
        {
          ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
          ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
        }
        
        if (ImGui::Begin(("ALERT " + to_string(Alerts_List.gen_alert_id(alert_num))).c_str(), nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          ImVec2 screen_pos = ImGui::GetCursorScreenPos();
          ImGui::Text(Alerts_List.gen_alert_text_line_1(alert_num).c_str());
          ImGui::Text(Alerts_List.res_alert_text_line_2(alert_num).c_str());

          ImGui::SetCursorScreenPos(screen_pos);
          if (ImGui::InvisibleButton(("Acknowlege Alert" + to_string(alert_num)).c_str(), ImGui::GetContentRegionAvail()))
          {
            Alerts_List.gen_acknowlege(alert_num);
          }
        }
        ImGui::End();

        ImGui::PopStyleColor(3);
      }
    }
  }
}

// -------------------------------------------------------------------------------------

#endif