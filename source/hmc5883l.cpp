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

float dist(float X, float Y)
{
  return sqrtf((X * X) + (Y * Y));
}

bool XYZ_MIN_MAX(COMPASS_XYZ &Raw_XYZ, bool &Has_Data, MIN_MAX_SIMPLE &Xmm, MIN_MAX_SIMPLE &Ymm, MIN_MAX_SIMPLE &Zmm)
{
  bool ret_changed = false;

  if (Has_Data == false)
  {
    Xmm.MIN_VALUE = (float)Raw_XYZ.X;
    Xmm.MAX_VALUE = (float)Raw_XYZ.X;
    Ymm.MIN_VALUE = (float)Raw_XYZ.Y;
    Ymm.MAX_VALUE = (float)Raw_XYZ.Y;
    Zmm.MIN_VALUE = (float)Raw_XYZ.Z;
    Zmm.MAX_VALUE = (float)Raw_XYZ.Z;

    Has_Data = true;
    ret_changed = true;
  }
  else
  {
    if (Raw_XYZ.X < Xmm.MIN_VALUE)
    {
      Xmm.MIN_VALUE = Raw_XYZ.X;
      ret_changed = true;
    }
    if (Raw_XYZ.X > Xmm.MAX_VALUE)
    {
      Xmm.MAX_VALUE = Raw_XYZ.X;
      ret_changed = true;
    }
    if (Raw_XYZ.Y < Ymm.MIN_VALUE)
    {
      Ymm.MIN_VALUE = Raw_XYZ.Y;
      ret_changed = true;
    }
    if (Raw_XYZ.Y > Ymm.MAX_VALUE)
    {
      Ymm.MAX_VALUE = Raw_XYZ.Y;
      ret_changed = true;
    }
    if (Raw_XYZ.Z < Zmm.MIN_VALUE)
    {
      Zmm.MIN_VALUE = Raw_XYZ.Z;
      ret_changed = true;
    }
    if (Raw_XYZ.Z > Zmm.MAX_VALUE)
    {
      Zmm.MAX_VALUE = Raw_XYZ.Z;
      ret_changed = true;
    }
  }

  return ret_changed;
}

// -------------------------------------------------------------------------------------

/*
void CAL_LEVEL_1::clear()
{  
  MIN_MAX_HAS_DATA = false;
  OFFSET.X = 0;
  OFFSET.Y = 0;
  OFFSET.Z = 0;
}

COMPASS_XYZ CAL_LEVEL_1::offset()
{
  return OFFSET;
}

MIN_MAX_SIMPLE CAL_LEVEL_1::x_min_max()
{
  return X_MIN_MAX;
}
MIN_MAX_SIMPLE CAL_LEVEL_1::y_min_max()
{
  return Y_MIN_MAX;
}
MIN_MAX_SIMPLE CAL_LEVEL_1::z_min_max()
{
  return Z_MIN_MAX;
}

bool CAL_LEVEL_1::calibration_level_1_check(vector<COMPASS_XYZ> &Raw_Points)
{
  bool ret_needs_calibration = false;

  if (Raw_Points.size() >=5)
  {
    for (int pos = Raw_Points.size() - 5; 
          pos < (int)Raw_Points.size() && ret_needs_calibration == false; 
          pos++)
    {
      if (Raw_Points[pos].X < X_MIN_MAX.MIN_VALUE || Raw_Points[pos].X > X_MIN_MAX.MAX_VALUE)
      {
        ret_needs_calibration = true;
      }
      if (Raw_Points[pos].Y < Y_MIN_MAX.MIN_VALUE || Raw_Points[pos].Y > Y_MIN_MAX.MAX_VALUE)
      {
        ret_needs_calibration = true;
      }
      if (Raw_Points[pos].Z < Z_MIN_MAX.MIN_VALUE || Raw_Points[pos].Z > Z_MIN_MAX.MAX_VALUE)
      {
        ret_needs_calibration = true;
      }
    }
  }
  else
  {
    ret_needs_calibration = true;
  }

  return ret_needs_calibration;
}

void CAL_LEVEL_1::calibration_level_1(unsigned long tmeFrame_Time, COMPASS_XYZ &Raw_XYZ)
{
  OFFSET_CHANGED = false;

  if (XYZ_MIN_MAX(Raw_XYZ, MIN_MAX_HAS_DATA, X_MIN_MAX, Y_MIN_MAX, Z_MIN_MAX))
  {
    {
      CALIBRATION_TIMER_LEVEL_1.ping_up(tmeFrame_Time, 1 * 60000);
      OFFSET.X = (X_MIN_MAX.MAX_VALUE + X_MIN_MAX.MIN_VALUE) / 2.0f;
      OFFSET.Y = (Y_MIN_MAX.MAX_VALUE + Y_MIN_MAX.MIN_VALUE) / 2.0f;
      OFFSET.Z = (Z_MIN_MAX.MAX_VALUE + Z_MIN_MAX.MIN_VALUE) / 2.0f;

      OFFSET_CHANGED = true;
    }
  }
}
*/

