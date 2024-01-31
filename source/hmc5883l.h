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
#include <stdio.h>      // standard input / output functions
#include <string>
#include <vector>

#include <unistd.h>         // UNIX standard function definitions
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>  // LINUX i2c device libraries.
#include <cmath>

// RASFled related header files
#include "fled_time.h"
#include "helper.h"
//#include "stringthings.h"
//#include "rasapi.h"

//#include <stdlib.h>
//#include <fcntl.h>      // File control definitions
//#include <errno.h>      // Error number definitions
//#include <termios.h>    // POSIX terminal control definitions

using namespace std;

// -------------------------------------------------------------------------------------

float dist(float X, float Y);

// -------------------------------------------------------------------------------------

class COMPASS_XYZ
{
  public:

  float X = 0;
  float Y = 0;
  float Z = 0;
};

// -------------------------------------------------------------------------------------

class COMPASS_XY
{
  public:

  float X = 0;
  float Y = 0;
};

// -------------------------------------------------------------------------------------

bool XYZ_MIN_MAX(COMPASS_XYZ &Raw_XYZ, bool &Has_Data, MIN_MAX_SIMPLE &Xmm, MIN_MAX_SIMPLE &Ymm, MIN_MAX_SIMPLE &Zmm);

COMPASS_XYZ calc_offset(COMPASS_XYZ Y_Min,  COMPASS_XYZ Y_Max, COMPASS_XYZ X_Min, COMPASS_XYZ X_Max);

// -------------------------------------------------------------------------------------

bool four_point_check(COMPASS_XYZ Top, COMPASS_XYZ Bottom, 
                      COMPASS_XYZ Left, COMPASS_XYZ Right);

// -------------------------------------------------------------------------------------

class CAL_LEVEL_2_QUAD_RECORD
{
  private:

  int SIZE = 500;

  public:

  bool OVERFLOW = false;

  vector<COMPASS_XYZ> DATA_POINTS;
  vector<float> VARIANCE_COLLECTION;

  void add_point(COMPASS_XYZ &Raw_XYZ);

  void clear();

};

class CALIBRATION_DATA
{
  private:

  public:

  CAL_LEVEL_2_QUAD_RECORD QUAD_DATA;
  COMPASS_XYZ COORD;
  float LAST_KNOWN_VARIANCE = -1;
  bool HAS_DATA = false;

  float variance_from_offset(COMPASS_XYZ Offset, bool &Good_Data_Count);
  // If Quadrant = -1 then no stick the landing is performed.

  bool stick_the_landing(COMPASS_XYZ Current_Offset, int Quadrant);

  void clear();
};

// -------------------------------------------------------------------------------------

class CAL_LEVEL_2
{
  private:
  
  // Simple calibration
  bool MIN_MAX_HAS_DATA = false;

  bool SIMPLE_CALIBRATION = true;
  
  MIN_MAX_SIMPLE X_MIN_MAX;
  MIN_MAX_SIMPLE Y_MIN_MAX;
  MIN_MAX_SIMPLE Z_MIN_MAX;

  float SKEW_X = 0;
  float SKEW_Y = 0;

  COMPASS_XYZ OFFSET;
  
  //       A              |skew y
  //    AC   AD           |
  //  C         D   ------|------ skew x
  //    BC   BD           |
  //       B              |

  int QUAD      = -1;
  int QUAD_PREV = -1;
  // 0 = A
  // 1 = D
  // 2 = B
  // 3 = C

  bool COMPLETE_QUAD_DATA_SET = false;
  float DISTANCE_VARIANCE_FULL = -1;

  COMPASS_XYZ A_Cal_Pt_PRELOAD;
  COMPASS_XYZ B_Cal_Pt_PRELOAD;
  COMPASS_XYZ C_Cal_Pt_PRELOAD;
  COMPASS_XYZ D_Cal_Pt_PRELOAD;
  float A_Cal_Var_PRELOAD;
  float B_Cal_Var_PRELOAD;
  float C_Cal_Var_PRELOAD;
  float D_Cal_Var_PRELOAD;

  int get_quad(COMPASS_XYZ &Raw_XYZ, float Distance);

  public:

  CALIBRATION_DATA A;
  CALIBRATION_DATA C;
  CALIBRATION_DATA D;
  CALIBRATION_DATA B;
  CALIBRATION_DATA ACTIVE_QUAD_DATA;

  void calibration_preload(COMPASS_XYZ A_Cal_Pt, float A_Cal_Var, 
                            COMPASS_XYZ B_Cal_Pt, float B_Cal_Var, 
                            COMPASS_XYZ C_Cal_Pt, float C_Cal_Var, 
                            COMPASS_XYZ D_Cal_Pt, float D_Cal_Var);

  void calibration_preload_set();

  MIN_MAX_SIMPLE x_min_max();
  MIN_MAX_SIMPLE y_min_max();
  MIN_MAX_SIMPLE z_min_max();

  COMPASS_XYZ offset();

  float skew_x();
  float skew_y();

  float variance();
  bool simple_calibration();
  
  void clear();

  void calibration_level_2(COMPASS_XYZ &Raw_XYZ);
  // Run Level 2 cal routines.

};

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

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

