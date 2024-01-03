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

//void NMEA::translate_gpgsv(vector<string> &Input)
//{
//}

void NMEA::translate_gpvtg(vector<string> &Input, unsigned long tmeFrame_Time)
{
  //  The NMEA-0183 message $GNVTG is used for course over ground and ground speed.
  //  $GNVTG,131.32,T,,M,6.353,N,11.766,K,D*10
  if (Input.size() == 10)
  {
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
  }
  else
  {
    VALID_TRACK_INFO = false;
  }
}

//void NMEA::translate_gpgsa(vector<string> &Input)
//{
  //  GPS DOP (Dilution of Precision)
//}

void NMEA::translate_gpgga(vector<string> &Input)
{
  if (Input.size() == 15)
  {
    GGA_TIME = string_to_float(Input[1]);
    
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
    //CHECKSUM = string_to_int(tmp_string);
  }
  else
  {
    QUALITY = 0;
  }
}

void NMEA::clear_changes()
{
  CURRENT_POSITION.CHANGED = false;
}

void NMEA::process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time)
{
  if (Com_Port.recieve_size() > 0)
  {
    /*
    // Activity Checks for other parts of the program to reference.
    if (AVAILABILITY.check_for_live_data(tmeFrame_Time) == false)
    {
      if (AVAILABILITY.set_active(STATUS, false, tmeFrame_Time) == true)
      {
        CHANGED = true;
      }
    }
    */

    // Working Variables
    vector<string> INPUT_LINE;

    while (Com_Port.recieve_size() > 0)
    {
      // parse
      string working_line = trim(Com_Port.recieve());

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
        if (INPUT_LINE[0] == "$GPVTG" || INPUT_LINE[0] == "$GNVTG")
        {
          // Ground Speed
          translate_gpvtg(INPUT_LINE, tmeFrame_Time);

          // Update Details
          CURRENT_POSITION.TRUE_HEADING = TRUE_TRACK;
          CURRENT_POSITION.SPEED = SPEED_KMPH;
          CURRENT_POSITION.VALID_TRACK = VALID_TRACK_INFO;
          
          CURRENT_POSITION.CHANGED = true;
        }

        else if (INPUT_LINE[0] == "$GPGGA" || INPUT_LINE[0] == "$GNGGA")
        {
          // Global Position
          translate_gpgga(INPUT_LINE);

          // Update Details
          if (QUALITY > 0)
          {
            CURRENT_POSITION.SYSTTEM_UPDATE_TIME = tmeFrame_Time;
            CURRENT_POSITION.LIVE = true;
            CURRENT_POSITION.LATITUDE = LATITUDE;
            CURRENT_POSITION.LONGITUDE = LONGITUDE;
            CURRENT_POSITION.ALTITUDE = ALTITUDE;
            CURRENT_POSITION.GEOID_HEIGHT = GEOID_HEIGHT;
            CURRENT_POSITION.LATITUDE = LATITUDE;

            CURRENT_POSITION.VALID_COORDS = true;
          }
          else
          {
            CURRENT_POSITION.LIVE = false;
          }

          CURRENT_POSITION.CHANGED = true;
        }
      }

      // clear woring vector
      INPUT_LINE.clear();
    }
  }
}

GLOBAL_POSITION_DETAILED NMEA::current_position()
{
  return CURRENT_POSITION;
}

// -------------------------------------------------------------------------------------


#endif