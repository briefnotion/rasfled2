// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: hmc5883l.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef HMC5883L_CPP
#define HMC5883L_CPP

#include "hmc5883l.h"

using namespace std;

// -------------------------------------------------------------------------------------

FLOAT_XYZ CAL_LEVEL_3::fake_compass_input(unsigned long tmeFrame_Time)
{
  FLOAT_XYZ ret_point;

  int speed = 10;  // 1 fastest

  //simulations
  bool noise = false;
  float noise_factor = 0.5f;

  bool sparatic_random = true;
  float sparatic_random_factor = 1.0f;
  int sparatic_random_chance_percent = 5; // 5% chance of sparatic random jump

  bool time_based_drift = false;
  float time_based_drift_factor = 100.0f;

  bool interference_zone = true;

  bool skew = true;
  //float skew_factor = 1.00f; // 100% skew
  float skew_factor = 0.50f; // 50% skew
  //float skew_factor = 0.10f; // 10% skew

  bool figure_eight = false; // if true, will do a figure eight pattern

  // parameters
  float radius = 250.0f;
  FLOAT_XYZ offset;
  offset.X = 50.0f;
  offset.Y = 100.3f;
  //offset.Z = 0.0f;

  // gen
  float angle = (float)((tmeFrame_Time/speed) % 360);

  angle = angle * (3.14159265f / 180.0f);

  if (figure_eight == false)
  {
    ret_point.X = radius * cos(angle) + offset.X;
    ret_point.Y = radius * sin(angle) + offset.Y;
    ret_point.Z = 0.0f + offset.Z;
  }
  else
  {
    ret_point.X = radius * sin(angle);
    ret_point.Y = radius * sin(angle) * cos(angle);
    ret_point.Z = 0.0f + offset.Z;
  }


  // simulations
  if (noise)
  {
    float noise_X = ((rand() % 100) - 50) * noise_factor; // -0.5 to +0.5
    float noise_Y = ((rand() % 100) - 50) * noise_factor; // -0.5 to +0.5
    ret_point.X += noise_X;
    ret_point.Y += noise_Y;
  }

  if (sparatic_random)
  {
    if (sparatic_random && (rand() % 100) < sparatic_random_chance_percent) // ~5% chance
    {
      ret_point.X += ((rand() % ((int)radius * 2)) - (int)radius) * sparatic_random_factor; // large jump
      ret_point.Y += ((rand() % ((int)radius * 2)) - (int)radius) * sparatic_random_factor;
    }
  }

  if (time_based_drift)
  {
    offset.X += time_based_drift_factor * sin(tmeFrame_Time * 0.01f);
    offset.Y += time_based_drift_factor * cos(tmeFrame_Time * 0.01f);
  }

  if (skew)
  {
    ret_point.X += skew_factor * ret_point.Y;
    ret_point.Y += skew_factor * ret_point.X;
  }
  
  if (interference_zone)
  {
    bool in_interference_zone = (tmeFrame_Time % 10000) < 2000; // 20% of the time
    if (in_interference_zone) 
    {
        ret_point.X += 50.0f * sin(tmeFrame_Time * 0.05f);
        ret_point.Y += 50.0f * cos(tmeFrame_Time * 0.05f);
    }
  }

  return ret_point;
}

bool CAL_LEVEL_3::xyz_equal(FLOAT_XYZ &A, FLOAT_XYZ &B)
{
  return (A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z);
}

float CAL_LEVEL_3::dist_xyz(FLOAT_XYZ &A, FLOAT_XYZ &B)
{
  return sqrtf(powf(A.X - B.X, 2) + powf(A.Y - B.Y, 2) + powf(A.Z - B.Z, 2));
}

void CAL_LEVEL_3::clear_all_flags()
{
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = false;

    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      COMPASS_HISTORY[pos].X_LOWER = false;
      COMPASS_HISTORY[pos].X_UPPER = false;
      COMPASS_HISTORY[pos].Y_LOWER = false;
      COMPASS_HISTORY[pos].Y_UPPER = false;

      COMPASS_HISTORY[pos].X_LOWER_M = false;
      COMPASS_HISTORY[pos].X_UPPER_M = false;
      COMPASS_HISTORY[pos].Y_LOWER_M = false;
      COMPASS_HISTORY[pos].Y_UPPER_M = false;
    }
  }
}

