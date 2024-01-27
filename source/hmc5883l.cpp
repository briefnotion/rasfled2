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

COMPASS_XYZ calc_offset(COMPASS_XYZ Y_Min,  COMPASS_XYZ Y_Max, COMPASS_XYZ X_Min, COMPASS_XYZ X_Max)
{
  COMPASS_XYZ ret_offset;

  ret_offset.X = (X_Min.X + X_Max.X) / 2.0f;
  ret_offset.Y = (Y_Min.Y + Y_Max.Y) / 2.0f;

  return ret_offset;
}

// -------------------------------------------------------------------------------------

bool four_point_check(COMPASS_XYZ Top, COMPASS_XYZ Bottom, 
                      COMPASS_XYZ Left, COMPASS_XYZ Right)
{
  if (is_within(Top.X, Left.X,  Right.X)    && 
      is_within(Bottom.X, Left.X,  Right.X) &&
      is_within(Left.Y, Top.Y,  Bottom.Y)   &&
      is_within(Right.Y, Top.Y,  Bottom.Y))
  {
    return true;
  }
  else 
  {
    return false;
  }
}

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

float CALIBRATION_DATA::variance_from_offset(COMPASS_XYZ Offset)
{
  // Calculate new offset
  float minimum_distance = 0;
  float maximum_distance = 0;
  float r_dist = 0;

  // get min y
  if (QUAD_DATA.DATA_POINTS.size() == 0)
  {
    HAS_DATA = false;
  }
  else
  {
    HAS_DATA = true;

    for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
    {
      r_dist = dist(QUAD_DATA.DATA_POINTS[pos].X - Offset.X, QUAD_DATA.DATA_POINTS[pos].Y - Offset.Y);

      if (pos == 0)
      {
        minimum_distance = r_dist;
        maximum_distance = r_dist;
      }
      else
      {
        // Check Min Distance
        if (r_dist < minimum_distance)
        {
          minimum_distance = r_dist;
        }

        // Check Max Distance
        if (r_dist > maximum_distance)
        {
          maximum_distance = r_dist;
        }
      }
    }
  }
  
  return maximum_distance - minimum_distance;
}


void CALIBRATION_DATA::stick_the_landing_point(int Quadrant)
{

  if (QUAD_DATA.DATA_POINTS.size() > 0)
  {

    COMPASS_XYZ calc_Y_min_pt;
    COMPASS_XYZ calc_Y_max_pt;
    COMPASS_XYZ calc_X_min_pt;
    COMPASS_XYZ calc_X_max_pt;

    for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
    {
      if (pos == 0)
      {
        calc_Y_min_pt = QUAD_DATA.DATA_POINTS[0];
        calc_Y_max_pt = QUAD_DATA.DATA_POINTS[0];
        calc_X_min_pt = QUAD_DATA.DATA_POINTS[0];
        calc_X_max_pt = QUAD_DATA.DATA_POINTS[0];
      }
      else
      {
        // Check Min Y
        if (QUAD_DATA.DATA_POINTS[pos].Y < calc_Y_min_pt.Y)
        {
          calc_Y_min_pt = QUAD_DATA.DATA_POINTS[pos];
        }

        // Check Max Y
        if (QUAD_DATA.DATA_POINTS[pos].Y > calc_Y_max_pt.Y)
        {
          calc_Y_max_pt = QUAD_DATA.DATA_POINTS[pos];
        }

        // Check Min X
        if (QUAD_DATA.DATA_POINTS[pos].X < calc_X_min_pt.X)
        {
          calc_X_min_pt = QUAD_DATA.DATA_POINTS[pos];
        }

        // Check Max X
        if (QUAD_DATA.DATA_POINTS[pos].X > calc_X_max_pt.X)
        {
          calc_X_max_pt = QUAD_DATA.DATA_POINTS[pos];
        }
      }
    }

    if (Quadrant == 0)
    {
      COORD = calc_Y_min_pt;
    }
    else if (Quadrant == 1)
    {
      COORD = calc_X_max_pt;
    }
    else if (Quadrant == 2)
    {
      COORD = calc_Y_max_pt;
    }
    else if (Quadrant == 3)
    {
      COORD = calc_X_min_pt;
    }

  }
}

void CALIBRATION_DATA::clear()
{
  COMPASS_XYZ t_coord;

  COORD = t_coord;

  QUAD_DATA.clear();

  HAS_DATA = false;
}

// -------------------------------------------------------------------------------------

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