// -------------------------------------------------------------------------------------

void CAL_LEVEL_2_QUAD_RECORD::add_point(COMPASS_XYZ &Raw_XYZ)
{
  if ((int)DATA_POINTS.size() < SIZE)
  {
    DATA_POINTS.push_back(Raw_XYZ);
  }
  else
  {
    OVERFLOW = true;
  }
}

void CAL_LEVEL_2_QUAD_RECORD::clear()
{
  DATA_POINTS.clear();
  DATA_POINTS.reserve(SIZE);
  OVERFLOW = false;
}

int CAL_LEVEL_2::get_quad(COMPASS_XYZ &Raw_XYZ, float Distance)
{
  // A
  if ((Raw_XYZ.Y - OFFSET.Y) < -(Distance * 2.0f / 3.0f))
  {
    return 0;
  }
  // C
  else if ((Raw_XYZ.X - OFFSET.X) < -(Distance * 2.0f  / 3.0f))
  {
    return 3;
  }
  // D
  else if ((Raw_XYZ.X - OFFSET.X) > (Distance * 2.0f  / 3.0f))
  {
    return 1;
  }
  // B
  else if ((Raw_XYZ.Y - OFFSET.Y) > (Distance * 2.0f  / 3.0f))
  {
    return 2;
  }
  else
  {
    return -1;
  }
}

void CAL_LEVEL_2::calibration_preload(CALIBRATION_DATA A_Cal_Pt, CALIBRATION_DATA B_Cal_Pt, 
                                      CALIBRATION_DATA C_Cal_Pt, CALIBRATION_DATA D_Cal_Pt)
{
  A_Cal_Pt_PRELOAD = A_Cal_Pt;
  A_Cal_Pt_PRELOAD.DISTANCE_VARIANCE_HAS_DATA = true;
  A_Cal_Pt_PRELOAD.HAS_DATA = true;

  B_Cal_Pt_PRELOAD = B_Cal_Pt;
  B_Cal_Pt_PRELOAD.DISTANCE_VARIANCE_HAS_DATA = true;
  B_Cal_Pt_PRELOAD.HAS_DATA = true;

  C_Cal_Pt_PRELOAD = C_Cal_Pt;
  C_Cal_Pt_PRELOAD.DISTANCE_VARIANCE_HAS_DATA = true;
  C_Cal_Pt_PRELOAD.HAS_DATA = true;

  D_Cal_Pt_PRELOAD = D_Cal_Pt;
  D_Cal_Pt_PRELOAD.DISTANCE_VARIANCE_HAS_DATA = true;
  D_Cal_Pt_PRELOAD.HAS_DATA = true;

  PRELOAD_DATA_LOADED = true;
}

