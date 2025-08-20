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
#include <vector>

// RASFled related header files
#include "globe_helper.h"
#include "stringthings.h"
#include "screen4_helper.h"
#include "comport.h"

// -------------------------------------------------------------------------------------

class NMEA_PROPERTIES
{
  public:
  bool TRUE_TRACK_ASSIST = true;
};

class NMEA
{
  private:

  TIMED_PING ACTIVITY_TIMER;

  // ALL CURRENT STATUSs

  // -------------------------------------------------------------------------------------
  // Track made good and ground speed

  float TRUE_TRACK_PREV = 0.0f;
  float TRUE_TRACK = 0.0f;
  float TRUE_TRACK_ASSIST = 0.0f;
  
  bool TRUE_TRACK_INDICATOR = false;

  float MAGENTIC_TRACK = 0.0f;
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

  // -------------------------------------------------------------------------------------
  // GPS DOP and active satellites
  // Dilution of Precision
  // $GNGSA

  string MANUAL_AUTOMATIC = "";
  
  int GSA_MODE = 0;
    //  Fix type: 1 = not available, 2 = 2D, 3 = 3D

    //  01 to 32 for GPS, 33 to 64 for SBAS, 64+ for GLONASS
  int PRNNUMBER_01 = 0;
  int PRNNUMBER_02 = 0;
  int PRNNUMBER_03 = 0;
  int PRNNUMBER_04 = 0;
  int PRNNUMBER_05 = 0;
  int PRNNUMBER_06 = 0;
  int PRNNUMBER_07 = 0;
  int PRNNUMBER_08 = 0;
  int PRNNUMBER_09 = 0;
  int PRNNUMBER_10 = 0;
  int PRNNUMBER_11 = 0;
  int PRNNUMBER_12 = 0;

  float PDOP = 0.0f;  // .5 - 99.9
  float HDOP = 0.0f;  // .5 - 99.9
  float VDOP = 0.0f;  // .5 - 99.9

  // if NMEA-0183 version 4.10, 7th and 8th fields:
  //int SYSTEM_ID = 0;
    //  GPS 1, GLONASS 2, Galileo 3, BeiDou 4, QZSS 0

  // -------------------------------------------------------------------------------------
  // Global Positioning System Fix Data

  double LATITUDE = 0;
  double LONGITUDE = 0;
  DISTANCE ALTITUDE;     // M
  DISTANCE GEOID_HEIGHT; // M (mean sea level above WGS84 ellipsoid)

  float GGA_TIME = 0;
  float GGA_TIME_PREV = 0;
  
  float TIME_SINCE_LAST_DGPS_UPDATE = 0;
  int DGPS_STATION_ID;

  int NUMBER_OF_SATILITES = 0;
  float DILUTION_OF_POSITION = 0;

  int QUALITY = 0;  // 0 - 8;

  // -------------------------------------------------------------------------------------
  // NO CONSOLIDATION OCCURS
  // -------------------------------------------------------------------------------------

  bool CHANGED = false;

  GLOBAL_POSITION_DETAILED CURRENT_POSITION;

  // Routines:
  void translate_gnvtg(vector<string> &Input, unsigned long tmeFrame_Time);   //  Track made good and ground speed
  void translate_gngsa(vector<string> &Input);    //  GPS DOP and active satellites 
  void translate_gngga(vector<string> &Input);    //  Global Positioning System Fix Data
  //void translate_gpgsv(vector<string> &Input);  //  GPS Satellites in view
  //void translate_gngll(vector<string> &Input);  //  Geographic Position, Latitude/Longitude
  //void translate_gnrmc(vector<string> &Input);  //  Recommended minimum specific GPS/Transit data

  TIMED_PING ADD_TRACK_POINT_TIMER;
  
  public:

  NMEA_PROPERTIES PROPS;

  DETAILED_TRACK TRACK;

  // TEMPORARY _ ERASE AS SOON AS CONSOLE IS GONE.
  vector<string> RECIEVE_HISTORY;

  // Data:
  float pdop();
  float hdop();
  float vdop();

  // Routines:
  string device_change_baud_rate_string(int Baud_Rate);
  
  void process(CONSOLE_COMMUNICATION &cons, COMPORT &Com_Port, unsigned long tmeFrame_Time);

  GLOBAL_POSITION_DETAILED current_position();
  void current_position_change_acknowleged();

  bool active(unsigned long tmeFrame_Time);
};

// -------------------------------------------------------------------------------------


#endif