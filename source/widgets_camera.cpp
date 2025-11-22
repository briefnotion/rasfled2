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

void add_line_mod_v(ImDrawList *Draw_List, ImVec2 &Point1, ImVec2 &Point2, ImColor &Color, float Thickness)
{
  ImVec2 y_top_l = ImVec2(Point1.x - (Thickness / 2.0f), Point1.y);
  ImVec2 y_top_r = ImVec2(Point1.x + (Thickness / 2.0f), Point1.y);
  ImVec2 y_bot_r = ImVec2(Point2.x + (Thickness / 2.0f), Point2.y);
  ImVec2 y_bot_l = ImVec2(Point2.x - (Thickness / 2.0f), Point2.y);

  Draw_List->AddQuadFilled(y_top_l, y_top_r, y_bot_r, y_bot_l, Color);
}

void add_line_mod_h(ImDrawList *Draw_List, ImVec2 &Point1, ImVec2 &Point2, ImColor &Color, float Thickness)
{
  ImVec2 y_top_l = ImVec2(Point1.x + (Thickness / 2.0f), Point1.y - (Thickness / 2.0f));
  ImVec2 y_top_r = ImVec2(Point2.x - (Thickness / 2.0f), Point2.y - (Thickness / 2.0f));
  ImVec2 y_bot_r = ImVec2(Point2.x + (Thickness / 2.0f), Point2.y + (Thickness / 2.0f));
  ImVec2 y_bot_l = ImVec2(Point1.x - (Thickness / 2.0f), Point1.y + (Thickness / 2.0f));

  Draw_List->AddQuadFilled(y_top_l, y_top_r, y_bot_r, y_bot_l, Color);
}

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
    add_line_mod_v(draw_list, P0L, P1L, COLOR_BLK, THICKNESS_1_A + 6.0f); // Left Line
    add_line_mod_v(draw_list, P0R, P1R, COLOR_BLK, THICKNESS_1_A + 6.0f); // Right Line
    add_line_mod_h(draw_list, P1L, P1R, COLOR_BLK, THICKNESS_1_B + 6.0f); // Top Line (Green boundary)

    // --- Draw Segment 2: Yellow Path (Y1 to Y2) ---
    // Draw Left, Right, and Top (Y2). The bottom is already connected by P1L/P1R.
    add_line_mod_v(draw_list, P1L, P2L, COLOR_BLK, THICKNESS_2_A + 6.0f); // Left Line
    add_line_mod_v(draw_list, P1R, P2R, COLOR_BLK, THICKNESS_2_A + 6.0f); // Right Line
    add_line_mod_h(draw_list, P2L, P2R, COLOR_BLK, THICKNESS_2_B + 6.0f); // Top Line (Yellow boundary)

    // --- Draw Segment 3: Red Path (Y2 to Y3) ---
    // Draw Left, Right, and Top (Y3). The bottom is already connected by P2L/P2R.
    add_line_mod_v(draw_list, P2L, P3L, COLOR_BLK, THICKNESS_3_A + 6.0f); // Left Line
    add_line_mod_v(draw_list, P2R, P3R, COLOR_BLK, THICKNESS_3_A + 6.0f); // Right Line
    add_line_mod_h(draw_list, P3L, P3R, COLOR_BLK, THICKNESS_3_B + 6.0f); // Top Line (Farthest boundary)
  }
  
  // Color Lines
  {
    ImColor COLOR_GRN = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_GREEN);   // 5ft
    ImColor COLOR_YLW = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_YELLOW);  // 10ft 
    ImColor COLOR_RED = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED);     // 15ft

    COLOR_GRN.Value.w = 0.25f;
    COLOR_YLW.Value.w = 0.25f;
    COLOR_RED.Value.w = 0.25f;

    // --- Draw Segment 1: Green Path (Y0 to Y1) ---
    // Draw Left, Right, and Top (Y1)
    add_line_mod_v(draw_list, P0L, P1L, COLOR_GRN, THICKNESS_1_A); // Left Line
    add_line_mod_v(draw_list, P0R, P1R, COLOR_GRN, THICKNESS_1_A); // Right Line
    add_line_mod_h(draw_list, P1L, P1R, COLOR_GRN, THICKNESS_1_B); // Top Line (Green boundary)

    // --- Draw Segment 2: Yellow Path (Y1 to Y2) ---
    // Draw Left, Right, and Top (Y2). The bottom is already connected by P1L/P1R.
    
    add_line_mod_v(draw_list, P1L, P2L, COLOR_YLW, THICKNESS_2_A); // Left Line
    add_line_mod_v(draw_list, P1R, P2R, COLOR_YLW, THICKNESS_2_A); // Right Line
    add_line_mod_h(draw_list, P2L, P2R, COLOR_YLW, THICKNESS_2_B); // Top Line (Yellow boundary)
    
    // --- Draw Segment 3: Red Path (Y2 to Y3) ---
    // Draw Left, Right, and Top (Y3). The bottom is already connected by P2L/P2R.
    add_line_mod_v(draw_list, P2L, P3L, COLOR_RED, THICKNESS_3_A); // Left Line
    add_line_mod_v(draw_list, P2R, P3R, COLOR_RED, THICKNESS_3_A); // Right Line
    add_line_mod_h(draw_list, P3L, P3R, COLOR_RED, THICKNESS_3_B); // Top Line (Farthest boundary)
  }
}

