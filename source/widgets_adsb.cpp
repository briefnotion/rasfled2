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

/*
void BLANK::display(const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {

  }
  ImGui::End();
}
*/

// ---------------------------------------------------------------------------------------
// Map Tools

float calculate_distance(float lat1, float lon1, float lat2, float lon2)
{
  // Convert latitude and longitude from degrees to radians
  lat1 = lat1 * M_PI / 180.0;
  lon1 = lon1 * M_PI / 180.0;
  lat2 = lat2 * M_PI / 180.0;
  lon2 = lon2 * M_PI / 180.0;

  // Calculate the differences between the coordinates
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  // Calculate the great circle distance using the haversine formula
  double a = pow(sin(dlat/2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon/2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  double R = 3963.191;  // Radius of the Earth in miles
  double distance = R * c;

  return (float)distance;
}


ImVec2 get_coords_x_miles_from_coords(float Latitude, float Longitude, float Distance_Miles, float Bearing_Degrees) 
{
  ImVec2 ret_coords;

  double EARTH_RADIUS_MILES = 3963.191;  // Radius of the Earth in miles

  double lat1Rad = (double)Latitude * M_PI / 180.0;
  double lon1Rad = (double)Longitude * M_PI / 180.0;
  double angularDistance = (double)Distance_Miles / EARTH_RADIUS_MILES;

  double lat2Rad = asin(sin(lat1Rad) * cos(angularDistance) + cos(lat1Rad) * sin(angularDistance) * cos((double)Bearing_Degrees * M_PI / 180.0));
  double lon2Rad = lon1Rad + atan2(sin(Bearing_Degrees * M_PI / 180.0) * sin(angularDistance) * cos(lat1Rad),
                                    cos(angularDistance) - sin(lat1Rad) * sin(lat2Rad));

  ret_coords.x = (float)(lat2Rad * 180.0 / M_PI);
  ret_coords.y = (float)(lon2Rad * 180.0 / M_PI);

  return ret_coords;
}

ImVec2 point_position_center(ImVec4 Working_Area)
{
  ImVec2 ret_center;
  ret_center.x = Working_Area.x + Working_Area.z / 2;
  ret_center.y = Working_Area.y + Working_Area.w / 2;
  return ret_center;
}

ImVec2 point_position_lat_lon(ImVec4 Working_Area, ImVec2 Scale, 
                                ImVec2 Lat_Lon_Center, ImVec2 Lat_Lon, bool &Drawn)
{
  ImVec2 center = point_position_center(Working_Area);

  ImVec2 ret_point;

  float lat_diff = Lat_Lon.x - Lat_Lon_Center.x;
  float lon_diff = Lat_Lon.y - Lat_Lon_Center.y;

  ret_point.y = -(lat_diff * Scale.x) + center.y;
  ret_point.x = (lon_diff * Scale.y) + center.x;

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

void draw_marker(system_data &sdSysData, ImVec2 Screen_Position, COLOR_COMBO &Color)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  draw_list->AddNgon(Screen_Position, 4, Color.STANDARD, 4, 1.5f);
}

void draw_airport_marker(system_data &sdSysData, ImVec2 Screen_Position, COLOR_COMBO &Color)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  draw_list->AddNgon(Screen_Position, 4, Color.STANDARD, 4, 1.5f);
}

void draw_point_marker(system_data &sdSysData, ImVec2 Screen_Position, ImColor Color, float Size)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  draw_list->AddNgonFilled(Screen_Position, Size, Color, 4);
  //draw_list->AddNgon(Screen_Position, Size, Color.TEXT, 6, 2.0f);
}

void draw_aircraft_marker(system_data &sdSysData, ImVec2 Screen_Position, COLOR_COMBO &Color, float Size)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  //draw_list->AddNgonFilled(Screen_Position, Size, Color.STANDARD, 6);
  draw_list->AddNgon(Screen_Position, Size, Color.TEXT, 6, 2.0f);
}

//void draw_arrow(ImDrawList* draw_list, ImVec2 p1, ImVec2 p2, ImU32 col, float thickness, float arrow_size, float direction)
void draw_aircraft_marker_direction(ImVec2 Screen_Position, COLOR_COMBO &Color, int Size, float Heading)
{
  float thickness = 2;

  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Adjust thhe degrees into heading.
  Heading = Heading + 90;

  // Convert direction from degrees to radians
  float rad = Heading * M_PI / 180.0f;

  // Calculate the arrow points
  ImVec2 arrow_p1 = ImVec2(Screen_Position.x + Size * cos(rad + M_PI / 8), Screen_Position.y + Size * sin(rad + M_PI / 8));
  ImVec2 arrow_p2 = ImVec2(Screen_Position.x + Size * cos(rad - M_PI / 8), Screen_Position.y + Size * sin(rad - M_PI / 8));

  // Draw the line and the arrow
  //draw_list->AddLine(p1, p2, col, thickness);
  draw_list->AddLine(Screen_Position, arrow_p1, Color.TEXT, thickness);
  draw_list->AddLine(Screen_Position, arrow_p2, Color.TEXT, thickness);
  draw_list->AddLine(arrow_p1, arrow_p2, Color.STANDARD, thickness);
}


// ---------------------------------------------------------------------------------------

void MAP_MARKER::clear()
{
  LAT_LON = ImVec2(0,0);
  DISPLAY_NAME = "";
  LONG_NAME = "";
  TYPE = 0;
  AIRPORT_LANDING_VECTORS.clear();
}

void MAP_MARKER::draw(system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, ImVec2 Center_Lat_Lon)
{
  bool draw = false;
  ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, LAT_LON, draw);

  if (draw)
  {
    // Possibly set colors here
    // ---

    // Draw Additional Vectors
    if (AIRPORT_LANDING_VECTORS.size() > 1)
      {
        bool on_screen = false;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        for(int vector = 0; vector < AIRPORT_LANDING_VECTORS.size(); vector++)
        {
          // 5 miles out.
          ImVec2 landing_vector_end = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, 
                                                    get_coords_x_miles_from_coords(LAT_LON.x, LAT_LON.y, 5.0f, AIRPORT_LANDING_VECTORS[vector]), 
                                                    on_screen);

          draw_list->AddLine(draw_position, landing_vector_end, 
                              sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.STANDARD, 2);
        }
      }

    // Draw Symbol
    switch (TYPE)
    {
      case 0: //  0 - Generic
      {
        draw_marker(sdSysData, draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.STANDARD));
        ImGui::Text("%s", DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }
      case 1: //  1 - Airport
      {
        draw_airport_marker(sdSysData, draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
        ImGui::SetCursorScreenPos(ImVec2(draw_position.x, draw_position.y + 5));

        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.STANDARD));
        ImGui::Text("%s", DISPLAY_NAME.c_str());
        ImGui::PopStyleColor();
        break;
      }
    }
  }
}

// ---------------------------------------------------------------------------------------

//  ADSB_Channel Classes