void CAL_LEVEL_2::calibration_preload_set()
{
  if (PRELOAD_DATA_LOADED)
  {
    A = A_Cal_Pt_PRELOAD;
    B = B_Cal_Pt_PRELOAD;
    C = C_Cal_Pt_PRELOAD;
    D = D_Cal_Pt_PRELOAD;

    // Prep:
    
    SIMPLE_CALIBRATION = false;

    // Adjust new offset
    Y_MIN_MAX.MIN_VALUE = A.COORD.Y;
    Y_MIN_MAX.MAX_VALUE = B.COORD.Y;
    X_MIN_MAX.MIN_VALUE = C.COORD.X;
    X_MIN_MAX.MAX_VALUE = D.COORD.X;

    OFFSET.X = (X_MIN_MAX.MAX_VALUE + X_MIN_MAX.MIN_VALUE) / 2.0f;
    OFFSET.Y = (Y_MIN_MAX.MAX_VALUE + Y_MIN_MAX.MIN_VALUE) / 2.0f;
    OFFSET.Z = (Z_MIN_MAX.MAX_VALUE + Z_MIN_MAX.MIN_VALUE) / 2.0f;
  }
}

MIN_MAX_SIMPLE CAL_LEVEL_2::x_min_max()
{
  return X_MIN_MAX;
}
MIN_MAX_SIMPLE CAL_LEVEL_2::y_min_max()
{
  return Y_MIN_MAX;
}
MIN_MAX_SIMPLE CAL_LEVEL_2::z_min_max()
{
  return Z_MIN_MAX;
}

CALIBRATION_DATA CAL_LEVEL_2::a()
{
  return A;
}
CALIBRATION_DATA CAL_LEVEL_2::c()
{
  return C;
}
CALIBRATION_DATA CAL_LEVEL_2::d()
{
  return D;
}
CALIBRATION_DATA CAL_LEVEL_2::b()
{
  return B;
}

COMPASS_XYZ CAL_LEVEL_2::offset()
{
  return OFFSET;
}

void CAL_LEVEL_2::clear()
{
  MIN_MAX_HAS_DATA = false;

  SIMPLE_CALIBRATION = true;

  MIN_MAX_SIMPLE t_MIN_MAX;
  COMPASS_XYZ t_MIN_POINT;

  X_MIN_MAX = t_MIN_MAX;

  Y_MIN_MAX = t_MIN_MAX;

  Z_MIN_MAX = t_MIN_MAX;

  OFFSET = t_MIN_POINT;

  QUAD_DATA.clear();

  QUAD      = -1;
  QUAD_PREV = -1;
  
  CALIBRATION_DATA t;

  A = t;
  C = t;
  D = t;
  B = t;
}


float CAL_LEVEL_2::skew_x()
{
  if (SIMPLE_CALIBRATION)
  {
    return 0.0f;
  }
  else
  {
    return SKEW_X;
  }
}
float CAL_LEVEL_2::skew_y()
{
  if (SIMPLE_CALIBRATION)
  {
    return 0.0f;
  }
  else
  {
    return SKEW_Y;
  }
}