bool CAL_LEVEL_3::add_point(FLOAT_XYZ &Raw_XYZ)
{

  bool ret_pass_filter = false;

  // Simple noise filter
  if (!xyz_equal(Raw_XYZ, LAST_READ_VALUE) &&
      dist_xyz(Raw_XYZ, LAST_READ_VALUE) < NOISE_FILTER_DISTANCE)
  {
    // If the new point is too far from the last read value, do not store it.
    ret_pass_filter = true;
    
    if (ret_pass_filter)
    {
      float dist_newpoint_anyother = 0.0f;

      for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
      {
        if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
        {
          dist_newpoint_anyother = dist_xyz(Raw_XYZ, COMPASS_HISTORY[pos].POINT);
          
          if (dist_newpoint_anyother < CLOSEST_ALLOWED)
          {
            COMPASS_HISTORY.erase_p(pos);
          }
        }
      }

      COMPASS_POINT tmp_compass_point;
      tmp_compass_point.POINT = Raw_XYZ;
      COMPASS_HISTORY.push_back(tmp_compass_point);
    }
  }
  
  LAST_READ_VALUE = Raw_XYZ;


  return ret_pass_filter;
}

FLOAT_XYZ CAL_LEVEL_3::get_center_based_on_extremes()
{
  FLOAT_XYZ ret_center;

  int x_min = 0;
  int x_max = 0;
  
  int y_min = 0;
  int y_max = 0;

  // pick out extremes
  if (COMPASS_HISTORY.size() > 0)
  {
    for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
    {
      if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
      {
        if (COMPASS_HISTORY[pos].POINT.X < COMPASS_HISTORY[x_min].POINT.X)
        {
          x_min = pos;
        }
        if (COMPASS_HISTORY[pos].POINT.X > COMPASS_HISTORY[x_max].POINT.X)
        {
          x_max = pos;
        }
        if (COMPASS_HISTORY[pos].POINT.Y < COMPASS_HISTORY[y_min].POINT.Y)
        {
          y_min = pos;
        }
        if (COMPASS_HISTORY[pos].POINT.Y > COMPASS_HISTORY[y_max].POINT.Y)
        {
          y_max = pos;
        }
      }
    }

    // get center of extremes
    ret_center.X = (COMPASS_HISTORY[x_max].POINT.X + COMPASS_HISTORY[x_min].POINT.X) / 2.0f;
    ret_center.Y = (COMPASS_HISTORY[y_max].POINT.Y + COMPASS_HISTORY[y_min].POINT.Y) / 2.0f;
  }

  return ret_center;
}

void CAL_LEVEL_3::group_upper_lower()
{
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].POINT.X <= COMPASS_CENTER.X)
      {
        COMPASS_HISTORY[pos].X_LOWER = true;
      }

      if (COMPASS_HISTORY[pos].POINT.X >= COMPASS_CENTER.X)
      {
        COMPASS_HISTORY[pos].X_UPPER = true;
      }
      
      if (COMPASS_HISTORY[pos].POINT.Y <= COMPASS_CENTER.Y)
      {
        COMPASS_HISTORY[pos].Y_LOWER = true;
      }

      if (COMPASS_HISTORY[pos].POINT.Y >= COMPASS_CENTER.Y)
      {
        COMPASS_HISTORY[pos].Y_UPPER = true;
      }
    }
  }
}

void CAL_LEVEL_3::calculate_upper_lower_means()
{
  X_LOWER_SUM = 0.0f;
  X_LOWER_COUNT = 0;

  X_UPPER_SUM = 0.0f;
  X_UPPER_COUNT = 0;

  Y_LOWER_SUM = 0.0f;
  Y_LOWER_COUNT = 0;

  Y_UPPER_SUM = 0.0f;
  Y_UPPER_COUNT = 0;

  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].X_LOWER)
      {
        X_LOWER_COUNT++;
        X_LOWER_SUM += COMPASS_HISTORY[pos].POINT.X;
      }
      if (COMPASS_HISTORY[pos].X_UPPER)
      {
        X_UPPER_COUNT++;
        X_UPPER_SUM += COMPASS_HISTORY[pos].POINT.X;
      }
      if (COMPASS_HISTORY[pos].Y_LOWER)
      {
        Y_LOWER_COUNT++;
        Y_LOWER_SUM += COMPASS_HISTORY[pos].POINT.Y;
      }
      if (COMPASS_HISTORY[pos].Y_UPPER)
      {
        Y_UPPER_COUNT++;
        Y_UPPER_SUM += COMPASS_HISTORY[pos].POINT.Y;
      }
    }
  }

  X_LOWER_MEAN = X_LOWER_SUM / (float)X_LOWER_COUNT;
  X_UPPER_MEAN = X_UPPER_SUM / (float)X_UPPER_COUNT;
  Y_LOWER_MEAN = Y_LOWER_SUM / (float)Y_LOWER_COUNT;
  Y_UPPER_MEAN = Y_UPPER_SUM / (float)Y_UPPER_COUNT;
}