void ADSB_WIDGET::create()
// Prepare gadget to be drawn.  
//  Define PROP (properties before calling this routine)
//    Property Size and Position is necessary before calling create.
{
  /*
  // Create Gadget Window
  // Create Panel
  ADSB_PANEL.PROP.POSY = PROP.POSY;
  ADSB_PANEL.PROP.POSX = PROP.POSX;
  ADSB_PANEL.PROP.SIZEY = PROP.SIZEY;
  ADSB_PANEL.PROP.SIZEX = PROP.SIZEX;

  ADSB_PANEL.create();

  // Create Text Fields

  // Flight
  FLIGHT.PROP.POSY = 0;
  FLIGHT.PROP.POSX = 0;
  FLIGHT.PROP.COLORS_ON = true;
  FLIGHT.PROP.COLOR = COLOR_BLACK;
  FLIGHT.PROP.BCOLOR = COLOR_BLACK;
  FLIGHT.PROP.SIZEX = 8;
  FLIGHT.PROP.DONT_BLANK = true;
  FLIGHT.PROP.JUSTIFICATION_LEFT = true;
  FLIGHT.PROP.UPDATE_INDICATION = true;

  // Squawk
  SQUAWK.PROP.POSY = 0;
  SQUAWK.PROP.POSX = 16;
  SQUAWK.PROP.COLORS_ON = true;
  SQUAWK.PROP.COLOR = COLOR_BLACK;
  SQUAWK.PROP.BCOLOR = COLOR_BLACK;
  SQUAWK.PROP.SIZEX = 5;
  SQUAWK.PROP.DONT_BLANK = true;
  SQUAWK.PROP.JUSTIFICATION_RIGHT = true;
  SQUAWK.PROP.UPDATE_INDICATION = true;

  // Altitude
  ALTITUDE.PROP.POSY = 1;
  ALTITUDE.PROP.POSX = 14;
  ALTITUDE.PROP.SIZEX = 6;
  ALTITUDE.PROP.DONT_BLANK = true;
  ALTITUDE.PROP.JUSTIFICATION_RIGHT = true;
  ALTITUDE.PROP.UPDATE_INDICATION = true;

  // Altitude Strengh Direction
  ALTITUDE_DIRECION.PROP.SLICES = 5;
  ALTITUDE_DIRECION.PROP.TIME_SPAN = 5000;
  ALTITUDE_DIRECION.PROP.DIRECTION_NUTRAL_RANGE = .83;

  // Altitude Indicator
  ALTITUDE_IND.PROP.POSY = 1;
  ALTITUDE_IND.PROP.POSX = 20;
  ALTITUDE_IND.PROP.COLORS_ON = true;
  ALTITUDE_IND.PROP.COLOR = COLOR_BLACK;
  ALTITUDE_IND.PROP.BCOLOR = COLOR_BLACK;
  ALTITUDE_IND.PROP.SIZEX = 1;
  ALTITUDE_IND.PROP.SIZEY = 2;
  ALTITUDE_IND.PROP.DONT_BLANK = true;
  ALTITUDE_IND.PROP.JUSTIFICATION_LEFT = true;

  // Altitude Nav
  //NAV_ALTITUDE_MCP.PROP.LABEL = "nava";
  NAV_ALTITUDE_MCP.PROP.POSY = 2;
  NAV_ALTITUDE_MCP.PROP.POSX = 14;
  NAV_ALTITUDE_MCP.PROP.COLORS_ON = true;
  NAV_ALTITUDE_MCP.PROP.COLOR = COLOR_BLUE;
  NAV_ALTITUDE_MCP.PROP.BCOLOR = COLOR_BLACK;
  NAV_ALTITUDE_MCP.PROP.SIZEX = 6;
  NAV_ALTITUDE_MCP.PROP.DONT_BLANK = true;
  NAV_ALTITUDE_MCP.PROP.JUSTIFICATION_RIGHT = true;
  NAV_ALTITUDE_MCP.PROP.UPDATE_INDICATION = true;

  // Vertical Rate
  D_VERTICAL_RATE.PROP.POSY = 3;
  D_VERTICAL_RATE.PROP.POSX = 14;
  D_VERTICAL_RATE.PROP.SIZEX = 6;
  D_VERTICAL_RATE.PROP.DONT_BLANK = true;
  D_VERTICAL_RATE.PROP.JUSTIFICATION_RIGHT = true;
  D_VERTICAL_RATE.PROP.UPDATE_INDICATION = true;

  // Vertical Rate Indicator
  D_VERTICAL_RATE_IND.PROP.POSY = 3;
  D_VERTICAL_RATE_IND.PROP.POSX = 20;
  D_VERTICAL_RATE_IND.PROP.COLORS_ON = true;
  D_VERTICAL_RATE_IND.PROP.COLOR = COLOR_BLACK;
  D_VERTICAL_RATE_IND.PROP.BCOLOR = COLOR_BLACK;
  D_VERTICAL_RATE_IND.PROP.SIZEX = 1;
  D_VERTICAL_RATE_IND.PROP.SIZEY = 2;
  D_VERTICAL_RATE_IND.PROP.DONT_BLANK = true;
  D_VERTICAL_RATE_IND.PROP.JUSTIFICATION_LEFT = true;

  // Speed
  SPEED.PROP.POSY = 1;
  SPEED.PROP.POSX = 1;
  SPEED.PROP.SIZEX = 5;
  SPEED.PROP.DONT_BLANK = true;
  SPEED.PROP.JUSTIFICATION_LEFT = true;
  SPEED.PROP.UPDATE_INDICATION = true;

  // Altitude Strengh Direction
  SPEED_DIRECION.PROP.SLICES = 5;
  SPEED_DIRECION.PROP.TIME_SPAN = 5000;
  SPEED_DIRECION.PROP.DIRECTION_NUTRAL_RANGE = 1;

  // Speed Indicator
  SPEED_IND.PROP.POSY = 1;
  SPEED_IND.PROP.POSX = 0;
  SPEED_IND.PROP.COLORS_ON = true;
  SPEED_IND.PROP.COLOR = COLOR_BLACK;
  SPEED_IND.PROP.BCOLOR = COLOR_BLACK;
  SPEED_IND.PROP.SIZEX = 1;
  SPEED_IND.PROP.SIZEY = 2;
  SPEED_IND.PROP.DONT_BLANK = true;
  SPEED_IND.PROP.JUSTIFICATION_LEFT = true;

  // Track
  TRACK.PROP.POSY = 1;
  TRACK.PROP.POSX = 7;
  TRACK.PROP.SIZEX = 3;
  TRACK.PROP.DONT_BLANK = true;
  TRACK.PROP.JUSTIFICATION_RIGHT = true;
  TRACK.PROP.UPDATE_INDICATION = true;

  // Track Mini Compass
  TRACK_MINI_COMPASS.PROP.POSY = 2;
  TRACK_MINI_COMPASS.PROP.POSX = 7;
  TRACK_MINI_COMPASS.PROP.COLOR = COLOR_BLACK;
  TRACK_MINI_COMPASS.PROP.BCOLOR = COLOR_BLACK;
  
  // Track Nav
  //NAV_HEADING.PROP.LABEL = "navt";
  NAV_HEADING.PROP.POSY = 1;
  NAV_HEADING.PROP.POSX = 11;
  NAV_HEADING.PROP.COLORS_ON = true;
  NAV_HEADING.PROP.COLOR = COLOR_BLUE;
  NAV_HEADING.PROP.BCOLOR = COLOR_BLACK;
  NAV_HEADING.PROP.SIZEX = 3;
  NAV_HEADING.PROP.DONT_BLANK = true;
  NAV_HEADING.PROP.JUSTIFICATION_LEFT = true;
  NAV_HEADING.PROP.UPDATE_INDICATION = true;

  // Track Nav Mini Compass
  NAV_HEADING_MINI_COMPASS.PROP.POSY = 2;
  NAV_HEADING_MINI_COMPASS.PROP.POSX = 11;
  NAV_HEADING_MINI_COMPASS.PROP.COLOR = COLOR_BLACK;
  NAV_HEADING_MINI_COMPASS.PROP.BCOLOR = COLOR_BLACK;
  
  // Coordinate Data TTL Indicator
  COORD_TTL_IND.PROP.POSY = 0;
  COORD_TTL_IND.PROP.POSX = 8;
  COORD_TTL_IND.PROP.COLORS_ON = true;
  COORD_TTL_IND.PROP.COLOR = COLOR_BLACK;
  COORD_TTL_IND.PROP.BCOLOR = COLOR_BLACK;
  COORD_TTL_IND.PROP.SIZEX = 3;
  COORD_TTL_IND.PROP.DONT_BLANK = true;
  COORD_TTL_IND.PROP.JUSTIFICATION_LEFT = true;
  COORD_TTL_IND.PROP.UPDATE_INDICATION = true;

  // Aircraft Data TTL Indicator
  DATA_TTL_IND.PROP.POSY = 0;
  DATA_TTL_IND.PROP.POSX = 11;
  DATA_TTL_IND.PROP.COLORS_ON = true;
  DATA_TTL_IND.PROP.COLOR = COLOR_BLACK;
  DATA_TTL_IND.PROP.BCOLOR = COLOR_BLACK;
  DATA_TTL_IND.PROP.SIZEX = 2;
  DATA_TTL_IND.PROP.DONT_BLANK = true;
  DATA_TTL_IND.PROP.JUSTIFICATION_LEFT = true;

  // Signal Strengh Indicator
  SIG_STR_IND.PROP.POSY = 0;
  SIG_STR_IND.PROP.POSX = 13;
  SIG_STR_IND.PROP.COLORS_ON = true;
  SIG_STR_IND.PROP.COLOR = COLOR_BLACK;
  SIG_STR_IND.PROP.BCOLOR = COLOR_BLACK;
  SIG_STR_IND.PROP.SIZEX = 3;
  SIG_STR_IND.PROP.DONT_BLANK = true;
  SIG_STR_IND.PROP.JUSTIFICATION_LEFT = true;

  // Signal Strengh Direction
  SIG_STR_DIRECION.PROP.SLICES = 5;
  SIG_STR_DIRECION.PROP.TIME_SPAN = 5000;
  SIG_STR_DIRECION.PROP.DIRECTION_NUTRAL_RANGE = .375;

  // Message
  MESSAGE.PROP.POSY = PROP.SIZEY - 1;
  MESSAGE.PROP.POSX = 1;
  MESSAGE.PROP.SIZEX = PROP.SIZEX - 2;
  MESSAGE.PROP.JUSTIFICATION_LEFT = true;

  refresh();
  */

  //PROP.CHANGED = true;
}

