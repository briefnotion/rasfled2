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
#include "definitions.h"
#include "fled_time.h"
#include "helper.h"
#include "json_interface.h"
#include "nmea.h"

// -------------------------------------------------------------------------------------

// Definitions

/*
Address   Name  
    00    Configuration Register A    Read/Write
    01    Configuration Register B    Read/Write
    02    Mode Register               Read/Write
    03    Data Output X MSB Register  Read
    04    Data Output X LSB Register  Read
    05    Data Output Z MSB Register  Read
    06    Data Output Z LSB Register  Read
    07    Data Output Y MSB Register  Read
    08    Data Output Y LSB Register  Read
    09    Status Register Read
    10    Identification Register A   Read
    11    Identification Register B   Read
    12    Identification Register C   Read
*/

// Register A (CR A)
 
// Sample Average
// CRA6-CRA5
//  MA1-MA0
//  00 = 1(Default); 01 = 2; 10 = 4; 11 = 8
#define   HMC_MA_1          0x00  // Sample of 1  (default)
#define   HMC_MA_2          0x01  // Sample average of 2
#define   HMC_MA_4          0x02  // Sample average of 4
#define   HMC_MA_8          0x03  // Sample average of 8

// Data Rates (hertz)
// CRA4-CRA2
//  DO2-DO0
#define   HMC_DO_0_75       0x00  // Output rate of 0.75 Hz
#define   HMC_DO_1_5        0x01  // Output rate of 1.5 Hz
#define   HMC_DO_3          0x02  // Output rate of 3 Hz
#define   HMC_DO_7_5        0x03  // Output rate of 7.5 Hz
#define   HMC_DO_15         0x04  // Output rate of 15 Hz  (default)
#define   HMC_DO_30         0x05  // Output rate of 30 Hz
#define   HMC_DO_75         0x06  // Output rate of 75 Hz

// Measurement Bias
// CRA1-CRA0
//  MS1-MS0
#define   HMC_MS_NORM       0x00  // Normal Measurtement configuration  (default)
#define   HMC_MS_POS_BIAS   0x01  // Positive current fored across sisistive loads of all axis.
#define   HMC_MS_NEG_BIAS   0x02  // Negative current fored across sisistive loads of all axis.


// Register B (CR B)

// Gain (GAUSS) - Choose lower gain when field streangth causes overflow.
//                Range: -2048 – 2047
// CRB7-CRB5
//  GN2-GN0
#define   HMC_GN_1370       0x00  // 1370 Gauss
#define   HMC_GN_1090       0x01  // 1090 Gauss  (default)
#define   HMC_GN_820        0x02  // 820 Gauss
#define   HMC_GN_660        0x03  // 660 Gauss
#define   HMC_GN_440        0x04  // 440 Gauss
#define   HMC_GN_390        0x05  // 390 Gauss
#define   HMC_GN_330        0x06  // 330 Gauss
#define   HMC_GN_230        0x07  // 230 Gauss

// Keep clear for correct operation
// CRB4-CRB0


// Mode Register (MR)

// High Speed I2C, 3400kHz
// MR7-MR2
//  HS

// Mode Select
// MR1-MR0
//  MD1-MD0
#define   HMC_MD_CONT_MEAS  0x00  // Continuous Measurement Mode
#define   HMC_MD_SING_MEAS  0x01  // Single Measurement Mode
#define   HMC_MD_IDLE_1     0x02  // Devgice is placed in idle mode
#define   HMC_MD_IDLE_2     0x03  // Devgice is placed in idle mode


// Status Register
// SR1-SR0

// SR1 - Lock
// SR0 - Ready


// Identfication Register
// IRA, IRB, IRC
// ASCII Chip MANUF ID


// -------------------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------------------------------

float dist(float X, float Y);

void calc_offset_and_skew(bool Simple, FLOAT_XYZ Top, FLOAT_XYZ Right, FLOAT_XYZ Bot, FLOAT_XYZ Left, 
                          FLOAT_XYZ &Ret_Offset, FLOAT_XYZ &Ret_Skew);

