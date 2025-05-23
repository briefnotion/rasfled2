// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: aircraft_coordinator.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AIRCRAFT_COORDINATOR_CPP
#define AIRCRAFT_COORDINATOR_CPP

#include "aircraft_coordinator.h"


// -------------------------------------------------------------------------------------

/*
aircraft.json
This file contains dump1090's list of recently seen aircraft. The keys are:

now:        the time this file was generated, in seconds since Jan 1 1970 00:00:00 GMT (the Unix epoch).
messages:   the total number of Mode S messages processed since dump1090 started.
aircraft:   an array of JSON objects, one per known aircraft. Each aircraft has the following keys. Keys will be omitted if data is not available.
hex:        the 24-bit ICAO identifier of the aircraft, as 6 hex digits. The identifier may start with '~', this means that the address is a non-ICAO address (e.g. from TIS-B).
squawk:     the 4-digit squawk (octal representation)
flight:     the flight name / callsign
lat, lon:   the aircraft position in decimal degrees
nucp:       the NUCp (navigational uncertainty category) reported for the position
seen_pos:   how long ago (in seconds before "now") the position was last updated
altitude:   the aircraft altitude in feet, or "ground" if it is reporting it is on the ground
vert_rate:  vertical rate in feet/minute
track:      true track over ground in degrees (0-359)
speed:      reported speed in kt. This is usually speed over ground, but might be IAS - you can't tell the difference here, sorry!
messages:   total number of Mode S messages received from this aircraft
seen:       how long ago (in seconds before "now") a message was last received from this aircraft
rssi:       recent average RSSI (signal power), in dbFS; this will always be negative.
*/


//using namespace std;
string AIRCRAFT::simple_float_to_string(int Decimal_Positions, float Number)
{
  int c1 = 0;

  string return_string = to_string(Number);

  c1 = return_string.find(".") + 1;

  if(c1 != (int)std::string::npos)
  {
    return_string.erase(c1 + Decimal_Positions, return_string.size() - c1 - Decimal_Positions);
  }

  return return_string;
}

int AIRCRAFT::data_count()
{
  return DATA_COUNT;
}

void AIRCRAFT::count_data()
{
  DATA_COUNT =  SQUAWK.conversion_success() +
                FLIGHT.conversion_success() +
                //VERSION.conversion_success() +
                SPEED.conversion_success() +
                SEEN_POS.conversion_success() +
                ALTITUDE.conversion_success() +
                ALTITUDE_GEOM.conversion_success() +
                VERT_RATE.conversion_success() +
                GEOM_RATE.conversion_success() +
                TRACK.conversion_success() +
                POSITION.LATITUDE.conversion_success() +
                POSITION.LONGITUDE.conversion_success() +
                SIL.conversion_success() +
                //(SIL_TYPE.length() > 0) +
                EMERGENCY.conversion_success() +
                //(CATAGORY.length() > 0) +
                //NAV_QNH.conversion_success() +
                NAV_HEADING.conversion_success() +
                NAV_ALTITUDE_MCP.conversion_success() +
                //NAV_MODES.conversion_success() +
                NIC.conversion_success() +
                RC.conversion_success() +
                NIC_BARO.conversion_success() +
                NAC_P.conversion_success() +
                NAC_V.conversion_success() +
                GVA.conversion_success() +
                SDA.conversion_success() 
                //MLAT.conversion_success() +
                //TISB.conversion_success()
                ;
}

