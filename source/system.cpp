// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: system.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef SYSTEM_CPP
#define SYSTEM_CPP

#include "system.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

bool FILES::assign(CONSOLE_COMMUNICATION &cons)
{
  FALSE_CATCH ret_success;

  // Get Home Directory
  const char *username = getenv("SUDO_USER");
  struct passwd *pw = username ? getpwnam(username) : nullptr;
  const char *homedir = pw ? pw->pw_dir : nullptr;
  string homedir_str = (string)homedir + "/";


  //USB_DEV_DIR = (string)DEF_USB_DEV_DIR;
  AIRCRAFT_1090_DIR = (string)DEF_AIRCRAFT_1090_DIR;

  // Check Directories
  if (!TEST_MODE)
  {
    WORKING_DIR = homedir_str + (string)DEF_WORKING_DIR;
  }
  else
  {
    WORKING_DIR = homedir_str + (string)DEF_WORKING_DIR_TEST;
  }

  LOGS_CAN_BUS_DIR = WORKING_DIR +  (string)DEF_CAN_BUS_SUB_DIR;
  LOGS_AIRCRAFT_DIR = WORKING_DIR + (string)DEF_AIRCRAFT_SUB_DIR;
  LOGS_GPS_DIR = WORKING_DIR + (string)DEF_GPS_SUB_DIR;
  LOGS_ADVERTS_DIR = WORKING_DIR + (string)DEF_ADVERTS_SUB_DIR;
  LOGS_AUDIO_DIR = WORKING_DIR + (string)DEF_AUDIO_DIR;
  LOGS_IMAGES_DIR = WORKING_DIR + (string)DEF_IMAGES_DIR;
  LOGS_SETTINGS_DIR = WORKING_DIR + (string)DEF_SETTINGS_DIR;
  LOGS_COMPASS_DIR = WORKING_DIR + (string)DEF_COMPASS_SUB_DIR;
  LOGS_MAPS_DIR = WORKING_DIR + (string)DEF_MAP_DIR;
  CAMERA_DIR = WORKING_DIR + (string)DEF_CAMERA_DIR;
  CAMERA_SETTINGS_DIR = CAMERA_DIR + (string)DEF_CAMERA_SETTINGS_DIR;

  LOGS_CAN_BUS_HISTORY_SUB_DIR = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_HISTORY_LOG_SUB_DIR;
  TRACK_ARCHIVE_DIR = LOGS_MAPS_DIR + (string)DEF_TRACK_ARCHIVE_DIR;
  
  // Crash occurs if directory to be created is within a directory that doesnt exist.
  ret_success.catch_false(check_create_working_dir(WORKING_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_CAN_BUS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_CAN_BUS_HISTORY_SUB_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_AIRCRAFT_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_GPS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_ADVERTS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_AUDIO_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_IMAGES_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_SETTINGS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_COMPASS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_MAPS_DIR, true));
  ret_success.catch_false(check_create_working_dir(TRACK_ARCHIVE_DIR, true));
  ret_success.catch_false(check_create_working_dir(CAMERA_DIR, true));
  ret_success.catch_false(check_create_working_dir(CAMERA_SETTINGS_DIR, true));

  
  if (!ret_success.has_false())
  {
    cons.printw("File Assignments:");

    // Main Control for working rasfled
    ANIMATIONS_FILE = LOGS_SETTINGS_DIR + (string)DEF_ANIMATIONS_FILE;
    cons.printw("  " + ANIMATIONS_FILE);

    CONFIGURATION_FILE = LOGS_SETTINGS_DIR + (string)DEF_CONFIGURATION;
    cons.printw("  " + CONFIGURATION_FILE);

    RUNNING_STATE_FILE = LOGS_SETTINGS_DIR + (string)DEF_RUNNING_STATE;
    cons.printw("  " + RUNNING_STATE_FILE);

    QR_CODE_FILE = LOGS_IMAGES_DIR + (string)DEF_QR_CODE;
    cons.printw("  " + QR_CODE_FILE);

    REFERENCE_CARD_FILE = LOGS_IMAGES_DIR + (string)DEF_REFERENCE_CARD;
    cons.printw("  " + REFERENCE_CARD_FILE);

    // // CAN Bus
    CAN_BUS_HISTORY_FILE = LOGS_CAN_BUS_HISTORY_SUB_DIR + (string)DEF_CAN_BUS_SAVE_LOG_NAME;
    cons.printw("  " + CAN_BUS_HISTORY_FILE);

    CAN_BUS_LOG_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_SAVE_LOG_NAME;
    cons.printw("  " + CAN_BUS_LOG_FILE);

    CAN_BUS_TEST_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_TEST_DATA_NAME;
    cons.printw("  " + CAN_BUS_TEST_FILE);

    CAN_BUS_ERROR_LOG_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_ERROR_LOG;
    cons.printw("  " + CAN_BUS_ERROR_LOG_FILE);

    // GPS
    GPS_LOG_FILE = LOGS_GPS_DIR + (string)DEF_GPS_SAVE_LOG_NAME;
    cons.printw("  " + GPS_LOG_FILE);

    GPS_TEST_FILE = LOGS_GPS_DIR + (string)DEF_GPS_TEST_DATA_NAME;
    cons.printw("  " + GPS_TEST_FILE);

    // Compass
    COMPASS_OFFSET_HISTORY_FILE = LOGS_COMPASS_DIR + (string)DEF_COMPASS_OFFSET_HISTORY;
    cons.printw("  " + COMPASS_OFFSET_HISTORY_FILE);

    // Maps
    LOGS_MAPS_GENERICS_JSON = LOGS_MAPS_DIR + (string)DEF_MAP_GENERICS_JSON;
    cons.printw("  " + LOGS_MAPS_GENERICS_JSON);

    LOGS_MAPS_AIRPORTS_JSON = LOGS_MAPS_DIR + (string)DEF_MAP_AIRPORTS_JSON;
    cons.printw("  " + LOGS_MAPS_AIRPORTS_JSON);

    LOGS_MAPS_REGIONS_JSON = LOGS_MAPS_DIR + (string)DEF_MAP_REGIONS_JSON;
    cons.printw("  " + LOGS_MAPS_REGIONS_JSON);

    LOGS_MAPS_ROADS_JSON = LOGS_MAPS_DIR + (string)DEF_MAP_ROADS_JSON;
    cons.printw("  " + LOGS_MAPS_ROADS_JSON);

    TRACK_CURRENT_JSON = LOGS_MAPS_DIR + (string)DEF_CURRENT_TRACK_JSON;
    cons.printw("  " + TRACK_CURRENT_JSON);

    // Camera
    CAMERA_TEST_IMAGE_FILENAME = CAMERA_DIR + (string)DEF_CAMERA_TEST_IMAGE;
    cons.printw("  " + CAMERA_TEST_IMAGE_FILENAME);

    // Specific Hardware Directories and Files
    cons.printw("");
    cons.printw("Hardware and External Sources:");

    DEAMON_LOG = DEF_DEAMON_LOG;
    cons.printw("  " + DEAMON_LOG);

    {
      if (!TEST_DATA_AIRCRAFT)
      {
        AIRCRAFT_FA_FILE = AIRCRAFT_1090_DIR + (string)DEF_AIRCRAFT_FA_JSON_FILE;
      }
      else
      {
        AIRCRAFT_FA_FILE = LOGS_AIRCRAFT_DIR + (string)DEF_AIRCRAFT_FA_JSON_FILE;
      }
      cons.printw("  " + AIRCRAFT_FA_FILE);
    }

    CAN_BUS_DEVICE_FILE = (string)DEF_CAN_BUS_DEV_NAME;
    cons.printw("  " + CAN_BUS_DEVICE_FILE);

    GPS_DEVICE_FILE = (string)DEF_GPS_DEV_NAME;
    cons.printw("  " + GPS_DEVICE_FILE);

    COMPASS_DEVICE_FILE = (string)DEF_COMPASS_DEV_NAME;
    cons.printw("  " + COMPASS_DEVICE_FILE);

    CAMERA_BACKUP = (string)DEF_CAMERA_BACKUP;
    CAMERA_BACKUP_ID = (int)DEF_CAMERA_BACKUP_ID;
    cons.printw("  ID: " + to_string(CAMERA_BACKUP_ID) + " \t" + CAMERA_BACKUP);

    cons.printw("");
  }

  return !ret_success.has_false();
}

