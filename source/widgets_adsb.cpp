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

  PROP.CHANGED = true;
}

void ADSB_WIDGET::clear()
// Clear values
{
  // Clear Variables
  ADSB_WIDGET_Properties cleared_properties;
  PROP = cleared_properties;
  
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

bool ADSB_WIDGET::changed()
//  Return true is screen will be redrawn on next draw.
//  Return false if no changes made.
{
  return PROP.CHANGED; 
}

void ADSB_WIDGET::update_aircraft(AIRCRAFT Aircraft, unsigned long &tmeCurrentMillis)
// Update values of gadget
//  Gadget will be redrawn if values did changed or animations scheduled. 
{
  // Start or Continue expiration timer
  EXPIREED.ping_up(tmeCurrentMillis, EXPIRATION_TIME);

  PROP.AIRCRAFT_DATA = Aircraft;

  PROP.CHANGED = true;
}

bool ADSB_WIDGET::draw(bool Refresh, unsigned long tmeFrame_Time)
// Draw all changes to Panel.
//  Set Refresh to true to force redraw.
//  Animations will be ignored without time reference.
// Returns true if panel was redrawn.
{
  bool ret_redrawn = false;

  /*
  int tmp_bcolor = 0;
  int tmp_color = 0;

  // Check Expiration
  if ((EXPIREED.blip_moved(tmeFrame_Time) == true && EXPIREED.ping_down(tmeFrame_Time) == false) || 
      (Refresh == true && EXPIREED.ping_down(tmeFrame_Time) == false))
  {
    // Clear all data and reset to start empty state.
    clear();
    Refresh = true;
  }
  */

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
  PROP.CHANGED = false;

  // Draw the Gadget.
  //return ADSB_PANEL.draw(Refresh);

  return false;

}

// -------------------------------------------------------------------------------------

int ADSB_SCREEN::find_HEX(string Hex)
// Gadget Internal:
//  returns gadget position of aircraft with Hex ID
{
  int return_int = -1;
  for(int x=0; (x < ADSB_WIDGET_q.size()) && (return_int == -1); x++)
  {
    if(ADSB_WIDGET_q[x].PROP.AIRCRAFT_DATA.HEX.get_str_value() == Hex)
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
    if(ADSB_WIDGET_q[x].PROP.AIRCRAFT_DATA.data_count() == 0)
    {
      return_int = x;
    }
  }
  return return_int;
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
          // do nothing. nothing avail.
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
    ImGui::BeginChild("ADSB Buttons", ImVec2(90, ImGui::GetContentRegionAvail().y), true, DEFAULTS.flags_c);
    {
      if (button_simple_toggle_color("ADSB", "ADSB", SDATA.ADSB_ACTIVE, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON))
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

      if (button_simple_enabled("ADSB\nSNAP\nSHOT", SDATA.ADSB_ACTIVE, DEFAULTS.SIZE_BUTTON))
      {
        Screen_Comms.command_text_set(" adsbsnap");
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    
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
          ImGui::TableSetupColumn("ALTITUDE    ");
          ImGui::TableSetupColumn("ALTITUDE NAV");
          ImGui::TableSetupColumn("HDG");
          ImGui::TableSetupColumn("HDG NAV");
          ImGui::TableSetupColumn("SEEN P");
          ImGui::TableSetupColumn("SEEN");
          ImGui::TableSetupColumn("RSSI");
          ImGui::TableHeadersRow();
        }
        for (int pos = 0; pos < SDATA.AIRCRAFT_LIST.AIRCRAFTS.size(); pos++)
        {
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
        }
        ImGui::EndTable();
      }
    }
    ImGui::EndChild();

  //}
  //ImGui::End();
}

// ---------------------------------------------------------------------------------------
#endif