void AIRCRAFT::post_process()
{
  // Convert Data to Display Standard.
  // Vertical Rate
  if (VERT_RATE.conversion_success() == true)
  {
    D_VERTICAL_RATE = VERT_RATE;
  }
  else
  {
    D_VERTICAL_RATE = GEOM_RATE;
  }

  // Calculate Flight Angle
  if (D_VERTICAL_RATE.conversion_success() == true && SPEED.conversion_success() == true )
  {
    float scent_rate  = (float)D_VERTICAL_RATE.get_int_value();
    float speed       = ((float)SPEED.get_float_value()) * (5280/ 60);

    D_FLIGHT_ANGLE.store (to_string(abs((atan( scent_rate / speed) * (180 / float_PI)))));
  }

  if (POSITION.LATITUDE.conversion_success() == true && 
      POSITION.LONGITUDE.conversion_success() == true)
  {
    POSITION.GLOBAL_POSITION_FOUND = true;
  }
  else
  {
    POSITION.GLOBAL_POSITION_FOUND = false;
  }

  // Fill Data Size Field
  count_data();
}

bool AIRCRAFT::alert()
{
  return ALERT;
}

// -------------------------------------------------------------------------------------

void AIRCRAFT_MAP_DETAILS::clear()
// Clear values
{
  // Clear Variables
  WIDGET_ACTIVE = false;

  AIRCRAFT blank_aircraft_data;
  AIRCRAFT_ITEM = blank_aircraft_data;

  TRACK.clear();
}

bool AIRCRAFT_MAP_DETAILS::is_expired(unsigned long tmeCurrentMillis)
{
  if (EXPIREED.ping_down(tmeCurrentMillis) == true)
  {
    return false;
  }
  else
  {
    clear();
    return true;
  }
}

void AIRCRAFT_MAP_DETAILS::update_aircraft(AIRCRAFT Aircraft_o, unsigned long tmeCurrentMillis)
// Update values of gadget
//  Gadget will be redrawn if values did changed or animations scheduled. 
{
  // Start or Continue expiration timer
  EXPIREED.ping_up(tmeCurrentMillis, EXPIRATION_TIME);
  WIDGET_ACTIVE = true;

  AIRCRAFT_ITEM = Aircraft_o;

  if (AIRCRAFT_ITEM.POSITION.GLOBAL_POSITION_FOUND)
  {
    // Add only if new value not the same as prev, or no other values exist.
    if (TRACK.TRACK_POINTS_DETAILED.size() == 0)
    {
      {
        DETAILED_TRACK_POINT new_lat_lon;
        new_lat_lon.LATITUDE = AIRCRAFT_ITEM.POSITION.LATITUDE.get_float_value();
        new_lat_lon.LONGITUDE = AIRCRAFT_ITEM.POSITION.LONGITUDE.get_float_value();
        new_lat_lon.ALTITUDE = (float)AIRCRAFT_ITEM.ALTITUDE.get_int_value();
        new_lat_lon.TIME = (float)tmeCurrentMillis;

        float intensity = (32.0f + AIRCRAFT_ITEM.RSSI.get_float_value()) / 32.0f;

        if (intensity < 0.2f)
        {
          intensity = 0.2f;
        }
        
        new_lat_lon.RSSI_INTENSITY = intensity;

        TRACK.store(new_lat_lon);
      }
    }
    else if (AIRCRAFT_ITEM.POSITION.LATITUDE.get_float_value() != TRACK.TRACK_POINTS_DETAILED[TRACK.TRACK_POINTS_DETAILED.size() -1].LATITUDE || 
              AIRCRAFT_ITEM.POSITION.LONGITUDE.get_float_value() != TRACK.TRACK_POINTS_DETAILED[TRACK.TRACK_POINTS_DETAILED.size() -1].LONGITUDE)
    {
      DETAILED_TRACK_POINT new_lat_lon;
      new_lat_lon.LATITUDE = AIRCRAFT_ITEM.POSITION.LATITUDE.get_float_value();
      new_lat_lon.LONGITUDE = AIRCRAFT_ITEM.POSITION.LONGITUDE.get_float_value();
      new_lat_lon.ALTITUDE = (float)AIRCRAFT_ITEM.ALTITUDE.get_int_value();
      new_lat_lon.TIME = (float)tmeCurrentMillis;

      float intensity = (32.0f + AIRCRAFT_ITEM.RSSI.get_float_value()) / 32.0f;

      if (intensity < 0.2f)
      {
        intensity = 0.2f;
      }
      
      new_lat_lon.RSSI_INTENSITY = intensity;

      TRACK.store(new_lat_lon);
    }
  }

  //PROP.CHANGED = true;
}

