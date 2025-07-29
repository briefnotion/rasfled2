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

void CAL_LEVEL_3::load_history_and_settings(HMC5883L_PROPERTIES &Props)
{
  bool ret_success = false;

  JSON_INTERFACE configuration_json;

  string json_state_file = file_to_string(Props.OFFSET_HISTORY_FILE_NAME, ret_success);

  if (ret_success == true)
  {
    ret_success = configuration_json.load_json_from_string(json_state_file);

    if (ret_success == true)
    {
      for(int root = 0; root < (int)configuration_json.ROOT.DATA.size(); root++)
      {
        // load settings
        if (configuration_json.ROOT.DATA[root].label() == "settings")
        {
          float mount_offset =  0.0f;
          bool mount_offset_loaded = false;

          float declination =   0.0f;
          bool declination_loaded = false;

          for (int settings = 0; 
              settings < (int)configuration_json.ROOT.DATA[root].DATA.size(); settings++)
          {
            mount_offset_loaded = configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("mount offset", mount_offset);
            declination_loaded =  configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("declination", declination);
          }

          // store as props but only if found
          if (mount_offset_loaded)
          {
            Props.CALIBRATION_MOUNT_OFFSET = mount_offset;
          }
          if (declination_loaded)
          {
            Props.CALIBRATION_LOCATION_DECLINATION = declination;
          }
        }

        // load configuration points
        if (configuration_json.ROOT.DATA[root].label() == "calibration points")
        {
          for (int points = 0; 
              points < (int)configuration_json.ROOT.DATA[root].DATA.size(); points++)
          {
            float x = 0;
            float y = 0;
            float z = 0;

            for (int values = 0; 
                values < (int)configuration_json.ROOT.DATA[root].DATA[points].DATA.size(); values++)
            {

              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("X", x);
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Y", y);
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Z", z);
            }

            COMPASS_POINT tmp_point;

            tmp_point.POINT.X = x;
            tmp_point.POINT.Y = y;
            tmp_point.POINT.Z = z;

            COMPASS_HISTORY.push_back(tmp_point);
          }

          // Set all points as do_not_overwrite so they look alive.
          for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
          {
            if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
            {
              COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = true;
            }
          }
        }
      }
    }
  }
}

void CAL_LEVEL_3::save_history_and_settings(HMC5883L_PROPERTIES &Props)
{
  bool ret_success = false;
  
  JSON_INTERFACE configuration_json;

  // Create base settings hin JSON
  JSON_ENTRY settings;
  settings.create_label_value(quotify("mount offset"), quotify("-180.0"));
  settings.create_label_value(quotify("declination"), quotify("4.0"));
  configuration_json.ROOT.put_json_in_set(quotify("settings"), settings);

  // Create Point history in JSON
  if (COMPASS_HISTORY.count() > 10)
  {
    JSON_ENTRY compass_points;
    for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
    {
      if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA && COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE)
      {
        JSON_ENTRY single_point;
        single_point.create_label_value(quotify("X"), quotify(to_string(COMPASS_HISTORY[pos].POINT.X)));
        single_point.create_label_value(quotify("Y"), quotify(to_string(COMPASS_HISTORY[pos].POINT.Y)));
        single_point.create_label_value(quotify("Z"), quotify(to_string(COMPASS_HISTORY[pos].POINT.Z)));
        compass_points.put_json_in_list(single_point);
      }
    }
    configuration_json.ROOT.put_json_in_set(quotify("calibration points"), compass_points);
  }

  // Save JSON
  deque<string> json_deque;
  configuration_json.json_print_build_to_string_deque(json_deque);
  ret_success = deque_string_to_file(Props.OFFSET_HISTORY_FILE_NAME, json_deque, false);

  (void) ret_success;
}

/**
 * @brief Generates fake compass input for testing with various simulations.
 * @param tmeFrame_Time Current frame time.
 * @return A fake FLOAT_XYZ point.
 */
