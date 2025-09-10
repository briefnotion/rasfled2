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

void track_save_detailed(const DETAILED_TRACK_ALTERNATIVE &Track, const string Filename)
{
  // Track
  JSON_INTERFACE track_map;
  JSON_ENTRY all_track;
  deque<string> track_json_deque;

  JSON_ENTRY points;

  // Detailed
  for (size_t pos = 0; pos < Track.TRACK_POINTS_DETAILED.size(); pos++)
  {
    JSON_ENTRY position_info;
    position_info.create_label_value(quotify("unix_epoch_nmea_time"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].TIMESTAMP)));
    position_info.create_label_value(quotify("accuracy"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].ACCURACY)));
    position_info.create_label_value(quotify("value"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].VALUE)));
    position_info.create_label_value(quotify("resolution"), quotify(to_string(Track.TRACK_POINTS_DETAILED[pos].RESOLUTION)));

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

  threaded_deque_string_to_file(Filename, track_json_deque);
}

// -------------------------------------------------------------------------------------

void PIXEL_SIZE_META_DATA::create()
{
  LOD.clear();
  LOD_TRACK_LIST.clear();

  LOD.push_back(  0.01  / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  0.03  / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  0.05  / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  0.1   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  0.25  / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  0.5   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  1.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  2.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  5.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(  7.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 10.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 15.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 25.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 35.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 50.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back( 75.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);
  LOD.push_back(100.0   / (double)CURRENT_CIRCLE_SIZE_PIXELS);

  DOUBLE_VEC2 tmp_vec;
  for (size_t pos = 0; pos < LOD.size(); pos++)
  {
    LOD_TRACK_LIST.push_back(tmp_vec);
  }
}

void PIXEL_SIZE_META_DATA::check_circle_size_pixels(float Circle_Size_Pixels)
{
  if (Circle_Size_Pixels != CURRENT_CIRCLE_SIZE_PIXELS)
  {
    if (Circle_Size_Pixels >= 1.0f)
    CURRENT_CIRCLE_SIZE_PIXELS = Circle_Size_Pixels;
    create();
  }
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
    if (map_hardcoded_load(LANDMARKS))
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

void MAP::track_distill(DETAILED_TRACK_ALTERNATIVE &Original_Track, 
                        DETAILED_TRACK_ALTERNATIVE &Recent_Track, 
                        DETAILED_TRACK_ALTERNATIVE &Old_Track)
{
  double current_system_time = get_current_timestamp();

  Recent_Track.clear();
  Old_Track.clear();

  for (size_t pos = 0; pos < Original_Track.TRACK_POINTS_DETAILED.size(); pos++)
  {
    if (current_system_time - Original_Track.TRACK_POINTS_DETAILED[pos].TIMESTAMP < PROPS.TRACK_HISTORY_CUTOFF)
    {
      Recent_Track.TRACK_POINTS_DETAILED.push_back(Original_Track.TRACK_POINTS_DETAILED[pos]);
    }
    else
    {
      Old_Track.TRACK_POINTS_DETAILED.push_back(Original_Track.TRACK_POINTS_DETAILED[pos]);
    }
  }
}

void MAP::track_save_detailed_forgetable(DETAILED_TRACK_ALTERNATIVE &Track, string Filename)
{
  DETAILED_TRACK_ALTERNATIVE  forgetable_track = Track;
  string                      forgetable_filename = Filename;

  thread_and_forget(track_save_detailed, forgetable_track, forgetable_filename);
}

bool MAP::track_load_detailed(DETAILED_TRACK_ALTERNATIVE &Track, 
                              DETAILED_TRACK_ALTERNATIVE &Track_Discard, 
                              string Filename)
{
  bool ret_success = false;
  bool tmp_success = false;

  DETAILED_TRACK_ALTERNATIVE tmp_load_track;
  
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
                DETAILED_TRACK_POINT location;
                STRING_DOUBLE timestamp;
                STRING_FLOAT  accuracy;
                STRING_FLOAT  value;  
                STRING_DOUBLE resolution;  
                STRING_DOUBLE location_latitude;
                STRING_DOUBLE location_longitude;

                for (size_t entry = 0;                                         //root/marker_list/points_entry/entry
                      entry < track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA.size(); entry++)
                {
                  track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].get_if_is("unix_epoch_nmea_time", timestamp);
                  track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].get_if_is("accuracy", accuracy);
                  track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].get_if_is("value", value);
                  track_json.ROOT.DATA[root].DATA[marker_list].DATA[points_entry].DATA[entry].get_if_is("resolution", resolution);

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
                location.ACCURACY = accuracy.get_float_value();
                location.VALUE = value.get_float_value();
                location.RESOLUTION = resolution.get_double_value();
                location.LATITUDE = location_latitude.get_double_value();
                location.LONGITUDE = location_longitude.get_double_value();

                tmp_load_track.TRACK_POINTS_DETAILED.push_back(location);
              }
            }
          }
        }
      }
    }

    track_distill(tmp_load_track, Track, Track_Discard);
  }

  /*
  if (!Track.TRACK_POINTS_DETAILED.empty())
  {
  cout << "     Track track size: " << Track.TRACK_POINTS_DETAILED.size() << endl;
  cout << "Track timestamp front: " << to_string(Track.TRACK_POINTS_DETAILED.front().TIMESTAMP) << endl;
  cout << "Track timestamp back : " << to_string(Track.TRACK_POINTS_DETAILED.back().TIMESTAMP) << endl;
  cout << "Track diff      front: " << to_string(current_system_time - Track.TRACK_POINTS_DETAILED.front().TIMESTAMP) << endl;
  cout << "Track diff      back : " << to_string(current_system_time - Track.TRACK_POINTS_DETAILED.back().TIMESTAMP) << endl;
  }

  if (!Track_Discard.TRACK_POINTS_DETAILED.empty())
  {
    
  cout << "     Track_Discard track size: " << Track_Discard.TRACK_POINTS_DETAILED.size() << endl;
  cout << "Track_Discard timestamp front: " << to_string(Track_Discard.TRACK_POINTS_DETAILED.front().TIMESTAMP) << endl;
  cout << "Track_Discard timestamp back : " << to_string(Track_Discard.TRACK_POINTS_DETAILED.back().TIMESTAMP) << endl;
  cout << "Track_Discard diff      front: " << to_string(current_system_time - Track_Discard.TRACK_POINTS_DETAILED.front().TIMESTAMP) << endl;
  cout << "Track_Discard diff      back : " << to_string(current_system_time - Track_Discard.TRACK_POINTS_DETAILED.back().TIMESTAMP) << endl;
  }
  */

  return ret_success;
}

