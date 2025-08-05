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
#include <iomanip>   // For std::fixed, std::setprecision

// RASFled related header files
#include "definitions.h"
#include "fled_time.h"
#include "helper.h"
#include "rasapi.h"
#include "nmea.h"
#include "json_interface.h"
#include "globe_helper.h"


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
//  CRB4-CRB0


// Mode Register (MR)

// High Speed I2C, 3400kHz
//  MR7-MR2
//  HS

// Mode Select
//  MR1-MR0
//  MD1-MD0
#define   HMC_MD_CONT_MEAS  0x00  // Continuous Measurement Mode
#define   HMC_MD_SING_MEAS  0x01  // Single Measurement Mode
#define   HMC_MD_IDLE_1     0x02  // Device is placed in idle mode
#define   HMC_MD_IDLE_2     0x03  // Device is placed in idle mode


// Status Register
//  SR1-SR0

//  SR1 - Lock
//  SR0 - Ready

// Identfication Register
//  IRA, IRB, IRC
//  ASCII Chip MANUF ID


// -------------------------------------------------------------------------------------

using namespace std;

// -------------------------------------------------------------------------------------

class FLOAT_XYZ_MATRIX
{
  public:
  float X = 0.0f;
  float Y = 0.0f;
  float Z = 0.0f;

  // Default constructor
  FLOAT_XYZ_MATRIX() : X(0.0f), Y(0.0f), Z(0.0f) {}

  // Parameterized constructor
  FLOAT_XYZ_MATRIX(float x, float y, float z) : X(x), Y(y), Z(z) {}

  // Overload for addition
  FLOAT_XYZ_MATRIX operator+(const FLOAT_XYZ_MATRIX& other) const 
  {
    return FLOAT_XYZ_MATRIX(X + other.X, Y + other.Y, Z + other.Z);
  }

  // Overload for subtraction
  FLOAT_XYZ_MATRIX operator-(const FLOAT_XYZ_MATRIX& other) const 
  {
    return FLOAT_XYZ_MATRIX(X - other.X, Y - other.Y, Z - other.Z);
  }

  // Overload for scalar multiplication
  FLOAT_XYZ_MATRIX operator*(float scalar) const 
  {
    return FLOAT_XYZ_MATRIX(X * scalar, Y * scalar, Z * scalar);
  }

  // Overload for scalar division
  FLOAT_XYZ_MATRIX operator/(float scalar) const 
  {
    if (scalar != 0.0f) 
    {
      return FLOAT_XYZ_MATRIX(X / scalar, Y / scalar, Z / scalar);
    }
    return *this; // Avoid division by zero, return current state
  }

  // Dot product
  float dot(const FLOAT_XYZ_MATRIX& other) const 
  {
    return X * other.X + Y * other.Y + Z * other.Z;
  }

  // Magnitude squared
  float magSq() const 
  {
    return X*X + Y*Y + Z*Z;
  }

  // Magnitude
  float mag() const 
  {
    return std::sqrt(magSq());
  }
};

// -------------------------------------------------------------------------------------

/**
 * @brief Represents a 3x3 matrix for linear transformations.
 * Used for soft iron correction.
 */
class Matrix3x3 
{
  public:
  float m[3][3];

  // Default constructor (identity matrix)
  Matrix3x3() 
  {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
  }

  // Constructor from values
  Matrix3x3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22) 
  {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
  }

  // Matrix-Vector multiplication (M * V)
  FLOAT_XYZ_MATRIX operator*(const FLOAT_XYZ_MATRIX& v) const 
  {
    return FLOAT_XYZ_MATRIX(
      m[0][0] * v.X + m[0][1] * v.Y + m[0][2] * v.Z,
      m[1][0] * v.X + m[1][1] * v.Y + m[1][2] * v.Z,
      m[2][0] * v.X + m[2][1] * v.Y + m[2][2] * v.Z
    );
  }

  // Matrix-Matrix multiplication (M1 * M2)
  Matrix3x3 operator*(const Matrix3x3& other) const 
  {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) 
    {
      for (int j = 0; j < 3; ++j) 
      {
        result.m[i][j] = 0.0f;
        for (int k = 0; k < 3; ++k) 
        {
          result.m[i][j] += m[i][k] * other.m[k][j];
        }
      }
    }
    return result;
  }

  // Transpose of the matrix
  Matrix3x3 transpose() const 
  {
    return Matrix3x3(
      m[0][0], m[1][0], m[2][0],
      m[0][1], m[1][1], m[2][1],
      m[0][2], m[1][2], m[2][2]
    );
  }

  // Determinant of the matrix
  float determinant() const 
  {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
            m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
            m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
  }

  // Inverse of the matrix (using adjugate method for 3x3)
  Matrix3x3 inverse() const 
  {
    float det = determinant();
    if (std::abs(det) < std::numeric_limits<float>::epsilon()) 
    {
      std::cerr << "Warning: Matrix is singular, cannot invert. Returning identity." << std::endl;
      return Matrix3x3(); // Return identity or handle error appropriately
    }

    Matrix3x3 inv;
    inv.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / det;
    inv.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / det;
    inv.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
    inv.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / det;
    inv.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
    inv.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / det;
    inv.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / det;
    inv.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / det;
    inv.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / det;
    return inv;
  }

  // Scalar multiplication
  Matrix3x3 operator*(float scalar) const 
  {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) 
    {
      for (int j = 0; j < 3; ++j) 
      {
        result.m[i][j] = m[i][j] * scalar;
      }
    }
    return result;
  }
};

