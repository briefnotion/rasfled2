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

ImVec2 point_position(ImVec4 Working_Area, ImVec2 Position)
{
  ImVec2 ret_center;
  ret_center.x = Working_Area.x + Position.x;
  ret_center.y = Working_Area.y + Position.y;
  return ret_center;
}

// ---------------------------------------------------------------------------------------
// Markers

void draw_line(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size)
{
  // simple draw line
  Draw_List->AddLine(Screen_Position_1, Screen_Position_2, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(Color), Size);
}

void draw_box(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size)
{
  // simple draw line
  Draw_List->AddRect(Screen_Position_1, Screen_Position_2, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(Color), 0.0f, 0, Size);
}

void draw_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgonFilled(Screen_Position, 2.0f, (ImU32)sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(Color), 4.0f);
}

void draw_marker_filled(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgonFilled(Screen_Position, 4.0f, (ImU32)sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(Color), 4.0f);
}

void draw_airport_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgon(Screen_Position, 4.0f, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(Color), 4.0f, 1.5f);
}

void draw_point_marker(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, float Size)
{
  Draw_List->AddNgonFilled(Screen_Position, Size, Color, 4.0f);
}

void draw_track(ImDrawList *Draw_List, system_data &sdSysData, 
                ImVec4 &Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                float Initial_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK &Track)
{
  // Seperate simple and detailed tracks from each other
  //  There is no guarantee that they are connected.

  bool draw_0 = false;
  bool draw_1 = false;

  ImVec2 track_position_0;
  ImVec2 track_position_1;


  // LOD
  if (Draw_Level_Of_Detail < 1)
  {
    Draw_Level_Of_Detail = 1;
  }

  // Simple
  // Set First Point
  
  if (Track.TRACK_POINTS_SIMPLE.size() > 1)
  {
    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_SIMPLE[0].LATITUDE, 
                                                                  Track.TRACK_POINTS_SIMPLE[0].LONGITUDE), 
                                                      Map_Bearing, draw_1);

    // Siple step through
    for(int position = 1; position < (int)Track.TRACK_POINTS_SIMPLE.size(); position = position + Draw_Level_Of_Detail)
    {
      track_position_0 = track_position_1;
      draw_0 = draw_1;

      track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_SIMPLE[position].LATITUDE, 
                                                                  Track.TRACK_POINTS_SIMPLE[position].LONGITUDE), 
                                                      Map_Bearing, draw_1);
      track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_SIMPLE[position].LATITUDE, 
                                                                  Track.TRACK_POINTS_SIMPLE[position].LONGITUDE), 
                                                      Map_Bearing, draw_1);

      Draw_List->AddLine(track_position_0, track_position_1, 
                          sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_GREY), 1.0f);
    }
  }


  // Detailed
  // Set First Point
  if ((int)Track.TRACK_POINTS_DETAILED.size() > 1)
  {
    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_DETAILED[0].LATITUDE, 
                                                                  Track.TRACK_POINTS_DETAILED[0].LONGITUDE), 
                                                      Map_Bearing, draw_1);

    // Detailed step through
    for(int position = 1; position < (int)Track.TRACK_POINTS_DETAILED.size(); position++)
    {
      track_position_0 = track_position_1;
      draw_0 = draw_1;

      track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_DETAILED[position].LATITUDE, 
                                                                  Track.TRACK_POINTS_DETAILED[position].LONGITUDE), 
                                                      Map_Bearing, draw_1);

      if (draw_0 || draw_1)
      {
        ImColor point_color = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(Color_Scale.get_color(Track.TRACK_POINTS_DETAILED[position - 1].VALUE));
        point_color.Value.w = 0.75f;

        Draw_List->AddLine(track_position_0, track_position_1, point_color, 
                            (1.0f + (Initial_Point_Size * Track.TRACK_POINTS_DETAILED[position].ACCURACY)));

        Draw_List->AddLine( track_position_0, track_position_1, 
                            sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_GREY), 1.0f);
      }
    }
  }
}

void draw_track_2(ImDrawList *Draw_List, system_data &sdSysData, 
                ImVec4 &Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                float Initial_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK_ALTERNATIVE &Track)
{
  // Seperate simple and detailed tracks from each other
  //  There is no guarantee that they are connected.

  //bool draw_0 = false;
  ImVec2 track_position_0;
  bool   track_position_0_end = false;
  
  bool draw_1 = false;
  ImVec2 track_position_1;
  bool   track_position_1_end = false;

  // LOD
  if (Draw_Level_Of_Detail < 1)
  {
    Draw_Level_Of_Detail = 1;
  }

  // Detailed
  // Set First Point
  if ((int)Track.TRACK_POINTS_DETAILED.size() > 1)
  {
    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_DETAILED[0].LATITUDE, 
                                                                  Track.TRACK_POINTS_DETAILED[0].LONGITUDE), 
                                                      Map_Bearing, draw_1);
    track_position_1_end = Track.TRACK_POINTS_DETAILED[0].END_POINT;

    // Detailed step through
    for(size_t position = 1; position < Track.TRACK_POINTS_DETAILED.size(); position++)
    {
      track_position_0 = track_position_1;
      track_position_0_end = track_position_1_end;
      //draw_0 = draw_1;

      track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      DOUBLE_VEC2(Track.TRACK_POINTS_DETAILED[position].LATITUDE, 
                                                                  Track.TRACK_POINTS_DETAILED[position].LONGITUDE), 
                                                      Map_Bearing, draw_1);
      track_position_1_end = Track.TRACK_POINTS_DETAILED[position].END_POINT;

      //if ((draw_0 || draw_1) && track_position_0_end == false)
      if (track_position_0_end == false)  // Dont worry about off screen.  Track.TRACK_POINTS_DETAILED[position] will limit.
                                          //  draw_0 = draw_1; wasnt working right anyway because the values were carried over.
      {
        ImColor point_color = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(Color_Scale.get_color(Track.TRACK_POINTS_DETAILED[position - 1].VALUE));
        point_color.Value.w = 0.50f;

        Draw_List->AddLine(track_position_0, track_position_1, point_color, 
                            (1.0f + (Initial_Point_Size * Track.TRACK_POINTS_DETAILED[position].ACCURACY)));

        Draw_List->AddLine( track_position_0, track_position_1, 
                            sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_GREY), 1.0f);
      }
    }
  }
}


// Draw all aircraft onto the maps.
AIRCRAFT draw_aircraft_map_marker(AIRCRAFT_MAP_DETAILS Aircraft, ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                                            int Draw_Level_Of_Detail, DOUBLE_VEC2 Center_Lat_Lon, float Map_Bearing, NEW_COLOR_SCALE &Altitude_Color_Scale)
{
  AIRCRAFT ret_clicked_aircraft;

  if (Aircraft.AIRCRAFT_ITEM.POSITION.GLOBAL_POSITION_FOUND)
  {
    bool draw = false;
    ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon,
                                                  DOUBLE_VEC2( Aircraft.AIRCRAFT_ITEM.POSITION.LATITUDE.get_double_value(), 
                                                          Aircraft.AIRCRAFT_ITEM.POSITION.LONGITUDE.get_double_value()), 
                                                          Map_Bearing, draw);

    // Draw track first then overlay aircraft.
    if (Aircraft.TRACK.TRACK_POINTS_DETAILED.size() > 1)
    {
      draw_track( Draw_List, sdSysData, Working_Area, Scale, Draw_Level_Of_Detail, 6.0f, 
                  Altitude_Color_Scale, Center_Lat_Lon, Map_Bearing, Aircraft.TRACK);
    }

    // Text Describing Aircraft
   
    if (Aircraft.AIRCRAFT_ITEM.SEEN_POS.get_int_value() <= 5)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));
    }

    ImGui::SetCursorScreenPos(draw_position + Aircraft.AIRCRAFT_ITEM.META.POSITION_SQUAWK);
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_SQUAWK, 135.0f, BB_BL);   // Squawk

    ImGui::SetCursorScreenPos(draw_position + Aircraft.AIRCRAFT_ITEM.META.POSITION_FLIGHT);
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_FLIGHT, 225.0f, BB_BL);   // Flight
    
    ImGui::SetCursorScreenPos(draw_position + Aircraft.AIRCRAFT_ITEM.META.POSITION_SPEED);
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_SPEED, 135.0f, BB_BL);    // Speed
    
    ImGui::SetCursorScreenPos(draw_position + Aircraft.AIRCRAFT_ITEM.META.POSITION_ALTITUDE);
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_ALTITUDE, 225.0f, BB_BL); // Altitude
    
    if (Aircraft.AIRCRAFT_ITEM.DISTANCE_FROM_BASE >= 0.0f)
    {
      ImGui::SetCursorScreenPos(draw_position + Aircraft.AIRCRAFT_ITEM.META.POSITION_DISTANCE);
      Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_DISTANCE_FROM_BASE, 225.0f, BB_BL); // Distance 
    }

    ImGui::PopStyleColor();

    // Draw Aircraft Marker
    draw_compass(Draw_List, sdSysData, 1, draw_position, 15.0f, false, (Aircraft.AIRCRAFT_ITEM.SEEN_POS.get_int_value() <= 5), 
                        Aircraft.AIRCRAFT_ITEM.NAV_HEADING.conversion_success(), Aircraft.AIRCRAFT_ITEM.NAV_HEADING.get_float_value(), 
                        Aircraft.AIRCRAFT_ITEM.TRACK.conversion_success(), Aircraft.AIRCRAFT_ITEM.TRACK.get_float_value(), 
                        false, Map_Bearing, Aircraft.COLOR);

    ImGui::SetCursorScreenPos(ImVec2(draw_position.x - 20.0f, draw_position.y - 20.0f));
    if (ImGui::InvisibleButton(Aircraft.AIRCRAFT_ITEM.HEX.get_str_value().c_str(), ImVec2(40.0f, 40.0f)))
    {
      ret_clicked_aircraft = Aircraft.AIRCRAFT_ITEM;
    }
  }

  return ret_clicked_aircraft;
}