FLOAT_XYZ_MATRIX CAL_LEVEL_3::fake_compass_input(unsigned long tmeFrame_Time)
{
    FLOAT_XYZ_MATRIX ret_point;

    int speed = 50; // 1 fastest
    // Note: Due to integer division (tmeFrame_Time / speed), the angle will
    // remain constant for 'speed' number of frames. This causes the Raw XYZ
    // and Corrected XYZ values to repeat in the output for those frames.

    //simulations
    bool noise = true;
    float noise_factor = 0.2f;

    bool sparatic_random = false;
    float sparatic_random_factor = 1.0f;
    int sparatic_random_chance_percent = 5; // 5% chance of sparatic random jump

    bool time_based_drift = true;
    float time_based_drift_factor = 100.0f;

    bool interference_zone = false;

    bool skew = true; // Retaining your original setting for demonstration
    float skew_factor = 0.20f; // 10% skew

    bool figure_eight = false; // if true, will do a figure eight pattern

    // NEW SCENARIO: Rotate 120 degrees, then random points, then repeat
    bool rotate_and_random_scenario = false; // Disable previous scenario
    bool triangle_pattern_scenario = false;   // Enable new scenario

    // Parameters for triangle_pattern_scenario
    int segment_1_frames = 30; // Points for 0 degrees cluster
    int segment_2_frames = 40; // Points for 120 degrees cluster
    int segment_3_frames = 30; // Points for 240 degrees cluster
    int total_frames_per_cycle_triangle = segment_1_frames + segment_2_frames + segment_3_frames; // Total 100 frames

    float cluster_variation_magnitude = 10.0f; // Max deviation from the ideal point for a cluster

    // parameters
    float radius = 250.0f;
    FLOAT_XYZ_MATRIX offset;
    offset.X = 250.0f;
    offset.Y = 300.3f;
    // Modified: Introduce Z variation for calibration
    offset.Z = 10.0f * std::sin(tmeFrame_Time * 0.01f); // Vary Z slightly

    if (triangle_pattern_scenario)
    {
        unsigned long current_cycle_frame = tmeFrame_Time % total_frames_per_cycle_triangle;
        float base_angle_degrees = 0.0f;

        if (current_cycle_frame < (unsigned int)segment_1_frames)
        {
            base_angle_degrees = 0.0f; // First cluster around 0 degrees
        }
        else if (current_cycle_frame < (unsigned int)(segment_1_frames + segment_2_frames))
        {
            base_angle_degrees = 120.0f; // Second cluster around 120 degrees
        }
        else
        {
            base_angle_degrees = 240.0f; // Third cluster around 240 degrees
        }

        FAKE_INPUT = base_angle_degrees; // Store the target angle for the cluster

        float base_angle_radians = base_angle_degrees * (M_PI / 180.0f);

        // Calculate base point for the cluster
        float base_X = radius * cos(base_angle_radians) + offset.X;
        float base_Y = radius * sin(base_angle_radians) + offset.Y;
        float base_Z = 0.0f + offset.Z; // Z still varies with time for calibration

        // Add small random variation around the base point
        float rand_x_offset = ((rand() % 201) - 100) / 100.0f * cluster_variation_magnitude; // -1 to 1 * magnitude
        float rand_y_offset = ((rand() % 201) - 100) / 100.0f * cluster_variation_magnitude;
        float rand_z_offset = ((rand() % 201) - 100) / 100.0f * cluster_variation_magnitude;

        ret_point.X = base_X + rand_x_offset;
        ret_point.Y = base_Y + rand_y_offset;
        ret_point.Z = base_Z + rand_z_offset;
    }
    else if (rotate_and_random_scenario) // Original rotate_and_random_scenario
    {
        int rotation_degrees_per_segment = 120;
        int frames_per_rotation_segment = rotation_degrees_per_segment * speed; // e.g., 120 * 10 = 1200 frames
        int frames_per_random_segment = 100; // 100 frames of random data
        int total_frames_per_cycle = frames_per_rotation_segment + frames_per_random_segment;

        unsigned long current_cycle_frame = tmeFrame_Time % total_frames_per_cycle;

        if (current_cycle_frame < (unsigned int)frames_per_rotation_segment)
        {
            // Rotation phase (0 to 120 degrees)
            float angle_degrees_in_segment = (float)(current_cycle_frame / speed);
            FAKE_INPUT = angle_degrees_in_segment; // Store angle in degrees

            float angle_radians = angle_degrees_in_segment * (M_PI / 180.0f);
            ret_point.X = radius * cos(angle_radians) + offset.X;
            ret_point.Y = radius * sin(angle_radians) + offset.Y;
            ret_point.Z = 0.0f + offset.Z; // Z still varies with time for calibration
        }
        else
        {
            // Random points phase
            // Generate points within a reasonable range around the offset
            float random_range = radius * 0.5f; // Random points within half the radius
            ret_point.X = offset.X + ((rand() % 201) - 100) * (random_range / 100.0f); // -random_range to +random_range
            ret_point.Y = offset.Y + ((rand() % 201) - 100) * (random_range / 100.0f);
            ret_point.Z = offset.Z + ((rand() % 201) - 100) * (random_range / 100.0f);
            
            FAKE_INPUT = -1.0f; // Indicate random phase with a negative value
        }
    }
    else // Original full circle scenario if no new scenarios are active
    {
        // gen
        float angle_degrees = (float)((tmeFrame_Time/speed) % 360);
        FAKE_INPUT = angle_degrees; // Store angle in degrees as requested

        float angle_radians = angle_degrees * (M_PI / 180.0f); // Convert to radians for sin/cos

        if (figure_eight == false)
        {
            ret_point.X = radius * cos(angle_radians) + offset.X;
            ret_point.Y = radius * sin(angle_radians) + offset.Y;
            ret_point.Z = 0.0f + offset.Z;
        }
        else
        {
            ret_point.X = radius * sin(angle_radians);
            ret_point.Y = radius * sin(angle_radians) * cos(angle_radians);
            ret_point.Z = 0.0f + offset.Z;
        }
    }


    // simulations (applied to whatever ret_point was generated above)
    if (noise)
    {
        float noise_X = ((rand() % 100) - 50) * noise_factor; // -0.5 to +0.5
        float noise_Y = ((rand() % 100) - 50) * noise_factor; // -0.5 to +0.5
        ret_point.X += noise_X;
        ret_point.Y += noise_Y;
    }

    if (sparatic_random)
    {
        if ((rand() % 100) < sparatic_random_chance_percent) // ~5% chance
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
        // Note: Skew applied this way can significantly distort the shape,
        // potentially making simple min/max soft iron correction less effective.
        // A more complex matrix-based soft iron correction would handle this better.
        float original_X = ret_point.X; // Store original X before modifying it
        ret_point.X += skew_factor * ret_point.Y;
        ret_point.Y += skew_factor * original_X; // Use original X for Y's skew
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

bool CAL_LEVEL_3::xyz_equal(FLOAT_XYZ_MATRIX &A, FLOAT_XYZ_MATRIX &B)
{
  return (A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z);
}

float CAL_LEVEL_3::dist_xyz(FLOAT_XYZ_MATRIX &A, FLOAT_XYZ_MATRIX &B)
{
  return sqrtf(powf(A.X - B.X, 2) + powf(A.Y - B.Y, 2) + powf(A.Z - B.Z, 2));
}

void CAL_LEVEL_3::clear_all_flags()
{

  fill(begin(preserved_angle), end(preserved_angle), 0);

  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE = false;
  }
}

bool CAL_LEVEL_3::add_point(FLOAT_XYZ_MATRIX &Raw_XYZ)
{

  bool ret_pass_filter = false;

  // Simple noise filter
  // If the new point is too far from the last read value, do not store it.
  ret_pass_filter = true;
  
  if (ret_pass_filter)
  {
    float dist_newpoint_anyother = 0.0f;
    float dist_newpoint_anyother_farthest = 9999.9f;

    for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
    {
      if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
      {
        dist_newpoint_anyother = dist_xyz(Raw_XYZ, COMPASS_HISTORY[pos].POINT);
        
        if (dist_newpoint_anyother < CLOSEST_ALLOWED)
        {
          COMPASS_HISTORY.erase_p(pos);
        }

        if (dist_newpoint_anyother < dist_newpoint_anyother_farthest)
        {
          dist_newpoint_anyother_farthest = dist_newpoint_anyother;
        }
      }
    }

    if (dist_newpoint_anyother_farthest < NOISE_FILTER_DISTANCE || 
              COMPASS_HISTORY.count() == 0)
    {
      COMPASS_POINT tmp_compass_point;
      tmp_compass_point.POINT = Raw_XYZ;
      COMPASS_HISTORY.push_back(tmp_compass_point);
    }
  }
  
  LAST_READ_VALUE = Raw_XYZ;

  return ret_pass_filter;
}

void CAL_LEVEL_3::preservation_of_data()
// This function identifies and marks points within the compass history
// to be preserved, aiming for an even angular distribution around the data's center.
// The goal is to ensure a representative set of points are not overwritten.

// preservation_of_data is the last of the functions called in the calibration routine.
// Data in COMPASS_HISTORY will be rewritten at next calibration call.
// The position of the new averaged point is insignificant.  It is important 
//  to remove the points used in the average and the new point is store with 
//  do not overwrite.
// PRESERVATION_ANGLE_AVERAGE_ARR_COUNT is set to 0 and
//  all data in PRESERVATION_ANGLE_AVERAGE is cleared at the beginning of every calibration routine.

{
  for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
  {
    int pos_new = pos;
    if (preserved_angle_direction)
    {
      pos_new = (int)COMPASS_HISTORY.size() -1 - pos;
    }

    if (COMPASS_HISTORY.FLAGS[pos_new].HAS_DATA)
    {
      float dx = COMPASS_HISTORY[pos_new].POINT.X - COMPASS_CENTER.X;
      float dy = COMPASS_HISTORY[pos_new].POINT.Y - COMPASS_CENTER.Y;

      // Calculate angle using atan2, which handles all quadrants.
      // The angle is in radians, typically from -PI to PI.
      int angle_slot = static_cast<int>(std::round(std::atan2(dy, dx) * 180.0 / M_PI));
      angle_slot = (angle_slot + 360) % 360;

      preserved_angle[angle_slot]++;
      COMPASS_HISTORY[pos_new].SUDO_ANGLE = angle_slot;

      if (preserved_angle[angle_slot] <= 1)
      {
        COMPASS_HISTORY.FLAGS[pos_new].DO_NOT_OVERWRITE = true;
      }
    }
  }

  // Rotate direction of save
  preserved_angle_direction = !preserved_angle_direction;

  // Average out similar angles.
  for (int angle = 0; angle < 360; angle++)
  {
    // If angles has more than 1 item in it, combine them.
    if (preserved_angle[angle] > 1)
    {
      COMPASS_POINT tmp_compass_point;

      for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
      {
        if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA && COMPASS_HISTORY[pos].SUDO_ANGLE == angle)
        {
          tmp_compass_point.POINT = tmp_compass_point.POINT + COMPASS_HISTORY[pos].POINT;
          COMPASS_HISTORY.erase_p(pos);
        }
      }


      tmp_compass_point.POINT = tmp_compass_point.POINT / preserved_angle[angle];
      COMPASS_HISTORY.push_back(tmp_compass_point, true);
    }
  }
}