bool AIRCRAFT_MAP_DETAILS::active()
{
  return WIDGET_ACTIVE;
}


int AIRCRAFT_MAP_INFO::find_HEX(string Hex)
// Gadget Internal:
//  returns gadget position of aircraft with Hex ID
{
  int return_int = -1;
  for(int x=0; (x < (int)AIRCRAFT_DETAIL_LIST.size()) && (return_int == -1); x++)
  {
    if(AIRCRAFT_DETAIL_LIST[x].AIRCRAFT_ITEM.HEX.get_str_value() == Hex)
    {
      return_int = x;
    }
  }
  return return_int;
}

int AIRCRAFT_MAP_INFO::find_expired()
// Gadget Internal:
//  returns gadget position of aircraft with time expired.
{
  int return_int = -1;
  for(int x=0; (x < (int)AIRCRAFT_DETAIL_LIST.size()) && (return_int == -1); x++)
  {
    if(AIRCRAFT_DETAIL_LIST[x].active() == false || AIRCRAFT_DETAIL_LIST[x].AIRCRAFT_ITEM.data_count() == 0)
    {
      return_int = x;
    }
  }
  return return_int;
}

void AIRCRAFT_MAP_INFO::create_reserve_alerts(ALERT_SYSTEM_2 &ALERTS_ADSB, int Item_Position)
{
  // Initialize ADSB Alert_2 System - Generate Reserve Alerts
  ALERTS_ADSB.clear_all();

  ALERTS_ADSB.PROPS.ALERT_SYSTEM_NAME = "ADS-B (" + to_string(Item_Position) + ")";
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_PROXIMITY, "PROXIMITY");
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_FLIGHT_ANGLE, "FLT ANGLE");
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_ADSB_EMERGENCY, "EMERGENCY FLAG");
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_SQUAWK_HIJACKING, "SQK HIJACKING");
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_SQUAWK_RADIO, "SQK RADIO");
  ALERTS_ADSB.generate_reserve_alert(ADSB_RESERVE_ALERT_SQUAWK_EMERGENCY, "SQK EMERGENCY");
}