void CAL_LEVEL_3::group_means()
{
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].X_LOWER)
      {
        if (is_within(COMPASS_HISTORY[pos].POINT.X - COMPASS_CENTER.X, 
                      (X_LOWER_MEAN - COMPASS_CENTER.X) * 2.0f, 
                      (X_LOWER_MEAN - COMPASS_CENTER.X) * 0.8f))    
        {
          COMPASS_HISTORY[pos].X_LOWER_M = true;
        }
      }
      
      if (COMPASS_HISTORY[pos].X_UPPER)
      {
        if (is_within(COMPASS_HISTORY[pos].POINT.X - COMPASS_CENTER.X, 
                      (X_UPPER_MEAN - COMPASS_CENTER.X) * 2.0f, 
                      (X_UPPER_MEAN - COMPASS_CENTER.X) * 0.8f)) 
        {
          COMPASS_HISTORY[pos].X_UPPER_M = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_LOWER)
      {
        if (is_within(COMPASS_HISTORY[pos].POINT.Y - COMPASS_CENTER.Y, 
                      (Y_LOWER_MEAN - COMPASS_CENTER.Y) * 2.0f, 
                      (Y_LOWER_MEAN - COMPASS_CENTER.Y) * 0.8f))  
        {
          COMPASS_HISTORY[pos].Y_LOWER_M = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_UPPER)
      {
        if (is_within(COMPASS_HISTORY[pos].POINT.Y - COMPASS_CENTER.Y, 
                      (Y_UPPER_MEAN - COMPASS_CENTER.Y) * 2.0f, 
                      (Y_UPPER_MEAN - COMPASS_CENTER.Y) * 0.8f))
        {
          COMPASS_HISTORY[pos].Y_UPPER_M = true;
        }
      }

      if (COMPASS_HISTORY[pos].X_LOWER_M + 
          COMPASS_HISTORY[pos].X_UPPER_M + 
          COMPASS_HISTORY[pos].Y_LOWER_M + 
          COMPASS_HISTORY[pos].Y_UPPER_M > 1)
      {
        // if a point is in more than one mean group, it is suspect, remove all flags
        COMPASS_HISTORY[pos].X_LOWER_M = false;
        COMPASS_HISTORY[pos].X_UPPER_M = false;
        COMPASS_HISTORY[pos].Y_LOWER_M = false;
        COMPASS_HISTORY[pos].Y_UPPER_M = false;
      }
    }
  }
}

void CAL_LEVEL_3::delete_unnecessary_points()
{
  int max_size = COMPASS_HISTORY_SIZE / 5;

  int x_up = 0;
  int x_lo = 0;
  int y_up = 0;
  int y_lo = 0;

  // Pass 1: Keep max_size of Main Mean points.
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].X_LOWER_M)
      {
        if (x_lo < max_size)
        {
          x_lo++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].X_UPPER_M)
      {
        if (x_up < max_size)
        {
          x_up++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_LOWER_M)
      {
        if (y_lo < max_size)
        {
          y_lo++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_UPPER_M)
      {
        if (y_up < max_size)
        {
          y_up++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }
    }
  }

  // Pass 2: Keep max_size of upper and lower points with leftover reserves.
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].X_LOWER)
      {
        if (x_lo < max_size)
        {
          x_lo++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].X_UPPER)
      {
        if (x_up < max_size)
        {
          x_up++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_LOWER)
      {
        if (y_lo < max_size)
        {
          y_lo++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }

      if (COMPASS_HISTORY[pos].Y_UPPER)
      {
        if (y_up < max_size)
        {
          y_up++;
          COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
        }
      }
    }
  }
}