class HMC5883L
{
  private:

  int DEVICE = 0;                 // For file IO.

  unsigned char BUFFER[16];

  // Data

  COMPASS_XYZ RAW_XYZ;            // Temporary storage of XYZ before process;

  vector<COMPASS_XYZ> RAW_POINTS;     // History of data received.
  vector<float> CALIBRATED_BEARINGS;  // History of calculated bearings

  float RAW_BEARING = 0;              // Recent Bearing
  float BEARING = 0;
  float BEARING_JITTER_MIN = 0;
  float BEARING_JITTER_MAX = 0;

  // Calibration
  void add_point(COMPASS_XYZ Point);
  bool CALIBRATE = false;
  bool CALIBRATE_LOCK = false;
  int CURRENT_CALIBRATION_LEVEL = 0;

  TIMED_PING DATA_RECIEVED_TIMER;
  TIMED_PING CYCLE_TIMER;
  int CYCLE = 99;     // Connection cycles, independent of io
    // -1 - Ignore Cycles, autoconnect is off.
    //  0 - Normal connected read write cycle mode
    //  1 - Shutdown Cycle
    //  2 - Starting Up Cycle

  int CYCLE_CHANGE = 99;  // Internal: for controling start stop and autostart.

  float KNOWN_DEVICE_DEGREE_OFFSET_PRELOAD = 0.0f;
  float KNOWN_DEVICE_DEGREE_OFFSET = 0.0f;   // If device is mounted offset to  
                                              //  true forward facing, adjust compass 
                                              //  bearing to reflect

  // Preload Calibration Data
  bool PRELOAD_DATA_LOADED = false;

  bool CONNECTED = false;                     // Set to true if connected.

  // Calibration Variables and Routines
  TIMED_PING CALIBRATION_SPOT_CHECK;

  //CAL_LEVEL_1 LEVEL_1;
  CAL_LEVEL_2 LEVEL_2;

  // Comms Routines
  bool register_write(char Register, char Value);
    // Internal: Change chip settings.

  bool create();      // Internal: Opens port for access.

  void stop();        // Internal: Closes port for access.
                      //  Not yet fully implemented.

  COMPASS_XYZ calculate_calibrated_xyz(COMPASS_XYZ &Raw_XYZ);

  // Process
  void process();     // Internal: Processes most recent received data. 
                      // Performs Calibration Routines

  public:

  HMC5883L_PROPERTIES PROPS;

  int raw_points_size();              // Returns size of xyz history.

  COMPASS_XYZ raw_xyz();              // Return most recent raw xyz;
  COMPASS_XYZ raw_xyz(int Position);  // Returns raw xyz data from history

  COMPASS_XYZ calibrated_xyz();             // Return most recent xyz data with the current calibratons.
  COMPASS_XYZ calibrated_xyz(int Position); // Returns xyz data from history with the current calibratons.

  void calibration_preload(COMPASS_XYZ A_Cal_Pt, float A_Cal_Var, 
                            COMPASS_XYZ B_Cal_Pt, float B_Cal_Var, 
                            COMPASS_XYZ C_Cal_Pt, float C_Cal_Var, 
                            COMPASS_XYZ D_Cal_Pt, float D_Cal_Var, 
                            float Cal_Offset);

  void calibration_preload_set();

  int current_calibration_level();
  // Not yet implemented
  //  0 - No calibration Done
  //  1 - Calibration of MIN MAX in progress.

  void calibrateion_reset();
  // Removes all calibration data

  void calibrate_toggle();
  // Start / Stop Display Calibration

  void calibrate_lock_toggle();
  // Start / Stop Calibration

  bool calibrate_on();
  // Returns true if calibration is displayed.

  bool calibrate_lock_on();
  // Returns true if calibration in progress.

  // calibration

  MIN_MAX_SIMPLE calibration_min_max_x();
  MIN_MAX_SIMPLE calibration_min_max_y();
  MIN_MAX_SIMPLE calibration_min_max_z();
  COMPASS_XYZ calibration_offset();

  CAL_LEVEL_2_QUAD_RECORD calibration_points_active_quad_data();
  bool calibration_points_active_quad_overflow();
  COMPASS_XYZ calibration_max_coord_a();
  COMPASS_XYZ calibration_max_coord_b();
  COMPASS_XYZ calibration_max_coord_c();
  COMPASS_XYZ calibration_max_coord_d();
  float calibration_known_var_a();
  float calibration_known_var_b();
  float calibration_known_var_c();
  float calibration_known_var_d();
  float calibration_variance();
  bool calibration_simple();

  void calibration_preload(COMPASS_XYZ A_Cal_Pt, float A_Cal_Var, 
                            COMPASS_XYZ B_Cal_Pt, float B_Cal_Var, 
                            COMPASS_XYZ C_Cal_Pt, float C_Cal_Var, 
                            COMPASS_XYZ D_Cal_Pt, float D_Cal_Var);

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

  void bearing_known_offset_calibration(float Known_Bearing);

  float bearing();
  // Direction Facing.

  float bearing_jitter_min();
  float bearing_jitter_max();
};


#endif