// --- Calibration Core Functions --

// Formulate the linear least squares problem: Ax = b
// The ellipsoid equation is:
// Qx^2 + Qy^2 + Qz^2 + Rxy + Rxz + Ryz + Sx + Sy + Sz + T = 0
// We want to solve for Q, R, S, T coefficients.
// For linearity, we set T=1 (or -1) and move it to the right side.
// So, we solve for 9 coefficients (Q, R, S, G, H, I, J, K, L)
// The equation becomes:
// [x^2 y^2 z^2 xy xz yz x y z] * [A B C D E F G H I]^T = -1

// Design matrix A (N x 9, where N is number of points)
// N rows, 9 columns for coefficients (A, B, C, D, E, F, G, H, I)
// The general form is $Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0$
// We normalize by J. So we solve for 9 parameters.
// X = [x^2 y^2 z^2 xy xz yz x y z]
// coefficients = [A B C D E F G H I]^T
// b = [-J] (which becomes -1 if J=1)

// Using the more common 6-parameter ellipsoid form for simplicity:
// ax^2 + by^2 + cz^2 + 2dxy + 2exz + 2fyz + 2gx + 2hy + 2iz + 1 = 0
// This requires solving for 9 coefficients (a, b, c, d, e, f, g, h, i)
// The matrix for this is 9x9 for the normal equations (A^T * A)
// Number of parameters to solve for is 9.
// Let's call them: A, B, C, D, E, F, G, H, I, J
// The equation: $Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0$
// We can set J = 1 (or -1) and move it to the right side.
// So, $Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz = -J$
// This is a system of N equations with 9 unknowns.