void CAL_LEVEL_3::set_heading_degrees_report(FLOAT_XYZ &Raw_XYZ)
{
  FLOAT_XYZ cent;

  float dx = Raw_XYZ.X - COMPASS_CENTER.X;
  float dy = Raw_XYZ.Y - COMPASS_CENTER.Y;

  float heading_rad = std::atan2(dy, dx);
  float heading_deg = heading_rad * (180.0f / M_PI);

  heading_deg -= 180.0f;

  if (heading_deg < 0.0f)
      heading_deg += 360.0f;

  HEADING_DEGREES_REPORT = heading_deg;
}

void CAL_LEVEL_3::clear()
{
  COMPASS_HISTORY.set_size(COMPASS_HISTORY_SIZE);
}

void CAL_LEVEL_3::calibration_level_3(unsigned long tmeFrame_Time, FLOAT_XYZ &Raw_XYZ)
{
  // Set to true to use fake compass input for testing
  if (false)
  {
    Raw_XYZ = fake_compass_input(tmeFrame_Time);
  }

  // Add point to history.  
  // Includes a simple noise filter.
  // Retain the last read value for future reference.
  bool successful_add = add_point(Raw_XYZ);

  // Analyze the points in the history. Only performed during iterations.
  // Analysis Determines Compass Center and Circle. 
  ITERATION_COUNTER++;
  if (successful_add && ITERATION_COUNTER > ITERATION_TRIGGER)
  {
    ITERATION_COUNTER = 0;

    clear_all_flags();

    group_upper_lower();
    calculate_upper_lower_means();

    if (X_LOWER_COUNT == 0 || X_UPPER_COUNT == 0 || Y_LOWER_COUNT == 0 || Y_UPPER_COUNT == 0)
    {
      COMPASS_CENTER = get_center_based_on_extremes();
      group_upper_lower();            // based on extremes
      calculate_upper_lower_means();  // based on extremes
    }
    else
    {
      COMPASS_CENTER.X = (X_UPPER_MEAN + X_LOWER_MEAN) * 0.5f;
      COMPASS_CENTER.Y = (Y_UPPER_MEAN + Y_LOWER_MEAN) * 0.5f;
    }

    group_means();
    delete_unnecessary_points();
  }
  
  // Calculate heading
  // (simple)
  set_heading_degrees_report(Raw_XYZ);

  //ITERATION_COUNTER2++;
  //if (ITERATION_COUNTER2 > 400)
  //exit(0);
}

// -------------------------------------------------------------------------------------

bool HMC5883L::register_write(char Register, char Value)
{
  bool ret_write_success = false;
  char buffer[2];

  buffer[0] = Register;
  buffer[1] = Value;

  if (write(DEVICE, buffer, 2) == 2)
  {
    ret_write_success = true;
  }

  return ret_write_success;
}

bool HMC5883L::create(string Offset_History_Filename)
{
  bool ret_success = false;

  CALIBRATED_BEARINGS_SIZE = (int)(COMMS_COMPASS_HISTORY_TIME_SPAN_MS / COMMS_COMPASS_POLLING_RATE_MS); 

  CALIBRATED_BEARINGS.clear();
  CALIBRATED_BEARINGS.reserve(CALIBRATED_BEARINGS_SIZE + 1);
  
  LEVEL_3.OFFSET_HISTORY_FILENAME = Offset_History_Filename;
  LEVEL_3.clear();

  CONNECTED = false;

  CALIBRATE_LOCK = PROPS.CALIBRATION_LOCK_AT_START;

  if (PROPS.AUTOSTART == false)
  {
    // Disable Cycles
    CYCLE = -1;
  }

  DEVICE = open(PROPS.PORT.c_str(), O_RDWR);
  if (DEVICE >= 0)
  {
    if (ioctl(DEVICE, I2C_SLAVE, PROPS.I2C_ID) >= 0)
    {
      FALSE_CATCH write_success;

      //register_write(0x00, 0x20); // Register A
      //                            // Sample of 1  (default)
      //                            // Output rate of 15 Hz  (default)
      //                            // Normal Measurtement configuration  (default)

      write_success.catch_false(register_write(0x01, 0x20));   // Register B  (00100000)
                                    // 1090 Gauss  (default)

      write_success.catch_false(register_write(0x02, 0x00));   // Mode Register
                                    // Continuous Measurement Mode

      if (write_success.has_false())
      {
        // Errored
        //exit(0);
      }
      
      ret_success = true;
      CONNECTED = true;

      //calibration_preload_set();
    }
  }
  else
  {
    // Program error correct if needed
    //exit(0);
  }

  return ret_success;
}

void HMC5883L::stop()
{
  close(DEVICE);
  CONNECTED = false;
}