void ADSB_WIDGET::clear()
// Clear values
{
  // Clear Variables
  ADSB_WIDGET_Properties cleared_properties;
  PROP = cleared_properties;
  WIDGET_ACTIVE = false;

  AIRCRAFT blank_aircraft_data;
  AIRCRAFT_DATA = blank_aircraft_data;

  TRACK.clear();

  /*
  // Reset Text Fields
  FLIGHT.clear();
  SQUAWK.clear();
  ALTITUDE.clear();
  NAV_ALTITUDE_MCP.clear();
  D_VERTICAL_RATE.clear();
  SPEED.clear();

  TRACK.clear();
  TRACK_MINI_COMPASS.clear();
  
  NAV_HEADING.clear();
  NAV_HEADING_MINI_COMPASS.clear();

  SIG_STR_IND.set_color(COLOR_BLACK, COLOR_BLACK);
  SIG_STR_IND.clear();

  COORD_TTL_IND.set_color(COLOR_BLACK, COLOR_BLACK);
  COORD_TTL_IND.clear();
  
  DATA_TTL_IND.set_color(COLOR_BLACK, COLOR_BLACK);
  DATA_TTL_IND.clear();

  //NAV_QNH.set_text(PROP.AIRCRAFT_DATA.NAV_QNH.get_str_value());
  */
}

bool ADSB_WIDGET::is_expired(system_data &sdSysData)
{
  if (EXPIREED.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == true)
  {
    return false;
  }
  else
  {
    clear();
    return true;
  }
}

bool ADSB_WIDGET::changed()
//  Return true is screen will be redrawn on next draw.
//  Return false if no changes made.
{
  //return PROP.CHANGED; 
  return false;
}

void ADSB_WIDGET::update_aircraft(AIRCRAFT Aircraft, unsigned long &tmeCurrentMillis)
// Update values of gadget
//  Gadget will be redrawn if values did changed or animations scheduled. 
{
  // Start or Continue expiration timer
  EXPIREED.ping_up(tmeCurrentMillis, EXPIRATION_TIME);
  WIDGET_ACTIVE = true;

  AIRCRAFT_DATA = Aircraft;

  if (AIRCRAFT_DATA.POSITION.GLOBAL_POSITION_FOUND)
  {
    // Add only if new value not the same as prev, or no other values exist.
    if (TRACK.size() == 0)
    {
      {
        AIRCRAFT_TRACK_POINT new_lat_lon;
        new_lat_lon.LAT_LON = ImVec2(Aircraft.POSITION.LATITUDE.get_float_value(), Aircraft.POSITION.LONGITUDE.get_float_value());
        
        float intensity = (32.0f + Aircraft.RSSI.get_float_value()) / 32.0f;

        if (intensity < 0.2f)
        {
          intensity = 0.2f;
        }
        
        new_lat_lon.RSSI_INTENSITY = intensity;

        TRACK.push_back(new_lat_lon);
      }
    }
    else if (AIRCRAFT_DATA.POSITION.LATITUDE.get_float_value() != TRACK[TRACK.size() -1].LAT_LON.x || 
              AIRCRAFT_DATA.POSITION.LONGITUDE.get_float_value() != TRACK[TRACK.size() -1].LAT_LON.y)
    {
      AIRCRAFT_TRACK_POINT new_lat_lon;
      new_lat_lon.LAT_LON = ImVec2(Aircraft.POSITION.LATITUDE.get_float_value(), Aircraft.POSITION.LONGITUDE.get_float_value());
      
      float intensity = (32.0f + Aircraft.RSSI.get_float_value()) / 32.0f;

      if (intensity < 0.2f)
      {
        intensity = 0.2f;
      }
      
      new_lat_lon.RSSI_INTENSITY = intensity;

      TRACK.push_back(new_lat_lon);
    }
  }

  //PROP.CHANGED = true;
}

