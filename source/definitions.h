// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: definitions.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define RASFLED

// Debug Counters
#define DEBUG_COUNTER false

// -------------------------------------------------------------------------------------
// DEFINITIONS
// -------------------------------------------------------------------------------------

#define Revision "2.253_250523"
#define  QACODE  "A565"

// -------------------------------------------------------------------------------------
// TEST MODES
#define TEST_MODE           false   // file accessed will be the in test dir, "~/rasfled-t"

#define TEST_DATA_CAN_BUS   false   // "rascomms.txt" in the auto dir
#define TEST_DATA_AIRCRAFT  false   //
#define TEST_DATA_GPS       false   //

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP
//  Defined here for simplicity.  All set during compile
//  Affects:
//    Button sizes
//    Gadget Animations
//    Font Sizes
//    Gadget Sizes

#define DEF_FRAME_RATE 3
// Variable Redraw Rates
// 1: 60 ms - 15 fps
// 2: 30 ms - 30 fps
// 3: 15 ms - 60 fps

#if DEF_FRAME_RATE == 3
  #define DEF_FRAME_RATE_DELAY 15
  #define DEF_FRAME_RATE_DELAY_MULTIPLIER 2.0f
#elif DEF_FRAME_RATE == 2
  #define DEF_FRAME_RATE_DELAY 30
  #define DEF_FRAME_RATE_DELAY_MULTIPLIER 1.0f
#else // DEF_FRAME_RATE == 1
  #define DEF_FRAME_RATE_DELAY 60
  #define DEF_FRAME_RATE_DELAY_MULTIPLIER 0.5f
#endif

//---

#define DEF_SCREEN_SIZE 2
// Size 1 = 1280 x 720
// Size 2 = 1920 x 1080

#if DEF_SCREEN_SIZE == 1
  #define DEF_SCREEN_WIDTH 1920
  #define DEF_SCREEN_HEIGHT 1080
  #define DEF_SCREEN_SIZE_X_MULTIPLIER 1.5f
  #define DEF_SCREEN_SIZE_Y_MULTIPLIER 1.5f
#else
  #define DEF_SCREEN_WIDTH 1280
  #define DEF_SCREEN_HEIGHT 720
  #define DEF_SCREEN_SIZE_X_MULTIPLIER 1.0f
  #define DEF_SCREEN_SIZE_Y_MULTIPLIER 1.0f
#endif

// -------------------------------------------------------------------------------------
// FILES SETUP

// Specific Directories
//#define DEF_USB_DEV_DIR                   "/dev/serial/by-id/"     // Not Created

// not supported anymore: #define DEF_DEAMON_LOG                    "/var/log/daemon.log"   // Not Created
// try:       sudo apt update && sudo apt install rsyslog
// settings:  sudo nano /etc/rsyslog.conf
//       add the following lines to make it move viewable:
//            # Traditional Template
//            $ActionFileDefaultTemplate RSYSLOG_TraditionalFileFormat

#define DEF_DEAMON_LOG                    "/var/log/syslog"

#define DEF_AIRCRAFT_1090_DIR             "/run/dump1090-fa/"     // Not Created
#define DEF_AIRCRAFT_FA_JSON_FILE         "aircraft.json"         // Not Created

// Main Control for working rasfled
#define DEF_WORKING_DIR                   "/etc/rasfled/"         // Create Directory
#define DEF_WORKING_DIR_TEST              "/etc/rasfled-t/"       // Create Directory

#define DEF_COMPASS_SUB_DIR               "compass/"              // Create Directory

#define DEF_ANIMATIONS_FILE              "animations.json"
#define DEF_CONFIGURATION                 "configuration.json"
#define DEF_RUNNING_STATE                 "runningstate.json"
#define DEF_QR_CODE                       "qr_rasfled2_rev.png"
#define DEF_REFERENCE_CARD                "refer_advert.png"