void CAMERA_WIDGET::display_camera_settings_window(system_data &sdSysData)
{
  //ImVec4 working_area_col = get_working_area();

  // Divide sub window into 3
  ImGui::BeginChild("Controls", ImVec2(sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    // Connect to camera
    if (BC_CONNECT.button_color(sdSysData, "CONNECT##Connect to camera", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.camera_start();
    }

    //ImGui:: SameLine();
    // Disconnect from camera
    if (BC_DISCONNECT.button_color(sdSysData, "CLOSE##Disconnect from camera", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.camera_stop();
    }

    //ImGui:: SameLine();
    // Disconnect from camera
    if (BC_DEFAULT.button_color(sdSysData, "DEFAULTS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.APPLY_DEFAULTS = true;
    }

    //ImGui:: SameLine();
    // Disconnect from camera
    /*
    if (BC_APPLY.button_color(sdSysData, "APPLY", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.APPLY_CHANGES = true;
    }
    */
  }
  ImGui::EndChild();

  ImGui:: SameLine();

  ImGui::BeginChild("Settings", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    float y_height = ImGui::GetTextLineHeightWithSpacing() + 4.0f;
    float x_start_for_name = 385.0f;

    float y_pos = ImGui::GetCursorPosY();
    for (size_t pos = 0; pos < sdSysData.CAMERA_BACKUP.SETTINGS.size(); pos++)
    {
      if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 6)
      {
        // Skip class type
      }
      else
      {
        ImGui::SetCursorPosY(y_pos);
        //ImGui::Text("0x%08X ", sdSysData.CAMERA_BACKUP.SETTINGS[pos].ADDRESS);
        //ImGui::SameLine();

        ImGui::Text("dft(%4d) min(%4d) max(%4d)", 
                      sdSysData.CAMERA_BACKUP.SETTINGS[pos].DEFAULT, 
                      sdSysData.CAMERA_BACKUP.SETTINGS[pos].MINIMUM, 
                      sdSysData.CAMERA_BACKUP.SETTINGS[pos].MAXIMUM
                    );

        ImGui::SameLine();

        if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 0)
        {
          ImGui::Text("%d", sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE);
          ImGui::SameLine();
          ImGui::Text(" --- ");
          ImGui::SameLine();
        }
        else if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 1)
        {
          // type int
          ImGui::PushItemWidth(100);
          std::string label = "##";
          label +=  sdSysData.CAMERA_BACKUP.SETTINGS[pos].NAME;
          if (ImGui::InputInt(label.c_str(), &sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE, sdSysData.CAMERA_BACKUP.SETTINGS[pos].STEP))
          {
            sdSysData.CAMERA_BACKUP.APPLY_CHANGES = true;
          }
          ImGui::PopItemWidth();
          ImGui::SameLine();
        }
        else if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 2)
        {
          // type bool
          bool checked = sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE;
          std::string label = "##";
          label +=  sdSysData.CAMERA_BACKUP.SETTINGS[pos].NAME;
          if (ImGui::Checkbox(label.c_str(), &checked))
          {
            sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE = checked;
            sdSysData.CAMERA_BACKUP.APPLY_CHANGES = true;
          }
          ImGui::SameLine();
        }
        else if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 3)
        {
          // type menu
          ImGui::PushItemWidth(100);
          std::string label = "##";
          label +=  sdSysData.CAMERA_BACKUP.SETTINGS[pos].NAME;
          if (ImGui::InputInt(label.c_str(), &sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE))
          {
            sdSysData.CAMERA_BACKUP.APPLY_CHANGES = true;
          }
          ImGui::PopItemWidth();
          ImGui::SameLine();
        }
        else if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 6)
        {
          // type class
          ImGui::Text("%d", sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE);
          ImGui::SameLine();
          ImGui::Text("clas ");
          ImGui::SameLine();
        }
        else //if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 4)
        {
          // type not coded
          ImGui::Text("%d", sdSysData.CAMERA_BACKUP.SETTINGS[pos].SET_VALUE);
          ImGui::SameLine();
          ImGui::Text("unkn ");
          ImGui::SameLine();
        }

        ImGui::SetCursorPosX(x_start_for_name);
        ImGui::Text("%s ", sdSysData.CAMERA_BACKUP.SETTINGS[pos].NAME.c_str());

        // more lines
        if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 3)
        {
          for (size_t menu_item = 0; menu_item < sdSysData.CAMERA_BACKUP.SETTINGS[pos].MENU_LIST.size(); menu_item++)
          {
            y_pos += y_height;
            ImGui::SetCursorPosY(y_pos);
            ImGui::SetCursorPosX(x_start_for_name + 10.0f);
            ImGui::Text("%2d %s", sdSysData.CAMERA_BACKUP.SETTINGS[pos].MENU_LIST[menu_item].ID,
                                  sdSysData.CAMERA_BACKUP.SETTINGS[pos].MENU_LIST[menu_item].NAME.c_str());
          }
        }

        y_pos += y_height;
      }
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
      // Adjust the Low Light Threshold and Gama.
      ImGui::InputInt("LL_THR", &sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT_THRESHOLD);
      ImGui::InputDouble("LL_GAM", &sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT_GAMMA);

      // Adjust the multiplier for steering wheel.
      ImGui::InputFloat("AMLT", &sdSysData.CAMERA_BACKUP.PROPS.ANGLE_MULTIPLIER);

      // Adjust the positions of the paths.
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

    // Display Fake Frame
    if (BC_ENH_FAKE_FRAME.button_toggle_color(sdSysData, "FAKE\nFRAME", "FAKE\nFRAME", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_FAKE_FRAMES, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_FAKE_FRAMES = !sdSysData.CAMERA_BACKUP.PROPS.ENH_FAKE_FRAMES;
    }

    // Display Fake Frame
    if (BC_ENH_DOUBLE_MASK.button_toggle_color(sdSysData, "DOUBLE\nMASKS", "DOUBLE\nMASKS", 
                                              sdSysData.CAMERA_BACKUP.PROPS.ENH_DOUBLE_MASK, 
                                              RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.PROPS.ENH_DOUBLE_MASK = !sdSysData.CAMERA_BACKUP.PROPS.ENH_DOUBLE_MASK;
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

void CAMERA_WIDGET::display_camera_stats_enhancements(system_data &sdSysData)
{
  if (sdSysData.CAMERA_BACKUP.camera_online() == false)
  {
    ImGui::Text("Camera Offline");
  }
  else 
  {
    std::string frame_gen = "  ";
    std::string frame_gen_on = "  ";
    std::string enh_low_light = "  ";
    std::string enh_low_light_on = "  ";
    std::string enh_color = "   ";
    std::string enh_glare_mask = "   ";
    std::string enh_canny_mask = "   ";
    std::string enh_double_mask = "   ";
    

    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_FAKE_FRAMES)
    {
      frame_gen = "FG";
    }
    
    if (sdSysData.CAMERA_BACKUP.FRAME_GEN)
    {
      frame_gen_on = " *";
    }

    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_LOW_LIGHT)
    {
      enh_low_light = "LL";
    }

    if (sdSysData.CAMERA_BACKUP.IS_LOW_LIGHT)
    {
      enh_low_light_on = " *";
    }

    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_COLOR)
    {
      enh_color = "COL";
    }

    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_GLARE_MASK)
    {
      enh_glare_mask = "GLM";
    }

    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_CANNY_MASK)
    {
      enh_canny_mask = "CAM";
    }
    if (sdSysData.CAMERA_BACKUP.PROPS.ENH_DOUBLE_MASK)
    {
      enh_double_mask = "DFM";
    }

    
    ImGui::Text("%s %s %s %s %s%s(%2d) %s%s(%3d)", 
                  enh_color.c_str(), enh_glare_mask.c_str(), enh_canny_mask.c_str(), enh_double_mask.c_str(),
                  frame_gen_on.c_str(), frame_gen.c_str(), (int)sdSysData.CAMERA_BACKUP.TIME_ACTUAL_FPS, 
                  enh_low_light_on.c_str(), enh_low_light.c_str(), sdSysData.CAMERA_BACKUP.LOW_LIGHT_VALUE);
  }

}

