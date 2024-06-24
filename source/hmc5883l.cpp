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
/*

Notes:
To improve code:
  The entire variance routine is unnecessary. It could be cut out to remove a large 
    chunk of code.  Only the offset routines are necessary.  Although the variance 
    routines are good for determining if a quad data set is out of range, the emperical 
    rule on the offset points do the same thing, and varience (curve smoothnes) is 
    completely unnecessary.
  Code could be rewritten in more simplified ways by just concentrating on finding good 
    offsets to calculate skew and range.
  Memory usage is very arbitrary and can be tuned to require much less memory with more 
    efficientn methods of choosing points to store in the quads.

*/
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

void CALIBRATION_DATA::add_point_to_offset_point_list(FLOAT_XYZ Point)
{
  OFFSET_POINT_LIST.push_back(Point);
  
  if ((int)OFFSET_POINT_LIST.size() >= OFFSET_POINT_VECTOR_SIZE)
  {
    while ((int)OFFSET_POINT_LIST.size() >= OFFSET_POINT_VECTOR_SIZE)
    {
      OFFSET_POINT_LIST.erase(OFFSET_POINT_LIST.begin());
    }
  }
}

void CALIBRATION_DATA::calculate_offset_point()
{
  OFFSET_POINT = emperical_mean_zyz(OFFSET_POINT_LIST, 1.0);
}

void CALIBRATION_DATA::add_last_known_offset_point()
{
  add_point_to_offset_point_list(LAST_KNOWN_OFFSET_POINT);
  calculate_offset_point();
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
    else if (QUAD == 4)
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
    calc_offset_and_skew(SIMPLE_CALIBRATION, CALIBRATION_QUADS[1].OFFSET_POINT, CALIBRATION_QUADS[2].OFFSET_POINT, CALIBRATION_QUADS[3].OFFSET_POINT, CALIBRATION_QUADS[4].OFFSET_POINT, 
                          OFFSET, SKEW);

    Y_MIN_MAX.MIN_VALUE = CALIBRATION_QUADS[1].OFFSET_POINT.Y;
    X_MIN_MAX.MAX_VALUE = CALIBRATION_QUADS[2].OFFSET_POINT.X;
    Y_MIN_MAX.MAX_VALUE = CALIBRATION_QUADS[3].OFFSET_POINT.Y;
    X_MIN_MAX.MIN_VALUE = CALIBRATION_QUADS[4].OFFSET_POINT.X;
  }
}

bool CAL_LEVEL_2::offset_history_read()
{
  JSON_INTERFACE offset_list_json;
  string json_offset_list = "";

  bool ret_success = false;

  json_offset_list = file_to_string(OFFSET_HISTORY_FILENAME, ret_success);

  if (ret_success == true)
  {

    ret_success = offset_list_json.load_json_from_string(json_offset_list);

    if (ret_success == true)
    {
      for (int scan = 0; scan < (int)offset_list_json.ROOT.DATA.size(); scan++)
      {
        offset_list_json.ROOT.DATA[scan].get_if_is("bearing offset", BEARING_OFFSET_LOAD);
      }

      int history_pos = offset_list_json.ROOT.find_label_pos("offset history");

      if (history_pos != -1)
      {
        for (int group = 1; group <= 4; group++)
        {
          string group_name = "quad offset group ";
          group_name = group_name + to_string(group);

          int group_id_pos = offset_list_json.ROOT.DATA[history_pos].find_label_pos(group_name.c_str());

          if ((group_id_pos != -1) && (group >= 1) && (group <= 4))
          {
            int coordinates_pos = offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].find_label_pos("offset coordinates");

            if (coordinates_pos != -1)
            {
              int coordinate_list_size = offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].DATA[coordinates_pos].DATA.size();

              for (int coordinate = 0; coordinate < coordinate_list_size; coordinate++)
              {
                int coordinate_item_size = offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].DATA[coordinates_pos].DATA[coordinate].DATA.size();

                STRING_INT x;
                STRING_INT y;
                STRING_INT z;
                
                for (int coordinate_item = 0; coordinate_item < coordinate_item_size; coordinate_item++)
                {
                  offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].DATA[coordinates_pos].DATA[coordinate].DATA[coordinate_item].get_if_is("x", x);
                  offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].DATA[coordinates_pos].DATA[coordinate].DATA[coordinate_item].get_if_is("y", y);
                  offset_list_json.ROOT.DATA[history_pos].DATA[group_id_pos].DATA[coordinates_pos].DATA[coordinate].DATA[coordinate_item].get_if_is("z", z);
                }

                FLOAT_XYZ point;
                point.X = (float)(x.get_int_value());
                point.Y = (float)(y.get_int_value());
                point.Z = (float)(z.get_int_value());

                CALIBRATION_QUADS[group].add_point_to_offset_point_list(point);
              }
            }
          }
        }
      }
    }
  }

  return ret_success;
}