void AIRCRAFT_MAP_INFO::update(unsigned long tmeCurrentTime, AIRCRAFT_DATA &DATA)
{
  int pos_found = 0;
  int pos_avail = 0;
  int pos_expired_avail = 0;

  // Prepare list to display.
  for (int pos_search = 0; pos_search < (int)DATA.AIRCRAFTS.size(); pos_search++)
  {
    //Search gadget list for existing item to update.
    pos_found = find_HEX(DATA.AIRCRAFTS[pos_search].HEX.get_str_value());

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
          AIRCRAFT_MAP_DETAILS new_aircraft_map_info;

          // Initialize ADSB Alert_2 System - Generate Reserve Alerts
          create_reserve_alerts(new_aircraft_map_info.ALERTS_ADSB, AIRCRAFT_DETAIL_LIST.size() +1);

          new_aircraft_map_info.update_aircraft(DATA.AIRCRAFTS[pos_search], tmeCurrentTime);
          AIRCRAFT_DETAIL_LIST.push_back(new_aircraft_map_info);
        }
        else
        {
          // Initialize ADSB Alert_2 System - Generate Reserve Alerts
          create_reserve_alerts(AIRCRAFT_DETAIL_LIST[pos_expired_avail].ALERTS_ADSB, pos_expired_avail +1);

          AIRCRAFT_DETAIL_LIST[pos_expired_avail].clear();
          AIRCRAFT_DETAIL_LIST[pos_expired_avail].update_aircraft(DATA.AIRCRAFTS[pos_search], tmeCurrentTime);
        }
      }
      else // slot found and pos avail.
      {
        // Initialize ADSB Alert_2 System - Generate Reserve Alerts
        create_reserve_alerts(AIRCRAFT_DETAIL_LIST[pos_avail].ALERTS_ADSB, pos_avail +1);

        AIRCRAFT_DETAIL_LIST[pos_avail].update_aircraft(DATA.AIRCRAFTS[pos_search], tmeCurrentTime);
      }
    }
    else // put in found pos.
    {
      AIRCRAFT_DETAIL_LIST[pos_found].update_aircraft(DATA.AIRCRAFTS[pos_search], tmeCurrentTime);
    }
  }

  //---

  TIME_OF_SIGNAL =  (
                        to_string(DATA.CONVERTED_TIME.get_year()) + 
                  "-" + linemerge_right_justify(2, "00", to_string(DATA.CONVERTED_TIME.get_month())) + 
                  "-" + linemerge_right_justify(2, "00", to_string(DATA.CONVERTED_TIME.get_day())) + 
                  " " + linemerge_right_justify(2, "00", to_string(DATA.CONVERTED_TIME.get_hour())) + 
                  ":" + linemerge_right_justify(2, "00", to_string(DATA.CONVERTED_TIME.get_minute())) + 
                  ":" + linemerge_right_justify(2, "00", to_string(DATA.CONVERTED_TIME.get_second())) + 
                  "." + to_string(DATA.CONVERTED_TIME.get_deciseconds())
                          );

  POSITIONED_COUNT = to_string(DATA.AIRCRAFTS.size());
  POSITIONED_AIRCRAFT = to_string(DATA.POSITIONED_AIRCRAFT);

  //---

  DATA.CHANGED = false;
  CHANGED = true;
}

void AIRCRAFT_MAP_INFO::clear_panel_flags()
{
  PANEL_FLAG_CHANGED = false;
  PANEL_FLAG_PROXIMITY = false;
  PANEL_FLAG_LONG_DISTANCE = false;
  PANEL_FLAG_EMERGENCY = false;
}

// -------------------------------------------------------------------------------------