void CAL_LEVEL_2::calibration_preload(COMPASS_XYZ A_Cal_Pt, COMPASS_XYZ B_Cal_Pt, 
                                      COMPASS_XYZ C_Cal_Pt, COMPASS_XYZ D_Cal_Pt)
{
  A_Cal_Pt_PRELOAD = A_Cal_Pt;

  B_Cal_Pt_PRELOAD = B_Cal_Pt;

  C_Cal_Pt_PRELOAD = C_Cal_Pt;

  D_Cal_Pt_PRELOAD = D_Cal_Pt;

  PRELOAD_DATA_LOADED = true;
}

void CAL_LEVEL_2::calibration_preload_set()
{
  if (PRELOAD_DATA_LOADED)
  {
    SIMPLE_CALIBRATION = true;

    // Adjust new offset

    X_MIN_MAX.MIN_VALUE = C_Cal_Pt_PRELOAD.X;
    X_MIN_MAX.MAX_VALUE = D_Cal_Pt_PRELOAD.X;
    Y_MIN_MAX.MIN_VALUE = A_Cal_Pt_PRELOAD.Y;
    Y_MIN_MAX.MAX_VALUE = B_Cal_Pt_PRELOAD.Y;

    // Adjust new offset
    OFFSET.X = (C_Cal_Pt_PRELOAD.X + D_Cal_Pt_PRELOAD.X) / 2.0f;
    OFFSET.Y = (A_Cal_Pt_PRELOAD.Y + B_Cal_Pt_PRELOAD.Y) / 2.0f;

    // Adjust offset skew
    SKEW_X = (abs(A_Cal_Pt_PRELOAD.X - B_Cal_Pt_PRELOAD.X)) / 2.0f;
    SKEW_Y = (abs(C_Cal_Pt_PRELOAD.Y - D_Cal_Pt_PRELOAD.Y)) / 2.0f;
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

  SKEW_X = 0;
  SKEW_Y = 0;

  OFFSET = t_MIN_POINT;

  QUAD      = -1;
  QUAD_PREV = -1;

  A.clear();
  B.clear();
  C.clear();
  D.clear();
  ACTIVE_QUAD_DATA.clear();

  COMPLETE_QUAD_DATA_SET = false;
  DISTANCE_VARIANCE_FULL = -1;
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

float CAL_LEVEL_2::variance()
{
  return DISTANCE_VARIANCE_FULL;
}

bool CAL_LEVEL_2::simple_calibration()
{
  return SIMPLE_CALIBRATION;
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
  

  {
    // Complex Calibration

    float distance = dist(Raw_XYZ.X - OFFSET.X, Raw_XYZ.Y - OFFSET.Y);
    int quad_current = get_quad(Raw_XYZ, distance);

    if (quad_current != -1)
    {
      // Store point
      ACTIVE_QUAD_DATA.QUAD_DATA.add_point(Raw_XYZ);

      // Simple Filter
      if (distance > 100.0f && (int)ACTIVE_QUAD_DATA.QUAD_DATA.DATA_POINTS.size() > 10)
      {
        // Check for quadrant changes
        if (quad_current != QUAD)
        {
          if (quad_current != QUAD_PREV)
          {
            if (ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW == false)
            {
              bool changed = false;
              float variance = 0;
              COMPASS_XYZ tmp_offset;

              ACTIVE_QUAD_DATA.stick_the_landing_point(QUAD);

              // Get variance in with new quad data set, with temporary offset.
              if (QUAD == 0)
              {
                tmp_offset = calc_offset(ACTIVE_QUAD_DATA.COORD, B.COORD, C.COORD, D.COORD);
                variance = ACTIVE_QUAD_DATA.variance_from_offset(tmp_offset) + B.variance_from_offset(tmp_offset) +
                            C.variance_from_offset(tmp_offset) + D.variance_from_offset(tmp_offset);
              }
              else if (QUAD == 1)
              {
                tmp_offset = calc_offset(A.COORD, B.COORD, C.COORD, ACTIVE_QUAD_DATA.COORD);
                variance = A.variance_from_offset(tmp_offset) + B.variance_from_offset(tmp_offset) +
                            C.variance_from_offset(tmp_offset) + ACTIVE_QUAD_DATA.variance_from_offset(tmp_offset);
              }
              else if (QUAD == 2)
              {
                tmp_offset = calc_offset(A.COORD, ACTIVE_QUAD_DATA.COORD, C.COORD, D.COORD);
                variance = A.variance_from_offset(tmp_offset) + ACTIVE_QUAD_DATA.variance_from_offset(tmp_offset) +
                            C.variance_from_offset(tmp_offset) + D.variance_from_offset(tmp_offset);
              }
              else if (QUAD == 3)
              {
                tmp_offset = calc_offset(A.COORD, B.COORD, ACTIVE_QUAD_DATA.COORD, D.COORD);
                variance = A.variance_from_offset(tmp_offset) + B.variance_from_offset(tmp_offset) +
                            ACTIVE_QUAD_DATA.variance_from_offset(tmp_offset) + D.variance_from_offset(tmp_offset);
              }

              // validate positions
              bool pass_four_point_check = false;

              if (SIMPLE_CALIBRATION)
              {
                COMPASS_XYZ a_tmp;
                COMPASS_XYZ b_tmp;
                COMPASS_XYZ c_tmp;
                COMPASS_XYZ d_tmp;

                a_tmp.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
                a_tmp.Y = Y_MIN_MAX.MIN_VALUE;

                b_tmp.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
                b_tmp.Y = Y_MIN_MAX.MAX_VALUE;

                c_tmp.X = X_MIN_MAX.MIN_VALUE;
                c_tmp.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;

                d_tmp.X = X_MIN_MAX.MAX_VALUE;
                d_tmp.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;
                
                if (QUAD == 0)
                {
                  pass_four_point_check = four_point_check(ACTIVE_QUAD_DATA.COORD, b_tmp, c_tmp, d_tmp);
                }
                else if (QUAD == 1)
                {
                  pass_four_point_check = four_point_check(a_tmp, b_tmp, c_tmp, ACTIVE_QUAD_DATA.COORD);
                }
                else if (QUAD == 2)
                {
                  pass_four_point_check = four_point_check(a_tmp, ACTIVE_QUAD_DATA.COORD, c_tmp, d_tmp);
                }
                else if (QUAD == 3)
                {
                  pass_four_point_check = four_point_check(a_tmp, b_tmp, ACTIVE_QUAD_DATA.COORD, d_tmp);
                }
              }
              else
              {
                if (QUAD == 0)
                {
                  pass_four_point_check = four_point_check(ACTIVE_QUAD_DATA.COORD, B.COORD, C.COORD, D.COORD);
                }
                else if (QUAD == 1)
                {
                  pass_four_point_check = four_point_check(A.COORD, B.COORD, C.COORD, ACTIVE_QUAD_DATA.COORD);
                }
                else if (QUAD == 2)
                {
                  pass_four_point_check = four_point_check(A.COORD, ACTIVE_QUAD_DATA.COORD, C.COORD, D.COORD);
                }
                else if (QUAD == 3)
                {
                  pass_four_point_check = four_point_check(A.COORD, B.COORD, ACTIVE_QUAD_DATA.COORD, D.COORD);
                }
              }

              // Store all passing and calc new Min Max and Offsets
              if (pass_four_point_check || SIMPLE_CALIBRATION)
              {
                // If all quadrants have data get all min max points if distance variance improves.
                if (SIMPLE_CALIBRATION == false)
                {
                  if ((variance < (DISTANCE_VARIANCE_FULL * 1.05f)) || (DISTANCE_VARIANCE_FULL < 0.0f))
                  {
                    DISTANCE_VARIANCE_FULL = variance;
                    changed = true;
                  }
                }

                // Store info if data set incomplete or successfully changed DISTANCE_VARIANCE_FULL.
                if (SIMPLE_CALIBRATION || changed == true)
                {
                  if (QUAD == 0)
                  {
                    A.QUAD_DATA.DATA_POINTS.swap(ACTIVE_QUAD_DATA.QUAD_DATA.DATA_POINTS);
                    A.QUAD_DATA.OVERFLOW = ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW;
                    A.COORD = ACTIVE_QUAD_DATA.COORD;
                    A.HAS_DATA = ACTIVE_QUAD_DATA.HAS_DATA;
                  }
                  else if (QUAD == 1)
                  {
                    D.QUAD_DATA.DATA_POINTS.swap(ACTIVE_QUAD_DATA.QUAD_DATA.DATA_POINTS);
                    D.QUAD_DATA.OVERFLOW = ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW;
                    D.COORD = ACTIVE_QUAD_DATA.COORD;
                    D.HAS_DATA = ACTIVE_QUAD_DATA.HAS_DATA;
                  }
                  else if (QUAD == 2)
                  {
                    B.QUAD_DATA.DATA_POINTS.swap(ACTIVE_QUAD_DATA.QUAD_DATA.DATA_POINTS);
                    B.QUAD_DATA.OVERFLOW = ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW;
                    B.COORD = ACTIVE_QUAD_DATA.COORD;
                    B.HAS_DATA = ACTIVE_QUAD_DATA.HAS_DATA;
                  }
                  else if (QUAD == 3)
                  {
                    C.QUAD_DATA.DATA_POINTS.swap(ACTIVE_QUAD_DATA.QUAD_DATA.DATA_POINTS);
                    C.QUAD_DATA.OVERFLOW = ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW;
                    C.COORD = ACTIVE_QUAD_DATA.COORD;
                    C.HAS_DATA = ACTIVE_QUAD_DATA.HAS_DATA;
                  }
                }

                // calculate new offsets if successfully changed DISTANCE_VARIANCE_FULL
                if (changed)
                {
                  X_MIN_MAX.MIN_VALUE = C.COORD.X;
                  X_MIN_MAX.MAX_VALUE = D.COORD.X;
                  Y_MIN_MAX.MIN_VALUE = A.COORD.Y;
                  Y_MIN_MAX.MAX_VALUE = B.COORD.Y;

                  // Adjust new offset
                  OFFSET = tmp_offset;
                  //OFFSET.Z = no idea;

                  // Adjust offset skew
                  SKEW_X = (abs(A.COORD.X - B.COORD.X)) / 2.0f;
                  SKEW_Y = (abs(C.COORD.Y - D.COORD.Y)) / 2.0f;
                }
              }
            }            
          }

          // Quad rotate full
          ACTIVE_QUAD_DATA.clear();
          QUAD_PREV = QUAD;
          QUAD = quad_current;

        }
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
  
  float bearing = (atan2(calibrated_xyz().Y, calibrated_xyz().X) * 180 / M_PI) + 90.0f - KNOWN_DEVICE_DEGREE_OFFSET;

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

    float bearing_start_value = CALIBRATED_BEARINGS[pos_start];

    int bearing_total = bearing_start_value;
    float bearing_value = 0.0f;

    for (int pos = (pos_start + 1); pos < (int)CALIBRATED_BEARINGS.size(); pos++)
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

      bearing_total = bearing_total + bearing_value;
      
      if (pos == pos_start)
      {
        BEARING_JITTER_MIN = bearing_value;
        BEARING_JITTER_MAX = bearing_value;
      }

      // Determine Jitter
      if (bearing_value < BEARING_JITTER_MIN)
      {
        BEARING_JITTER_MIN = bearing_value;
      }
      if (bearing_value> BEARING_JITTER_MAX)
      {
        BEARING_JITTER_MAX = bearing_value;
      }
    }

    BEARING = (bearing_total / 5.0f);

    if (BEARING <= 0.0f)
    {
      BEARING = BEARING + 360.0f;
    }
    else if (BEARING > 360.0f)
    {
      BEARING = BEARING - 360.0f;
    }
    else

    BEARING_JITTER_MIN = BEARING_JITTER_MIN;
    BEARING_JITTER_MAX = BEARING_JITTER_MAX;
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

CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_a()
{
  return LEVEL_2.A.QUAD_DATA;
}
CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_b()
{
  return LEVEL_2.B.QUAD_DATA;
}
CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_c()
{
  return LEVEL_2.C.QUAD_DATA;
}
CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_d()
{
  return LEVEL_2.D.QUAD_DATA;
}
CAL_LEVEL_2_QUAD_RECORD HMC5883L::calibration_points_active_quad_data()
{
  return LEVEL_2.ACTIVE_QUAD_DATA.QUAD_DATA;
}
bool HMC5883L::calibration_points_active_quad_overflow()
{
  return LEVEL_2.ACTIVE_QUAD_DATA.QUAD_DATA.OVERFLOW;
}

COMPASS_XYZ HMC5883L::calibration_max_coord_a()
{
  return LEVEL_2.A.COORD;
}
COMPASS_XYZ HMC5883L::calibration_max_coord_b()
{
  return LEVEL_2.B.COORD;
}
COMPASS_XYZ HMC5883L::calibration_max_coord_c()
{
  return LEVEL_2.C.COORD;
}
COMPASS_XYZ HMC5883L::calibration_max_coord_d()
{
  return LEVEL_2.D.COORD;
}
float HMC5883L::calibration_variance()
{
  return LEVEL_2.variance();
}
bool HMC5883L::calibration_simple()
{
  return LEVEL_2.simple_calibration();
}

void HMC5883L::calibration_preload(COMPASS_XYZ A_Cal_Pt, COMPASS_XYZ B_Cal_Pt, 
                                    COMPASS_XYZ C_Cal_Pt, COMPASS_XYZ D_Cal_Pt)
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