void CAL_LEVEL_2::offset_history_write()
{
  if (CALIBRATION_QUADS.size() == 5)
  {
    deque<string> file_dq_string;

    JSON_INTERFACE offset_list_json;

    JSON_ENTRY history;
    JSON_ENTRY quad_group;
    JSON_ENTRY offset_list;
    JSON_ENTRY xyz_offset;
    JSON_ENTRY offset;

    for (int quad = 1; quad < (int)CALIBRATION_QUADS.size(); quad++)
    {
      if (CALIBRATION_QUADS[quad].OFFSET_POINT_LIST.size() > 0)
      {
        quad_group.create_label_value(quotify("size"), to_string((int)CALIBRATION_QUADS[quad].OFFSET_POINT_LIST.size()));

        for (int pos = 0; pos < (int)CALIBRATION_QUADS[quad].OFFSET_POINT_LIST.size(); pos++)
        {
          offset.create_label_value(quotify("x"), to_string((int)CALIBRATION_QUADS[quad].OFFSET_POINT_LIST[pos].X));
          offset.create_label_value(quotify("y"), to_string((int)CALIBRATION_QUADS[quad].OFFSET_POINT_LIST[pos].Y));
          offset.create_label_value(quotify("z"), to_string((int)CALIBRATION_QUADS[quad].OFFSET_POINT_LIST[pos].Z));

          xyz_offset.put_json_in_list(offset);
          offset.clear_data();
        }

        quad_group.put_json_in_set(quotify("offset coordinates"), xyz_offset);
        xyz_offset.clear_data();
      }

      history.put_json_in_set(quotify("quad offset group " + to_string(quad)), quad_group);
      quad_group.clear_data();

    }

    offset_list_json.ROOT.create_label_value(quotify("bearing offset"), to_string((int)BEARING_OFFSET_LOAD));
    offset_list_json.ROOT.put_json_in_set(quotify("offset history"), history);

    // Create string list of file
    offset_list_json.json_print_build_to_string_deque(file_dq_string);
    
    // Write string list of file

    // bool ret_success = false;

    deque_string_to_file(OFFSET_HISTORY_FILENAME, file_dq_string, false);
  }
}

void CAL_LEVEL_2::calibration_preload(FLOAT_XYZ Cal_Pt_1, float Cal_Var_1, 
                                      FLOAT_XYZ Cal_Pt_2, float Cal_Var_2, 
                                      FLOAT_XYZ Cal_Pt_3, float Cal_Var_3, 
                                      FLOAT_XYZ Cal_Pt_4, float Cal_Var_4)
{
  Cal_Pt_PRELOAD_1 = Cal_Pt_1;
  Cal_Var_PRELOAD_1 = Cal_Var_1;
  
  Cal_Pt_PRELOAD_2 = Cal_Pt_2;
  Cal_Var_PRELOAD_2 = Cal_Var_2;
  
  Cal_Pt_PRELOAD_3 = Cal_Pt_3;
  Cal_Var_PRELOAD_3 = Cal_Var_3;

  Cal_Pt_PRELOAD_4 = Cal_Pt_4;
  Cal_Var_PRELOAD_4 = Cal_Var_4;
}

