// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: map.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef MAP_CPP
#define MAP_CPP

#include "map.h"

using namespace std;

// -------------------------------------------------------------------------------------

void MAP_INFO::clear()
{
  LAT_LON = DOUBLE_VEC2(0.0, 0.0);
  DISPLAY_NAME = "";
  LONG_NAME = "";
  TYPE = MAP_POINT_TYPE_GENERIC;
  AIRPORT_LANDING_VECTORS.clear();
  REGION_GPS_COORDS.clear();
}

// -------------------------------------------------------------------------------------

void MAP::add_landmark(DOUBLE_VEC2 Lat_Lon, string Display_Name, int Type)
{
  MAP_INFO new_landmark;

  new_landmark.LAT_LON = Lat_Lon;
  new_landmark.DISPLAY_NAME = Display_Name;
  new_landmark.TYPE = Type;

  LANDMARKS.push_back(new_landmark);
}

bool MAP::map_hardcoded_load()
{
  bool ret_suc = false;

  // Add Landmarks
  MAP_INFO tmp_map_marker;

  // add_landmark(ImVec2(30.205f, -91.987833f), "LFY", 0);             // KLFT - Lafayette Regional/Paul Fournet Field Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.205f, -91.987833f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.4663333333f, -92.4238333333f);
  tmp_map_marker.DISPLAY_NAME = "4R7";
  tmp_map_marker.LONG_NAME = "Eunice Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(160.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(340.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2426666667f, -92.6735f), "3R7", 0);        //
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.2426666667f, -92.6735f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.533f, -91.1498333333f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.0378333333f, -91.8838333333f);
  tmp_map_marker.DISPLAY_NAME = "ARA";
  tmp_map_marker.LONG_NAME = "Acadiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.2418333333f, -92.8306666667f), "6R1", 0);  // 6R1 - Welsh Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.2418333333f, -92.8306666667f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.126f, -93.2235f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.7505f, -92.6885f);
  tmp_map_marker.DISPLAY_NAME = "ACP";
  tmp_map_marker.LONG_NAME = "Allen Parish Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.9566666667f, -92.2341666667f), "2R6", 0);  // 2R6 - Bunkie Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.9566666667f, -92.2341666667f);
  tmp_map_marker.DISPLAY_NAME = "2R6";
  tmp_map_marker.LONG_NAME = "Bunkie Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.3273333333f, -92.5485f), "AEX", 0);        // AEX - Alexandria International Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(31.3273333333f, -92.5485f);
  tmp_map_marker.DISPLAY_NAME = "AEX";
  tmp_map_marker.LONG_NAME = "Alexandria International Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(140.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(320.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.7183333333f, -91.4786666667f), "HZR", 0);  // HZR - False River Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.7183333333f, -91.4786666667f);
  tmp_map_marker.DISPLAY_NAME = "HZR";
  tmp_map_marker.LONG_NAME = "False River Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.1713333333f, -90.9403333333f), "REG", 0);  // REG - Louisiana Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.1713333333f, -90.9403333333f);
  tmp_map_marker.DISPLAY_NAME = "REG";
  tmp_map_marker.LONG_NAME = "Louisiana Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.5216666667f, -90.4183333333f), "HDC", 0);  // HDC - Hammond Northshore Regional Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.5216666667f, -90.4183333333f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.445f, -89.9888333333f);
  tmp_map_marker.DISPLAY_NAME = "L31";
  tmp_map_marker.LONG_NAME = "St Tammany Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.3463333333f, -89.8208333333f), "ASD", 0);  // ASD - Slidell Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.3463333333f, -89.8208333333f);
  tmp_map_marker.DISPLAY_NAME = "ASD";
  tmp_map_marker.LONG_NAME = "Slidell Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.4875f, -89.6511666667f), "MJD", 0);        // MJD - Picayune Municipal Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.4875f, -89.6511666667f);
  tmp_map_marker.DISPLAY_NAME = "MJD";
  tmp_map_marker.LONG_NAME = "Picayune Municipal Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(30.0425f, -90.0281666667f), "NEW", 0);        // NEW - Lakefront Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.0425f, -90.0281666667f);
  tmp_map_marker.DISPLAY_NAME = "NEW";
  tmp_map_marker.LONG_NAME = "Lakefront Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(180.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(360.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(29.8271666667f, -90.0266666667f), "NBG", 0);  // NBG - New Orleans NAS Jrb (Alvin Callender Field) Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(29.8271666667f, -90.0266666667f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(29.9933333333f, -90.259f);
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
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.0875f, -90.5828333333f);
  tmp_map_marker.DISPLAY_NAME = "APS";
  tmp_map_marker.LONG_NAME = "Port of South Louisiana Exec Regional Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // add_landmark(ImVec2(31.1785f, -90.4718333333f), "MCB", 0);        // MCB - Mc Comb/Pike County/John E Lewis Field Airport
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(31.1785f, -90.4718333333f);
  tmp_map_marker.DISPLAY_NAME = "MCB";
  tmp_map_marker.LONG_NAME = "Mc Comb/Pike County/John E Lewis Field Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(170.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(350.0f);
  LANDMARKS.push_back(tmp_map_marker);
  
  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(29.9821666667f, -92.0871666667f);
  tmp_map_marker.DISPLAY_NAME = "IYA";
  tmp_map_marker.LONG_NAME = "Abbeville Chris Crusta Memorial Airport";
  tmp_map_marker.TYPE = 1;
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(159.0f);
  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(339.0f);
  LANDMARKS.push_back(tmp_map_marker);

  // Regions

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.171892f, -92.021630f);
  tmp_map_marker.DISPLAY_NAME = "LAFAYETTE";
  tmp_map_marker.LONG_NAME = "Lafayette";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.256323f, -91.990737f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.209173f, -91.975376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.192592f, -91.987432f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.160652f, -91.951085f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.108378f, -91.920498f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.069544f, -91.979683f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.073432f, -92.023356f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.100806f, -92.026423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.137705f, -92.069318f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.131181f, -92.105622f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.189960f, -92.081542f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.191015f, -92.113194f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.262811f, -92.133407f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.260867f, -92.089104f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.249578f, -92.091143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247825f, -92.046105f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.258446f, -92.021630f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.465774f, -91.155707f);
  tmp_map_marker.DISPLAY_NAME = "BATON ROUGE";
  tmp_map_marker.LONG_NAME = "Baton Rouge";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.605345f, -91.149123f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.532499f, -91.095232f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.526884f, -91.122601f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.504312f, -91.042169f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.471935f, -91.033231f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.462739f, -90.995275f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.449013f, -90.992849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.436054f, -91.037770f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418108f, -91.036161f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.409920f, -90.971694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.411330f, -90.972019f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.348683f, -90.981922f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.350909f, -91.144685f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.409352f, -91.198959f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.428363f, -91.194472f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.543076f, -91.199386f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.605002f, -91.185916f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.218697f, -92.375267f);
  tmp_map_marker.DISPLAY_NAME = "CROWLEY";
  tmp_map_marker.LONG_NAME = "Crowley";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.240594f, -92.362782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.231363f, -92.360189f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.230868f, -92.371161f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.227915f, -92.370526f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.227074f, -92.362757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.219310f, -92.358634f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.213647f, -92.359365f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.213433f, -92.350869f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.205640f, -92.350325f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.204999f, -92.356780f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.201381f, -92.357070f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.201009f, -92.362556f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.197011f, -92.362773f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.197899f, -92.370694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.190975f, -92.371303f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.191711f, -92.380043f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.200629f, -92.379919f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.205035f, -92.383075f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.199896f, -92.393879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.214584f, -92.403712f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.222032f, -92.386592f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235882f, -92.390233f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235987f, -92.395576f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.243646f, -92.393876f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.243016f, -92.382460f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.241127f, -92.382946f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.232104f, -92.272845f);
  tmp_map_marker.DISPLAY_NAME = "RAYNE";
  tmp_map_marker.LONG_NAME = "Rayne";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.263934f, -92.263041f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.253550f, -92.261335f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.252656f, -92.255841f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.242865f, -92.254478f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.242267f, -92.248640f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235740f, -92.247274f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235451f, -92.256546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.225365f, -92.256557f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.224185f, -92.264798f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.214397f, -92.267211f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.225086f, -92.280935f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.253266f, -92.279538f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.222796f, -92.681546f);
  tmp_map_marker.DISPLAY_NAME = "JENNINGS";
  tmp_map_marker.LONG_NAME = "Jennings";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246112f, -92.635782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.199958f, -92.635548f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.199327f, -92.684337f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246110f, -92.684109f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.216662f, -93.242004f);
  tmp_map_marker.DISPLAY_NAME = "LAKE CHARLES";
  tmp_map_marker.LONG_NAME = "Lake Charles";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.264687f, -93.174817f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.217717f, -93.140827f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.147629f, -93.242207f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.173839f, -93.291604f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.533170f, -92.101464f);
  tmp_map_marker.DISPLAY_NAME = "OPELOUSAS";
  tmp_map_marker.LONG_NAME = "Opelousas";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.552350f, -92.066830f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.542738f, -92.065975f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.527506f, -92.068727f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.519371f, -92.063407f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.506949f, -92.067187f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.497337f, -92.068735f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.495411f, -92.056721f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.488609f, -92.060156f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.494530f, -92.088988f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.509762f, -92.085210f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.513311f, -92.102376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.532536f, -92.107356f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.548952f, -92.098257f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.551170f, -92.090529f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.550874f, -92.086064f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.557085f, -92.083831f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(31.295272f, -92.485128f);
  tmp_map_marker.DISPLAY_NAME = "ALEXANDRIA";
  tmp_map_marker.LONG_NAME = "Alexandria";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.336785f, -92.457796f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.296117f, -92.423285f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.230499f, -92.506281f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.250400f, -92.531065f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.330122f, -92.500070f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.489848f, -92.440667f);
  tmp_map_marker.DISPLAY_NAME = "EUNICE";
  tmp_map_marker.LONG_NAME = "Eunice";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.509657f, -92.401657f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.496970f, -92.386068f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.480553f, -92.402812f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.480797f, -92.436296f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.506169f, -92.440347f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(29.982557f, -90.168530f);
  tmp_map_marker.DISPLAY_NAME = "NEW ORLEANS";
  tmp_map_marker.LONG_NAME = "New Orleans";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.076122f, -89.943296f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.060643f, -89.938536f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.079633f, -89.918518f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.029114f, -89.868705f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.980538f, -89.945799f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.994771f, -89.974354f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.980730f, -90.016086f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.942213f, -89.909177f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.888317f, -89.876726f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.859412f, -89.907073f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.880833f, -89.965787f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.786873f, -90.019137f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.818193f, -90.096541f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.848682f, -90.131552f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.869876f, -90.115191f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.899694f, -90.183680f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.882054f, -90.223685f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.910337f, -90.212227f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.942214f, -90.270960f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.897669f, -90.377874f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.853799f, -90.412114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.771734f, -90.408804f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.811301f, -90.475680f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.851625f, -90.475728f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.896247f, -90.399902f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.953521f, -90.434220f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.012152f, -90.529841f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.971273f, -90.706125f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.967735f, -90.729782f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.005187f, -90.742850f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.033462f, -90.695519f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.119668f, -90.650582f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.080552f, -90.435780f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.063568f, -90.425206f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.007084f, -90.454750f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.000685f, -90.435989f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.025353f, -90.403260f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.976339f, -90.313589f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.009468f, -90.282421f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.048329f, -90.275722f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.022659f, -90.167189f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.037946f, -90.042940f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.272281f, -89.788092f);
  tmp_map_marker.DISPLAY_NAME = "SLIDEL";
  tmp_map_marker.LONG_NAME = "Slidel";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.332989f, -89.753014f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.277625f, -89.701522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.215956f, -89.801522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.223910f, -89.823857f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.327115f, -89.766132f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.374954f, -90.094526f);
  tmp_map_marker.DISPLAY_NAME = "MANDEVILLE";
  tmp_map_marker.LONG_NAME = "Mandeville";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.423856f, -90.075888f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.373179f, -90.019071f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.366490f, -90.103757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418023f, -90.104268f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(30.443740f, -90.462038f);
  tmp_map_marker.DISPLAY_NAME = "PONCHATOULA";
  tmp_map_marker.LONG_NAME = "Ponchatoula";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.467505f, -90.439530f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.431609f, -90.418708f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.420697f, -90.449317f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.460811f, -90.469741f));
  LANDMARKS.push_back(tmp_map_marker);

  tmp_map_marker.clear();
  tmp_map_marker.LAT_LON = DOUBLE_VEC2(29.973729f, -92.145558f);
  tmp_map_marker.DISPLAY_NAME = "ABBEVILLE";
  tmp_map_marker.LONG_NAME = "Abbeville";
  tmp_map_marker.TYPE = 2;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.983607f, -92.085836f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.964678f, -92.078805f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.964678f, -92.078805f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.969312f, -92.106233f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.963425f, -92.113759f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.961744f, -92.131721f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.957327f, -92.132449f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.951439f, -92.138516f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.951646f, -92.160117f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.956692f, -92.162303f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.965314f, -92.160849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.967629f, -92.152354f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.976252f, -92.149443f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.981087f, -92.155999f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.992654f, -92.152603f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.993286f, -92.144834f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.990553f, -92.122010f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.987188f, -92.119097f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.986346f, -92.110114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.989500f, -92.105258f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.975409f, -92.097735f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.975617f, -92.090938f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.981925f, -92.088265f));
  LANDMARKS.push_back(tmp_map_marker);

  // Interstates

  // I-10
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-10";
  tmp_map_marker.LONG_NAME = "Interstate 10";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.121233f, -93.747711f)); // Orange Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.118660f, -93.731668f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.118658f, -93.724240f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.118658f, -93.724240f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.175864f, -93.587440f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.185467f, -93.565849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.215613f, -93.410769f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.215991f, -93.358877f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.216338f, -93.324832f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.237370f, -93.278771f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.237407f, -93.225437f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235950f, -93.217031f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.236660f, -93.191596f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246475f, -93.180497f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.243948f, -93.134131f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247503f, -93.113007f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247699f, -93.036777f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.251020f, -92.996948f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246471f, -92.970745f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246818f, -92.861474f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246684f, -92.821686f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247497f, -92.753457f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.248122f, -92.747143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247719f, -92.726514f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246667f, -92.719473f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246673f, -92.645429f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.236373f, -92.607313f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.236405f, -92.554399f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.231784f, -92.532307f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.231128f, -92.484475f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.236344f, -92.451684f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.235553f, -92.339542f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.251878f, -92.284636f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.252538f, -92.247032f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247901f, -92.214248f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.247521f, -92.125423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.246852f, -92.052080f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.268464f, -91.992376f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.282270f, -91.947423f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.291112f, -91.928536f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.308086f, -91.869546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.317895f, -91.817319f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.324020f, -91.790897f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.343731f, -91.718318f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.367349f, -91.630403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.403197f, -91.499818f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.410717f, -91.471644f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.413424f, -91.453426f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418706f, -91.433795f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.428949f, -91.401501f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.450937f, -91.317753f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.453631f, -91.270167f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.441023f, -91.217734f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.439199f, -91.178414f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.427473f, -91.169442f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.424321f, -91.153667f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418494f, -91.114572f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.393500f, -91.083705f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.372317f, -91.050757f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.344477f, -91.028084f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.314689f, -90.998398f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.308020f, -90.994626f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.301352f, -90.992741f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.281934f, -90.989632f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.269776f, -90.985861f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.259397f, -90.980202f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.232552f, -90.965092f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.205399f, -90.942625f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.200501f, -90.936965f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.171885f, -90.876563f));  // Sorrento Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.169776f, -90.870422f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.155533f, -90.818317f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.153301f, -90.803879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.149051f, -90.777177f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.144551f, -90.761590f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.127205f, -90.704998f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.124453f, -90.692873f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.123574f, -90.681469f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.111333f, -90.546501f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.107418f, -90.504062f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.105107f, -90.495979f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.088491f, -90.441849f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.077938f, -90.405111f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.056330f, -90.372632f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.015148f, -90.311917f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.010158f, -90.303897f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.006656f, -90.292133f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.006533f, -90.282173f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.008154f, -90.261097f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.009471f, -90.242837f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.007468f, -90.212306f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.005217f, -90.207976f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.001653f, -90.202275f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.000151f, -90.197656f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.999345f, -90.187839f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.997530f, -90.155504f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.995716f, -90.115806f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.992827f, -90.082257f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.993013f, -90.077622f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.990732f, -90.070403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.991373f, -90.058692f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.991856f, -90.049852f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(29.995254f, -90.046437f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.001193f, -90.039516f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.004085f, -90.029383f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.007079f, -90.016939f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.008918f, -90.013660f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.012019f, -90.013052f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.022268f, -90.014030f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.025316f, -90.012755f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.026787f, -90.007717f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.028660f, -89.997476f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.029942f, -89.993546f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.035774f, -89.979205f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.038756f, -89.973005f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.060082f, -89.939298f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.062733f, -89.935115f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.106713f, -89.896114f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.150238f, -89.859165f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.211706f, -89.793770f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.221652f, -89.783806f));
  LANDMARKS.push_back(tmp_map_marker);

  // I - 49
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-49";
  tmp_map_marker.LONG_NAME = "Interstate 49";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.324306f, -92.462141f));	// Pineville North Exit
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.321225f, -92.459222f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.312571f, -92.453902f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.303182f, -92.447039f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.295261f, -92.444295f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.285286f, -92.441553f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.258583f, -92.435530f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.249334f, -92.429525f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.243317f, -92.429528f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.235790f, -92.456694f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.229980f, -92.463739f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.197596f, -92.474676f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.184723f, -92.471040f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.163748f, -92.459401f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.154385f, -92.454271f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.140048f, -92.451123f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.124460f, -92.442150f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.099112f, -92.445563f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.087890f, -92.442900f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.071040f, -92.433390f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.034219f, -92.412049f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.027350f, -92.403074f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(31.014641f, -92.376143f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.987392f, -92.308129f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.984683f, -92.303761f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.896798f, -92.227066f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.838660f, -92.229249f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.807382f, -92.215206f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.780294f, -92.189657f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.767177f, -92.176041f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.728954f, -92.125567f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.725401f, -92.120229f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.714113f, -92.102761f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.694104f, -92.082098f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.686167f, -92.078221f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.665280f, -92.070234f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.640408f, -92.048659f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.610766f, -92.046631f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.584225f, -92.049337f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.545556f, -92.053919f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.537408f, -92.063645f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.471334f, -92.078038f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.434281f, -92.068025f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.422548f, -92.056140f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.416266f, -92.054448f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.395754f, -92.058597f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.336736f, -92.039325f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.272556f, -92.018741f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.258295f, -92.015600f));
  LANDMARKS.push_back(tmp_map_marker);
  
  // I - 12
  tmp_map_marker.clear();
  tmp_map_marker.DISPLAY_NAME = "I-12";
  tmp_map_marker.LONG_NAME = "Interstate 12";
  tmp_map_marker.TYPE = 3;
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418484f, -91.115550f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418410f, -91.109713f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.417448f, -91.099328f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.417151f, -91.093577f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.418113f, -91.088341f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.423809f, -91.075121f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.427434f, -91.066879f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.430243f, -91.056403f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.438243f, -91.024397f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.447637f, -90.986027f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.455112f, -90.956419f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.464700f, -90.917742f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.466899f, -90.886457f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.470194f, -90.862571f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.473320f, -90.838689f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.473583f, -90.793522f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.474024f, -90.785051f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.474462f, -90.729371f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.475916f, -90.710745f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.474635f, -90.666558f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.475020f, -90.615462f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.479254f, -90.563930f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.478995f, -90.489157f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.478890f, -90.414238f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.478959f, -90.357259f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.482305f, -90.312589f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.480147f, -90.245230f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.475742f, -90.228390f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.466162f, -90.205832f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.446896f, -90.140711f));
  tmp_map_marker.REGION_GPS_COORDS.push_back(DOUBLE_VEC2(30.429031f, -90.084776f));	// Chinchuba Exit
  LANDMARKS.push_back(tmp_map_marker);

  ret_suc = true;

  return ret_suc;
}