void CAL_LEVEL_2::calibration_level_2(COMPASS_XYZ &Raw_XYZ)
{ 
  // Inititialization of Offset, to be ignored after second set.

  if (SIMPLE_CALIBRATION)
  {
    if (XYZ_MIN_MAX(Raw_XYZ, MIN_MAX_HAS_DATA, X_MIN_MAX, Y_MIN_MAX, Z_MIN_MAX))
    {
      OFFSET.X = (X_MIN_MAX.MAX_VALUE + X_MIN_MAX.MIN_VALUE) / 2.0f;
      OFFSET.Y = (Y_MIN_MAX.MAX_VALUE + Y_MIN_MAX.MIN_VALUE) / 2.0f;
      OFFSET.Z = (Z_MIN_MAX.MAX_VALUE + Z_MIN_MAX.MIN_VALUE) / 2.0f;
    }

    if (A.HAS_DATA && B.HAS_DATA && C.HAS_DATA && D.HAS_DATA)
    {
      SIMPLE_CALIBRATION = false;
    }
  }
  
  float distance = dist(Raw_XYZ.X - OFFSET.X, Raw_XYZ.Y - OFFSET.Y);
  int quad_current = get_quad(Raw_XYZ, distance);

  if (quad_current != -1)
  {
    // Store point
    QUAD_DATA.DATA_POINTS.push_back(Raw_XYZ);

    // Simple Filter
    if (distance > 100.0f && (int)QUAD_DATA.DATA_POINTS.size() > 10)
    {
      // Check for quadrant changes
      if (quad_current != QUAD)
      {
        if (quad_current != QUAD_PREV && QUAD_DATA.OVERFLOW == false)
        {
          bool changed = false;

          // Calculate new offset
          COMPASS_XYZ outstanding_point;
          float minimum_distance = 0;
          float maximum_distance = 0;
          float r_dist = 0;

          // filter outliers in future.

          //  A
          if (QUAD == 0)
          {
            // get min y
            for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
            {
              r_dist = dist(QUAD_DATA.DATA_POINTS[pos].X - OFFSET.X, QUAD_DATA.DATA_POINTS[pos].Y - OFFSET.Y);

              if (pos == 0)
              {
                outstanding_point = QUAD_DATA.DATA_POINTS[0];
                minimum_distance = r_dist;
                maximum_distance = r_dist;
              }
              else
              {
                if (QUAD_DATA.DATA_POINTS[pos].Y < outstanding_point.Y)
                {
                  outstanding_point = QUAD_DATA.DATA_POINTS[pos];
                }

                if (r_dist < minimum_distance)
                {
                  minimum_distance = r_dist;
                }

                if (r_dist > maximum_distance)
                {
                  maximum_distance = r_dist;
                }
              }
            }

            if ((maximum_distance - minimum_distance) < (A.DISTANCE_VARIANCE * 1.1f) || A.DISTANCE_VARIANCE_HAS_DATA == false)
            {
              // validate positions
              if (SIMPLE_CALIBRATION || 
                  (is_within(outstanding_point.X, C.COORD.X,  D.COORD.X) &&  
                  outstanding_point.Y < C.COORD.Y && 
                  outstanding_point.Y < D.COORD.Y && 
                  outstanding_point.Y < B.COORD.Y))
              {
                A.COORD = outstanding_point;

                A.DISTANCE_VARIANCE = (maximum_distance - minimum_distance);
                A.DISTANCE_VARIANCE_HAS_DATA = true;

                A.HAS_DATA = true;
                Y_MIN_MAX.MIN_VALUE = outstanding_point.Y;
                
                changed = true;
              }
            }
          }

          // B
          if (QUAD == 2)
          {
            // get max y
            for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
            {
              r_dist = dist(QUAD_DATA.DATA_POINTS[pos].X - OFFSET.X, QUAD_DATA.DATA_POINTS[pos].Y - OFFSET.Y);

              if (pos == 0)
              {
                outstanding_point = QUAD_DATA.DATA_POINTS[0];
                minimum_distance = r_dist;
                maximum_distance = r_dist;
              }
              else
              {
                if (QUAD_DATA.DATA_POINTS[pos].Y > outstanding_point.Y)
                {
                  outstanding_point = QUAD_DATA.DATA_POINTS[pos];
                }

                if (r_dist < minimum_distance)
                {
                  minimum_distance = r_dist;
                }

                if (r_dist > maximum_distance)
                {
                  maximum_distance = r_dist;
                }
              }
            }

            if ((maximum_distance - minimum_distance) < (B.DISTANCE_VARIANCE * 1.1f) || B.DISTANCE_VARIANCE_HAS_DATA == false)
            {
              // validate positions
              if (SIMPLE_CALIBRATION || 
                  (is_within(outstanding_point.X, C.COORD.X, D.COORD.X) &&  
                  outstanding_point.Y > C.COORD.Y && 
                  outstanding_point.Y > D.COORD.Y && 
                  outstanding_point.Y > A.COORD.Y))
              {
                B.COORD = outstanding_point;

                B.DISTANCE_VARIANCE = (maximum_distance - minimum_distance);
                B.DISTANCE_VARIANCE_HAS_DATA = true;

                B.HAS_DATA = true;
                Y_MIN_MAX.MAX_VALUE = outstanding_point.Y;
                
                changed = true;
              }
            }
          }

          // C
          if (QUAD == 3)
          {
            // get min x
            for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
            {
              r_dist = dist(QUAD_DATA.DATA_POINTS[pos].X - OFFSET.X,QUAD_DATA.DATA_POINTS[pos].Y - OFFSET.Y);
              
              if (pos == 0)
              {
                outstanding_point = QUAD_DATA.DATA_POINTS[0];
                minimum_distance = r_dist;
                maximum_distance = r_dist;
              }
              else
              {
                if (QUAD_DATA.DATA_POINTS[pos].X < outstanding_point.X)
                {
                  outstanding_point = QUAD_DATA.DATA_POINTS[pos];
                }

                if (r_dist < minimum_distance)
                {
                  minimum_distance = r_dist;
                }

                if (r_dist > maximum_distance)
                {
                  maximum_distance = r_dist;
                }
              }
            }

            if ((maximum_distance - minimum_distance) < (C.DISTANCE_VARIANCE * 1.1f) || C.DISTANCE_VARIANCE_HAS_DATA == false)
            {
              // validate positions
              if (SIMPLE_CALIBRATION || 
                  (is_within(outstanding_point.Y, A.COORD.Y, B.COORD.Y) &&  
                  outstanding_point.X < A.COORD.X && 
                  outstanding_point.X < D.COORD.X && 
                  outstanding_point.X < B.COORD.X))
              {
                C.COORD = outstanding_point;

                C.DISTANCE_VARIANCE = (maximum_distance - minimum_distance);
                C.DISTANCE_VARIANCE_HAS_DATA = true;

                C.HAS_DATA = true;
                X_MIN_MAX.MIN_VALUE = outstanding_point.X;
                
                changed = true;
              }
            }
          }

          // D
          if (QUAD == 1)
          {
            // get max x
            for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
            {
              r_dist = dist(QUAD_DATA.DATA_POINTS[pos].X - OFFSET.X,QUAD_DATA.DATA_POINTS[pos].Y - OFFSET.Y);
              
              if (pos == 0)
              {
                outstanding_point = QUAD_DATA.DATA_POINTS[0];
                minimum_distance = r_dist;
                maximum_distance = r_dist;
              }
              else
              {
                if (QUAD_DATA.DATA_POINTS[pos].X > outstanding_point.X)
                {
                  outstanding_point = QUAD_DATA.DATA_POINTS[pos];
                }

                if (r_dist < minimum_distance)
                {
                  minimum_distance = r_dist;
                }

                if (r_dist > maximum_distance)
                {
                  maximum_distance = r_dist;
                }
              }
            }

            if ((maximum_distance - minimum_distance) < (D.DISTANCE_VARIANCE * 1.1f) || D.DISTANCE_VARIANCE_HAS_DATA == false)
            {
              // validate positions
              if (SIMPLE_CALIBRATION || 
                  (is_within(outstanding_point.Y, A.COORD.Y, B.COORD.Y) &&  
                  outstanding_point.X > A.COORD.X && 
                  outstanding_point.X > C.COORD.X && 
                  outstanding_point.X > B.COORD.X))
              {
                D.COORD = outstanding_point;
                D.DISTANCE_VARIANCE = (maximum_distance - minimum_distance);
                D.DISTANCE_VARIANCE_HAS_DATA = true;

                D.HAS_DATA = true;
                X_MIN_MAX.MAX_VALUE = outstanding_point.X;
                
                changed = true;
              }
            }
          }

          if (changed)
          {
            // Adjust new offset
            OFFSET.X = (X_MIN_MAX.MAX_VALUE + X_MIN_MAX.MIN_VALUE) / 2.0f;
            OFFSET.Y = (Y_MIN_MAX.MAX_VALUE + Y_MIN_MAX.MIN_VALUE) / 2.0f;
            OFFSET.Z = (Z_MIN_MAX.MAX_VALUE + Z_MIN_MAX.MIN_VALUE) / 2.0f;

            // Adjust offset skew

            if (SIMPLE_CALIBRATION == false)
            {
              SKEW_X = (abs(A.COORD.X - B.COORD.X)) / 2.0f;
              SKEW_Y = (abs(C.COORD.Y - D.COORD.Y)) / 2.0f;
            }
          }
        }

        // Clear old record
        QUAD_DATA.clear();
        
        // Quad rotate full
        QUAD_PREV = QUAD;
        QUAD = quad_current;
      }
    }
  }
}

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