// -------------------------------------------------------------------------------------

class COMPASS_POINT
{
  public:
  FLOAT_XYZ_MATRIX  POINT;          // The actual XYZ coordinates of the compass reading

  // Default constructor to initialize POINT
  COMPASS_POINT() : POINT() {}
  // Constructor to initialize POINT with XYZ values
  COMPASS_POINT(float x, float y, float z) : POINT(x, y, z) {}
};
 
// -------------------------------------------------------------------------------------

// Structure to hold point index and its angle for sorting.
struct PointAngle 
{
  float angle;
  size_t original_index;
};
 
// -------------------------------------------------------------------------------------

/**
 * @brief Stores the calculated calibration parameters.
 * Hard iron offset corrects for magnetic biases.
 * Soft iron matrix corrects for axis distortions (makes ellipse/ellipsoid spherical).
 */
struct CalibrationParameters 
{
  FLOAT_XYZ_MATRIX hard_iron_offset;
  Matrix3x3 soft_iron_matrix;
  float average_field_magnitude; // Added back for dynamic range
  CalibrationParameters() : hard_iron_offset(0,0,0), soft_iron_matrix(), average_field_magnitude(0.0f) {} // Initialize new member
};

// -------------------------------------------------------------------------------------

FLOAT_XYZ_MATRIX fake_compass_input(unsigned long tmeFrame_Time, float &True_Fake_Bearing);
// Generates a fake compass input for testing purposes.

// -------------------------------------------------------------------------------------

class HMC5883L_PROPERTIES
{
  public:

  bool AUTOSTART =  false;

  bool CALIBRATION_ENABLED = true;

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

  float CALIBRATION_MOUNT_OFFSET = -180.0f;
  float CALIBRATION_LOCATION_DECLINATION = 0.0f;
  bool GPS_ASSIST_HEADING = true;

  bool ENABLE_FAKE_COMPASS = false;
};

class PRESERVE_ANGLE_CALC
{
  private:
  int               COUNT = 0;
  FLOAT_XYZ_MATRIX  POINT_SUM;

  public:
  int count();
  void add(FLOAT_XYZ_MATRIX Add_Point);
  FLOAT_XYZ_MATRIX average();
  void clear();
};

// -------------------------------------------------------------------------------------

/**
 * @brief Helper function to perform a 5x5 matrix inversion.
 * A standard Gaussian elimination with partial pivoting is used.
 * @param matrix The 5x5 matrix to invert.
 * @return The inverted 5x5 matrix.
 */
std::vector<std::vector<float>> invert5x5(const std::vector<std::vector<float>>& matrix);

// -------------------------------------------------------------------------------------


class CAL_LEVEL_3
{
  private:
  void save_history_and_settings(HMC5883L_PROPERTIES &Props);
  // Save history and settings

  bool xyz_equal(FLOAT_XYZ_MATRIX &A, FLOAT_XYZ_MATRIX &B);
  // Checks if two FLOAT_XYZ points are equal.

  float dist_xyz(FLOAT_XYZ_MATRIX &A, FLOAT_XYZ_MATRIX &B);
  // Calculates the distance between two points in 3D space.

  // Functions to analyze the points in the history.
  void clear_all_flags();
  bool add_point(FLOAT_XYZ_MATRIX &Raw_XYZ);

  PRESERVE_ANGLE_CALC preserved_angle_buffer[360];
  void                preservation_of_data();
  int                 preservation_of_data_buffer_size = 3;

  int COMPASS_HISTORY_SIZE              = 60;
  int COMPASS_CALIBRATION_HISTORY_SIZE  = 360;

  // Constants for noise filtering
  float CLOSEST_ALLOWED = 3.0f;
  float NOISE_FILTER_DISTANCE = 40.0f;

  // Amount of time that is waited before the compass runs the calibration routines.
  TIMED_IS_READY  CALIBRATION_TIMER;
  int             CALIBRATION_DELAY = 1000;

  // ---
  // Complex Calibration functions.
  // Core logic and data structures for magnetometer calibration.

  // Stores the currently active hard and soft iron calibration parameters.
  CalibrationParameters current_calibration_params;

  // --- Internal Buffers for Optimized Calibration Calculations ---
  // These are pre-allocated class members to minimize dynamic memory operations.

  // Buffer for filtered magnetic points used in ellipsoid fitting.
  std::vector<FLOAT_XYZ_MATRIX> active_points;

  // Left-hand side matrix (A^T * A) for the least-squares ellipsoid fit.
  std::vector<std::vector<float>> A_transpose_A;

  // Right-hand side vector (A^T * b) for the least-squares ellipsoid fit.
  std::vector<float> A_transpose_b;