// ---------------------------------------------------------------------------------------

void ADSB_RANGE::calculate_lat_lon_to_point_scale(int &Map_Location_Focus)
{
  double latitude_diff = 0;
  double longitude_diff = 0;
 
  DOUBLE_VEC2 new_coords_east;
  DOUBLE_VEC2 new_coords_south;

  if (Map_Location_Focus == 1)
  {
    new_coords_east = get_coords_x_miles_from_coords(GPS_POS_LAT_LON.x, GPS_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(GPS_POS_LAT_LON.x, GPS_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 180.0f);

    latitude_diff = abs(new_coords_south.x - GPS_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - GPS_POS_LAT_LON.y);
  }
  else if (Map_Location_Focus == 2)
  {
    new_coords_east = get_coords_x_miles_from_coords(AIRCRAFT_POS_LAT_LON.x, AIRCRAFT_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(AIRCRAFT_POS_LAT_LON.x, AIRCRAFT_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 180.0f);

    latitude_diff = abs(new_coords_south.x - AIRCRAFT_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - AIRCRAFT_POS_LAT_LON.y);
  }
  else
  {
    new_coords_east = get_coords_x_miles_from_coords(NO_POS_LAT_LON.x, NO_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(NO_POS_LAT_LON.x, NO_POS_LAT_LON.y,
                                                          RANGE_IMP_LATEST, 180.0f);

    latitude_diff = abs(new_coords_south.x - NO_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - NO_POS_LAT_LON.y);
  }

  LAT_LON_TO_POINT_SCALE.x =(RADIUS_CIRCLE_POINT_SIZE / latitude_diff);
  LAT_LON_TO_POINT_SCALE.y = (RADIUS_CIRCLE_POINT_SIZE / longitude_diff);
}

void ADSB_RANGE::set_zoom_level(int Zoom_Level, int &Map_Location_Focus)
{
  switch (Zoom_Level)
  {
    case 0:
    {
      set_range(0.01f, Map_Location_Focus);
      break;
    }

    case 1:
    {
      set_range(0.03f, Map_Location_Focus);
      break;
    }

    case 2:
    {
      set_range(0.05f, Map_Location_Focus);
      break;
    }
    
    case 3:
    {
      set_range(0.1f, Map_Location_Focus);
      break;
    }
    
    case 4:
    {
      set_range(0.25f, Map_Location_Focus);
      break;
    }
    
    case 5:
    {
      set_range(0.5f, Map_Location_Focus);
      break;
    }

    case 6:
    {
      set_range(1.0f, Map_Location_Focus);
      break;
    }

    case 7:
    {
      set_range(2.0f, Map_Location_Focus);
      break;
    }

    case 8:
    {
      set_range(5.0f, Map_Location_Focus);
      break;
    }
    
    case 9:
    {
      set_range(7.0f, Map_Location_Focus);
      break;
    }
    
    case 10:
    {
      set_range(10.0f, Map_Location_Focus);
      break;
    }
    
    case 11:
    {
      set_range(15.0f, Map_Location_Focus);
      break;
    }
    
    case 12:
    {
      set_range(25.0f, Map_Location_Focus);
      break;
    }
    
    case 13:
    {
      set_range(35.0f, Map_Location_Focus);
      break;
    }
    
    case 14:
    {
      set_range(50.0f, Map_Location_Focus);
      break;
    }
    
    case 15:
    {
      set_range(75.0f, Map_Location_Focus);
      break;
    }
    
    case 16:
    {
      set_range(100.0f, Map_Location_Focus);
      break;
    }
  }
}

ImVec2 ADSB_RANGE::ll_2_pt_scale()
{
  return LAT_LON_TO_POINT_SCALE;
}

void ADSB_RANGE::gps_display_current_location_toggle(int &Map_Location_Focus)
{
  if (Map_Location_Focus == 1)
  {
    Map_Location_Focus = -1;
  }
  else if (Map_Location_Focus == -1)
  {
    Map_Location_Focus = 1;
  }
  else if (Map_Location_Focus == 2)
  {
    Map_Location_Focus = -2;
  }
  else if (Map_Location_Focus == -2)
  {
    Map_Location_Focus = 2;
  }
}

bool ADSB_RANGE::gps_display_current_location(int &Map_Location_Focus)
{
  // return true if it is on.
  return Map_Location_Focus > 0;
}

void ADSB_RANGE::set_no_pos_lat_lon(DOUBLE_VEC2 Lat_Lon)
{
  NO_POS_LAT_LON = Lat_Lon;
}

void ADSB_RANGE::set_gps_pos_lat_lon(DOUBLE_VEC2 Lat_Lon, int &Map_Location_Focus)
{
  GPS_POS_LAT_LON = Lat_Lon;

  if (Map_Location_Focus == 1)
  {
    NO_POS_LAT_LON = Lat_Lon;
  }
}

void ADSB_RANGE::set_aircraft_pos_lat_lon(DOUBLE_VEC2 Lat_Lon, int &Map_Location_Focus)
{
  AIRCRAFT_POS_LAT_LON = Lat_Lon;

  if (Map_Location_Focus == 2)
  {
    NO_POS_LAT_LON = Lat_Lon;
  }
}

DOUBLE_VEC2 ADSB_RANGE::get_center_lat_lon(int &Map_Location_Focus)
{
  if (Map_Location_Focus == 1)
  {
    return GPS_POS_LAT_LON;
  }
  else if (Map_Location_Focus == 2)
  {
    return AIRCRAFT_POS_LAT_LON;
  }
  else 
  {
    return NO_POS_LAT_LON;
  }
}

DOUBLE_VEC2 ADSB_RANGE::get_gps_lat_lon()
{
  return GPS_POS_LAT_LON;
}

float ADSB_RANGE::range()
{
  return RANGE_IMP_LATEST;
}

void ADSB_RANGE::create()
{
  PROPS.COLOR = RAS_ORANGE;
  //int tmp_map_loc_focus = 0;
  //set_range(25.0f, tmp_map_loc_focus);
  RANGE_IMP.set_size(180);
  RANGE_IMP.set_alive_time(6000);
}

/// Updates the current range block size based on the latest range impact value.
/// The range block size is adjusted in discrete steps to provide a smooth visual representation of the range.
/// The range block size is used to determine the spacing and size of the range markers on the display.
void ADSB_RANGE::update_range_block_size()
{
  if (RANGE_IMP_LATEST <= 0.01f + ((0.03f - 0.01f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.01f;
  }
  else if (RANGE_IMP_LATEST <= 0.03f + ((0.05f - 0.03f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.03f;
  }
  else if (RANGE_IMP_LATEST <= 0.05f + ((0.1f - 0.05f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.05f;
  }
  else if (RANGE_IMP_LATEST <= 0.1f + ((0.25f -  0.1f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.1f;
  }
  else if (RANGE_IMP_LATEST <= 0.25f + ((0.5f - 0.25f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.25f;
  }
  else if (RANGE_IMP_LATEST <= 0.5f + ((1.0f - 0.5f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 0.5f;
  }
  else if (RANGE_IMP_LATEST <= 1.0f + ((2.0f - 1.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 1.0f;
  }
  else if (RANGE_IMP_LATEST <= 2.0f + ((5.0f - 2.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 2.0f;
  }
  else if (RANGE_IMP_LATEST <= 5.0f + ((7.0f - 5.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 5.0f;
  }
  else if (RANGE_IMP_LATEST <= 7.0f + ((10.0f - 7.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 7.0f;
  }
  else if (RANGE_IMP_LATEST <= 10.0f + ((15.0f - 10.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 10.0f;
  }
  else if (RANGE_IMP_LATEST <= 15.0f + ((25.0f - 15.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 15.0f;
  }
  else if (RANGE_IMP_LATEST <= 25.0f + ((35.0f - 25.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 25.0f;
  }
  else if (RANGE_IMP_LATEST <= 35.0f + ((50.0f - 35.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 35.0f;
  }
  else if (RANGE_IMP_LATEST <= 50.0f + ((75.0f - 50.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 50.0f;
  }
  else if (RANGE_IMP_LATEST <= 75.0f + ((100.0f - 75.0f) / 2.0f))
  {
    RANGE_BLOCK_CURRENT = 75.0f;
  }
  else //if (RANGE_IMP_LATEST < 100.0f)
  {
    RANGE_BLOCK_CURRENT = 100.0f;
  }
}

void ADSB_RANGE::range_update(unsigned long Frame_Time, int &Map_Location_Focus)
{
  RANGE_IMP.set_value(Frame_Time, RANGE);

  if (RANGE_IMP_LATEST != RANGE)
  {
    RANGE_IMP_LATEST = RANGE_IMP.impact(Frame_Time);
    calculate_lat_lon_to_point_scale(Map_Location_Focus);
    update_range_block_size();

    bool draw = true;
    ImVec2 point_distance = point_position_lat_lon(WORKING_AREA, LAT_LON_TO_POINT_SCALE, GPS_POS_LAT_LON, 
                                                  get_coords_x_miles_from_coords(GPS_POS_LAT_LON.x, GPS_POS_LAT_LON.y, RANGE_BLOCK_CURRENT, 0.0f), 
                                                  180.0f, draw);
    RANGE_POINT_DISTANCE = abs(point_distance.y - CENTER.y);

    //calculate_lat_lon_to_point_scale();
  }
}

void ADSB_RANGE::set_range(float Range_Miles, int &Map_Location_Focus)
{
  RANGE = Range_Miles;
  calculate_lat_lon_to_point_scale(Map_Location_Focus);
}

void ADSB_RANGE::zoom_in(system_data &sdSysData)
{ 
  if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL > 0)
  {
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL -= 1;
    set_zoom_level(sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
  }
}

void ADSB_RANGE::zoom_out(system_data &sdSysData)
{
  if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL < 16)
  {
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL += 1;
    set_zoom_level(sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
  }
}

void ADSB_RANGE::zoom_return(int Zoom_Level,  int &Map_Location_Focus)
{
  set_zoom_level(Zoom_Level, Map_Location_Focus);
}

double ADSB_RANGE::resolution()
{
  return (double)RANGE_IMP_LATEST / (double)RADIUS_CIRCLE_POINT_SIZE;
}

void ADSB_RANGE::draw_scale(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area)
{
  WORKING_AREA = Working_Area;

  // initial start  (CRITICAL.  PROGRAM WILL HARD LOCK IF NOT RAN)
  if (ZOOM_LEVEL_UNSET == true)
  {
    ZOOM_LEVEL_UNSET = false;
    
    RADIUS_CIRCLE_POINT_SIZE = WORKING_AREA.w / 2.0f * 0.6f;

    if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL == -1)
    {
      // Default start zoom level if not defined.
      sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL = 1;
    }

    set_zoom_level(sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
  }

  CENTER = point_position_center(WORKING_AREA);
      
  Draw_List->AddNgon(CENTER, RANGE_POINT_DISTANCE, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR)), 32, 1.5f);
  Draw_List->AddNgon(CENTER, RANGE_POINT_DISTANCE * 2.0f, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR)), 32, 1.5f);

  ImGui::SetCursorScreenPos(ImVec2(CENTER.x, CENTER.y - RANGE_POINT_DISTANCE + 5));
  
  // Text Range
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR)));

  if (RANGE_BLOCK_CURRENT < 1.0f)
  {
    ImGui::Text("%.2f mi", RANGE_BLOCK_CURRENT);
  }
  else
  {
    ImGui::Text("%.0f mi", RANGE_BLOCK_CURRENT);
  }
  ImGui::PopStyleColor();
}

void ADSB_RANGE::draw_info(int &Map_Location_Focus)
{
  ImGui::Text("LAT: %f", get_center_lat_lon(Map_Location_Focus).x);
  ImGui::Text("LON: %f", get_center_lat_lon(Map_Location_Focus).y);
  ImGui::Text("RNG: %.2f mi", RANGE_IMP_LATEST);

  // test

  /*
  ImGui::Text("RADIUS_CIRCLE_POINT_SIZE: %f", RADIUS_CIRCLE_POINT_SIZE);
  ImGui::Text("Pixel Size: %f", RANGE_IMP_LATEST / RADIUS_CIRCLE_POINT_SIZE);

  for (int degrees = 0; degrees < 360; degrees = degrees + 15)
  {  
    ImVec2 x_miles_west;
    x_miles_west = get_coords_x_miles_from_coords(CURRENT_POSITION.LATITUDE.get_float_value(),
                                                CURRENT_POSITION.LONGITUDE.get_float_value(),
                                                RANGE, degrees);
    //x_miles_west.x = CURRENT_POSITION.LATITUDE.get_float_value();
    //x_miles_west.y = CURRENT_POSITION.LONGITUDE.get_float_value();

    ImGui::Text("---");

    ImGui::Text("lat: %f", x_miles_west.x);
    ImGui::Text("lon: %f", x_miles_west.y);

    float distance = calculate_distance(CURRENT_POSITION.LATITUDE.get_float_value(),
                                        CURRENT_POSITION.LONGITUDE.get_float_value(),
                                        x_miles_west.x, x_miles_west.y);
    ImGui::Text("dis: %f", distance);
    
    ImGui::Text("---");

    ImGui::Text("lat: %f", abs(x_miles_west.x - CURRENT_POSITION.LATITUDE.get_float_value()));
    ImGui::Text("lon: %f", abs(x_miles_west.y - CURRENT_POSITION.LONGITUDE.get_float_value()));
    ImGui::Text("rad: %f", RADIUS_CIRCLE_POINT_SIZE);
    ImGui::Text("scl:  %f %f", LAT_LON_TO_POINT_SCALE.x, LAT_LON_TO_POINT_SCALE.y);
    
    ImGui::Text("---");

    ImVec2 scr_pos = point_position_lat_lon(PREV_WORKING_AREA, LAT_LON_TO_POINT_SCALE, 
                                            CURRENT_POSITION.LATITUDE.get_float_value(), CURRENT_POSITION.LONGITUDE.get_float_value(),
                                            x_miles_west.x, x_miles_west.y);

    ImGui::Text("pnt: %f %f", scr_pos.x, scr_pos.y);
    draw_marker(sdSysData, point_position(PREV_WORKING_AREA, scr_pos));
  }
  */
}

// -------------------------------------------------------------------------------------

void ADSB_MAP::draw_landmark( ImDrawList *Draw_List, system_data &sdSysData, MAP_INFO &Landmark, 
                              ImVec4 &Working_Area, ImVec2 Scale, 
                              DOUBLE_VEC2 Center_Lat_Lon, float &Map_Bearing, float Range)
{
  bool draw = false;
  ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.LAT_LON, Map_Bearing, draw);

  if (draw || Landmark.TYPE == 3)
  {
    // Possibly set colors here
    // ---

    // Draw Symbol
    switch (Landmark.TYPE)
    {
      case 0: //  0 - Generic
      {
        draw_marker(Draw_List, sdSysData, draw_position, RAS_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_YELLOW)));
        ImGui::Text("%s", Landmark.DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }
      
      case 1: //  1 - Airport
      {
        if (Landmark.AIRPORT_LANDING_VECTORS.size() > 1)
        {
          bool on_screen = false;
          
          for(int vector = 0; vector < (int)Landmark.AIRPORT_LANDING_VECTORS.size(); vector++)
          {
            // 5 miles out.
            ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      get_coords_x_miles_from_coords(Landmark.LAT_LON.x, Landmark.LAT_LON.y, 5.0f, Landmark.AIRPORT_LANDING_VECTORS[vector]), 
                                                      Map_Bearing, on_screen);

            Draw_List->AddLine(draw_position, landing_vector_end, 
                                sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE), 2);
          }
        }
        
        draw_airport_marker(Draw_List, sdSysData, draw_position, RAS_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));
        ImGui::Text("%s", Landmark.DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }

      case 2: // 2 - Region
      {
        ImColor Color = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD_V(RAS_CYAN);
        if (Landmark.REGION_GPS_COORDS.size() > 1.0f)
        {
          //bool on_screen = false;
          
          ImVec2 landing_vector_start = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.REGION_GPS_COORDS[Landmark.REGION_GPS_COORDS.size() -1.0f], Map_Bearing, draw);
          ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.REGION_GPS_COORDS[0], Map_Bearing, draw);
          Draw_List->AddLine(landing_vector_start, landing_vector_end, Color, 2.0f);

          for(int pos = 1; pos < (int)Landmark.REGION_GPS_COORDS.size(); pos++)
          {
            landing_vector_start = landing_vector_end;
            landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.REGION_GPS_COORDS[pos], Map_Bearing, draw);

            Draw_List->AddLine(landing_vector_start, landing_vector_end, 
                                Color, 2.0f);
          }
        }
        
        //draw_airport_marker(sdSysData, draw_position, sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_COMB_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        if (Range < 35.0f)
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(Color));
          ImGui::Text("%s", Landmark.DISPLAY_NAME.c_str());
          ImGui::PopStyleColor();
        }
        break;
      }

      case 3: // 2 - Interstate
      {
        ImColor Color = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_CYAN);
        if (Landmark.REGION_GPS_COORDS.size() > 1)
        {
          bool on_screen_1 = false;
          bool on_screen_2 = false;
  
          ImVec2 landing_vector_start; 
          ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.REGION_GPS_COORDS[0], Map_Bearing, on_screen_2);

          for(int pos = 1; pos < (int)Landmark.REGION_GPS_COORDS.size(); pos++)
          {
            landing_vector_start = landing_vector_end;
            on_screen_1 = on_screen_2;

            landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, Landmark.REGION_GPS_COORDS[pos], Map_Bearing, on_screen_2);

            if (on_screen_1 == true || on_screen_2 == true)
            {
              Draw_List->AddLine(landing_vector_start, landing_vector_end, 
                                  Color, 2.0f);
            }
          }
        }

        break; 
      }
    }
  }
}

void ADSB_MAP::screen_buttons(system_data &sdSysData)
{
  // Range and Location Buttons

  // Zoom In
  ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (1.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                    WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));

  if (BC_MINUS.button_color(sdSysData, "-", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(1, 0);
    RANGE_INDICATOR.zoom_in(sdSysData);
  }

  // Zoom Out
  ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                    WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));

  if (BC_PLUS.button_color(sdSysData, "+", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(1, 0);
    RANGE_INDICATOR.zoom_out(sdSysData);
  }

  // North Up
  ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (3.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                    WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));

  if (BTC_NORTH_UP.button_toggle_color(sdSysData, "NORTH\nUP", "DIR\nUP", sdSysData.PANEL_CONTROL.PANELS.ADSB_NORTH_UP,
                                  RAS_GREEN, RAS_BLUE, 
                                  sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
  {
    SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
    sdSysData.PANEL_CONTROL.PANELS.ADSB_NORTH_UP = !sdSysData.PANEL_CONTROL.PANELS.ADSB_NORTH_UP;
  }
  
  // Current Location Toggle
  if (ACTIVE_GPS)
  {
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (4.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));
    if (BTC_CENT.button_toggle_color( sdSysData, "CENT\n(On)", "CENT\n(Off)", 
                                      RANGE_INDICATOR.gps_display_current_location(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS),
                                      RAS_GREEN, RAS_BLUE, 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      RANGE_INDICATOR.gps_display_current_location_toggle(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
    }
  }
  else
  {
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (4.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));
    button_simple_enabled(sdSysData, "GPS\n", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM);
  }

  // Center on location
  if (ACTIVE_GPS && sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS != 1)
  {
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (5.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));


    if (BC_CENT_LOCATION.button_color(sdSysData, "CENT\nLOC", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS = 1;
    }
  }
  
  if (ACTIVE_ADSB && ACTIVE_GPS)
  {
    // MIN Airplane
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (6.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));

    if (BC_MIN.button_toggle_color(sdSysData, "MIN\n(On)", "MIN", 
                                      ( sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0 && 
                                        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 1), 
                                      RAS_GREEN, RAS_BLUE, 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);

      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 1)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 1);
      }
      else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 1)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 0);
      }
      else
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 1);
      }
    }

    // MAX Airplane
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (7.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));


    if (BC_MAX.button_toggle_color(sdSysData, "MAX\n(On)", "MAX", 
                                      ( sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0 && 
                                        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 2), 
                                      RAS_GREEN, RAS_BLUE, 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);

      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 1)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 2);
      }
      else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 2)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 0);
      }
      else
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(0, 2);
      }
    }
  }

  if (ACTIVE_GPS)
  {
    // Driving 
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - (8.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)), 
                                      WORKING_AREA.y + WORKING_AREA.w - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f)));


    if (BC_DRV.button_toggle_color(sdSysData, "DRV\n(On)", "DRV", 
                                      ( sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 1 && 
                                        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_1() == 1),
                                      RAS_GREEN, RAS_BLUE, 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);

      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(1, 1);
      }
      else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_1() == 1)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(1, 0);
      }
      else
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection_value(1, 1);
      }
    }
  }

  // Compass Calibration
  if (ACTIVE_COMPASS)
  {
    ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - 1.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                      WORKING_AREA.y));

    if (BTC_CALI.button_toggle_color(sdSysData, "<--", "...", sdSysData.COMMS_COMPASS.calibrate_on(),
                                    RAS_GREEN, RAS_BLUE, 
                                    sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      sdSysData.COMMS_COMPASS.calibrate_toggle();
    }

    if (sdSysData.COMMS_COMPASS.calibrate_on())
    {
      ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - 2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                        WORKING_AREA.y));
      if (BTC_LOCK.button_toggle_color(sdSysData, "CALI\n(On)", "CALI\n(Off)", sdSysData.COMMS_COMPASS.PROPS.CALIBRATION_ENABLED,
                                    RAS_GREEN, RAS_BLUE, 
                                    sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        sdSysData.COMMS_COMPASS.PROPS.CALIBRATION_ENABLED = !sdSysData.COMMS_COMPASS.PROPS.CALIBRATION_ENABLED;
      }

      ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - 3.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                        WORKING_AREA.y));
      if (BC_BEAR_RESET.button_color(sdSysData, "BEAR\nCLEAR", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        sdSysData.COMMS_COMPASS.bearing_known_offset_clear();
      }

      ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - 4.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                        WORKING_AREA.y));
      if (BC_CALI_RESET.button_color(sdSysData, "CALI\nRESET", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        sdSysData.COMMS_COMPASS.calibrateion_reset();
      }



      ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + WORKING_AREA.z - 6.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                        WORKING_AREA.y));
      if (BTC_GPS_ASSIST.button_toggle_color(sdSysData, "GPS-A\n(On)", "GPS-A\n(Off)", sdSysData.COMMS_COMPASS.PROPS.GPS_ASSIST_HEADING, 
                                        RAS_GREEN, RAS_BLUE, 
                                        sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        sdSysData.COMMS_COMPASS.PROPS.GPS_ASSIST_HEADING = !sdSysData.COMMS_COMPASS.PROPS.GPS_ASSIST_HEADING;
      }
    }
  }
}