void AIRCRAFT_COORDINATOR::check_alerts(AIRCRAFT_MAP_DETAILS &Aircraft_Deets)
{
  // Set default
  Aircraft_Deets.ALERT_LEVEL = 0; // No Alert
  Aircraft_Deets.COLOR = 1;       // White

  // Alert Level 0 -----
  
  // Alert Level 1 -----

  // Check Proximity

  if (Aircraft_Deets.AIRCRAFT_ITEM.DISTANCE_FROM_BASE >= 0.0f)
  {
    /*
    if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition(ADSB_RESERVE_ALERT_PROXIMITY, 
        (Aircraft_Deets.AIRCRAFT_ITEM.DISTANCE_FROM_BASE < 1.0f) && (Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_int_value() < 5000),
        (Aircraft_Deets.AIRCRAFT_ITEM.DISTANCE_FROM_BASE > 1.0f) || (Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_int_value() > 5000)))
    {
      Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_PROXIMITY, 
                                                              "SQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                              "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                              "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                              "  ANG: " + to_string_round_to_nth(Aircraft_Deets.AIRCRAFT_ITEM.ANGLE_FROM_BASE, 1) +
                                                              "\nSPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());
      Aircraft_Deets.ALERTS_ADSB.res_update_additional_line_with_conditions(ADSB_RESERVE_ALERT_PROXIMITY);
      Aircraft_Deets.ALERTS_ADSB.ALERTS_RESERVE[ADSB_RESERVE_ALERT_PROXIMITY].set_show_value_bar(true);
    }
    */

    //if (Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_int_value() < 5000)
    {
      if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition_less_than(ADSB_RESERVE_ALERT_PROXIMITY, 
          Aircraft_Deets.AIRCRAFT_ITEM.DISTANCE_FROM_BASE, 1.0f, 2.0f))
      {
        Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_PROXIMITY, 
                                                                "SQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                                "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                                "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                                "  ANG: " + to_string_round_to_nth(Aircraft_Deets.AIRCRAFT_ITEM.ANGLE_FROM_BASE, 1) +
                                                                "\nSPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());
        Aircraft_Deets.ALERTS_ADSB.res_update_additional_line_with_conditions(ADSB_RESERVE_ALERT_PROXIMITY);
        Aircraft_Deets.ALERTS_ADSB.ALERTS_RESERVE[ADSB_RESERVE_ALERT_PROXIMITY].set_show_value_bar(true);
      }

      if ((Aircraft_Deets.AIRCRAFT_ITEM.DISTANCE_FROM_BASE < 1.0f) && (Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_int_value() < 5000))
      {
        AIRCRAFTS_MAP.PANEL_FLAG_PROXIMITY = true;
      }
    }
  }

  if(Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.conversion_success() == true)
  {
    // VFR RULES
    if (Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() == 1200)
    {
      Aircraft_Deets.ALERT_LEVEL = 1;
      Aircraft_Deets.COLOR = 6;
    }
  }

  // Alert Level 2 -----
  if (Aircraft_Deets.AIRCRAFT_ITEM.D_FLIGHT_ANGLE.get_float_value() > 6)
  {
    //tmp_alert_entry.ALERT_LEVEL = 2;
    //tmp_alert_entry.ALERT = "  \\___ FLIGHT ANGLE: " + simple_float_to_string(1, D_FLIGHT_ANGLE.get_float_value()) + " deg";
    //ALERT_LIST.push_back(tmp_alert_entry);
    Aircraft_Deets.ALERT_LEVEL = 2;
    Aircraft_Deets.COLOR = 10;
  }

  // Alert Level 3 -----

  // Check Emergency Stat from ADS-B info

  if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition(ADSB_RESERVE_ALERT_ADSB_EMERGENCY, 
        Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() != "" && Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() != "none",
        Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() == "" || Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() == "none"))
  {
    Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_ADSB_EMERGENCY, 
                                                              "ADS-B EMERGENCY:" + Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() + 
                                                              "\nSQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                              "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                              "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                              "  SPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());

    Aircraft_Deets.ALERT_LEVEL = 3;
    Aircraft_Deets.COLOR = 3;
    AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY = true;
  }

  // Check Squak Codes
  if(Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.conversion_success() == true)
  {
    // Hi-Jack
    if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition(ADSB_RESERVE_ALERT_SQUAWK_HIJACKING, 
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() == 7500,
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() != 7500))
    {
      Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_SQUAWK_HIJACKING, 
                                                              "SQUAWK HIJACKING:" + Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() + 
                                                              "\nSQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                              "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                              "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                              "  SPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());
      Aircraft_Deets.ALERT_LEVEL = 3;
      Aircraft_Deets.COLOR = 3;
      AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY = true;
    }

    // Radio Failure
    if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition(ADSB_RESERVE_ALERT_SQUAWK_RADIO, 
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() == 7600,
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() != 7600))
    {
      Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_SQUAWK_RADIO,
                                                              "SQUAWK RADIO FAILURE:" + Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() + 
                                                              "\nSQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                              "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                              "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                              "  SPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());
      Aircraft_Deets.ALERT_LEVEL = 3;
      Aircraft_Deets.COLOR = 3;
      AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY = true;
    }

    // Emergency
    if (Aircraft_Deets.ALERTS_ADSB.res_alert_condition(ADSB_RESERVE_ALERT_SQUAWK_EMERGENCY, 
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() == 7700,
          Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_int_value() != 7700))
    {
      Aircraft_Deets.ALERTS_ADSB.res_update_alert_text_line(ADSB_RESERVE_ALERT_SQUAWK_EMERGENCY,
                                                              "SQUAWK EMERGENCY:" + Aircraft_Deets.AIRCRAFT_ITEM.EMERGENCY.get_str_value() + 
                                                              "\nSQK: " + Aircraft_Deets.AIRCRAFT_ITEM.SQUAWK.get_str_value() + 
                                                              "  FLT: " + Aircraft_Deets.AIRCRAFT_ITEM.FLIGHT.get_str_value() +
                                                              "\nALT: " + Aircraft_Deets.AIRCRAFT_ITEM.ALTITUDE.get_str_value() +
                                                              "  SPD: " + Aircraft_Deets.AIRCRAFT_ITEM.SPEED.get_str_value());
      Aircraft_Deets.ALERT_LEVEL = 3;
      Aircraft_Deets.COLOR = 3;
      AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY = true;
    }
  }
}

