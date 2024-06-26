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

bool FILES::assign()
{
  FALSE_CATCH ret_success;
  USB_DEV_DIR = (string)DEF_USB_DEV_DIR;
  AIRCRAFT_1090_DIR = (string)DEF_AIRCRAFT_1090_DIR;

  // Check Directories
  if (!TEST_MODE)
  {
    WORKING_DIR = (string)DEF_WORKING_DIR;
  }
  else
  {
    WORKING_DIR = (string)DEF_WORKING_DIR_TEST;
  }

  COMPASS_DIR = WORKING_DIR + (string)DEF_COMPASS_SUB_DIR;

  if (!TEST_MODE)
  {
    LOGS_DIR = (string)DEF_LOGS_DIR;
  }
  else
  {
    LOGS_DIR = (string)DEF_LOGS_DIR_TEST;
  }

  LOGS_CAN_BUS_DIR = LOGS_DIR +  (string)DEF_CAN_BUS_SUB_DIR;
  LOGS_CAN_BUS_HISTORY_SUB_DIR = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_HISTORY_LOG_SUB_DIR;
  LOGS_AIRCRAFT_DIR = LOGS_DIR + (string)DEF_AIRCRAFT_SUB_DIR;
  LOGS_GPS_DIR = LOGS_DIR + (string)DEF_GPS_SUB_DIR;
  LOGS_ADVERTS_DIR = LOGS_DIR + (string)DEF_ADVERTS_SUB_DIR;
  
  // Crash occurs if directory to be created is within a directory that doesnt exist.
  ret_success.catch_false(check_create_working_dir(WORKING_DIR, true));
  ret_success.catch_false(check_create_working_dir(COMPASS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_CAN_BUS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_CAN_BUS_HISTORY_SUB_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_AIRCRAFT_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_GPS_DIR, true));
  ret_success.catch_false(check_create_working_dir(LOGS_ADVERTS_DIR, true));
  
  if (!ret_success.has_false())
  {
    // Specific Directories and Files
    DEAMON_LOG = DEF_DEAMON_LOG;
    AIRCRAFT_FA_FILE = AIRCRAFT_1090_DIR + (string)DEF_AIRCRAFT_FA_JSON_FILE;

    // Main Control for working rasfled
    ANIMATIONS_FILE = WORKING_DIR + (string)DEF_ANIMATIONS_FILE;
    CONFIGURATION_FILE = WORKING_DIR + (string)DEF_CONFIGURATION;
    RUNNING_STATE_FILE = WORKING_DIR + (string)DEF_RUNNING_STATE;
    QR_CODE_FILE = WORKING_DIR + (string)DEF_QR_CODE;
    REFERENCE_CARD_FILE = WORKING_DIR + (string)DEF_REFERENCE_CARD;

    // // CAN Bus
    CAN_BUS_DEVICE_FILE = USB_DEV_DIR + (string)DEF_CAN_BUS_USB_DEV_NAME;
    CAN_BUS_HISTORY_FILE = LOGS_CAN_BUS_HISTORY_SUB_DIR + (string)DEF_CAN_BUS_SAVE_LOG_NAME;
    CAN_BUS_LOG_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_SAVE_LOG_NAME;
    CAN_BUS_TEST_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_TEST_DATA_NAME;
    CAN_BUS_ERROR_LOG_FILE = LOGS_CAN_BUS_DIR + (string)DEF_CAN_BUS_ERROR_LOG;

    // GPS
    GPS_DEVICE_FILE = USB_DEV_DIR + (string)DEF_GPS_USB_DEV_NAME;
    GPS_LOG_FILE = LOGS_GPS_DIR + (string)DEF_GPS_SAVE_LOG_NAME;
    GPS_TEST_FILE = LOGS_GPS_DIR + (string)DEF_GPS_TEST_DATA_NAME;

    // Compass
    COMPASS_DEVICE_FILE = (string)DEF_COMPASS_DEV_NAME;
    COMPASS_OFFSET_HISTROY_FILE = COMPASS_DIR + (string)DEF_COMPASS_OFFSET_HISTORY;
  }

  return !ret_success.has_false();
}

void SECTION_STATUS::on(bool &Needs_Refresh)
{
  if (Value == false)
  {
    Value = true;
    Needs_Refresh = true;
  }
}

void SECTION_STATUS::off(bool &Needs_Refresh)
{
  if (Value == true)
  {
    Value = false;
    Needs_Refresh = true;
  }
}

bool SECTION_STATUS::value()
{
  return Value;
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
  booRunning_State_File_Dirty = true;
}

void system_data::start_timer(int Seconds)
{
  cdTIMER.set_timer(PROGRAM_TIME.current_frame_time(), Seconds);
}

double system_data::store_sleep_time(double tmeSleep)
// Pass through variable
// Stores the Sleep time to be displayed in diag, then returns the same value.
{
  dblPREVSLEEPTIME.set_data(tmeSleep);
  return tmeSleep;
}