void ADSB_MAP::screen_draw_calibration(ImDrawList *Draw_List, system_data &sdSysData)
{
  ImVec2 center = point_position_center(WORKING_AREA);
  
  ImVec2 p1;
  ImVec2 p2;
  ImVec2 p3;
  ImVec2 p4;

  ImVec2 r1;
  ImVec2 r2;
  ImVec2 c1;
  ImVec2 c2;
  
  // level 0 and level 1

  //if (!TEST_ALTERTATIVE_COMPASS)    // Display calibration
  /*
  {

    for (int quad = 1; quad < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS.size(); quad++)
    {
      for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS.size(); pos++)
      {
        p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS[pos].X / 4.0f), 
                            center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS[pos].Y / 4.0f));

        draw_marker(Draw_List, sdSysData, p1, RAS_ORANGE);
      }

      for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED.size(); pos++)
      {
        p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED[pos].X / 4.0f), 
                            center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED[pos].Y / 4.0f));

        draw_marker(Draw_List, sdSysData, p1, RAS_BLUE);
      }
    }

    // draw quad calibration
    if (sdSysData.COMMS_COMPASS.calibration_points_active_quad_overflow() == false)
    {
      for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS.size(); pos++)
      {
        p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS[pos].X / 4.0f), 
                            center.y + (sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS[pos].Y / 4.0f));

        draw_marker(Draw_List, sdSysData, p1, RAS_GREEN);
      }
    }

    // draw outlining box
    r1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_min_max_x().MIN_VALUE / 4.0f), 
                                    center.y + (sdSysData.COMMS_COMPASS.calibration_min_max_y().MIN_VALUE / 4.0f));
    r2 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_min_max_x().MAX_VALUE / 4.0f), 
                                    center.y + (sdSysData.COMMS_COMPASS.calibration_min_max_y().MAX_VALUE / 4.0f));
    
    draw_box(Draw_List, sdSysData, r1, r2, RAS_ORANGE, 2.0f);

    c1 = ImVec2(center.x + ((sdSysData.COMMS_COMPASS.calibration_min_max_x().MIN_VALUE - 
                                    sdSysData.COMMS_COMPASS.calibration_offset().X) / 4.0f), 
                        center.y + ((sdSysData.COMMS_COMPASS.calibration_min_max_y().MIN_VALUE - 
                                    sdSysData.COMMS_COMPASS.calibration_offset().Y) / 4.0f));
    c2 = ImVec2(center.x + ((sdSysData.COMMS_COMPASS.calibration_min_max_x().MAX_VALUE - 
                                    sdSysData.COMMS_COMPASS.calibration_offset().X) / 4.0f), 
                        center.y + ((sdSysData.COMMS_COMPASS.calibration_min_max_y().MAX_VALUE - 
                                    sdSysData.COMMS_COMPASS.calibration_offset().Y) / 4.0f));

    draw_box(Draw_List, sdSysData, c1, c2, RAS_WHITE, 2.0f);

    draw_line(Draw_List, sdSysData, r1, c1, RAS_WHITE, 2.0f);
    draw_line(Draw_List, sdSysData, r2, c2, RAS_WHITE, 2.0f);

    // level 2
    // A
    if (sdSysData.COMMS_COMPASS.calibration_simple() == false)
    {
      p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[1].OFFSET_POINT.X/ 4.0f), 
                    center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[1].OFFSET_POINT.Y / 4.0f));
      p2 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[2].OFFSET_POINT.X / 4.0f), 
                    center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[2].OFFSET_POINT.Y / 4.0f));
      p3 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[3].OFFSET_POINT.X / 4.0f), 
                    center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[3].OFFSET_POINT.Y / 4.0f));
      p4 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[4].OFFSET_POINT.X / 4.0f), 
                    center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[4].OFFSET_POINT.Y / 4.0f));

      draw_line(Draw_List, sdSysData, p1, p2, RAS_GREEN, 2.0f);
      draw_line(Draw_List, sdSysData, p2, p3, RAS_GREEN, 2.0f);
      draw_line(Draw_List, sdSysData, p3, p4, RAS_GREEN, 2.0f);
      draw_line(Draw_List, sdSysData, p4, p1, RAS_GREEN, 2.0f);

      draw_marker_filled(Draw_List, sdSysData, p1, RAS_WHITE);
      draw_marker_filled(Draw_List, sdSysData, p2, RAS_WHITE);
      draw_marker_filled(Draw_List, sdSysData, p3, RAS_WHITE);
      draw_marker_filled(Draw_List, sdSysData, p4, RAS_WHITE);
    }

  }
  */

  {
    for (size_t pos = 0; pos < sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_HISTORY.size(); pos++)
    {
      if (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
      {
        int color = RAS_GREY;

        if (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_HISTORY[pos].TAG)
        {
          color = RAS_BLUE;
        }

        //if (color != RAS_GREY)
        {
          c1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_HISTORY[pos].POINT.X / 4.0f), 
                        center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_HISTORY[pos].POINT.Y / 4.0f));
          draw_marker_filled(Draw_List, sdSysData, c1, color);
        }
      }
    }

    
    for (size_t pos = 0; pos < sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CALIBRATION_HISTORY.size(); pos++)
    {
      if (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CALIBRATION_HISTORY.FLAGS[pos].HAS_DATA)
      {
        int color = RAS_RED;

        if (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CALIBRATION_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE)
        {
          color = RAS_GREEN;
        }

        //if (color != RAS_GREY)
        {
          c1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CALIBRATION_HISTORY[pos].POINT.X / 4.0f), 
                        center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CALIBRATION_HISTORY[pos].POINT.Y / 4.0f));
          draw_marker_filled(Draw_List, sdSysData, c1, color);
        }
      }
    }


    // Draw Center
    c1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.X / 4.0f), 
                  center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.Y / 4.0f));
    draw_marker_filled(Draw_List, sdSysData, c1, RAS_WHITE);

    /*
    // Draw Upper and Lower Means
    p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.X_LOWER_MEAN/ 4.0f), 
                  center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.Y / 4.0f));

    p2 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.X_UPPER_MEAN/ 4.0f), 
                  center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.Y / 4.0f));

    p3 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.X/ 4.0f), 
                  center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.Y_LOWER_MEAN / 4.0f));

    p4 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_3.COMPASS_CENTER.X/ 4.0f), 
                  center.y + (sdSysData.COMMS_COMPASS.LEVEL_3.Y_UPPER_MEAN / 4.0f));


    draw_line(Draw_List, sdSysData, c1, p1, RAS_YELLOW, 2.0f);
    draw_line(Draw_List, sdSysData, c1, p2, RAS_YELLOW, 2.0f);
    draw_line(Draw_List, sdSysData, c1, p3, RAS_YELLOW, 2.0f);
    draw_line(Draw_List, sdSysData, c1, p4, RAS_YELLOW, 2.0f);
    */

    p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.RAW_XYZ.X/ 4.0f), 
              center.y + (sdSysData.COMMS_COMPASS.RAW_XYZ.Y / 4.0f));

    draw_line(Draw_List, sdSysData, c1, p1, RAS_WHITE, 4.0f);
  }
}