bool HMC5883L::create()
{
  bool ret_success = false;

  RAW_POINTS.clear();
  RAW_POINTS.reserve(1000);  
  CALIBRATED_BEARINGS.clear();
  CALIBRATED_BEARINGS.reserve(1000);
  
  LEVEL_2.clear();

  CONNECTED = false;

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
      // default settings
      register_write(0x01, 0x20); // Still figuring out register writes
      register_write(0x02, 0x00); // Still figuring out register writes
      
      ret_success = true;
      CONNECTED = true;

      LEVEL_2.calibration_preload_set();
    }
  }

  return ret_success;
}

void HMC5883L::stop()
{
  close(DEVICE);
  CONNECTED = false;
}

COMPASS_XYZ HMC5883L::calculate_calibrated_xyz(COMPASS_XYZ &Raw_XYZ)
{
  COMPASS_XYZ tmp_point;

  //  x = x - ( x skew extreme * ( y / y val extreme) )
  //  y = y - ( y skew extreme * ( x / x val extreme) )

  tmp_point.X = (Raw_XYZ.X - LEVEL_2.offset().X) - (LEVEL_2.skew_x() * ((Raw_XYZ.Y - LEVEL_2.offset().Y) / (-LEVEL_2.offset().Y)));
  
  //tmp_point.Y = Raw_XYZ.Y - LEVEL_2.offset().Y;
  tmp_point.Y = (Raw_XYZ.Y - LEVEL_2.offset().Y) - (LEVEL_2.skew_y() * ((Raw_XYZ.X - LEVEL_2.offset().X) / (-LEVEL_2.offset().X)));
  
  tmp_point.Z = Raw_XYZ.Z - LEVEL_2.offset().Z;

  return tmp_point;
}

