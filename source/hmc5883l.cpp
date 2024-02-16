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

void calc_offset_and_skew(bool Simple, FLOAT_XYZ Top, FLOAT_XYZ Right, FLOAT_XYZ Bot, FLOAT_XYZ Left, 
                          FLOAT_XYZ &Ret_Offset, FLOAT_XYZ &Ret_Skew)
{
  // 1 top, 2 right, 3 bot, 4 left

  Ret_Offset.Y = (Top.Y + Bot.Y) / 2.0f;
  Ret_Offset.X = (Left.X + Right.X) / 2.0f;

  if (Simple)
  {
    Ret_Skew.Y = 1.0f;
    Ret_Skew.X = 1.0f;
  }
  else
  {
    Ret_Skew.Y = (abs(Right.Y - Left.Y)) / 2.0f;
    Ret_Skew.X = (abs(Bot.X - Top.X)) / 2.0f;
  }
}

FLOAT_XYZ calculate_calibrated_xyz(FLOAT_XYZ &Raw_XYZ, FLOAT_XYZ Offset, FLOAT_XYZ Skew)
{
  FLOAT_XYZ tmp_point;

  //  x = x - ( x skew extreme * ( y / y val extreme) )
  //  y = y - ( y skew extreme * ( x / x val extreme) )

  tmp_point.X = (Raw_XYZ.X - Offset.X) - (Skew.X * ((Raw_XYZ.Y - Offset.Y) / (-Offset.Y)));
  tmp_point.Y = (Raw_XYZ.Y - Offset.Y) - (Skew.Y * ((Raw_XYZ.X - Offset.X) / (-Offset.X)));
  tmp_point.Z = Raw_XYZ.Z - Offset.Z;

  return tmp_point;
}

// -------------------------------------------------------------------------------------

bool four_point_check(FLOAT_XYZ Top, FLOAT_XYZ Bottom, 
                      FLOAT_XYZ Left, FLOAT_XYZ Right)
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

void CAL_LEVEL_2_QUAD_RECORD::add_point(FLOAT_XYZ &Raw_XYZ)
{
  if ((int)DATA_POINTS.size() <= VARIANCE_COLLECTION_SIZE)
  {
    DATA_POINTS.push_back(Raw_XYZ);
  }
  else
  {
    OVERFLOW = true;
  }
}

void CAL_LEVEL_2_QUAD_RECORD::clear(bool Simple_Calibration)
{
  if (Simple_Calibration)
  {
    VARIANCE_COLLECTION_SIZE = VARIANCE_COLLECTION_SIZE_SIMPLE;
  }
  else
  {
    VARIANCE_COLLECTION_SIZE = VARIANCE_COLLECTION_SIZE_COMPLEX;
  }

  DATA_POINTS.clear();
  DATA_POINTS.reserve(VARIANCE_COLLECTION_SIZE);

  DATA_POINTS_CALIBRATED.clear();
  DATA_POINTS_CALIBRATED.reserve(VARIANCE_COLLECTION_SIZE);

  OVERFLOW = false;
}

void CALIBRATION_DATA::add_last_known_offset_point()
{
  if (OFFSET_POINT_LIST.size() == 0)
  {
    OFFSET_POINT = LAST_KNOWN_OFFSET_POINT;
    OFFSET_POINT_LIST.push_back(LAST_KNOWN_OFFSET_POINT);
  }
  else 
  {
    if ((int)OFFSET_POINT_LIST.size() >= OFFSET_POINT_VECTOR_SIZE)
    {
      while ((int)OFFSET_POINT_LIST.size() >= OFFSET_POINT_VECTOR_SIZE)
      {
        OFFSET_POINT_LIST.erase(OFFSET_POINT_LIST.begin());
      }
    }

    OFFSET_POINT_LIST.push_back(LAST_KNOWN_OFFSET_POINT);

    OFFSET_POINT = emperical_mean_zyz(OFFSET_POINT_LIST, 1.0);
  }
}