void ADSB_MAP::screen_text(system_data &sdSysData)
{
  //Put cursor in upper left
  ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x, WORKING_AREA.y));

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_RED)));
  ImGui::Text("WARNING: Information may be considered CONFIDENTIAL");
  ImGui::PopStyleColor();

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_GREY)));
  {
    ImGui::Text("TIME: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TIME_OF_SIGNAL.c_str());
    ImGui::Text("COUNT: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_COUNT.c_str());
    ImGui::Text("  POS: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_AIRCRAFT.c_str());

    // Maps and Other Information
    RANGE_INDICATOR.draw_info(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);

    // GPS Information
    if (ACTIVE_GPS)
    {
      ImGui::NewLine();

      ImGui::Text("GPS POSITION");
      ImGui::Text("   SPEED: %.1f", sdSysData.GPS_SYSTEM.CURRENT_POSITION.SPEED.val_mph());
      ImGui::Text("ALTITUDE: %.1f", sdSysData.GPS_SYSTEM.CURRENT_POSITION.ALTITUDE.feet_val());
      ImGui::Text(" HEADING: %.1f°", sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE);
      ImGui::Text("%3.0f%%  P:%2.1f \nH:%2.1f V:%2.1f", sdSysData.GPS_SYSTEM.CURRENT_POSITION.ACCURACY_SCORE * 100.0f,
                                                        sdSysData.GPS_SYSTEM.CURRENT_POSITION.PDOP,  
                                                        sdSysData.GPS_SYSTEM.CURRENT_POSITION.HDOP, 
                                                        sdSysData.GPS_SYSTEM.CURRENT_POSITION.VDOP);
    }
    //ImGui::Text("DSP TRK SZ: %.ld", sdSysData.MAP_SYSTEM.DISPLAYED_TRACK.TRACK_POINTS_DETAILED.size());
    ImGui::Text("Zoom Level: %d", sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL);

    // Compass Information
    if (ACTIVE_COMPASS)
    {
      ImGui::NewLine();

      ImGui::Text("COMPASS");

      ImGui::Text("BEARING:  %5.1f° ", sdSysData.COMMS_COMPASS.bearing_calibrated());

      if (sdSysData.GPS_SYSTEM.active(sdSysData.PROGRAM_TIME.current_frame_time())) // Enable
      {
        ImGui::Text("(%5.1f°) (%5.1f°)", sdSysData.COMMS_COMPASS.accumulated_gps_to_compass_bearing_error(),
                                  signed_angular_error(sdSysData.COMMS_COMPASS.bearing_calibrated(),
                                                        sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE));
        ImGui::Text("         (%5.1f°)", sdSysData.COMMS_COMPASS.bearing_known_offset());
      }

      // Print calibration data if calibration is on
      if (sdSysData.COMMS_COMPASS.calibrate_on())
      {
        //ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + (WORKING_AREA.z * 0.25f), WORKING_AREA.y + (WORKING_AREA.w * 0.55f)));
        ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x + (WORKING_AREA.z * 0.52f), WORKING_AREA.y + sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y));
        string info = sdSysData.COMMS_COMPASS.INFORMATION + "\n" + sdSysData.COMMS_COMPASS.LEVEL_3.INFORMATION_CALIBRATION;
        ImGui::Text("%s", info.c_str());
      }
    }
  }
  ImGui::PopStyleColor();
}

