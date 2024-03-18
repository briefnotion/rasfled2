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

#define Revision "2.141_240313"
#define  QACODE  "A565"

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP

// Variable Redraw Rates
// 60 - 15 fps
// 30 - 30 fps
// 15 - 60 fps
#define SCREENUPDATEDELAY 30  // in milliseconds 
//#define CONSOLESPLITSIZE  6

// Files
#define FILES_DIRECTORY           "/etc/RasFLED/"
//#define FILES_DIRECTORY           "/home/pi/source/RasFLED/configuration/"   // For Testing:  Configuration Files Directory

#define SUB_DIRECTORY_ADVERTS     "adverts/"

#define FILES_CONFIGURATION       "configuration.json"
#define FILES_RUNNING_STATE_SAVE  "runningstate.json"
#define FILES_PLAYLIST            "playlist.json"
#define FILES_ANIMATIONS          "animations.json"
#define FILES_QR_CODE             "qr_rasfled2_rev.png"

#define FILES_DEAMON_LOG          "/var/log/daemon.log"             // Raspi System Log

#define ERROR_LOG_FILENAME       "/home/pi/auto/error_log.txt"

#define TEST_MODE           false

// COMMS Settings ADS_B
#define FILES_AIRCRAFT_JSON       "/run/dump1090-fa/aircraft.json"  // FlightAware Aircraft JSON File
//#define FILES_AIRCRAFT_JSON       "/home/pi/flightaware/aircraft.json"  // For testing:  FlightAware Aircraft JSON File 

// COMMS Settings CAN Bus
#define COMMS_PORT_CAN     "/dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00"
#define COMMS_AUTOSTART                       true
#define COMMS_BAUD                            115200
#define COMMS_BIT_COUNT                       8
#define COMMS_BIT_PARITY                      false
#define COMMS_STOP_BITS                       1
#define COMMS_HARDWARE_FLOW_CONTROL           false
#define COMMS_DISABLE_CANONICAL_MODE          false
#define COMMS_XONXOFF                         false

#define COMMS_SAVE_LOG_FILENAME               "/home/pi/auto/rascomms"

#define COMMS_RECEIVE_TEST_DATA               false
#define COMMS_TEST_DATA_FILENAME              "/home/pi/auto/rascomms.txt"

#define AUTOMOBILE_LOG_FILE_DIRECTORY         "/home/pi/automobile_log/"

// COMMS Settings GPS
#define COMMS_PORT_GPS     "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_B001JKAO-if00-port0"
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

#define COMMS_SAVE_LOG_FILENAME_GPS           "/home/pi/gps/gpscomms"

#define COMMS_RECEIVE_TEST_DATA_GPS           false
#define COMMS_TEST_DATA_FILENAME_GPS          "/home/pi/gps/gpscomms.txt"

#define AUTOMOBILE_LOG_FILE_DIRECTORY_GPS     "/home/pi/gps_log/"

// COMMS Settings HMC5883L 3-Axis Compass
#define COMMS_PORT_COMPASS                    "/dev/i2c-1"

#define COMMS_PORT_COMPASS_OFFSET_HISTORY     "compass_offset_history.json"
#define COMMS_PORT_COMPASS_OFFSET_HISTORY_T   "compass_offset_history_test.json"

#define COMMS_ID_COMPASS                      0x1E
#define COMMS_AUTOSTART_COMPASS               true
#define CONTINUOUS_DATA_COMPASS               true
#define COMMS_COMPASS_CAL_LOCK_AT_START       false

#define COMMS_COMPASS_POLLING_RATE_FPS        15

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

#define COMMS_COMPASS_CAL_OFFSET              90.0f - 23.0f

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