bool MAP::map_load_json(string Filename)
{
  bool ret_success = false;
  bool tmp_success = false;

  // Load Map
  JSON_INTERFACE map_json;
  string json_map_file = file_to_string(Filename, tmp_success);
  if (tmp_success)
  {
    INFORMATION += "  Map \"" + Filename + "\" loaded successfully.\n";

    // parse file
    ret_success = map_json.load_json_from_string(json_map_file);

    if (ret_success == true)
    {
      for(size_t root = 0; root < map_json.ROOT.DATA.size(); root++)
      {
        if (map_json.ROOT.DATA[root].label() == "generic"   ||
            map_json.ROOT.DATA[root].label() == "airports"  ||
            map_json.ROOT.DATA[root].label() == "region"    ||
            map_json.ROOT.DATA[root].label() == "roads")
        {
          for (size_t marker_list = 0; 
                      marker_list < map_json.ROOT.DATA[root].DATA.size(); marker_list++)
          {
            MAP_INFO tmp_map_marker;

            string display_name = "";
            string long_name = "";
            string type = "";
            STRING_DOUBLE latitude;
            STRING_DOUBLE longitude;

            for (size_t marker_item = 0; 
                        marker_item < map_json.ROOT.DATA[root].DATA[marker_list].DATA.size(); marker_item++)
            {
              // common marker values
              map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].get_if_is("display name", display_name);
              map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].get_if_is("long name", long_name);
              map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].get_if_is("type", type);

              // location
              if (map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].label() == "location")
              {
                for (size_t location_entry = 0; 
                      location_entry < map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA.size(); location_entry++)
                {
                  map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[location_entry].get_if_is("latitude", latitude);
                  map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[location_entry].get_if_is("longitude", longitude);
                }
                tmp_map_marker.LAT_LON.x = latitude.get_double_value();
                tmp_map_marker.LAT_LON.y = longitude.get_double_value();
              }

              // points
              if (map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].label() == "points")
              {
                for (size_t points_entry = 0; 
                      points_entry < map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA.size(); points_entry++)
                {
                  STRING_DOUBLE points_entry_location_latitude;
                  STRING_DOUBLE points_entry_location_longitude;
                  DOUBLE_VEC2 point;
                  for (size_t points_entry_location = 0; 
                        points_entry_location < map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[points_entry].DATA.size(); 
                        points_entry_location++)
                  {
                    map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[points_entry].DATA[points_entry_location].get_if_is("latitude", points_entry_location_latitude);
                    map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[points_entry].DATA[points_entry_location].get_if_is("longitude", points_entry_location_longitude);
                  }
                  point.x = points_entry_location_latitude.get_double_value();
                  point.y = points_entry_location_longitude.get_double_value();
                  tmp_map_marker.REGION_GPS_COORDS.push_back(point);
                }
              }

              // runway vecotors
              if (map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].label() == "runway vectors")
              {
                for (size_t vector_entry = 0; 
                      vector_entry < map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA.size(); vector_entry++)
                {
                  STRING_FLOAT vector;
                  map_json.ROOT.DATA[root].DATA[marker_list].DATA[marker_item].DATA[vector_entry].get_if_is("vector", vector);
                  tmp_map_marker.AIRPORT_LANDING_VECTORS.push_back(vector.get_float_value());
                }
              }
            }
            
            // build entry (vectors and points already made)
            tmp_map_marker.DISPLAY_NAME = display_name;
            tmp_map_marker.LONG_NAME = long_name;

            if (type == "generic")
            {
              tmp_map_marker.TYPE = MAP_POINT_TYPE_GENERIC;
              tmp_map_marker.LAT_LON.x = latitude.get_double_value();
              tmp_map_marker.LAT_LON.y = longitude.get_double_value();

              // add entry to map list
              LANDMARKS.push_back(tmp_map_marker);
            }
            else if (type == "airport")
            {
              tmp_map_marker.TYPE = MAP_POINT_TYPE_AIRPORT;
              tmp_map_marker.LAT_LON.x = latitude.get_double_value();
              tmp_map_marker.LAT_LON.y = longitude.get_double_value();
              // runway vectors already created

              // add entry to map list
              LANDMARKS.push_back(tmp_map_marker);
            }
            else if (type == "region")
            {
              tmp_map_marker.TYPE = MAP_POINT_TYPE_REGION;
              tmp_map_marker.LAT_LON.x = latitude.get_double_value();
              tmp_map_marker.LAT_LON.y = longitude.get_double_value();
              // points already created

              // add entry to map list
              LANDMARKS.push_back(tmp_map_marker);
            }
            else if (type == "road")
            {
              tmp_map_marker.TYPE = MAP_POINT_TYPE_ROAD;
              // roads have no location info
              // points already created

              // add entry to map list
              LANDMARKS.push_back(tmp_map_marker);
            }
          }
        }
      }

      // all items successfully loaded.  return success.
      ret_success = true;
    }
    else
    {
      INFORMATION += "  Failed to parse json \"" + Filename + "\"\n";
    }
  }
  else
  {
    INFORMATION += "  Failed to load \"" + Filename + "\"\n";
  }
  
  return ret_success;
}