void MAP::rebuild_track(CONSOLE_COMMUNICATION &cons)
{
  DETAILED_TRACK_ALTERNATIVE new_track;
  DETAILED_TRACK_ALTERNATIVE discard_track;

  track_distill(TRACK_2, new_track, discard_track);
  TRACK_2 = new_track;

  if (discard_track.TRACK_POINTS_DETAILED.size() > 0)
  {
    string track_history_filename = PROPS.TRACK_HISTORY_FOLDER + "track_" + 
                                    file_format_system_time(discard_track.TRACK_POINTS_DETAILED[0].TIMESTAMP) + 
                                    ".json";

    cons.printw("Initiated history save \"" + track_history_filename);
    track_save_detailed_forgetable(discard_track, track_history_filename);
  }
}

void MAP::generate_displayed_track(double Resolution)
{
  DISPLAYED_TRACK.clear();

  // Always include first and last point
  if (TRACK_2.TRACK_POINTS_DETAILED.size() > 1)
  {
    // Skip map generation is screen size is zero.  Likely never opened.
    if (CURRENT_WORKING_AREA.z > 0.0 && CURRENT_WORKING_AREA.w > 0.0f)
    {
      // To determine if track point should be included.
      ImVec2 screen_position_not_used;
      bool add_track_point = false;     // add point if within working area.
      ImVec4 extar_working_area;

      // give working area extra room
      // for now, just increas the size by 50 pixels top down left right
      // X = x starting pos (position of left most window, if no write)
      // Y = y starting pos (position of top most window, if no write)
      // Z = x size
      // W = y size
      extar_working_area.x = CURRENT_WORKING_AREA.x - 50.0f;
      extar_working_area.y = CURRENT_WORKING_AREA.y - 50.0f;
      extar_working_area.z = CURRENT_WORKING_AREA.z + 100.0f;
      extar_working_area.w = CURRENT_WORKING_AREA.w + 100.0f;

      // First point
      screen_position_not_used = point_position_lat_lon(extar_working_area, CURRENT_LAT_LON_SCALE, CURRENT_CENTER_LAT_LON, 
                                                        DOUBLE_VEC2(TRACK_2.TRACK_POINTS_DETAILED.front().LATITUDE, 
                                                                    TRACK_2.TRACK_POINTS_DETAILED.front().LONGITUDE ), 
                                                        CURRENT_MAP_BEARING, add_track_point);

      if (add_track_point)
      {
        DISPLAYED_TRACK.TRACK_POINTS_DETAILED.push_back(TRACK_2.TRACK_POINTS_DETAILED.front());
      }

      // Points in middle
      for (size_t pos = 1; pos < TRACK_2.TRACK_POINTS_DETAILED.size() -1; pos++)
      {
        if (TRACK_2.TRACK_POINTS_DETAILED[pos].RESOLUTION >= Resolution)
        {
          screen_position_not_used = point_position_lat_lon(extar_working_area, CURRENT_LAT_LON_SCALE, CURRENT_CENTER_LAT_LON, 
                                                  DOUBLE_VEC2(TRACK_2.TRACK_POINTS_DETAILED[pos].LATITUDE, 
                                                              TRACK_2.TRACK_POINTS_DETAILED[pos].LONGITUDE ), 
                                                  CURRENT_MAP_BEARING, add_track_point);

          if (add_track_point)
          {
            DISPLAYED_TRACK.TRACK_POINTS_DETAILED.push_back(TRACK_2.TRACK_POINTS_DETAILED[pos]);
          }
          else
          {
            if (!DISPLAYED_TRACK.TRACK_POINTS_DETAILED.empty())
            {
              DISPLAYED_TRACK.TRACK_POINTS_DETAILED.back().END_POINT = true;
            }
          }
        }
      }

      screen_position_not_used = point_position_lat_lon(extar_working_area, CURRENT_LAT_LON_SCALE, CURRENT_CENTER_LAT_LON, 
                                                        DOUBLE_VEC2(TRACK_2.TRACK_POINTS_DETAILED.back().LATITUDE, 
                                                                    TRACK_2.TRACK_POINTS_DETAILED.back().LONGITUDE ), 
                                                        CURRENT_MAP_BEARING, add_track_point);

      if (add_track_point)
      {
        DISPLAYED_TRACK.TRACK_POINTS_DETAILED.push_back(TRACK_2.TRACK_POINTS_DETAILED.back());
      }
    }
  }
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

void MAP::load_track(CONSOLE_COMMUNICATION &cons, double Time_error)
{
  DETAILED_TRACK_ALTERNATIVE  tmp_discard_track;

  TIME_ERROR = Time_error;

  if (track_load_detailed(TRACK_2, tmp_discard_track, PROPS.CURRENT_TRACK_FILENAME))
  {
    cons.printw("Successfully loaded \"" + PROPS.CURRENT_TRACK_FILENAME);

    if (tmp_discard_track.TRACK_POINTS_DETAILED.size() > 0)
    {
      string track_history_filename = PROPS.TRACK_HISTORY_FOLDER + "track_" + 
                                      file_format_system_time(tmp_discard_track.TRACK_POINTS_DETAILED[0].TIMESTAMP) + 
                                      ".json";

      cons.printw("Initiated history save \"" + track_history_filename);
      track_save_detailed_forgetable(tmp_discard_track, track_history_filename);
    }
  }
  else
  {
    cons.printw("Failed to loaded \"" + PROPS.CURRENT_TRACK_FILENAME);
  }
}

void MAP::update( CONSOLE_COMMUNICATION &cons, NMEA &GPS_System, unsigned long tmeFrame_Time)
{

  // Determine if the track should be rebuilt
  double new_time_error = GPS_System.current_time_error();
  if (abs(TIME_ERROR - new_time_error) > PROPS.TRACK_HISTORY_CUTOFF)
  {
    cons.printw("Track rebuild initiated because of time change.");
    TIME_ERROR = new_time_error;
    rebuild_track(cons);
  }

  // Save new track point
  if (GPS_System.CURRENT_POSITION.CHANGED_POSITION && 
      GPS_System.CURRENT_POSITION.CHANGED_TIME)
  {
    GPS_System.CURRENT_POSITION.CHANGED_POSITION = false;
    GPS_System.CURRENT_POSITION.CHANGED_TIME = false;

    if (GPS_System.valid_position())
    {
      DETAILED_TRACK_POINT tmp_track_point;
      LEVEL_OF_DETAIL_META.check_circle_size_pixels(RANGE_RADIUS_CIRCLE_POINT_SIZE);

      // get distand from previous point
      if (TRACK_2.TRACK_POINTS_DETAILED.size() > 0)
      {
        // Determine LOD
        // Step backwards through the list to find the highest lod
        int found_pos = 0;
        for (int search = (int)LEVEL_OF_DETAIL_META.LOD.size() -1; search > 0 && found_pos == 0; search--)
        {
          double distance_lod_pos = calculate_distance(LEVEL_OF_DETAIL_META.LOD_TRACK_LIST[search].x, 
                                                        LEVEL_OF_DETAIL_META.LOD_TRACK_LIST[search].y, 
                                                        GPS_System.CURRENT_POSITION.LATITUDE, 
                                                        GPS_System.CURRENT_POSITION.LONGITUDE);

          if (distance_lod_pos >= LEVEL_OF_DETAIL_META.LOD[search])
          {
            found_pos = search;
          }
        }

        // Replace current and below with point
        for (int search = 0; search <= found_pos; search++)
        {
          LEVEL_OF_DETAIL_META.LOD_TRACK_LIST[search].x = GPS_System.CURRENT_POSITION.LATITUDE;
          LEVEL_OF_DETAIL_META.LOD_TRACK_LIST[search].y = GPS_System.CURRENT_POSITION.LONGITUDE;
        }

        tmp_track_point.RESOLUTION = LEVEL_OF_DETAIL_META.LOD[found_pos];
      }
      else if (TRACK_2.TRACK_POINTS_DETAILED.size() == 1)
      {
        // build LOD Track List

        DOUBLE_VEC2 tmp_gp;
        tmp_gp.x =  GPS_System.CURRENT_POSITION.LATITUDE;
        tmp_gp.y =  GPS_System.CURRENT_POSITION.LONGITUDE;

        for (size_t lodpos = 0; lodpos < LEVEL_OF_DETAIL_META.LOD_TRACK_LIST.size(); lodpos++)
        {
          LEVEL_OF_DETAIL_META.LOD_TRACK_LIST[lodpos] = tmp_gp;
        }
  
        tmp_track_point.RESOLUTION = LEVEL_OF_DETAIL_META.LOD[LEVEL_OF_DETAIL_META.LOD.size() -1];
      }

      tmp_track_point.TIMESTAMP = GPS_System.CURRENT_POSITION.UNIX_EPOC_NMEA_TIME;
      tmp_track_point.LATITUDE = GPS_System.CURRENT_POSITION.LATITUDE;
      tmp_track_point.LONGITUDE = GPS_System.CURRENT_POSITION.LONGITUDE;
      tmp_track_point.TRUE_HEADING = GPS_System.CURRENT_POSITION.TRUE_HEADING.VALUE;
      tmp_track_point.VALUE = GPS_System.CURRENT_POSITION.SPEED.val_mph();
      tmp_track_point.ACCURACY = GPS_System.CURRENT_POSITION.ACCURACY_SCORE;

      TRACK_2.store(tmp_track_point);

      if (CURRENT_RESOLUTION >= -1.0f)
      {
        generate_displayed_track(CURRENT_RESOLUTION);
      }
    } 
  }

  // Save Track
  if (SAVE_TRACK_TIMER.is_ready(tmeFrame_Time))
  {
    SAVE_TRACK_TIMER.set(tmeFrame_Time, PROPS.SAVE_TRACK_TIMER);

    track_save_detailed_forgetable(TRACK_2, PROPS.CURRENT_TRACK_FILENAME);
    cons.printw("Initiated save \"" + PROPS.CURRENT_TRACK_FILENAME);
  }
}

void MAP::close(CONSOLE_COMMUNICATION &cons)
{

  track_save_detailed_forgetable(TRACK_2, PROPS.CURRENT_TRACK_FILENAME);
  cons.printw("Initiated save \"" + PROPS.CURRENT_TRACK_FILENAME);
}

// -------------------------------------------------------------------------------------

#endif