void AIRCRAFT_COORDINATOR::post_post_process()
// Calculate full aircraft data.
{
  // Convert time
  if (DATA.NOW.conversion_success() == true)
  {
    int dec_pos = DATA.NOW.get_str_value().find('.');
    if (dec_pos != (int)string::npos)
    {
      DATA.CONVERTED_TIME.put_seconds(string_to_ulong(DATA.NOW.get_str_value().erase(dec_pos)));
      DATA.CONVERTED_TIME.put_deciseconds(string_to_int(DATA.NOW.get_str_value().erase(0, dec_pos +1)));
    }
  }
  
  // Step through each aircraft
  for (int aircraft = 0; aircraft < (int)DATA.AIRCRAFTS.size(); aircraft++)
  {
    // Calculate Distance from base for each Aircraft
    if (DATA.CURRENT_POS_AVAIL && DATA.AIRCRAFTS[aircraft].POSITION.GLOBAL_POSITION_FOUND && 
        DATA.CURRENT_POS_AVAIL)
    {
      DATA.AIRCRAFTS[aircraft].DISTANCE_FROM_BASE = calculate_distance(
                                  DATA.AIRCRAFTS[aircraft].POSITION.LATITUDE.get_float_value(), 
                                  DATA.AIRCRAFTS[aircraft].POSITION.LONGITUDE.get_float_value(), 
                                  DATA.CURRENT_LAT, 
                                  DATA.CURRENT_LON);

      DATA.AIRCRAFTS[aircraft].ANGLE_FROM_BASE = calculate_angle(DATA.AIRCRAFTS[aircraft].DISTANCE_FROM_BASE * 5280.0f, 
                                                            (float)DATA.AIRCRAFTS[aircraft].ALTITUDE.get_int_value());                                 
    }
    else
    {
      DATA.AIRCRAFTS[aircraft].DISTANCE_FROM_BASE = -1;
      DATA.AIRCRAFTS[aircraft].ANGLE_FROM_BASE = -1;
    }
    
    // Meta
    DATA.AIRCRAFTS[aircraft].META.COMPASS_INFO_DISP_FLIGHT = " " + trim(DATA.AIRCRAFTS[aircraft].FLIGHT.get_str_value());
    DATA.AIRCRAFTS[aircraft].META.COMPASS_INFO_DISP_SQUAWK = trim(DATA.AIRCRAFTS[aircraft].SQUAWK.get_str_value());
    DATA.AIRCRAFTS[aircraft].META.COMPASS_INFO_DISP_ALTITUDE = "  " + to_string_round_to_nth(float(DATA.AIRCRAFTS[aircraft].ALTITUDE.get_int_value() / 1000.0f), 1) + " kft";
    //DATA.AIRCRAFTS[aircraft].META.POSITION_ANGLE = to_string(DATA.AIRCRAFTS[aircraft].ANGLE_FROM_BASE);
    DATA.AIRCRAFTS[aircraft].META.COMPASS_INFO_DISP_SPEED = trim(DATA.AIRCRAFTS[aircraft].SPEED.get_str_value());
    DATA.AIRCRAFTS[aircraft].META.COMPASS_INFO_DISP_DISTANCE_FROM_BASE = to_string_round_to_nth(DATA.AIRCRAFTS[aircraft].DISTANCE_FROM_BASE, 1) + " mi";

    DATA.AIRCRAFTS[aircraft].META.POSITION_SQUAWK.x = 0.0f;
    DATA.AIRCRAFTS[aircraft].META.POSITION_SQUAWK.y = -49.0f;

    DATA.AIRCRAFTS[aircraft].META.POSITION_FLIGHT.x = -49.0f;
    DATA.AIRCRAFTS[aircraft].META.POSITION_FLIGHT.y = 0.0f;

    DATA.AIRCRAFTS[aircraft].META.POSITION_SPEED.x = -43.0f;
    DATA.AIRCRAFTS[aircraft].META.POSITION_SPEED.y = -3.0f;

    DATA.AIRCRAFTS[aircraft].META.POSITION_ALTITUDE.x = 0.0f;
    DATA.AIRCRAFTS[aircraft].META.POSITION_ALTITUDE.y = 40.0f;

    DATA.AIRCRAFTS[aircraft].META.POSITION_DISTANCE.x = 25.0f;
    DATA.AIRCRAFTS[aircraft].META.POSITION_DISTANCE.y = 40.0f;
  }
}

