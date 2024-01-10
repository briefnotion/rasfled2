// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: nmea.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef NMEA_CPP
#define NMEA_CPP

#include "nmea.h"

using namespace std;

// -------------------------------------------------------------------------------------

/*
In NMEA sentences, the first two letters after the $ denote the Talker ID, which 
identifies the source of the information1.

GP stands for GPS satellites only and is mostly used for integration with legacy 
devices and software2.

GN stands for Global Navigation Satellite System (GNSS), indicating that the data 
comes from a combination of navigation systems3. This is the latest industry standard 
and is required when you use data from multiple satellite systems2.

So, the difference between GP and GN lies in the source of the satellite data they 
represent. GP is specific to GPS, while GN represents a combination of different 
satellite systems.
*/

void NMEA::translate_gnvtg(vector<string> &Input, unsigned long tmeFrame_Time)
{
  // Track made good and ground speed
  //  The NMEA-0183 message $GNVTG is used for course over ground and ground speed.

  if (Input.size() == 10)
  {
    SPEED_KNOTS.store_knots(string_to_float(Input[5]), tmeFrame_Time, 0);
    if (Input[6] == "N")
    {
      SPEED_KNOTS_INDICATOR = true;
    }
    else
    {
      SPEED_KNOTS_INDICATOR = false;
    }

    SPEED_KMPH.store_kmph(string_to_float(Input[7]), tmeFrame_Time, 0);
    if (Input[8] == "K")
    {
      SPEED_KMPH_INDICATOR = true;
    }
    else
    {
      SPEED_KMPH_INDICATOR = false;
    }

    VELOCITY_TRACK_MODE = Input[9];

    if (VELOCITY_TRACK_MODE == "N")
    {
      VALID_TRACK_INFO = false;
    }
    else
    {
      VALID_TRACK_INFO = true;
    }

    TRUE_TRACK = string_to_float(Input[1]);
    if (Input[2] == "T")
    {
      TRUE_TRACK_INDICATOR = true;
    }
    else
    {
      TRUE_TRACK_INDICATOR = false;
    }

    MAGENTIC_TRACK = string_to_float(Input[3]);
    if (Input[4] == "M")
    {
      MAGENTIC_TRACK_INDICATOR = true;
    }
    else
    {
      MAGENTIC_TRACK_INDICATOR = false;
    }

    // Valid track only if v > 1kmph
    //  Filters out zero heading track during coodinate drif.
    if (SPEED_KMPH.val_kmph() < 2.0f)
    {
      VALID_TRACK_INFO = false;
    }

  }
  else
  {
    VALID_TRACK_INFO = false;
  }
}

void NMEA::translate_gngsa(vector<string> &Input)
{
  // GPS DOP and active satellites
  
  if (Input.size() == 18)
  {
    MANUAL_AUTOMATIC = Input[1];
    GSA_MODE = string_to_int(Input[2]);

    PRNNUMBER_01 = string_to_int(Input[3]);
    PRNNUMBER_02 = string_to_int(Input[4]);
    PRNNUMBER_03 = string_to_int(Input[5]);
    PRNNUMBER_04 = string_to_int(Input[6]);
    PRNNUMBER_05 = string_to_int(Input[7]);
    PRNNUMBER_06 = string_to_int(Input[8]);
    PRNNUMBER_07 = string_to_int(Input[9]);
    PRNNUMBER_08 = string_to_int(Input[10]);
    PRNNUMBER_09 = string_to_int(Input[11]);
    PRNNUMBER_10 = string_to_int(Input[12]);
    PRNNUMBER_11 = string_to_int(Input[13]);
    PRNNUMBER_12 = string_to_int(Input[14]);

    PDOP = string_to_float(Input[15]);
    HDOP = string_to_float(Input[16]);
    VDOP = string_to_float(Input[17]);
  }
}

void NMEA::translate_gngga(vector<string> &Input)
{
  if (Input.size() == 15)
  {
    GGA_TIME = string_to_float(Input[1]);
    
    // primarily for file access reloading file (bug, not a feature)
    if (GGA_TIME_PREV > GGA_TIME)
    {
      GGA_TIME_PREV = GGA_TIME;
    }
    
    LATITUDE = string_to_float(Input[2]);
    // Convert long degrees minutes to degrees decimal
    LATITUDE = (int)(LATITUDE / 100.0f) + ( ((LATITUDE / 100.0f) - (int)(LATITUDE / 100.0f)) *100 / 60 );
    if (Input[3] == "S")
    {
      LATITUDE = LATITUDE * -1;
    }

    LONGITUDE = string_to_float(Input[4]);
    // Convert long degrees minutes to degrees decimal
    LONGITUDE = (int)(LONGITUDE / 100.0f) + ( ((LONGITUDE / 100.0f) - (int)(LONGITUDE / 100.0f)) *100 / 60 );
    if (Input[5] == "W")
    {
      LONGITUDE = LONGITUDE * -1;
    }

    QUALITY = string_to_int(Input[6]);
    NUMBER_OF_SATILITES = string_to_int(Input[7]);
    DILUTION_OF_POSITION = string_to_float(Input[8]);

    if (Input[10] == "M")
    {
      ALTITUDE.store_meters(string_to_float(Input[9]));
    }

    if (Input[12] == "M")
    {
      GEOID_HEIGHT.store_meters(string_to_float(Input[11]));
    }

    TIME_SINCE_LAST_DGPS_UPDATE = string_to_float(Input[13]);
    
    string tmp_string = "";

    left_of_char(Input[14], '*', tmp_string);
    DGPS_STATION_ID = string_to_int(tmp_string);

    right_of_char(Input[14], '*', tmp_string);

    // CHECKSUM
    //printf(" %d\n", string_to_int(tmp_string));

  }
  else
  {
    QUALITY = 0;
  }
}
// Data:
float NMEA::pdop()
{
  return PDOP;
}