void HMC5883L::process(NMEA &GPS_System, unsigned long tmeFrame_Time)
{
  //FLOAT_XYZ calibrated_bearing_xyz = calculate_calibrated_xyz(RAW_XYZ, LEVEL_2.offset(), LEVEL_2.skew());
  //RAW_BEARING = (atan2(calibrated_bearing_xyz.Y, calibrated_bearing_xyz.X) * 180 / M_PI);
  RAW_BEARING = LEVEL_3.HEADING_DEGREES_REPORT;
  
  float bearing = RAW_BEARING + KNOWN_DEVICE_DEGREE_OFFSET;

  while (bearing <= 0.0f)
  {
    bearing = bearing + 360.0f;
  }

  while (bearing > 360.0f)
  {
    bearing = bearing - 360.0f;
  }

  CALIBRATED_BEARINGS.push_back(bearing);

  // Maintain point history size
  while ((int)CALIBRATED_BEARINGS.size() > (CALIBRATED_BEARINGS_SIZE))
  {
    CALIBRATED_BEARINGS.erase(CALIBRATED_BEARINGS.begin());
  }

  // Level 2 - Calibration (Always Active.)
  if (CALIBRATE_LOCK == false)
  {
    LEVEL_3.calibration_level_3(tmeFrame_Time, RAW_XYZ);
  }

  // Determine Jitter
  if (CALIBRATED_BEARINGS.size() > 0)
  {
    float bearing_start_value = CALIBRATED_BEARINGS[0];

    float bearing_value = 0.0f;
    float bearing_min = CALIBRATED_BEARINGS[0];
    float bearing_max = CALIBRATED_BEARINGS[0];

    for (int pos = 1; pos < (int)CALIBRATED_BEARINGS.size(); pos++)
    {
      bearing_value = CALIBRATED_BEARINGS[pos];

      if (bearing_start_value < 90.0f && bearing_value > 270.0f)
      {
        bearing_value = bearing_value - 360.0f;
      }
      else if (bearing_start_value > 270.0f && bearing_value < 90.0f)
      {
        bearing_value = bearing_value + 360.0f;
      }

      // Determine Jitter
      if (pos > 0)
      {
        if (bearing_value < bearing_min)
        {
          bearing_min = bearing_value;
        }
        if (bearing_value > bearing_max)
        {
          bearing_max = bearing_value;
        }
      }
    }

    if (CALIBRATED_BEARINGS.size() > 0)
    {
      BEARING = CALIBRATED_BEARINGS.back();

      if (BEARING <= 0.0f)
      {
        BEARING = BEARING + 360.0f;
      }
      else if (BEARING > 360.0f)
      {
        BEARING = BEARING - 360.0f;
      }
      else

      BEARING_JITTER_MIN = bearing_min;
      BEARING_JITTER_MAX = bearing_max;
    }
  }

  // Passive GPS Bearing adjustment
  if (GPS_System.active(tmeFrame_Time)) // Enable
  {
    // Avoid constant recalibration from the GPS heading (gps only updated once per sec)
    //  If calibration sucessful, wait 1 min. if not, try again 
    //  in 10 sec.

    if (GPS_HEADING_CALIBRATION_TIMER.ping_down(tmeFrame_Time) == false)
    { 
      bool calibrated = false;

      if (GPS_System.current_position().SPEED.val_mph() > 30.0f)  // if speed over X mps
      {

        if (GPS_System.TRACK.TRACK_POINTS_DETAILED.size() > 3)
        {
          // check track difference over past 3 seconds to be less than 1 degree. (not turning)
          if (no_roll_difference(GPS_System.TRACK.TRACK_POINTS_DETAILED.back().TRUE_HEADING,
                                  GPS_System.TRACK.TRACK_POINTS_DETAILED[GPS_System.TRACK.TRACK_POINTS_DETAILED.size() - 3].TRUE_HEADING, 
                                  360.0f)
                                  < 1.0f)
          {
            // Needs no roll system.  
            float difference = no_roll_difference(BEARING_JITTER_MIN, BEARING_JITTER_MAX, 360.0f);
            
            // continue if jitter difference is small
            if (difference < 3.0f) // if jitter less than X degrees
            {
              bearing_known_offset_calibration(GPS_System.current_position().TRUE_HEADING);

              // cal good, wait 60 seconds
              calibrated = true;
            }
          }
        }
      }

      if (calibrated)
      {
        GPS_HEADING_CALIBRATION_TIMER.ping_up(tmeFrame_Time, 60000);
      }
      else
      {
        // turning over 1 degree in 3 seconds.
        // jitter too large, wait 10 sec
        // not over 30 mph, wait 10 seconds
        GPS_HEADING_CALIBRATION_TIMER.ping_up(tmeFrame_Time, 10000);
      }
    }
  }
}

