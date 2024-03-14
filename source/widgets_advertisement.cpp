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
    if (STOP_PAUSE_PLAY > -1)
    {
      if (STOP_PAUSE_PLAY == 1)
      {
        if (PING_NEXT_AD.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
        {
          ADVET_POS++;
          PING_NEXT_AD.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 45000);
          CHANGED = true;
        }

        if (ADVET_POS < 0 || ADVET_POS >= (int)AD_IMAGE_LIST.size())
        {
          ADVET_POS = 0;
          CHANGED = true;
        }

        if (CHANGED)
        {
          if (!ADVERT_IMAGE.create(AD_IMAGE_LIST[ADVET_POS], ImVec2(1000.0f, 540.0f)))
          {
            ACTIVE = false;
          }
        }
      }

      ImGui::SetNextWindowSize(ADVERT_IMAGE.get_should_be_window_size());

      ImGui::Begin(AD_IMAGE_LIST[ADVET_POS].c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
      {
        ImVec4 working_area = get_working_area();
        
        ADVERT_IMAGE.draw();
        
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