float NMEA::hdop()
{
  return HDOP;
}

float NMEA::vdop()
{
  return VDOP;
}

// Routines:

string NMEA::device_change_baud_rate_string(int Baud_Rate)
{
  // Unknown what will happen if wrong baud rate entered
  string send_string = "$PUBX,41,1,0007,0003," + to_string(Baud_Rate) + ",0*";
  return (send_string + to_string_hex(xor_checksum(send_string, '$', '*')).c_str());
}

void NMEA::process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time)
{
  // Read New Data
  if (Com_Port.recieve_size() > 0)
  {
    // Working Variables
    vector<string> INPUT_LINE;

    while (Com_Port.recieve_size() > 0)
    {
      // parse
      string working_line = trim(Com_Port.recieve());

      //CHECKSUM
      //printf(" %s\n", working_line.c_str());
      //printf("cs - %s ", to_string_hex(xor_checksum(working_line, '$', '*')).c_str());

      if (Com_Port.PROPS.PRINT_RECEIVED_DATA)
      {
        cons.printw(working_line);
      }

      RECIEVE_HISTORY.push_back(working_line);

      string left = "";

      bool exit_parse_while = false;
      while (exit_parse_while == false)
      {
        if (left_of_char(working_line, ',', left))
        {
          INPUT_LINE.push_back(left);
          right_of_char(working_line, ',', working_line);
        }
        else
        {
          INPUT_LINE.push_back(working_line);
          exit_parse_while = true;
        }
      }

      // translate
      if (INPUT_LINE.size() > 1)
      {
        ACTIVITY_TIMER.ping_up(tmeFrame_Time, 5000);

        if (INPUT_LINE[0] == "$GPVTG" || INPUT_LINE[0] == "$GNVTG")
        {
          // Track made good and ground speed
          translate_gnvtg(INPUT_LINE, tmeFrame_Time);

          // Update Details
          CURRENT_POSITION.TRUE_HEADING = TRUE_TRACK;
          CURRENT_POSITION.SPEED = SPEED_KMPH;
          CURRENT_POSITION.VALID_TRACK = VALID_TRACK_INFO;

          ACTIVITY_TIMER.ping_up(tmeFrame_Time, 5000);
          CURRENT_POSITION.CHANGED = true;
        }

        else if (INPUT_LINE[0] == "$GPGSA" || INPUT_LINE[0] == "$GNGSA")
        {
          // GPS DOP and active satellites
          translate_gngsa(INPUT_LINE);
        }

        else if (INPUT_LINE[0] == "$GPGGA" || INPUT_LINE[0] == "$GNGGA")
        {
          // Global Positioning System Fix Data
          translate_gngga(INPUT_LINE);

          // Update Details
          if (QUALITY > 0)
          {
            CURRENT_POSITION.SYSTEM_UPDATE_TIME = tmeFrame_Time;
            CURRENT_POSITION.VALID_GPS_FIX = true;
            CURRENT_POSITION.LATITUDE = LATITUDE;
            CURRENT_POSITION.LONGITUDE = LONGITUDE;
            CURRENT_POSITION.ALTITUDE = ALTITUDE;
            CURRENT_POSITION.GEOID_HEIGHT = GEOID_HEIGHT;
            CURRENT_POSITION.LATITUDE = LATITUDE;

            CURRENT_POSITION.VALID_COORDS = true;

            // Add new track point if conditions are correct.
            //if (ADD_TRACK_POINT_TIMER.ping_down(tmeFrame_Time) == false)
            if (GGA_TIME - GGA_TIME_PREV >= 1.0f && SPEED_KMPH.val_kmph() >= 2.0f)
            {
              GGA_TIME_PREV = GGA_TIME;

              ADD_TRACK_POINT_TIMER.ping_up(tmeFrame_Time, 60000);

              DETAILED_TRACK_POINT tmp_track_point;

              tmp_track_point.LAT_LON.x = LATITUDE;
              tmp_track_point.LAT_LON.y = LONGITUDE;

              // Velocity color instead of altitude color
              //tmp_track_point.ALTITUDE = ALTITUDE.feet_val();
              tmp_track_point.ALTITUDE = SPEED_KMPH.val_mph();
              
              //(32.0f + Aircraft.RSSI.get_float_value()) / 32.0f;
              tmp_track_point.RSSI_INTENSITY = 0.5f; // Sat sig str?

              TRACK.store(tmp_track_point);

              if ((int)TRACK.size() % 100 == 0)
              {
                printf("size %d\n", TRACK.size());
              }
            }
          }
          else
          {
            CURRENT_POSITION.VALID_GPS_FIX = false;
          }

          ACTIVITY_TIMER.ping_up(tmeFrame_Time, 5000);
          CURRENT_POSITION.CHANGED = true;
        }
      }

      // clear working vector
      INPUT_LINE.clear();
    }
  }
}

GLOBAL_POSITION_DETAILED NMEA::current_position()
{
  return CURRENT_POSITION;
}

void NMEA::current_position_change_acknowleged()
{
  CURRENT_POSITION.CHANGED = false;
}

bool NMEA::active(unsigned long tmeFrame_Time)
{
  return ACTIVITY_TIMER.ping_down(tmeFrame_Time);
}

// -------------------------------------------------------------------------------------


#endif