bool ADSB_WIDGET::draw(system_data &sdSysData)
// Draw all changes to Panel.
//  Set Refresh to true to force redraw.
//  Animations will be ignored without time reference.
// Returns true if panel was redrawn.
{
  if (is_expired(sdSysData) == false)
  {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.FLIGHT.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.SQUAWK.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.SPEED.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.VERT_RATE.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.ALTITUDE.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.NAV_ALTITUDE_MCP.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.TRACK.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.NAV_HEADING.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.SEEN_POS.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.SEEN.get_str_value().c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", AIRCRAFT_DATA.RSSI.get_str_value().c_str());
  }
  else
  {
    // Clear all data and reset to start empty state.
    clear();  // CLEAR NEEDS CLEANING

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
  }
  
  //bool ret_redrawn = false;

  /*
  int tmp_bcolor = 0;
  int tmp_color = 0;
  */

  // Check Expiration
  //if ((EXPIREED.blip_moved(sdSysData.tmeCURRENT_FRAME_TIME) == true && EXPIREED.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == false) || 
  //    (Refresh == true && EXPIREED.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == false))
  //if ((EXPIREED.blip_moved(sdSysData.tmeCURRENT_FRAME_TIME) == true && EXPIREED.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == false))
  //{
    // Clear all data and reset to start empty state.
    //clear();
    //Refresh = true;
  //}

  /*
  if ((PROP.CHANGED == true || Refresh == true))
  {
    // Draw Background
    // If No Data at all
    if (PROP.AIRCRAFT_DATA.data_count() == 0 && PROP.AIRCRAFT_DATA.HEX.conversion_success() == false)
    {
      PROP.BCOLOR = COLOR_BLACK;
      PROP.COLOR = COLOR_BLACK;
      MESSAGE.set_text("");
    }
    // If No Aircraft Data
    else if (PROP.AIRCRAFT_DATA.data_count() == 0 && PROP.AIRCRAFT_DATA.HEX.conversion_success() == true)
    {
      PROP.BCOLOR = COLOR_BLUE;
      PROP.COLOR = COLOR_WHITE;
    }
    // If Position Found
    else if(PROP.AIRCRAFT_DATA.POSITION.GLOBAL_POSITION_FOUND == true)
    {
      PROP.BCOLOR = COLOR_GREEN;
      PROP.COLOR = COLOR_BLACK;

      if (LATITUDE == PROP.AIRCRAFT_DATA.POSITION.LATITUDE.get_float_value() && 
          LONGITUDE == PROP.AIRCRAFT_DATA.POSITION.LONGITUDE.get_float_value())
      {
        // Position has not changed.
      }
      else
      {
        // Coordinate Indicate Blink
        COORD_TTL_IND.blink(tmeFrame_Time);
        LATITUDE = PROP.AIRCRAFT_DATA.POSITION.LATITUDE.get_float_value();
        LONGITUDE = PROP.AIRCRAFT_DATA.POSITION.LONGITUDE.get_float_value();
      }
    }
    // If No Position Found but Data Exist
    else
    {
      PROP.BCOLOR = COLOR_YELLOW;
      PROP.COLOR = COLOR_BLACK;
    }

    // Write Additional Messages
    if (PROP.AIRCRAFT_DATA.data_count() == 0 && PROP.AIRCRAFT_DATA.HEX.conversion_success() == true)
    {
      MESSAGE.set_text("No Data");
    }
    else
    {
      MESSAGE.set_text("");
    }

    // If Emergency set message
    if (PROP.AIRCRAFT_DATA.EMERGENCY.conversion_success() == true)
    {
      if (PROP.AIRCRAFT_DATA.EMERGENCY.get_str_value() != "none")
      {
        PROP.BCOLOR = COLOR_RED;
        PROP.COLOR = COLOR_WHITE;
        MESSAGE.set_text(PROP.AIRCRAFT_DATA.EMERGENCY.get_str_value());
      }  
    }
    
    ADSB_PANEL.set_color(PROP.BCOLOR, PROP.COLOR);

    // Set Colors and Text

    // Flight and Squawk
    if (PROP.AIRCRAFT_DATA.data_count() > 0)
    {
      //TOP_BAR.set_color(COLOR_WHITE, COLOR_BLACK);
      FLIGHT.set_color(COLOR_WHITE, COLOR_BLACK);
      SQUAWK.set_color(COLOR_WHITE, COLOR_BLACK);
    }
    else
    {
      //TOP_BAR.set_color(COLOR_BLACK, COLOR_BLACK);
      FLIGHT.set_color(COLOR_BLACK, COLOR_BLACK);
      SQUAWK.set_color(COLOR_BLACK, COLOR_BLACK);
    }

    FLIGHT.set_text(PROP.AIRCRAFT_DATA.FLIGHT.get_str_value(), tmeFrame_Time);
    SQUAWK.set_text(PROP.AIRCRAFT_DATA.SQUAWK.get_str_value(), tmeFrame_Time);

    //Altitude and Altitude Inicator
    if (PROP.AIRCRAFT_DATA.ALTITUDE.conversion_success()==true)
    {
      // Aircraft Altitude data ok
      tmp_text_color_correction(color_range(abs(PROP.AIRCRAFT_DATA.ALTITUDE.get_int_value()), 50, 500, 3000, 12000, 40000),
                                            tmp_bcolor, tmp_color);

      ALTITUDE_IND.set_color(tmp_bcolor, tmp_color);

      // Direction of value.
      ALTITUDE_DIRECION.put_value(PROP.AIRCRAFT_DATA.ALTITUDE.get_int_value(), tmeFrame_Time);
      int altitude_direction = ALTITUDE_DIRECION.direction();        
      if (altitude_direction == 1)
      {
        ALTITUDE_IND.set_text("^\n|");
      }
      else if (altitude_direction == 0)
      {
        ALTITUDE_IND.set_text("_\n ");
      }
      else
      {
        ALTITUDE_IND.set_text("|\nv");
      }
      
      ALTITUDE.set_text(PROP.AIRCRAFT_DATA.ALTITUDE.get_str_value(), tmeFrame_Time);
    }
    else
    {
      if (PROP.AIRCRAFT_DATA.ALTITUDE.get_str_value() == "ground")
      // Aircraft Altitude data says on ground
      {
        ALTITUDE_IND.set_color(COLOR_RED, COLOR_WHITE);
        ALTITUDE_IND.set_text("X\nX");
        ALTITUDE.set_text("GROUND");
      }
      else
      {
        // Aircraft Altitude data unknown
        ALTITUDE_IND.set_color(PROP.BCOLOR, PROP.BCOLOR);
        ALTITUDE_IND.set_text(" \n ");
        ALTITUDE.set_text(PROP.AIRCRAFT_DATA.ALTITUDE.get_str_value(), tmeFrame_Time);
      }
    }

    // Aircraft Nav Altitude
    NAV_ALTITUDE_MCP.set_color(PROP.BCOLOR, COLOR_BLUE);
    NAV_ALTITUDE_MCP.set_text(PROP.AIRCRAFT_DATA.NAV_ALTITUDE_MCP.get_str_value(), tmeFrame_Time);
    
    // Aircraft Vertical Rate and Vertical Rate Indicator
    if (PROP.AIRCRAFT_DATA.D_FLIGHT_ANGLE.conversion_success()==true)
    {
      tmp_text_color_correction(color_scale(abs(PROP.AIRCRAFT_DATA.D_FLIGHT_ANGLE.get_float_value()), 2, 4, 6, 8, 10),
                                      tmp_bcolor, tmp_color);


      D_VERTICAL_RATE_IND.set_color(tmp_bcolor, tmp_color);
      D_VERTICAL_RATE_IND.set_text("^\nv");
    }
    else
    {
      D_VERTICAL_RATE_IND.set_color(PROP.BCOLOR, COLOR_BLACK);
      D_VERTICAL_RATE_IND.set_text(" ");
    }
    
    D_VERTICAL_RATE.set_text(PROP.AIRCRAFT_DATA.D_VERTICAL_RATE.get_str_value(), tmeFrame_Time);

    // Aircraft Speed and Speed Indicator
    if (PROP.AIRCRAFT_DATA.SPEED.conversion_success()==true)
    {
      tmp_text_color_correction(color_range(abs(PROP.AIRCRAFT_DATA.SPEED.get_float_value()), 60, 80, 100, 160, 600),
                                            tmp_bcolor, tmp_color);

      SPEED_IND.set_color(tmp_bcolor, tmp_color);

      // Direction of value.
      SPEED_DIRECION.put_value(PROP.AIRCRAFT_DATA.SPEED.get_float_value(), tmeFrame_Time);
      int speed_direction = SPEED_DIRECION.direction();        
      if (speed_direction == 1)
      {
        SPEED_IND.set_text("^\n|");
      }
      else if (speed_direction == 0)
      {
        SPEED_IND.set_text("_\n ");
      }
      else
      {
        SPEED_IND.set_text("|\nv");
      }
    }
    else
    {
      SPEED_IND.set_color(PROP.BCOLOR, COLOR_BLACK);
      SPEED_IND.set_text(" \n ");
    }

    SPEED.set_text(PROP.AIRCRAFT_DATA.SPEED.get_str_value(), tmeFrame_Time);

    // Aircraft Track and Compass Track
    if (PROP.AIRCRAFT_DATA.TRACK.conversion_success() == true)
    {
      TRACK_MINI_COMPASS.set_color(COLOR_BLUE, COLOR_WHITE);
      TRACK_MINI_COMPASS.set_heading(PROP.AIRCRAFT_DATA.TRACK.get_float_value());
      TRACK.set_text(to_string(PROP.AIRCRAFT_DATA.TRACK.get_int_value()), tmeFrame_Time);
    }
    else
    {
      TRACK_MINI_COMPASS.set_color(PROP.BCOLOR, PROP.BCOLOR);
      TRACK_MINI_COMPASS.clear();
      TRACK.set_text("");
    }

    // Aircraft NAV Track and Compass NAV Track
    NAV_HEADING.set_color(PROP.BCOLOR, COLOR_BLUE);
    if (PROP.AIRCRAFT_DATA.NAV_HEADING.conversion_success() == true)
    {
      NAV_HEADING_MINI_COMPASS.set_color(COLOR_BLUE, COLOR_WHITE);
      NAV_HEADING_MINI_COMPASS.set_heading(PROP.AIRCRAFT_DATA.NAV_HEADING.get_float_value());
      NAV_HEADING.set_text(to_string(PROP.AIRCRAFT_DATA.NAV_HEADING.get_int_value()), tmeFrame_Time);
    }
    else
    {
      NAV_HEADING_MINI_COMPASS.set_color(PROP.BCOLOR, PROP.BCOLOR);
      NAV_HEADING_MINI_COMPASS.clear();
      NAV_HEADING.set_text("");
    }

    // Aircraft COORD TTL, Data TTL, and RSSI Str.
    if (PROP.AIRCRAFT_DATA.HEX.conversion_success() == true)
    {
      // Aircraft Geo Coordinates TTL
      if (  PROP.AIRCRAFT_DATA.SEEN_POS.conversion_success()==true &&
            PROP.AIRCRAFT_DATA.POSITION.GLOBAL_POSITION_FOUND == true)
      {
        tmp_text_color_correction(color_scale(abs(PROP.AIRCRAFT_DATA.SEEN_POS.get_float_value()), 5, 45, 65, 0, 0),
                                              tmp_bcolor, tmp_color);

        COORD_TTL_IND.set_color(tmp_bcolor, tmp_color);
        COORD_TTL_IND.set_text("( )");
      }
      else
      {
        COORD_TTL_IND.set_color(PROP.BCOLOR, PROP.BCOLOR);
      }

      // Aircraft Data TTL
      if (PROP.AIRCRAFT_DATA.SEEN.conversion_success()==true)
      {
        tmp_text_color_correction(color_scale(abs(PROP.AIRCRAFT_DATA.SEEN.get_float_value()), 70, 170, 290, 0, 0),
                                              tmp_bcolor, tmp_color);

        DATA_TTL_IND.set_color(tmp_bcolor, tmp_color);
        DATA_TTL_IND.set_text("()");
      }

      // Aircraft RSSI Strength
      if (PROP.AIRCRAFT_DATA.RSSI.conversion_success()==true)
      {
        tmp_text_color_correction(color_scale(abs(PROP.AIRCRAFT_DATA.RSSI.get_float_value()), 18, 30, 32, 34, 36),
                                              tmp_bcolor, tmp_color);

        SIG_STR_IND.set_color(tmp_bcolor, tmp_color);

        // Direction of value.
        SIG_STR_DIRECION.put_value(PROP.AIRCRAFT_DATA.RSSI.get_float_value(), tmeFrame_Time);
        int sig_direction = SIG_STR_DIRECION.direction();        
        if (sig_direction == 1)
        {
          SIG_STR_IND.set_text("(^)");
        }
        else if (sig_direction == 0)
        {
          SIG_STR_IND.set_text("(-)");
        }
        else
        {
          SIG_STR_IND.set_text("(v)");
        }
      }
    }
    else
    {
      COORD_TTL_IND.set_color(PROP.BCOLOR, PROP.BCOLOR);
      COORD_TTL_IND.set_text("   ");

      DATA_TTL_IND.set_color(PROP.BCOLOR, PROP.BCOLOR);
      DATA_TTL_IND.set_text("  ");

      SIG_STR_IND.set_color(PROP.BCOLOR, PROP.BCOLOR);
      SIG_STR_IND.set_text("   ");
    }
  }
  */

  // Write All text fields.

  /*
  FLIGHT.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  SQUAWK.draw(ADSB_PANEL, Refresh, tmeFrame_Time);

  ALTITUDE_IND.draw(ADSB_PANEL, Refresh);
  
  ALTITUDE.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  
  NAV_ALTITUDE_MCP.draw(ADSB_PANEL, Refresh, tmeFrame_Time);

  D_VERTICAL_RATE_IND.draw(ADSB_PANEL, Refresh);
  D_VERTICAL_RATE.draw(ADSB_PANEL, Refresh, tmeFrame_Time);

  SPEED_IND.draw(ADSB_PANEL, Refresh);
  SPEED.draw(ADSB_PANEL, Refresh, tmeFrame_Time);

  TRACK.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  TRACK_MINI_COMPASS.draw(ADSB_PANEL, Refresh);

  NAV_HEADING.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  NAV_HEADING_MINI_COMPASS.draw(ADSB_PANEL, Refresh);

  SIG_STR_IND.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  COORD_TTL_IND.draw(ADSB_PANEL, Refresh, tmeFrame_Time);
  DATA_TTL_IND.draw(ADSB_PANEL, Refresh, tmeFrame_Time);

  MESSAGE.draw(ADSB_PANEL, Refresh);
  */

  // Reset Properties Changed.
  //PROP.CHANGED = false;

  // Draw the Gadget.
  //return ADSB_PANEL.draw(Refresh);

  return false;

}