float CALIBRATION_DATA::variance_from_offset(FLOAT_XYZ Offset, FLOAT_XYZ Skew, bool &Good_Data_Count_Pass)
{
  float max_variance = 0;

  // get min y
  if (QUAD_DATA.DATA_POINTS.size() == 0)
  {
    if (HAS_DATA)
    {
      // Situation to handle preloaded data
      LAST_KNOWN_VARIANCE = VARIANCE;
      return VARIANCE;
    }
    else
    {
      // Startup blank state.
      Good_Data_Count_Pass = false;
      return 0;
    }
  }
  else
  {
    // Load VARIANCE_COLLECTION
    vector<float> variance_collection;

    FLOAT_XYZ calculated_point;

    for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
    {
      calculated_point = calculate_calibrated_xyz(QUAD_DATA.DATA_POINTS[pos], Offset, Skew);
      variance_collection.push_back(dist(calculated_point.X, calculated_point.Y));
    }

    // return mean
    float mean = 0;
    float qualifying_value_for_variance = 0;

    // Set to false if fail, keep same (false or true) if true.
    bool good_data = false;

    good_data = emperical_mean(variance_collection, 3.0, 1 * COMMS_COMPASS_POLLING_RATE_FPS, mean, qualifying_value_for_variance);
    // X * COMMS_COMPASS_POLLING_RATE_FPS - refers to how much good data is the bottom limit - X is X seconds of good data

    if (good_data == false)
    {
      Good_Data_Count_Pass = false;
    }
    else
    {
      Good_Data_Count_Pass = true;

      float tmp_variance = 0;

      for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
      {
        // Check to see if the distance measured is passed its qualifying value to store
        //  only valid points as an extreme max.  Not liking that this loop is ran twice, 
        //  but i like the idea of changing emperical_mean() even less.
        // This is only to ensure that only a value that has been considered in the mean 
        //  value will be checked for a qualifying stick the landing value.
        //  Also, the landing should have already been stuck.
        if (pos < (int)variance_collection.size())
        {
          if(abs(variance_collection[pos] - mean) <= qualifying_value_for_variance)
          {
            tmp_variance = ((abs(variance_collection[pos] - mean)) / mean);

            if (tmp_variance > max_variance)
            {
              max_variance = tmp_variance;
            }
          }
        }
      }

      LAST_KNOWN_VARIANCE = max_variance;
    }

    return max_variance;
  }
}