void CAL_LEVEL_2::calibration_preload_set()
{
  // Load history list
  if (offset_history_read() == false || COMMS_COMPASS_CAL_LOAD_AT_START)
  {
    if (CALIBRATION_QUADS.size() == 5 && COMMS_COMPASS_CAL_LOAD_AT_START)
    {
      SIMPLE_CALIBRATION = false;

      int preload_pushback_count = 5;    // Build small offset history.

      // Load Data
      CALIBRATION_QUADS[1].LAST_KNOWN_OFFSET_POINT = Cal_Pt_PRELOAD_1;
      //CALIBRATION_QUADS[1].VARIANCE = Cal_Var_PRELOAD_1;
      CALIBRATION_QUADS[1].HAS_DATA = true;
      for (int x = 0; x < preload_pushback_count; x++)
      {
        CALIBRATION_QUADS[1].add_point_to_offset_point_list(Cal_Pt_PRELOAD_1);
      }

      CALIBRATION_QUADS[2].LAST_KNOWN_OFFSET_POINT = Cal_Pt_PRELOAD_2;
      //CALIBRATION_QUADS[2].VARIANCE = Cal_Var_PRELOAD_2;
      CALIBRATION_QUADS[2].HAS_DATA = true;
      for (int x = 0; x < preload_pushback_count; x++)
      {
        CALIBRATION_QUADS[2].add_point_to_offset_point_list(Cal_Pt_PRELOAD_2);
      }

      CALIBRATION_QUADS[3].LAST_KNOWN_OFFSET_POINT = Cal_Pt_PRELOAD_3;
      //CALIBRATION_QUADS[3].VARIANCE = Cal_Var_PRELOAD_3;
      CALIBRATION_QUADS[3].HAS_DATA = true;
      for (int x = 0; x < preload_pushback_count; x++)
      {
        CALIBRATION_QUADS[3].add_point_to_offset_point_list(Cal_Pt_PRELOAD_3);
      }

      CALIBRATION_QUADS[4].LAST_KNOWN_OFFSET_POINT = Cal_Pt_PRELOAD_4;
      //CALIBRATION_QUADS[4].VARIANCE = Cal_Var_PRELOAD_4;
      CALIBRATION_QUADS[4].HAS_DATA = true;
      for (int x = 0; x < preload_pushback_count; x++)
      {
        CALIBRATION_QUADS[4].add_point_to_offset_point_list(Cal_Pt_PRELOAD_4);
      }
    }
  }
  else
  {
    CALIBRATION_QUADS[1].calculate_offset_point();
    CALIBRATION_QUADS[2].calculate_offset_point();
    CALIBRATION_QUADS[3].calculate_offset_point();
    CALIBRATION_QUADS[4].calculate_offset_point();

    SIMPLE_CALIBRATION = false;

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

  OFFSET_HISTORY_CHANGED = true;
}

float CAL_LEVEL_2::variance()
{
  return DISTANCE_VARIANCE_FULL;
}

bool CAL_LEVEL_2::simple_calibration()
{
  return SIMPLE_CALIBRATION;
}

void CAL_LEVEL_2::calibration_level_2(unsigned long tmeFrame_Time, FLOAT_XYZ &Raw_XYZ)
{ 

  // Check to see if the history needs to be written.
  if (OFFSET_HISTORY_CHANGED)
  {
    if (OFFSET_HISTORY_TIMER.ping_down(tmeFrame_Time) == false)
    {
      offset_history_write();
      OFFSET_HISTORY_CHANGED = false;
    }
  }

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
              float variance = 0.0f;

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
                  { 
                    // Copy Active Quad values to appropriate QUAD
                    CALIBRATION_QUADS[QUAD].QUAD_DATA.DATA_POINTS.swap(CALIBRATION_QUADS[0].QUAD_DATA.DATA_POINTS);
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

                  OFFSET_HISTORY_CHANGED = true;
                  OFFSET_HISTORY_TIMER.ping_up(tmeFrame_Time, 10000); // 10 second timer to write
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

bool HMC5883L::create(string Offset_History_Filename)
{
  bool ret_success = false;

  CALIBRATED_BEARINGS.clear();
  CALIBRATED_BEARINGS.reserve(CALIBRATED_BEARINGS_SIZE + 1);
  
  LEVEL_2.OFFSET_HISTORY_FILENAME = Offset_History_Filename;
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

void HMC5883L::process(NMEA &GPS_System, unsigned long tmeFrame_Time)
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
    LEVEL_2.calibration_level_2(tmeFrame_Time, RAW_XYZ);
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

  LEVEL_2.BEARING_OFFSET_LOAD = KNOWN_DEVICE_DEGREE_OFFSET;
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
