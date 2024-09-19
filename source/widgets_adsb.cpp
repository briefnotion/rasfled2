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

float degrees_to_radians(float Degrees)
{
  return ((Degrees * float_PI) / 180.0f);
}

void rotate_point(ImVec2 Center, float Angle_In_Rads, ImVec2 &Point) 
{
  float s = sin(Angle_In_Rads);
  float c = cos(Angle_In_Rads);

  // Translate point back to origin
  Point.x -= Center.x;
  Point.y -= Center.y;

  // Rotate point
  float xnew = Point.x * c + Point.y * s;
  float ynew = -Point.x * s + Point.y * c;

  // Translate point back
  Point.x = xnew + Center.x;
  Point.y = ynew + Center.y;
}

ImVec2 point_position_center(ImVec4 Working_Area)
{
  ImVec2 ret_center;
  ret_center.x = Working_Area.x + Working_Area.z / 2.0f;
  ret_center.y = Working_Area.y + Working_Area.w / 2.0f;
  return ret_center;
}

ImVec2 point_position_lat_lon(ImVec4 Working_Area, ImVec2 Scale, 
                                ImVec2 Lat_Lon_Center, ImVec2 Lat_Lon, 
                                float Degrees, bool &Drawn)
{
  ImVec2 center = point_position_center(Working_Area);

  ImVec2 ret_point;

  float lat_diff = Lat_Lon.x - Lat_Lon_Center.x;
  float lon_diff = Lat_Lon.y - Lat_Lon_Center.y;

  ret_point.y = -(lat_diff * Scale.x) + center.y;
  ret_point.x = (lon_diff * Scale.y) + center.x;

  if (Degrees != 0.0f)
  {
    rotate_point(center, degrees_to_radians(Degrees), ret_point);
  }

  // check if draw_position is within screen size + offset.
  if (ret_point.x >= Working_Area.x && ret_point.x <= Working_Area.x + Working_Area.z && 
      ret_point.y >= Working_Area.y && ret_point.y <= Working_Area.y + Working_Area.w)
  {
    Drawn = true;
  }
  else
  {
    Drawn = false;
  }

  return ret_point;
}

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
  Draw_List->AddLine(Screen_Position_1, Screen_Position_2, sdSysData.COLOR_SELECT.neo_color(Color).STANDARD.color(), Size);
}

void draw_box(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position_1, ImVec2 Screen_Position_2, int Color, float Size)
{
  // simple draw line
  Draw_List->AddRect(Screen_Position_1, Screen_Position_2, sdSysData.COLOR_SELECT.neo_color(Color).STANDARD.color(), 0.0f, 0, Size);
}

void draw_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgonFilled(Screen_Position, 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color(Color).STANDARD.color(), 4.0f);
}

void draw_marker_filled(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgonFilled(Screen_Position, 4.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color(Color).STANDARD.color(), 4.0f);
}

void draw_airport_marker(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Screen_Position, int Color)
{
  Draw_List->AddNgon(Screen_Position, 4.0f, sdSysData.COLOR_SELECT.neo_color(Color).STANDARD.color(), 4.0f, 1.5f);
}

void draw_point_marker(ImDrawList *Draw_List, ImVec2 Screen_Position, ImColor Color, float Size)
{
  Draw_List->AddNgonFilled(Screen_Position, Size, Color, 4.0f);
}

void draw_track(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, int Draw_Level_Of_Detail, 
                float Strength_Point_Size, NEW_COLOR_SCALE &Color_Scale, 
                ImVec2 Center_Lat_Lon, float Map_Bearing, DETAILED_TRACK &Track)
{
  bool draw_0 = false;
  bool draw_1 = false;

  ImVec2 track_position_0;
  ImVec2 track_position_1;

  // Set First Point
  if ((int)Track.TRACK_POINTS_SIMPLE.size() > 0)
  {
    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      ImVec2(Track.TRACK_POINTS_SIMPLE[0].LATITUDE, Track.TRACK_POINTS_SIMPLE[0].LONGITUDE), Map_Bearing, draw_1);
  }
  else
  {
    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      ImVec2(Track.TRACK_POINTS_DETAILED[0].LATITUDE, Track.TRACK_POINTS_DETAILED[0].LONGITUDE), Map_Bearing, draw_1);
  }

  // Draw Simple Track
  if (Draw_Level_Of_Detail < 1)
  {
    Draw_Level_Of_Detail = 1;
  }

  for(int position = 1; position < (int)Track.TRACK_POINTS_SIMPLE.size(); position = position + Draw_Level_Of_Detail)
  {
    track_position_0 = track_position_1;
    draw_0 = draw_1;

    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                    ImVec2(Track.TRACK_POINTS_SIMPLE[position].LATITUDE, Track.TRACK_POINTS_SIMPLE[position].LONGITUDE), Map_Bearing, draw_1);

    Draw_List->AddLine(track_position_0, track_position_1, 
                        sdSysData.COLOR_SELECT.neo_c_grey().TEXT.color(), 1.0f);
  }

  // Draw Detailed Track
  for(int position = 1; position < (int)Track.TRACK_POINTS_DETAILED.size(); position++)
  {
    track_position_0 = track_position_1;
    draw_0 = draw_1;

    track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                    ImVec2(Track.TRACK_POINTS_DETAILED[position].LATITUDE, Track.TRACK_POINTS_DETAILED[position].LONGITUDE), Map_Bearing, draw_1);

    if (draw_0 || draw_1)
    {
      ImColor point_color = sdSysData.COLOR_SELECT.neo_color(Color_Scale.get_color(Track.TRACK_POINTS_DETAILED[position].ALTITUDE)).TEXT.color();

      point_color.Value.w = Track.TRACK_POINTS_DETAILED[position].RSSI_INTENSITY;

      draw_point_marker(Draw_List, track_position_0, point_color, Strength_Point_Size);

      Draw_List->AddLine(track_position_0, track_position_1, 
                          sdSysData.COLOR_SELECT.neo_c_grey().TEXT.color(), 2.0f);
    }
  }
}