bool CALIBRATION_DATA::stick_the_landing(FLOAT_XYZ Current_Offset, int Quadrant)
{
  bool ret_landing_stuck = false;

  // Stick the landing
  FLOAT_XYZ calc_Y_min_pt;
  FLOAT_XYZ calc_Y_max_pt;
  FLOAT_XYZ calc_X_min_pt;
  FLOAT_XYZ calc_X_max_pt;

  // Load VARIANCE_COLLECTION    
  vector<float> variance_collection;

  for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
  {
    variance_collection.push_back(dist(QUAD_DATA.DATA_POINTS[pos].X - Current_Offset.X, QUAD_DATA.DATA_POINTS[pos].Y - Current_Offset.Y));
  }

  // return mean
  float mean = 0;
  float mean_difference_from_variance = 0;

  // Set to false if fail, keep same (false or true) if true.
  bool good_data = false;

  good_data = emperical_mean(variance_collection, 2.0, 1 * COMMS_COMPASS_POLLING_RATE_FPS, mean, mean_difference_from_variance);
  // Good data count of 2 seconds worth of valid points.

  if (good_data)
  {
    // Proceed with sticking the landing with good data points
    bool just_started = true;

    for (int pos = 0; pos < (int)QUAD_DATA.DATA_POINTS.size(); pos++)
    {
      // Check to see if the distance measured is passed its qualifying value to store
      //  only valid points as an extreme max.  Not liking that this loop is ran twice, 
      //  but i like the idea of changing emperical_mean() even less.
      // This is only to ensure that only a value that has been considered in the mean 
      //  value will be checked for a qualifying stick the landing value.
      //  Also, the landing should have already been stuck.
      if(abs(variance_collection[pos] - mean) <= mean_difference_from_variance)
      {
        if (just_started)
        {
          // Set first point.
          calc_Y_min_pt = QUAD_DATA.DATA_POINTS[pos];
          calc_Y_max_pt = QUAD_DATA.DATA_POINTS[pos];
          calc_X_min_pt = QUAD_DATA.DATA_POINTS[pos];
          calc_X_max_pt = QUAD_DATA.DATA_POINTS[pos];
          just_started = false;
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

      if (Quadrant == 1)
      {
        LAST_KNOWN_OFFSET_POINT = calc_Y_min_pt;
      }
      else if (Quadrant == 2)
      {
        LAST_KNOWN_OFFSET_POINT = calc_X_max_pt;
      }
      else if (Quadrant == 3)
      {
        LAST_KNOWN_OFFSET_POINT = calc_Y_max_pt;
      }
      else if (Quadrant == 4)
      {
        LAST_KNOWN_OFFSET_POINT = calc_X_min_pt;
      }

      HAS_DATA = true;
      ret_landing_stuck = true;
    }
  }

  return ret_landing_stuck;
}

void CALIBRATION_DATA::clear(bool Simple_Calibration)
{
  QUAD_DATA.clear(Simple_Calibration);

  FLOAT_XYZ t_coord;
  OFFSET_POINT = t_coord;
  LAST_KNOWN_OFFSET_POINT = t_coord;

  OFFSET_POINT_LIST.clear();
  OFFSET_POINT_LIST.reserve(OFFSET_POINT_VECTOR_SIZE);

  VARIANCE = 1000;
  LAST_KNOWN_VARIANCE = 1000;

  HAS_DATA = false;
}

// -------------------------------------------------------------------------------------

int CAL_LEVEL_2::get_quad(FLOAT_XYZ &Raw_XYZ, float Distance)
{
  // A
  if ((Raw_XYZ.Y - OFFSET.Y) < -(Distance * 2.0f / 3.0f))
  {
    return 1;
  }
  // C
  else if ((Raw_XYZ.X - OFFSET.X) < -(Distance * 2.0f  / 3.0f))
  {
    return 4;
  }
  // D
  else if ((Raw_XYZ.X - OFFSET.X) > (Distance * 2.0f  / 3.0f))
  {
    return 2;
  }
  // B
  else if ((Raw_XYZ.Y - OFFSET.Y) > (Distance * 2.0f  / 3.0f))
  {
    return 3;
  }
  else
  {
    return -1;
  }
}

bool CAL_LEVEL_2::XYZ_MIN_MAX(FLOAT_XYZ &Raw_XYZ, bool &Has_Data, MIN_MAX_SIMPLE &Xmm, MIN_MAX_SIMPLE &Ymm, MIN_MAX_SIMPLE &Zmm)
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

float CAL_LEVEL_2::calc_all_quad_variance(int Swap_0_Quad_With, bool &Ret_Good_Data_Count_Pass)
{
  float ret_variance = 0.0f;

  bool tmp_good_data_count_pass = false;

  FLOAT_XYZ tmp_offset;
  FLOAT_XYZ tmp_skew;

  FLOAT_XYZ tmp_1;
  FLOAT_XYZ tmp_2;
  FLOAT_XYZ tmp_3;
  FLOAT_XYZ tmp_4;

  if (SIMPLE_CALIBRATION)
  {
    // top
    tmp_1.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
    tmp_1.Y = Y_MIN_MAX.MIN_VALUE;

    // bot
    tmp_3.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
    tmp_3.Y = Y_MIN_MAX.MAX_VALUE;

    // left
    tmp_4.X = X_MIN_MAX.MIN_VALUE;
    tmp_4.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;

    // right
    tmp_2.X = X_MIN_MAX.MAX_VALUE;
    tmp_2.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;

    calc_offset_and_skew(SIMPLE_CALIBRATION, tmp_1, tmp_2, tmp_3, tmp_4, 
                            tmp_offset, tmp_skew);
  }
  else
  {    
    if (QUAD == 1)
    {
      calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[0].OFFSET_POINT, CALIBRATION_QUADS[2].OFFSET_POINT, CALIBRATION_QUADS[3].OFFSET_POINT, CALIBRATION_QUADS[4].OFFSET_POINT, 
                            tmp_offset, tmp_skew);
    }
    else if (QUAD == 2)
    {
      calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[1].OFFSET_POINT, CALIBRATION_QUADS[0].OFFSET_POINT, CALIBRATION_QUADS[3].OFFSET_POINT, CALIBRATION_QUADS[4].OFFSET_POINT, 
                            tmp_offset, tmp_skew);
    }
    else if (QUAD == 3)
    {
      calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[1].OFFSET_POINT, CALIBRATION_QUADS[2].OFFSET_POINT, CALIBRATION_QUADS[0].OFFSET_POINT, CALIBRATION_QUADS[4].OFFSET_POINT, 
                            tmp_offset, tmp_skew);
    }
    else //if (QUAD == 4)
    {
      calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[1].OFFSET_POINT, CALIBRATION_QUADS[2].OFFSET_POINT, CALIBRATION_QUADS[3].OFFSET_POINT, CALIBRATION_QUADS[0].OFFSET_POINT, 
                            tmp_offset, tmp_skew);
    }
  }

  
  for (int quad = 1; quad < (int)CALIBRATION_QUADS.size(); quad++)
  {
    if (quad == Swap_0_Quad_With)
    {
      ret_variance = ret_variance + CALIBRATION_QUADS[0].variance_from_offset(tmp_offset, tmp_skew, tmp_good_data_count_pass);
      Ret_Good_Data_Count_Pass = tmp_good_data_count_pass;
    }
    else 
    {
      ret_variance = ret_variance + CALIBRATION_QUADS[quad].variance_from_offset(tmp_offset, tmp_skew, tmp_good_data_count_pass);
    }
  }

  return ret_variance;
}