void ADSB_MAP::screen_draw_map_tracks(ImDrawList *Draw_List, system_data &sdSysData)
{
  // Load Current Values for Map Draw Routine, likely once per second.
  sdSysData.MAP_SYSTEM.RANGE_RADIUS_CIRCLE_POINT_SIZE = RANGE_INDICATOR.RADIUS_CIRCLE_POINT_SIZE;
  sdSysData.MAP_SYSTEM.CURRENT_RESOLUTION = RANGE_INDICATOR.resolution();
  sdSysData.MAP_SYSTEM.CURRENT_WORKING_AREA = WORKING_AREA;
  sdSysData.MAP_SYSTEM.CURRENT_CENTER_LAT_LON = RANGE_INDICATOR.get_center_lat_lon(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
  sdSysData.MAP_SYSTEM.CURRENT_MAP_BEARING = MAP_HEADING_DEGREES_LATEST;
  sdSysData.MAP_SYSTEM.CURRENT_LAT_LON_SCALE = RANGE_INDICATOR.ll_2_pt_scale();

  // Draw track of GPS Position.
  if (sdSysData.MAP_SYSTEM.TRACK_2.TRACK_POINTS_DETAILED.size() > 1)
  {
    draw_track_2(Draw_List, sdSysData, WORKING_AREA, RANGE_INDICATOR.ll_2_pt_scale(), 
                (int)RANGE_INDICATOR.range(), 6.0f, 
                GPS_ALTITUDE_COLOR_SCALE, 
                RANGE_INDICATOR.get_center_lat_lon(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS), 
                MAP_HEADING_DEGREES_LATEST, sdSysData.MAP_SYSTEM.DISPLAYED_TRACK);

  }
}

void ADSB_MAP::screen_draw_position_marker(ImDrawList *Draw_List, system_data &sdSysData)
{
  bool draw = false;
  ImVec2 gps_pos = point_position_lat_lon(WORKING_AREA, RANGE_INDICATOR.ll_2_pt_scale(), 
                                          RANGE_INDICATOR.get_center_lat_lon(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS), 
                                          RANGE_INDICATOR.get_gps_lat_lon(), MAP_HEADING_DEGREES_LATEST, draw);

  // Draw point position compass
  if (draw)
  {
    //if (RANGE_INDICATOR.CENTER_ON_LOCATION == 1)
    {
      // draw compass at center location
      CURRENT_POSITION_COMPASS.draw(Draw_List, sdSysData, 2, gps_pos, WORKING_AREA.w / 2.0f * 0.66f, true, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                          sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE, 
                          ACTIVE_COMPASS, sdSysData.COMMS_COMPASS.bearing_calibrated(), true, 
                          true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), MAP_HEADING_DEGREES_LATEST);
    }
    /*
    else
    {
      // draw compass at gps pos
      CURRENT_POSITION_COMPASS.draw(Draw_List, sdSysData, 1, gps_pos, 15.0f, true, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                          sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING, 
                          ACTIVE_COMPASS, sdSysData.COMMS_COMPASS.bearing_calibrated(), !NORTH_UP, MAP_HEADING_DEGREES_LATEST);
    }
    */

    ImGui::SetCursorScreenPos(ImVec2(gps_pos.x - 20.0f, gps_pos.y - 20.0f));
    if (ImGui::InvisibleButton("GPS CURRENT POSITION COMPASS", ImVec2(40.0f, 40.0f)))
    {
      sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS = 1;
    }
  }
}