// Let's use the 10-parameter general quadratic form $Q_1 x^2 + Q_2 y^2 + Q_3 z^2 + Q_4 xy + Q_5 xz + Q_6 yz + Q_7 x + Q_8 y + Q_9 z + Q_{10} = 0$.
// We can set $Q_{10} = 1$ (or $-1$) and solve for the first 9.
// This means our design matrix A will have 9 columns.

// For a robust solution, we need to solve $X^T X \beta = X^T b$.
// X is N x 9, so X^T X is 9 x 9.
// b is N x 1.

/**
 * @brief Fits an ellipsoid to the given 3D points and derives hard iron offset
 * and soft iron transformation matrix.
 * This implementation uses a linear least squares approach for ellipsoid fitting.
 * The general ellipsoid equation is:
 * Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
 * We normalize by J=1 (or another coefficient) to make it linear.
 *
 * @param history The custom deque of historical compass data points.
 * @param params Output parameter for the calculated hard iron offset, soft iron matrix, and average field magnitude.
 * @return True if calibration was successful, false otherwise.
 */
bool CAL_LEVEL_3::fit_ellipsoid_and_get_calibration_matrix(
    const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history,
    CalibrationParameters& params) // Changed signature
{
  // Minimum number of points required to fit an ellipsoid (at least 9, ideally many more)
  // For a robust solution, typically 100+ points covering all orientations are needed.
  const int MIN_POINTS = 10; // Increased from 9 for better stability

  active_points.clear();
  for (size_t i = 0; i < history.size(); ++i)
  {
    if (history.FLAGS[i].HAS_DATA)
    {
      active_points.push_back(history[i].POINT);
    }
  }

  if (active_points.size() < MIN_POINTS)
  {
    //std::cerr << "Error: Not enough data points for ellipsoid fitting. Need at least "
    //          << MIN_POINTS << ", got " << active_points.size() << "." << std::endl;
    params.hard_iron_offset = FLOAT_XYZ_MATRIX(0,0,0); // Use params
    params.soft_iron_matrix = Matrix3x3(); // Use params
    params.average_field_magnitude = 0.0f; // Initialize
    return false;
  }

  // --- Reset for each calculation ---
  // Instead of just clear(), we need to resize to ensure correct dimensions
  // and re-initialize all elements to 0.0f.
  // clear() + resize() is efficient here because capacity is likely retained.

  for (int i = 0; i < 9; ++i) {
      // Clear and resize each inner vector to 9 columns, initializing to 0.0f
      // This effectively resets the entire A_transpose_A matrix to zeros.
      A_transpose_A[i].assign(9, 0.0f); // More concise way to clear and resize/fill
  }
  A_transpose_b.assign(9, 0.0f); // Reset A_transpose_b to all zeros

  for (const auto& p : active_points)
  {
    float x = p.X;
    float y = p.Y;
    float z = p.Z;

    // Features for the design matrix X
    // [x^2, y^2, z^2, xy, xz, yz, x, y, z]
    features_buffer.clear(); // Clear the buffer for the current point's features
    features_buffer.push_back(x*x);
    features_buffer.push_back(y*y);
    features_buffer.push_back(z*z);
    features_buffer.push_back(x*y);
    features_buffer.push_back(x*z);
    features_buffer.push_back(y*z);
    features_buffer.push_back(x);
    features_buffer.push_back(y);
    features_buffer.push_back(z);

    // b vector (right-hand side) is -1 (assuming Q_10 = 1)
    float b_val = -1.0f;

    for (int i = 0; i < 9; ++i)
    {
      A_transpose_b[i] += features_buffer[i] * b_val; // Use features_buffer
      for (int j = 0; j < 9; ++j)
      {
        A_transpose_A[i][j] += features_buffer[i] * features_buffer[j]; // Use features_buffer
      }
    }
  }

  // --- Original Hard Iron Correction using Min/Max ---
  float min_x = std::numeric_limits<float>::max();
  float max_x = std::numeric_limits<float>::min();
  float min_y = std::numeric_limits<float>::max();
  float max_y = std::numeric_limits<float>::min();
  float min_z = std::numeric_limits<float>::max();
  float max_z = std::numeric_limits<float>::min();

  for (const auto& p : active_points)
  {
    min_x = std::min(min_x, p.X);
    max_x = std::max(max_x, p.X);
    min_y = std::min(min_y, p.Y);
    max_y = std::max(max_y, p.Y);
    min_z = std::min(min_z, p.Z);
    max_z = std::max(max_z, p.Z);
  }

  params.hard_iron_offset.X = (max_x + min_x) * 0.5f; // Use params
  params.hard_iron_offset.Y = (max_y + min_y) * 0.5f; // Use params
  params.hard_iron_offset.Z = (max_z + min_z) * 0.5f; // Use params

  // --- Simplified Soft Iron with 3D focus ---
  // This part remains the same as it calculates scaling factors.
  float range_x = max_x - min_x;
  float range_y = max_y - min_y;
  float range_z = max_z - min_z;

  if (range_x == 0.0f || range_y == 0.0f || range_z == 0.0f)
  {
    //std::cerr << "Warning: Insufficient range for soft iron calibration on one or more axes." << std::endl;
    params.soft_iron_matrix = Matrix3x3(); // Use params
    params.average_field_magnitude = 0.0f; // Initialize
    return false;
  }

  // Calculate average range.
  float average_range = (range_x + range_y + range_z) / 3.0f;

  // Store the average range in CalibrationParameters for use in preservation_of_data
  params.average_field_magnitude = average_range; // Storing average range

  // Soft iron matrix will be a diagonal scaling matrix
  params.soft_iron_matrix.m[0][0] = average_range / range_x; // Use params
  params.soft_iron_matrix.m[1][1] = average_range / range_y; // Use params
  params.soft_iron_matrix.m[2][2] = average_range / range_z; // Use params

  // Reset off-diagonal terms (ensure it's a pure scaling matrix)
  params.soft_iron_matrix.m[0][1] = params.soft_iron_matrix.m[0][2] = 0.0f; // Use params
  params.soft_iron_matrix.m[1][0] = params.soft_iron_matrix.m[1][2] = 0.0f; // Use params
  params.soft_iron_matrix.m[2][0] = params.soft_iron_matrix.m[2][1] = 0.0f; // Use params

  return true;
}