void HMC5883L::process()
{
  RAW_POINTS.push_back(RAW_XYZ);

  CALIBRATED_BEARINGS.push_back((atan2(calibrated_xyz().Y, calibrated_xyz().X) * 180 / M_PI) + 
                                  90.0f - KNOWN_DEVICE_DEGREE_OFFSET);

  // Maintain point history size
  if (RAW_POINTS.size() > 990)
  {
    RAW_POINTS.erase(RAW_POINTS.begin(), RAW_POINTS.begin() + 190);
  }
  if (CALIBRATED_BEARINGS.size() > 990)
  {
    CALIBRATED_BEARINGS.erase(CALIBRATED_BEARINGS.begin(), CALIBRATED_BEARINGS.begin() + 190);
  }

  /*
  // Spot check Calibration
  //  Spot check calibration if not in current callibration check.
  if (CALIBRATE == false && CALIBRATION_SPOT_CHECK.ping_down(tmeFrame_Time) == false)
  {
    // Check Level 1
    if (LEVEL_1.calibration_level_1_check(RAW_POINTS))
    {
      // Start 1 minute timer to calibrate
      LEVEL_1.CALIBRATION_TIMER_LEVEL_1.ping_up(tmeFrame_Time, 1 * 60000);

      // Turn on Level 1 Calibration
      CALIBRATE = true;
    }
    else
    {
      // Wait 5 minutes for next calibration check
      CALIBRATION_SPOT_CHECK.ping_up(tmeFrame_Time, 5 * 60000);
    }

    // Level 2
    // Check Bearing with GPS Heading
    
  }
  */

  // Level 2 - Calibration (Always Active.)
  if (CALIBRATE_LOCK == false)
  {
    LEVEL_2.calibration_level_2(RAW_XYZ);
  }

  // Calclulate Bearing

  // Simple Filter
  if (RAW_POINTS.size() >= 5)
  {
    int pos_start = CALIBRATED_BEARINGS.size() - 5;

    int bearing_total = 0;
    for (int pos = pos_start; pos < (int)CALIBRATED_BEARINGS.size(); pos++)
    {
      bearing_total = bearing_total + CALIBRATED_BEARINGS[pos] + 360.0f;
      
      if (pos == pos_start)
      {
        BEARING_JITTER_MIN = CALIBRATED_BEARINGS[pos_start] + 360.0f;
        BEARING_JITTER_MAX = CALIBRATED_BEARINGS[pos_start] + 360.0f;
      }

      // Determine Jitter
      if (CALIBRATED_BEARINGS[pos] + 360.0f < BEARING_JITTER_MIN + 360.0f)
      {
        BEARING_JITTER_MIN = CALIBRATED_BEARINGS[pos] + 360.0f;
      }
      if (CALIBRATED_BEARINGS[pos] + 360.0f > BEARING_JITTER_MAX + 360.0f)
      {
        BEARING_JITTER_MAX = CALIBRATED_BEARINGS[pos] + 360.0f;
      }
    }

    BEARING = (bearing_total / 5.0f) - 360.0f;
    BEARING_JITTER_MIN = BEARING_JITTER_MIN - 360.0f;
    BEARING_JITTER_MAX = BEARING_JITTER_MAX - 360.0f;
  }
}