// Rasfled logs and user files
#define DEF_LOGS_DIR                       "/home/delmane/rasfled/"    // Create Directory
#define DEF_LOGS_DIR_TEST                  "/home/delmane/rasfled-t/"  // Create Directory

#define DEF_CAN_BUS_SUB_DIR               "auto/"                 // Create Directory
#define DEF_AIRCRAFT_SUB_DIR              "adsb/"                 // Create Directory
#define DEF_GPS_SUB_DIR                   "gps_log/"              // Create Directory
#define DEF_ADVERTS_SUB_DIR               "adverts/"              // Create Directory

// CAN Bus
#define DEF_CAN_BUS_DEV_NAME              "/dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00"         // System File
#define DEF_CAN_BUS_HISTORY_LOG_SUB_DIR   "history/"
#define DEF_CAN_BUS_SAVE_LOG_NAME         "rascomms"
#define DEF_CAN_BUS_TEST_DATA_NAME        "rascomms.txt"
#define DEF_CAN_BUS_ERROR_LOG             "error_log.txt"

// GPS
//#define DEF_GPS_DEV_NAME                "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_B001JKAO-if00-port0"  // GPS via serial dongle
#define DEF_GPS_DEV_NAME                  "/dev/ttyAMA0"                                                    // GPS via ttyAMA0  
#define DEF_GPS_SAVE_LOG_NAME             "gpscomms"
#define DEF_GPS_TEST_DATA_NAME            "gpscomms_test.txt"

// Compass
#define DEF_COMPASS_DEV_NAME              "/dev/i2c-1"                                    // System File
//#define DEF_COMPASS_DEV_NAME              "/dev/i2c-2"                                    // System File

#define DEF_COMPASS_OFFSET_HISTORY        "compass_offset_history.json"


// -------------------------------------------------------------------------------------
// AUDIO SETUP

// Enable disable calling "aplay" for audio.
#define DEF_ENABLE_AUDIO    false

// -------------------------------------------------------------------------------------
// CAN BUS COMMS SETUP

// COMMS Settings CAN Bus
#define COMMS_AUTOSTART                       true
#define COMMS_BAUD                            115200
#define COMMS_BIT_COUNT                       8
#define COMMS_BIT_PARITY                      false
#define COMMS_STOP_BITS                       1
#define COMMS_HARDWARE_FLOW_CONTROL           false
#define COMMS_DISABLE_CANONICAL_MODE          false
#define COMMS_XONXOFF                         false

// -------------------------------------------------------------------------------------
// GPS COMMS SETUP

#define COMMS_AUTOSTART_GPS                   true
#define COMMS_BAUD_GPS                        9600
#define COMMS_BAUD_BAUD_RATE_CHANGE_TO_GPS    false
#define COMMS_BAUD_TARGET_GPS                 115200
#define COMMS_BIT_COUNT_GPS                   8
#define COMMS_BIT_PARITY_GPS                  false
#define COMMS_STOP_BITS_GPS                   1
#define COMMS_HARDWARE_FLOW_CONTROL_GPS       false
#define COMMS_DISABLE_CANONICAL_MODE_GPS      false
#define COMMS_XONXOFF_GPS                     false
#define CONTINUOUS_DATA_GPS                   true

// -------------------------------------------------------------------------------------
// COMPASS COMMS SETUP

#define COMMS_AUTOSTART_COMPASS               true
#define COMMS_ID_COMPASS                      0x1E
#define CONTINUOUS_DATA_COMPASS               true

#define COMMS_COMPASS_POLLING_RATE_MS         15
#define COMMS_COMPASS_HISTORY_TIME_SPAN_MS    500

#define COMMS_COMPASS_CAL_LOCK_AT_START       false
#define COMMS_COMPASS_CAL_LOAD_AT_START       false

#define COMMS_COMPASS_CAL_PT_1_X              385.0f
#define COMMS_COMPASS_CAL_PT_1_Y              94.0f
#define COMMS_COMPASS_CAL_PT_1_VAR            0.263f