FLOAT_XYZ calculate_calibrated_xyz(FLOAT_XYZ &Raw_XYZ, FLOAT_XYZ Offset, FLOAT_XYZ Skew);

// -------------------------------------------------------------------------------------

class COMPASS_XY
{
  public:

  float X = 0;
  float Y = 0;
};

// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------

bool four_point_check(FLOAT_XYZ Top, FLOAT_XYZ Bottom, 
                      FLOAT_XYZ Left, FLOAT_XYZ Right);

// -------------------------------------------------------------------------------------

class CAL_LEVEL_2_QUAD_RECORD
{
  private:

  int VARIANCE_COLLECTION_SIZE_SIMPLE = 60 * COMMS_COMPASS_POLLING_RATE_MS;
  int VARIANCE_COLLECTION_SIZE_COMPLEX = 30 * COMMS_COMPASS_POLLING_RATE_MS;
  // To assist with first round calibration times, the larger 
  //  VARIANCE_COLLECTION_SIZE_SIMPLE will be the collection size, 
  //  and the system will return to VARIANCE_COLLECTION_SIZE_COMPLEX 
  //  collection size, for a greater refined, accuracy after simple 
  //  calibration isn't active.

  int VARIANCE_COLLECTION_SIZE = 0.0f;
  // Restriting calibration to occur only if 180 degree turned happened
  //  within the numeber multiplied of seconds, as default.
  // Whatever this value is, will be replaced with either 
  // 

  public:

  bool OVERFLOW = false;

  vector<FLOAT_XYZ> DATA_POINTS;
  vector<FLOAT_XYZ> DATA_POINTS_CALIBRATED;

  void add_point(FLOAT_XYZ &Raw_XYZ);

  void clear(bool Simple_Calibration);
};

class CALIBRATION_DATA
{
  private:

  int OFFSET_POINT_VECTOR_SIZE = 45;

  public:

  CAL_LEVEL_2_QUAD_RECORD QUAD_DATA;
  
  //FLOAT_XYZ COORD;

  FLOAT_XYZ OFFSET_POINT;               // Stores Offset point value after ER 
                                        //  cycle. Refer to OFFSET_POINT when 
                                        //  building skew and boundaries. 

  FLOAT_XYZ LAST_KNOWN_OFFSET_POINT;    // Stores the Offset point while it is being
                                        //  considered to be added to the list. 

  vector<FLOAT_XYZ> OFFSET_POINT_LIST;  // Stores a list of previous Offset point

  float VARIANCE = 1000;            // The Good Variance value. Copied from 
                                    //  LAST_KNOWN_VARIANCE when copy made.
  float LAST_KNOWN_VARIANCE = 1000; // Stores Variance from the previous 
                                    //  variance_from_offset() pass
  bool HAS_DATA = false;

  void add_point_to_offset_point_list(FLOAT_XYZ Point);
  // Adds new offset point to list and resizes if necessary.

  void calculate_offset_point();
  // Calculates the best offset point from the OFFSET_POINT_LIST and 
  //  saves it in OFFSET_POINT;

  void add_last_known_offset_point();
  // Takes the LAST_KNOWN_OFFSET_POINT and puts it in the OFFSET_POINT_LIST, then 
  //  calculates the best offset point and saves it in OFFSET_POINT;

  float variance_from_offset(FLOAT_XYZ Offset, FLOAT_XYZ Skew, bool &Good_Data_Count);
  // If Quadrant = -1 then no stick the landing is performed.

  bool stick_the_landing(FLOAT_XYZ Current_Offset, int Quadrant);
  // Reviews all the points in QUAD_DATA, and finds the best point to represent the 
  //  offset point and saves it in LAST_KNOWN_OFFSET_POINT

  void clear(bool Simple_Calibration);
};

// -------------------------------------------------------------------------------------

class CALIBRATION_DATA_ALTERNATIVE
{
  public:

  MIN_MAX_TIME_SLICE X_MIN_MAX;
  MIN_MAX_TIME_SLICE Y_MIN_MAX;
  MIN_MAX_TIME_SLICE Z_MIN_MAX;
};