  // Temporary buffer for a single point's features during ellipsoid fitting.
  std::vector<float> features_buffer; 

  // --- Calibration Core Functions ---

  // Performs a 3-parameter (diagonal) ellipsoid fit to calculate hard and soft iron parameters.
  // Returns true on success.
  bool fit_ellipsoid_and_get_calibration_matrix(
    const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history,
    CalibrationParameters& params);

  std::vector<std::vector<float>> A_transpose_A_2D;
  std::vector<float> A_transpose_b_2D;
  std::vector<float> features_buffer_2D;
  // other private members from original code
    
  bool fit_ellipse_and_get_calibration_matrix_2D(
    const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history,
    CalibrationParameters& params);

  // Orchestrates the hard and soft iron calibration process.
  CalibrationParameters perform_hard_soft_iron_calibration(const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history);

  // --- Heading Calculation and Reporting Functions ---

  // Calculates calibrated heading from raw magnetic data using current parameters (no tilt compensation).
  void calculate_calibrated_heading(const FLOAT_XYZ_MATRIX& raw_point, const CalibrationParameters& params);

  // Prepares and formats the calibrated heading for external reporting or display.
  void set_heading_degrees_report(const FLOAT_XYZ_MATRIX& Raw_XYZ);

  // ---
  
  public:
  string INFORMATION_CALIBRATION = "";

  VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT> COMPASS_HISTORY;
  // Stores a history of compass points, using a non-sequential vector deque.
  
  VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT> COMPASS_CALIBRATION_HISTORY;
  // Stores the Average of the points, by radial, from the COMPASS_HISTORY.

  FLOAT_XYZ_MATRIX COMPASS_CENTER;
  // The center of the compass, calculated

  float HEADING_DEGREES_REPORT                = 0.0f;
  float HEADING_DEGREES_REPORT_NON_CALIBRATED = 0.0f;
  // The heading degrees report, calculated based on the compass center and points.
  
  void clear();
  //float variance();
  //bool simple_calibration();

  void calibration_level_3(unsigned long tmeFrame_Time, FLOAT_XYZ_MATRIX &Raw_XYZ, bool Enable_Calibration_Routines);
  // Run Level 2 cal routines.
};

// -------------------------------------------------------------------------------------

class HMC5883L
{
  private:

  int DEVICE = 0;                 // For file IO.

  unsigned char BUFFER[16];

  int CALIBRATED_BEARINGS_SIZE = 31;  // Calculated by: COMMS_COMPASS_HISTORY_TIME_SPAN_MS / 
                                      //                COMMS_COMPASS_POLLING_RATE_MS

  vector<float> CALIBRATED_BEARINGS;  // History of calculated bearings

  float RAW_BEARING             = 0;  // Recent Bearing
  float BEARING                 = 0;
  float BEARING_NON_CALIBRATED  = 0;
  float BEARING_JITTER_MIN      = 0;
  float BEARING_JITTER_MAX      = 0;

  // Calibration
  void add_point(FLOAT_XYZ_MATRIX Point);
  bool CALIBRATE = false;
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

  // Amount of time that is waited before the system stores its calibration history
  // to file.
  TIMED_IS_READY  CALIBRATION_DATA_SAVE;
  int             CALIBRATION_DATA_SAVE_DELAY = 60000;

  // Preload Calibration Data
  bool PRELOAD_DATA_LOADED = false;

  bool CONNECTED = false;                     // Set to true if connected.

  MIN_MAX_TIME_SLICE GPS_ERROR_MEAN;

  public:

  // Load history and settings
  // Creates JSON file with saved settings.
  void load_history_and_settings();
  void save_history_and_settings();

  // Comms Routines
  bool register_write(char Register, char Value);
    // Internal: Change chip settings.

  bool create();      // Internal: Opens port for access.

  void stop();        // Internal: Closes port for access.
                      //  Not yet fully implemented.

  // Process
  void process(NMEA &GPS_System, unsigned long tmeFrame_Time);
  // Internal: Processes most recent received data. 
  // Performs Calibration Routines

  FLOAT_XYZ_MATRIX RAW_XYZ_PREVIOUS_VALUE;
  
  float TRUE_FAKE_BEARING;

  public:

  FLOAT_XYZ_MATRIX RAW_XYZ;
  // Most Recent XYZ coords from compass.  
  // Useful for drawing on 2d plane.

  // When using fake compass, holds the unmodified bearing.

  HMC5883L_PROPERTIES PROPS;

  CAL_LEVEL_3 LEVEL_3;

  string INFORMATION = "";

  void calibrateion_reset();
  // Removes all calibration data

  void calibrate_toggle();
  // Start / Stop Display Calibration

  bool calibrate_on();
  // Returns true if calibration is displayed.

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

  void bearing_known_offset_calibration_to_gps();
  void bearing_known_offset_clear();
  float bearing_known_offset();

  float accumulated_gps_to_compass_bearing_error();

  float bearing_calibrated();
  float bearing_non_calibrated();
  float bearing_non_true_fake();
  // Direction Facing.

  float bearing_jitter_min();
  float bearing_jitter_max();
};


#endif