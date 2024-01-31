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
  TIMED_IS_READY  input_from_switches;    // Delay for the hardware switches.
  TIMED_IS_READY  events_and_render;      // Delay for the events and render system.
  TIMED_IS_READY  input_from_user;        // Delay for the input from mouse and keyboard.
  TIMED_IS_READY  display;                // Delay for displaying information on the console.
  TIMED_IS_READY  comms_timer;            // Delay for communicating with Automobile and GPS
                                          //  serial comms.
  TIMED_IS_READY  compass_timer;          // Delay for communicating with compass 
                                          // serial comms.
  unsigned long   tmeSleep_Wake_time = 0; // Will contain time the cycle sleeper wakes.

  EFFICIANTCY_TIMER effi_timer;           // Diagnostic timer to measure cycle times.
  EFFICIANTCY_TIMER effi_timer_screen;    // Diagnostic timer to measure cycle times.
  EFFICIANTCY_TIMER effi_timer_comms;     // Diagnostic timer to measure cycle times.

  // Create Threads
  future<void> thread_render; // The thread containing function to send the led color array 
                              //  to the leds.
  bool thread_render_running = false; // Set to true when thread is active.

  // Control for the output thread. 
  //  The only part of output asynced is the printf function of the player 1001 type.
  //  Everything else in output will be paused if async is actively running and does 
  //  not need to be asynced because the interface is fast enough to run within one 
  //  cycle.
  future<void> thread_output;         // The thread containing function to printf a large 
                                      //  string.
  //bool thread_output_running = false; // Set to true when thread is active.
  string raw_string_buffer = "";      // A string buffer to contain the Screen buffer. 
                                      //  Redundant. Possibly consolidate the line. 

  // Define System Data and Console
  int return_code = 0;
  system_data sdSystem;
  sdSystem.TTY_ONLY = TTY_Only;

  // Load Windows or Console
  SCREEN4 cons_2;

  // Switch Lights On
  sdSystem.Lights_On.set(true);
  
  // Initialize wiring pi
  //int intRet = wiringPiSetup();
  wiringPiSetup();

  // Set is_ready variables
  input_from_switches.set(20);
  events_and_render.set(get_frame_interval(sdSystem.CONFIG.iFRAMES_PER_SECOND));
  input_from_user.set(65);
  display.set(SCREENUPDATEDELAY);

  // System LogFile Variables
  FILE_WATCH watcher_daemon_log;
  watcher_daemon_log.start(FILES_DEAMON_LOG);

  // System LogFile Variables
  FILE_WATCH watcher_aircraft_json;
  watcher_aircraft_json.PROP.READ_FROM_BEGINNING = true;
  watcher_aircraft_json.PROP.WATCH_SIZE_CHANGE = false;
  watcher_aircraft_json.PROP.WATCH_TIME_CHANGE = true;
  watcher_aircraft_json.start(FILES_AIRCRAFT_JSON);

  // Disposable Variables
  int count  = 0;

  // ---------------------------------------------------------------------------------------
  comms_timer.set(2);
  
  // Can Bus Comm Port Setup
  sdSystem.COMMS_AUTO.PROPS.PORT = COMMS_PORT_CAN;
  
  sdSystem.COMMS_AUTO.PROPS.AUTOSTART = COMMS_AUTOSTART;
  
  sdSystem.COMMS_AUTO.PROPS.BAUD_RATE = COMMS_BAUD;
  sdSystem.COMMS_AUTO.PROPS.BIT_COUNT = COMMS_BIT_PARITY;
  sdSystem.COMMS_AUTO.PROPS.PARITY = COMMS_BIT_PARITY;
  sdSystem.COMMS_AUTO.PROPS.STOP_BITS = COMMS_STOP_BITS;
  sdSystem.COMMS_AUTO.PROPS.HARDWARE_FLOW_CONTROL = COMMS_HARDWARE_FLOW_CONTROL;
  sdSystem.COMMS_AUTO.PROPS.DISABLE_CANONICAL_MODE = COMMS_DISABLE_CANONICAL_MODE;
  sdSystem.COMMS_AUTO.PROPS.XONXOFF = COMMS_XONXOFF;

  sdSystem.COMMS_AUTO.PROPS.SAVE_LOG_FILENAME = COMMS_SAVE_LOG_FILENAME;
  
  sdSystem.COMMS_AUTO.PROPS.RECEIVE_TEST_DATA = COMMS_RECEIVE_TEST_DATA;
  sdSystem.COMMS_AUTO.PROPS.TEST_DATA_FILENAME = COMMS_TEST_DATA_FILENAME;

  sdSystem.COMMS_AUTO.PROPS.FLASH_DATA_RECORDER_ACTIVE = COMMS_FLASH_DATA_RECORDER_ACTIVE;

  // Prep Automobile Send Messages
  sdSystem.CAR_INFO.set_default_request_pid_list();

  // ---------------------------------------------------------------------------------------
  // GPS Comm Port Setup
  compass_timer.set(100);

  sdSystem.COMMS_GPS.PROPS.PORT = COMMS_PORT_GPS;

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

  sdSystem.COMMS_GPS.PROPS.SAVE_LOG_FILENAME = COMMS_SAVE_LOG_FILENAME_GPS;
  
  sdSystem.COMMS_GPS.PROPS.RECEIVE_TEST_DATA = COMMS_RECEIVE_TEST_DATA_GPS;
  sdSystem.COMMS_GPS.PROPS.TEST_DATA_FILENAME = COMMS_TEST_DATA_FILENAME_GPS;

  sdSystem.COMMS_GPS.device_baud_rate_change_to_target_string(
  sdSystem.GPS_SYSTEM.device_change_baud_rate_string(COMMS_BAUD_TARGET_GPS));

  // ---------------------------------------------------------------------------------------
  // Compass Comm Port Setup
  sdSystem.COMMS_COMPASS.PROPS.PORT = COMMS_PORT_COMPASS;
  sdSystem.COMMS_COMPASS.PROPS.I2C_ID = COMMS_ID_COMPASS;
  //sdSystem.COMMS_COMPASS.PROPS.BAUD_RATE = COMMS_BAUD_GPS;

  sdSystem.COMMS_COMPASS.PROPS.AUTOSTART = COMMS_AUTOSTART_COMPASS;
  sdSystem.COMMS_COMPASS.PROPS.CONTINUOUS_DATA = CONTINUOUS_DATA_GPS;
  
  // Preload Calibration Data
  COMPASS_XYZ A_Cal_Pt;
  COMPASS_XYZ B_Cal_Pt;
  COMPASS_XYZ C_Cal_Pt;
  COMPASS_XYZ D_Cal_Pt;

  A_Cal_Pt.X = 386.0f;
  A_Cal_Pt.Y = 68.0f;
  float A_Cal_Var = 300.0f;

  B_Cal_Pt.X = 270.0f;
  B_Cal_Pt.Y = 532.0f;
  float B_Cal_Var = 300.0f;

  C_Cal_Pt.X = 120.0f;
  C_Cal_Pt.Y = 384.0f;
  float C_Cal_Var = 300.0f;

  D_Cal_Pt.X = 572.0f;
  D_Cal_Pt.Y = 194.0f;
  float D_Cal_Var = 300.0f;

  float Cal_Offset = 90.0f - 23.0f;

  sdSystem.COMMS_COMPASS.calibration_preload(A_Cal_Pt, A_Cal_Var, B_Cal_Pt, B_Cal_Var, C_Cal_Pt, C_Cal_Var, D_Cal_Pt, D_Cal_Var, Cal_Offset);

  // ---------------------------------------------------------------------------------------
  // Initialize the console
  cons_2.create(sdSystem);    // Prepare console.
  cons_2.SCREEN_COMMS.DEBUG_STATUS.DOOR.resize(4);
  
  // Print Start Info
  cons_2.SCREEN_COMMS.printw("Console Initialized ...  OK");
  cons_2.SCREEN_COMMS.printw("");
  cons_2.SCREEN_COMMS.printw("RasFLED");
  cons_2.SCREEN_COMMS.printw("  'X'    - Exit");
  cons_2.SCREEN_COMMS.printw("  'help' - Command List)");
  cons_2.SCREEN_COMMS.printw("");

  // ---------------------------------------------------------------------------------------
  // System Init

  // ---------------------------------------------------------------------------------------

  // Initialize Alert System
  cons_2.SCREEN_COMMS.printw("Initializing Alert System ...");

  // Prepare Alerts
  sdSystem.ALERTS.PROP.SWITCH_COUNT = sdSystem.CONFIG.iNUM_SWITCHES;
  sdSystem.ALERTS.create();
  cons_2.SCREEN_COMMS.printw("Initializing Timer ...");
  //FledTime tmeFled;

  // Sleeping Loop Variables
  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.
  
  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    cons_2.SCREEN_COMMS.printw("");
    cons_2.SCREEN_COMMS.printw("Adjusting Program Clock");
    cons_2.SCREEN_COMMS.printw("");

    sdSystem.ALERTS_2.add_generic_alert("Adjusting Program Clock");
    
    sdSystem.PROGRAM_TIME.clear_error();
  }
  
  // ---------------------------------------------------------------------------------------
  // Load system configuration and set data
  
  cons_2.SCREEN_COMMS.printw("Loading Configuration ...");

  // Set Running Color to white.
  sdSystem.init_running_color_list();
  sdSystem.set_running_color(CRGB(32,32,32),"White");

  // File System
  string Working_Directory = FILES_DIRECTORY;
  string Configuration_Files_JSON = FILES_CONFIGURATION;
  string Animations_Library_JSON = FILES_ANIMATIONS;
  //check_create_working_dir(FILES_DIRECTORY);

  //  -----
  // Create Filenames as Variables
  string Running_State_Filename = Working_Directory + FILES_RUNNING_STATE_SAVE;

  // Loading Configuration from files
  // yes, it resaves the file.  as is for now.

  if (load_json_configuration(sdSystem, Working_Directory, Configuration_Files_JSON) == true)
  {
    cons_2.SCREEN_COMMS.printw("  Configuration file loaded.");
  }
  else
  {
    cons_2.SCREEN_COMMS.printw("  Configuration file not loaded.  Generating Working Configuration File.");
    sdSystem.ALERTS.add_generic_alert("Configuration file not loaded.  Generating Working Configuration File.");

    if (save_json_configuration(sdSystem, Working_Directory, Configuration_Files_JSON) == true)
    {
      cons_2.SCREEN_COMMS.printw("    Configuration file created.");
      sdSystem.ALERTS.add_generic_alert("Configuration file created.");
    }
    else
    {
      cons_2.SCREEN_COMMS.printw("    Configuration file not created.");
      sdSystem.ALERTS.add_generic_alert("Configuration file not created.");
    }
  }
  
  // Loading Running State
  cons_2.SCREEN_COMMS.printw("  Loading running state ...");
  // yes, it resaves the file.  as is for now.
  if (load_saved_running_state_json(cons_2.SCREEN_COMMS, sdSystem, Running_State_Filename) != true)
  {
    cons_2.SCREEN_COMMS.printw("    Running state file not loaded.");
    sdSystem.ALERTS.add_generic_alert("Running state file not loaded.");
  }

  // Loading Animations Library.
  ANIMATION_HANDLER animations;

  animations.create_events(sdSystem);
  
  if (animations.load_collections(Working_Directory, Animations_Library_JSON) == true)
  {
    cons_2.SCREEN_COMMS.printw("  Animations file loaded.");
  }
  else
  {
    cons_2.SCREEN_COMMS.printw("    Animations file not loaded.");
    sdSystem.ALERTS.add_generic_alert("Animations file not loaded.");
  }

  // ---------------------------------------------------------------------------------------
  // LED Library Vars and Init
  cons_2.SCREEN_COMMS.printw("Initializing LEDS ...");

  int led_count = sdSystem.CONFIG.LED_MAIN.at(0).led_count();

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
    fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
    //return ret;
    //cons_2.SCREEN_COMMS.printw("ws2811_init failed: " + ws2811_get_return_t_str(ret))
    return_code = (int)ret;
  }
  else
  {
    cons_2.SCREEN_COMMS.printw("  LED count: " + to_string(led_count));
  }

  // ---------------------------------------------------------------------------------------
  // Automobile Handler

  AUTOMOBILE_HANDLER automobile_handler;

  // ---------------------------------------------------------------------------------------
  // The Player
  /*
  fstream fsPlayer;
  bool sucess = false;

  string Playlist_Filename = Working_Directory + FILES_PLAYLIST;

  cons_2.SCREEN_COMMS.printw("Initializing Player ...");
  if (load_playlist_json(cons, sdSystem, Playlist_Filename) == true)
  {
    if (cons.play_next_movie(fsPlayer) == true)
    {
      //cons_2.SCREEN_COMMS.printw("  Loading Reel");
    }
    else
    { 
      cons.the_player.booDisable = true;
      cons_2.SCREEN_COMMS.printw("FAILED - (Initializing Player)");
      sdSystem.ALERTS.add_generic_alert("FAILED - (Initializing Player)");
    }
  }
  */

  // ---------------------------------------------------------------------------------------
  // Define the Supid Random Numbers
  cons_2.SCREEN_COMMS.printw("Initializing Random Number Generator ...");
  stupid_random sRND;
  // Initialize the Stupid Random Numbers
  sRND.set();

  // ---------------------------------------------------------------------------------------
  // Light Strip Event System
  cons_2.SCREEN_COMMS.printw("Initializing Event System ...");

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
  cons_2.SCREEN_COMMS.printw("Initializing Hardware Sensors ...");
  for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
  {
    pinMode(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin, INPUT);
    pullUpDnControl(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin, PUD_UP);
  }

  // -------------------------------------------------------------------------------------
  // Door Sensor
  cons_2.SCREEN_COMMS.printw("Initializing Hardware Sensor Interface ...");

  // Initialize Switches
  hardware_monitor tmpSwitch;
  tmpSwitch.set(true, sdSystem.PROGRAM_TIME.current_frame_time(), DOOR_SWITCH_LEEWAY_TIME, true);
  for(int x=0; x<sdSystem.CONFIG.iNUM_SWITCHES; x++)
  {
    sdSystem.CONFIG.vhwDOORS.push_back(tmpSwitch);
  }

  // -------------------------------------------------------------------------------------

  // Start Power On Animation
  process_power_animation(sdSystem, sdSystem.PROGRAM_TIME.now(), animations, CRGB(0, 0, 25));
  
  // ---------------------------------------------------------------------------------------
  //  Repeating Sleeping Loop until eXit is triggered.
  // ---------------------------------------------------------------------------------------

  cons_2.SCREEN_COMMS.printw("Starting System ...");

  // Start the the compute timer (stopwatch) for first iteration. 
  effi_timer.start_timer(sdSystem.PROGRAM_TIME.current_frame_time());
  
  // **************************************************************************************
  // **************************************************************************************
  // **************************************************************************************

  // MAIN LOOP START
  while(cons_2.WINDOW_CLOSE == false)
  {
    // ---------------------------------------------------------------------------------------
    // Thread Management

    // Close all completed and active threads after sleep cycle is complete.

    // Before starting a new loop, close the render thread from the previous loop, if 
    //  render is complete
    if(thread_render_running == true)
    // Check to see if render thread was started before checking the completion status.
    {
      if(thread_render.wait_for(0ms) == future_status::ready)
      // Check to verify the thread is complete before allowing the render to start again. 
      {
        thread_render_running = false;
      }
    }

    /*
    // Before starting a new loop, close the console thread from the previous loop, if 
    //  data being printed to the screen has completed.
    if(thread_output_running == true)
    // Check to see if output thread was started before checking the completion status.
    {
      if(thread_output.wait_for(0ms) == future_status::ready)
      // Check to verify thte thread is complete before allowing the console to be updated again. 
      {
        thread_output_running = false;
      }
    }
    */

    // ---------------------------------------------------------------------------------------
    // --- Prpare the Loop ---

    //  Get current time.  This will be our timeframe to work in.
    if (sdSystem.PROGRAM_TIME.setframetime() == true)
    {
      cons_2.SCREEN_COMMS.printw("");
      cons_2.SCREEN_COMMS.printw("ALERT: PROGRAM TIME STREAM INTURPTED OR CORRUPT");
      cons_2.SCREEN_COMMS.printw("           DIFFERANCE: "  + to_string(sdSystem.PROGRAM_TIME.error()));
      cons_2.SCREEN_COMMS.printw("");

      sdSystem.ALERTS_2.add_generic_alert("ALERT: PROGRAM TIME\nSTREAM INTURPTED OR CORRUPT\nDIFFERANCE: " + 
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
    if (comms_timer.is_ready_no_reset(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      sdSystem.COMMS_AUTO.request_to_send();
    }

    // ---------------------------------------------------------------------------------------
    // --- Read Switchs --- 

     // Are switches ready -----------------
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
          sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).value = digitalRead(sdSystem.CONFIG.vSWITCH_PIN_MAP.at(x).pin);
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
      if(cons_2.SCREEN_COMMS.DEBUG_STATUS.DEBUG == true)
      {
        // Toggle on and off the door sensors with keyboard.
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(0).value = cons_2.SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(1).value = cons_2.SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(2).value = cons_2.SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
        sdSystem.CONFIG.vSWITCH_PIN_MAP.at(3).value = cons_2.SCREEN_COMMS.DEBUG_STATUS.DOOR[3];        
        
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
      v_DoorMonitorAndAnimationControlModule2(cons_2.SCREEN_COMMS, sdSystem, animations, sdSystem.PROGRAM_TIME.current_frame_time());
    }

    // Read light switchs and set day on or day off modes.
    sdSystem.Day_On_With_Override.set(sdSystem.CAR_INFO.active() == true && 
                                sdSystem.CAR_INFO.STATUS.INDICATORS.available() == true,
                                sdSystem.CAR_INFO.STATUS.INDICATORS.val_light_switch() == false, sdSystem.Day_On);

    // ---------------------------------------------------------------------------------------
    // --- Check and Execute Timed Events That Are Ready ---

    // Is Events and Render ready -----------------
    if (events_and_render.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
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

        /*
        // If debug mode Display all lights static color are selectted, replace all generated led colors
        // with a static color
        if ((cons.keywatch.getnoreset(KEYDEBUG) != 0) && (cons.keywatch.get(KEYLEDTEST) !=0))
        {
          for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
          {
            for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
            {
              MatxixFill(sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY, 
              sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(), 
              CRGB(25,25,25));
            }
          }

          booUpdate = true;
        }
        */
      }
      else // lights are off, blank values if neccessary.
      {
        // make sure the thread is free, otherwise the blank will be thrown away.
        if (thread_render_running == false)
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
        /*
        else
        {
          // Build TEST array to display
          int selected_test_array = cons.keywatch.get(KEYLEDDRCYCL) - 1;
          int pos = 0;
          int g = 0; 
          int s = 0;

          // Find Strip
          for(int group=0; group < sdSystem.CONFIG.LED_MAIN.at(0).g_size(); group++)
          {
            for(int strip=0; strip < sdSystem.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
            {
              if (selected_test_array == pos)
              {
                g = group;
                s = strip;
                sdSystem.t_group = group;
                sdSystem.t_strip = strip;
              }
              pos++;
            }
          }

          // Draw found strip
          MatrixPrepare(cons, sdSystem, 
                            sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(g).vLED_STRIPS.at(s).crgbARRAY, 
                            sdSystem.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(g).vLED_STRIPS.at(s).led_count(), 
                            matrix, mcount);
        }
        */

        //  Are the lights enable to display.       
        //    Lights off will not turn the lights off and clear their values.  
        //      Instead, transmitting those color are to the lights disabled.
        
        // LED Library Renderer -- Recommend: DON'T TOUCH
        matrix_render(led_count);
        
        // Create a seperate thread only to render the LEDs with the hardware.  This process
        //  is very intensive for the system and is only one way.  The render thread only needs 
        //  to rejoin with the main program, at the end of the main loop, to signify its 
        //  completion, so that the loop can restart and begin computing its values and colors 
        //  again. 
        // A render thread should not be created if no changes have been made to the led values.
        if (thread_render_running == false)
        {
          thread_render = async(proc_render_thread);
          thread_render_running = true;
        }
      }
    
    } // Is Events and Render ready -----------------
     
    // ---------------------------------------------------------------------------------------
    // Now that we have done all the hard work, read hardware, computed, generated, displayed 
    // all the lights, we will take the latter clock cycles to get keybord input and update 
    // console with status and so on.

    // Is Keyboard or Mouse read ready -----------------
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
        sdSystem.AIRCRAFT_COORD.process(file_to_string(FILES_AIRCRAFT_JSON));
      }

      processcommandlineinput(cons_2.SCREEN_COMMS, sdSystem, sdSystem.PROGRAM_TIME.current_frame_time(), animations);
      extraanimationdoorcheck2(cons_2.SCREEN_COMMS, sdSystem, sdSystem.PROGRAM_TIME.current_frame_time(), animations);
      
      // Also delayed, File maintenance.
      if (sdSystem.booRunning_State_File_Dirty == true)
      {
        save_running_state_json(sdSystem, Running_State_Filename);

        // set false even if there was a save error to avoid repeats.
        sdSystem.booRunning_State_File_Dirty = false;
      }
    }

    // ---------------------------------------------------------------------------------------
    // Comm Port Read
    // Automobile Data Process.
    // No need to thread. Comms are actually much faster than I was led to believe.

    effi_timer_comms.start_timer(sdSystem.PROGRAM_TIME.now());

    if (comms_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // CAN_Bus Serial Communications
      if (sdSystem.COMMS_AUTO.cycle(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_AUTO.cycle_change() == 99)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == -1)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 0)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: NORMAL READ MODE");
          sdSystem.ALERTS_2.sound_alert(1);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 1)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALERTS_2.sound_alert(3);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 2)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: CONNECT START");
          sdSystem.ALERTS_2.sound_alert(2);
        }
        else if (sdSystem.COMMS_AUTO.cycle_change() == 3)
        {
          cons_2.SCREEN_COMMS.printw("Automobile COMMS changed to: BAUD RATE CHECK");
        }
      }

      // GPS Serial Communications
      if (sdSystem.COMMS_GPS.cycle(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_GPS.cycle_change() == 99)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == -1)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 0)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: NORMAL READ MODE");
          sdSystem.ALERTS_2.sound_alert(1);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 1)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALERTS_2.sound_alert(3);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 2)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: CONNECT START");
          sdSystem.ALERTS_2.sound_alert(2);
        }
        else if (sdSystem.COMMS_GPS.cycle_change() == 3)
        {
          cons_2.SCREEN_COMMS.printw("GPS COMMS changed to: BAUD RATE CHECK");
        }
      }

      // ---------------------------------------------------------------------------------------
      // CAN Bus Process

      // Process info from comm port int automobile system.
      sdSystem.CAR_INFO.process(cons_2.SCREEN_COMMS, sdSystem.ALERTS_2, sdSystem.COMMS_AUTO, sdSystem.PROGRAM_TIME.current_frame_time());

      // Process Automobile Lights
      automobile_handler.update_events(sdSystem, cons_2.SCREEN_COMMS, animations, sdSystem.PROGRAM_TIME.current_frame_time());

      // ---------------------------------------------------------------------------------------
      // GPS Process
      
      sdSystem.GPS_SYSTEM.process(cons_2.SCREEN_COMMS, sdSystem.COMMS_GPS, sdSystem.PROGRAM_TIME.current_frame_time());
      cons_2.update_GPS_gadgets(sdSystem);
    }

    if (compass_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // Compass Serial Communications
      if (sdSystem.COMMS_COMPASS.cycle(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.COMMS_COMPASS.cycle_change() == 99)
        {
          cons_2.SCREEN_COMMS.printw("COMPASS COMMS changed to: INITIALIZE");
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == -1)
        {
          cons_2.SCREEN_COMMS.printw("COMPASS COMMS changed to: NO AUTOCONNECT MODE");
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 0)
        {
          cons_2.SCREEN_COMMS.printw("COMPASS COMMS changed to: NORMAL READ MODE");
          sdSystem.ALERTS_2.sound_alert(1);
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 1)
        {
          cons_2.SCREEN_COMMS.printw("COMPASS COMMS changed to: SHUTTING DOWN CYCLE");
          sdSystem.ALERTS_2.sound_alert(3);
        }
        else if (sdSystem.COMMS_COMPASS.cycle_change() == 2)
        {
          cons_2.SCREEN_COMMS.printw("COMPASS COMMS changed to: CONNECT START");
          sdSystem.ALERTS_2.sound_alert(2);
        }
      }
    }
    
    sdSystem.dblCOMMS_TRANSFER_TIME.set_data(effi_timer_comms.simple_elapsed_time(sdSystem.PROGRAM_TIME.now()));
      

    // ---------------------------------------------------------------------------------------
    // Is display to console ready -----------------
    if (display.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      // Call the Interface routine. (IO from user)
      //if (thread_output_running == false)
      // If thread is not running, then update the screen display terminal interface.
      //  After the interface has an update, see if the player has a frame left over inside.
      //  the buffer. 
      //  Only start a thread and pause future screen updateas if the buffer contains a movie 
      //  frame to be displayed.

      {
        // Refresh console data storeage from main program. This will be a pass through buffer. 
        // so the console will not have to access any real data. 
        sdSystem.store_door_switch_states();

        store_event_counts(sdSystem, animations);

        // ADS-B - Update all ADS-B gadgets with new data.
        cons_2.update_ADS_B_gadgets(sdSystem);

        // Automobile - Update all automobile Reference Data
        sdSystem.CAR_INFO.translate(sdSystem.ALERTS_2, sdSystem.PROGRAM_TIME.current_frame_time());
        cons_2.update_automobile_gadgets(sdSystem);

        // Update Switches to Alert system.
        /*
        for (int door=0; door < sdSystem.CONFIG.vhwDOORS.size(); door++)
        {
          sdSystem.ALERTS.update_switch(door, sdSystem.CONFIG.vhwDOORS.at(door).booVALUE);
        }
        */
        
        
        // Redraw the console screen with what the screen determines needs to be displayed.
        //cons.display(fsPlayer, sdSystem, sdSystem.PROGRAM_TIME.current_frame_time());
        effi_timer_screen.start_timer(sdSystem.PROGRAM_TIME.now());
        cons_2.draw(sdSystem);
        sdSystem.dblSCREEN_RENDER_TIME.set_data(effi_timer_screen.simple_elapsed_time(sdSystem.PROGRAM_TIME.now()));

        /*
        if (cons.the_player.get_next_frame_draw_time() > 0)
        {
          display.set_earliest_ready_time(cons.the_player.get_next_frame_draw_time());
        }

        // If anything in the screen buffer is waiting to be displayed, start a new thread 
        //  then print out the data. Likely to take a long time so pause further screen 
        //  updates while thread is running.
        if (cons.Screen.buffer_active == true)
        {
          raw_string_buffer = cons.Screen.buffer();
          thread_output = async(raw_window_player_draw_frame, raw_string_buffer);

          thread_output_running = true;
          cons.Screen.buffer_active = false;     
        }
        */
      }
  
      // Alert system checks
      sdSystem.ALERTS_2.alert_list_clean();
      
    } // Is display to console ready -----------------

    // ---------------------------------------------------------------------------------------
    // Now that the complete cycle is over, we need figure out how much time is remaining in 
    // the cycle and go to sleep for the appropriate amount of time. 
    // Calculate times and sleep till next frame is ready.

    // Determine how long to sleep and then sleep by 
    //  finding the earliest sleep wake time.
    tmeSleep_Wake_time = input_from_switches.get_ready_time();   
    if (events_and_render.get_ready_time() < tmeSleep_Wake_time)
    {
      tmeSleep_Wake_time = events_and_render.get_ready_time();
    }
    if (input_from_user.get_ready_time() < tmeSleep_Wake_time)
    {
      tmeSleep_Wake_time = input_from_user.get_ready_time();
    }
    if (display.get_ready_time() < tmeSleep_Wake_time)
    {
      tmeSleep_Wake_time = display.get_ready_time();
    }
    if (comms_timer.get_ready_time() < tmeSleep_Wake_time)
    {
      tmeSleep_Wake_time = comms_timer.get_ready_time();
    }
    if (compass_timer.get_ready_time() < tmeSleep_Wake_time)
    {
      tmeSleep_Wake_time = comms_timer.get_ready_time();
    }

    // Measure how much time has passed since the previous time the program was at 
    //  this point and store that value to be displayed in diag.
    sdSystem.dblCYCLETIME.set_data(effi_timer.elapsed_time(sdSystem.PROGRAM_TIME.now()));

    // Reset the the compute timer (stopwatch) and store the value before the program sleeps. 
    sdSystem.dblCOMPUTETIME.set_data(effi_timer.elapsed_timer_time(sdSystem.PROGRAM_TIME.now()));

    // Determine how long the program will sleep, store the value to be displayed in diag, and put the cycle
    //  to sleep.
    usleep ((int)(1000 * sdSystem.store_sleep_time(sdSystem.get_sleep_time(sdSystem.PROGRAM_TIME.now(), tmeSleep_Wake_time))));

    // Start the the compute timer (stopwatch) before the program as the program wakes to measure the amount of 
    //  time the compute cycle is. 
    effi_timer.start_timer(sdSystem.PROGRAM_TIME.now());

  }// End MAIN CYCLE WHILE loop.

  // ---------------------------------------------------------------------------------------
  // If we are here, then we are closing the program.
  
  // Wait for threads to end before continuing to shutdown.

  if(thread_render_running == true)
  // Check to see if render thread was started before checking the completion status.
  {
    while(thread_render.wait_for(10ms) != future_status::ready)
    {
      cons_2.SCREEN_COMMS.printw("Shutting thread down.");
    }
    thread_render_running = false;
  }

  /*
  if(thread_output_running == true)
  // Check to see if output thread was started before checking the completion status.
  {
    while(thread_output.wait_for(10ms) != future_status::ready)
    {
      cons_2.SCREEN_COMMS.printw("Shutting thread down.");
    }
    thread_output_running = false;
  }
  */
  
  // Shutdown RPI.
  shutdown();

  // Shutdown Graphical Window
  cons_2.shutdown(sdSystem);

  // close open files
  watcher_daemon_log.stop();

  if(sdSystem.booREBOOT == false)
  {
    // Just print we have ended the program.
    printf ("\nRasFLED Loop ... Exit\n");
  }
  else
  {
    // Just print are restarting the program.
    printf ("\nRasFLED Loop ... Rebooting\n");
    return_code = 9999;
  }

  return return_code;
}