bool MAP::map_save()
{
  // Generics
  JSON_INTERFACE generic_map;
  JSON_ENTRY all_generics;
  deque<string> generic_json_deque;

  // Airports  
  JSON_INTERFACE airport_map;
  JSON_ENTRY all_airports;
  deque<string> airport_json_deque;

  // Regions
  JSON_INTERFACE region_map;
  JSON_ENTRY all_regions;
  deque<string> region_json_deque;

  // Roads
  JSON_INTERFACE road_map;
  JSON_ENTRY all_roads;
  deque<string> road_json_deque;

  for (size_t pos = 0; pos < LANDMARKS.size(); pos++)
  {
    // Generics
    if (LANDMARKS[pos].TYPE == MAP_POINT_TYPE_GENERIC)
    {
      JSON_ENTRY generic;

      generic.create_label_value(quotify("display name"), quotify(LANDMARKS[pos].DISPLAY_NAME));
      generic.create_label_value(quotify("long name"), quotify(LANDMARKS[pos].LONG_NAME));
      generic.create_label_value(quotify("type"), quotify("generic"));

      JSON_ENTRY latitude_longitude;
      latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.x)));
      latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.y)));
      generic.put_json_in_set(quotify("location"), latitude_longitude);

      all_generics.put_json_in_list(generic);
    }

    // Airports
    if (LANDMARKS[pos].TYPE == MAP_POINT_TYPE_AIRPORT)
    {
      JSON_ENTRY airport;

      airport.create_label_value(quotify("display name"), quotify(LANDMARKS[pos].DISPLAY_NAME));
      airport.create_label_value(quotify("long name"), quotify(LANDMARKS[pos].LONG_NAME));
      airport.create_label_value(quotify("type"), quotify("airport"));

      JSON_ENTRY latitude_longitude;
      latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.x)));
      latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.y)));
      airport.put_json_in_set(quotify("location"), latitude_longitude);

      JSON_ENTRY runway_vecors;
      for (size_t vectors = 0; vectors < LANDMARKS[pos].AIRPORT_LANDING_VECTORS.size(); vectors++)
      {
        runway_vecors.create_label_value(quotify("vector"), to_string(LANDMARKS[pos].AIRPORT_LANDING_VECTORS[vectors]));
      }
      airport.put_json_in_set(quotify("runway vectors"),  runway_vecors);

      all_airports.put_json_in_list(airport);
    }

    // Regions
    if (LANDMARKS[pos].TYPE == MAP_POINT_TYPE_REGION)
    {
      JSON_ENTRY region;

      region.create_label_value(quotify("display name"), quotify(LANDMARKS[pos].DISPLAY_NAME));
      region.create_label_value(quotify("long name"), quotify(LANDMARKS[pos].LONG_NAME));
      region.create_label_value(quotify("type"), quotify("region"));

      JSON_ENTRY latitude_longitude;
      latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.x)));
      latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(LANDMARKS[pos].LAT_LON.y)));
      region.put_json_in_set(quotify("location"), latitude_longitude);


      JSON_ENTRY points;
      for (size_t coord = 0; coord < LANDMARKS[pos].REGION_GPS_COORDS.size(); coord++)
      {
        JSON_ENTRY latitude_longitude;
        latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(LANDMARKS[pos].REGION_GPS_COORDS[coord].x)));
        latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(LANDMARKS[pos].REGION_GPS_COORDS[coord].y)));
        points.put_json_in_list(latitude_longitude);
      }
      region.put_json_in_set(quotify("points"), points);
      all_regions.put_json_in_list(region);
    }

    // Roads
    if (LANDMARKS[pos].TYPE == MAP_POINT_TYPE_ROAD)
    {
      JSON_ENTRY road;

      road.create_label_value(quotify("display name"), quotify(LANDMARKS[pos].DISPLAY_NAME));
      road.create_label_value(quotify("long name"), quotify(LANDMARKS[pos].LONG_NAME));
      road.create_label_value(quotify("type"), quotify("road"));

      JSON_ENTRY points;
      for (size_t coord = 0; coord < LANDMARKS[pos].REGION_GPS_COORDS.size(); coord++)
      {
        JSON_ENTRY latitude_longitude;
        latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(LANDMARKS[pos].REGION_GPS_COORDS[coord].x)));
        latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(LANDMARKS[pos].REGION_GPS_COORDS[coord].y)));
        points.put_json_in_list(latitude_longitude);
      }
      road.put_json_in_set(quotify("points"), points);
      
      all_roads.put_json_in_list(road);
    }
  }

  // Put Maps together and Save

  FALSE_CATCH json_saved;

  // Generics
  generic_map.ROOT.put_json_in_set(quotify("generic"), all_generics);
  generic_map.json_print_build_to_string_deque(generic_json_deque);
  if(deque_string_to_file(PROPS.FILENAME_GENERICS_MAP, generic_json_deque, false))
  {
    INFORMATION += "  Successfully saved \"" + PROPS.FILENAME_GENERICS_MAP + "\"\n";
    json_saved.catch_false(true);
  }
  else
  {
    INFORMATION += "  Failed to save \"" + PROPS.FILENAME_GENERICS_MAP + "\"\n";
    json_saved.catch_false(false);
  }

  // Airports
  airport_map.ROOT.put_json_in_set(quotify("airports"), all_airports);
  airport_map.json_print_build_to_string_deque(airport_json_deque);
  if(deque_string_to_file(PROPS.FILENAME_AIRPORTS_MAP, airport_json_deque, false))
  {
    INFORMATION += "  Successfully saved \"" + PROPS.FILENAME_AIRPORTS_MAP + "\"\n";
    json_saved.catch_false(true);
  }
  else
  {
    INFORMATION += "  Failed to save \"" + PROPS.FILENAME_AIRPORTS_MAP + "\"\n";
    json_saved.catch_false(false);
  }

  // Regions
  region_map.ROOT.put_json_in_set(quotify("region"), all_regions);
  region_map.json_print_build_to_string_deque(region_json_deque);
  if(deque_string_to_file(PROPS.FILENAME_REGIONS_MAP, region_json_deque, false))
  {
    INFORMATION += "  Successfully saved \"" + PROPS.FILENAME_REGIONS_MAP + "\"\n";
    json_saved.catch_false(true);
  }
  else
  {
    INFORMATION += "  Failed to save \"" + PROPS.FILENAME_REGIONS_MAP + "\"\n";
    json_saved.catch_false(false);
  }

  // Roads
  road_map.ROOT.put_json_in_set(quotify("roads"), all_roads);
  road_map.json_print_build_to_string_deque(road_json_deque);
  if(deque_string_to_file(PROPS.FILENAME_ROADS_MAP, road_json_deque, false))
  {
    INFORMATION += "  Successfully saved \"" + PROPS.FILENAME_ROADS_MAP + "\"\n";
    json_saved.catch_false(true);
  }
  else
  {
    INFORMATION += "  Failed to save \"" + PROPS.FILENAME_ROADS_MAP + "\"\n";
    json_saved.catch_false(false);
  }

  return !(json_saved.has_false());
}