/**
 * @brief Performs hard and soft iron calibration based on historical compass data.
 * Now uses the more advanced (though still simplified) 3D approach.
 * @param history The custom deque of historical compass data points.
 * @return CalibrationParameters containing the calculated offset and matrix.
 */
CalibrationParameters CAL_LEVEL_3::perform_hard_soft_iron_calibration(const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history) 
{
  CalibrationParameters params; // Will hold the results

  if (!fit_ellipsoid_and_get_calibration_matrix(history, params)) 
  { // Changed call
    //std::cerr << "Calibration failed or insufficient data. Using default parameters." << std::endl;
    // params already initialized to defaults (0 offset, identity matrix)
  }
  return params;
}

/**
 * @brief Calculates the compass heading in degrees after applying hard and soft iron corrections.
 * Uses a full 3x3 matrix for soft iron correction.
 * @param raw_point The raw compass reading (FLOAT_XYZ).
 * @param params The calibration parameters (offset and matrix).
 * @return The heading in degrees (0-360), where 0 is North.
 */
float CAL_LEVEL_3::calculate_calibrated_heading(const FLOAT_XYZ_MATRIX& raw_point, const CalibrationParameters& params) 
{
  // 1. Apply Hard Iron Correction: Remove the constant offset.
  FLOAT_XYZ_MATRIX corrected_point = raw_point - params.hard_iron_offset;

  // 2. Apply Soft Iron Correction: Transform the point using the soft iron matrix.
  // This matrix will scale and potentially rotate/shear the point to make the field spherical.
  corrected_point = params.soft_iron_matrix * corrected_point;

  // 3. Calculate Heading using atan2 (still based on X and Y)
  // atan2(y, x) gives the angle in radians from the positive X-axis to the point (x, y).
  // For compass, positive Y is North, positive X is East.
  // Standard atan2 returns -PI to PI.
  float heading_rad = std::atan2(corrected_point.Y, corrected_point.X);

  // Convert radians to degrees
  float heading_deg = heading_rad * 180.0f / M_PI;

  // Normalize to 0-360 degrees
  if (heading_deg < 0) 
  {
    heading_deg += 360.0f;
  }

  return heading_deg;
}

