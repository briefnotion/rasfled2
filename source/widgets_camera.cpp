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

void CAMERA_WIDGET::display_camera_settings_window(system_data &sdSysData)
{
  ImVec4 working_area_col = get_working_area();

  float column_count = 2.0f;

  // Divide sub window into 3
  ImGui::BeginChild("Col1", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Connect to camera
    if (BC_CONNECT.button_color(sdSysData, "CONNECT##Connect to camera", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.create();
    }

    ImGui:: SameLine();
    // Disconnect from camera
    if (BC_DISCONNECT.button_color(sdSysData, "CLOSE##Disconnect from camera", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.close_camera();
    }

    // Temporary for path calibration
    if (DISPLAY_PATH_SETTINGS)
    {
      ImGui::Text("Path Points:");
      ImGui::InputFloat("Y0", &Y0);
      ImGui::InputFloat("XL0", &XL0);
      ImGui::InputFloat("XR0", &XR0);

      ImGui::InputFloat("Y1", &Y1);
      ImGui::InputFloat("XL1", &XL1);
      ImGui::InputFloat("XR1", &XR1);
      
      ImGui::InputFloat("Y2", &Y2);
      ImGui::InputFloat("XL2", &XL2);
      ImGui::InputFloat("XR2", &XR2);
      
      ImGui::InputFloat("Y3", &Y3);
      ImGui::InputFloat("XL3", &XL3);
      ImGui::InputFloat("XR3", &XR3);
    }

  }
  ImGui::EndChild();

  ImGui:: SameLine();
  ImGui::BeginChild("Col 2", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Display_Path
    if (BC_DISPLAY_PATH.button_color(sdSysData, "PATH##Display_Path", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      DISPLAY_PATH = !DISPLAY_PATH;
    }

    ImGui:: SameLine();
    // Display_Path_Settings
    if (BC_DISCONNECT.button_color(sdSysData, "PATH S##Display_Path_Settings", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      DISPLAY_PATH_SETTINGS = !DISPLAY_PATH_SETTINGS;
    }

    // Auto Focus
    ImGui::Text("Auto Focus:");
    ImGui::Text("%d", sdSysData.CAMERAS.PROPS.CTRL_FOCUS_AUTO.SET_VALUE);

    if (BC_AUTO_FOCUS_MINUS.button_color(sdSysData, "-##Auto_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_FOCUS_AUTO, 
                                            sdSysData.CAMERAS.PROPS.CTRL_FOCUS_AUTO.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_AUTO_FOCUS_PLUS.button_color(sdSysData, "+##Auto_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_FOCUS_AUTO, 
                                            sdSysData.CAMERAS.PROPS.CTRL_FOCUS_AUTO.SET_VALUE + 1);
    }

    // Focus Absolute
    ImGui::Text("Absl Focus:");
    ImGui::Text("%d", sdSysData.CAMERAS.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE);

    if (BC_ABSL_FOCUS_MINUS.button_color(sdSysData, "-##Absl_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_FOCUS_ABSOLUTE, 
                                            sdSysData.CAMERAS.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_ABSL_FOCUS_PLUS.button_color(sdSysData, "+##Absl_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_FOCUS_ABSOLUTE, 
                                            sdSysData.CAMERAS.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE + 1);
    }

    /*  Not able to set until researched.
    // Auto Exposure (menu)
    ImGui::Text("Auto Exp:");
    ImGui::Text("%d", sdSysData.CAMERAS.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE);

    if (BC_AUTO_EXPOSURE_PLUS.button_color(sdSysData, "-##Auto_Exposure", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_EXPOSURE_AUTO, 
                                            sdSysData.CAMERAS.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE - 1);
    }
    */

    ImGui::SameLine();
    if (BC_AUTO_EXPOSURE_MINUS.button_color(sdSysData, "+##Auto_Exposure", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_EXPOSURE_AUTO, 
                                            sdSysData.CAMERAS.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE + 1);
    }

    // Backlight Compensation
    ImGui::Text("Backlight Comp:");
    ImGui::Text("%d", sdSysData.CAMERAS.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE);

    if (BC_BACKLIGHT_COMP_PLUS.button_color(sdSysData, "-##Backlight_Comp", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_BACKLIGHT_COMPENSATION, 
                                            sdSysData.CAMERAS.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_BACKLIGHT_COMP_MINUS.button_color(sdSysData, "+##Backlight_Comp", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERAS.set_camera_control(sdSysData.CAMERAS.PROPS.CTRL_BACKLIGHT_COMPENSATION, 
                                            sdSysData.CAMERAS.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE + 1);
    }
  }

  ImGui::EndChild();  

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 3", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //}
  //ImGui::EndChild();

}

void CAMERA_WIDGET::display(system_data &sdSysData)
{
  // Check if the camera and video are available.
  if (sdSysData.CAMERAS.CAM_VIDEO_AVAILABLE)
  {
    // Get the current texture ID from the camera system.
    GLuint textureID = sdSysData.CAMERAS.TEXTURE_ID;
    
    // Get a thread-safe copy of the frame to get its dimensions.
    cv::Mat currentFrame = sdSysData.CAMERAS.get_current_frame();

    // Check if the texture ID and frame are valid before displaying.
    if (textureID != 0 && !currentFrame.empty()) 
    {

      // Variables for image size and centering, declared here to be accessible in both scopes.
      ImVec2 final_size = {0, 0};
      float padding_x = 0.0f;
      float padding_y = 0.0f;


      // Draw video frame
      {
        // 1. Get the available space in the current window.
        ImVec2 availRegion = ImGui::GetContentRegionAvail();

        float frame_aspect = (float)currentFrame.cols / (float)currentFrame.rows;
        float region_aspect = availRegion.x / availRegion.y;

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
        padding_x = (availRegion.x - final_size.x) * 0.5f;
        padding_y = (availRegion.y - final_size.y) * 0.5f;

        // Use Dummy to create the centering space.
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding_x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding_y);
        
        // 3. Display the image with the calculated size.
        ImGui::Image((ImTextureID)textureID, final_size);
      }

      // Draw Path Lines
      if (DISPLAY_PATH)
      {
          // Get the current window's draw list for overlay graphics.
          ImDrawList* draw_list = ImGui::GetWindowDrawList();
          
          // Calculate the top-left corner of the displayed image in screen coordinates.
          ImVec2 window_pos = ImGui::GetWindowPos();
          
          ImVec2 image_start_pos(
              window_pos.x + padding_x,
              window_pos.y + padding_y
          );

          float width = final_size.x;
          float height = final_size.y;
          float thickness = 10.0f; // Line thickness

          // Colors (Fully opaque)
          const ImU32 color_green = IM_COL32(0, 255, 0, 96);   // Bottom
          const ImU32 color_yellow = IM_COL32(255, 255, 0, 96); // Middle
          const ImU32 color_red = IM_COL32(255, 0, 0, 96);     // Top

          // Helper Lambda to convert normalized (X, Y) to absolute screen coordinates (ImVec2)
          auto NormToScreen = [&](float nx, float ny) -> ImVec2 {
              return ImVec2(
                  image_start_pos.x + nx * width,
                  image_start_pos.y + ny * height
              );
          };

          // Convert all 4 levels of points to screen coordinates
          ImVec2 P0L = NormToScreen(XL0, Y0); // Closest Left
          ImVec2 P0R = NormToScreen(XR0, Y0); // Closest Right
          ImVec2 P1L = NormToScreen(XL1, Y1); // Boundary 1 Left
          ImVec2 P1R = NormToScreen(XR1, Y1); // Boundary 1 Right
          ImVec2 P2L = NormToScreen(XL2, Y2); // Boundary 2 Left
          ImVec2 P2R = NormToScreen(XR2, Y2); // Boundary 2 Right
          ImVec2 P3L = NormToScreen(XL3, Y3); // Farthest Left
          ImVec2 P3R = NormToScreen(XR3, Y3); // Farthest Right


          // --- Draw Segment 1: Green Path (Y0 to Y1) ---
          // Draw Left, Right, and Top (Y1)
          draw_list->AddLine(P0L, P1L, color_green, thickness); // Left Line
          draw_list->AddLine(P0R, P1R, color_green, thickness); // Right Line
          draw_list->AddLine(P1L, P1R, color_green, thickness); // Top Line (Green boundary)


          // --- Draw Segment 2: Yellow Path (Y1 to Y2) ---
          // Draw Left, Right, and Top (Y2). The bottom is already connected by P1L/P1R.
          draw_list->AddLine(P1L, P2L, color_yellow, thickness); // Left Line
          draw_list->AddLine(P1R, P2R, color_yellow, thickness); // Right Line
          draw_list->AddLine(P2L, P2R, color_yellow, thickness); // Top Line (Yellow boundary)

          
          // --- Draw Segment 3: Red Path (Y2 to Y3) ---
          // Draw Left, Right, and Top (Y3). The bottom is already connected by P2L/P2R.
          draw_list->AddLine(P2L, P3L, color_red, thickness); // Left Line
          draw_list->AddLine(P2R, P3R, color_red, thickness); // Right Line
          draw_list->AddLine(P3L, P3R, color_red, thickness); // Top Line (Farthest boundary)
      }
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

  // Show Display Camera Setting Button
  ImGui::SetCursorPos(ImVec2(0,0));
  if (BC_DISPLAY_SETTINGS.button_color(sdSysData, "...##Display Camera Settings", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    DISPLAY_CAMERA_SETTINGS = !DISPLAY_CAMERA_SETTINGS;
  }

  if (sdSysData.CAMERAS.CAM_AVAILABLE == false)
  {
    ImGui::Text("Camera offline");
  }

  // Draw Camera Controls
  {
    if (DISPLAY_CAMERA_SETTINGS)
    {
      ImGui::SetNextWindowSize(ImVec2(290.0f, 475.0f));
      if (ImGui::Begin("CAMERA SETTINGS", nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        display_camera_settings_window(sdSysData);
      }
      ImGui::End();
    }
  }

}

// ---------------------------------------------------------------------------------------

#endif