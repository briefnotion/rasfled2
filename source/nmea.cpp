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

/**
 * @brief Calculates a normalized accuracy score based on DOP values.
 *
 * This function takes a vector of DOP values (PDOP, HDOP, VDOP) and
 * returns a float value between 0.0 and 1.0. The score is calculated as follows:
 * - An average DOP less than 1.0 returns a perfect score of 1.0.
 * - An average DOP between 1.0 and 10.0 is scaled linearly, where 1.0 returns
 * 1.0 and 10.0 returns 0.0.
 * - An average DOP of 10.0 or greater returns a score of 0.0.
 *
 * @param dop_values A vector of float values representing DOP metrics.
 * @return A float representing the normalized accuracy score (0.0 to 1.0).
 */
float NMEA::calculate_accuracy_score()
{
  // Calculate the average of the DOP values.
  float average_dop = PDOP + HDOP + VDOP / 3.0f;

  // Normalize the score based on the average DOP.
  if (average_dop == 0.0f) 
  {
    // Score 0 should be impossible, assume no values
    return 0.0f;
  }
  else if (average_dop <= 1.0f) 
  {
    // Ideal case: score is 1.0.
    return 1.0f;
  } 
  else if (average_dop > 1.0f && average_dop < 10.0f) 
  {
    // Linear scaling from 1.0 to 0.0.
    // Formula: 1.0 - ((average_dop - 1.0) / (10.0 - 1.0))
    return 1.0f - ((average_dop - 1.0f) / 9.0f);
  } else 
  {
    // Average DOP is 10.0 or greater: score is 0.0.
    return 0.0f;
  }
}

/**
 * @brief Converts a UTC date and time from NMEA format into a Unix epoch timestamp.
 *
 * This function takes the date (DDMMYY) and time (HHMMSS.ss) as separate
 * numeric values and returns a single double representing the Unix epoch
 * timestamp with sub-second precision. It uses timegm to ensure the conversion
 * is based on UTC, not the local system time.
 *
 * @param utc_date An integer representing the UTC date in DDMMYY format (e.g., 130625 for June 13, 2025).
 * @param utc_time A float representing the UTC time in HHMMSS.ss format (e.g., 235354.00).
 * @return The Unix epoch timestamp as a double.
 */