bool ADSB_WIDGET::active()
{
  return WIDGET_ACTIVE;
}

// Draw all aircraft onto the maps.
void ADSB_WIDGET::draw_aircraft_map_marker(system_data &sdSysData, ImVec4 Working_Area, ImVec2 Scale, ImVec2 Center_Lat_Lon)
{
  if (AIRCRAFT_DATA.POSITION.GLOBAL_POSITION_FOUND == true || TRACK.size() > 1)
  {
    bool draw = false;
    ImVec2 draw_position = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon,
                                                  ImVec2( AIRCRAFT_DATA.POSITION.LATITUDE.get_float_value(), 
                                                          AIRCRAFT_DATA.POSITION.LONGITUDE.get_float_value()), 
                                                          draw);
    
    //if (draw)
    {
      // Draw track first then overlay aircraft.
      if (TRACK.size() > 1)
      {
        bool draw_0 = false;
        bool draw_1 = false;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 track_position_0;
        ImVec2 track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, TRACK[0].LAT_LON, draw_1);
        
        for(int position = 1; position < TRACK.size(); position++)
        {
          track_position_0 = track_position_1;
          draw_0 = draw_1;

          track_position_1 = point_position_lat_lon(Working_Area, Scale, Center_Lat_Lon, TRACK[position].LAT_LON, draw_1);

          if (draw_0 || draw_1)
          {
            ImColor point_color = sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.TEXT;

            point_color.Value.w = TRACK[position].RSSI_INTENSITY;

            draw_point_marker(sdSysData, track_position_0, point_color, 3);

            draw_list->AddLine(track_position_0, track_position_1, 
                                sdSysData.COLOR_SELECT.COLOR_COMB_GREY.TEXT, 2);
          }
        }
      }


      // Draw Arrow of Aircraft nav heading on map at position
      if (AIRCRAFT_DATA.NAV_HEADING.conversion_success())
      {
        draw_aircraft_marker_direction(draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, 20, AIRCRAFT_DATA.TRACK.get_float_value());
      }

      // Draw Arrow of Aircraft track heading on map at position, grey if old.
      if (AIRCRAFT_DATA.SEEN_POS.get_int_value() <= 5)
      {
        if (AIRCRAFT_DATA.TRACK.conversion_success())
        {
          draw_aircraft_marker_direction(draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, 15, AIRCRAFT_DATA.TRACK.get_float_value());
        }
        else
        {
          draw_aircraft_marker(sdSysData, draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, 6);
        }
      }
      else
      {
        if (AIRCRAFT_DATA.TRACK.conversion_success())
        {
          draw_aircraft_marker_direction(draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_GREY, 12, AIRCRAFT_DATA.TRACK.get_float_value());
        }
        else
        {
          draw_aircraft_marker(sdSysData, draw_position, sdSysData.COLOR_SELECT.COLOR_COMB_GREY, 4);
        }
      }

      // Text describing Aircraft
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));
  
      ImGui::SetCursorScreenPos(ImVec2(draw_position.x + 5, draw_position.y + 8));
      ImGui::Text("%d %s", AIRCRAFT_DATA.SEEN_POS.get_int_value(), AIRCRAFT_DATA.FLIGHT.get_str_value().c_str());
      
      ImGui::SetCursorScreenPos(ImVec2(draw_position.x + 5, draw_position.y + 25));
      ImGui::Text("S: %d", AIRCRAFT_DATA.SPEED.get_int_value());

      ImGui::SetCursorScreenPos(ImVec2(draw_position.x + 5, draw_position.y + 41));
      if (AIRCRAFT_DATA.ALTITUDE.conversion_success())
      {
        ImGui::Text("A: %.1f", float(AIRCRAFT_DATA.ALTITUDE.get_int_value() / 1000.0f));
      }
      else
      {
        ImGui::Text("A: %s", AIRCRAFT_DATA.ALTITUDE.get_str_value().c_str());
      }

      ImGui::PopStyleColor();
    }
  }
}