struct Vector3 
{
  float X, Y, Z;
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

  FLOAT_XYZ OFFSET;
  FLOAT_XYZ SKEW;
  
  //           |skew y
  //           |
  //     ------|------ skew x
  //           |
  //           |

  int QUAD      = -1;
  int QUAD_PREV = -1;

  bool COMPLETE_QUAD_DATA_SET = false;
  float DISTANCE_VARIANCE_FULL = -1.0f;

  FLOAT_XYZ Cal_Pt_PRELOAD_1;
  FLOAT_XYZ Cal_Pt_PRELOAD_2;
  FLOAT_XYZ Cal_Pt_PRELOAD_3;
  FLOAT_XYZ Cal_Pt_PRELOAD_4;
  float Cal_Var_PRELOAD_1 = 0.0f;
  float Cal_Var_PRELOAD_2 = 0.0f;
  float Cal_Var_PRELOAD_3 = 0.0f;
  float Cal_Var_PRELOAD_4 = 0.0f;

  // Offset Point History
  bool OFFSET_HISTORY_CHANGED = false;
  TIMED_PING OFFSET_HISTORY_TIMER;

  public:

  string OFFSET_HISTORY_FILENAME = "";

  vector<CALIBRATION_DATA> CALIBRATION_QUADS;

  //       1                |skew y
  //    --   --             |
  //  4         2     ------|------ skew x
  //    --   --             |
  //       3    0 - Active  |

  // CALIBRATION_DATA A;
  // CALIBRATION_DATA C;
  // CALIBRATION_DATA D;
  // CALIBRATION_DATA B;
  // CALIBRATION_DATA ACTIVE_QUAD_DATA;
  
  int BEARING_OFFSET_LOAD = -1;

  private:

  int get_quad(FLOAT_XYZ &Raw_XYZ, float Distance);

  bool XYZ_MIN_MAX(FLOAT_XYZ &Raw_XYZ, bool &Has_Data, MIN_MAX_SIMPLE &Xmm, MIN_MAX_SIMPLE &Ymm, MIN_MAX_SIMPLE &Zmm);

  float calc_all_quad_variance(int Swap_0_Quad_With, bool &Ret_Good_Data_Count_Pass);

  void build_calibration_display_data();

  void build_non_simple_offsets();

  bool offset_history_read();
  void offset_history_write();

  public:



        // Testing ----------------------------------------


        
  
  // Alternative method to calculate offset and skew
  //vector<CALIBRATION_DATA_ALTERNATIVE> CALIBRATION_DATA_A;

  //size_t CALIBRATION_DATA_A_DATA_SIZE = 20;
  //FLOAT_XYZ CALIBRATION_DATA_A_CENTER;

  std::vector<Vector3> calibrationData;
  const size_t MAX_DATA_POINTS = 1000;
  Vector3 center;
  Vector3 calibrated_reading;
  float heading = 0.0f;

  Vector3 calibrateReading(const Vector3& raw, const Vector3& center);
  void removeNonExtremes();
  Vector3 computeCalibrationOffsets();



        // Testing ----------------------------------------


        

  void calibration_preload(FLOAT_XYZ Cal_Pt_1, float Cal_Var_1, 
                            FLOAT_XYZ Cal_Pt_2, float Cal_Var_2, 
                            FLOAT_XYZ Cal_Pt_3, float Cal_Var_3, 
                            FLOAT_XYZ Cal_Pt_4, float Cal_Var_4);

  void calibration_preload_set();

  MIN_MAX_SIMPLE x_min_max();
  MIN_MAX_SIMPLE y_min_max();
  MIN_MAX_SIMPLE z_min_max();

  FLOAT_XYZ offset();
  FLOAT_XYZ skew();
  
  void clear();

  float variance();
  bool simple_calibration();

  void calibration_level_2(unsigned long tmeFrame_Time, FLOAT_XYZ &Raw_XYZ);
  // Run Level 2 cal routines.

};

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

class HMC5883L_PROPERTIES
{
  public:

  bool AUTOSTART =  false;