void HMC5883L::calibrateion_reset()
{
  CALIBRATED_BEARINGS.clear();

  CALIBRATE_LOCK = false;

  LEVEL_3.clear();
}

void HMC5883L::calibrate_toggle()
{
  CALIBRATE = !CALIBRATE;
}

bool HMC5883L::calibrate_on()
{
  return CALIBRATE;
}

/*
void HMC5883L::calibrate_lock_toggle()
{
  CALIBRATE_LOCK = !CALIBRATE_LOCK;
}

bool HMC5883L::calibrate_lock_on()
{
  return CALIBRATE_LOCK;
}
*/

/*
MIN_MAX_SIMPLE HMC5883L::calibration_min_max_x()
{
  return LEVEL_2.x_min_max();
}
MIN_MAX_SIMPLE HMC5883L::calibration_min_max_y()
{
  return LEVEL_2.y_min_max();
}
MIN_MAX_SIMPLE HMC5883L::calibration_min_max_z()
{
  return LEVEL_2.z_min_max();
}

FLOAT_XYZ HMC5883L::calibration_offset()
{
  return LEVEL_2.offset();
}

CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_active_quad_data()
{
  return LEVEL_2.CALIBRATION_QUADS[0].QUAD_DATA;
}
bool HMC5883L::calibration_points_active_quad_overflow()
{
  return LEVEL_2.CALIBRATION_QUADS[0].QUAD_DATA.OVERFLOW;
}

float HMC5883L::calibration_variance()
{
  return LEVEL_2.variance();
}
bool HMC5883L::calibration_simple()
{
  return LEVEL_2.simple_calibration();
}
*/

/*
void HMC5883L::calibration_preload(FLOAT_XYZ Cal_Pt_1, float Cal_Var_1, 
                                    FLOAT_XYZ Cal_Pt_2, float Cal_Var_2, 
                                    FLOAT_XYZ Cal_Pt_3, float Cal_Var_3, 
                                    FLOAT_XYZ Cal_Pt_4, float Cal_Var_4, 
                                    float Cal_Offset)
{
  PRELOAD_DATA_LOADED = true;
  KNOWN_DEVICE_DEGREE_OFFSET_PRELOAD = Cal_Offset;
  LEVEL_2.calibration_preload(Cal_Pt_1, Cal_Var_1, Cal_Pt_2, Cal_Var_2, 
                              Cal_Pt_3, Cal_Var_3, Cal_Pt_4, Cal_Var_4);
}

void HMC5883L::calibration_preload_set()
{
  if (PRELOAD_DATA_LOADED)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = KNOWN_DEVICE_DEGREE_OFFSET_PRELOAD;
    
    LEVEL_2.calibration_preload_set();
    
    if (LEVEL_2.BEARING_OFFSET_LOAD != -1)
    {
      KNOWN_DEVICE_DEGREE_OFFSET = LEVEL_2.BEARING_OFFSET_LOAD;
    }
  }
}
*/

bool HMC5883L::connected()
{
  return CONNECTED;
}

