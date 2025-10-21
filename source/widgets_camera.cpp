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

// Helper to convert normalized (X, Y) to absolute screen coordinates (ImVec2)
ImVec2 CAMERA_WIDGET::norm_to_screen( float &Nx, float &Ny, float &Slide, 
                                      ImVec2 &Image_Start_Position, 
                                      ImVec2 &Final_Size)
{
  float depth_factor = 1.0f - Ny;

  return ImVec2(
      (Image_Start_Position.x + Nx * Final_Size.x) + (Slide * Final_Size.y * depth_factor * depth_factor),
      Image_Start_Position.y + Ny * Final_Size.y
  );
}

void CAMERA_WIDGET::display_path(system_data &sdSysData)
{
  // Get the current window's draw list for overlay graphics.
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  ImVec2 image_start_pos(
      WORKING_AREA.x + PADDING.x,
      WORKING_AREA.y + PADDING.y
  );

  // Determine the cars steering roation skew
  float slide = ANGLE * sdSysData.CAMERA_BACKUP.PROPS.ANGLE_MULTIPLIER;

  // Convert all 4 levels of points to screen coordinates
  ImVec2 P0L = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XL0, sdSysData.CAMERA_BACKUP.PROPS.Y0, slide, image_start_pos, FINAL_SIZE); // Closest Left
  ImVec2 P0R = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XR0, sdSysData.CAMERA_BACKUP.PROPS.Y0, slide, image_start_pos, FINAL_SIZE); // Closest Right
  ImVec2 P1L = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XL1, sdSysData.CAMERA_BACKUP.PROPS.Y1, slide, image_start_pos, FINAL_SIZE); // Boundary 1 Left
  ImVec2 P1R = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XR1, sdSysData.CAMERA_BACKUP.PROPS.Y1, slide, image_start_pos, FINAL_SIZE); // Boundary 1 Right
  ImVec2 P2L = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XL2, sdSysData.CAMERA_BACKUP.PROPS.Y2, slide, image_start_pos, FINAL_SIZE); // Boundary 2 Left
  ImVec2 P2R = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XR2, sdSysData.CAMERA_BACKUP.PROPS.Y2, slide, image_start_pos, FINAL_SIZE); // Boundary 2 Right
  ImVec2 P3L = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XL3, sdSysData.CAMERA_BACKUP.PROPS.Y3, slide, image_start_pos, FINAL_SIZE); // Farthest Left
  ImVec2 P3R = norm_to_screen(sdSysData.CAMERA_BACKUP.PROPS.XR3, sdSysData.CAMERA_BACKUP.PROPS.Y3, slide, image_start_pos, FINAL_SIZE); // Farthest Right

  // Black Lines
  {
    // --- Draw Segment 1: Green Path (Y0 to Y1) ---
    // Draw Left, Right, and Top (Y1)
    draw_list->AddLine(P0L, P1L, COLOR_BLK, THICKNESS_1_A); // Left Line
    draw_list->AddLine(P0R, P1R, COLOR_BLK, THICKNESS_1_A); // Right Line
    draw_list->AddLine(P1L, P1R, COLOR_BLK, THICKNESS_1_A); // Top Line (Green boundary)

    // --- Draw Segment 2: Yellow Path (Y1 to Y2) ---
    // Draw Left, Right, and Top (Y2). The bottom is already connected by P1L/P1R.
    draw_list->AddLine(P1L, P2L, COLOR_BLK, THICKNESS_2_A); // Left Line
    draw_list->AddLine(P1R, P2R, COLOR_BLK, THICKNESS_2_A); // Right Line
    draw_list->AddLine(P2L, P2R, COLOR_BLK, THICKNESS_2_A); // Top Line (Yellow boundary)

    // --- Draw Segment 3: Red Path (Y2 to Y3) ---
    // Draw Left, Right, and Top (Y3). The bottom is already connected by P2L/P2R.
    draw_list->AddLine(P2L, P3L, COLOR_BLK, THICKNESS_3_A); // Left Line
    draw_list->AddLine(P2R, P3R, COLOR_BLK, THICKNESS_3_A); // Right Line
    draw_list->AddLine(P3L, P3R, COLOR_BLK, THICKNESS_3_A); // Top Line (Farthest boundary)
  }
  
  // Color Lines
  {
    ImU32 COLOR_GRN = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_GREEN);   // 5ft
    ImU32 COLOR_YLW = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_YELLOW);  // 10ft 
    ImU32 COLOR_RED = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED);     // 15ft

    // --- Draw Segment 1: Green Path (Y0 to Y1) ---
    // Draw Left, Right, and Top (Y1)
    draw_list->AddLine(P0L, P1L, COLOR_GRN, THICKNESS_1_B); // Left Line
    draw_list->AddLine(P0R, P1R, COLOR_GRN, THICKNESS_1_B); // Right Line
    draw_list->AddLine(P1L, P1R, COLOR_GRN, THICKNESS_1_B); // Top Line (Green boundary)

    // --- Draw Segment 2: Yellow Path (Y1 to Y2) ---
    // Draw Left, Right, and Top (Y2). The bottom is already connected by P1L/P1R.
    draw_list->AddLine(P1L, P2L, COLOR_YLW, THICKNESS_2_B); // Left Line
    draw_list->AddLine(P1R, P2R, COLOR_YLW, THICKNESS_2_B); // Right Line
    draw_list->AddLine(P2L, P2R, COLOR_YLW, THICKNESS_2_B); // Top Line (Yellow boundary)

    // --- Draw Segment 3: Red Path (Y2 to Y3) ---
    // Draw Left, Right, and Top (Y3). The bottom is already connected by P2L/P2R.
    draw_list->AddLine(P2L, P3L, COLOR_RED, THICKNESS_3_B); // Left Line
    draw_list->AddLine(P2R, P3R, COLOR_RED, THICKNESS_3_B); // Right Line
    draw_list->AddLine(P3L, P3R, COLOR_RED, THICKNESS_3_B); // Top Line (Farthest boundary)
  }
}

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
      sdSysData.CAMERA_BACKUP.camera_start();
    }

    ImGui:: SameLine();
    // Disconnect from camera
    if (BC_DISCONNECT.button_color(sdSysData, "CLOSE##Disconnect from camera", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.camera_stop();
    }
  }
  ImGui::EndChild();

  ImGui:: SameLine();
  ImGui::BeginChild("Col 2", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Auto Focus
    ImGui::Text("Auto Focus:");
    ImGui::Text("%d", sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_AUTO.SET_VALUE);

    if (BC_AUTO_FOCUS_MINUS.button_color(sdSysData, "-##Auto_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_AUTO, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_AUTO.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_AUTO_FOCUS_PLUS.button_color(sdSysData, "+##Auto_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_AUTO, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_AUTO.SET_VALUE + 1);
    }

    // Focus Absolute
    ImGui::Text("Absl Focus:");
    ImGui::Text("%d", sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE);

    if (BC_ABSL_FOCUS_MINUS.button_color(sdSysData, "-##Absl_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_ABSOLUTE, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_ABSL_FOCUS_PLUS.button_color(sdSysData, "+##Absl_Focus", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_ABSOLUTE, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_FOCUS_ABSOLUTE.SET_VALUE + 1);
    }

    /*  Not able to set until researched.
    // Auto Exposure (menu)
    ImGui::Text("Auto Exp:");
    ImGui::Text("%d", sdSysData.CAMERA_BACKUP.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE);

    if (BC_AUTO_EXPOSURE_PLUS.button_color(sdSysData, "-##Auto_Exposure", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_EXPOSURE_AUTO, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE - 1);
    }
    */

    ImGui::SameLine();
    if (BC_AUTO_EXPOSURE_MINUS.button_color(sdSysData, "+##Auto_Exposure", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_EXPOSURE_AUTO, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_EXPOSURE_AUTO.SET_VALUE + 1);
    }

    // Backlight Compensation
    ImGui::Text("Backlight Comp:");
    ImGui::Text("%d", sdSysData.CAMERA_BACKUP.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE);

    if (BC_BACKLIGHT_COMP_PLUS.button_color(sdSysData, "-##Backlight_Comp", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_BACKLIGHT_COMPENSATION, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE - 1);
    }

    ImGui::SameLine();
    if (BC_BACKLIGHT_COMP_MINUS.button_color(sdSysData, "+##Backlight_Comp", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.set_camera_control(sdSysData.CAMERA_BACKUP.PROPS.CTRL_BACKLIGHT_COMPENSATION, 
                                            sdSysData.CAMERA_BACKUP.PROPS.CTRL_BACKLIGHT_COMPENSATION.SET_VALUE + 1);
    }
  }

  ImGui::EndChild();  

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 3", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //}
  //ImGui::EndChild();

}

void CAMERA_WIDGET::display_camera_adjustments_window(system_data &sdSysData)
{
  ImVec4 working_area_col = get_working_area();

  float column_count = 1.0f;

  // Divide sub window into 3
  ImGui::BeginChild("Col1", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Temporary for path calibration
    {
      ImGui::InputFloat("AMLT", &sdSysData.CAMERA_BACKUP.PROPS.ANGLE_MULTIPLIER);

      //ImGui::Text("Path Points:");
      ImGui::InputFloat("Y0", &sdSysData.CAMERA_BACKUP.PROPS.Y0);
      ImGui::InputFloat("XL0", &sdSysData.CAMERA_BACKUP.PROPS.XL0);
      ImGui::InputFloat("XR0", &sdSysData.CAMERA_BACKUP.PROPS.XR0);

      ImGui::InputFloat("Y1", &sdSysData.CAMERA_BACKUP.PROPS.Y1);
      ImGui::InputFloat("XL1", &sdSysData.CAMERA_BACKUP.PROPS.XL1);
      ImGui::InputFloat("XR1", &sdSysData.CAMERA_BACKUP.PROPS.XR1);
      
      ImGui::InputFloat("Y2", &sdSysData.CAMERA_BACKUP.PROPS.Y2);
      ImGui::InputFloat("XL2", &sdSysData.CAMERA_BACKUP.PROPS.XL2);
      ImGui::InputFloat("XR2", &sdSysData.CAMERA_BACKUP.PROPS.XR2);
      
      ImGui::InputFloat("Y3", &sdSysData.CAMERA_BACKUP.PROPS.Y3);
      ImGui::InputFloat("XL3", &sdSysData.CAMERA_BACKUP.PROPS.XL3);
      ImGui::InputFloat("XR3", &sdSysData.CAMERA_BACKUP.PROPS.XR3);
    }

  }
  ImGui::EndChild();

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 2", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //}
  //ImGui::EndChild();  

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 3", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //}
  //ImGui::EndChild();

}

void CAMERA_WIDGET::display_camera_enhancements_window(system_data &sdSysData)
{
  ImVec4 working_area_col = get_working_area();

  float column_count = 1.0f;

  // Divide sub window into 3
  ImGui::BeginChild("Col1", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Display Path
    if (BC_SHOW_PATH.button_toggle_color(sdSysData, "PATH##Display_Path", "PATH##Display_Path", 
                                              sdSysData.CAMERA_BACKUP.PROPS.SHOW_PATH, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.SHOW_PATH = !sdSysData.CAMERA_BACKUP.PROPS.SHOW_PATH;
    }

    // Display Median Blur only when low light
    if (BC_ENH_LOW_LIGHT.button_toggle_color(sdSysData, "LOW\nLIGHT", "LOW\nLIGHT", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT = !sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT;
    }

    /*
    if (BC_ENH_OVERLAY_LINES.button_toggle_color(sdSysData, "LINES\nMASK", "LINES\nMASK", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_OVERLAY_LINES, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_OVERLAY_LINES = !sdSysData.CAMERA_BACKUP.PROPS.ENH_OVERLAY_LINES;
    }
    */

    ImGui::SameLine();
    if (BC_ENH_GLARE_MASK.button_toggle_color(sdSysData, "GLARE\nMASK", "GLARE\nMASK", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_GLARE_MASK, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_GLARE_MASK = !sdSysData.CAMERA_BACKUP.PROPS.ENH_GLARE_MASK;
    }

    ImGui::SameLine();
    if (BC_ENH_CANNY.button_toggle_color(sdSysData, "CANNY\nMASK", "CANNY\nMASK", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_CANNY_MASK, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_CANNY_MASK = !sdSysData.CAMERA_BACKUP.PROPS.ENH_CANNY_MASK;
    }

    if (BC_ENH_COLOR.button_toggle_color(sdSysData, "COLOR", "COLOR", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_COLOR, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_COLOR = !sdSysData.CAMERA_BACKUP.PROPS.ENH_COLOR;
    }
    
    ImGui::SameLine();
    if (BC_ENH_HOUGH.button_toggle_color(sdSysData, "HOUGH", "HOUGH", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_HOUGH, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_HOUGH = !sdSysData.CAMERA_BACKUP.PROPS.ENH_HOUGH;
    }

    // Display Car Detection
    if (BC_ENH_CAR_DETECTION.button_toggle_color(sdSysData, "CAR\nDET", "CAR\nDET", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_CAR_DETECTION, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_CAR_DETECTION = !sdSysData.CAMERA_BACKUP.PROPS.ENH_CAR_DETECTION;
    }
  }
  ImGui::EndChild();

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 2", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //
  //}
  //ImGui::EndChild();  

  //ImGui:: SameLine();
  //ImGui::BeginChild("Col 3", ImVec2((working_area_col.z) / column_count, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  //{
  //}
  //ImGui::EndChild();

}

void CAMERA_WIDGET::display(system_data &sdSysData, float Angle)
{
  // Check if the camera and video are available.
  if (sdSysData.CAMERA_BACKUP.camera_avalable())
  {
    ANGLE = Angle;

    WORKING_AREA = get_working_area();
    // X = x starting pos (position of left most window, if no write)
    // Y = y starting pos (position of top most window, if no write)
    // Z = x size
    // W = y size

    // Get the current texture ID from the camera system.
    GLuint textureID = sdSysData.CAMERA_BACKUP.TEXTURE_ID;
    
    // Get a thread-safe copy of the frame to get its dimensions.
    cv::Mat currentFrame = sdSysData.CAMERA_BACKUP.get_current_frame();

    // Check if the texture ID and frame are valid before displaying.
    if (textureID != 0 && !currentFrame.empty()) 
    {
      // Draw video frame
      {
        float frame_aspect = (float)currentFrame.cols / (float)currentFrame.rows;
        float region_aspect = WORKING_AREA.z / WORKING_AREA.w;

        if (region_aspect > frame_aspect)
        {
          // The available region is wider than the frame, so we scale by height.
          FINAL_SIZE.y = WORKING_AREA.w;
          FINAL_SIZE.x = WORKING_AREA.w * frame_aspect;
        }
        else
        {
          // The available region is taller or has the same aspect ratio, so we scale by width.
          FINAL_SIZE.x = WORKING_AREA.z;
          FINAL_SIZE.y = WORKING_AREA.z / frame_aspect;
        }
        
        // 2. Center the image horizontally.
        // Calculate the padding needed to center the image.
        PADDING.x = (WORKING_AREA.z - FINAL_SIZE.x) * 0.5f;
        PADDING.y = (WORKING_AREA.w - FINAL_SIZE.y) * 0.5f;

        // Use Dummy to create the centering space.
        ImGui::SetCursorPos(PADDING);
        
        // 3. Display the image with the calculated size.
        ImGui::Image((ImTextureID)textureID, FINAL_SIZE);
      }

      // Draw Path Lines
      if (sdSysData.CAMERA_BACKUP.PROPS.SHOW_PATH)
      {
        display_path(sdSysData);
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

  // Take a photo
  if (BC_SNAPSHOT.button_color(sdSysData, "SNAP\nSHOT", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    sdSysData.CAMERA_BACKUP.take_snapshot();
  }

  //ImGui::InvisibleButton("camera_no_show", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM);
  // Notification
  if (sdSysData.CAMERA_BACKUP.camera_avalable() == false)
  {
    ImGui::Text("Camera");
    ImGui::Text("  Offline");
  }
  //else
  {
    ImGui::Text("%2.1f fps", sdSysData.CAMERA_BACKUP.TIME_MAX_FPS);
    ImGui::Text("Grab:");
    ImGui::Text("  %2.1f ms", sdSysData.CAMERA_BACKUP.TIME_FRAME_RETRIEVAL);
    ImGui::Text("Prc Tme:");
    ImGui::Text("  %2.1f ms", sdSysData.CAMERA_BACKUP.TIME_FRAME_PROCESSING);
  }

  // Open Settings Screen
  if (BC_DISPLAY_SETTINGS.button_toggle_color(sdSysData, "CAM##Display Camera Settings", "CAM##Display Camera Settings", 
                                              DISPLAY_CAMERA_SETTINGS, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    DISPLAY_CAMERA_SETTINGS = !DISPLAY_CAMERA_SETTINGS;
  }

  // Open Adjustments Screen
  if (BC_DISPLAY_ADJUSTMENTS.button_toggle_color(sdSysData, "ADJ##Display Camera Adjustments", "ADJ##Display Camera Adjustments", 
                                              DISPLAY_ADJUSTMENTS, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    DISPLAY_ADJUSTMENTS = !DISPLAY_ADJUSTMENTS;
  }

  // Open Enhancements Screen
  if (BC_DISPLAY_ENHANCEMENTS.button_toggle_color(sdSysData, "ENH##Display Camera Enhancements", "ENH##Display Camera Enhancements",
                                              DISPLAY_ENHANCEMENTS, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    DISPLAY_ENHANCEMENTS = !DISPLAY_ENHANCEMENTS;
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

    if (DISPLAY_ADJUSTMENTS)
    {
      ImGui::SetNextWindowSize(ImVec2(290.0f, 475.0f));
      if (ImGui::Begin("CAMERA ADJUSTMENTS", nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        display_camera_adjustments_window(sdSysData);
      }
      ImGui::End();
    }
    
    if (DISPLAY_ENHANCEMENTS)
    {
      ImGui::SetNextWindowSize(ImVec2(290.0f, 475.0f));
      if (ImGui::Begin("CAMERA ENHANCEMENTS", nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        display_camera_enhancements_window(sdSysData);
      }
      ImGui::End();
    }
  }

}

// ---------------------------------------------------------------------------------------

#endif