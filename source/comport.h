// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: comport.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *  Derived from miniterm.c.
// ***************************************************************************************

#ifndef COMPORT_H
#define COMPORT_H

// Standard Header Files
#include <string>
#include <deque>
#include <cstring>      // for memcpy

#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

// RASFled related header files

#include "helper.h"
#include "stringthings.h"
#include "rasapi.h"
#include "fled_time.h"

using namespace std;

// -------------------------------------------------------------------------------------
// 

class COMPORT_PROPERTIES
// Panel border characters.
{
  public:

  bool AUTOSTART =  false;
  // If true, will open a comm port without being called. 
  
  bool CONTINUOUS_DATA =  false;
  // If true, and data stops for time, will shut down port and 
  //  fall back into autostart mode. // No data is recognized as 
  //  a disconnect, resulting in autoconnet starting again. 
  // If false, port will stay open even if no data is being seen.
  //  No checks for hardware disconnects.

  // ----
  // Comm port settings
  string PORT = "";
  int BAUD_RATE = 38400;
  
  bool BAUD_RATE_CHANGE_TO = false;
  int BAUD_RATE_TARGET = 115200;
  // After connecting to initial baud rate, 
  //  If connection sucess, send command preloaded in 
  //    "device_baud_rate_change_to_target_string("COMMAND")" and attempt 
  //    to reconnect at BAUD_RATE_TARGET.
  //  If connection fails, attempts to reconnect at BAUD_RATE_TARGET regardless.
  //  Will rotate between BAUD_RATE_CHANGE_TO and BAUD_RATE untill data is found.

  int BIT_COUNT = 8;
  bool PARITY = false;
  int STOP_BITS = 1;
  bool HARDWARE_FLOW_CONTROL = false;
  bool XONXOFF = false;
  bool DISABLE_CANONICAL_MODE = false;  // Experiemental (Not Implemented)
  
  // Not access because implementation is non blocking.
  // Part of the tty settings
  int READ_BYTE_MIN = 1;  // Minimum bytes to process.
  int READ_MIN_TIME = 5;  // Minimum time to wait till process.

  // End of serial comm line character.
  char ENDING_CHAR = '\n';  // \r cr, \n lf

  bool PRINTABLE_CHARACTERS_ONLY = true;
  // If true, Filters out all non printable characters from serial input stream.
  // If false, all characters from stream, including garbage characters from 
  //  improper configuration, and control characters, are accepted as valid text.
  //  Will still recognize ENDING_CHAR as end of line.
  //  Buffer overflow is not handled.

  // Log File Save Directory and Name
  string SAVE_LOG_FILENAME = "";

  // Can't be hot enabled.  For testing.
  // Reads data generated in save log as if from serial port.
  bool RECEIVE_TEST_DATA = false;
  string TEST_DATA_FILENAME = "";
  int TEST_DATA_MS_DELAY_BETWEEN_MESSAGES = 100;
  int TEST_DATA_MS_MESSAGE_COUNT_PER_ITTERATION = 1;

  // Flash Data Recorder
  bool FLASH_DATA_RECORDER_ACTIVE = false;  // Enable cacheing of serial data.
  int FLASH_DATA_SIZE = 5000;
  // Data written to SAVE_TO_LOG_FILENAME appended flash in name, when 
  //  write_flash_data() is called.

  // Simple property to for exteral routines to know if output should be sent
  //  to console. Does not do anything.  Just a variable holder for exteral
  //  process() routines to check and output data to screen if true.
  bool PRINT_RECEIVED_DATA = false;

};

class COMPORT
{
  private:

  struct termios tty;
  struct termios tty_old;

  bool SAVE_TO_LOG = false;
  bool SAVE_TO_LOG_SENT_DATA = false;
  string SAVE_TO_LOG_FILENAME = "";

  int USB = 0;

  deque<string> WRITE_TO_COMM;
  // List of text to be sent to com port on next cycle.

  deque<string> READ_FROM_COMM;
  // list of received data from com port
  
  deque<string> TEST_DATA;
  TIMED_IS_READY TEST_DATA_DELAY_TIMER;

  string RESPONSE = "";
  // Retain partially received responses for when data is 
  //  available again.

  // Flash Data
  deque<string> FLASH_DATA;
  bool FLASH_DATA_WRITE = false;

  int ACTIVE_BAUD_RATE = 0;

  // BAUD Rate Change
  string BAUD_RATE_TARGET_DEVICE_CHANGE_BAUD_RATE_STRING = "";

  //bool CYCLE_CLOSE = false;
  //bool CYCLE_AUTO_START = true;
  //bool CYCLE_CHANGE_BAUD = false;

  TIMED_PING DATA_RECIEVED_TIMER;
  TIMED_PING CYCLE_TIMER;
  int CYCLE = 99;     // Connection cycles, independent of io
    // -1 - Ignore Cycles, autoconnect is off.
    //  0 - Normal connected read write cycle mode
    //  1 - Shutdown Cycle
    //  2 - Starting Up Cycle
    //  3 - Changing Baud Speed Cycle

    // 99 - First Run

  int CYCLE_CHANGE = 99;

  public:

  COMPORT_PROPERTIES PROPS;

  private:

  void write_to_comm(string Command);
  // Sends text Command to comm

  bool read_from_comm();
  // Read a line from comm port.
  // Returns true if data added to queue.
  // Returns false if error or no data available.

  bool create();
  // Prepares communications.

  void stop();
  // Prepares communications.

  bool CONNECTED = false;

  public:

  // Error detection not implemented. Comm can be disabled
  //  by setting ACTIVE TO FALSE.
  //  Queue stacks will remain available.

  bool record_in_progress();
  // returns true if comms are being recored to file.

  void log_file_on();
  // Starts logging to file
  
  void log_file_off();
  // Stops logging to file

  void device_baud_rate_change_to_target_string(string Device_Baud_Rate_Change_String);
  // String to send to device to change baud rate to target.
  //  Needs preloading.

  void write_flash_data();
  // Sets Flash data to be written on next flash data check.

  void flash_data_check();
  // Maintains Flash Data size.
  // Writes Flash Data if FLASH_DATA_WRITE is set to true,
  //  then sets FLASH_DATA_WRITE to false.

  void send(string Text);
  // Stack send commands to be processed at cycle.

  string recieve();
  // Get first item in recieve stack to be processed at cycle.

  int recieve_size();
  // Recieve Stack size.

  void request_to_send();
  // A seperate request to send.
  //  Needs to allow the client time to process andput together the message
  //  together.  If seperating the request further up the program stack 
  //  isnt working, then maybe introducing a time delay.
  //  Timing is essential for flow of info.
 
  bool cycle(unsigned long tmeFrame_Time);
  // Writes and Reads data to comm port to send and
  //  recieve queues.

  int cycle_change();
  // returns true on cycle change
  // -1 - Ignore Cycles, autoconnect is off.
  //  1 - Shutdown Cycle
  //  2 - Starting Up Cycle
  //  3 - Changing Baud Speed Cycle

  void open_port();
  // External open port call.
  //  Starts connection at next cycle() call.
  //  All intial properties are restored.
  //    Autoconnect value is restored, as was.
  //    Continuous data checks are restore, as was.
  //    Search for target baud rate is restored, as was.

  void close_port();
  // External close port call.
  //  Turns off autoconnect and stops continuous data checks.

};



#endif