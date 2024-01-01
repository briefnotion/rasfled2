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

void NMEA::translate_gpgga(vector<string> &Input, GLOBAL_POSITION_VALUE &Position)
{
  if (Input.size() == 15)
  {
    Position.TIME = string_to_float(Input[1]);
    
    Position.LATITUDE = string_to_float(Input[2]);
    // Convert long degrees minutes to degrees decimal
    Position.LATITUDE = (int)(Position.LATITUDE / 100.0f) + ( ((Position.LATITUDE / 100.0f) - (int)(Position.LATITUDE / 100.0f)) *100 / 60 );
    if (Input[3] == "S")
    {
      Position.LATITUDE = Position.LATITUDE * -1;
    }

    Position.LONGITUDE = string_to_float(Input[4]);
    // Convert long degrees minutes to degrees decimal
    Position.LONGITUDE = (int)(Position.LONGITUDE / 100.0f) + ( ((Position.LONGITUDE / 100.0f) - (int)(Position.LONGITUDE / 100.0f)) *100 / 60 );
    if (Input[5] == "W")
    {
      Position.LONGITUDE = Position.LONGITUDE * -1;
    }

    Position.QUALITY = string_to_int(Input[6]);
    Position.NUMBER_OF_SATILITES = string_to_int(Input[7]);
    Position.DILUTION_OF_POSITION = string_to_float(Input[8]);

    if (Input[10] == "M")
    {
      Position.ALTITUDE.store_meters(string_to_float(Input[9]));
    }

    if (Input[12] == "M")
    {
      Position.GEOID_HEIGHT.store_meters(string_to_float(Input[11]));
    }

    Position.TIME_SINCE_LAST_DGPS_UPDATE = string_to_float(Input[13]);
    
    string tmp_string = "";

    left_of_char(Input[14], '*', tmp_string);
    Position.DGPS_STATION_ID = string_to_int(tmp_string);

    right_of_char(Input[14], '*', tmp_string);
    Position.CHECKSUM = string_to_int(tmp_string);

    Position.VALID_COORDS = true;
    Position.CHANGED = true;
  }
  else
  {
    Position.QUALITY = 0;
  }
}

void NMEA::clear_changes()
{
  CURRENT_POSITION.CHANGED = false;
}

//void NMEA::process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time)
void NMEA::process(COMPORT &Com_Port)
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
        if (INPUT_LINE[0] == "$GPGGA" || INPUT_LINE[0] == "$GNGGA")
        {
          translate_gpgga(INPUT_LINE, CURRENT_POSITION);
        }
      }

      // clear woring vector
      INPUT_LINE.clear();
    }
  }
}

GLOBAL_POSITION_VALUE NMEA::current_position()
{
  return CURRENT_POSITION;
}

// -------------------------------------------------------------------------------------


#endif