// -------------------------------------------------------------------------------------

void AIRCRAFT_COORDINATOR::post_post_post_process(COMMAND_THREAD &Thread, SOUNDS &Sound_System)
{
  ADSB_ALERT_COUNT = 0;

  if (AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size() > 0)
  {
    bool set_first = true;

    for (int aircraft = 0; aircraft < (int)AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); aircraft++)
    {
      check_alerts(AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft]);
      AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].ALERTS_ADSB.alert_list_clean(Thread, Sound_System);
      ADSB_ALERT_COUNT += AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].ALERTS_ADSB.alert_count();

      if (AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE != -1)
      {
        if (set_first)
        {
          AIRCRAFTS_MAP.DISTANCE_CLOSEST = AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE;
          AIRCRAFTS_MAP.DISTANCE_FURTHEST = AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE;

          set_first = false;
        }
        else
        {
          if (AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE < AIRCRAFTS_MAP.DISTANCE_CLOSEST)
          {
            AIRCRAFTS_MAP.DISTANCE_CLOSEST = AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE;
          }

          if (AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE > AIRCRAFTS_MAP.DISTANCE_FURTHEST)
          {
            AIRCRAFTS_MAP.DISTANCE_FURTHEST = AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE;
          }
        }

        if (AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE > AIRCRAFTS_MAP.FURTHEST_AIRCRAFT_SINCE_START)
        {
          AIRCRAFTS_MAP.FURTHEST_AIRCRAFT_SINCE_START = AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[aircraft].AIRCRAFT_ITEM.DISTANCE_FROM_BASE;
          AIRCRAFTS_MAP.PANEL_FLAG_LONG_DISTANCE = true;
        }
      }
    }

    if (AIRCRAFTS_MAP.PANEL_FLAG_PROXIMITY || AIRCRAFTS_MAP.PANEL_FLAG_LONG_DISTANCE || AIRCRAFTS_MAP.PANEL_FLAG_EMERGENCY)
    {
      AIRCRAFTS_MAP.PANEL_FLAG_CHANGED = true;
    }

  }
}

// -------------------------------------------------------------------------------------

bool AIRCRAFT_COORDINATOR::is_active()
{
  return IS_ACTIVE;
}

// -------------------------------------------------------------------------------------