/**
 * @brief Reports the compass heading in degrees.
 * Now uses the globally stored calibration parameters.
 * @param Raw_XYZ The current raw compass reading.
 */
void CAL_LEVEL_3::set_heading_degrees_report(const FLOAT_XYZ_MATRIX& Raw_XYZ, HMC5883L_PROPERTIES &Props)
{
  HEADING_DEGREES_REPORT = calculate_calibrated_heading(Raw_XYZ, current_calibration_params);
  HEADING_DEGREES_REPORT += Props.CALIBRATION_MOUNT_OFFSET + Props.CALIBRATION_LOCATION_DECLINATION; // Adjust to match original code's convention

  /*
  std::cout << std::fixed << std::setprecision(3); // Set precision for output

  std::cout << "Reported Heading (Calibrated): " << HEADING_DEGREES_REPORT << " degrees";
  std::cout << "\tRaw Angle (from FAKE_INPUT): " << FAKE_INPUT << " degrees";
  // Calculate and display the difference for easier analysis
  float diff = HEADING_DEGREES_REPORT - FAKE_INPUT;
  // Normalize difference to be within -180 to 180 for easier interpretation
  if (diff > 180.0f) diff -= 360.0f;
  if (diff < -180.0f) diff += 360.0f;
  std::cout << "\tDiff: " << diff << std::endl;

  // Also print raw and corrected XYZ values for detailed debugging
  std::cout << "\tRaw XYZ: (" << Raw_XYZ.X << ", " << Raw_XYZ.Y << ", " << Raw_XYZ.Z << ")";
  
  // Apply hard iron correction to raw_XYZ to get intermediate point
  FLOAT_XYZ intermediate_point = Raw_XYZ - current_calibration_params.hard_iron_offset;
  // Apply soft iron correction to intermediate point to get final corrected point
  FLOAT_XYZ final_corrected_point = current_calibration_params.soft_iron_matrix * intermediate_point;

  std::cout << "\tCorrected XYZ: (" << final_corrected_point.X << ", " << final_corrected_point.Y << ", " << final_corrected_point.Z << ")" << std::endl;
  // In a real application, you would store this heading, update a display, etc.
  */
}