bool HMC5883L::cycle(NMEA &GPS_System, unsigned long tmeFrame_Time)
{
  //bool data_received = true;
  bool ret_cycle_changed = false;

  if (CYCLE == 99)
  {
    // First Run
    DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 2000);

    CYCLE_CHANGE = 99;
    ret_cycle_changed = true;

    if (PROPS.AUTOSTART)
    {
      CYCLE = 2;  // Start connection cycle.
    }
    else
    {
      CYCLE = -1; // Read as if connected
    }
  }
  else if (CYCLE == -1)
  {
    // Ignore all cycles if Cycle is -1.
  }
  else if (CYCLE == 0)
  {
    // Keep active with checking for live data
    if (PROPS.AUTOSTART && PROPS.CONTINUOUS_DATA)
    {
      if (DATA_RECIEVED_TIMER.ping_down(tmeFrame_Time) == false)
      {
        CYCLE = 1;
      }
    }
  }
  else if (CYCLE == 1)
  {
    // Check for cycle close. Need to ensure all comm sent before closing connection and reconnecting
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 1;
      ret_cycle_changed = true;

      // Close the connection.
      //  Don't start reconnect if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        CYCLE_TIMER.ping_up(tmeFrame_Time, 1000); // Wait before starting next cycle.
        CYCLE = 2;

        stop();
      }
      else
      {
        CYCLE = -1;
      }
    }
  }
  else if (CYCLE == 2)
  {
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 2;
      ret_cycle_changed = true;

      //  Open a new connection at current baud rate.
      //  Don't prepare check baud rate if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 2000);   // Refresh the data recieve timer.

        CYCLE = 0; // Go into normal cycle.
        create(PROPS.OFFSET_HISTORY_FILE_NAME);
      }
      else
      {
        CYCLE = -1;
        create(PROPS.OFFSET_HISTORY_FILE_NAME);
      }
    }
  }

  if (CONNECTED == true)
  {
    // get raw x, y, z compass data
    BUFFER[0] = 0x03;   // Set to request xyz.

    // Write buffer
    if ((write(DEVICE, BUFFER, 1)) != 1)
    {
      // Write Failed
      if (PROPS.AUTOSTART)
      {
        // set to stop
        CYCLE_CHANGE = -1;
        stop();
      }
      else
      {
        // Set to disconnect and restart
        CYCLE_CHANGE = 1;
      }

      ret_cycle_changed = true;
    }
    else
    {
      // Read buffer
      if (read(DEVICE, BUFFER, 6) != 6)
      {
        // Read Failed
        if (PROPS.AUTOSTART)
        {
          // set to stop
          CYCLE_CHANGE = -1;
          stop();
        }
        else
        {
          // Set to disconnect and restart
          CYCLE_CHANGE = 1;
        }

        ret_cycle_changed = true;
      }
      else
      {
        short x = 0;
        short y = 0;
        short z = 0;

        x = (BUFFER[0] << 8) | BUFFER[1];
        y = (BUFFER[4] << 8) | BUFFER[5];
        z = (BUFFER[2] << 8) | BUFFER[3];

        RAW_XYZ.X = y;
        RAW_XYZ.Y = x;
        RAW_XYZ.Z = z;

        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 5000);   // Looking for live data

        // Process
        process(GPS_System, tmeFrame_Time);
      }
    }
  }

  return ret_cycle_changed;
}

int HMC5883L::cycle_change()
{
  return CYCLE_CHANGE;
}

void HMC5883L::open_port()
{
  CYCLE = 2;
}

void HMC5883L::close_port()
{
  CYCLE = -1;
  stop();
}

//bool HMC5883L::active(unsigned long tmeFrame_Time)
//{
//  return DATA_RECIEVED_TIMER.ping_down(tmeFrame_Time);
//}

void HMC5883L::bearing_known_offset_calibration(float Known_Bearing)
{
  KNOWN_DEVICE_DEGREE_OFFSET = Known_Bearing - RAW_BEARING;

  if (KNOWN_DEVICE_DEGREE_OFFSET < 0.0f)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = KNOWN_DEVICE_DEGREE_OFFSET + 360.0f;
  }

  //LEVEL_3.BEARING_OFFSET_LOAD = KNOWN_DEVICE_DEGREE_OFFSET;
}

float HMC5883L::bearing_known_offset()
{
  if (KNOWN_DEVICE_DEGREE_OFFSET > 180.0f)
  {
    // If offset is greater than 180, then it is negative.
    return KNOWN_DEVICE_DEGREE_OFFSET -= 360.0f;
  }
  else
  {
    // If offset is less than 180, then it is positive.
    return KNOWN_DEVICE_DEGREE_OFFSET;
  }
}

float HMC5883L::bearing()
{
  if (BEARING < -0.0f)
  {
    return 360.0f + BEARING;
  }
  else
  {
    return BEARING;
  }
}
        
float HMC5883L::bearing_jitter_min()
{
  if (BEARING_JITTER_MIN < -0.0f)
  {
    return 360.0f + BEARING_JITTER_MIN;
  }
  else
  {
    return BEARING_JITTER_MIN;
  }
}

float HMC5883L::bearing_jitter_max()
{
  if (BEARING_JITTER_MAX < -0.0f)
  {
    return 360.0f + BEARING_JITTER_MAX;
  }
  else
  {
    return BEARING_JITTER_MAX;
  }
}


#endif