// ---------------------------------------------------------------------------------------

void ADSB_RANGE::calculate_lat_lon_to_point_scale()
{
  float latitude_diff = 0;
  float longitude_diff = 0;

  ImVec2 new_coords_east = get_coords_x_miles_from_coords(CURRENT_LAT_LON.x, CURRENT_LAT_LON.y,
                                                          RANGE, 90);
  ImVec2 new_coords_south = get_coords_x_miles_from_coords(CURRENT_LAT_LON.x, CURRENT_LAT_LON.y,
                                                          RANGE, 180);

  latitude_diff = abs(new_coords_south.x - CURRENT_LAT_LON.x);
  longitude_diff = abs(new_coords_east.y - CURRENT_LAT_LON.y);

  LAT_LON_TO_POINT_SCALE.x =(RADIUS_CIRCLE_POINT_SIZE / latitude_diff);
  LAT_LON_TO_POINT_SCALE.y = (RADIUS_CIRCLE_POINT_SIZE / longitude_diff);
}

void ADSB_RANGE::set_zoom_level()
{
  switch (ZOOM_LEVEL)
  {
    case 0:
    {
      set_range(1);
      calculate_lat_lon_to_point_scale();
      break;
    }

    case 1:
    {
      set_range(5);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 2:
    {
      set_range(10);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 3:
    {
      set_range(25);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 4:
    {
      set_range(50);
      calculate_lat_lon_to_point_scale();
      break;
    }
    
    case 5:
    {
      set_range(100);
      calculate_lat_lon_to_point_scale();
      break;
    }
  }
}

ImVec2 ADSB_RANGE::ll_2_pt_scale()
{
  return LAT_LON_TO_POINT_SCALE;
}

ImVec2 ADSB_RANGE::current_lat_lon()
{
  return CURRENT_LAT_LON;
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
  if (ZOOM_LEVEL < 5)
  {
    ZOOM_LEVEL = ZOOM_LEVEL + 1;
    set_zoom_level();
  }
}

void ADSB_RANGE::set_current_global_position(ImVec2 Lat_Lon)
{
  CURRENT_LAT_LON = Lat_Lon;
}

void ADSB_RANGE::draw(system_data &sdSysData, ImVec4 Working_Area)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  if (ZOOM_LEVEL == -1)
  {
    RADIUS_CIRCLE_POINT_SIZE = Working_Area.w / 2 * 0.6f;

    ZOOM_LEVEL = 3;
    set_zoom_level();
  }

  /*
  if (  PREV_WORKING_AREA.x != Working_Area.x || 
      PREV_WORKING_AREA.y != Working_Area.y || 
      PREV_WORKING_AREA.z != Working_Area.z || 
      PREV_WORKING_AREA.w != Working_Area.w )
  {
    PREV_WORKING_AREA.x = Working_Area.x;
    PREV_WORKING_AREA.y = Working_Area.y;
    PREV_WORKING_AREA.z = Working_Area.z;
    PREV_WORKING_AREA.w = Working_Area.w;
    
    
    calculate_lat_lon_to_point_scale();
  }
  */

  const float spacing = 10.0f;

  ImVec2 center = point_position_center(Working_Area);
  
  draw_list->AddNgon(center, RADIUS_CIRCLE_POINT_SIZE, ImU32(PROPS.COLOR.STANDARD), 32, 1.5f);
  draw_list->AddNgon(center, RADIUS_CIRCLE_POINT_SIZE *2, ImU32(PROPS.COLOR.STANDARD), 32, 1.5f);
  
  ImGui::SetCursorScreenPos(ImVec2(center.x, center.y - RADIUS_CIRCLE_POINT_SIZE + 5));
  
  // Text Range
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(PROPS.COLOR.STANDARD));
  ImGui::Text("%.0f", RANGE);
  ImGui::PopStyleColor();
}

void ADSB_RANGE::draw_info(system_data &sdSysData)
{
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_GREY.TEXT));
  ImGui::Text("lat: %f", CURRENT_LAT_LON.x);
  ImGui::Text("lon: %f", CURRENT_LAT_LON.y);
  ImGui::Text("rng: %.0f", RANGE);
  ImGui::PopStyleColor();

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
  RANGE_INDICATOR.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;
  RANGE_INDICATOR.set_range(25.0f);

  // KLFT - Lafayette Regional/Paul Fournet Field Airport
  // Coordinates: 
  //                                LATITUDE      LONGITUDE
  // Degrees Decimal Minutes (DDM): N30°12.30'  / W91°59.27'
  // Decimal Degrees (DD):          30.205      / -91.987833

  RANGE_INDICATOR.set_current_global_position(ImVec2(30.205f, -91.987833f));

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
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(110);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(290);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.4663333333f, -92.4238333333f), "4R7", 0);  //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.4663333333f, -92.4238333333f);
  tmp_map_marker.DISPLAY_NAME = "4R7";
  tmp_map_marker.LONG_NAME = "Eunice Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(160);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(340);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2426666667f, -92.6735f), "3R7", 0);        //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.2426666667f, -92.6735f);
  tmp_map_marker.DISPLAY_NAME = "3R7";
  tmp_map_marker.LONG_NAME = "Jennings Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(80);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(260);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.533f, -91.1498333333f), "BTR", 0);         //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.533f, -91.1498333333f);
  tmp_map_marker.DISPLAY_NAME = "BTR";
  tmp_map_marker.LONG_NAME = "Baton Rouge Metro, Ryan";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0378333333f, -91.8838333333f), "ARA", 0);  //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0378333333f, -91.8838333333f);
  tmp_map_marker.DISPLAY_NAME = "ARA";
  tmp_map_marker.LONG_NAME = "Acadiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2418333333f, -92.8306666667f), "6R1", 0);  // 6R1 - Welsh Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.2418333333f, -92.8306666667f);
  tmp_map_marker.DISPLAY_NAME = "6R1";
  tmp_map_marker.LONG_NAME = "Welsh Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(70);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(250);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(90);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(270);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.126f, -93.2235f), "LCH", 0);               // LCH - Lake Charles Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.126f, -93.2235f);
  tmp_map_marker.DISPLAY_NAME = "LCH";
  tmp_map_marker.LONG_NAME = "Lake Charles Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(150);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(330);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(50);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(230);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.7505f, -92.6885f), "ARA", 0);              // ACP - Allen Parish Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.7505f, -92.6885f);
  tmp_map_marker.DISPLAY_NAME = "ACP";
  tmp_map_marker.LONG_NAME = "Allen Parish Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.9566666667f, -92.2341666667f), "2R6", 0);  // 2R6 - Bunkie Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.9566666667f, -92.2341666667f);
  tmp_map_marker.DISPLAY_NAME = "2R6";
  tmp_map_marker.LONG_NAME = "Bunkie Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.3273333333f, -92.5485f), "AEX", 0);        // AEX - Alexandria International Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(31.3273333333f, -92.5485f);
  tmp_map_marker.DISPLAY_NAME = "AEX";
  tmp_map_marker.LONG_NAME = "Alexandria International Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(140);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(320);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.7183333333f, -91.4786666667f), "HZR", 0);  // HZR - False River Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.7183333333f, -91.4786666667f);
  tmp_map_marker.DISPLAY_NAME = "HZR";
  tmp_map_marker.LONG_NAME = "False River Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.1713333333f, -90.9403333333f), "REG", 0);  // REG - Louisiana Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.1713333333f, -90.9403333333f);
  tmp_map_marker.DISPLAY_NAME = "REG";
  tmp_map_marker.LONG_NAME = "Louisiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.5216666667f, -90.4183333333f), "HDC", 0);  // HDC - Hammond Northshore Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.5216666667f, -90.4183333333f);
  tmp_map_marker.DISPLAY_NAME = "HDC";
  tmp_map_marker.LONG_NAME = "Hammond Northshore Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(130);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(310);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.445f, -89.9888333333f), "L31", 0);         // L31 - St Tammany Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.445f, -89.9888333333f);
  tmp_map_marker.DISPLAY_NAME = "L31";
  tmp_map_marker.LONG_NAME = "St Tammany Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.3463333333f, -89.8208333333f), "ASD", 0);  // ASD - Slidell Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.3463333333f, -89.8208333333f);
  tmp_map_marker.DISPLAY_NAME = "ASD";
  tmp_map_marker.LONG_NAME = "Slidell Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.4875f, -89.6511666667f), "MJD", 0);        // MJD - Picayune Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.4875f, -89.6511666667f);
  tmp_map_marker.DISPLAY_NAME = "MJD";
  tmp_map_marker.LONG_NAME = "Picayune Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0425f, -90.0281666667f), "NEW", 0);        // NEW - Lakefront Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0425f, -90.0281666667f);
  tmp_map_marker.DISPLAY_NAME = "NEW";
  tmp_map_marker.LONG_NAME = "Lakefront Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(29.8271666667f, -90.0266666667f), "NBG", 0);  // NBG - New Orleans NAS Jrb (Alvin Callender Field) Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.8271666667f, -90.0266666667f);
  tmp_map_marker.DISPLAY_NAME = "NBG";
  tmp_map_marker.LONG_NAME = "New Orleans NAS Jrb (Alvin Callender Field) Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(40);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(220);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(140);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(320);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(29.9933333333f, -90.259f), "MSY", 0);         // MSY - Louis Armstrong New Orleans International Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(29.9933333333f, -90.259f);
  tmp_map_marker.DISPLAY_NAME = "MSY";
  tmp_map_marker.LONG_NAME = "Louis Armstrong New Orleans International Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(110);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(290);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(20);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(200);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0875f, -90.5828333333f), "APS", 0);        // APS - Port of South Louisiana Exec Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(30.0875f, -90.5828333333f);
  tmp_map_marker.DISPLAY_NAME = "APS";
  tmp_map_marker.LONG_NAME = "Port of South Louisiana Exec Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.1785f, -90.4718333333f), "MCB", 0);        // MCB - Mc Comb/Pike County/John E Lewis Field Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = ImVec2(31.1785f, -90.4718333333f);
  tmp_map_marker.DISPLAY_NAME = "MCB";
  tmp_map_marker.LONG_NAME = "Mc Comb/Pike County/John E Lewis Field Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350);
  LANDMARKS.push_back(tmp_map_marker);

  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
  //add_landmark(ImVec2(f, f), "", 0);  //
}