bool MAP::map_load()
{
  FALSE_CATCH ret_failure;

  ret_failure.catch_false(map_load_json(PROPS.FILENAME_GENERICS_MAP));
  ret_failure.catch_false(map_load_json(PROPS.FILENAME_AIRPORTS_MAP));
  ret_failure.catch_false(map_load_json(PROPS.FILENAME_REGIONS_MAP));
  ret_failure.catch_false(map_load_json(PROPS.FILENAME_ROADS_MAP));

  // If map was never loaded, atttempt to load via hardcode.
  if (ret_failure.has_false())
  {
    INFORMATION += "  Maps failed to load.\n";
    if (map_hardcoded_load())
    {
      INFORMATION += "  Map created from hardcode.\n";

      if (map_save())
      {
        INFORMATION += "  Map file created from hardcode and saved for next load.\n";
      }
      else
      {
        ret_failure.catch_false(false);
        INFORMATION += "  Failed to create a map file from hardcoded information.\n";
      }
    }
    else
    {
      ret_failure.catch_false(false);
      INFORMATION += "  Failed creating map from hardcode.\n";
    }
  }

  return !(ret_failure.has_false());
}

bool MAP::track_save(DETAILED_TRACK &Track, string Filename)
{
  // Track
  JSON_INTERFACE track_map;
  JSON_ENTRY all_track;
  deque<string> track_json_deque;

  JSON_ENTRY points;

  // Simple
  for (size_t pos = 0; pos < Track.TRACK_POINTS_SIMPLE.size(); pos++)
  {
    JSON_ENTRY position_info;
    position_info.create_label_value(quotify("unix_epoch_nmea_time"), quotify(to_string(Track.TRACK_POINTS_SIMPLE[pos].TIMESTAMP)));

    JSON_ENTRY latitude_longitude;
    latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(Track.TRACK_POINTS_SIMPLE[pos].LATITUDE)));
    latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(Track.TRACK_POINTS_SIMPLE[pos].LONGITUDE)));

    position_info.put_json_in_set(quotify("location"), latitude_longitude);
    points.put_json_in_list(position_info);
  }

  // Detailed
  for (size_t pos = 0; pos < Track.TRACK_POINTS_DETAILED.size(); pos++)
  {
    JSON_ENTRY position_info;
    position_info.create_label_value(quotify("unix_epoch_nmea_time"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].TIMESTAMP)));

    JSON_ENTRY latitude_longitude;
    latitude_longitude.create_label_value(quotify("latitude"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].LATITUDE)));
    latitude_longitude.create_label_value(quotify("longitude"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].LONGITUDE)));

    position_info.put_json_in_set(quotify("location"), latitude_longitude);
    points.put_json_in_list(position_info);
  }

  all_track.put_json_in_set(quotify("points"), points);

  // Save Track
  FALSE_CATCH json_saved;

  track_map.ROOT.put_json_in_set(quotify("track"), all_track);
  track_map.json_print_build_to_string_deque(track_json_deque);

  if(deque_string_to_file(Filename, track_json_deque, false))
  {
    json_saved.catch_false(true);
  }
  else
  {
    json_saved.catch_false(false);
  }

  return !(json_saved.has_false());
}

