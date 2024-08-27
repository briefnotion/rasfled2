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

#define Revision "2.189_240826"
#define  QACODE  "A565"

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP

// Variable Redraw Rates
// 60 - 15 fps
// 30 - 30 fps
// 15 - 60 fps
#define SCREENUPDATEDELAY 30  // in milliseconds 

#define TEST_MODE           false

#define TEST_DATA_CAN_BUS   false
#define TEST_DATA_AIRCRAFT  false
#define TEST_DATA_GPS       false

// Files

// Specific Directories
#define DEF_USB_DEV_DIR                   "/dev/serial/by-id/"     // Not Created
#define DEF_DEAMON_LOG                    "/var/log/daemon.log"   // Not Created

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
#define DEF_LOGS_DIR                       "/home/pi/rasfled/"    // Create Directory
#define DEF_LOGS_DIR_TEST                  "/home/pi/rasfled-t/"  // Create Directory

#define DEF_CAN_BUS_SUB_DIR               "auto/"                 // Create Directory
#define DEF_AIRCRAFT_SUB_DIR              "adsb/"                 // Create Directory
#define DEF_GPS_SUB_DIR                   "gps_log/"              // Create Directory
#define DEF_ADVERTS_SUB_DIR               "adverts/"              // Create Directory

// CAN Bus
#define DEF_CAN_BUS_USB_DEV_NAME          "usb-Arduino_LLC_Arduino_Leonardo-if00"         // System File
#define DEF_CAN_BUS_HISTORY_LOG_SUB_DIR   "history/"
#define DEF_CAN_BUS_SAVE_LOG_NAME         "rascomms"
#define DEF_CAN_BUS_TEST_DATA_NAME        "rascomms.txt"
#define DEF_CAN_BUS_ERROR_LOG             "error_log.txt"

// GPS
#define DEF_GPS_USB_DEV_NAME              "usb-FTDI_FT232R_USB_UART_B001JKAO-if00-port0"  // System File
#define DEF_GPS_SAVE_LOG_NAME             "gpscomms"
#define DEF_GPS_TEST_DATA_NAME            "gpscomms_test.txt"

// Compass
#define DEF_COMPASS_DEV_NAME              "/dev/i2c-1"                                    // System File
#define DEF_COMPASS_OFFSET_HISTORY        "compass_offset_history.json"

// COMMS Settings CAN Bus
#define COMMS_AUTOSTART                       true
#define COMMS_BAUD                            115200
#define COMMS_BIT_COUNT                       8
#define COMMS_BIT_PARITY                      false
#define COMMS_STOP_BITS                       1
#define COMMS_HARDWARE_FLOW_CONTROL           false
#define COMMS_DISABLE_CANONICAL_MODE          false
#define COMMS_XONXOFF                         false

// COMMS Settings GPS
#define COMMS_AUTOSTART_GPS                   true
#define COMMS_BAUD_GPS                        9600
#define COMMS_BAUD_BAUD_RATE_CHANGE_TO_GPS    true
#define COMMS_BAUD_TARGET_GPS                 115200
#define COMMS_BIT_COUNT_GPS                   8
#define COMMS_BIT_PARITY_GPS                  false
#define COMMS_STOP_BITS_GPS                   1
#define COMMS_HARDWARE_FLOW_CONTROL_GPS       false
#define COMMS_DISABLE_CANONICAL_MODE_GPS      false
#define COMMS_XONXOFF_GPS                     false
#define CONTINUOUS_DATA_GPS                   true

// COMMS Settings Compass
#define COMMS_ID_COMPASS                      0x1E
#define COMMS_AUTOSTART_COMPASS               true
#define CONTINUOUS_DATA_COMPASS               true

#define COMMS_COMPASS_POLLING_RATE_FPS        15

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

// TTL Values - Individually adjust the TTL values of the auto.
#define TTL_LF_Top_Percentage   100.15
#define TTL_LF_Low_Percentage   99.6

#define TTL_RF_Top_Percentage   100.15
#define TTL_RF_Low_Percentage   99.6

#define TTL_LB_Top_Percentage   100.15
#define TTL_LB_Low_Percentage   99.6

#define TTL_RB_Top_Percentage   99.37
#define TTL_RB_Low_Percentage   98.92

#define COMMS_FLASH_DATA_RECORDER_ACTIVE  true

// Key Mapping
#define KEYEXIT           'X'
#define KEYRESIZE         '*' // Key Resize

// Debugging and Diagnosis Keys
#define KEYDEBUG          '/' // Enter, Exit debug mode.
#define KEYLEDTEST        'c' // Turn all LEDs on
#define KEYLEDDRCYCL      't' // Cycle through the doors
#define KEYTESTANIM       'a' // Cycle Upper or Lower limits of LEDs

// Testing and Debugging
#define BOOTEST       false       // Not Implemented - Fading Away

// Default Timer
#define DEFAULTTIMER      5   //Minutes

#define LED_FPS           50  // Compile time LED Frames per Second, frams/second.

// -------------------------------------------------------------------------------------
// Door Switches

//#define DOOR_SWITCH_LEEWAY_TIME 60  // Debounce - Leeway time for door switches in ms.
//#define DOOR_SWITCH_LEEWAY_TIME 120 // Debounce - Leeway time for door switches in ms.
//#define DOOR_SWITCH_LEEWAY_TIME 240 // Debounce - Leeway time for door switches in ms.
#define DOOR_SWITCH_LEEWAY_TIME 480   // Debounce - Leeway time for door switches in ms.

// -------------------------------------------------------------------------------------
// LED Strip Declarations

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2812_STRIP		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

// StupidRandom Parameters
#define StuRNDsize              100


#endif