void ADSB_MAP::screen_draw_compass_center(ImDrawList *Draw_List, system_data &sdSysData)
{
  // For No Valid Coordinates

  ImVec2 screen_position;
  screen_position.x = WORKING_AREA.x + WORKING_AREA.z / 2.0f;
  screen_position.y = WORKING_AREA.y + WORKING_AREA.w / 2.0f;

  CURRENT_POSITION_COMPASS.draw(Draw_List, sdSysData, 2, screen_position, WORKING_AREA.w / 2.0f * 0.66f, true, false, 
                      false, 0.0f, 
                      ACTIVE_COMPASS, sdSysData.COMMS_COMPASS.bearing_calibrated(), false, 
                      true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), 0.0f);

  /*
  CURRENT_POSITION_COMPASS.draw(Draw_List, sdSysData, 2, screen_position, WORKING_AREA.w / 2.0f * 0.66f, true, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                      sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING, 
                      ACTIVE_COMPASS, sdSysData.COMMS_COMPASS.bearing_calibrated(), true, 
                      true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), MAP_HEADING_DEGREES_LATEST);
  */

  /*
  CURRENT_POSITION_COMPASS.draw(Draw_List, sdSysData, 1, screen_position, 15.0f, true, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                      sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING, 
                      ACTIVE_COMPASS, sdSysData.COMMS_COMPASS.bearing_calibrated(), !NORTH_UP, MAP_HEADING_DEGREES_LATEST);  
                      */
}

void ADSB_MAP::screen_draw_aircraft(ImDrawList *Draw_List, system_data &sdSysData)
{
  for (int aircraft = 0; aircraft < (int)sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); aircraft ++)
  {
    if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].active())
    {
      AIRCRAFT tmp_clicked_aircraft = draw_aircraft_map_marker(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft], 
                                                                Draw_List, sdSysData, WORKING_AREA, 
                                                                RANGE_INDICATOR.ll_2_pt_scale(), (int)RANGE_INDICATOR.range(), 
                                                                RANGE_INDICATOR.get_center_lat_lon(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS), 
                                                                MAP_HEADING_DEGREES_LATEST, 
                                                                ALTITUDE_COLOR_SCALE);
      
      if (tmp_clicked_aircraft.HEX.get_str_value() != "")
      {
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT = tmp_clicked_aircraft;
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT_HEX = tmp_clicked_aircraft.HEX.get_str_value();
        sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS = 2;
      }
    }
  }
}
  