bool MAP::track_load(DETAILED_TRACK &Track, string Filename)
{
  bool ret_success = false;
  bool tmp_success = false;
  
  // Load Map
  JSON_INTERFACE track_json;
  string json_track_file = file_to_string(Filename, tmp_success);
  if (tmp_success)
  {
    // parse file
    ret_success = track_json.load_json_from_string(json_track_file);

    if (ret_success == true)
    {
      for(size_t root = 0; root < track_json.ROOT.DATA.size(); root++)        //root
      {
        if (track_json.ROOT.DATA[root].label() == "track")
        {
          for (size_t marker_list = 0;                                        //root/marker_list
                      marker_list < track_json.ROOT.DATA[root].DATA.size(); marker_list++)
          {
            if (track_json.ROOT.DATA[root].DATA[marker_list].label() == "points")
            {
              for (size_t points_entry = 0;                                         //root/marker_list/points_entry
                    points_entry < track_json.ROOT.DATA[root].DATA[marker_list].DATA.size(); points_entry++)
              {
                SIMPLE_TRACK_POINT location;
                STRING_DOUBLE timestamp;
                STRING_DOUBLE location_latitude;
                STRING_DOUBLE location_longitude;

                for (size_t entry = 0;                                         //root/marker_list/points_entry/entry
                      entry < track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA.size(); entry++)
                {
                  track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].get_if_is("unix_epoch_nmea_time", timestamp);

                  if (track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].label() == "location")
                  {
                    for (size_t location = 0; 
                          location < track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].DATA.size(); 
                          location++)
                    {
                      track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].DATA[location].get_if_is("latitude", location_latitude);
                      track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].DATA[location].get_if_is("longitude", location_longitude);
                    }
                  }
                }

                location.TIMESTAMP = timestamp.get_double_value();
                location.LATITUDE = location_latitude.get_double_value();
                location.LONGITUDE = location_longitude.get_double_value();
                Track.TRACK_POINTS_SIMPLE.push_back(location);
              }
            }
          }
        }
      }
    }
  }

  return ret_success;
}

bool MAP::create()
{
  bool ret_suc = false;
  INFORMATION = "";

  // Load world map
  if(map_load())
  {
    INFORMATION += "  Map loaded successfully.";
    ret_suc = true;
  }
  else
  {
    INFORMATION += "  Map load and save not fully implemented.\n";
  }

  return ret_suc;
}

bool MAP::save_track(DETAILED_TRACK &Track, string Filename)
{
  return track_save(Track, Filename);
}

bool MAP::load_track(DETAILED_TRACK &Track, string Filename)
{
  return track_load(Track, Filename);
}

// -------------------------------------------------------------------------------------

#endif