// -------------------------------------------------------------------------------------

void system_data::init()
{
  for(int x=0; x<CONFIG.iNUM_SWITCHES; x++)
  {
    boolDOOR_SENSOR_STATUS.push_back(false);
  }

  for(int x=0; x<CONFIG.iNUM_CHANNELS; x++)
  {
    intCHANNEL_GROUP_EVENTS_COUNTS.push_back(0);
  }
}

void system_data::store_Current_Frame_Time(unsigned long tmeCurrent_Time_millis)
{
  //tmeCURRENT_FRAME_TIME = tmeCurrent_Time_millis;
  cdTIMER.update(tmeCurrent_Time_millis);
}

// Reference to the door values
void system_data::store_door_switch_states()
{
  for(int x=0; x < CONFIG.iNUM_SWITCHES; x++)
  {
    boolDOOR_SENSOR_STATUS.at(x) = CONFIG.vSWITCH_PIN_MAP.at(x).value;
  }
}

void system_data::init_running_color_list()
{
  running_color_list.strRunningColor = "Blank";
  running_color_list.color[0] = CRGB(0,0,0);
  running_color_list.color[1] = CRGB(0,64,0);  // Green
  running_color_list.color[2] = CRGB(48,48,0); // Yellow
  running_color_list.color[3] = CRGB(64,0,0);  // Red
  running_color_list.color[4] = CRGB(20,20,20); // White
  running_color_list.color[5] = CRGB(64,16,0); // Orange
}

