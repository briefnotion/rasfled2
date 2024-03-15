// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_advertisement.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef WIDGETS_ADVERTISEMENT_CPP
#define WIDGETS_ADVERTISEMENT_CPP

#include "widgets_advertisement.h"

using namespace std;

// -------------------------------------------------------------------------------------

void ADVERTISEMENT_SCREENS::stop()
{
  STOP_PAUSE_PLAY = -1;
}

void ADVERTISEMENT_SCREENS::pause()
{
  STOP_PAUSE_PLAY = 0;
}

void ADVERTISEMENT_SCREENS::play(system_data &sdSysData)
{
  PING_NEXT_AD.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 45000);
  STOP_PAUSE_PLAY = 1;
}

void ADVERTISEMENT_SCREENS::create(system_data &sdSysData)
{
  if (!ACTIVE)
  {
    if (get_files_list(sdSysData.DIRECTORY_ADVERTISEMENTS, AD_IMAGE_LIST, ".png"))
    {
      printf("%d\n", AD_IMAGE_LIST.size());

      for (int pos = 0; pos < (int)AD_IMAGE_LIST.size(); pos++)
      {
        TEXTURE_IMAGE tmp_image;
        
        printf("%s\n", AD_IMAGE_LIST[pos].c_str());

        if (tmp_image.create(AD_IMAGE_LIST[pos], ImVec2(1000.0f, 540.0f)))
        {
          ADVERT_IMAGES.push_back(tmp_image);
        }
      }

      PING_NEXT_AD.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 45000);
      ACTIVE = true;
    }
    else
    {
      ACTIVE = false;
    }
  }
}

void ADVERTISEMENT_SCREENS::draw(system_data &sdSysData)
{
  if (ACTIVE)
  {
    if ((STOP_PAUSE_PLAY > -1) && (ADVERT_IMAGES.size() > 0))
    {
      if (STOP_PAUSE_PLAY == 1)
      {
        if (PING_NEXT_AD.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
        {
          ADVERT_POS++;
          PING_NEXT_AD.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 45000);
          CHANGED = true;
        }

        if (ADVERT_POS < 0 || ADVERT_POS >= (int)ADVERT_IMAGES.size())
        {
          ADVERT_POS = 0;
          CHANGED = true;
        }
      }

      ImGui::SetNextWindowSize(ADVERT_IMAGES[ADVERT_POS].get_should_be_window_size());

      ImGui::Begin(ADVERT_IMAGES[ADVERT_POS].FILENAME.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
      {
        ImVec4 working_area = get_working_area();
        
        ADVERT_IMAGES[ADVERT_POS].draw();

        // SCAN
        {
          SCAN_LINE++;
          if (SCAN_LINE > ADVERT_IMAGES[ADVERT_POS].size().y)
          {
            SCAN_LINE = 0;
          }

          ImDrawList* draw_list_advertisement = ImGui::GetWindowDrawList();

          ImVec2 line_start = ImVec2(working_area.x, working_area.y + (float)SCAN_LINE);
          ImVec2 line_end = ImVec2(working_area.x + float((sdSysData.PROGRAM_TIME.current_frame_time() % 10) * 100), working_area.y + (float)SCAN_LINE);

          draw_list_advertisement->AddLine(line_start, line_end, sdSysData.COLOR_SELECT.c_black().STANDARD, 1.0f);
        }
        
        if (button_area(working_area))
        {
          STOP_PAUSE_PLAY = -1;
        }

        ImGui::End();
      }

      CHANGED = false;
    }
  }
}


// -------------------------------------------------------------------------------------

#endif