double NMEA::unix_epoch_nmea_time(int utc_date, float utc_time) {
    // --- Step 1: Parse the UTC Date ---
    // The date format is DDMMYY. We need to extract each component.
    int day = utc_date / 10000;
    int month = (utc_date / 100) % 100;
    int year = utc_date % 100;

    // --- Step 2: Parse the UTC Time ---
    // The time format is HHMMSS.ss. We need to extract each component.
    int hours = static_cast<int>(utc_time / 10000);
    int minutes = static_cast<int>(utc_time / 100) % 100;
    int seconds = static_cast<int>(utc_time) % 100;
    double fractional_seconds = utc_time - static_cast<int>(utc_time);

    // --- Step 3: Populate a struct tm ---
    // This structure is used by C functions to handle date and time.
    std::tm time_struct = {};
    time_struct.tm_year = year + 100; // tm_year is years since 1900
    time_struct.tm_mon = month - 1;   // tm_mon is 0-11
    time_struct.tm_mday = day;        // tm_mday is 1-31
    time_struct.tm_hour = hours;      // tm_hour is 0-23
    time_struct.tm_min = minutes;     // tm_min is 0-59
    time_struct.tm_sec = seconds;     // tm_sec is 0-60

    // --- Step 4: Convert to UTC time_t using timegm ---
    // timegm() converts the time structure to a Unix epoch timestamp,
    // assuming the input time is UTC.
    std::time_t time_t_value = timegm(&time_struct);

    // --- Step 5: Add the fractional seconds for high precision ---
    // The Unix epoch timestamp is the total number of seconds.
    // We add the fractional part to the time_t value to get a double with precision.
    double unix_timestamp = static_cast<double>(time_t_value) + fractional_seconds;

    return unix_timestamp;
}

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
  // tmp vars
  float tmp_float = 0.0f;

  // Track made good and ground speed
  //  The NMEA-0183 message $GNVTG is used for course over ground and ground speed.

  if (Input.size() == 10)
  {
    if (string_to_value(Input[5], tmp_float))
    {
      SPEED_KNOTS.store_knots(tmp_float, tmeFrame_Time, 0);
      if (Input[6] == "N")
      {
        SPEED_KNOTS_INDICATOR = true;
      }
      else
      {
        SPEED_KNOTS_INDICATOR = false;
      }
    }

    if(string_to_value(Input[7], tmp_float))
    {
      SPEED_KMPH.store_kmph(tmp_float, tmeFrame_Time, 0);
      if (Input[8] == "K")
      {
        SPEED_KMPH_INDICATOR = true;
      }
      else
      {
        SPEED_KMPH_INDICATOR = false;
      }
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

    if(string_to_value(Input[1], tmp_float))
    {
      TRUE_TRACK = tmp_float;
      if (Input[2] == "T")
      {
        TRUE_TRACK_INDICATOR = true;
      }
      else
      {
        TRUE_TRACK_INDICATOR = false;
      }
    }

    if(string_to_value(Input[3], tmp_float))
    {
      MAGENTIC_TRACK = tmp_float;
      if (Input[4] == "M")
      {
        MAGENTIC_TRACK_INDICATOR = true;
      }
      else
      {
        MAGENTIC_TRACK_INDICATOR = false;
      }
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
  // tmp vars
  int tmp_int = 0;
  float tmp_float = 0.0f;

  // GPS DOP and active satellites
  
  if (Input.size() == 18)
  {
    MANUAL_AUTOMATIC = Input[1];
    if(string_to_value(Input[2], tmp_int))
    {
      GSA_MODE = tmp_int;
    }

    if(string_to_value(Input[3], tmp_int))
    {
      PRNNUMBER_01 = tmp_int;
    }
    if(string_to_value(Input[4], tmp_int))
    {
      PRNNUMBER_02 = tmp_int;
    }
    if(string_to_value(Input[5], tmp_int))
    {
      PRNNUMBER_03 = tmp_int;
    }
    if(string_to_value(Input[6], tmp_int))
    {
      PRNNUMBER_04 = tmp_int;
    }
    if(string_to_value(Input[7], tmp_int))
    {
      PRNNUMBER_05 = tmp_int;
    }
    if(string_to_value(Input[8], tmp_int))
    {
      PRNNUMBER_06 = tmp_int;
    }
    if(string_to_value(Input[9], tmp_int))
    {
      PRNNUMBER_07 = tmp_int;
    }
    if(string_to_value(Input[10], tmp_int))
    {
      PRNNUMBER_08 = tmp_int;
    }
    if(string_to_value(Input[11], tmp_int))
    {
      PRNNUMBER_09 = tmp_int;
    }
    if(string_to_value(Input[12], tmp_int))
    {
      PRNNUMBER_10 = tmp_int;
    }
    if(string_to_value(Input[13], tmp_int))
    {
      PRNNUMBER_11 = tmp_int;
    }
    if(string_to_value(Input[14], tmp_int))
    {
      PRNNUMBER_12 = tmp_int;
    }

    if(string_to_value(Input[15], tmp_float))
    {
      PDOP = tmp_float;
    }
    if(string_to_value(Input[16], tmp_float))
    {
      HDOP = tmp_float;
    }
    if(string_to_value(Input[17], tmp_float))
    {
      VDOP = tmp_float;
    }
    ACCURACY_SCORE = calculate_accuracy_score();
  }
}

void NMEA::translate_gngga(vector<string> &Input)
{
  // tmp vars
  int tmp_int = 0;
  float tmp_float = 0.0f;
  double tmp_double = 0.0;

  if (Input.size() == 15)
  {
    // primarily for file access reloading file (bug, not a feature)
    if(string_to_value(Input[1], tmp_float))
    {
      GGA_TIME = tmp_float;
      if (GGA_TIME_PREV > GGA_TIME)
      {
        GGA_TIME_PREV = GGA_TIME;
      }
    }
   
    // Convert long degrees minutes to degrees decimal
    if(string_to_value(Input[2], tmp_double))
    {
      LATITUDE = tmp_double;
      LATITUDE = (int)(LATITUDE / 100.0f) + ( ((LATITUDE / 100.0f) - (int)(LATITUDE / 100.0f)) *100 / 60 );
      if (Input[3] == "S")
      {
        LATITUDE = LATITUDE * -1;
      }
    }

    // Convert long degrees minutes to degrees decimal
    if(string_to_value(Input[4], tmp_double))
    {
      LONGITUDE = tmp_double;
      LONGITUDE = (int)(LONGITUDE / 100.0f) + ( ((LONGITUDE / 100.0f) - (int)(LONGITUDE / 100.0f)) *100 / 60 );
      if (Input[5] == "W")
      {
        LONGITUDE = LONGITUDE * -1;
      }
    }

    if(string_to_value(Input[6], tmp_int))
    {
      QUALITY = tmp_int;
    }

    if(string_to_value(Input[7], tmp_int))
    {
      NUMBER_OF_SATILITES = tmp_int;
    }

    if(string_to_value(Input[8], tmp_float))
    {
      DILUTION_OF_POSITION = tmp_float;
    }

    if (Input[10] == "M")
    {
      if(string_to_value(Input[9], tmp_float))
      {
        ALTITUDE.store_meters(tmp_float);
      }
    }

    if (Input[12] == "M")
    {
      if(string_to_value(Input[11], tmp_float))
      {
        GEOID_HEIGHT.store_meters(tmp_float);
      }
    }

    if(string_to_value(Input[13], tmp_float))
    {
      TIME_SINCE_LAST_DGPS_UPDATE = tmp_float;
    }
    
    string tmp_string = "";

    left_of_char(Input[14], '*', tmp_string);
    if(string_to_value(tmp_string, tmp_int))
    {
      DGPS_STATION_ID = tmp_int;
    }
    else
    {
      DGPS_STATION_ID = 0;
    }

    right_of_char(Input[14], '*', tmp_string);

    // CHECKSUM
    //printf(" %d\n", string_to_int(tmp_string));

  }
  else
  {
    QUALITY = 0;
  }
}

void NMEA::translate_gnrmc(vector<string> &Input)
{
  // tmp vars
  int tmp_int = 0;
  float tmp_float = 0.0f;

  if (Input.size() == 13)
  {
    // $GNRMC,235356.00,A,3014.84041,N,09208.86217,W,57.991,90.37,130625,,,A*6D

    // primarily for file access reloading file (bug, not a feature)
    if(string_to_value(Input[1], tmp_float))
    {
      UTC_TIME = tmp_float;
    }

    // Status. "A" means the data is valid or "Active." A "V" would mean it's invalid.
    //if(string_to_value(Input[2], tmp_float))

    //  Latitude. The position is at 30 degrees, 14.84041 minutes North.
    //if(string_to_value(Input[3], tmp_float))

    // North.
    //if(string_to_value(Input[4], tmp_float))

    // Longitude. The position is at 92 degrees, 08.86217 minutes West.
    //if(string_to_value(Input[5], tmp_float))

    // West.
    //if(string_to_value(Input[6], tmp_float))

    // Speed over ground. The speed is 57.991 knots.
    //if(string_to_value(Input[7], tmp_float))

    // True course. The vessel is heading on a course of 90.37 degrees.
    //if(string_to_value(Input[8], tmp_float))
   
    // UTC Date. This is the date of the fix: 13th day of the 06th month (June) of the year '25.
    if(string_to_value(Input[9], tmp_int))
    {
      UTC_DATE = tmp_int;
    }

    // Magnetic Variation. This field is empty in your sentence, which is common.
    //if(string_to_value(Input[10], tmp_float))

    // Magnetic Variation Direction. This field is also empty.
    //if(string_to_value(Input[11], tmp_float))

    // Mode Indicator. "A" means the fix is from an "Autonomous" GNSS system. Other modes include D (Differential), E (Estimated), and N (No Fix).
    //if(string_to_value(Input[12], tmp_float))

    // The checksum for data integrity.
    //if(string_to_value(Input[13], tmp_float))

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

float NMEA::accuracy_score()
{
  return ACCURACY_SCORE;
}

int NMEA::satilite_count()
{
  return NUMBER_OF_SATILITES;
}

// Routines:

string NMEA::device_change_baud_rate_string(int Baud_Rate)
{
  // Unknown what will happen if wrong baud rate entered
  string send_string = "$PUBX,41,1,0007,0003," + to_string(Baud_Rate) + ",0*";
  return (send_string + to_string_hex(xor_checksum(send_string, '$', '*')).c_str());
}

void NMEA::load_track(CONSOLE_COMMUNICATION &cons, MAP &Current_map)
{
  if (Current_map.load_track(TRACK, PROPS.CURRENT_TRACK_FILENAME))
  {
    cons.printw("Successfully saved \"" + PROPS.CURRENT_TRACK_FILENAME + "\"\n");
  }
  else
  {
    cons.printw("Failed to save \"" + PROPS.CURRENT_TRACK_FILENAME + "\"\n");
  }
}

void NMEA::process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time, MAP &Current_map)
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
      string l = working_line;

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
        if (INPUT_LINE[0] == "$GPVTG" || INPUT_LINE[0] == "$GNVTG")
        {
          // Track made good and ground speed
          translate_gnvtg(INPUT_LINE, tmeFrame_Time);

          // Update Details
          CURRENT_POSITION.SPEED = SPEED_KMPH;
          CURRENT_POSITION.VALID_TRACK = VALID_TRACK_INFO;

          // Store track as track assist or track
          if (PROPS.TRUE_TRACK_ASSIST)
          {
            // Determine TRUE_TRACK_ASSIST
            float TRACK_DIFFERENCE = signed_angular_error(TRUE_TRACK, TRUE_TRACK_PREV);
            if (TRACK_DIFFERENCE > -180.0f && TRACK_DIFFERENCE < 180.0f)
            {
              TRUE_TRACK_ASSIST = TRUE_TRACK + TRACK_DIFFERENCE;
            }
            else
            {
              TRUE_TRACK_ASSIST = TRUE_TRACK;
            }

            TRUE_TRACK_PREV = TRUE_TRACK;   // Store prev track heading
            CURRENT_POSITION.TRUE_HEADING.VALUE = TRUE_TRACK_ASSIST;
          }
          else
          {
            CURRENT_POSITION.TRUE_HEADING.VALUE = TRUE_TRACK;
          }

          // Set Track Valid if speed > min
          if (SPEED_KMPH.val_mph() > 10.0f)
          {
            CURRENT_POSITION.TRUE_HEADING.VALID = true;
          }
          else
          {
            CURRENT_POSITION.TRUE_HEADING.VALID = false;
          }

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

            CURRENT_POSITION.VALID_COORDS = true;

            // Add new track point if conditions are correct.
            //if (ADD_TRACK_POINT_TIMER.ping_down(tmeFrame_Time) == false)
            if (GGA_TIME - GGA_TIME_PREV >= 1.0f && SPEED_KMPH.val_kmph() >= 2.0f)
            {
              GGA_TIME_PREV = GGA_TIME;

              ADD_TRACK_POINT_TIMER.ping_up(tmeFrame_Time, 60000);

              DETAILED_TRACK_POINT tmp_track_point;

              tmp_track_point.TIMESTAMP = UNIX_EPOC_NMEA_TIME;

              tmp_track_point.LATITUDE = LATITUDE;
              tmp_track_point.LONGITUDE = LONGITUDE;

              tmp_track_point.TRUE_HEADING = TRUE_TRACK;

              // Velocity color instead of altitude color
              //tmp_track_point.ALTITUDE = ALTITUDE.feet_val();
              tmp_track_point.ALTITUDE = SPEED_KMPH.val_mph();
              
              //DILUTION_OF_POSITION ranges 0 - 100.
              // 0.5 to 2.5 is good to bad.
              tmp_track_point.ACCURACY = ACCURACY_SCORE;

              TRACK.store(tmp_track_point);
            }
          }
          else
          {
            CURRENT_POSITION.VALID_GPS_FIX = false;
          }

          ACTIVITY_TIMER.ping_up(tmeFrame_Time, 5000);
          CURRENT_POSITION.CHANGED = true;
        }

        else if (INPUT_LINE[0] == "$GPRMC" || INPUT_LINE[0] == "$GNRMC")
        {
          // Global Positioning System Fix Data
          translate_gnrmc(INPUT_LINE);

          UNIX_EPOC_NMEA_TIME = unix_epoch_nmea_time(UTC_DATE, UTC_TIME);
        }
      }

      // clear working vector
      INPUT_LINE.clear();
    }
  }

  // Save Track
  if (SAVE_TRACK_TIMER.is_ready(tmeFrame_Time))
  {
    SAVE_TRACK_TIMER.set(tmeFrame_Time, PROPS.SAVE_TRACK_TIMER);
    if (Current_map.save_track(TRACK, PROPS.CURRENT_TRACK_FILENAME))
    {
      cons.printw("Successfully saved \"" + PROPS.CURRENT_TRACK_FILENAME + "\"\n");
    }
    else
    {
      cons.printw("Failed to save \"" + PROPS.CURRENT_TRACK_FILENAME + "\"\n");
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
  return ACTIVITY_TIMER.ping_down(tmeFrame_Time) && CURRENT_POSITION.VALID_COORDS;
}

double NMEA::unix_epoch_nmea_time()
{
  return UNIX_EPOC_NMEA_TIME;
}

// -------------------------------------------------------------------------------------


#endif