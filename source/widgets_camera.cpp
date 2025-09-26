// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_camera.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_CAMERA_CPP
#define WIDGETS_CAMERA_CPP

#include "widgets_camera.h"

using namespace std;
// ---------------------------------------------------------------------------------------

void CAMERA_WIDGET::display(system_data &sdSysData)
{
  // Check if the camera and video are available.
  if (sdSysData.CAMERAS.CAM_VIDEO_AVAILABLE)
  {
    // Show some settings
    ImGui::Text("Auto Focus: %d", sdSysData.CAMERAS.CAM_FOCUS_AUTO);
    ImGui::Text("Absl Focus: %d", sdSysData.CAMERAS.CAM_FOCUS_ABSOLUTE);

    // Get the current texture ID from the camera system.
    GLuint textureID = sdSysData.CAMERAS.TEXTURE_ID;
    
    // Get a thread-safe copy of the frame to get its dimensions.
    cv::Mat currentFrame = sdSysData.CAMERAS.get_current_frame();

    // Check if the texture ID and frame are valid before displaying.
    if (textureID != 0 && !currentFrame.empty()) 
    {
      // 1. Get the available space in the current window.
      ImVec2 availRegion = ImGui::GetContentRegionAvail();

      float frame_aspect = (float)currentFrame.cols / (float)currentFrame.rows;
      float region_aspect = availRegion.x / availRegion.y;

      ImVec2 final_size;
      if (region_aspect > frame_aspect)
      {
        // The available region is wider than the frame, so we scale by height.
        final_size.y = availRegion.y;
        final_size.x = availRegion.y * frame_aspect;
      }
      else
      {
        // The available region is taller or has the same aspect ratio, so we scale by width.
        final_size.x = availRegion.x;
        final_size.y = availRegion.x / frame_aspect;
      }
      
      // 2. Center the image horizontally.
      // Calculate the padding needed to center the image.
      float padding_x = (availRegion.x - final_size.x) * 0.5f;
      float padding_y = (availRegion.y - final_size.y) * 0.5f;

      // Use Dummy to create the centering space.
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding_x);
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding_y);
      
      // 3. Display the image with the calculated size.
      ImGui::Image((ImTextureID)textureID, final_size);
    } 
    else 
    {
      // Display a more specific error based on the failure condition.
      if (textureID == 0) 
      {
        ImGui::Text("Error: Could not display camera feed. (Texture ID is 0)");
      } 
      else if (currentFrame.empty()) 
      {
        ImGui::Text("Error: Could not display camera feed. (Frame is empty)");
      } 
      else 
      {
        ImGui::Text("Error: Could not display camera feed.");
      }
    }
  }
  else
  {
    ImGui::Text("Waiting for camera feed...");
  }
}

// ---------------------------------------------------------------------------------------

#endif