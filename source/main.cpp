// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
/* *  PROJECTID: gi6$b*E>*q%; */
/* *  TEST CODE:              */ // CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************
// *  Programmer Notes:
// *
// *            Coder:  Robert Lebowski
// *    Support Coder:  None
// *  --- And:
// *    Other Sources:  None
// *
// *  Description:
// *    LED is an acronym for Light Emitting Diode. My goal for this little project is
// *    to create a control module multi colored light strips. The individual modules,
// *    running on an Arduino board, will be able to run as stand alone and or, while
// *    working together, as a single device. The module will be able to be
// *    preprogrammed to display certain effects when triggered by different
// *    preprogrammed events.
// *
// *    The first step in the project is to create a single working module.  It will
// *    need to be easily configured. It will need to be able to handle timed events,
// *    triggers events, calculated events, and possibly a combination of events triggers.
// *
// *    The second goal will be too create a communication system between the modules
// *    so that Multiple modules can be triggered by different events all module will
// *    be able to function independently or in parallel while working together.
// *
// *    The third goal will be to create a control module within each device. This
// *    control module device will just be added to each device as just another type of
// *    triggered event and will not be external code. It will just be another way to
// *    configure the device.
// *
// *    The fourth goal will be to determine a way to easily configure all devices in its
// *    system without the need to re flash each device independently. This goal is
// *    basically a central and easy way to setup all triggered events and reactions on
// *    all devices on its system by reading a simple program.
// *
// *    The F in FLED stands of Failed, as in I failed to receive backing for this
// *    project, and I failed to assemble a team to assist me with this. Because of
// *    this, the F can also stand for fiction, because at the time of writing this
// *    description, and having neither of those 2 things, that may be all this amounts
// *    to being.
// *
// *  Background:
// *    https://github.com/briefnotion/Fled/blob/master/Description%20and%20Background.txt
// *
// ***************************************************************************************

#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// Matrix Manipulation

// -------------------------------------------------------------------------------------
// Display Materix Prepare.

//  Copy the Prepared Matrix to the Display Matrix.
void MatrixPrepare(CRGB crgbPrepedMatrix[], int intLEDCOUNT, int* DisplayMatrix, int &mcount)
{
  for (int lcount = 0; lcount < intLEDCOUNT; lcount++)
  {
    // Normal Display
    DisplayMatrix[mcount]=crgbPrepedMatrix[lcount].b + (crgbPrepedMatrix[lcount].g << 8) + (crgbPrepedMatrix[lcount].r << 16) + (0 << 24);
    mcount++;
  }
}

void MatxixFill(CRGB crgbPreparedMatix[], int intLEDCOUNT, CRGB crgbColor)
{
  for (int lcount = 0; lcount < intLEDCOUNT; lcount++)
  {
    if ((lcount +1 ) == 1)
    {
      crgbPreparedMatix[lcount] = CRGB(25,0,25);
    }
    else if (((lcount +1 ) % 100) == 0)
    {
      crgbPreparedMatix[lcount] = CRGB(25,25,0);
    }
    else if (((lcount +1 ) % 50) == 0)
    {
      crgbPreparedMatix[lcount] = CRGB(25,0,0);
    }
    else if (((lcount +1 ) % 20) == 0)
    {
      crgbPreparedMatix[lcount] = CRGB(0,25,0);
    }
    else if (((lcount +1 ) % 5) == 0)
    {
      crgbPreparedMatix[lcount] = CRGB(0,0,25);
    }
    else
    {
      crgbPreparedMatix[lcount] = crgbColor;
    }
  }
}

void MatxixBlank(CRGB crgbPreparedMatix[], int intLEDCOUNT, CRGB crgbColor)
{
  for (int lcount = 0; lcount < intLEDCOUNT; lcount++)
  {
    crgbPreparedMatix[lcount] = crgbColor;
  }
}



// -------------------------------------------------------------------------------------
// Console Update

// Reference for the amount for events running.
void store_event_counts(system_data &sdSysData, ANIMATION_HANDLER Animations)
{
  for(int channel=0; channel < sdSysData.CONFIG.iNUM_CHANNELS; channel++)
  {
    sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(channel) = 0;
  }

  for(int group=0; group<sdSysData.CONFIG.LED_MAIN.at(0).g_size(); group++)
  {
    for(int strip=0; strip<sdSysData.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
    {
      int channel = sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).intCHANNEL;

      sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(group) =
                  sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(group) +
                  (Animations.EVENTS[channel].teDATA.size());

    }
  }
}


// ***************************************************************************************
// The Following chunk of code is what handles the imported library's ability to put the
// lights on and off or whatever.  Will not pretend to understand it.  Instead, will
// squish it down as much as possible so that I can pretend its not there.
// ***************************************************************************************
void matrix_render(int led_count)
{
    int x;

    for (x = 0; x < led_count; x++)
    {
    ledstring.channel[0].leds[x] = matrix[x];
    }
}
static void ctrl_c_handler(int signum)
{
  (void)(signum);
    running = 0;
}
static void setup_handlers(void)
{
    struct sigaction sa;
      sa.sa_handler = ctrl_c_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}
void ledprep(ws2811_t *ws2811, int led_count)
{
  ws2811->channel[0].count = led_count;
}

// ***************************************************************************************



// ***************************************************************************************
// MAIN PROGRAM
// ***************************************************************************************

// ---------------------------------------------------------------------------------------

void shutdown()
{
  // Shutdown the LED strip routine.
  ws2811_fini(&ledstring);
}

// ---------------------------------------------------------------------------------------
void setup()
{
  // Keeping this for now to remind me of what I haven't implementd, from the preport,
  //  yet.
}

// ---------------------------------------------------------------------------------------
// Global function for Main Loop Threads
// By passing the global variable, difficult to work with, ledstring to the, just as
//  difficult to work with, ws2811_render routine, all led and values will be transmitted
//  to the lights on a seperate thread.
void proc_render_thread()
{
  //int ret = 0;  // contains fail or pass status of the render routine.
  //ret = ws2811_render(&ledstring);  // Send values of ledstring to hardware.
  ws2811_render(&ledstring);  // Send values of ledstring to hardware.
}

// ---------------------------------------------------------------------------------------
// MAIN LOOP
int loop_2(bool TTY_Only)
//  Main Loop:
//    Events and Light Path animations should only be called when their time is up.