void CAL_LEVEL_2::build_calibration_display_data()
{
  // Build Calibrated List for display only

  for (int quad = 1; quad < (int)CALIBRATION_QUADS.size(); quad++)
  {
    CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED.clear();
    for (int pos = 0; pos < (int)CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS.size(); pos++)
    {
      CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS_CALIBRATED.push_back(calculate_calibrated_xyz(CALIBRATION_QUADS[quad].QUAD_DATA.DATA_POINTS[pos], OFFSET, SKEW));
    }
  }
}

void CAL_LEVEL_2::build_non_simple_offsets()
{
  if (CALIBRATION_QUADS.size() == 5)
  {
    Y_MIN_MAX.MIN_VALUE = CALIBRATION_QUADS[1].OFFSET_POINT.Y;
    X_MIN_MAX.MAX_VALUE = CALIBRATION_QUADS[2].OFFSET_POINT.X;
    Y_MIN_MAX.MAX_VALUE = CALIBRATION_QUADS[3].OFFSET_POINT.Y;
    X_MIN_MAX.MIN_VALUE = CALIBRATION_QUADS[4].OFFSET_POINT.X;

    calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[1].OFFSET_POINT, CALIBRATION_QUADS[2].OFFSET_POINT, CALIBRATION_QUADS[3].OFFSET_POINT, CALIBRATION_QUADS[4].OFFSET_POINT, 
                          OFFSET, SKEW);
  }
}

void CAL_LEVEL_2::calibration_preload(FLOAT_XYZ A_Cal_Pt, float A_Cal_Var, 
                                      FLOAT_XYZ B_Cal_Pt, float B_Cal_Var, 
                                      FLOAT_XYZ C_Cal_Pt, float C_Cal_Var, 
                                      FLOAT_XYZ D_Cal_Pt, float D_Cal_Var)
{
  A_Cal_Pt_PRELOAD = A_Cal_Pt;
  A_Cal_Var_PRELOAD = A_Cal_Var;
  
  B_Cal_Pt_PRELOAD = B_Cal_Pt;
  B_Cal_Var_PRELOAD = B_Cal_Var;
  
  C_Cal_Pt_PRELOAD = C_Cal_Pt;
  C_Cal_Var_PRELOAD = C_Cal_Var;

  D_Cal_Pt_PRELOAD = D_Cal_Pt;
  D_Cal_Var_PRELOAD = D_Cal_Var;
}