bool AIRCRAFT_COORDINATOR::process(unsigned long tmeCurrentTime, string JSON_Text, COMMAND_THREAD &Thread, SOUNDS &Sound_System, 
                                    bool GPS_Avail, float Current_Latitude, float Current_Longitude)
{
  bool ret_success = false;

  if (GPS_Avail)
  {
    DATA.CURRENT_POS_AVAIL = true;
    DATA.CURRENT_LAT = Current_Latitude;
    DATA.CURRENT_LON = Current_Longitude;
  }
  else
  {
    DATA.CURRENT_POS_AVAIL = false;
  }

  if (AIRCRAFT_JSON.load_json_from_string(JSON_Text))
  {
    IS_ACTIVE = true;

    // Clear the current list for new list replacement.
    DATA.AIRCRAFTS.clear();
    DATA.POSITIONED_AIRCRAFT = 0;

    for(int root = 0; root < (int)AIRCRAFT_JSON.ROOT.DATA.size(); root++)
    {
      // Load Top Level Messages
      if (AIRCRAFT_JSON.ROOT.DATA[root].label() == "now")
      {
        DATA.NOW.store(AIRCRAFT_JSON.ROOT.DATA[root].value());
      }
      if (AIRCRAFT_JSON.ROOT.DATA[root].label() == "messages")
      {
        int prev_message_count = DATA.MESSAGES.get_int_value();
        DATA.MESSAGES.store(AIRCRAFT_JSON.ROOT.DATA[root].value());
        DATA.DELTA_MESSAGES = DATA.MESSAGES.get_int_value() - prev_message_count;
      }

      // Process Aircraft List
      if (AIRCRAFT_JSON.ROOT.DATA[root].label() == "aircraft")
      {
        for (int aircraft_list = 0; 
              aircraft_list < (int)AIRCRAFT_JSON.ROOT.DATA[root].DATA.size(); aircraft_list++)
        {
          // For Each Aircraft

          AIRCRAFT tmpAircraft;

          string first = "";
          string second = "";

          string tmp = "";
          string version = "";

          // Process Messages in Aircraft
          for (int data_element = 0; 
                data_element < (int)AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA.size(); data_element++)
          {
            // For Each Data Element

            // Idents
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("hex", tmpAircraft.HEX);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("squawk", tmpAircraft.SQUAWK);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("flight", tmpAircraft.FLIGHT);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("version", tmpAircraft.VERSION);
            
            // Speed Rate Positions
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("gs", tmpAircraft.SPEED);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("seen_pos", tmpAircraft.SEEN_POS);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("alt_baro", tmpAircraft.ALTITUDE);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("alt_geom", tmpAircraft.ALTITUDE_GEOM);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("nav_altitude_mcp", tmpAircraft.NAV_ALTITUDE_MCP);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("baro_rate", tmpAircraft.VERT_RATE);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("geom_rate", tmpAircraft.GEOM_RATE);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("track", tmpAircraft.TRACK);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("nav_heading", tmpAircraft.NAV_HEADING);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("lat", tmpAircraft.POSITION.LATITUDE);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("lon", tmpAircraft.POSITION.LONGITUDE);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("nav_modes", tmpAircraft.NAV_MODES);

            // Aircraft Configuration
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("emergency", tmpAircraft.EMERGENCY);
            
            //  Not yet grabbing all data.
            //      .
            //      .
            //      .

            // Radio Information
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("messages", tmpAircraft.MESSAGES);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("seen", tmpAircraft.SEEN);
            AIRCRAFT_JSON.ROOT.DATA[root].DATA[aircraft_list].DATA[data_element].get_if_is("rssi", tmpAircraft.RSSI);
          }

          if(tmpAircraft.POSITION.LATITUDE.conversion_success() == true &&  tmpAircraft.POSITION.LONGITUDE.conversion_success() == true)
          {
            DATA.POSITIONED_AIRCRAFT ++;
          }

          // Process Data Recieved on Aircraft
          tmpAircraft.post_process();

          // Store Aircraft ADS-B Data into list.
          DATA.AIRCRAFTS.push_back(tmpAircraft);
        }
      }
    }

    // Process Data Recieved on All Aircraft
    post_post_process();
    DATA.CHANGED = true;
    ret_success = true;
  }  
  else
  {
    if(IS_ACTIVE == true)
    {
      DATA.CHANGED = true;
    }

    IS_ACTIVE = false;
    ret_success = false;
  }

  AIRCRAFTS_MAP.update(tmeCurrentTime, DATA);

  post_post_post_process(Thread, Sound_System);

  return ret_success;
}



#endif