CRGB system_data::get_running_color()
{
  return running_color_list.color[0];
}

string system_data::get_running_color_str()
{
  return running_color_list.strRunningColor;
}

CRGB system_data::get_countdown_color()
{
  if (cdTIMER.is_active() == false)
  {
    return running_color_list.color[0];
  }
  else
  {
    // Compute the timer color.
    CRGB countdown_color;

    // Get % complete
    float pos = cdTIMER.timer_position(PROGRAM_TIME.current_frame_time());

    // Get Section of Color List
    int section = floor((running_color_list.size -1) * pos);
    
    // Determine size in time of each secion.
    float section_size = 1 / (float)(running_color_list.size -1);

    // Get % Section complete
    unsigned long section_time_removed = section * (cdTIMER.duration() * section_size);
    unsigned long section_elaped_time = cdTIMER.elapsed_time(PROGRAM_TIME.current_frame_time()) - section_time_removed;

    float pwr = ComputePower(section_elaped_time, cdTIMER.duration() * section_size);

    // Dither Color and get results
    if (section < running_color_list.size -1)
    {
      bigCRGB big_countdown_color = Dither(pwr, running_color_list.color[section], running_color_list.color[section + 1]);
      
      countdown_color.r = big_countdown_color.r;
      countdown_color.g = big_countdown_color.g;
      countdown_color.b = big_countdown_color.b;
    }
    else
    {
      //cdTIMER.end();
      countdown_color = running_color_list.color[running_color_list.size - 1];
    }

    return countdown_color;
  }
}

void system_data::set_running_color(CRGB Running_Color, string strColor)
{
  running_color_list.color[0] = Running_Color;
  running_color_list.strRunningColor = strColor;
}

void system_data::start_timer(int Seconds)
{
  cdTIMER.set_timer(PROGRAM_TIME.current_frame_time(), Seconds);
}

void system_data::read_hardware_status(int Milis_Frequency)
{
  hsHardware_Status.read_hardware_status(PROGRAM_TIME.current_frame_time(), Milis_Frequency);
}

// -------------------------------------------------------------------------------------



#endif