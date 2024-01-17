// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: hmc5883l.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *  Derived from miniterm.c.
// ***************************************************************************************

#ifndef HMC5883L_H
#define HMC5883L_H

// Standard Header Files
//#include <stdio.h>      // standard input / output functions
#include <string>
#include <vector>

#include <unistd.h>         // UNIX standard function definitions
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>  // LINUX i2c device libraries.

// RASFled related header files

#include "fled_time.h"

//#include <stdlib.h>
//#include <string.h>     // string function definitions
//#include <unistd.h>     // UNIX standard function definitions
//#include <fcntl.h>      // File control definitions
//#include <errno.h>      // Error number definitions
//#include <termios.h>    // POSIX terminal control definitions

// RASFled related header files

//#include "helper.h"
//#include "stringthings.h"
//#include "rasapi.h"

using namespace std;

// -------------------------------------------------------------------------------------
// 

class HMC5883L_PROPERTIES
{
  public:

  bool AUTOSTART =  false;

  // Comm port settings
  string PORT = "";
  unsigned char I2C_ID;

  bool CONTINUOUS_DATA =  false;
  // If true, and data stops for time, will shut down port and 
  //  fall back into autostart mode. // No data is recognized as 
  //  a disconnect, resulting in autoconnet starting again. 
  // If false, port will stay open even if no data is being seen.
  //  No checks for hardware disconnects.
};

class COMPASS_XYZ
{
  public:

  float X = 0;
  float Y = 0;
  float Z = 0;
};

class HMC5883L
{
  private:

  int DEVICE = 0;                 // For file IO.

  unsigned char BUFFER[16];

  // Data

  COMPASS_XYZ RAW_XYZ;            // Temporary storage of XYZ before process;

  vector<COMPASS_XYZ> RAW_POINTS;     // History of data received.
  vector<float> CALIBRATED_BEARINGS;  // History of calculated bearings

  float BEARING = 0;              // Recent Bearing
  float BEARING_JITTER_MIN = 0;
  float BEARING_JITTER_MAX = 0;

  // Calibration
  void add_point(COMPASS_XYZ Point);
  bool CALIBRATE = false;
  int CURRENT_CALIBRATION_LEVEL = 0;

  // Simple calibration
  bool MIN_MAX_HAS_DATA = false;
  float X_MIN = 0;
  float X_MAX = 0;
  float Y_MIN = 0;
  float Y_MAX = 0;
  float Z_MIN = 0;
  float Z_MAX = 0;

  float X_OFFSET = 0;
  float Y_OFFSET = 0;
  float Z_OFFSET = 0;

  //

  TIMED_PING DATA_RECIEVED_TIMER;
  TIMED_PING CYCLE_TIMER;
  int CYCLE = 99;     // Connection cycles, independent of io
    // -1 - Ignore Cycles, autoconnect is off.
    //  0 - Normal connected read write cycle mode
    //  1 - Shutdown Cycle
    //  2 - Starting Up Cycle

  int CYCLE_CHANGE = 99;  // Internal: for controling start stop and autostart.

  float KNOWN_DEVICE_DEGREE_OFFSET = 23.0f;   // If device is mounted offset to  
                                              //  true forward facing, adjust compass 
                                              //  bearing to reflect

  bool CONNECTED = false;                     // Set to true if connected.

  // Calibration Variables and Routines
  TIMED_PING CALIBRATION_SPOT_CHECK;

  TIMED_PING CALIBRATION_TIMER_LEVEL_1;
  bool calibration_level_1_check(); // Checks most recent RAW_POINTS
                                    //  Returns true if outside max min
  void calibration_level_1();       // Run Level 1 cal routines.

  // Comms Routines
  bool register_write(char Register, char Value);
    // Internal: Change chip settings.

  bool create();      // Internal: Opens port for access.

  void stop();        // Internal: Closes port for access.
                      //  Not yet fully implemented.

  // Process
  void process(unsigned long tmeFrame_Time);     // Internal: Processes most recent received data. 
                      // Performs Calibration Routines

  public:

  HMC5883L_PROPERTIES PROPS;

  int raw_points_size();              // Returns size of xyz history.

  COMPASS_XYZ raw_xyz();              // Return most recent raw xyz;
  COMPASS_XYZ raw_xyz(int Position);  // Returns raw xyz data from history

  COMPASS_XYZ calibrated_xyz();             // Return most recent xyz data with the current calibratons.
  COMPASS_XYZ calibrated_xyz(int Position); // Returns xyz data from history with the current calibratons.

  int current_calibration_level();
  // Not yet implemented
  //  0 - No calibration Done
  //  1 - Calibration of MIN MAX in progress.

  void calibrateion_reset();
  // Removes all calibration data

  void calibrate_toggle(unsigned long tmeFrame_Time);
  // Start / Stop Calibration

  bool calibrate_on();
  // Returns true if calibration in progress.

  bool connected();
  // Returns true if hmc5883l is successfully connected.
  //  Will assume data is being received.

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

  //bool active(unsigned long tmeFrame_Time);
  // check to see if data recieved recently.
  // Not needed at this point. Instead, access 
  // connected.

  float bearing();
  // Direction Facing.
};


#endif