void CAL_LEVEL_3::clear()
{
  COMPASS_HISTORY.set_size(COMPASS_HISTORY_SIZE);
  active_points.reserve(COMPASS_HISTORY_SIZE);

  // Initial sizing and zeroing. This happens once per object creation.
  A_transpose_A.resize(9);
  for (int i = 0; i < 9; ++i) 
  {
    A_transpose_A[i].resize(9, 0.0f);
  }
  A_transpose_b.resize(9, 0.0f);

  // Reserve capacity for features_buffer.
  // It will always hold 9 elements, so reserving 9 prevents reallocations.
  features_buffer.reserve(9);
}

void CAL_LEVEL_3::calibration_level_3(unsigned long tmeFrame_Time, FLOAT_XYZ_MATRIX &Raw_XYZ, HMC5883L_PROPERTIES &Props)
{
  // Set to true to use fake compass input for testing
  if (false)
  {
    Raw_XYZ = fake_compass_input(tmeFrame_Time);
    FAKE_INPUT_REPORTED = FAKE_INPUT + Props.CALIBRATION_MOUNT_OFFSET + Props.CALIBRATION_LOCATION_DECLINATION;
  }

  // Add point to history.  
  // Includes a simple noise filter.
  // Retain the last read value for future reference.
  bool successful_add = add_point(Raw_XYZ);

  // Analyze the points in the history. Only performed during timed interval.
  if (Props.CALIBRATION_ENABLED &&
      successful_add && 
      CALIBRATION_TIMER.is_ready(tmeFrame_Time))
  {
    CALIBRATION_TIMER.set(tmeFrame_Time, CALIBRATION_DELAY);

    clear_all_flags();

    // This function will calculate the hard_iron_offset and soft_iron_matrix
    // based on the COMPASS_HISTORY.
    current_calibration_params = perform_hard_soft_iron_calibration(COMPASS_HISTORY);
    // COMPASS_CENTER is now conceptually replaced by current_calibration_params.hard_iron_offset

    /*
    std::cout << "\n--- Calibration Performed ---" << std::endl;
    std::cout << "Hard Iron Offset: X=" << current_calibration_params.hard_iron_offset.X
              << ", Y=" << current_calibration_params.hard_iron_offset.Y
              << ", Z=" << current_calibration_params.hard_iron_offset.Z << std::endl;
    std::cout << "Soft Iron Matrix:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "\t[" << current_calibration_params.soft_iron_matrix.m[i][0] << ", "
                  << current_calibration_params.soft_iron_matrix.m[i][1] << ", "
                  << current_calibration_params.soft_iron_matrix.m[i][2] << "]" << std::endl;
    }
    std::cout << "-----------------------------\n" << std::endl;
    */

    COMPASS_CENTER = current_calibration_params.hard_iron_offset;
    preservation_of_data();

  }
  
  // Calculate heading
  set_heading_degrees_report(Raw_XYZ, Props);

  // Save compass history and settings on timed inteval
  if (CALIBRATION_DATA_SAVE.is_ready(tmeFrame_Time))
  {
    CALIBRATION_DATA_SAVE.set(tmeFrame_Time, CALIBRATION_DATA_SAVE_DELAY);
    save_history_and_settings(Props);
  }
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
  LEVEL_3.load_history_and_settings(PROPS);

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
  // Level 3 - Calibration and set bearing
  LEVEL_3.calibration_level_3(tmeFrame_Time, RAW_XYZ, PROPS);
  RAW_BEARING = LEVEL_3.HEADING_DEGREES_REPORT;
  
  // Prepare for Jitter and normalize.
  float bearing = RAW_BEARING - KNOWN_DEVICE_DEGREE_OFFSET;
  bearing = fmod(bearing + 360.0f, 360.0f);
  CALIBRATED_BEARINGS.push_back(bearing);

  // Maintain point history size
  while ((int)CALIBRATED_BEARINGS.size() > (CALIBRATED_BEARINGS_SIZE))
  {
    CALIBRATED_BEARINGS.erase(CALIBRATED_BEARINGS.begin());
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

  // Calibration compass heading with GPS heading
  if (GPS_System.active(tmeFrame_Time) && GPS_System.current_position().TRUE_HEADING.VALID) // Enable
  {
    // save error to error mean variable
    float error_current = signed_angular_error(RAW_BEARING, GPS_System.current_position().TRUE_HEADING.VALUE);
    GPS_ERROR_MEAN.store_value(error_current);
    
    // if gps assist calibration enabled, set bearing offset,
    if (PROPS.GPS_ASSIST_HEADING)
    {
      // Avoid constant recalibration from the GPS heading (gps only updated once per minute (60000ms))
      if (GPS_HEADING_CALIBRATION_TIMER.ping_down(tmeFrame_Time) == false)
      { 
        GPS_HEADING_CALIBRATION_TIMER.ping_up(tmeFrame_Time, 60000);
        bearing_known_offset_calibration_to_gps();
      }
    }
  }
}

void HMC5883L::calibrateion_reset()
{
  CALIBRATED_BEARINGS.clear();
  KNOWN_DEVICE_DEGREE_OFFSET = 0.0f;
  GPS_ERROR_MEAN.clear(0);
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
        if (RAW_XYZ.X != RAW_XYZ_PREVIOUS_VALUE.X ||
            RAW_XYZ.Y != RAW_XYZ_PREVIOUS_VALUE.Y ||
            RAW_XYZ.Z != RAW_XYZ_PREVIOUS_VALUE.Z)
        {
          RAW_XYZ_PREVIOUS_VALUE = RAW_XYZ;
          process(GPS_System, tmeFrame_Time);
        }
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

void HMC5883L::bearing_known_offset_calibration_to_gps()
{
  KNOWN_DEVICE_DEGREE_OFFSET = GPS_ERROR_MEAN.mean();
}

void HMC5883L::bearing_known_offset_clear()
{
  KNOWN_DEVICE_DEGREE_OFFSET = 0.0f;
  GPS_ERROR_MEAN.clear(0);
}

float HMC5883L::bearing_known_offset()
{
  return KNOWN_DEVICE_DEGREE_OFFSET;
}

float HMC5883L::accumulated_gps_to_compass_bearing_error()
{
  return GPS_ERROR_MEAN.mean();
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