void ADSB_MAP::create()
{
  // Prepare Map Orientation with Impact Resistance
  MAP_HEADING_DEGREES.set_size(60);

  // Prepare Compass
  CURRENT_POSITION_COMPASS.set_size(32, (15 / 2));
  // set at frame rate for slow and jitter size for fast.

  RANGE_INDICATOR.create();

  if (GPS_ALTITUDE_COLOR_SCALE.active() == false)
  {
    // This is not representing altitude. It is representing speed in mph.
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(10.0f, RAS_RED);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(15.0f, RAS_YELLOW);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(35.0f, RAS_ORANGE);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(45.0f, RAS_GREEN);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(55.0f, RAS_BLUE);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(75.0f, RAS_PURPLE);
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(100.0f, RAS_WHITE);
  }

  // Create vars if first run
  if (ALTITUDE_COLOR_SCALE.active() == false)
  {
    ALTITUDE_COLOR_SCALE.add_color_value_pair(500.0f, RAS_RED);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(1000.0f, RAS_YELLOW);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(2500.0f, RAS_ORANGE);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(12000.0f, RAS_GREEN);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(45000.0f, RAS_BLUE);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(65000.0f, RAS_PURPLE);
    ALTITUDE_COLOR_SCALE.add_color_value_pair(100000.0f, RAS_WHITE);
  }

  // KLFT - Lafayette Regional/Paul Fournet Field Airport
  // Coordinates: 
  //                                LATITUDE      LONGITUDE
  // Degrees Decimal Minutes (DDM): N30°12.30'  / W91°59.27'
  // Decimal Degrees (DD):          30.205      / -91.987833

  RANGE_INDICATOR.set_no_pos_lat_lon(DOUBLE_VEC2(30.205f, -91.987833f));

}

void ADSB_MAP::draw(system_data &sdSysData)
{
  // ADSB Draw List
  ImDrawList* draw_list_map = ImGui::GetWindowDrawList();

  WORKING_AREA = get_working_area();

  // -------------------------------------------------------------------------------------

  // Gather some frequent variables
  ACTIVE_GPS        = sdSysData.GPS_SYSTEM.active(sdSysData.PROGRAM_TIME.current_frame_time());
  ACTIVE_COMPASS    = sdSysData.COMMS_COMPASS.connected();
  ACTIVE_ADSB       = sdSysData.AIRCRAFT_COORD.is_active();
  ACTIVE_AUTOMOBILE = sdSysData.CAR_INFO.active();

  // -------------------------------------------------------------------------------------

  // Update current position if requested by panel
  if (sdSysData.PANEL_CONTROL.PANELS.MAP_CENTER_TO_LAST_KNOWN_POSITION)
  {
    // Will force map to center on current location
    // Set to map to focus on current location.  first start, likely -9 meaning no location
    sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS = 1;

    sdSysData.PANEL_CONTROL.PANELS.MAP_CENTER_TO_LAST_KNOWN_POSITION = false;
    RANGE_INDICATOR.set_gps_pos_lat_lon(sdSysData.PANEL_CONTROL.PANELS.LAST_KNOWN_GOOD_POSITION, 
                                        sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
  }

  // -------------------------------------------------------------------------------------
  
  // If airplanes are not visible and ADSB Min Max, switch to driving mode settings. 
  //  And, back again when Airplanes show up.
  bool ACTIVE_ADSB_WITH_AIRCRAFT = ACTIVE_ADSB && sdSysData.AIRCRAFT_COORD.DATA.POSITIONED_AIRCRAFT > 0;
  if (ACTIVE_ADSB_WITH_AIRCRAFT == false && 
      sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0)
  {
    RANGE_INDICATOR.AIRCRAFT_COUNT_ZERO_HOLD_OFF = true;
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection(1);
  }
  else if ( ACTIVE_ADSB_WITH_AIRCRAFT && 
            RANGE_INDICATOR.AIRCRAFT_COUNT_ZERO_HOLD_OFF)
  {
    RANGE_INDICATOR.AIRCRAFT_COUNT_ZERO_HOLD_OFF = false;
    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.set_selection(0);
  }
  

  // Return to normal zoom level if drv minmax turned off.  
  if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.CHANGED)
  {
    // Monitor zoom level changes from internal or panel control.
    // zoom return if min or max or drv turned off.

    sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.CHANGED = false;

    if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0)
    {
      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 0)
      {
        RANGE_INDICATOR.zoom_return(sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
      }
    }
    else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 1)
    {
      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_1() == 0)
      {
        RANGE_INDICATOR.zoom_return(sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_LEVEL, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
      }
    }
  }

  // Adjust dynamic zooms, but only if GPS Active, bc all mesured from position
  if (ACTIVE_GPS)
  {
    if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection() == 0)
    {
      // set ADSB Range if min or max
      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() > 0)
      { 
        // Set zoom level to range.
        if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 1)        // Zoom at MIN aircraft distance
        {
          RANGE_INDICATOR.set_range(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.DISTANCE_CLOSEST * 0.75f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
        }
        else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_0() == 2)   // Zoom at MAX aircraft distance
        {
          RANGE_INDICATOR.set_range(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.DISTANCE_FURTHEST * 0.75f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
        }

      }
    }
    else // if (sdSysData.PANEL_CONTROL.ADSB_ZOOM_MODE_SELECTION.value_selection() == 1)
    {
      // set Driving Range in DRV
      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_ZOOM_MODE_SELECTION.value_selection_1() == 1)
      {    
        // Check all ADSB_RANGE_INDICATOR driving zooming
        if (ACTIVE_AUTOMOBILE)
        {
          if (sdSysData.CAR_INFO.STATUS.SPEED.SPEED_ALL_TIRES_AVERAGE.val_mph() < 10.0f)
          {
            RANGE_INDICATOR.set_range(.03f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
          }
          else 
          {
            RANGE_INDICATOR.set_range(((sdSysData.CAR_INFO.STATUS.SPEED.SPEED_ALL_TIRES_AVERAGE.val_mph() - 10.0f) * (0.5f / 60.0f)) + 0.03f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
          }
        }
        else if (ACTIVE_GPS)
        {
          if (sdSysData.GPS_SYSTEM.CURRENT_POSITION.SPEED.val_mph() < 10.0f)
          {
            RANGE_INDICATOR.set_range(.03f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
          }
          else 
          {
            RANGE_INDICATOR.set_range(((sdSysData.GPS_SYSTEM.CURRENT_POSITION.SPEED.val_mph() - 10.0f) * (0.5f / 60.0f)) + 0.03f, sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
          }
        }
      }
    }
  }

  // -------------------------------------------------------------------------------------

  // Store some map info regarding positions
  // Store current gps location
  {
    if (sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_COORDS && sdSysData.GPS_SYSTEM.CURRENT_POSITION.CHANGED_POSITION_FOR_DRAW)
    {
      // Mark Changes as seen.
      sdSysData.GPS_SYSTEM.CURRENT_POSITION.CHANGED_POSITION_FOR_DRAW = false;

      // Check and store GPS Current Location
      RANGE_INDICATOR.set_gps_pos_lat_lon(DOUBLE_VEC2(sdSysData.GPS_SYSTEM.CURRENT_POSITION.LATITUDE, 
                                                      sdSysData.GPS_SYSTEM.CURRENT_POSITION.LONGITUDE), 
                                                      sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
    }
  }

  // Store tracked ADSB Aircraft
  {
    if (ACTIVE_ADSB)
    {
      // Check and store AIRCRAFT Current Location
      RANGE_INDICATOR.set_aircraft_pos_lat_lon(DOUBLE_VEC2( sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.POSITION.LATITUDE.get_double_value(), 
                                                            sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.POSITION.LONGITUDE.get_double_value()), 
                                                            sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);
    }
  }

  // -------------------------------------------------------------------------------------

  RANGE_INDICATOR.range_update(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS);

  // -------------------------------------------------------------------------------------

  // Before drawing the map, get map heading in degrees from sources Compass or GPS or both

  if (!sdSysData.PANEL_CONTROL.PANELS.ADSB_NORTH_UP)
  {
    if (sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS == 1)
    {
      // Map direction for GPS
      if (ACTIVE_COMPASS && ACTIVE_GPS)
      {
        if (sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALID)
        {
          MAP_HEADING_DEGREES.set_value(sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE);
        }
        else
        {
          MAP_HEADING_DEGREES.set_value(sdSysData.COMMS_COMPASS.bearing_calibrated());
        }
      }
      else if (ACTIVE_COMPASS)
      {
        MAP_HEADING_DEGREES.set_value(sdSysData.COMMS_COMPASS.bearing_calibrated());
      }
      else if (ACTIVE_GPS)
      {
        MAP_HEADING_DEGREES.set_value(sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE);
      }
      else
      {
        MAP_HEADING_DEGREES.set_value(0.0f);
      }
    }
    else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS == 2)
    {
      // Map Heading for Aircraft
      if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.TRACK.conversion_success())
      {
        MAP_HEADING_DEGREES.set_value(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.TRACK.get_float_value());
      }
      else
      {
        MAP_HEADING_DEGREES.set_value(0.0f);
      }
    }
  }
  else
  {
    MAP_HEADING_DEGREES.set_value(0.0f);
  }

  MAP_HEADING_DEGREES_LATEST = MAP_HEADING_DEGREES.value_no_roll(360.0f);

  // -------------------------------------------------------------------------------------

  // Hide buttons if on screen for time
  if (SHOW_BUTTONS && SHOW_BUTTONS_TIMER.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
  {
    SHOW_BUTTONS = false;
  }


  // -------------------------------------------------------------------------------------
  // Screen Draw Start

  // Button for Hide Buttons
  //Put cursor in upper left
  ImGui::SetCursorScreenPos(ImVec2(WORKING_AREA.x, WORKING_AREA.y));

  if (SHOW_BUTTONS == false)
  {
    if (ImGui::InvisibleButton("Hide Buttons", ImVec2(WORKING_AREA.z, WORKING_AREA.w)))
    {
      SHOW_BUTTONS = !SHOW_BUTTONS;

      if (SHOW_BUTTONS)
      {
        SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      }
    }
  }

  // Draw Compass Calibration
  if (ACTIVE_COMPASS)
  {
    if (sdSysData.COMMS_COMPASS.calibrate_on())
    {
      screen_draw_calibration(draw_list_map, sdSysData); 
    }
  }

  // All Text Below Here
  screen_text(sdSysData);

  // All Buttons
  if (SHOW_BUTTONS)
  {
    screen_buttons(sdSysData);
  }

  // All Text Above Here
  // -------------------------------------------------------------------------------------
  RANGE_INDICATOR.draw_scale(draw_list_map, sdSysData, WORKING_AREA);

  // Draw Landmarks
  for (int landmark = 0; landmark < (int)sdSysData.MAP_SYSTEM.LANDMARKS.size(); landmark++)
  {
    draw_landmark(draw_list_map, sdSysData, sdSysData.MAP_SYSTEM.LANDMARKS[landmark], 
                    WORKING_AREA, RANGE_INDICATOR.ll_2_pt_scale(), 
                    RANGE_INDICATOR.get_center_lat_lon(sdSysData.PANEL_CONTROL.PANELS.ADSB_MAP_LOCATION_FOCUS), 
                    MAP_HEADING_DEGREES_LATEST, RANGE_INDICATOR.range());
  }

  // Draw map tracks
  screen_draw_map_tracks(draw_list_map, sdSysData);

  // Draw Current Position Marker
  if (ACTIVE_GPS && sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_COORDS)
  {
    screen_draw_position_marker(draw_list_map, sdSysData);
  }
  else
  {
    screen_draw_compass_center(draw_list_map, sdSysData);
  }
  
  // Draw Aircraft
  screen_draw_aircraft(draw_list_map, sdSysData);
}

void ADSB_SCREEN::adsb_table_draw(system_data &sdSysData)
{
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

  ImGui::BeginChild("ADSB Display", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    ImGui::Text("Time: %s  Count: %s  Pos: %s  Furthest: %.2f", 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TIME_OF_SIGNAL.c_str(), 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_COUNT.c_str(), 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_AIRCRAFT.c_str(),
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.FURTHEST_AIRCRAFT_SINCE_START);
    
    //      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
    //                              ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

    if (ImGui::BeginTable("Aircraft Data", 13, sdSysData.SCREEN_DEFAULTS.flags_t))
    {
      {
        ImGui::TableSetupColumn(" ");
        ImGui::TableSetupColumn("FLIGHT");
        ImGui::TableSetupColumn("SQWK");
        ImGui::TableSetupColumn("G SPD");
        ImGui::TableSetupColumn("V RTE");
        ImGui::TableSetupColumn("ALT");
        ImGui::TableSetupColumn("ANAV");
        ImGui::TableSetupColumn("HDG");
        ImGui::TableSetupColumn("HNAV");
        ImGui::TableSetupColumn("DIST");
        ImGui::TableSetupColumn("ANG");
        ImGui::TableSetupColumn("SEEN");  // seen p
        //ImGui::TableSetupColumn("SEEN");
        ImGui::TableSetupColumn("RSSI");
        ImGui::TableHeadersRow();
      }
      
      //for (int pos = 0; pos < SDATA.AIRCRAFT_LIST.AIRCRAFTS.size(); pos++)
      for (int pos = 0; pos < (int)sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); pos++)
      {
        if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].is_expired(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
        {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("%d", (pos + 1));
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.FLIGHT.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.SQUAWK.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.SPEED.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.VERT_RATE.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.ALTITUDE.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.NAV_ALTITUDE_MCP.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.TRACK.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.NAV_HEADING.get_str_value().c_str());
          ImGui::TableNextColumn();
          
          if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.DISTANCE_FROM_BASE != -1.0f)
          {
            ImGui::Text("%.2f", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.DISTANCE_FROM_BASE);
          }
          else
          {
            ImGui::Text(" ");
          }

          ImGui::TableNextColumn();

          if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.ANGLE_FROM_BASE != -1.0f)
          {
            ImGui::Text("%.0f", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.ANGLE_FROM_BASE);
          }
          else
          {
            ImGui::Text(" ");
          }

          ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.SEEN_POS.get_str_value().c_str());
          ImGui::TableNextColumn();
          //ImGui::Text("%s", AIRCRAFT_DATA.SEEN.get_str_value().c_str());
          //ImGui::TableNextColumn();
          ImGui::Text("%s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].AIRCRAFT_ITEM.RSSI.get_str_value().c_str());
        }
        else
        {
          // Clear all data and reset to start empty state.
          sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos].clear();  // CLEAR NEEDS CLEANING

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          ImGui::Text(" ");
          ImGui::TableNextColumn();
          //ImGui::Text(" ");
          //ImGui::TableNextColumn();
          ImGui::Text(" ");
        }
      }
      ImGui::EndTable();
    }
  }
  ImGui::EndChild();

  ImGui::PopStyleColor();
}