  bool CALIBRATION_LOCK_AT_START = false;

  // Comm port settings
  string PORT = "";
  unsigned char I2C_ID;

  bool CONTINUOUS_DATA =  false;
  // If true, and data stops for time, will shut down port and 
  //  fall back into autostart mode. // No data is recognized as 
  //  a disconnect, resulting in autoconnet starting again. 
  // If false, port will stay open even if no data is being seen.
  //  No checks for hardware disconnects.

  string OFFSET_HISTORY_FILE_NAME = "";
};

class HMC5883L
{
  private:

  int DEVICE = 0;                 // For file IO.

  unsigned char BUFFER[16];

  // Data

  FLOAT_XYZ RAW_XYZ;            // Temporary storage of XYZ before process;

  int CALIBRATED_BEARINGS_SIZE = 31;  // Calculated by: COMMS_COMPASS_HISTORY_TIME_SPAN_MS / 
                                      //                COMMS_COMPASS_POLLING_RATE_MS
  // CALIBRATED_BEARINGS_SIZE stores a history of calculated bearing, to 
  //  be average to return the indicated bearing.

  vector<float> CALIBRATED_BEARINGS;  // History of calculated bearings

  float RAW_BEARING = 0;              // Recent Bearing
  float BEARING = 0;
  float BEARING_JITTER_MIN = 0;
  float BEARING_JITTER_MAX = 0;

  // Calibration
  void add_point(FLOAT_XYZ Point);
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

  // GPS Automatic Heading Calibration Timer
  TIMED_PING GPS_HEADING_CALIBRATION_TIMER;

  // Preload Calibration Data
  bool PRELOAD_DATA_LOADED = false;

  bool CONNECTED = false;                     // Set to true if connected.

  // Comms Routines
  bool register_write(char Register, char Value);
    // Internal: Change chip settings.

  bool create(string Offset_History_Filename);      // Internal: Opens port for access.

  void stop();        // Internal: Closes port for access.
                      //  Not yet fully implemented.

  // Process
  void process(NMEA &GPS_System, unsigned long tmeFrame_Time);     
  // Internal: Processes most recent received data. 
  // Performs Calibration Routines

  public:

  HMC5883L_PROPERTIES PROPS;

  CAL_LEVEL_2 LEVEL_2;

  void calibration_preload(FLOAT_XYZ Cal_Pt_1, float Cal_Var_1, 
                            FLOAT_XYZ Cal_Pt_2, float Cal_Var_2, 
                            FLOAT_XYZ Cal_Pt_3, float Cal_Var_3, 
                            FLOAT_XYZ Cal_Pt_4, float Cal_Var_4, 
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

  bool calibrate_on();
  // Returns true if calibration is displayed.

  void calibrate_lock_toggle();
  // Start / Stop Calibration

  bool calibrate_lock_on();
  // Returns true if calibration in progress.

  // calibration

  MIN_MAX_SIMPLE calibration_min_max_x();
  MIN_MAX_SIMPLE calibration_min_max_y();
  MIN_MAX_SIMPLE calibration_min_max_z();
  FLOAT_XYZ calibration_offset();

  CAL_LEVEL_2_QUAD_RECORD calibration_points_active_quad_data();
  bool calibration_points_active_quad_overflow();

  float calibration_variance();
  bool calibration_simple();

  void calibration_preload(FLOAT_XYZ A_Cal_Pt, float A_Cal_Var, 
                            FLOAT_XYZ B_Cal_Pt, float B_Cal_Var, 
                            FLOAT_XYZ C_Cal_Pt, float C_Cal_Var, 
                            FLOAT_XYZ D_Cal_Pt, float D_Cal_Var);

  bool connected();
  // Returns true if hmc5883l is successfully connected.
  //  Will assume data is being received.

  bool cycle(NMEA &GPS_System, unsigned long tmeFrame_Time);
  // This is the main update loop.

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
  float bearing_known_offset();

  float bearing();
  // Direction Facing.

  float test_heading();

  float bearing_jitter_min();
  float bearing_jitter_max();
};


#endif