#define COMMS_COMPASS_CAL_PT_2_X              517.0f
#define COMMS_COMPASS_CAL_PT_2_Y              254.0f
#define COMMS_COMPASS_CAL_PT_2_VAR            0.188f

#define COMMS_COMPASS_CAL_PT_3_X              238.0f
#define COMMS_COMPASS_CAL_PT_3_Y              574.0f
#define COMMS_COMPASS_CAL_PT_3_VAR            0.445f

#define COMMS_COMPASS_CAL_PT_4_X              80.0f
#define COMMS_COMPASS_CAL_PT_4_Y              423.0f
#define COMMS_COMPASS_CAL_PT_4_VAR            0.348f

#define COMMS_COMPASS_CAL_OFFSET              0.0f - 160.0f

// -------------------------------------------------------------------------------------
// AUTO TIRE TTL SETUP

// TTL Values - Individually adjust the TTL values of the auto.
#define TTL_LF_Top_Percentage   99.37
#define TTL_LF_Low_Percentage   98.92

#define TTL_RF_Top_Percentage   99.37
#define TTL_RF_Low_Percentage   98.92

#define TTL_LB_Top_Percentage   99.37 // Same Tire, Different Specs?
#define TTL_LB_Low_Percentage   98.92

#define TTL_RB_Top_Percentage   99.37
#define TTL_RB_Low_Percentage   98.92

// -------------------------------------------------------------------------------------
// Reserved Alert List

// AUTO
#define AUTO_RESERVE_ALERT_TEMP_S_TEMP     0
#define AUTO_RESERVE_ALERT_TEMP_COOLANT    1
#define AUTO_RESERVE_ALERT_ELEC_VOLTAGE    2
#define AUTO_RESERVE_ALERT_FUEL_LEVEL      3
#define AUTO_RESERVE_ALERT_TEMP_INTAKE     4
#define AUTO_RESERVE_ALERT_UNKNOWN_MESSAGE 5
#define AUTO_RESERVE_ALERT_MIL             6


// -------------------------------------------------------------------------------------
#define COMMS_FLASH_DATA_RECORDER_ACTIVE  true

// -------------------------------------------------------------------------------------
// DEBUG HOTKEY SETUP (no longer active)

// Key Mapping
#define KEYEXIT           'X'
#define KEYRESIZE         '*' // Key Resize

// Debugging and Diagnosis Keys
#define KEYDEBUG          '/' // Enter, Exit debug mode.
#define KEYLEDTEST        'c' // Turn all LEDs on
#define KEYLEDDRCYCL      't' // Cycle through the doors
#define KEYTESTANIM       'a' // Cycle Upper or Lower limits of LEDs

// -------------------------------------------------------------------------------------
// Testing and Debugging
#define BOOTEST       false       // Not Implemented - Fading Away

// -------------------------------------------------------------------------------------
// Default Timer
#define DEFAULTTIMER      5   //Minutes

// -------------------------------------------------------------------------------------
#define LED_FPS           15  // Compile time LED, delay in ms.

// -------------------------------------------------------------------------------------
// Door Switches

//#define DOOR_SWITCH_LEEWAY_TIME 60  // Debounce - Leeway time for door switches in ms.
//#define DOOR_SWITCH_LEEWAY_TIME 120 // Debounce - Leeway time for door switches in ms.
//#define DOOR_SWITCH_LEEWAY_TIME 240 // Debounce - Leeway time for door switches in ms.
#define DOOR_SWITCH_LEEWAY_TIME 480   // Debounce - Leeway time for door switches in ms.

// -------------------------------------------------------------------------------------
// LED Strip Declarations  (WS2811 LEDs SETUP)

#define DEF_WS2811_ENABLED      true

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2812_STRIP		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

// StupidRandom Parameters
#define StuRNDsize              100


#endif