void ADSB_SCREEN::create()
{
  ADSB_MAP_DISPLAY.create();
}

void ADSB_SCREEN::display(system_data &sdSysData)
{
  // Search for tracked aircraft and update its info if found  
  if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.CHANGED == true)
  {
    for (int pos_search = 0; pos_search < (int)sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); pos_search++)
    {
      if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos_search].AIRCRAFT_ITEM.HEX.get_str_value() == sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT_HEX)
      {
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT = sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[pos_search].AIRCRAFT_ITEM;
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.CHANGED = false;
      }
    }
  }

  ImGui::BeginChild("ADSB Buttons", ImVec2(90 * DEF_SCREEN_SIZE_X_MULTIPLIER, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (BTC_ADSB.button_toggle_color(sdSysData, "ADSB\n(On)", "ADSB\n(Off)", sdSysData.AIRCRAFT_COORD.is_active(), RAS_RED, RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      if (sdSysData.AIRCRAFT_COORD.is_active() == true)
      {
        sdSysData.SCREEN_COMMS.command_text_set(" stopadsb");
      }
      else
      {
        sdSysData.SCREEN_COMMS.command_text_set(" startadsb");
      }
    }

    if (button_simple_enabled(sdSysData, "ADSB\nSNAP\nSHOT", sdSysData.AIRCRAFT_COORD.is_active(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      sdSysData.SCREEN_COMMS.command_text_set(" adsbsnap");
    }

    if (BC_VIEW.button_color(sdSysData, "VIEW", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      if (sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_TABLE)
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_TABLE = false;
        sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_MAP = true;
      }
      else
      {
        sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_TABLE = true;
        sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_MAP = false;
      }
    }
  }
  ImGui::EndChild();

  ImGui::SameLine();
  
  if (sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_TABLE)
  {
    adsb_table_draw(sdSysData);
  }
  else if (sdSysData.PANEL_CONTROL.PANELS.ADSB_DISPLAY_MAP)
  {
    ImGui::BeginChild("ADSB Map", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      ADSB_MAP_DISPLAY.draw(sdSysData);
    }
    ImGui::EndChild();
  }
}

// ---------------------------------------------------------------------------------------
#endif