// Draw all aircraft onto the maps.
AIRCRAFT draw_aircraft_map_marker(AIRCRAFT_MAP_DETAILS Aircraft, ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                                            int Draw_Level_Of_Detail, ImVec2 Center_Lat_Lon, float Map_Bearing, NEW_COLOR_SCALE &Altitude_Color_Scale)
{
  AIRCRAFT ret_clicked_aircraft;

  if (Aircraft.AIRCRAFT_ITEM.POSITION.GLOBAL_POSITION_FOUND)
  {
    bool draw = false;
    ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon,
                                                  ImVec2( Aircraft.AIRCRAFT_ITEM.POSITION.LATITUDE.get_float_value(), 
                                                          Aircraft.AIRCRAFT_ITEM.POSITION.LONGITUDE.get_float_value()), 
                                                          Map_Bearing, draw);

    // Draw track first then overlay aircraft.
    if (Aircraft.TRACK.TRACK_POINTS_DETAILED.size() > 1)
    {
      draw_track(Draw_List, sdSysData, Working_Area, Scale, Draw_Level_Of_Detail, 3.0f, Altitude_Color_Scale, Center_Lat_Lon, Map_Bearing, Aircraft.TRACK);
    }

    // Text Describing Aircraft
   
    if (Aircraft.AIRCRAFT_ITEM.SEEN_POS.get_int_value() <= 5)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_yellow().STANDARD.color()));
    }

    ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y - 39.0f));
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_SQUAWK, 135.0f, BB_BL);   // Squawk

    ImGui::SetCursorScreenPos(ImVec2(draw_position.x - 39.0f, draw_position.y));
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_FLIGHT, 225.0f, BB_BL);   // Flight
    
    ImGui::SetCursorScreenPos(ImVec2(draw_position.x - 33.0f, draw_position.y - 3.0f));
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_SPEED, 135.0f, BB_BL);    // Speed
    
    ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 30.0f));
    Text_Rotate(Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_ALTITUDE, 225.0f, BB_BL); // Altitude
    
    if (Aircraft.AIRCRAFT_ITEM.DISTANCE_FROM_BASE >= 0.0f)
    {
      ImGui::SetCursorScreenPos(ImVec2(draw_position.x + 23.0f, draw_position.y + 23.0f));
      ImGui::Text("%s", Aircraft.AIRCRAFT_ITEM.META.COMPASS_INFO_DISP_DISTANCE_FROM_BASE.c_str());
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

void MAP_MARKER::clear()
{
  LAT_LON = ImVec2(0.0f, 0.0f);
  DISPLAY_NAME = "";
  LONG_NAME = "";
  TYPE = 0;
  AIRPORT_LANDING_VECTORS.clear();
  REGION_GPS_COORDS.clear();
}

void MAP_MARKER::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, 
                      ImVec2 Center_Lat_Lon, float Map_Bearing, float Range)
{
  bool draw = false;
  ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, LAT_LON, Map_Bearing, draw);

  if (draw || TYPE == 3)
  {
    // Possibly set colors here
    // ---

    // Draw Symbol
    switch (TYPE)
    {
      case 0: //  0 - Generic
      {
        draw_marker(Draw_List, sdSysData, draw_position, sdSysData.COLOR_SELECT.yellow());
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_yellow().STANDARD.color()));
        ImGui::Text("%s", DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }
      
      case 1: //  1 - Airport
      {
        if (AIRPORT_LANDING_VECTORS.size() > 1)
        {
          bool on_screen = false;
          
          for(int vector = 0; vector < (int)AIRPORT_LANDING_VECTORS.size(); vector++)
          {
            // 5 miles out.
            ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                      get_coords_x_miles_from_coords(LAT_LON.x, LAT_LON.y, 5.0f, AIRPORT_LANDING_VECTORS[vector]), 
                                                      Map_Bearing, on_screen);

            Draw_List->AddLine(draw_position, landing_vector_end, 
                                sdSysData.COLOR_SELECT.neo_c_yellow().STANDARD.color(), 2);
          }
        }
        
        draw_airport_marker(Draw_List, sdSysData, draw_position, sdSysData.COLOR_SELECT.yellow());
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_yellow().STANDARD.color()));
        ImGui::Text("%s", DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }

      case 2: // 2 - Region
      {
        ImColor Color = sdSysData.COLOR_SELECT.neo_c_cyan().STANDARD_V.color();
        if (REGION_GPS_COORDS.size() > 1.0f)
        {
          //bool on_screen = false;
          
          ImVec2 landing_vector_start = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, REGION_GPS_COORDS[REGION_GPS_COORDS.size() -1.0f], Map_Bearing, draw);
          ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, REGION_GPS_COORDS[0], Map_Bearing, draw);
          Draw_List->AddLine(landing_vector_start, landing_vector_end, Color, 2.0f);

          for(int pos = 1; pos < (int)REGION_GPS_COORDS.size(); pos++)
          {
            landing_vector_start = landing_vector_end;
            landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, REGION_GPS_COORDS[pos], Map_Bearing, draw);

            Draw_List->AddLine(landing_vector_start, landing_vector_end, 
                                Color, 2.0f);
          }
        }
        
        //draw_airport_marker(sdSysData, draw_position, sdSysData.COLOR_SELECT.neo_color_COMB_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5.0f));

        if (Range < 35.0f)
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(Color));
          ImGui::Text("%s", DISPLAY_NAME.c_str());
          ImGui::PopStyleColor();
        }
        break;
      }

      case 3: // 2 - Interstate
      {
        ImColor Color = sdSysData.COLOR_SELECT.neo_c_cyan().STANDARD.color();
        if (REGION_GPS_COORDS.size() > 1)
        {
          bool on_screen_1 = false;
          bool on_screen_2 = false;
  
          ImVec2 landing_vector_start; 
          ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, REGION_GPS_COORDS[0], Map_Bearing, on_screen_2);

          for(int pos = 1; pos < (int)REGION_GPS_COORDS.size(); pos++)
          {
            landing_vector_start = landing_vector_end;
            on_screen_1 = on_screen_2;

            landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, REGION_GPS_COORDS[pos], Map_Bearing, on_screen_2);

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

// ---------------------------------------------------------------------------------------

void ADSB_RANGE::calculate_lat_lon_to_point_scale()
{
  float latitude_diff = 0;
  float longitude_diff = 0;
 
  ImVec2 new_coords_east;
  ImVec2 new_coords_south;

  if (CENTER_ON_LOCATION == 1)
  {
    new_coords_east = get_coords_x_miles_from_coords(GPS_POS_LAT_LON.x, GPS_POS_LAT_LON.y,
                                                          RANGE, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(GPS_POS_LAT_LON.x, GPS_POS_LAT_LON.y,
                                                          RANGE, 180.0f);

    latitude_diff = abs(new_coords_south.x - GPS_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - GPS_POS_LAT_LON.y);
  }
  else if (CENTER_ON_LOCATION == 2)
  {
    new_coords_east = get_coords_x_miles_from_coords(AIRCRAFT_POS_LAT_LON.x, AIRCRAFT_POS_LAT_LON.y,
                                                          RANGE, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(AIRCRAFT_POS_LAT_LON.x, AIRCRAFT_POS_LAT_LON.y,
                                                          RANGE, 180.0f);

    latitude_diff = abs(new_coords_south.x - AIRCRAFT_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - AIRCRAFT_POS_LAT_LON.y);
  }
  else
  {
    new_coords_east = get_coords_x_miles_from_coords(NO_POS_LAT_LON.x, NO_POS_LAT_LON.y,
                                                          RANGE, 90.0f);
    new_coords_south = get_coords_x_miles_from_coords(NO_POS_LAT_LON.x, NO_POS_LAT_LON.y,
                                                          RANGE, 180.0f);

    latitude_diff = abs(new_coords_south.x - NO_POS_LAT_LON.x);
    longitude_diff = abs(new_coords_east.y - NO_POS_LAT_LON.y);
  }

  LAT_LON_TO_POINT_SCALE.x =(RADIUS_CIRCLE_POINT_SIZE / latitude_diff);
  LAT_LON_TO_POINT_SCALE.y = (RADIUS_CIRCLE_POINT_SIZE / longitude_diff);
}

void ADSB_RANGE::set_zoom_level()
{
  switch (ZOOM_LEVEL)
  {
    case 0:
    {
      set_range(0.01f);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 1:
    {
      set_range(0.03f);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 2:
    {
      set_range(0.05f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 3:
    {
      set_range(0.1f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 4:
    {
      set_range(0.25f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 5:
    {
      set_range(0.5f);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 6:
    {
      set_range(1.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 7:
    {
      set_range(2.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 8:
    {
      set_range(5.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 9:
    {
      set_range(7.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 10:
    {
      set_range(10.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 11:
    {
      set_range(15.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 12:
    {
      set_range(25.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 13:
    {
      set_range(35.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 14:
    {
      set_range(50.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 15:
    {
      set_range(75.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 16:
    {
      set_range(100.0f);
      calculate_lat_lon_to_point_scale();
      break;
    }
  }
}

ImVec2 ADSB_RANGE::ll_2_pt_scale()
{
  return LAT_LON_TO_POINT_SCALE;
}

void ADSB_RANGE::gps_display_current_location_toggle()
{
  if (GPS_POS_LAT_LON.x != 0.0f && GPS_POS_LAT_LON.y != 0.0f)
  {
    CENTER_ON_LOCATION = 1;
  }
  else
  {
    CENTER_ON_LOCATION = 0;
  }
}

bool ADSB_RANGE::gps_display_current_location()
{
  return CENTER_ON_LOCATION == 1;
}

void ADSB_RANGE::set_no_pos_lat_lon(ImVec2 Lat_Lon)
{
  NO_POS_LAT_LON = Lat_Lon;
}

void ADSB_RANGE::set_gps_pos_lat_lon(ImVec2 Lat_Lon)
{
  GPS_POS_LAT_LON = Lat_Lon;
}

void ADSB_RANGE::set_aircraft_pos_lat_lon(ImVec2 Lat_Lon)
{
  AIRCRAFT_POS_LAT_LON = Lat_Lon;
}

ImVec2 ADSB_RANGE::get_center_lat_lon()
{
  if (CENTER_ON_LOCATION == 1)
  {
    return GPS_POS_LAT_LON;
  }
  else if (CENTER_ON_LOCATION == 2)
  {
    return AIRCRAFT_POS_LAT_LON;
  }
  else 
  {
    return NO_POS_LAT_LON;
  }
}

ImVec2 ADSB_RANGE::get_gps_lat_lon()
{
  return GPS_POS_LAT_LON;
}

float ADSB_RANGE::range()
{
  return RANGE;
}

void ADSB_RANGE::set_range(float Range_Miles)
{
  RANGE = Range_Miles;
}

void ADSB_RANGE::zoom_in()
{ 
  if (ZOOM_LEVEL > 0)
  {
    ZOOM_LEVEL = ZOOM_LEVEL - 1;
    set_zoom_level();
  }
}

void ADSB_RANGE::zoom_out()
{
  if (ZOOM_LEVEL < 16)
  {
    ZOOM_LEVEL = ZOOM_LEVEL + 1;
    set_zoom_level();
  }
}

void ADSB_RANGE::draw(ImDrawList *Draw_List, system_data &sdSysData, ImVec4 Working_Area)
{
  if (ZOOM_LEVEL == -1)
  {
    RADIUS_CIRCLE_POINT_SIZE = Working_Area.w / 2.0f * 0.6f;

    ZOOM_LEVEL = 13;     // Default start zoom level
    set_zoom_level();
  }

  ImVec2 center = point_position_center(Working_Area);
  
  Draw_List->AddNgon(center, RADIUS_CIRCLE_POINT_SIZE, ImU32(sdSysData.COLOR_SELECT.neo_color(PROPS.COLOR).STANDARD_V.color()), 32, 1.5f);
  Draw_List->AddNgon(center, RADIUS_CIRCLE_POINT_SIZE * 2.0f, ImU32(sdSysData.COLOR_SELECT.neo_color(PROPS.COLOR).STANDARD_V.color()), 32, 1.5f);
  
  ImGui::SetCursorScreenPos(ImVec2(center.x, center.y - RADIUS_CIRCLE_POINT_SIZE + 5));
  
  // Text Range
  //ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color(PROPS.COLOR).STANDARD_V));

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color(PROPS.COLOR).STANDARD_V.color(sdSysData.PROGRAM_TIME.current_frame_time())));

  ImGui::Text("%.2f mi", RANGE);
  ImGui::PopStyleColor();
}

void ADSB_RANGE::draw_info()
{
  ImGui::Text("LAT: %f", get_center_lat_lon().x);
  ImGui::Text("LON: %f", get_center_lat_lon().y);
  ImGui::Text("RNG: %.2f mi", RANGE);

  // test
  /*
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

void ADSB_MAP::add_landmark(ImVec2 Lat_Lon, string Display_Name, int Type)
{
  MAP_MARKER new_landmark;

  new_landmark.LAT_LON = Lat_Lon;
  new_landmark.DISPLAY_NAME = Display_Name;
  new_landmark.TYPE = Type;

  LANDMARKS.push_back(new_landmark);
}

void ADSB_MAP::create(system_data &sdSysData)
{
  // Prepare Map Orientation with Impact Resistance
  MAP_HEADING_DEGREES.set_size(30);

  // Prepare Compass
  CURRENT_POSITION_COMPASS.set_size(32, (15 / 2));
  // set at frame rate for slow and jitter size for fast.

  RANGE_INDICATOR.PROPS.COLOR = sdSysData.COLOR_SELECT.orange();
  RANGE_INDICATOR.set_range(25.0f);

  if (GPS_ALTITUDE_COLOR_SCALE.active() == false)
  {
    // This is not representing altitude. It is representing speed in mph.
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(10.0f, sdSysData.COLOR_SELECT.red());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(15.0f, sdSysData.COLOR_SELECT.yellow());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(35.0f, sdSysData.COLOR_SELECT.orange());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(45.0f, sdSysData.COLOR_SELECT.green());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(55.0f, sdSysData.COLOR_SELECT.blue());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(75.0f, sdSysData.COLOR_SELECT.purple());
    GPS_ALTITUDE_COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.white());
  }

  // Create vars if first run
  if (ALTITUDE_COLOR_SCALE.active() == false)
  {
    ALTITUDE_COLOR_SCALE.add_color_value_pair(500.0f, sdSysData.COLOR_SELECT.red());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(1000.0f, sdSysData.COLOR_SELECT.yellow());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(2500.0f, sdSysData.COLOR_SELECT.orange());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(12000.0f, sdSysData.COLOR_SELECT.green());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(45000.0f, sdSysData.COLOR_SELECT.blue());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(65000.0f, sdSysData.COLOR_SELECT.purple());
    ALTITUDE_COLOR_SCALE.add_color_value_pair(100000.0f, sdSysData.COLOR_SELECT.white());
  }

  // KLFT - Lafayette Regional/Paul Fournet Field Airport
  // Coordinates: 
  //                                LATITUDE      LONGITUDE
  // Degrees Decimal Minutes (DDM): N30°12.30'  / W91°59.27'
  // Decimal Degrees (DD):          30.205      / -91.987833

  RANGE_INDICATOR.set_no_pos_lat_lon(ImVec2(30.205f, -91.987833f));
  RANGE_INDICATOR.CENTER_ON_LOCATION = 0;

  // Add Landmarks
  MAP_MARKER tmp_map_marker;

  // add_landmark(ImVec2(30.4663333333f, -92.4238333333f), "4R7", 0);  //
  //tmp_map_marker.clear();
  //tmp_map_marker.LAT_LON = ImVec2( 0.0f, 0.0f);
  //tmp_map_marker.DISPLAY_NAME = "";
  //tmp_map_marker.LONG_NAME = "";
  //tmp_map_marker.TYPE = 1;

  // add_landmark(ImVec2(30.205f, -91.987833f), "LFY", 0);             // KLFT - Lafayette Regional/Paul Fournet Field Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.205f, -91.987833f);
  tmp_map_marker.DISPLAY_NAME = "LFY";
  tmp_map_marker.LONG_NAME = "Lafayette Regional/Paul Fournet Field Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(110.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(290.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.4663333333f, -92.4238333333f), "4R7", 0);  //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.4663333333f, -92.4238333333f);
  tmp_map_marker.DISPLAY_NAME = "4R7";
  tmp_map_marker.LONG_NAME = "Eunice Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(160.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(340.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2426666667f, -92.6735f), "3R7", 0);        //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.2426666667f, -92.6735f);
  tmp_map_marker.DISPLAY_NAME = "3R7";
  tmp_map_marker.LONG_NAME = "Jennings Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(80.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(260.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.533f, -91.1498333333f), "BTR", 0);         //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.533f, -91.1498333333f);
  tmp_map_marker.DISPLAY_NAME = "BTR";
  tmp_map_marker.LONG_NAME = "Baton Rouge Metro, Ryan";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0378333333f, -91.8838333333f), "ARA", 0);  //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0378333333f, -91.8838333333f);
  tmp_map_marker.DISPLAY_NAME = "ARA";
  tmp_map_marker.LONG_NAME = "Acadiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2418333333f, -92.8306666667f), "6R1", 0);  // 6R1 - Welsh Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.2418333333f, -92.8306666667f);
  tmp_map_marker.DISPLAY_NAME = "6R1";
  tmp_map_marker.LONG_NAME = "Welsh Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(70.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(250.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(90.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(270.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.126f, -93.2235f), "LCH", 0);               // LCH - Lake Charles Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.126f, -93.2235f);
  tmp_map_marker.DISPLAY_NAME = "LCH";
  tmp_map_marker.LONG_NAME = "Lake Charles Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(150.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(330.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(50.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(230.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.7505f, -92.6885f), "ARA", 0);              // ACP - Allen Parish Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.7505f, -92.6885f);
  tmp_map_marker.DISPLAY_NAME = "ACP";
  tmp_map_marker.LONG_NAME = "Allen Parish Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.9566666667f, -92.2341666667f), "2R6", 0);  // 2R6 - Bunkie Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.9566666667f, -92.2341666667f);
  tmp_map_marker.DISPLAY_NAME = "2R6";
  tmp_map_marker.LONG_NAME = "Bunkie Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.3273333333f, -92.5485f), "AEX", 0);        // AEX - Alexandria International Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(31.3273333333f, -92.5485f);
  tmp_map_marker.DISPLAY_NAME = "AEX";
  tmp_map_marker.LONG_NAME = "Alexandria International Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(140.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(320.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.7183333333f, -91.4786666667f), "HZR", 0);  // HZR - False River Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.7183333333f, -91.4786666667f);
  tmp_map_marker.DISPLAY_NAME = "HZR";
  tmp_map_marker.LONG_NAME = "False River Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.1713333333f, -90.9403333333f), "REG", 0);  // REG - Louisiana Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.1713333333f, -90.9403333333f);
  tmp_map_marker.DISPLAY_NAME = "REG";
  tmp_map_marker.LONG_NAME = "Louisiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.5216666667f, -90.4183333333f), "HDC", 0);  // HDC - Hammond Northshore Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.5216666667f, -90.4183333333f);
  tmp_map_marker.DISPLAY_NAME = "HDC";
  tmp_map_marker.LONG_NAME = "Hammond Northshore Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.445f, -89.9888333333f), "L31", 0);         // L31 - St Tammany Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.445f, -89.9888333333f);
  tmp_map_marker.DISPLAY_NAME = "L31";
  tmp_map_marker.LONG_NAME = "St Tammany Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.3463333333f, -89.8208333333f), "ASD", 0);  // ASD - Slidell Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.3463333333f, -89.8208333333f);
  tmp_map_marker.DISPLAY_NAME = "ASD";
  tmp_map_marker.LONG_NAME = "Slidell Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.4875f, -89.6511666667f), "MJD", 0);        // MJD - Picayune Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.4875f, -89.6511666667f);
  tmp_map_marker.DISPLAY_NAME = "MJD";
  tmp_map_marker.LONG_NAME = "Picayune Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0425f, -90.0281666667f), "NEW", 0);        // NEW - Lakefront Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0425f, -90.0281666667f);
  tmp_map_marker.DISPLAY_NAME = "NEW";
  tmp_map_marker.LONG_NAME = "Lakefront Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(29.8271666667f, -90.0266666667f), "NBG", 0);  // NBG - New Orleans NAS Jrb (Alvin Callender Field) Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.8271666667f, -90.0266666667f);
  tmp_map_marker.DISPLAY_NAME = "NBG";
  tmp_map_marker.LONG_NAME = "New Orleans NAS Jrb (Alvin Callender Field) Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(140.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(320.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(29.9933333333f, -90.259f), "MSY", 0);         // MSY - Louis Armstrong New Orleans International Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.9933333333f, -90.259f);
  tmp_map_marker.DISPLAY_NAME = "MSY";
  tmp_map_marker.LONG_NAME = "Louis Armstrong New Orleans International Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(110.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(290.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(20.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(200.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0875f, -90.5828333333f), "APS", 0);        // APS - Port of South Louisiana Exec Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0875f, -90.5828333333f);
  tmp_map_marker.DISPLAY_NAME = "APS";
  tmp_map_marker.LONG_NAME = "Port of South Louisiana Exec Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.1785f, -90.4718333333f), "MCB", 0);        // MCB - Mc Comb/Pike County/John E Lewis Field Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(31.1785f, -90.4718333333f);
  tmp_map_marker.DISPLAY_NAME = "MCB";
  tmp_map_marker.LONG_NAME = "Mc Comb/Pike County/John E Lewis Field Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);
  
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.9821666667f, -92.0871666667f);
  tmp_map_marker.DISPLAY_NAME = "IYA";
  tmp_map_marker.LONG_NAME = "Abbeville Chris Crusta Memorial Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(159.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(339.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // Regions

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.171892f, -92.021630f);
  tmp_map_marker.DISPLAY_NAME = "LAFAYETTE";
  tmp_map_marker.LONG_NAME = "Lafayette";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.256323f, -91.990737f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.209173f, -91.975376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.192592f, -91.987432f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.160652f, -91.951085f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.108378f, -91.920498f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.069544f, -91.979683f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.073432f, -92.023356f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.100806f, -92.026423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.137705f, -92.069318f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.131181f, -92.105622f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.189960f, -92.081542f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.191015f, -92.113194f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.262811f, -92.133407f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.260867f, -92.089104f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.249578f, -92.091143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247825f, -92.046105f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.258446f, -92.021630f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.465774f, -91.155707f);
  tmp_map_marker.DISPLAY_NAME = "BATON ROUGE";
  tmp_map_marker.LONG_NAME = "Baton Rouge";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.605345f, -91.149123f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.532499f, -91.095232f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.526884f, -91.122601f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.504312f, -91.042169f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.471935f, -91.033231f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.462739f, -90.995275f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.449013f, -90.992849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.436054f, -91.037770f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418108f, -91.036161f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.409920f, -90.971694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.411330f, -90.972019f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.348683f, -90.981922f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.350909f, -91.144685f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.409352f, -91.198959f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.428363f, -91.194472f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.543076f, -91.199386f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.605002f, -91.185916f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.218697f, -92.375267f);
  tmp_map_marker.DISPLAY_NAME = "CROWLEY";
  tmp_map_marker.LONG_NAME = "Crowley";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.240594f, -92.362782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.231363f, -92.360189f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.230868f, -92.371161f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.227915f, -92.370526f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.227074f, -92.362757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.219310f, -92.358634f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.213647f, -92.359365f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.213433f, -92.350869f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.205640f, -92.350325f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.204999f, -92.356780f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.201381f, -92.357070f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.201009f, -92.362556f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.197011f, -92.362773f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.197899f, -92.370694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.190975f, -92.371303f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.191711f, -92.380043f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.200629f, -92.379919f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.205035f, -92.383075f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.199896f, -92.393879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.214584f, -92.403712f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.222032f, -92.386592f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235882f, -92.390233f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235987f, -92.395576f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.243646f, -92.393876f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.243016f, -92.382460f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.241127f, -92.382946f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.232104f, -92.272845f);
  tmp_map_marker.DISPLAY_NAME = "RAYNE";
  tmp_map_marker.LONG_NAME = "Rayne";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.263934f, -92.263041f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.253550f, -92.261335f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.252656f, -92.255841f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.242865f, -92.254478f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.242267f, -92.248640f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235740f, -92.247274f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235451f, -92.256546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.225365f, -92.256557f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.224185f, -92.264798f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.214397f, -92.267211f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.225086f, -92.280935f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.253266f, -92.279538f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.222796f, -92.681546f);
  tmp_map_marker.DISPLAY_NAME = "JENNINGS";
  tmp_map_marker.LONG_NAME = "Jennings";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246112f, -92.635782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.199958f, -92.635548f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.199327f, -92.684337f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246110f, -92.684109f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.216662f, -93.242004f);
  tmp_map_marker.DISPLAY_NAME = "LAKE CHARLES";
  tmp_map_marker.LONG_NAME = "Lake Charles";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.264687f, -93.174817f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.217717f, -93.140827f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.147629f, -93.242207f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.173839f, -93.291604f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.533170f, -92.101464f);
  tmp_map_marker.DISPLAY_NAME = "OPELOUSAS";
  tmp_map_marker.LONG_NAME = "Opelousas";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.552350f, -92.066830f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.542738f, -92.065975f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.527506f, -92.068727f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.519371f, -92.063407f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.506949f, -92.067187f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.497337f, -92.068735f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.495411f, -92.056721f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.488609f, -92.060156f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.494530f, -92.088988f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.509762f, -92.085210f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.513311f, -92.102376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.532536f, -92.107356f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.548952f, -92.098257f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.551170f, -92.090529f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.550874f, -92.086064f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.557085f, -92.083831f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(31.295272f, -92.485128f);
  tmp_map_marker.DISPLAY_NAME = "ALEXANDRIA";
  tmp_map_marker.LONG_NAME = "Alexandria";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.336785f, -92.457796f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.296117f, -92.423285f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.230499f, -92.506281f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.250400f, -92.531065f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.330122f, -92.500070f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.489848f, -92.440667f);
  tmp_map_marker.DISPLAY_NAME = "EUNICE";
  tmp_map_marker.LONG_NAME = "Eunice";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.509657f, -92.401657f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.496970f, -92.386068f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.480553f, -92.402812f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.480797f, -92.436296f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.506169f, -92.440347f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.982557f, -90.168530f);
  tmp_map_marker.DISPLAY_NAME = "NEW ORLEANS";
  tmp_map_marker.LONG_NAME = "New Orleans";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.076122f, -89.943296f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.060643f, -89.938536f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.079633f, -89.918518f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.029114f, -89.868705f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.980538f, -89.945799f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.994771f, -89.974354f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.980730f, -90.016086f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.942213f, -89.909177f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.888317f, -89.876726f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.859412f, -89.907073f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.880833f, -89.965787f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.786873f, -90.019137f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.818193f, -90.096541f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.848682f, -90.131552f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.869876f, -90.115191f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.899694f, -90.183680f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.882054f, -90.223685f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.910337f, -90.212227f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.942214f, -90.270960f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.897669f, -90.377874f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.853799f, -90.412114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.771734f, -90.408804f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.811301f, -90.475680f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.851625f, -90.475728f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.896247f, -90.399902f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.953521f, -90.434220f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.012152f, -90.529841f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.971273f, -90.706125f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.967735f, -90.729782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.005187f, -90.742850f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.033462f, -90.695519f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.119668f, -90.650582f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.080552f, -90.435780f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.063568f, -90.425206f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.007084f, -90.454750f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.000685f, -90.435989f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.025353f, -90.403260f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.976339f, -90.313589f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.009468f, -90.282421f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.048329f, -90.275722f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.022659f, -90.167189f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.037946f, -90.042940f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.272281f, -89.788092f);
  tmp_map_marker.DISPLAY_NAME = "SLIDEL";
  tmp_map_marker.LONG_NAME = "Slidel";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.332989f, -89.753014f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.277625f, -89.701522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.215956f, -89.801522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.223910f, -89.823857f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.327115f, -89.766132f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.374954f, -90.094526f);
  tmp_map_marker.DISPLAY_NAME = "MANDEVILLE";
  tmp_map_marker.LONG_NAME = "Mandeville";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.423856f, -90.075888f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.373179f, -90.019071f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.366490f, -90.103757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418023f, -90.104268f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.443740f, -90.462038f);
  tmp_map_marker.DISPLAY_NAME = "PONCHATOULA";
  tmp_map_marker.LONG_NAME = "Ponchatoula";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.467505f, -90.439530f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.431609f, -90.418708f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.420697f, -90.449317f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.460811f, -90.469741f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.973729f, -92.145558f);
  tmp_map_marker.DISPLAY_NAME = "ABBEVILLE";
  tmp_map_marker.LONG_NAME = "Abbeville";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.983607f, -92.085836f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.964678f, -92.078805f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.964678f, -92.078805f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.969312f, -92.106233f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.963425f, -92.113759f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.961744f, -92.131721f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.957327f, -92.132449f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.951439f, -92.138516f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.951646f, -92.160117f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.956692f, -92.162303f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.965314f, -92.160849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.967629f, -92.152354f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.976252f, -92.149443f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.981087f, -92.155999f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.992654f, -92.152603f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.993286f, -92.144834f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.990553f, -92.122010f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.987188f, -92.119097f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.986346f, -92.110114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.989500f, -92.105258f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.975409f, -92.097735f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.975617f, -92.090938f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.981925f, -92.088265f));
  LANDMARKS.push_back(tmp_map_marker);

  // Interstates

  // I-10
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-10";
  tmp_map_marker.LONG_NAME = "Interstate 10";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.121233f, -93.747711f)); // Orange Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.118660f, -93.731668f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.118658f, -93.724240f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.118658f, -93.724240f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.175864f, -93.587440f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.185467f, -93.565849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.215613f, -93.410769f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.215991f, -93.358877f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.216338f, -93.324832f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.237370f, -93.278771f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.237407f, -93.225437f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235950f, -93.217031f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.236660f, -93.191596f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246475f, -93.180497f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.243948f, -93.134131f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247503f, -93.113007f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247699f, -93.036777f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.251020f, -92.996948f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246471f, -92.970745f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246818f, -92.861474f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246684f, -92.821686f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247497f, -92.753457f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.248122f, -92.747143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247719f, -92.726514f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246667f, -92.719473f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246673f, -92.645429f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.236373f, -92.607313f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.236405f, -92.554399f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.231784f, -92.532307f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.231128f, -92.484475f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.236344f, -92.451684f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.235553f, -92.339542f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.251878f, -92.284636f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.252538f, -92.247032f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247901f, -92.214248f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.247521f, -92.125423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.246852f, -92.052080f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.268464f, -91.992376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.282270f, -91.947423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.291112f, -91.928536f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.308086f, -91.869546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.317895f, -91.817319f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.324020f, -91.790897f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.343731f, -91.718318f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.367349f, -91.630403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.403197f, -91.499818f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.410717f, -91.471644f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.413424f, -91.453426f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418706f, -91.433795f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.428949f, -91.401501f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.450937f, -91.317753f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.453631f, -91.270167f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.441023f, -91.217734f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.439199f, -91.178414f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.427473f, -91.169442f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.424321f, -91.153667f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418494f, -91.114572f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.393500f, -91.083705f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.372317f, -91.050757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.344477f, -91.028084f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.314689f, -90.998398f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.308020f, -90.994626f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.301352f, -90.992741f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.281934f, -90.989632f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.269776f, -90.985861f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.259397f, -90.980202f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.232552f, -90.965092f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.205399f, -90.942625f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.200501f, -90.936965f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.171885f, -90.876563f));  // Sorrento Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.169776f, -90.870422f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.155533f, -90.818317f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.153301f, -90.803879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.149051f, -90.777177f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.144551f, -90.761590f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.127205f, -90.704998f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.124453f, -90.692873f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.123574f, -90.681469f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.111333f, -90.546501f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.107418f, -90.504062f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.105107f, -90.495979f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.088491f, -90.441849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.077938f, -90.405111f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.056330f, -90.372632f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.015148f, -90.311917f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.010158f, -90.303897f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.006656f, -90.292133f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.006533f, -90.282173f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.008154f, -90.261097f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.009471f, -90.242837f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.007468f, -90.212306f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.005217f, -90.207976f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.001653f, -90.202275f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.000151f, -90.197656f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.999345f, -90.187839f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.997530f, -90.155504f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.995716f, -90.115806f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.992827f, -90.082257f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.993013f, -90.077622f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.990732f, -90.070403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.991373f, -90.058692f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.991856f, -90.049852f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(29.995254f, -90.046437f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.001193f, -90.039516f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.004085f, -90.029383f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.007079f, -90.016939f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.008918f, -90.013660f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.012019f, -90.013052f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.022268f, -90.014030f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.025316f, -90.012755f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.026787f, -90.007717f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.028660f, -89.997476f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.029942f, -89.993546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.035774f, -89.979205f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.038756f, -89.973005f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.060082f, -89.939298f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.062733f, -89.935115f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.106713f, -89.896114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.150238f, -89.859165f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.211706f, -89.793770f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.221652f, -89.783806f));
  LANDMARKS.push_back(tmp_map_marker);

  // I - 49
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-49";
  tmp_map_marker.LONG_NAME = "Interstate 49";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.324306f, -92.462141f));	// Pineville North Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.321225f, -92.459222f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.312571f, -92.453902f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.303182f, -92.447039f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.295261f, -92.444295f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.285286f, -92.441553f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.258583f, -92.435530f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.249334f, -92.429525f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.243317f, -92.429528f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.235790f, -92.456694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.229980f, -92.463739f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.197596f, -92.474676f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.184723f, -92.471040f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.163748f, -92.459401f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.154385f, -92.454271f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.140048f, -92.451123f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.124460f, -92.442150f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.099112f, -92.445563f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.087890f, -92.442900f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.071040f, -92.433390f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.034219f, -92.412049f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.027350f, -92.403074f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(31.014641f, -92.376143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.987392f, -92.308129f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.984683f, -92.303761f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.896798f, -92.227066f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.838660f, -92.229249f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.807382f, -92.215206f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.780294f, -92.189657f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.767177f, -92.176041f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.728954f, -92.125567f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.725401f, -92.120229f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.714113f, -92.102761f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.694104f, -92.082098f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.686167f, -92.078221f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.665280f, -92.070234f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.640408f, -92.048659f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.610766f, -92.046631f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.584225f, -92.049337f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.545556f, -92.053919f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.537408f, -92.063645f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.471334f, -92.078038f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.434281f, -92.068025f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.422548f, -92.056140f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.416266f, -92.054448f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.395754f, -92.058597f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.336736f, -92.039325f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.272556f, -92.018741f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.258295f, -92.015600f));
  LANDMARKS.push_back(tmp_map_marker);
  
  // I - 12
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-12";
  tmp_map_marker.LONG_NAME = "Interstate 12";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418484f, -91.115550f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418410f, -91.109713f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.417448f, -91.099328f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.417151f, -91.093577f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.418113f, -91.088341f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.423809f, -91.075121f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.427434f, -91.066879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.430243f, -91.056403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.438243f, -91.024397f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.447637f, -90.986027f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.455112f, -90.956419f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.464700f, -90.917742f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.466899f, -90.886457f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.470194f, -90.862571f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.473320f, -90.838689f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.473583f, -90.793522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.474024f, -90.785051f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.474462f, -90.729371f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.475916f, -90.710745f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.474635f, -90.666558f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.475020f, -90.615462f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.479254f, -90.563930f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.478995f, -90.489157f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.478890f, -90.414238f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.478959f, -90.357259f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.482305f, -90.312589f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.480147f, -90.245230f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.475742f, -90.228390f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.466162f, -90.205832f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.446896f, -90.140711f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(ImVec2(30.429031f, -90.084776f));	// Chinchuba Exit
  LANDMARKS.push_back(tmp_map_marker);

  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
}

void ADSB_MAP::draw(system_data &sdSysData)
{
  // ADSB Draw List
  ImDrawList* draw_list_map = ImGui::GetWindowDrawList();

  // working area, xy start position; zw, size.
  //    or
  //  xy is offset
  //  zw is screen size.
  ImVec4 working_area;

  working_area.x = ImGui::GetCursorScreenPos().x;
  working_area.y = ImGui::GetCursorScreenPos().y;
  working_area.z = ImGui::GetContentRegionAvail().x;
  working_area.w = ImGui::GetContentRegionAvail().y;

  // Gather some frequent variables
  bool active_gps     = sdSysData.GPS_SYSTEM.active(sdSysData.PROGRAM_TIME.current_frame_time());
  bool active_compass = sdSysData.COMMS_COMPASS.connected();

  // -------------------------------------------------------------------------------------

  // Store some map info regarding positions
  // Store current gps location
  {
    if (sdSysData.GPS_SYSTEM.current_position().VALID_COORDS && sdSysData.GPS_SYSTEM.current_position().CHANGED)
    {
      // Check and store GPS Current Location
      RANGE_INDICATOR.set_gps_pos_lat_lon(ImVec2(sdSysData.GPS_SYSTEM.current_position().LATITUDE, 
                                                  sdSysData.GPS_SYSTEM.current_position().LONGITUDE));

      // Mark Changes as seen.
      sdSysData.GPS_SYSTEM.current_position_change_acknowleged();
    }
  }

  // Store tracked ADSB Aircraft
  {
    if (sdSysData.AIRCRAFT_COORD.is_active())
    {
      // Check and store AIRCRAFT Current Location
      RANGE_INDICATOR.set_aircraft_pos_lat_lon(ImVec2(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.POSITION.LATITUDE.get_float_value(), 
                                                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT.POSITION.LONGITUDE.get_float_value()));
    }
  }
  
  // No pos has no store point other than direct
  {
    // No Pos
  }

  // -------------------------------------------------------------------------------------

  // Before drawing the map, get map heading in degrees from sources Compass or GPS or both
  float map_heading_degrees = 0.0f;

  if (!NORTH_UP)
  {
    if (RANGE_INDICATOR.CENTER_ON_LOCATION == 1)
    {
      // Map direction for GPS
      if (active_compass && active_gps)
      {
        if (sdSysData.GPS_SYSTEM.current_position().SPEED.val_mph() > 15.0f)
        {
          MAP_HEADING_DEGREES.set_value(sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING);
        }
        else
        {
          MAP_HEADING_DEGREES.set_value(sdSysData.COMMS_COMPASS.bearing());
        }
      }
      else if (active_compass)
      {
        MAP_HEADING_DEGREES.set_value(sdSysData.COMMS_COMPASS.bearing());
      }
      else if (active_gps)
      {
        MAP_HEADING_DEGREES.set_value(sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING);
      }
      else
      {
        MAP_HEADING_DEGREES.set_value(0.0f);
      }
    }
    else if (RANGE_INDICATOR.CENTER_ON_LOCATION == 2)
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

  map_heading_degrees = MAP_HEADING_DEGREES.value_no_roll(360.0f);

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
  ImGui::SetCursorScreenPos(ImVec2(working_area.x, working_area.y));

  if (SHOW_BUTTONS == false)
  {
    if (ImGui::InvisibleButton("Hide Buttons", ImVec2(working_area.z, working_area.w)))
    {
      SHOW_BUTTONS = !SHOW_BUTTONS;

      if (SHOW_BUTTONS)
      {
        SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      }
    }
  }

  // All Text Below Here

  //Put cursor in upper left
  ImGui::SetCursorScreenPos(ImVec2(working_area.x, working_area.y));

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_red().TEXT.color()));

  ImGui::Text("WARNING: Information may be considered CONFIDENTIAL");
  ImGui::PopStyleColor();
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_grey().TEXT.color()));
  ImGui::Text("TIME: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TIME_OF_SIGNAL.c_str());
  ImGui::Text("COUNT: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_COUNT.c_str());
  ImGui::Text("  POS: %s", sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_AIRCRAFT.c_str());

  // Maps and Other Information
  RANGE_INDICATOR.draw_info();

  // GPS Information
  if (active_gps)
  {
    ImGui::NewLine();

    ImGui::Text("GPS POSITION");
    ImGui::Text("   SPEED: %.1f", sdSysData.GPS_SYSTEM.current_position().SPEED.val_mph());
    ImGui::Text("ALTITUDE: %.1f", sdSysData.GPS_SYSTEM.current_position().ALTITUDE.feet_val());
    ImGui::Text(" HEADING: %.1f", sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING);

    if (SHOW_BUTTONS)
    {
      ImGui::Text("P:%.1f H:%.1f V:%.1f", sdSysData.GPS_SYSTEM.pdop(), sdSysData.GPS_SYSTEM.hdop(), sdSysData.GPS_SYSTEM.vdop());
    }
  }

  // Compass Information
  if (active_compass)
  {
    ImGui::NewLine();

    ImGui::Text("COMPASS");

    ImGui::Text("BEARING: %.1f", sdSysData.COMMS_COMPASS.bearing());

    if (sdSysData.COMMS_COMPASS.calibrate_on())
    {
      for (int quads = 1; quads < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS.size(); quads++)
      {
        ImGui::Text("CAL PT %d | %d | %.0f, %.0f: %.3f", quads,
                                                    sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quads].OFFSET_POINT_LIST.size(), 
                                                    sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quads].OFFSET_POINT.X, 
                                                    sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quads].OFFSET_POINT.Y, 
                                                    sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quads].VARIANCE);
      }

      ImGui::Text("CAL OFFS: %.0f, %.0f  VARI: %d %.3f  OFFSET: %.0f", sdSysData.COMMS_COMPASS.calibration_offset().X, sdSysData.COMMS_COMPASS.calibration_offset().Y, 
                                                                        sdSysData.COMMS_COMPASS.calibration_simple(), sdSysData.COMMS_COMPASS.calibration_variance(), 
                                                                        sdSysData.COMMS_COMPASS.bearing_known_offset());
    }
  }

  ImGui::PopStyleColor();

  // All Buttons
  if (SHOW_BUTTONS)
  {
    // Range and Location Buttons
    
    // Zoom Out
    ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                      working_area.y + working_area.w - (2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f))));

    if (BC_PLUS.button_color(sdSysData, "+", sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      RANGE_INDICATOR.zoom_out();
    }

    // North Up
    ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - 2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                      working_area.y + working_area.w - (1.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f))));

    if (BTC_NORTH_UP.button_toggle_color(sdSysData, "NORTH\nUP", "DIR\nUP", NORTH_UP,
                                    sdSysData.COLOR_SELECT.green(), sdSysData.COLOR_SELECT.blue(), 
                                    sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      NORTH_UP = !NORTH_UP;
    }

    // Zoom In
    ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                      working_area.y + working_area.w - (1.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y + 5.0f))));

    if (BC_MINUS.button_color(sdSysData, "-", sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
    {
      SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      RANGE_INDICATOR.zoom_in();
    }

    // Current Location Toggle
    ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                      working_area.y));

    if (active_gps)
    {
      if (BTC_CENT.button_toggle_color(sdSysData, "CENT\n(On)", "CENT\n(Off)", RANGE_INDICATOR.gps_display_current_location(),
                                      sdSysData.COLOR_SELECT.green(), sdSysData.COLOR_SELECT.blue(), 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SHOW_BUTTONS_TIMER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
        RANGE_INDICATOR.gps_display_current_location_toggle();
      }
    }
    else
    {
      button_simple_enabled(sdSysData, "GPS\n", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM);
    }

    // Compass Calibration
    if (active_compass)
    {
      ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                    working_area.y + sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f));

      if (BTC_CALI.button_toggle_color(sdSysData, "CALI\n(On)", "CALI\n(Off)", sdSysData.COMMS_COMPASS.calibrate_on(),
                                      sdSysData.COLOR_SELECT.green(), sdSysData.COLOR_SELECT.blue(), 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        sdSysData.COMMS_COMPASS.calibrate_toggle();
      }

      if (sdSysData.COMMS_COMPASS.calibrate_on())
      {
        ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - 2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                working_area.y + 2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f)));
        if (BTC_LOCK.button_toggle_color(sdSysData, "LOCK\n(On)", "LOCK\n(Off)", sdSysData.COMMS_COMPASS.calibrate_lock_on(),
                                      sdSysData.COLOR_SELECT.green(), sdSysData.COLOR_SELECT.blue(), 
                                      sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          sdSysData.COMMS_COMPASS.calibrate_lock_toggle();
        }

        ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                working_area.y + 2.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f)));
        if (BC_BEAR_RESET.button_color(sdSysData, "BEAR\nRESET", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          if (active_gps)
          {
            sdSysData.COMMS_COMPASS.bearing_known_offset_calibration(sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING);
          }
          else
          {
            sdSysData.COMMS_COMPASS.bearing_known_offset_calibration(0.0f);
          }
        }

        ImGui::SetCursorScreenPos(ImVec2(working_area.x + working_area.z - (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f), 
                                working_area.y + 3.0f * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x + 5.0f)));
        if (BC_CALI_RESET.button_color(sdSysData, "CALI\nRESET", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          sdSysData.COMMS_COMPASS.calibrateion_reset();
        }
      }
    }
  }

  // All Text Above Here
  // -------------------------------------------------------------------------------------
  RANGE_INDICATOR.draw(draw_list_map, sdSysData, working_area);

  // Draw Compass Calibration
  if (active_compass)
  {
    if (sdSysData.COMMS_COMPASS.calibrate_on())
    {
      ImVec2 center = point_position_center(working_area);
      
      ImVec2 p1;
      ImVec2 p2;
      ImVec2 p3;
      ImVec2 p4;

      ImVec2 r1;
      ImVec2 r2;
      ImVec2 c1;
      ImVec2 c2;
      
      // level 0 and level 1

      for (int quad = 1; quad < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS.size(); quad++)
      {
        for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS.size(); pos++)
        {
          p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS[pos].X / 4.0f), 
                              center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS[pos].Y / 4.0f));

          draw_marker(draw_list_map, sdSysData, p1, sdSysData.COLOR_SELECT.orange());
        }

        for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED.size(); pos++)
        {
          p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED[pos].X / 4.0f), 
                              center.y + (sdSysData.COMMS_COMPASS.LEVEL_2.CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED[pos].Y / 4.0f));

          draw_marker(draw_list_map, sdSysData, p1, sdSysData.COLOR_SELECT.blue());
        }
      }

      // draw quad calibration
      if (sdSysData.COMMS_COMPASS.calibration_points_active_quad_overflow() == false)
      {
        for (int pos = 0; pos < (int)sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS.size(); pos++)
        {
          p1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS[pos].X / 4.0f), 
                              center.y + (sdSysData.COMMS_COMPASS.calibration_points_active_quad_data().DATA_POINTS[pos].Y / 4.0f));

          draw_marker(draw_list_map, sdSysData, p1, sdSysData.COLOR_SELECT.green());
        }
      }

      // draw outlining box
      r1 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_min_max_x().MIN_VALUE / 4.0f), 
                                      center.y + (sdSysData.COMMS_COMPASS.calibration_min_max_y().MIN_VALUE / 4.0f));
      r2 = ImVec2(center.x + (sdSysData.COMMS_COMPASS.calibration_min_max_x().MAX_VALUE / 4.0f), 
                                      center.y + (sdSysData.COMMS_COMPASS.calibration_min_max_y().MAX_VALUE / 4.0f));
      
      draw_box(draw_list_map, sdSysData, r1, r2, sdSysData.COLOR_SELECT.orange(), 2.0f);

      c1 = ImVec2(center.x + ((sdSysData.COMMS_COMPASS.calibration_min_max_x().MIN_VALUE - 
                                      sdSysData.COMMS_COMPASS.calibration_offset().X) / 4.0f), 
                          center.y + ((sdSysData.COMMS_COMPASS.calibration_min_max_y().MIN_VALUE - 
                                      sdSysData.COMMS_COMPASS.calibration_offset().Y) / 4.0f));
      c2 = ImVec2(center.x + ((sdSysData.COMMS_COMPASS.calibration_min_max_x().MAX_VALUE - 
                                      sdSysData.COMMS_COMPASS.calibration_offset().X) / 4.0f), 
                          center.y + ((sdSysData.COMMS_COMPASS.calibration_min_max_y().MAX_VALUE - 
                                      sdSysData.COMMS_COMPASS.calibration_offset().Y) / 4.0f));

      draw_box(draw_list_map, sdSysData, c1, c2, sdSysData.COLOR_SELECT.white(), 2.0f);

      draw_line(draw_list_map, sdSysData, r1, c1, sdSysData.COLOR_SELECT.white(), 2.0f);
      draw_line(draw_list_map, sdSysData, r2, c2, sdSysData.COLOR_SELECT.white(), 2.0f);

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

        draw_line(draw_list_map, sdSysData, p1, p2, sdSysData.COLOR_SELECT.green(), 2.0f);
        draw_line(draw_list_map, sdSysData, p2, p3, sdSysData.COLOR_SELECT.green(), 2.0f);
        draw_line(draw_list_map, sdSysData, p3, p4, sdSysData.COLOR_SELECT.green(), 2.0f);
        draw_line(draw_list_map, sdSysData, p4, p1, sdSysData.COLOR_SELECT.green(), 2.0f);

        draw_marker_filled(draw_list_map, sdSysData, p1, sdSysData.COLOR_SELECT.white());
        draw_marker_filled(draw_list_map, sdSysData, p2, sdSysData.COLOR_SELECT.white());
        draw_marker_filled(draw_list_map, sdSysData, p3, sdSysData.COLOR_SELECT.white());
        draw_marker_filled(draw_list_map, sdSysData, p4, sdSysData.COLOR_SELECT.white());
      }
    }
  }

  // Draw Landmarks
  for (int landmark = 0; landmark < (int)LANDMARKS.size(); landmark++)
  {
    LANDMARKS[landmark].draw(draw_list_map, sdSysData, working_area, RANGE_INDICATOR.ll_2_pt_scale(), RANGE_INDICATOR.get_center_lat_lon(), map_heading_degrees, RANGE_INDICATOR.range());
  }

  // Draw Current Position Marker
  if (active_gps && sdSysData.GPS_SYSTEM.current_position().VALID_COORDS)
  {
    // Draw track of GPS Position.
    if (sdSysData.GPS_SYSTEM.TRACK.TRACK_POINTS_DETAILED.size() > 1)
    {
      draw_track(draw_list_map, sdSysData, working_area, RANGE_INDICATOR.ll_2_pt_scale(), (int)RANGE_INDICATOR.range(), 2.5f, GPS_ALTITUDE_COLOR_SCALE, RANGE_INDICATOR.get_center_lat_lon(), map_heading_degrees, sdSysData.GPS_SYSTEM.TRACK);
    }

    bool draw = false;
    ImVec2 gps_pos = point_position_lat_lon(working_area, RANGE_INDICATOR.ll_2_pt_scale(), RANGE_INDICATOR.get_center_lat_lon(), RANGE_INDICATOR.get_gps_lat_lon(), map_heading_degrees, draw);

    // Draw point position compass
    if (draw)
    {
      if (RANGE_INDICATOR.CENTER_ON_LOCATION == 1)
      {
        // draw compass at center location
        CURRENT_POSITION_COMPASS.draw(draw_list_map, sdSysData, 2, gps_pos, working_area.w / 2.0f * 0.66f, true, sdSysData.GPS_SYSTEM.current_position().VALID_GPS_FIX, 
                            sdSysData.GPS_SYSTEM.current_position().VALID_TRACK, sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING, 
                            active_compass, sdSysData.COMMS_COMPASS.bearing(), !NORTH_UP, 
                            true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), map_heading_degrees);
      }
      else
      {
        // draw compass at gps pos
        CURRENT_POSITION_COMPASS.draw(draw_list_map, sdSysData, 1, gps_pos, 15.0f, true, sdSysData.GPS_SYSTEM.current_position().VALID_GPS_FIX, 
                            sdSysData.GPS_SYSTEM.current_position().VALID_TRACK, sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING, 
                            active_compass, sdSysData.COMMS_COMPASS.bearing(), !NORTH_UP, map_heading_degrees);
      }
  
      ImGui::SetCursorScreenPos(ImVec2(gps_pos.x - 20.0f, gps_pos.y - 20.0f));
      if (ImGui::InvisibleButton("GPS CURRENT POSITION COMPASS", ImVec2(40.0f, 40.0f)))
      {
        RANGE_INDICATOR.CENTER_ON_LOCATION = 1;
      }
    }
  }
  
  /*
  else if (active_compass)
  {
    CURRENT_POSITION_COMPASS.draw(draw_list_map, sdSysData, 2, ImVec2(working_area.x + working_area.z / 2.0f, working_area.y + working_area.w / 2.0f), 
                    working_area.w / 2.0f * 0.66f, true, sdSysData.GPS_SYSTEM.current_position().VALID_GPS_FIX, 
                    sdSysData.GPS_SYSTEM.current_position().VALID_TRACK, sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING, 
                    active_compass, sdSysData.COMMS_COMPASS.bearing(), !NORTH_UP, 
                    true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), map_heading_degrees);
  }
  */
  
  // -------------------------------------------------------------------------------------

  // Draw Aircraft
  for (int aircraft = 0; aircraft < (int)sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); aircraft ++)
  {
    if (sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].active())
    {
      AIRCRAFT tmp_clicked_aircraft = draw_aircraft_map_marker(sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft], 
                                                                draw_list_map, sdSysData, working_area, 
                                                                RANGE_INDICATOR.ll_2_pt_scale(), (int)RANGE_INDICATOR.range(), 
                                                                RANGE_INDICATOR.get_center_lat_lon(), map_heading_degrees, 
                                                                ALTITUDE_COLOR_SCALE);
      
      if (tmp_clicked_aircraft.HEX.get_str_value() != "")
      {
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT = tmp_clicked_aircraft;
        sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TRACKED_AIRCRAFT_HEX = tmp_clicked_aircraft.HEX.get_str_value();
        RANGE_INDICATOR.CENTER_ON_LOCATION = 2;
      }
    }
  }
}

void ADSB_SCREEN::adsb_table_draw(system_data &sdSysData)
{
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));

  ImGui::BeginChild("ADSB Display", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    ImGui::Text("Time: %s  Count: %s  Pos: %s  Furthest: %.2f", 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.TIME_OF_SIGNAL.c_str(), 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_COUNT.c_str(), 
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.POSITIONED_AIRCRAFT.c_str(),
                  sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.FURTHEST_AIRCRAFT_SINCE_START);
    
    //      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
    //                              ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

    if (ImGui::BeginTable("Aircraft Data", 12, sdSysData.SCREEN_DEFAULTS.flags_t))
    {
      {
        ImGui::TableSetupColumn(" ");
        ImGui::TableSetupColumn("FLIGHT");
        ImGui::TableSetupColumn("SQWK");
        ImGui::TableSetupColumn("G SPD");
        ImGui::TableSetupColumn("V RTE");
        ImGui::TableSetupColumn("ALT");
        ImGui::TableSetupColumn("ALT NAV");
        ImGui::TableSetupColumn("HDG");
        ImGui::TableSetupColumn("HDG NAV");
        ImGui::TableSetupColumn("DIST");
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

void ADSB_SCREEN::create(system_data &sdSysData)
{
  ADSB_MAP_DISPLAY.create(sdSysData);
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

  ImGui::BeginChild("ADSB Buttons", ImVec2(90, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (BTC_ADSB.button_toggle_color(sdSysData, "ADSB\n(On)", "ADSB\n(Off)", sdSysData.AIRCRAFT_COORD.is_active(), sdSysData.COLOR_SELECT.red(), sdSysData.COLOR_SELECT.grey(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

    if (BC_VIEW.button_color(sdSysData, "VIEW", sdSysData.COLOR_SELECT.orange(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      if (DISPLAY_TABLE)
      {
        DISPLAY_TABLE = false;
        DISPLAY_MAP = true;
      }
      else
      {
        DISPLAY_TABLE = true;
        DISPLAY_MAP = false;
      }
    }
  }
  ImGui::EndChild();

  ImGui::SameLine();
  
  if (DISPLAY_TABLE)
  {
    adsb_table_draw(sdSysData);
  }
  else if (DISPLAY_MAP)
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