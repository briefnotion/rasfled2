// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: nmea.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef NMEA_H
#define NMEA_H

// Standard Header Files
//#include <stdio.h>
//#include <string>
#include <vector>

// RASFled related header files
//#include "system.h"
//#include "commands.h"

#include "globe_helper.h"
#include "stringthings.h"
#include "screen4_helper.h"
#include "comport.h"

// -------------------------------------------------------------------------------------

class NMEA
{
  private:

  // ALL CURRENT STATUSs

  //  Ground Speed and Heading
  float TRUE_TRACK = 0;
  bool TRUE_TRACK_INDICATOR = false;

  float MAGENTIC_TRACK = 0;
  bool MAGENTIC_TRACK_INDICATOR = false;

  VELOCITY SPEED_KNOTS;
  bool SPEED_KNOTS_INDICATOR = false;

  VELOCITY SPEED_KMPH;
  bool SPEED_KMPH_INDICATOR = false;

  string VELOCITY_TRACK_MODE = "N";
  // Positioning system mode indicator. 
  // A = Autonomous, 
  // D = Differential, 
  // E = Estimated (dead reckoning) mode, 
  // M = Manual input, 
  // N = Data not valid

  bool VALID_TRACK_INFO = false;

  //  Global Posion
  float LATITUDE = 0;
  float LONGITUDE = 0;
  DISTANCE ALTITUDE;     // M
  DISTANCE GEOID_HEIGHT; // M (mean sea level above WGS84 ellipsoid)

  float GGA_TIME = 0;
  
  float TIME_SINCE_LAST_DGPS_UPDATE = 0;
  int DGPS_STATION_ID;

  int NUMBER_OF_SATILITES = 0;
  float DILUTION_OF_POSITION = 0;

  int QUALITY = 0;  // 0 - 8;

  // ---
  // NO CONSOLIDATION OCCURS
  // ---

  bool VALID_COORDS = false;
  bool CHANGED = false;

  GLOBAL_POSITION_DETAILED CURRENT_POSITION;

  //void translate_gpgsv(vector<string> &Input);
  void translate_gpvtg(vector<string> &Input, unsigned long tmeFrame_Time);
  //void translate_gpgsa(vector<string> &Input);
  void translate_gpgga(vector<string> &Input);

  public:

  // TEMPORARY _ ERASE AS SOON AS CONSOLE IS GONE.
  vector<string> RECIEVE_HISTORY;

  void clear_changes();

  //void process(CONSOLE_COMMUNICATION &cons, ALERT_SYSTEM_2 &ALERTS_2, COMPORT &Com_Port, unsigned long tmeFrame_Time);
  void process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time);

  GLOBAL_POSITION_DETAILED current_position();
};

// -------------------------------------------------------------------------------------


#endif