double system_data::get_sleep_time(double Current_Time, unsigned long Wake_Time)
{
  // Return, in microseconds, the amount of time required to sleep.
  
  double sleeptime = 0;

  if(Current_Time < Wake_Time)
  {
    sleeptime = (unsigned long)Wake_Time - Current_Time;
  }

  return sleeptime;
}

void system_data::read_hardware_status(int Milis_Frequency)
{
  hsHardware_Status.read_hardware_status(PROGRAM_TIME.current_frame_time(), Milis_Frequency);
}

// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Tacks all key presses and remembers data associated to whatever key is pressed.

void Keys::cmdClear()
// Clear Command line
{
  Command.COMMANDLINE = "";
  Command.PRESSED = true;
  Command.CLEARED = true;
  Command.CARRAGE_RETURN = false;
}

bool Keys::cmdPressed()
// Return if Command Line Recently Changed.
{
  return Command.PRESSED;
}

bool Keys::cmdCleared()
// Return if the Command was cleared. Reset it after read if it was.
{
  if (Command.CLEARED == false)
  {
    return false;
  }
  else
  {
    Command.CLEARED = false;
    return true;
  }
}

string Keys::cmdRead()
// Read Command Line and sets recently changed to false (no longer dirty)
{
  Command.PRESSED = false;
  return(Command.COMMANDLINE);
}

void Keys::cmdIn(int c)
// Add another character to the command line.
{
  if(c == (char)'\n')
  {
    // If enter pressed, set the flag.
    Command.CARRAGE_RETURN = true;
    Command.PRESSED = true;
  }
  else
  {
    //if( (c>47 && c<57) || (c>65 && c<90) || (c>97 && c<122) )
    if( (c>=32 && c<126) )
    {
      // only accept letters and numbers.
      Command.COMMANDLINE = Command.COMMANDLINE + (char)c;
      Command.PRESSED = true;

      // Limit Command Line Size. Clear when too big.
      if (Command.COMMANDLINE.size() > 10)
      {
        cmdClear();
      }
    }
  }
}

void Keys::cmdInString(string cmd)
// Add another character to the command line.
{
  Command.COMMANDLINE = cmd;
  Command.PRESSED = true;

  if (Command.COMMANDLINE.size() > 10)
  {
    cmdClear();
  }
}

void Keys::set(int letter, int size)
// Set behavior of letter to be watched.
{
  Chars[letter].COUNT = size - 1;
  Chars[letter].VALUE = 0;
}

void Keys::in(int c)
// Letter watched is pressed, change its value atnd set pressed (dirty)
{
  if (Chars[c].ACTIVE == true)
  {
    Chars[c].VALUE++;
    Chars[c].PRESSED = true;
    if (Chars[c].VALUE > Chars[c].COUNT)
    {
      Chars[c].VALUE = 0;
    }
  }
}

int Keys::get(int c)
// return value of letter and reset pressed (clean)
{
  Chars[c].PRESSED = false;
  return Chars[c].VALUE;
}

int Keys::getnoreset(int c)
// return value of letter and reset pressed (clean)
{
  return Chars[c].VALUE;
}

// returns true false value of letter, reset pressed (clean)
bool Keys::getTF(int c)
{
  Chars[c].PRESSED = false;
  if (Chars[c].VALUE == 0)
    return false;
  else
    return true;
}

// returns pressed value (clean or dirty)
bool Keys::pressed(int c)
{
  return (Chars[c].PRESSED);
}
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Keeps track of mouse stuff.

int TheMouse::x()   // X Pos
{
  return X;
}

int TheMouse::y()   // Y Pos
{
  return Y;
}

//MEVENT mouse_event()   // B Pos
int TheMouse::b() // B Pos
{
  //return MOUSE_EVENT;
  return B;
}

int TheMouse::x_clicked()   // Clicked X pos
{
  return XCLICK;
}

int TheMouse::y_clicked()   // Clicked Y pos
{
  return YCLICK;
}

void TheMouse::check_click()
// Do stuff on click down and click up.
{
  //if (MOUSE_EVENT.bstate & (BUTTON1_CLICKED || BUTTON1_RELEASED))
  if (B == 1 || B == 4)
  // Mouse Button Down and Up.
  {
    XCLICK = X;
    YCLICK = Y;
    CLICKED = true;
  }
  //else if (MOUSE_EVENT.bstate & BUTTON1_PRESSED)
  else if (B == 2)
  // Mouse Button Down.
  {
    XDOWN = X;
    YDOWN = Y;
    CLICKED = false;
  }
}

//void store(int x, int y, MEVENT &mouse_event)
void TheMouse::store(int x, int y, int b)
// Put mouse changes into storage.
{
  X = x;
  Y = y;
  //MOUSE_EVENT = mouse_event;
  B = b;

  check_click();
}

bool TheMouse::Clicked()
// Check Clicked status.  If true clear click status.
{
  if (CLICKED == false)
  {
    return false;
  }
  else 
  {
    CLICKED = false;
    return true;
  }
}

// -------------------------------------------------------------------------------------



#endif