void ADSB_MAP::draw(system_data &sdSysData, DISPLAY_DATA_ADSB &SDATA, deque<ADSB_WIDGET> &ADSB_Widgets)
{
  // working area, xy start position; zw, size.
  //    or
  //  xy is offset
  //  zw is screen size.
  ImVec4 working_area;

  working_area.x = ImGui::GetCursorScreenPos().x;
  working_area.y = ImGui::GetCursorScreenPos().y;
  working_area.z = ImGui::GetContentRegionAvail().x;
  working_area.w = ImGui::GetContentRegionAvail().y;

  // -------------------------------------------------------------------------------------
  // All Text Below Here

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_RED.TEXT));
  ImGui::Text("WARNING: Information may be considered CONFIDENTIAL");
  ImGui::PopStyleColor();
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_GREY.TEXT));
  ImGui::Text("Time: %s", 
                SDATA.TIME_OF_SIGNAL.c_str());
  ImGui::Text("Count: %s  Pos: %s",
                SDATA.POSITIONED_COUNT.c_str(), 
                SDATA.POSITIONED_AIRCRAFT.c_str());
  ImGui::PopStyleColor();

  // Range Indicator
  RANGE_INDICATOR.draw_info(sdSysData);

  // Buttons
  ImGui::SetCursorScreenPos(ImVec2(working_area.x, working_area.y + working_area.w - (3 * (sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL.y + 5))));
  
  if (button_simple_color(sdSysData, "LOC", sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
  {
    DISPLAY_LOCATION = !DISPLAY_LOCATION;
  }
  
  //ImGui::SameLine();

  if (button_simple_color(sdSysData, "+", sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
  {
    RANGE_INDICATOR.zoom_out();
  }

  //ImGui::SameLine();

  if (button_simple_color(sdSysData, "-", sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
  {
    RANGE_INDICATOR.zoom_in();
  }

  //ImGui::SameLine();





  // All Text Above Here
  // -------------------------------------------------------------------------------------

  RANGE_INDICATOR.draw(sdSysData, working_area);

  // Draw Landmarks

  for (int landmark = 0; landmark < LANDMARKS.size(); landmark++)
  {
    LANDMARKS[landmark].draw(sdSysData, working_area, RANGE_INDICATOR.ll_2_pt_scale(), RANGE_INDICATOR.current_lat_lon());
  }
  
  // Draw Aircraft
  for (int aircraft = 0; aircraft < ADSB_Widgets.size(); aircraft ++)
  {
    if (ADSB_Widgets[aircraft].is_expired(sdSysData) == false)
    {
      ADSB_Widgets[aircraft].draw_aircraft_map_marker(sdSysData, working_area, RANGE_INDICATOR.ll_2_pt_scale(), RANGE_INDICATOR.current_lat_lon());
    }
  }

  // Show Location Chooser Window
  if (DISPLAY_LOCATION)
  {
    ImGui::SetNextWindowSize(ImVec2(133, 350));
    if (ImGui::Begin("Location", &DISPLAY_LOCATION, sdSysData.SCREEN_DEFAULTS.flags_w_pop_scroll)) 
    {
      for (int location = 0; location < LANDMARKS.size(); location ++)
      {
        if (button_simple_color(sdSysData, LANDMARKS[location].DISPLAY_NAME.c_str(), sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
        {
          RANGE_INDICATOR.set_current_global_position(LANDMARKS[location].LAT_LON);
        }
      }
    }
    ImGui::End();

  }
  

  // -------------------------------------------------------------------------------------
  // test for point position accuracy by getting geo from range, then putting geo marker 
  //  on range indicator.

  /*
  ImVec2 test_position;
  ImVec2 test_position2;

  test_position = get_coords_x_miles_from_coords(RANGE_INDICATOR.current_lat_lon().x, RANGE_INDICATOR.current_lat_lon().y,
                                                  RANGE_INDICATOR.range(), 225);

  test_position2 = point_position_lat_lon(working_area, RANGE_INDICATOR.ll_2_pt_scale(), 
                                  RANGE_INDICATOR.current_lat_lon(), test_position);

  draw_marker(sdSysData, test_position2);

  ImGui::Text("ll: %f %f", test_position.x, test_position.y);
  ImGui::Text("pt: %f %f", test_position2.x, test_position2.y);
  */

  /*
  test_position.x = 323;
  test_position.y = 323;
  draw_marker(sdSysData, point_position(working_area, test_position));

  test_position.x = 360;
  test_position.y = 400;
  draw_marker(sdSysData, point_position(working_area, test_position));

  test_position.x = 500;
  test_position.y = 580;
  draw_marker(sdSysData, point_position(working_area, test_position));

  test_position.x = 700;
  test_position.y = 500;
  draw_marker(sdSysData, point_position(working_area, test_position));

  test_position.x = 653;
  test_position.y = 223;
  draw_marker(sdSysData, point_position(working_area, test_position));
  */
}

// -------------------------------------------------------------------------------------

int ADSB_SCREEN::find_HEX(string Hex)
// Gadget Internal:
//  returns gadget position of aircraft with Hex ID
{
  int return_int = -1;
  for(int x=0; (x < ADSB_WIDGET_q.size()) && (return_int == -1); x++)
  {
    if(ADSB_WIDGET_q[x].AIRCRAFT_DATA.HEX.get_str_value() == Hex)
    {
      return_int = x;
    }
  }
  return return_int;
}

int ADSB_SCREEN::find_expired()
// Gadget Internal:
//  returns gadget position of aircraft with time expired.
{
  int return_int = -1;
  for(int x=0; (x < ADSB_WIDGET_q.size()) && (return_int == -1); x++)
  {
    if(ADSB_WIDGET_q[x].active() == false || ADSB_WIDGET_q[x].AIRCRAFT_DATA.data_count() == 0)
    {
      return_int = x;
    }
  }
  return return_int;
} 

void ADSB_SCREEN::create(system_data &sdSysData)
{
  ADSB_MAP_DISPLAY.create(sdSysData);
}

void ADSB_SCREEN::update(system_data &sdSysData)
{

  int pos_found = 0;
  int pos_avail = 0;
  int pos_expired_avail = 0;

  // Get new list of Aircraft.
  SDATA.AIRCRAFT_LIST = sdSysData.AIRCRAFT_COORD.DATA;
  SDATA.ADSB_ACTIVE = sdSysData.AIRCRAFT_COORD.is_active();

  // Prepare list to display.
  for (int pos_search = 0; pos_search < SDATA.AIRCRAFT_LIST.AIRCRAFTS.size(); pos_search++)
  {
    //Search gadget list for existing item to update.
    pos_found = find_HEX(SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos_search].HEX.get_str_value());

    // if not found, put aircraft info in first avail slot.
    if (pos_found == -1)
    {
      pos_avail = find_HEX("");
      
      //if slot found
      if (pos_avail == -1)
      {
        // search to put in empty position
        pos_expired_avail = find_expired();

        if (pos_expired_avail == -1)
        {
          ADSB_WIDGET new_adsb_widget;
          new_adsb_widget.update_aircraft(SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos_search], sdSysData.tmeCURRENT_FRAME_TIME);
          ADSB_WIDGET_q.push_back(new_adsb_widget);
        }
        else
        {
          ADSB_WIDGET_q[pos_expired_avail].clear();
          ADSB_WIDGET_q[pos_expired_avail].update_aircraft(SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos_search], sdSysData.tmeCURRENT_FRAME_TIME);
        }
      }
      else // slot found and pos avail.
      {
        ADSB_WIDGET_q[pos_avail].update_aircraft(SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos_search], sdSysData.tmeCURRENT_FRAME_TIME);
      }
    }
    else // put in found pos.
    {
      ADSB_WIDGET_q[pos_found].update_aircraft(SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos_search], sdSysData.tmeCURRENT_FRAME_TIME);
    }
  }

  //---

  SDATA.TIME_OF_SIGNAL =  (
                        to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_year()) + 
                  "-" + linemerge_right_justify(2, "00", to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_month())) + 
                  "-" + linemerge_right_justify(2, "00", to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_day())) + 
                  " " + linemerge_right_justify(2, "00", to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_hour())) + 
                  ":" + linemerge_right_justify(2, "00", to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_minute())) + 
                  ":" + linemerge_right_justify(2, "00", to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_second())) + 
                  "." + to_string(SDATA.AIRCRAFT_LIST.CONVERTED_TIME.get_deciseconds())
                          );

  SDATA.POSITIONED_COUNT = to_string(SDATA.AIRCRAFT_LIST.AIRCRAFTS.size() -1);
  SDATA.POSITIONED_AIRCRAFT = to_string(SDATA.AIRCRAFT_LIST.POSITIONED_AIRCRAFT);

  //---

  sdSysData.AIRCRAFT_COORD.DATA.CHANGED = false;
}