// This was once the main() program.  Ever since we abandoned global variable, the setup
//  portion of this program has become convoluted.  It could use a good reorganize and
//  maybe a way to get some of this stuff out, subroutine, much of these routines.
{
  using namespace std;

  // ---------------------------------------------------------------------------------------
  // Is_Ready varibles for main loop.

  TIMED_IS_READY  shutdown_procedure_delay;     // Delay for the hardware switches.

  TIMED_IS_READY  input_from_switches;    // Delay for the hardware switches.
  TIMED_IS_READY  input_from_user;        // Delay for the input from mouse and keyboard.
  TIMED_IS_READY  display;                // Delay for displaying information on the console.
  TIMED_IS_READY  comms_auto_timer;       // Delay for communicating with Automobile and GPS
                                          //  serial comms.
  TIMED_IS_READY  comms_gps_timer;        // Delay for communicating with Automobile and GPS
                                          //  serial comms.
  TIMED_IS_READY  compass_timer;          // Delay for communicating with compass
                                          // serial comms.

  TIMED_IS_READY  save_running_state_timer;          // Delay for communicating with compass
                                          // serial comms.

  // Define System Data and Console
  int return_code = 0;
  system_data sdSystem;
  sdSystem.TTY_ONLY = TTY_Only;

  // Control for threads.
  //THREAD_COMMAND.create();
  sdSystem.THREAD_RENDER.create(get_frame_interval(sdSystem.CONFIG.iFRAMES_PER_SECOND));
  sdSystem.THREAD_BACKUP_CAMERA.create(30); // 30 fps camera

  // Load Color System
  //sdSystem.PANEL_CONTROL.COLOR_SELECT.init_and_set_intensity(sdSystem.PROGRAM_TIME.current_frame_time(), 1.0f);
  sdSystem.PANEL_CONTROL.color_set_initial_values(true, 1.00f, 1.00f);
  sdSystem.PANEL_CONTROL.color_start(sdSystem.PROGRAM_TIME.current_frame_time());

  // Load Windows or Console
  SCREEN4 cons_2;

  // Check Directories
  if (sdSystem.FILE_NAMES.assign(sdSystem.SCREEN_COMMS) == false)
  {
    // not fully thought out
    sdSystem.SCREEN_COMMS.printw("Error while checking or creating directories.\n");
  }
  else
  {
    sdSystem.SCREEN_COMMS.printw("Succesful directory check or create.\n");
  }

  // Switch Lights On
  sdSystem.Lights_On.set(true);

  // Initialize wiring pi
  //int intRet = wiringPiSetup();
  wiringPiSetup();

  // Set is_ready variables
  input_from_switches.set(20);
  input_from_user.set(65);
  display.set(DEF_FRAME_RATE_DELAY);

  // System LogFile Variables
  FILE_WATCH watcher_daemon_log;
  watcher_daemon_log.start(sdSystem.FILE_NAMES.DEAMON_LOG);

  // ---------------------------------------------------------------------------------------

  // System LogFile Variables
  FILE_WATCH watcher_aircraft_json;
  watcher_aircraft_json.PROP.READ_FROM_BEGINNING = true;
  watcher_aircraft_json.PROP.WATCH_SIZE_CHANGE = false;
  watcher_aircraft_json.PROP.WATCH_TIME_CHANGE = true;
  watcher_aircraft_json.start(sdSystem.FILE_NAMES.AIRCRAFT_FA_FILE);

  // Disposable Variables
  int count  = 0;

  // ---------------------------------------------------------------------------------------
  comms_auto_timer.set(20);

  // Can Bus Comm Port Setup
  sdSystem.COMMS_AUTO.PROPS.PORT = sdSystem.FILE_NAMES.CAN_BUS_DEVICE_FILE;

  sdSystem.COMMS_AUTO.PROPS.AUTOSTART = COMMS_AUTOSTART;

  sdSystem.COMMS_AUTO.PROPS.BAUD_RATE = COMMS_BAUD;
  sdSystem.COMMS_AUTO.PROPS.BIT_COUNT = COMMS_BIT_PARITY;
  sdSystem.COMMS_AUTO.PROPS.PARITY = COMMS_BIT_PARITY;
  sdSystem.COMMS_AUTO.PROPS.STOP_BITS = COMMS_STOP_BITS;
  sdSystem.COMMS_AUTO.PROPS.HARDWARE_FLOW_CONTROL = COMMS_HARDWARE_FLOW_CONTROL;
  sdSystem.COMMS_AUTO.PROPS.DISABLE_CANONICAL_MODE = COMMS_DISABLE_CANONICAL_MODE;
  sdSystem.COMMS_AUTO.PROPS.XONXOFF = COMMS_XONXOFF;

  sdSystem.COMMS_AUTO.PROPS.SAVE_LOG_FILENAME = sdSystem.FILE_NAMES.CAN_BUS_LOG_FILE;
  // will be appeded with date and time.

  sdSystem.COMMS_AUTO.PROPS.RECEIVE_TEST_DATA = TEST_DATA_CAN_BUS;
  sdSystem.COMMS_AUTO.PROPS.TEST_DATA_FILENAME = sdSystem.FILE_NAMES.CAN_BUS_TEST_FILE;
  sdSystem.COMMS_AUTO.PROPS.TEST_DATA_MS_DELAY_BETWEEN_MESSAGES = 15;
  sdSystem.COMMS_AUTO.PROPS.TEST_DATA_MS_MESSAGE_COUNT_PER_ITTERATION = 5;

  sdSystem.COMMS_AUTO.PROPS.FLASH_DATA_RECORDER_ACTIVE = COMMS_FLASH_DATA_RECORDER_ACTIVE;

  // Prep Automobile History File
  sdSystem.CAR_INFO.PROPS.STATUS_HISTORY_HISTORY_DIR = sdSystem.FILE_NAMES.LOGS_CAN_BUS_HISTORY_SUB_DIR;

  // Prep Automobile Send Messages
  sdSystem.CAR_INFO.set_default_request_pid_list();

  // ---------------------------------------------------------------------------------------
  // GPS Compass Timer Set
  compass_timer.set( COMMS_COMPASS_POLLING_RATE_MS );

  // ---------------------------------------------------------------------------------------
  // GPS Comm Port Setup
  comms_gps_timer.set( 100 );

  sdSystem.COMMS_GPS.PROPS.PORT = sdSystem.FILE_NAMES.GPS_DEVICE_FILE;

  sdSystem.COMMS_GPS.PROPS.AUTOSTART = COMMS_AUTOSTART_GPS;

  sdSystem.COMMS_GPS.PROPS.BAUD_RATE = COMMS_BAUD_GPS;
  sdSystem.COMMS_GPS.PROPS.BAUD_RATE_CHANGE_TO = COMMS_BAUD_BAUD_RATE_CHANGE_TO_GPS;
  sdSystem.COMMS_GPS.PROPS.BAUD_RATE_TARGET = COMMS_BAUD_TARGET_GPS;
  sdSystem.COMMS_GPS.PROPS.BIT_COUNT = COMMS_BIT_PARITY_GPS;
  sdSystem.COMMS_GPS.PROPS.PARITY = COMMS_BIT_PARITY_GPS;
  sdSystem.COMMS_GPS.PROPS.STOP_BITS = COMMS_STOP_BITS_GPS;
  sdSystem.COMMS_GPS.PROPS.HARDWARE_FLOW_CONTROL = COMMS_HARDWARE_FLOW_CONTROL_GPS;
  sdSystem.COMMS_GPS.PROPS.DISABLE_CANONICAL_MODE = COMMS_DISABLE_CANONICAL_MODE_GPS;
  sdSystem.COMMS_GPS.PROPS.XONXOFF = COMMS_XONXOFF_GPS;

  sdSystem.COMMS_GPS.PROPS.CONTINUOUS_DATA = CONTINUOUS_DATA_GPS;

  sdSystem.COMMS_GPS.PROPS.SAVE_LOG_FILENAME = sdSystem.FILE_NAMES.GPS_LOG_FILE;

  sdSystem.COMMS_GPS.PROPS.RECEIVE_TEST_DATA = TEST_DATA_GPS;
  sdSystem.COMMS_GPS.PROPS.TEST_DATA_FILENAME = sdSystem.FILE_NAMES.GPS_TEST_FILE;
  //sdSystem.COMMS_GPS.PROPS.TEST_DATA_MS_DELAY_BETWEEN_MESSAGES = 1000 / 13; // 13 messages per second
  sdSystem.COMMS_GPS.PROPS.TEST_DATA_MS_DELAY_BETWEEN_MESSAGES = 1000 / 10 / 13; // playback times 10

  sdSystem.COMMS_GPS.device_baud_rate_change_to_target_string(
    sdSystem.GPS_SYSTEM.device_change_baud_rate_string(COMMS_BAUD_TARGET_GPS));

  // ---------------------------------------------------------------------------------------
  // Compass Comm Port Setup
  sdSystem.COMMS_COMPASS.PROPS.PORT = sdSystem.FILE_NAMES.COMPASS_DEVICE_FILE;
  sdSystem.COMMS_COMPASS.PROPS.I2C_ID = COMMS_ID_COMPASS;

  sdSystem.COMMS_COMPASS.PROPS.AUTOSTART = COMMS_AUTOSTART_COMPASS;
  sdSystem.COMMS_COMPASS.PROPS.CONTINUOUS_DATA = CONTINUOUS_DATA_GPS;

  sdSystem.COMMS_COMPASS.PROPS.OFFSET_HISTORY_FILE_NAME = sdSystem.FILE_NAMES.COMPASS_OFFSET_HISTORY_FILE;

  // ---------------------------------------------------------------------------------------
  // The Advertisements
  sdSystem.DIRECTORY_ADVERTISEMENTS = sdSystem.FILE_NAMES.LOGS_ADVERTS_DIR;

  // ---------------------------------------------------------------------------------------
  // Initialize the console
  cons_2.create(sdSystem);    // Prepare console.

  sdSystem.SCREEN_COMMS.DEBUG_STATUS.DOOR.resize(4);

  // Print Start Info
  sdSystem.SCREEN_COMMS.printw("Console Initialized ...  OK");
  sdSystem.SCREEN_COMMS.printw("");
  sdSystem.SCREEN_COMMS.printw("RasFLED");
  sdSystem.SCREEN_COMMS.printw("  'X'    - Exit");
  sdSystem.SCREEN_COMMS.printw("  'help' - Command List)");
  sdSystem.SCREEN_COMMS.printw("");

  // ---------------------------------------------------------------------------------------
  // System Init

  // ---------------------------------------------------------------------------------------
  // Initialize Camera
  
  // Assign Camera C0ordinator
  CAMERA_SYSTEM_COORDINATOR CAMERA_COORDINATOR;

  // Assign Properties and Controls

  sdSystem.CAMERA_BACKUP.PROPS.CAMERA_DEVICE_NAME     = sdSystem.FILE_NAMES.CAMERA_BACKUP;
  sdSystem.CAMERA_BACKUP.PROPS.CAMERA_DEVICE_ID       = sdSystem.FILE_NAMES.CAMERA_BACKUP_ID;

  sdSystem.CAMERA_BACKUP.PROPS.CAMERA_DIRECTORY       = sdSystem.FILE_NAMES.CAMERA_DIR;
  sdSystem.CAMERA_BACKUP.PROPS.CAMERA_SETTINGS_DIR    = sdSystem.FILE_NAMES.CAMERA_SETTINGS_DIR;

  sdSystem.CAMERA_BACKUP.PROPS.CAMERA_TEST_FILE_NAME  = sdSystem.FILE_NAMES.CAMERA_TEST_IMAGE_FILENAME;

  // Set Camera Screen Properties
  //sdSystem.CAMERA_BACKUP.PROPS.WIDTH = 640;
  //sdSystem.CAMERA_BACKUP.PROPS.HEIGHT = 480;

  //sdSystem.CAMERA_BACKUP.PROPS.WIDTH = 1280;
  //sdSystem.CAMERA_BACKUP.PROPS.HEIGHT = 720;
  
  sdSystem.CAMERA_BACKUP.PROPS.WIDTH = 800;
  sdSystem.CAMERA_BACKUP.PROPS.HEIGHT = 600;
  
  // Flip horizontal and vertical
  sdSystem.CAMERA_BACKUP.PROPS.FLIP_HORIZONTAL  = false;
  sdSystem.CAMERA_BACKUP.PROPS.FLIP_VERTICAL    = true;

  // Get list of controls to print to screen.
  sdSystem.SCREEN_COMMS.printw("Initializing Camera ...");
  sdSystem.CAMERA_BACKUP.list_controls();
  sdSystem.SCREEN_COMMS.printw(sdSystem.CAMERA_BACKUP.INFORMATION_COMMAND_LIST);

  sdSystem.CAMERA_BACKUP.create();
  sdSystem.SCREEN_COMMS.printw(sdSystem.CAMERA_BACKUP.INFORMATION);

  // ---------------------------------------------------------------------------------------
  // Initialize Alert System
  sdSystem.SCREEN_COMMS.printw("Initializing Alert System ...");

  // Prepare Alerts
  //sdSystem.ALERTS.PROP.SWITCH_COUNT = sdSystem.CONFIG.iNUM_SWITCHES;
  //sdSystem.ALERTS.create();
  sdSystem.SCREEN_COMMS.printw("Initializing Timer ...");
  //FledTime tmeFled;

  // Create Alerts
  ALERT_SYSTEM_COORDINATOR  ALERTS_COORDINATOR;
  ALERTS_COORDINATOR.create(sdSystem.ALL_ALERTS);

  // Sleeping Loop Variables
  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.

  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    sdSystem.SCREEN_COMMS.printw("");
    sdSystem.SCREEN_COMMS.printw("Adjusting Program Clock");
    sdSystem.SCREEN_COMMS.printw("");

    sdSystem.ALL_ALERTS.add_generic_alert("PROGRAM CLOCK\nAdjusting Program Clock", "");

    sdSystem.PROGRAM_TIME.clear_error();
  }

  // ---------------------------------------------------------------------------------------
  // Load system configuration and set data

  sdSystem.SCREEN_COMMS.printw("Loading Configuration ...");

  // Set Running Color to white.
  sdSystem.init_running_color_list();
  sdSystem.set_running_color(CRGB(32,32,32),"White");

  // File System
  //string Working_Directory = FILES_DIRECTORY;
  //string Configuration_Files_JSON = FILES_CONFIGURATION;
  //string Animations_Library_JSON = FILES_ANIMATIONS;
  //check_create_working_dir(FILES_DIRECTORY);

  //  -----
  // Create Filenames as Variables
  string Running_State_Filename = sdSystem.FILE_NAMES.RUNNING_STATE_FILE;

  // Loading Configuration from files
  // yes, it resaves the file.  as is for now.

  if (load_json_configuration(sdSystem, sdSystem.FILE_NAMES.CONFIGURATION_FILE) == true)
  {
    sdSystem.SCREEN_COMMS.printw("  Configuration file loaded.");
  }
  else
  {
    sdSystem.SCREEN_COMMS.printw("  Configuration file not loaded.  Generating Working Configuration File.");
    //sdSystem.ALERTS.add_generic_alert("Configuration file not loaded.  Generating Working Configuration File.");

    if (save_json_configuration(sdSystem, sdSystem.FILE_NAMES.CONFIGURATION_FILE) == true)
    {
      sdSystem.SCREEN_COMMS.printw("    Configuration file created.");
      //sdSystem.ALERTS.add_generic_alert("Configuration file created.");
    }
    else
    {
      sdSystem.SCREEN_COMMS.printw("    Configuration file not created.");
      //sdSystem.ALERTS.add_generic_alert("Configuration file not created.");
    }
  }

  // Loading Running State
  sdSystem.SCREEN_COMMS.printw("  Loading running state ...");
  // yes, it resaves the file.  as is for now.
  if (load_saved_running_state_json(sdSystem.SCREEN_COMMS, sdSystem, Running_State_Filename) != true)
  {
    sdSystem.SCREEN_COMMS.printw("    Running state file not loaded.");
    //sdSystem.ALERTS.add_generic_alert("Running state file not loaded.");
  }
  else
  {
    sdSystem.SCREEN_COMMS.printw("    Running state file loaded.");

    // Simple screen void color set
    if (sdSystem.running_color_list.strRunningColor == "White")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_WHITE;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_WHITE).SIMPLE_RGB.brightness(0.25f), "White");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_WHITE);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Red")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_RED;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_RED).SIMPLE_RGB.brightness(0.25f), "Red");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_RED);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Green")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_GREEN;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_GREEN).SIMPLE_RGB.brightness(0.25f), "Green");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_GREEN);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Blue")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_BLUE;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_BLUE).SIMPLE_RGB.brightness(0.25f), "Blue");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_BLUE);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Purple")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_PURPLE;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_PURPLE).SIMPLE_RGB.brightness(0.25f), "Purple");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_PURPLE);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Yellow")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_YELLOW;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_YELLOW).SIMPLE_RGB.brightness(0.25f), "Yellow");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_YELLOW);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Cyan")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_CYAN;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_CYAN).SIMPLE_RGB.brightness(0.25f), "Cyan");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_CYAN);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Orange")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_ORANGE;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_ORANGE).SIMPLE_RGB.brightness(0.25f), "Orange");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_ORANGE);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Black" || sdSystem.running_color_list.strRunningColor == "Monochrome")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_MONOCHROME;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_MONOCHROME).SIMPLE_RGB.brightness(0.25f), "Monochrome");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_MONOCHROME);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Grey")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_GREY;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_GREY).SIMPLE_RGB.brightness(0.25f), "Grey");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_GREY);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Pink")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_PINK;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_PINK).SIMPLE_RGB.brightness(0.25f), "Pink");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_PINK);
    }
    else if (sdSystem.running_color_list.strRunningColor == "Temperature")
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = true;
      sdSystem.RUNNING_COLOR = RAS_CUSTOM;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_CUSTOM).SIMPLE_RGB.brightness(0.25f), "Temperature");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_CUSTOM);
    }
    else
    {
      sdSystem.RUNNING_COLOR_TEMPERATURE = false;
      sdSystem.RUNNING_COLOR = RAS_RED;
      sdSystem.set_running_color(sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_RED).SIMPLE_RGB.brightness(0.25f), "Red");
      sdSystem.PANEL_CONTROL.COLOR_SELECT.void_color_set(sdSystem.PROGRAM_TIME.current_frame_time(), RAS_RED);
    }

    sdSystem.CUSTOM_COLOR = sdSystem.PANEL_CONTROL.COLOR_SELECT.pure_color(sdSystem.RUNNING_COLOR).SIMPLE_RGB.brightness(0.25f);
  }

  // Loading Animations Library.
  ANIMATION_HANDLER animations;

  animations.create_events(sdSystem);

  if (animations.load_collections(sdSystem.FILE_NAMES.ANIMATIONS_FILE) == true)
  {
    sdSystem.SCREEN_COMMS.printw("  Animations file loaded.");
  }
  else
  {
    sdSystem.SCREEN_COMMS.printw("    Animations file not loaded.");
    //sdSystem.ALERTS.add_generic_alert("Animations file not loaded.");
  }

  // ---------------------------------------------------------------------------------------
  // LED Library Vars and Init

  sdSystem.WS2811_ENABLED = DEF_WS2811_ENABLED;
  int led_count = sdSystem.CONFIG.LED_MAIN.at(0).led_count();
  if (sdSystem.WS2811_ENABLED)
  {
    sdSystem.SCREEN_COMMS.printw("Initializing LEDS ...");

    ledstring.freq = TARGET_FREQ;
    ledstring.dmanum = DMA;
    ledstring.channel[0].gpionum = GPIO_PIN;
    ledstring.channel[0].count = led_count;
    ledstring.channel[0].brightness = 255;
    ledstring.channel[0].invert = 0;
    ledstring.channel[0].strip_type = STRIP_TYPE;

    ws2811_return_t ret;
    ledprep(&ledstring, led_count);
    matrix = (int*)malloc(sizeof(ws2811_led_t) * led_count);
    setup_handlers();
    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
      sdSystem.SCREEN_COMMS.printw("");
      sdSystem.SCREEN_COMMS.printw("FAILURE:");
      //std::cerr << "ws2811_init failed: " << ws2811_get_return_t_str(ret) << std::endl;
      string return_description = ws2811_get_return_t_str(ret);
      sdSystem.SCREEN_COMMS.printw("     ws2811_init failed: " + return_description);
      sdSystem.SCREEN_COMMS.printw("");

      return_code = (int)ret;

      sdSystem.WS2811_ENABLED = false;
    }
    else
    {
      sdSystem.SCREEN_COMMS.printw("  WS2811 INIT SUCCESS");
      sdSystem.SCREEN_COMMS.printw("  LED count: " + to_string(led_count));
    }
  }

  // ---------------------------------------------------------------------------------------
  // Automobile Handler

  AUTOMOBILE_HANDLER automobile_handler;

  // ---------------------------------------------------------------------------------------
  // Define the Supid Random Numbers
  sdSystem.SCREEN_COMMS.printw("Initializing Random Number Generator ...");
  stupid_random sRND;
  // Initialize the Stupid Random Numbers
  sRND.set();

  // ---------------------------------------------------------------------------------------
  // Light Strip Event System
  sdSystem.SCREEN_COMMS.printw("Initializing Event System ...");

  // count number of strips
  // Count the amount of events needed
  count = 0;
  for(int x=0; x<sdSystem.CONFIG.LED_MAIN.at(0).g_size(); x++)
  {
    for(int y=0; y<sdSystem.CONFIG.LED_MAIN.at(0).s_size(x); y++)
    {
      count++;
    }
  }
  sdSystem.CONFIG.iNUM_CHANNELS = count;

  // Set a few variables in sdSystem for the console display,
  sdSystem.init();

  // -------------------------------------------------------------------------------------
  // Aditional DEBUG line that could only be set after the channels were created.

  //cons.keywatch.set((int)KEYLEDDRCYCL,sdSystem.CONFIG.iNUM_CHANNELS + 1);  // Test Doors

  // ---------------------------------------------------------------------------------------

  // Define Door Sensors.
  sdSystem.SCREEN_COMMS.printw("Initializing Hardware Sensors ...");
  for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
  {
    pinMode(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin, INPUT);
    pullUpDnControl(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin, PUD_UP);
  }

  // -------------------------------------------------------------------------------------
  // Door Sensor
  sdSystem.SCREEN_COMMS.printw("Initializing Hardware Sensor Interface ...");

  // Initialize Switches
  hardware_monitor tmpSwitch;
  tmpSwitch.set(true, sdSystem.PROGRAM_TIME.current_frame_time(), DOOR_SWITCH_LEEWAY_TIME, true);
  for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
  {
    sdSystem.CONFIG.vhwDOORS.push_back(tmpSwitch);
  }

  // -------------------------------------------------------------------------------------
  // Map load and initialize
  sdSystem.SCREEN_COMMS.printw("Initializing Map ...");

  if (TEST_DATA_GPS)
  {
    sdSystem.MAP_SYSTEM.PROPS.SAVE_TRACK_TIMER = 60 * 1000;     // 60 sec
  }
  else
  {
    sdSystem.MAP_SYSTEM.PROPS.SAVE_TRACK_TIMER = 9 * 60 * 1000; // 9 min
  }

  sdSystem.MAP_SYSTEM.PROPS.FILENAME_GENERICS_MAP = sdSystem.FILE_NAMES.LOGS_MAPS_GENERICS_JSON;
  sdSystem.MAP_SYSTEM.PROPS.FILENAME_AIRPORTS_MAP = sdSystem.FILE_NAMES.LOGS_MAPS_AIRPORTS_JSON;
  sdSystem.MAP_SYSTEM.PROPS.FILENAME_REGIONS_MAP = sdSystem.FILE_NAMES.LOGS_MAPS_REGIONS_JSON;
  sdSystem.MAP_SYSTEM.PROPS.FILENAME_ROADS_MAP = sdSystem.FILE_NAMES.LOGS_MAPS_ROADS_JSON;
  sdSystem.MAP_SYSTEM.PROPS.TRACK_HISTORY_FOLDER = sdSystem.FILE_NAMES.TRACK_ARCHIVE_DIR;
  
  sdSystem.MAP_SYSTEM.create();
  sdSystem.SCREEN_COMMS.printw(sdSystem.MAP_SYSTEM.INFORMATION);

  // Load Track into GPS_System
  sdSystem.MAP_SYSTEM.PROPS.CURRENT_TRACK_FILENAME = sdSystem.FILE_NAMES.TRACK_CURRENT_JSON;
  sdSystem.MAP_SYSTEM.load_track(sdSystem.SCREEN_COMMS, sdSystem.GPS_SYSTEM.current_time_error());

  // -------------------------------------------------------------------------------------
  // Initialize Audio Filenames
  sdSystem.SCREEN_COMMS.printw("Initializing Audio ...");
  sdSystem.SOUND_SYSTEM.create(sdSystem.FILE_NAMES.LOGS_AUDIO_DIR);

  // -------------------------------------------------------------------------------------

  // Start Power On Animation
  process_power_animation(sdSystem, sdSystem.PROGRAM_TIME.now(), animations, CRGB(0, 0, 25));

  // -------------------------------------------------------------------------------------

  // Set initial save_running_state_timer 15 seconds out for first save
  save_running_state_timer.set(sdSystem.PROGRAM_TIME.now(), 15 * 1000);

  // -------------------------------------------------------------------------------------

  // Create Terminal Thread
  sdSystem.TERMINAL_THREAD.create();

  // ---------------------------------------------------------------------------------------
  //  Repeating Sleeping Loop until eXit is triggered.
  // ---------------------------------------------------------------------------------------

  sdSystem.SCREEN_COMMS.printw("Starting System ...");

  // **************************************************************************************
  // **************************************************************************************
  // **************************************************************************************

  // MAIN LOOP START
  //while(sdSystem.SCREEN_COMMS.WINDOW_CLOSE == false)
  while(sdSystem.PANEL_CONTROL.shutdown_procedure_step < 100)
  {
    // ---------------------------------------------------------------------------------------
    // Thread Management

    // Close all completed and active threads after sleep cycle is complete.
    sdSystem.THREAD_RENDER.check_for_completition();

    // Temporary solution
    {    
      if (sdSystem.THREAD_BACKUP_CAMERA.check_for_completition())
      {
        sdSystem.CAMERA_BACKUP.process_frame();
      }
    }

    // ---------------------------------------------------------------------------------------
    // --- Prpare the Loop ---

    //  Get current time.  This will be our timeframe to work in.
    if (sdSystem.PROGRAM_TIME.setframetime() == true)
    {
      sdSystem.SCREEN_COMMS.printw("");
      sdSystem.SCREEN_COMMS.printw("ALERT: PROGRAM TIME STREAM INTURPTED OR CORRUPT");
      sdSystem.SCREEN_COMMS.printw("           DIFFERANCE: "  + to_string(sdSystem.PROGRAM_TIME.error()));
      sdSystem.SCREEN_COMMS.printw("");

      sdSystem.ALL_ALERTS.add_generic_alert("ALERT", "ALERT: PROGRAM TIME\nSTREAM INTURPTED OR CORRUPT\nDIFFERANCE: " +
                                          to_string(sdSystem.PROGRAM_TIME.error()));

      sdSystem.PROGRAM_TIME.clear_error();
    }

    //  Only update the hardware when changes have been detected.
    //    This vabiable will be checked at the end of the loop.  If nothing was updated,
    //    the loop will just walk on past any hardware updates that would otherwise be
    //    sent.

    // ---------------------------------------------------------------------------------------

    // Signal to RasCAM to get data on next read comm cycle.
    //  Screen draw cycle may be to much of a delay to handle?
    //  A half milsec delay after the send in the req is an alt.
    //  Never comment this out or the system will never sleep
    if (comms_auto_timer.is_ready_no_reset(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.COMMS_AUTO.request_to_send();
    }

    // ---------------------------------------------------------------------------------------
    // --- Read Switchs ---

    // Are switches ready -----------------
    //  Never comment this out or the system will never sleep
    if (input_from_switches.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // Read values of switches
      // Read information from car system. If not available, then get door info
      //  from original system as redundancy.

      if (sdSystem.CAR_INFO.active() == true && sdSystem.CAR_INFO.STATUS.DOORS.available() == true)
      {
        // Not sure how to tie this in with the load configuration file yet.
        sdSystem.CONFIG.vSWITCH_PIN_MAP[0].value = sdSystem.CAR_INFO.STATUS.DOORS.lb_door_open();
        sdSystem.CONFIG.vSWITCH_PIN_MAP[1].value = sdSystem.CAR_INFO.STATUS.DOORS.lf_door_open();
        sdSystem.CONFIG.vSWITCH_PIN_MAP[2].value = sdSystem.CAR_INFO.STATUS.DOORS.rb_door_open();
        sdSystem.CONFIG.vSWITCH_PIN_MAP[3].value = sdSystem.CAR_INFO.STATUS.DOORS.rf_door_open();

        if (sdSystem.CONFIG.vSWITCH_PIN_MAP.size() >= 4 && sdSystem.CONFIG.vSWITCH_PIN_MAP.size() <= 6)
        {
          sdSystem.CONFIG.vSWITCH_PIN_MAP[4].value = sdSystem.CAR_INFO.STATUS.DOORS.hatchback_door_open();
          sdSystem.CONFIG.vSWITCH_PIN_MAP[5].value = sdSystem.CAR_INFO.STATUS.DOORS.hood_door_open();
        }
      }
      else
      // Redundant system in case auto door switchs arent avail.
      {
        TRUTH_CATCH ret_changed;
        for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
        {
          sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).value = !digitalRead(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin);    // Default at fail state is closed.
          ret_changed.catch_truth(sdSystem.CAR_INFO.STATUS.DOORS.store_from_alt_source(x, sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).value));

          if (ret_changed.has_truth() == true)
          {
            sdSystem.CAR_INFO.STATUS.DOORS.set_source_availability(true);
            sdSystem.CAR_INFO.CHANGED = true;
          }
        }
      }

      // Override the digital pins if in debugging mode.
      //if(cons.keywatch.getnoreset(KEYDEBUG) == 1)
      if(sdSystem.SCREEN_COMMS.DEBUG_STATUS.DEBUG == true)
      {
        // Toggle on and off the door sensors with keyboard.
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(0).value = sdSystem.SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(1).value = sdSystem.SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(2).value = sdSystem.SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(3).value = sdSystem.SCREEN_COMMS.DEBUG_STATUS.DOOR[3];

        TRUTH_CATCH ret_changed;
        for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
        {
          ret_changed.catch_truth(sdSystem.CAR_INFO.STATUS.DOORS.store_from_alt_source(x, sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).value));
        }

        if (ret_changed.has_truth() == true)
        {
          sdSystem.CAR_INFO.STATUS.DOORS.set_source_availability(true);
          sdSystem.CAR_INFO.CHANGED = true;
        }
      }

      // Check the doors and start or end all animations
      v_DoorMonitorAndAnimationControlModule2(sdSystem.SCREEN_COMMS, sdSystem, animations, sdSystem.PROGRAM_TIME.current_frame_time());
    }

    // Read light switchs and set day on or day off modes.
    sdSystem.Day_On_With_Override.set(sdSystem.CAR_INFO.active() == true &&
                                sdSystem.CAR_INFO.STATUS.INDICATORS.available() == true,
                                sdSystem.CAR_INFO.STATUS.INDICATORS.val_light_switch() == false, sdSystem.Day_On);

    // ---------------------------------------------------------------------------------------
    // --- Check and Execute Timed Events That Are Ready ---

    // Is Events and Render ready -----------------
    //  Never comment this out or the system will never sleep
    if(sdSystem.THREAD_RENDER.check_to_run_routine_on_thread(sdSystem.PROGRAM_TIME.current_frame_time()))
    {
      // MOVE RENAME ELIMINATE ??? !!!
      bool booUpdate = false;

      //  Run ALL GLOBAL Timed Events
      animations.process_events(sdSystem, sdSystem.PROGRAM_TIME.current_frame_time());

      for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
      {
        for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
        {
          int channel = sdSystem.CONFIG.LED_MAIN[0].vLED_GROUPS[group].vLED_STRIPS[strip].intCHANNEL;

          sdSystem.CONFIG.LED_MAIN[0].vLED_GROUPS[group].vLED_STRIPS[strip].booARRAY_UPDATED
            = animations.EVENTS[channel].execute2(sdSystem, sRND,
                sdSystem.CONFIG.LED_MAIN[0].vLED_GROUPS[group].vLED_STRIPS[strip].crgbARRAY,
                sdSystem.PROGRAM_TIME.current_frame_time());
        }
      }

      // ---------------------------------------------------------------------------------------
      // Render all the LEDs if changes have been made.

      // --- Execute LED Hardware Changes If Anything Was Updated ---
      //  For now we are working with just one big LED strip.  So, just check to see if anything
      //    changed.  Then, Redraw the entire strip.

      // Calculate LED values and update if enabled.
      if (sdSystem.Lights_On.value() == true)
      {
        // Update?
        for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
        {
          for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
          {
            if (sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).booARRAY_UPDATED == true)
            {
              booUpdate = true;
            }
          }
        }
      }
      else // lights are off, blank values if neccessary.
      {
        // make sure the thread is free, otherwise the blank will be thrown away.
        if (sdSystem.THREAD_RENDER.is_running() == false)
        {
          // only actively blank if the bounce of lights off was detected.
          if (sdSystem.Lights_On.bounce() == true)
          {
            // Clear Lights
            for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
            {
              for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
              {
                MatxixBlank(sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY,
                sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(),
                CRGB(0,0,0));
              }
            }

            booUpdate = true;
          }
        }
      }

      if (booUpdate == true)
      {
        int mcount = 0;
        // Copy the prepared Matrixes to the display matrix
        //if((cons.keywatch.getnoreset(KEYDEBUG) == 0) || (cons.keywatch.get(KEYLEDDRCYCL) == 0))
        {
          for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
          {
            for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
            {
              MatrixPrepare(
                    sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY,
                    sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(),
                    matrix, mcount);
            }
          }
        }

        //  Are the lights enable to display.
        //    Lights off will not turn the lights off and clear their values.
        //      Instead, transmitting those color are to the lights disabled.

        if (sdSystem.WS2811_ENABLED)
        {
          // LED Library Renderer -- Recommend: DON'T TOUCH
          matrix_render(led_count);

          // Create a seperate thread only to render the LEDs with the hardware.  This process
          //  is very intensive for the system and is only one way.  The render thread only needs
          //  to rejoin with the main program, at the end of the main loop, to signify its
          //  completion, so that the loop can restart and begin computing its values and colors
          //  again.
          // A render thread should not be created if no changes have been made to the led values.

          // Get timer for render thread.
          sdSystem.dblCOMMS_LED_RENDER_TIME.start_timer(sdSystem.PROGRAM_TIME.current_frame_time());

          // Be careful with this because it looks like black magic to me.
          sdSystem.THREAD_RENDER.start_render_thread([&]()
                        {  proc_render_thread();  });

          sdSystem.dblCOMMS_LED_RENDER_TIME.end_timer(sdSystem.PROGRAM_TIME.current_frame_time());
        }
      }
    } // Is Events and Render ready -----------------
    
    // ---------------------------------------------------------------------------------------

    // Check Camera Frame Grab on thread
    {   
      if (sdSystem.THREAD_BACKUP_CAMERA.check_to_run_routine_on_thread(sdSystem.PROGRAM_TIME.current_frame_time())) 
      {
        // Start the camera update on a separate thread.
        // This call is non-blocking, so the main loop can continue immediately.
        sdSystem.THREAD_BACKUP_CAMERA.start_render_thread([&]() 
                  {  sdSystem.CAMERA_BACKUP.update_frame();  });
      }
    }

    // ---------------------------------------------------------------------------------------
    // Now that we have done all the hard work, read hardware, computed, generated, displayed
    // all the lights, we will take the latter clock cycles to get keybord input and update
    // console with status and so on.

    // Is Keyboard or Mouse read ready -----------------
    //  Never comment this out or the system will never sleep
    if (input_from_user.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // Read Hardware Status before printing to screen.
      sdSystem.read_hardware_status(1000);

      // Read System log files
      while (watcher_daemon_log.line_avail() == true)
      {
        //cons.Screen.Log_Screen_TEXT_BOX.add_line(sdSystem.PROGRAM_TIME.current_frame_time(), watcher_daemon_log.get_next_line());
        cons_2.update_daemon_log(sdSystem, watcher_daemon_log.get_next_line() + "\n");
      }

      // Read ADS-B Aircraft JSON
      if (watcher_aircraft_json.changed() == true)
      {
        sdSystem.AIRCRAFT_COORD.process(sdSystem.PROGRAM_TIME.current_frame_time(), file_to_string(sdSystem.FILE_NAMES.AIRCRAFT_FA_FILE),
                                                          sdSystem.COMMAND_THREADS, sdSystem.SOUND_SYSTEM,
                                                          sdSystem.GPS_SYSTEM.active(sdSystem.PROGRAM_TIME.current_frame_time()),
                                                          sdSystem.GPS_SYSTEM.CURRENT_POSITION.LATITUDE, sdSystem.GPS_SYSTEM.CURRENT_POSITION.LONGITUDE);
      }

      processcommandlineinput(sdSystem, sdSystem.PROGRAM_TIME.current_frame_time(), animations);
      extraanimationdoorcheck2(sdSystem, sdSystem.PROGRAM_TIME.current_frame_time(), animations);

      // Also delayed, File maintenance.
      //if (sdSystem.booRunning_State_File_Dirty == true)
      if (save_running_state_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        // Set Last Known Good Position
        if (sdSystem.GPS_SYSTEM.active(sdSystem.PROGRAM_TIME.current_frame_time()))
        {
          if (sdSystem.GPS_SYSTEM.valid_position())
          {
            sdSystem.PANEL_CONTROL.PANELS.LAST_KNOWN_GOOD_POSITION.x = sdSystem.GPS_SYSTEM.CURRENT_POSITION.LATITUDE;
            sdSystem.PANEL_CONTROL.PANELS.LAST_KNOWN_GOOD_POSITION.y = sdSystem.GPS_SYSTEM.CURRENT_POSITION.LONGITUDE;
          }
        }

        // save running state every 30 seconds.
        save_running_state_timer.set(sdSystem.PROGRAM_TIME.current_frame_time(), 30 * 1000);
        save_running_state_json(sdSystem, Running_State_Filename);
      }
    }

    // ---------------------------------------------------------------------------------------
    // Comm Port Read
    // Automobile Data Process.

    if (comms_auto_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.dblCOMMS_AUTO_TRANSFER_TIME.start_timer(sdSystem.PROGRAM_TIME.now());

      // CAN_Bus Serial Communications
      if (sdSystem.COMMS_AUTO.cycle(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_AUTO.cycle_change() == 99)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == -1)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 0)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: NORMAL READ MODE");
          sdSystem.ALL_ALERTS.sound_alert(1);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 1)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALL_ALERTS.sound_alert(3);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 2)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: CONNECT START");
          sdSystem.ALL_ALERTS.sound_alert(2);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 3)
        {
          sdSystem.SCREEN_COMMS.printw("Automobile COMMS changed to: BAUD RATE CHECK");
        }
      }

      // ---------------------------------------------------------------------------------------
      // CAN Bus Process

      // Process info from comm port int automobile system.
      sdSystem.CAR_INFO.process(sdSystem.SCREEN_COMMS, sdSystem.COMMS_AUTO, sdSystem.PROGRAM_TIME.current_frame_time());

      // Process Automobile Lights
      automobile_handler.update_events(sdSystem, animations, sdSystem.PROGRAM_TIME.current_frame_time());

      sdSystem.dblCOMMS_AUTO_TRANSFER_TIME.end_timer(sdSystem.PROGRAM_TIME.now());
    }

    // ---------------------------------------------------------------------------------------
    // Comm Port Read
    // GPS Data Process.

    if (comms_gps_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.dblCOMMS_GPS_TRANSFER_TIME.start_timer(sdSystem.PROGRAM_TIME.now());

      // GPS Serial Communications
      if (sdSystem.COMMS_GPS.cycle(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_GPS.cycle_change() == 99)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == -1)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 0)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: NORMAL READ MODE");
          sdSystem.ALL_ALERTS.sound_alert(1);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 1)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALL_ALERTS.sound_alert(3);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 2)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: CONNECT START");
          sdSystem.ALL_ALERTS.sound_alert(2);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 3)
        {
          sdSystem.SCREEN_COMMS.printw("GPS COMMS changed to: BAUD RATE CHECK");
        }
      }

      // ---------------------------------------------------------------------------------------
      // GPS Process

      if (sdSystem.GPS_SYSTEM.process(sdSystem.SCREEN_COMMS, sdSystem.COMMS_GPS, sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        sdSystem.MAP_SYSTEM.update( sdSystem.SCREEN_COMMS, sdSystem.GPS_SYSTEM, 
                                    sdSystem.PROGRAM_TIME.current_frame_time());
      }

      sdSystem.dblCOMMS_GPS_TRANSFER_TIME.end_timer(sdSystem.PROGRAM_TIME.now());
    }

    // ---------------------------------------------------------------------------------------
    // Comm Port Read
    // Compass Data Process.

    //  Never comment this out or the system will never sleep
    if (compass_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.dblCOMMS_COMPASS_DATA_READ_TIME.start_timer(sdSystem.PROGRAM_TIME.now());

      // Compass Serial Communications
      if (sdSystem.COMMS_COMPASS.cycle(sdSystem.GPS_SYSTEM, sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_COMPASS.cycle_change() == 99)
        {
          sdSystem.SCREEN_COMMS.printw("COMPASS COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == -1)
        {
          sdSystem.SCREEN_COMMS.printw("COMPASS COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 0)
        {
          sdSystem.SCREEN_COMMS.printw("COMPASS COMMS changed to: NORMAL READ MODE");
          sdSystem.ALL_ALERTS.sound_alert(1);
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 1)
        {
          sdSystem.SCREEN_COMMS.printw("COMPASS COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALL_ALERTS.sound_alert(3);
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 2)
        {
          sdSystem.SCREEN_COMMS.printw("COMPASS COMMS changed to: CONNECT START");
          sdSystem.ALL_ALERTS.sound_alert(2);
        }
      }
      sdSystem.dblCOMMS_COMPASS_DATA_READ_TIME.end_timer(sdSystem.PROGRAM_TIME.now());
    }

    // ---------------------------------------------------------------------------------------
    // Process Alerts
    ALERTS_COORDINATOR.check_for_alerts(sdSystem, automobile_handler);

    // ---------------------------------------------------------------------------------------
    // Is display to console ready -----------------
    //  Never comment this out or the system will never sleep
    if (display.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // if condition no longer needed.
      {
        // Refresh console data storeage from main program. This will be a pass through buffer.
        // so the console will not have to access any real data.
        sdSystem.store_door_switch_states();

        store_event_counts(sdSystem, animations);

        // Automobile - Update all automobile Reference Data
        sdSystem.CAR_INFO.translate(sdSystem.DNFWTS, sdSystem.PROGRAM_TIME.current_frame_time());
        cons_2.update_automobile_gadgets(sdSystem);

        // Check camera handler to see if camera should be on.
        CAMERA_COORDINATOR.check(sdSystem);

        // Check DNFWTS
        if (sdSystem.DNFWTS.check_for_change())
        {
          if (sdSystem.DNFWTS.active())
          {
            sdSystem.Lights_On.set(false);
            sdSystem.SOUND_SYSTEM.set_mute(true);
          }
          else
          {
            sdSystem.Lights_On.set(true);
            sdSystem.SOUND_SYSTEM.set_mute(false);
          }
        }

        // Redraw the console screen with what the screen determines needs to be displayed.
        //cons.display(fsPlayer, sdSystem, sdSystem.PROGRAM_TIME.current_frame_time());
        sdSystem.dblSCREEN_RENDER_TIME.start_timer(sdSystem.PROGRAM_TIME.now());
        cons_2.draw(sdSystem, animations);
        sdSystem.dblSCREEN_RENDER_TIME.end_timer(sdSystem.PROGRAM_TIME.now());

      }

      // Alert system checks
      sdSystem.ALL_ALERTS.alert_list_clean(sdSystem.COMMAND_THREADS, sdSystem.SOUND_SYSTEM);

    } // Is display to console ready -----------------

    // Shutdown Procedures if started.
    if (sdSystem.SCREEN_COMMS.WINDOW_CLOSE || sdSystem.PANEL_CONTROL.shutdown_procedure_step > 0)
    {
      if (shutdown_procedure_delay.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        shutdown_procedure_delay.set(sdSystem.PROGRAM_TIME.current_frame_time(), 1000);

        //sdSystem.SCREEN_COMMS.printw("Shutdown Step " + to_string(sdSystem.PANEL_CONTROL.shutdown_procedure_step));

        if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 0)
        {
          // Panel close called in gui
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 2;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 1 ||
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 11|| 
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 21)
        {
          // Panel close called in interface
          sdSystem.SCREEN_COMMS.WINDOW_CLOSE = true;
          sdSystem.PANEL_CONTROL.shutdown_procedure_step++;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 2 ||
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 12|| 
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 22)
        {
          // Change to console
          sdSystem.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 0;
          sdSystem.PANEL_CONTROL.shutdown_procedure_step++;
          // Save any pending maps
          sdSystem.MAP_SYSTEM.close(sdSystem.SCREEN_COMMS);
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 3 ||
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 13 || 
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 23)
        {
          // Start Power Down Animation
          process_power_animation(sdSystem, sdSystem.PROGRAM_TIME.current_frame_time(), animations, CRGB(25, 0, 0));
          sdSystem.PANEL_CONTROL.shutdown_procedure_step++;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 4 ||
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 14 || 
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 24)
        {
          // Doing Nothing
          sdSystem.PANEL_CONTROL.shutdown_procedure_step++;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 5 ||
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 15 || 
                  sdSystem.PANEL_CONTROL.shutdown_procedure_step == 25)
        {
          // Turn off the lights
          sdSystem.SCREEN_COMMS.command_text_set(" lightsoff");
          sdSystem.PANEL_CONTROL.shutdown_procedure_step++;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 6)
        {
          // Set Progrgam Exit
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 100;
        }

        // Rasfled Reboot Called
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 10)
        {
          // Start Shutdown
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 12;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 16)
        {
          // Call Shutdown command.
          COMMANDS command;
          command.reboot();
          // Set Progrgam Exit
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 100;
        }

        // Rasfled Shutdown Called
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 20)
        {
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 22;
        }
        else if (sdSystem.PANEL_CONTROL.shutdown_procedure_step == 26)
        {
          // Call Reboot command.
          COMMANDS command;
          command.shutdown_now();
          // Set Progrgam Exit
          sdSystem.PANEL_CONTROL.shutdown_procedure_step = 100;
        }
      }
    }

    // Run external commands, if pending
    {
      sdSystem.COMMAND_THREADS.execute();
    }

    // ---------------------------------------------------------------------------------------
    // PRINT C_ERRORS to console
    //  Not Coded.

    // ---------------------------------------------------------------------------------------
    // Now that the complete cycle is over, we need figure out how much time is remaining in
    // the cycle and go to sleep for the appropriate amount of time.
    // Calculate times and sleep till next frame is ready.

    // Determine how long to sleep and then sleep by
    //  finding the earliest sleep wake time.
    // Make sure non of these are commented out, or the system will never sleep.
    sdSystem.PROGRAM_TIME.request_ready_time(input_from_switches.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.THREAD_RENDER.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.THREAD_BACKUP_CAMERA.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(input_from_user.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(display.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(comms_auto_timer.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(comms_gps_timer.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(compass_timer.get_ready_time());

    sdSystem.PROGRAM_TIME.sleep_till_next_frame();

  }// End MAIN CYCLE WHILE loop.

  // ---------------------------------------------------------------------------------------
  // If we are here, then we are closing the program.

  // Wait for threads to end before continuing to shutdown.
  sdSystem.THREAD_RENDER.wait_for_thread_to_finish();
  sdSystem.THREAD_BACKUP_CAMERA.wait_for_thread_to_finish();

  sdSystem.CAMERA_BACKUP.close_camera();

  // Shutdown RPI.
  if (sdSystem.WS2811_ENABLED)
  {
    shutdown();
  }

  // Shutdown Graphical Window
  if (TTY_Only == false)
  {
    cons_2.shutdown(sdSystem);
  }

  // close open files
  watcher_daemon_log.stop();

  if(sdSystem.booREBOOT == false)
  {
    // Just print we have ended the program.
    cout << endl << "RasFLED Loop ... Exit" << endl;
  }
  else
  {
    // Just print are restarting the program.
    cout << endl << "RasFLED Loop ... Rebooting" << endl;
    return_code = 9999;
  }

  return return_code;
}

// ---------------------------------------------------------------------------------------
// Main entry point for the RasFLED application
int main(int argc, char *argv[])
{
  // Initialize exit code to indicate a run state (1) or failure (0)
  int ret = 1;  // Run

  // Flag to determine whether to load a graphics window
  bool load_gfx_window = true;

  // Parse command-line arguments to configure the program's behavior
  for (int pos = 1; pos < argc; pos++)
  {
    // Check for help or usage options
    if (strcmp(argv[pos], "-gfx") == 0)
    {
      load_gfx_window = true;
    }
    // Check for terminal or graphics window loading options
    else if (strcmp(argv[pos], "-tty") == 0)
    {
      load_gfx_window = false;
    }
    else //if (strcmp(argv[pos], "-h") == 0 || strcmp(argv[pos], "--help") == 0)
    {
      ret = 2;
      cout << "\n-tty    :Load in Terminal" << endl;
      cout << "             Warning - Command line input not yet implemented." << endl;
      cout << "                 There is no way of safely exiting the program." << endl;
      cout << "-gfx      :Load Graphics Window (default)\n" << endl << endl;
      cout << "-h --help :Load Graphics Window (default)\n" << endl << endl;
    }
  }

  // Perform initial setup for the program
  setup();

  // Main loop that runs until a termination condition is met
  while (ret == 1)
  {
    cout << "RasFLED Start (Rev: " << Revision << ") ... " << endl;
    cout << "  (The program needs to be started with SUDO otherwise a segmentation" << endl;
    cout << "   fault will occur.  Admin access is needed for the IRQ access)" << endl;

    try
    {
      // Run the main application logic with graphics window loading enabled or disabled
      if (load_gfx_window == true)
      {
        ret = loop_2(false);
        cout << "Load Graphics Window" << endl;
      }
      else
      {
        ret = loop_2(true);  // To be phased out?
        cout << "Do Not Load Graphics Window" << endl;
      }
    }

    // --- Catch Specific Standard Exceptions First ---
    // Catches std::out_of_range exceptions (e.g., from vector/array bounds checks)
    catch (const std::out_of_range& e)
    {
      // Print a critical error message to std::cerr for immediate visibility
      std::cerr << "!!! CRITICAL ERROR (Out of Range) !!!" << std::endl;
      std::cerr << "Details: " << e.what() << std::endl; // e.what() provides the exception message
      std::cerr << "Program Status Code (before exception): " << ret << std::endl;
      ret = 0; // Set return code to indicate a failure exit

      // Pause the program to allow viewing the error message
      press_enter_to_continue();
    }
    // Catches std::invalid_argument exceptions (e.g., from bad function arguments)
    catch (const std::invalid_argument& e)
    {
      std::cerr << "!!! CRITICAL ERROR (Invalid Argument) !!!" << std::endl;
      std::cerr << "Details: " << e.what() << std::endl;
      std::cerr << "Program Status Code (before exception): " << ret << std::endl;
      ret = 0;

      // Pause the program to allow viewing the error message
      press_enter_to_continue();
    }
    // Catches std::bad_alloc exceptions (e.g., when memory allocation fails)
    catch (const std::bad_alloc& e)
    {
      std::cerr << "!!! FATAL ERROR (Memory Allocation Failed) !!!" << std::endl;
      std::cerr << "Details: " << e.what() << std::endl;
      std::cerr << "Consider increasing available memory or reducing usage." << std::endl;
      std::cerr << "Program Status Code (before exception): " << ret << std::endl;
      ret = 0;

      // Pause the program to allow viewing the error message
      press_enter_to_continue();
    }
    // --- Catch General Standard Exceptions ---
    // This catches any other exception derived from std::exception
    // (e.g., std::runtime_error, std::logic_error, if not caught by a more specific handler above)
    catch (const std::exception& e)
    {
      std::cerr << "!!! UNEXPECTED ERROR IN MAIN LOOP !!!" << std::endl;
      std::cerr << "Details: " << e.what() << std::endl;
      std::cerr << "Program Status Code (before exception): " << ret << std::endl;
      ret = 0;

      // Pause the program to allow viewing the error message
      press_enter_to_continue();
    }
    // --- Catch All Other Unforeseen Exceptions ---
    // This is the catch-all for any non-std::exception derived throws
    // (e.g., throw "string", throw 123) or if a custom exception class
    // does not inherit from std::exception.
    // It provides a last-resort safety net.
    catch (...)
    {
      std::cerr << "!!! UNKNOWN CRITICAL ERROR IN MAIN LOOP !!!" << std::endl;
      std::cerr << "An exception of an unknown type occurred. This indicates a serious issue." << std::endl;
      std::cerr << "Program Status Code (before exception): " << ret << std::endl;
      ret = 0;

      // Pause the program to allow viewing the error message
      press_enter_to_continue();
    }

    // Check for a reboot condition and restart the main loop if necessary
    if (ret == 9999)
    {
      ret = 1;
    }
  }

  // Print exit code information based on the program's termination reason
  cout << "Exit Code: " << ret << " : " << endl;

  if (ret == 0)
  {
    cout << "Standard Exit" << endl;
  }
  else if (ret == 1)
  {
    cout << "Exit For Reboot" << endl;
  }
  else if (ret == 2)
  {
    cout << "Controlled Exit" << endl;
  }
  else
  {
    cout << "Non Standard Exit" << endl;
  }

  // Clean up and exit the program
  cout << "RasFLED ... Exit" << endl;
  return ret;
}




#endif