// ---------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  int ret = 1;  // Run

  bool load_gfx_window = true;

  for (int pos = 1; pos < argc; pos++)
  {
    if (strcmp(argv[pos], "-h") == 0 || strcmp(argv[pos], "--help") == 0)
    {
      ret = 2;
      printf("\n-tty  :Load in Terminal\n");
      printf("        Warning - Command line input not yet implemented.\n");
      printf("                  There is no way of safely exiting the program.\n");
      printf("-gfx  :Load Graphics Window (default)\n\n");
    }
    else
    if (strcmp(argv[pos], "-tty") == 0)
    {
      load_gfx_window = false;
    }
    if (strcmp(argv[pos], "-gfx") == 0)
    {
      load_gfx_window = true;
    }
  }

  // Setup the Program
  setup();

  // Start the main loop.
  while (ret == 1)
  {
    printf("RasFLED Start ... \n");

    try
    {
      if (load_gfx_window == true)
      {
        ret = loop_2(false);
        printf("Load Graphics Window\n");
      }
      else
      {
        ret = loop_2(true); // To be phased out?
        printf("Do Not Load Graphics Window\n");
      }
    }
    catch (std::exception const& e)
    {
      printf("An Error has occured in the main loop.\n");
      printf("Error Code: %d\n", ret);
      ret = 0;
    }
    
    // Reboot?
    if(ret == 9999)
    {
      ret = 1;
    }
  }

  printf("Exit Code: %d: ", ret);

  if (ret == 0)
  {
    printf("Standard Exit\n");
  }
  else if (ret == 1)
  {
    printf("Exit For Reboot\n");
  }
  else if (ret == 2)
  {
    printf("Controled Exit\n");
  }
  else
  {
    printf("Non Standard Exit\n");
  }

  // Exit the program.
  printf("RasFLED ... Exit\n");
  return ret;
  
}




#endif