void ADSB_SCREEN::display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms,
                          const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::BeginChild("ADSB Buttons", ImVec2(90, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (button_simple_toggle_color(sdSysData, "ADSB", "ADSB", SDATA.ADSB_ACTIVE, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      if (SDATA.ADSB_ACTIVE == true)
      {
        Screen_Comms.command_text_set(" adsboff");
      }
      else
      {
        Screen_Comms.command_text_set(" adsbon");
      }
    }

    if (button_simple_enabled(sdSysData, "ADSB\nSNAP\nSHOT", SDATA.ADSB_ACTIVE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      Screen_Comms.command_text_set(" adsbsnap");
    }

    if (button_simple_color(sdSysData, "VIEW", sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_WHITE.TEXT));

    ImGui::BeginChild("ADSB Display", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, DEFAULTS.flags_c);
    {
      ImGui::Text("Time: %s  Count: %s  Pos: %s", 
                    SDATA.TIME_OF_SIGNAL.c_str(), 
                    SDATA.POSITIONED_COUNT.c_str(), 
                    SDATA.POSITIONED_AIRCRAFT.c_str());
      
      //      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
      //                              ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;

      if (ImGui::BeginTable("Aircraft Data", 11, DEFAULTS.flags_t))
      {
        {
          ImGui::TableSetupColumn("FLIGHT");
          ImGui::TableSetupColumn("SQWK");
          ImGui::TableSetupColumn("G SPD");
          ImGui::TableSetupColumn("V RTE");
          ImGui::TableSetupColumn("ALTITUDE");
          ImGui::TableSetupColumn("ALT NAV");
          ImGui::TableSetupColumn("HDG");
          ImGui::TableSetupColumn("HDG NAV");
          ImGui::TableSetupColumn("SEEN P");
          ImGui::TableSetupColumn("SEEN");
          ImGui::TableSetupColumn("RSSI");
          ImGui::TableHeadersRow();
        }
        
        //for (int pos = 0; pos < SDATA.AIRCRAFT_LIST.AIRCRAFTS.size(); pos++)
        for (int pos = 0; pos < ADSB_WIDGET_q.size(); pos++)
        {
          ADSB_WIDGET_q[pos].draw(sdSysData);
          /*
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].FLIGHT.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].SQUAWK.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].SPEED.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].VERT_RATE.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].ALTITUDE.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].NAV_ALTITUDE_MCP.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].TRACK.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].NAV_HEADING.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].SEEN_POS.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].SEEN.get_str_value().c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", SDATA.AIRCRAFT_LIST.AIRCRAFTS[pos].RSSI.get_str_value().c_str());
          */
        }
        ImGui::EndTable();
      }
    }
    ImGui::EndChild();

    ImGui::PopStyleColor();
  }
  else if (DISPLAY_MAP)
  {
    ImGui::BeginChild("ADSB Map", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, DEFAULTS.flags_c);
    {
      ADSB_MAP_DISPLAY.draw(sdSysData, SDATA, ADSB_WIDGET_q);
    }
    ImGui::EndChild();
  }
}

// ---------------------------------------------------------------------------------------
#endif