int HMC5883L::raw_points_size()
{
  return RAW_POINTS.size();
}

COMPASS_XYZ HMC5883L::raw_xyz()
{
  if (RAW_POINTS.size() >0)
  {
    return RAW_POINTS[RAW_POINTS.size() -1];
  }
  else
  {
    COMPASS_XYZ tmp_point;
    return tmp_point;
  }
}

COMPASS_XYZ HMC5883L::raw_xyz(int Position)
{
  return RAW_POINTS[Position];
}

COMPASS_XYZ HMC5883L::calibrated_xyz()
{
  if (RAW_POINTS.size() >0)
  {
    return calculate_calibrated_xyz(RAW_POINTS[RAW_POINTS.size() -1]);
  }
  else
  {
    COMPASS_XYZ tmp_point;
    return tmp_point;
  }
}

COMPASS_XYZ HMC5883L::calibrated_xyz(int Position)
{
  return calculate_calibrated_xyz(RAW_POINTS[Position]);
}

void HMC5883L::calibrateion_reset()
{
  RAW_POINTS.clear();
  CALIBRATED_BEARINGS.clear();

  LEVEL_2.clear();
}

void HMC5883L::calibrate_toggle()
{
  CALIBRATE = !CALIBRATE;
}

void HMC5883L::calibrate_lock_toggle()
{
  CALIBRATE_LOCK = !CALIBRATE_LOCK;
}

bool HMC5883L::calibrate_on()
{
  return CALIBRATE;
}

bool HMC5883L::calibrate_lock_on()
{
  return CALIBRATE_LOCK;
}

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

COMPASS_XYZ HMC5883L::calibration_offset()
{
  return LEVEL_2.offset();
}

CALIBRATION_DATA HMC5883L::calibration_point_a()
{
  return LEVEL_2.a();
}
CALIBRATION_DATA HMC5883L::calibration_point_b()
{
  return LEVEL_2.b();
}
CALIBRATION_DATA HMC5883L::calibration_point_c()
{
  return LEVEL_2.c();
}
CALIBRATION_DATA HMC5883L::calibration_point_d()
{
  return LEVEL_2.d();
}

void HMC5883L::calibration_preload(CALIBRATION_DATA A_Cal_Pt, CALIBRATION_DATA B_Cal_Pt, 
                                    CALIBRATION_DATA C_Cal_Pt, CALIBRATION_DATA D_Cal_Pt)
{
  LEVEL_2.calibration_preload(A_Cal_Pt, B_Cal_Pt, C_Cal_Pt, D_Cal_Pt);
}

bool HMC5883L::connected()
{
  return CONNECTED;
}

bool HMC5883L::cycle(unsigned long tmeFrame_Time)
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
        create();
      }
      else
      {
        CYCLE = -1;
        create();
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
        process();
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
  KNOWN_DEVICE_DEGREE_OFFSET = BEARING + KNOWN_DEVICE_DEGREE_OFFSET - Known_Bearing;

  if (KNOWN_DEVICE_DEGREE_OFFSET < 0.0f)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = KNOWN_DEVICE_DEGREE_OFFSET + 360.0f;
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


#endif