void CAMERA_WIDGET::display_camera_stats_times(system_data &sdSysData)
{
  if (sdSysData.CAMERA_BACKUP.camera_online())
  {
    ImGui::Text("Rend: %5.1ffps (%5.1fms)  Camera: %5.1ffps (%5.1fms)  Grab: (%5.1fms)  Proc Tme: (%5.1fms)", 
                  sdSysData.CAMERA_BACKUP.TIME_ACTUAL_FPS, sdSysData.CAMERA_BACKUP.TIME_ACTUAL_FRAME_TIME, 
                  sdSysData.CAMERA_BACKUP.TIME_CAMERA_FPS, sdSysData.CAMERA_BACKUP.TIME_CAMERA_FRAME_TIME,
                  sdSysData.CAMERA_BACKUP.TIME_FRAME_RETRIEVAL,
                  sdSysData.CAMERA_BACKUP.TIME_FRAME_PROCESSING);
  }
}

void CAMERA_WIDGET::display(system_data &sdSysData, float Angle)
{
  WORKING_AREA = get_working_area();
  // likely working size without auto is x923 y472

  // Check if the camera and video are available.
  if (sdSysData.CAMERA_BACKUP.camera_online())
  {
    ANGLE = Angle;
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
      /*
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
      */
    }
  }
  else
  {
    ImGui::Text("Waiting for camera feed...");
  }

  ImGui::SetCursorPos(ImVec2(0,0));
  if (SHOW_BUTTONS == false)
  {
    if (ImGui::InvisibleButton("Hide Buttons", ImVec2(WORKING_AREA.z, WORKING_AREA.w)))
    {
      SHOW_BUTTONS = true;
      SHOW_BUTTONS_TIMER.set(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
    }
  }
  else
  {
    if (SHOW_BUTTONS_TIMER.is_ready(sdSysData.PROGRAM_TIME.current_frame_time()) && 
        DISPLAY_ADJUSTMENTS == false && 
        DISPLAY_CAMERA_SETTINGS == false)
    {
      SHOW_BUTTONS = false;
    }
  }

  // Display stats and times
  {
    ImGui::SetCursorPos(ImVec2(4,4));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    display_camera_stats_enhancements(sdSysData);
    if (SHOW_BUTTONS)
    display_camera_stats_times(sdSysData);
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    display_camera_stats_enhancements(sdSysData);
    if (SHOW_BUTTONS)
    display_camera_stats_times(sdSysData);
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(2,2));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
    display_camera_stats_enhancements(sdSysData);
    if (SHOW_BUTTONS)
    display_camera_stats_times(sdSysData);
    ImGui::PopStyleColor();
  }
    

  if (SHOW_BUTTONS)
  {
    // Take a photo
    if (BC_SNAPSHOT.button_color(sdSysData, "SNAP\nSHOT", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.CAMERA_BACKUP.take_snapshot();
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
        ImGuiStyle& style = ImGui::GetStyle();
        float titleHeight = ImGui::GetFontSize() + style.FramePadding.y * 2;
        float y_height = ImGui::GetTextLineHeightWithSpacing() + 4.0f;

        int lines = sdSysData.CAMERA_BACKUP.SETTINGS.size();
        // Count Lines
        for (size_t pos = 0; pos < sdSysData.CAMERA_BACKUP.SETTINGS.size(); pos++)
        {
          if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 3)
          {
            lines += sdSysData.CAMERA_BACKUP.SETTINGS[pos].MENU_LIST.size();
          }
          else if (sdSysData.CAMERA_BACKUP.SETTINGS[pos].VAR_TYPE == 6)
          {
            lines -= 1;
          }
        }

        ImVec2 window_size = ImVec2(800.0f, ImGui::GetFontSize() + titleHeight +
                                            (lines * y_height));
        ImGui::SetNextWindowSize(window_size);
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
}

// ---------------------------------------------------------------------------------------

#endif