void CAL_LEVEL_2::calibration_preload_set()
{
  if (CALIBRATION_QUADS.size() == 5)
  {
    SIMPLE_CALIBRATION = false;

    // Load Data
    CALIBRATION_QUADS[1].OFFSET_POINT = A_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[1].LAST_KNOWN_OFFSET_POINT = A_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[1].VARIANCE = A_Cal_Var_PRELOAD;
    CALIBRATION_QUADS[1].HAS_DATA = true;
    CALIBRATION_QUADS[1].add_last_known_offset_point();   // Build small offset history.
    CALIBRATION_QUADS[1].add_last_known_offset_point();
    CALIBRATION_QUADS[1].add_last_known_offset_point();

    CALIBRATION_QUADS[2].OFFSET_POINT = D_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[2].LAST_KNOWN_OFFSET_POINT = D_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[2].VARIANCE = D_Cal_Var_PRELOAD;
    CALIBRATION_QUADS[2].HAS_DATA = true;
    CALIBRATION_QUADS[2].add_last_known_offset_point();
    CALIBRATION_QUADS[2].add_last_known_offset_point();
    CALIBRATION_QUADS[2].add_last_known_offset_point();

    CALIBRATION_QUADS[3].OFFSET_POINT = B_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[3].LAST_KNOWN_OFFSET_POINT = B_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[3].VARIANCE = B_Cal_Var_PRELOAD;
    CALIBRATION_QUADS[3].HAS_DATA = true;
    CALIBRATION_QUADS[3].add_last_known_offset_point();
    CALIBRATION_QUADS[3].add_last_known_offset_point();
    CALIBRATION_QUADS[3].add_last_known_offset_point();

    CALIBRATION_QUADS[4].OFFSET_POINT = C_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[4].LAST_KNOWN_OFFSET_POINT = C_Cal_Pt_PRELOAD;
    CALIBRATION_QUADS[4].VARIANCE = C_Cal_Var_PRELOAD;
    CALIBRATION_QUADS[4].HAS_DATA = true;
    CALIBRATION_QUADS[4].add_last_known_offset_point();
    CALIBRATION_QUADS[4].add_last_known_offset_point();
    CALIBRATION_QUADS[4].add_last_known_offset_point();

    // Set Variance
    DISTANCE_VARIANCE_FULL = CALIBRATION_QUADS[1].VARIANCE + CALIBRATION_QUADS[2].VARIANCE + 
                              CALIBRATION_QUADS[3].VARIANCE + CALIBRATION_QUADS[4].VARIANCE;

    // Adjust new offset
    build_non_simple_offsets();
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

FLOAT_XYZ CAL_LEVEL_2::offset()
{
  return OFFSET;
}

FLOAT_XYZ CAL_LEVEL_2::skew()
{
  return SKEW;
}

void CAL_LEVEL_2::clear()
{
  MIN_MAX_HAS_DATA = false;

  SIMPLE_CALIBRATION = true;

  MIN_MAX_SIMPLE t_MIN_MAX;
  X_MIN_MAX = t_MIN_MAX;
  Y_MIN_MAX = t_MIN_MAX;
  Z_MIN_MAX = t_MIN_MAX;

  FLOAT_XYZ tmp_xyz_blank;
  OFFSET = tmp_xyz_blank;

  // Set Skew values to 1 for simple unset state.
  SKEW.X = 1.0f;
  SKEW.Y = 1.0f;
  SKEW.Z = 1.0f;

  QUAD      = -1;
  QUAD_PREV = -1;

  COMPLETE_QUAD_DATA_SET = false;
  DISTANCE_VARIANCE_FULL = -1;

  CALIBRATION_DATA tmp_calibration_data_0;
  CALIBRATION_DATA tmp_calibration_data_1;
  CALIBRATION_DATA tmp_calibration_data_2;
  CALIBRATION_DATA tmp_calibration_data_3;
  CALIBRATION_DATA tmp_calibration_data_4;

  CALIBRATION_QUADS.clear();

  CALIBRATION_QUADS.push_back(tmp_calibration_data_0);
  CALIBRATION_QUADS.push_back(tmp_calibration_data_1);
  CALIBRATION_QUADS.push_back(tmp_calibration_data_2);
  CALIBRATION_QUADS.push_back(tmp_calibration_data_3);
  CALIBRATION_QUADS.push_back(tmp_calibration_data_4);

  if (CALIBRATION_QUADS.size() == 5)
  {
    CALIBRATION_QUADS[0].clear(SIMPLE_CALIBRATION);
    CALIBRATION_QUADS[1].clear(SIMPLE_CALIBRATION);
    CALIBRATION_QUADS[2].clear(SIMPLE_CALIBRATION);
    CALIBRATION_QUADS[3].clear(SIMPLE_CALIBRATION);
    CALIBRATION_QUADS[4].clear(SIMPLE_CALIBRATION);
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

void CAL_LEVEL_2::calibration_level_2(FLOAT_XYZ &Raw_XYZ)
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

    if (CALIBRATION_QUADS.size() == 5)
    {
      if (CALIBRATION_QUADS[1].HAS_DATA && CALIBRATION_QUADS[2].HAS_DATA && CALIBRATION_QUADS[3].HAS_DATA && CALIBRATION_QUADS[4].HAS_DATA)
      {
        SIMPLE_CALIBRATION = false;

        build_non_simple_offsets();
        build_calibration_display_data();
      }
    }
  }
  
  if (CALIBRATION_QUADS.size() == 5)
  {
    // Complex Calibration

    float distance = dist(Raw_XYZ.X - OFFSET.X, Raw_XYZ.Y - OFFSET.Y);
    int quad_current = get_quad(Raw_XYZ, distance);

    if (quad_current != -1)
    {
      // Store point
      CALIBRATION_QUADS[0].QUAD_DATA.add_point(Raw_XYZ);

      // Simple Filter
      if (distance > 100.0f)
      {
        // Check for quadrant changes
        if (quad_current != QUAD)
        {
          if (quad_current != QUAD_PREV)
          {
            if (CALIBRATION_QUADS[0].QUAD_DATA.OVERFLOW == false)
            {
              bool changed = false;
              float variance = 0;

              // Need to stick the landing of point ACTIVE_QUAD_DATA first with current offsets
              if (CALIBRATION_QUADS[0].stick_the_landing(OFFSET, QUAD))
              {
                // Get variance in with new quad data set, with temporary offset.
                //  and stick the landing.

                bool pass_good_data_count = false; // Only down sets for fogotten reasons, start true.

                variance = calc_all_quad_variance(QUAD, pass_good_data_count);

                // validate positions
                bool pass_four_point_check = false;

                if (SIMPLE_CALIBRATION)
                {
                  FLOAT_XYZ tmp_1;
                  FLOAT_XYZ tmp_2;
                  FLOAT_XYZ tmp_3;
                  FLOAT_XYZ tmp_4;

                  // top
                  tmp_1.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
                  tmp_1.Y = Y_MIN_MAX.MIN_VALUE;

                  // bot
                  tmp_3.X = (X_MIN_MAX.MIN_VALUE + X_MIN_MAX.MAX_VALUE) / 2.0f;
                  tmp_3.Y = Y_MIN_MAX.MAX_VALUE;

                  // left
                  tmp_4.X = X_MIN_MAX.MIN_VALUE;
                  tmp_4.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;

                  // right
                  tmp_2.X = X_MIN_MAX.MAX_VALUE;
                  tmp_2.Y = (Y_MIN_MAX.MIN_VALUE + Y_MIN_MAX.MAX_VALUE) / 2.0f;
                  
                  if (QUAD == 1)
                  {
                    pass_four_point_check = four_point_check(CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              tmp_3, 
                                                              tmp_4, 
                                                              tmp_2);
                  }
                  else if (QUAD == 2)
                  {
                    pass_four_point_check = four_point_check(tmp_1, 
                                                              tmp_3, 
                                                              tmp_4, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT);
                  }
                  else if (QUAD == 3)
                  {
                    pass_four_point_check = four_point_check(tmp_1, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              tmp_4, 
                                                              tmp_2);
                  }
                  else if (QUAD == 4)
                  {
                    pass_four_point_check = four_point_check(tmp_1, 
                                                              tmp_3, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              tmp_2);
                  }
                }
                else
                {
                  if (QUAD == 1)
                  {
                    pass_four_point_check = four_point_check(CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              CALIBRATION_QUADS[3].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[4].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[2].OFFSET_POINT);
                  }
                  else if (QUAD == 2)
                  {
                    pass_four_point_check = four_point_check(CALIBRATION_QUADS[1].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[3].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[4].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT);
                  }
                  else if (QUAD == 3)
                  {
                    pass_four_point_check = four_point_check(CALIBRATION_QUADS[1].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              CALIBRATION_QUADS[4].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[2].OFFSET_POINT);
                  }
                  else if (QUAD == 4)
                  {
                    pass_four_point_check = four_point_check(CALIBRATION_QUADS[1].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[3].OFFSET_POINT, 
                                                              CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT, 
                                                              CALIBRATION_QUADS[2].OFFSET_POINT);
                  }
                }
 
                // Store all passing and calc new Min Max and Offsets
                if (pass_four_point_check && pass_good_data_count)
                {
                  // If all quadrants have data get all min max points if distance variance improves.
                  if (SIMPLE_CALIBRATION == false)
                  {
                    if ((variance < (DISTANCE_VARIANCE_FULL * 2.0f)) || (DISTANCE_VARIANCE_FULL < 0.0f))
                    {
                      changed = true;
                    }
                  }
                }

                // Store info if data set incomplete or successfully changed DISTANCE_VARIANCE_FULL.
                if ((SIMPLE_CALIBRATION && pass_good_data_count && pass_four_point_check) || changed == true)
                {
                  // Find best and worste variance values and only update values improved if 
                  //  not best and not improved if not worste. (quads should be queueed)
                  float best_variance = CALIBRATION_QUADS[1].LAST_KNOWN_VARIANCE;
                  float worst_variance = CALIBRATION_QUADS[1].LAST_KNOWN_VARIANCE;

                  for (int quad = 2; quad < (int)CALIBRATION_QUADS.size(); quad++)
                  {
                    if (CALIBRATION_QUADS[quad].LAST_KNOWN_VARIANCE < best_variance)
                    {
                      best_variance = CALIBRATION_QUADS[quad].LAST_KNOWN_VARIANCE;
                    }

                    if (CALIBRATION_QUADS[quad].LAST_KNOWN_VARIANCE > worst_variance)
                    {
                      worst_variance = CALIBRATION_QUADS[quad].LAST_KNOWN_VARIANCE;
                    }
                  }

                  // Do not copy if working quad is better than the best or worse than the worst.
                  if (SIMPLE_CALIBRATION == false && 
                      ((CALIBRATION_QUADS[QUAD].LAST_KNOWN_VARIANCE == best_variance && CALIBRATION_QUADS[0].LAST_KNOWN_VARIANCE < best_variance) || 
                      (CALIBRATION_QUADS[QUAD].LAST_KNOWN_VARIANCE == worst_variance && CALIBRATION_QUADS[0].LAST_KNOWN_VARIANCE > worst_variance)))
                  {
                    // do nothing
                  }
                  else
                  { 
                    // Copy Active Quad values to appropriate QUAD
                    CALIBRATION_QUADS[QUAD].QUAD_DATA.DATA_POINTS.swap(CALIBRATION_QUADS[0].QUAD_DATA.DATA_POINTS);
                    CALIBRATION_QUADS[QUAD].QUAD_DATA.OVERFLOW = CALIBRATION_QUADS[0].QUAD_DATA.OVERFLOW;
                    CALIBRATION_QUADS[QUAD].LAST_KNOWN_OFFSET_POINT = CALIBRATION_QUADS[0].LAST_KNOWN_OFFSET_POINT;
                    CALIBRATION_QUADS[QUAD].LAST_KNOWN_VARIANCE = CALIBRATION_QUADS[0].LAST_KNOWN_VARIANCE;
                    CALIBRATION_QUADS[QUAD].HAS_DATA = CALIBRATION_QUADS[0].HAS_DATA;

                    // Update Offset Point with new point
                    CALIBRATION_QUADS[QUAD].add_last_known_offset_point();

                    // Recalculate full variance. 
                    DISTANCE_VARIANCE_FULL = 0.0f;

                    // update all variance last know values as referenced good variance values from last variance scan.
                    for (int quad = 1; quad < (int)CALIBRATION_QUADS.size(); quad++)
                    {
                      CALIBRATION_QUADS[quad].VARIANCE = CALIBRATION_QUADS[quad].LAST_KNOWN_VARIANCE;
                      
                      // For display only. 
                      DISTANCE_VARIANCE_FULL = DISTANCE_VARIANCE_FULL + CALIBRATION_QUADS[quad].VARIANCE;
                    }
                  }
                }

                // calculate new offsets if successfully changed DISTANCE_VARIANCE_FULL
                if (changed)
                {
                  build_non_simple_offsets();
                }

                if (SIMPLE_CALIBRATION || changed == true)
                {
                  // Rebuild the display data points with new skews.
                  build_calibration_display_data();
                }

              }   // Landing Stuck

            } // New Quadrant Full Data   
          }   // Quadrant Changed

          // Quad rotate full
          
          CALIBRATION_QUADS[0].clear(SIMPLE_CALIBRATION);
          QUAD_PREV = QUAD;
          QUAD = quad_current;

        }
      }
    } // Simple Filter
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

  CALIBRATED_BEARINGS.clear();
  CALIBRATED_BEARINGS.reserve(CALIBRATED_BEARINGS_SIZE + 1);
  
  LEVEL_2.clear();

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
      //register_write(0x00, 0x20); // Register A
      //                            // Sample of 1  (default)
      //                            // Output rate of 15 Hz  (default)
      //                            // Normal Measurtement configuration  (default)

      register_write(0x01, 0x20);   // Register B  (00100000)
                                    // 1090 Gauss  (default)

      register_write(0x02, 0x00);   // Mode Register
                                    // Continuous Measurement Mode
      
      ret_success = true;
      CONNECTED = true;

      calibration_preload_set();
    }
  }

  return ret_success;
}

void HMC5883L::stop()
{
  close(DEVICE);
  CONNECTED = false;
}

void HMC5883L::process()
{
  FLOAT_XYZ calibrated_bearing_xyz = calculate_calibrated_xyz(RAW_XYZ, LEVEL_2.offset(), LEVEL_2.skew());
  RAW_BEARING = (atan2(calibrated_bearing_xyz.Y, calibrated_bearing_xyz.X) * 180 / M_PI);
  
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
    LEVEL_2.calibration_level_2(RAW_XYZ);
  }

  // Calclulate Bearing

  // Simple Filter
  if (CALIBRATED_BEARINGS.size() > 0)
  {
    float bearing_start_value = CALIBRATED_BEARINGS[0];

    int bearing_total = bearing_start_value;
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

      bearing_total = bearing_total + bearing_value;
      
      // Determine Jitter
      if (pos > 0)
      {
        if (bearing_value < bearing_min)
        {
          bearing_min = bearing_value;
        }
        if (bearing_value> bearing_max)
        {
          bearing_max = bearing_value;
        }
      }
    }

    if (CALIBRATED_BEARINGS.size() > 0)
    {
      BEARING = (bearing_total / (float)CALIBRATED_BEARINGS.size());

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
}

void HMC5883L::calibrateion_reset()
{
  CALIBRATED_BEARINGS.clear();

  CALIBRATE_LOCK = false;

  LEVEL_2.clear();
}

void HMC5883L::calibrate_toggle()
{
  CALIBRATE = !CALIBRATE;
}

bool HMC5883L::calibrate_on()
{
  return CALIBRATE;
}

void HMC5883L::calibrate_lock_toggle()
{
  CALIBRATE_LOCK = !CALIBRATE_LOCK;
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

void HMC5883L::calibration_preload(FLOAT_XYZ A_Cal_Pt, float A_Cal_Var, 
                                    FLOAT_XYZ B_Cal_Pt, float B_Cal_Var, 
                                    FLOAT_XYZ C_Cal_Pt, float C_Cal_Var, 
                                    FLOAT_XYZ D_Cal_Pt, float D_Cal_Var, 
                                    float Cal_Offset)
{
  PRELOAD_DATA_LOADED = true;
  KNOWN_DEVICE_DEGREE_OFFSET_PRELOAD = Cal_Offset;
  LEVEL_2.calibration_preload(A_Cal_Pt, A_Cal_Var, B_Cal_Pt, B_Cal_Var, 
                              C_Cal_Pt, C_Cal_Var, D_Cal_Pt, D_Cal_Var);
}

void HMC5883L::calibration_preload_set()
{
  if (PRELOAD_DATA_LOADED)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = KNOWN_DEVICE_DEGREE_OFFSET_PRELOAD;
    LEVEL_2.calibration_preload_set();
  }
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
  KNOWN_DEVICE_DEGREE_OFFSET = Known_Bearing - RAW_BEARING;

  if (KNOWN_DEVICE_DEGREE_OFFSET < 0.0f)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = KNOWN_DEVICE_DEGREE_OFFSET + 360.0f;
  }
}

float HMC5883L::bearing_known_offset()
{
  return KNOWN_DEVICE_DEGREE_OFFSET;
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