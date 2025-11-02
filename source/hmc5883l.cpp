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

/**
 * @brief Simulates a fake compass input based on time, and calculates a true fake bearing.
 *
 * This function generates X, Y, and Z values for a FLOAT_XYZ_MATRIX,
 * simulating sensor readings. The values are based on a 'true' bearing
 * that changes over time. This 'true' bearing is also calculated and
 * returned via the TRUE_FAKE_BEARING reference parameter.
 *
 * @param tmeFrame_Time An unsigned long representing the current frame time or elapsed time.
 * This is used to make the simulated bearing dynamic.
 * @param True_Fake_Bearing A reference to a float where the calculated
 * 'true' fake bearing (in degrees) will be stored.
 * This value can be used for comparison after calibration.
 * @return FLOAT_XYZ_MATRIX A matrix containing simulated X, Y, and Z sensor readings.
 */
DOUBLE_XYZ_MATRIX fake_compass_input(unsigned long tmeFrame_Time, double &True_Fake_Bearing)
{
  DOUBLE_XYZ_MATRIX ret_point;

  const double rotation_speed_deg_per_ms = 0.0573f * 1.0f; // Adjusted for degrees (approx 0.0010 rad/ms * 180/PI)

  bool skew = true;
  double skew_factor = 0.20f; // Added 'f' suffix for float literal

  // --- 1. Calculate the TRUE_FAKE_BEARING ahead of time ---
  // This simulates a continuous rotation.
  // We'll use a speed factor to control how fast the bearing changes with time.
  // The result is modulo 360.0f to keep it within a full circle (0 to 360 degrees).
  True_Fake_Bearing = fmod(tmeFrame_Time * rotation_speed_deg_per_ms, 360.0f);

  // Ensure the bearing is always positive (fmod can return negative for negative inputs)
  if (True_Fake_Bearing < 0) 
  {
    True_Fake_Bearing += 360.0f;
  }

  // --- 2. Calculate X, Y, and Z offsets based on the TRUE_FAKE_BEARING ---
  // These values simulate the raw sensor output that would correspond to the bearing.
  // Based on your provided raw data mapping (RAW_XYZ.X = y; RAW_XYZ.Y = x;),
  // we swap the sine and cosine components to match the expected rotation.

  const double magnitude_xy = 250.0f; // Scale factor for X and Y components
  const double x_bias_offset = 000.0f;   // Fixed offset for X
  const double y_bias_offset = 000.0f;  // Fixed offset for Y
  const double z_constant_offset = 000.0f; // Fixed offset for Z (e.g., vertical component)
  //const double x_bias_offset = 200.0f;   // Fixed offset for X
  //const double y_bias_offset = 200.0f;  // Fixed offset for Y
  //const double z_constant_offset = 10.0f; // Fixed offset for Z (e.g., vertical component)


  // Convert True_Fake_Bearing from degrees to radians for std::sin and std::cos
  double True_Fake_Bearing_Rad = True_Fake_Bearing * (M_PI / 180.0f);

  // Swapped sin and cos for X and Y to match the RAW_XYZ.X = y, RAW_XYZ.Y = x mapping
  ret_point.X = (magnitude_xy * std::cos(True_Fake_Bearing_Rad)) + x_bias_offset; // X corresponds to original 'y'
  ret_point.Y = (magnitude_xy * std::sin(True_Fake_Bearing_Rad)) + y_bias_offset; // Y corresponds to original 'x'
  ret_point.Z = z_constant_offset; // Z can be a constant or vary independently

  // Is skew on?
  if (skew)
  {
    // Store original values before skewing to avoid compounding effects
    double original_X = ret_point.X;
    double original_Y = ret_point.Y;

    // Skew the x and y components.
    // This simulates cross-axis interference or non-orthogonality.
    // For example, the X reading is influenced by the true Y value, and vice-versa.
    ret_point.X = original_X + original_Y * skew_factor;
    ret_point.Y = original_Y + original_X * skew_factor;
    // You can adjust the skew_factor or apply different skewing logic
    // to simulate various sensor imperfections.
  }

  return ret_point;
}

// -------------------------------------------------------------------------------------

bool CAL_LEVEL_3::xyz_equal(DOUBLE_XYZ_MATRIX &A, DOUBLE_XYZ_MATRIX &B)
{
  return (A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z);
}

double CAL_LEVEL_3::dist_xyz(DOUBLE_XYZ_MATRIX &A, DOUBLE_XYZ_MATRIX &B)
{
  return sqrtf(powf(A.X - B.X, 2) + powf(A.Y - B.Y, 2) + powf(A.Z - B.Z, 2));
}

void CAL_LEVEL_3::clear_all_flags()
{
  INFORMATION_CALIBRATION = "";
}

bool CAL_LEVEL_3::add_point(DOUBLE_XYZ_MATRIX &Raw_XYZ)
{

  bool ret_pass_filter = false;

  // Simple noise filter
  // If the new point is too far from the last read value, do not store it.
  ret_pass_filter = true;
  
  if (ret_pass_filter)
  {
    double dist_newpoint_anyother = 0.0f;
    double dist_newpoint_anyother_farthest = 9999.9f;

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
      COMPASS_HISTORY.push_back(tmp_compass_point, true);
    }
  }

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
    if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA)
    {
      if (COMPASS_HISTORY[pos].TAG == false)
      {
        COMPASS_HISTORY[pos].TAG = true;
      }
      else
      {
        double dx = COMPASS_HISTORY[pos].POINT.X - COMPASS_CENTER.X;
        double dy = COMPASS_HISTORY[pos].POINT.Y - COMPASS_CENTER.Y;

        // Calculate angle using atan2, which handles all quadrants.
        // The angle is in radians, typically from -PI to PI.
        int angle_slot = static_cast<int>(std::round(std::atan2(dy, dx) * 180.0 / M_PI));
        angle_slot = (angle_slot + 360) % 360;

        if (angle_slot >= 0 && angle_slot < 360)
        {
          preserved_angle_buffer[angle_slot].add(COMPASS_HISTORY[pos].POINT);
          COMPASS_HISTORY.erase_p(pos);
        }
      }
    }
  }

  // Average out similar angles.
  for (int angle = 0; angle < 360; angle++)
  {
    // If angles has more than 1 item in it, combine them.
    if (COMPASS_CALIBRATION_HISTORY.FLAGS[angle].HAS_DATA == false && 
        preserved_angle_buffer[angle].count() > 0)
    {
      COMPASS_POINT tmp_new_point;
      tmp_new_point.POINT = preserved_angle_buffer[angle].average();
      COMPASS_CALIBRATION_HISTORY.put_in_position(angle, tmp_new_point);
      preserved_angle_buffer[angle].clear();
    }
    else if (COMPASS_CALIBRATION_HISTORY.FLAGS[angle].HAS_DATA && 
              preserved_angle_buffer[angle].count() > preservation_of_data_buffer_size)
    {
      COMPASS_POINT tmp_new_point;
      tmp_new_point.POINT =  (COMPASS_CALIBRATION_HISTORY[angle].POINT + preserved_angle_buffer[angle].average()) / 2;
      COMPASS_CALIBRATION_HISTORY.put_in_position(angle, tmp_new_point);
      preserved_angle_buffer[angle].clear();
    }
  }
}


// --- Calibration Core Functions --

// This is no longer relevant because the fit_ellipsoid_and_get_calibration_matrix is flawed. 

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
    INFORMATION_CALIBRATION += "Error: Not enough data points for ellipsoid \n\tfitting. Need at least " +
                    to_string(MIN_POINTS) + ", got " + to_string(active_points.size()) + ".\n";
    params.hard_iron_offset = DOUBLE_XYZ_MATRIX(0,0,0); // Use params
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
    double x = p.X;
    double y = p.Y;
    double z = p.Z;

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
    double b_val = -1.0f;

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
  double min_x = std::numeric_limits<double>::max();
  double max_x = std::numeric_limits<double>::min();
  double min_y = std::numeric_limits<double>::max();
  double max_y = std::numeric_limits<double>::min();
  double min_z = std::numeric_limits<double>::max();
  double max_z = std::numeric_limits<double>::min();

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
  double range_x = max_x - min_x;
  double range_y = max_y - min_y;
  double range_z = max_z - min_z;

  if (range_x == 0.0f || range_y == 0.0f || range_z == 0.0f)
  {
    INFORMATION_CALIBRATION += "Warning: Insufficient range for soft iron \n\tcalibration on one or more axes.\n";
    params.soft_iron_matrix = Matrix3x3(); // Use params
    params.average_field_magnitude = 0.0f; // Initialize
    return false;
  }

  // Calculate average range.
  double average_range = (range_x + range_y + range_z) / 3.0f;

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
 * @brief Performs a 2D ellipse fit to the X and Y coordinates of the data points.
 * Derives the hard-iron offset and a 2D soft-iron transformation matrix.
 *
 * @param history The custom deque of historical compass data points.
 * @param params Output parameter for the calculated hard iron offset and soft iron matrix.
 * @return True if calibration was successful, false otherwise.
 */
bool CAL_LEVEL_3::fit_ellipse_and_get_calibration_matrix_2D(
    const VECTOR_DEQUE_NON_SEQUENTIAL<COMPASS_POINT>& history,
    CalibrationParameters& params)
{
  const int MIN_POINTS = 50; // Fewer points needed for a 2D fit, but still good to have many.

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
    INFORMATION_CALIBRATION += "Error: Not enough data points for 2D ellipse \n\tfitting. Need at least " +
                                to_string(MIN_POINTS) + ", got " + to_string(active_points.size()) + ".\n";
    params.hard_iron_offset = DOUBLE_XYZ_MATRIX(0,0,0);
    params.soft_iron_matrix = Matrix3x3();
    params.average_field_magnitude = 0.0f;
    return false;
  }

  // --- Reset for each calculation ---
  A_transpose_A_2D.assign(5, std::vector<double>(5, 0.0f));
  A_transpose_b_2D.assign(5, 0.0f);
  features_buffer_2D.assign(5, 0.0f);

  for (const auto& p : active_points)
  {
    double x = p.X;
    double y = p.Y;

    // Features for the design matrix A
    // [x^2, xy, y^2, x, y]
    features_buffer_2D[0] = x*x;
    features_buffer_2D[1] = x*y;
    features_buffer_2D[2] = y*y;
    features_buffer_2D[3] = x;
    features_buffer_2D[4] = y;

    // Right-hand side vector for the least-squares problem is -1
    double b_val = -1.0f;

    // Populate A_transpose_A and A_transpose_b
    for (int i = 0; i < 5; ++i)
    {
      A_transpose_b_2D[i] += features_buffer_2D[i] * b_val;
      for (int j = 0; j < 5; ++j)
      {
        A_transpose_A_2D[i][j] += features_buffer_2D[i] * features_buffer_2D[j];
      }
    }
  }

  // --- Solve the linear system ---
  std::vector<std::vector<double>> A_transpose_A_inv = invert5x5(A_transpose_A_2D);

  // Q_coeffs = (A^T * A)^-1 * (A^T * b)
  std::vector<double> Q_coeffs(5, 0.0f);
  for (int i = 0; i < 5; ++i)
  {
    for (int j = 0; j < 5; ++j)
    {
      Q_coeffs[i] += A_transpose_A_inv[i][j] * A_transpose_b_2D[j];
    }
  }

  // --- Extract calibration parameters from the ellipse coefficients ---
  // General equation: Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
  double A = Q_coeffs[0];
  double B = Q_coeffs[1];
  double C = Q_coeffs[2];
  double D = Q_coeffs[3];
  double E = Q_coeffs[4];

  // --- Calculate Hard Iron Offset (Center of Ellipse) ---
  double h_x = (B*E - 2.0f*C*D) / (4.0f*A*C - B*B);
  double h_y = (B*D - 2.0f*A*E) / (4.0f*A*C - B*B);
  params.hard_iron_offset = DOUBLE_XYZ_MATRIX(h_x, h_y, 0.0f);

  // --- Calculate Soft Iron Matrix (2D) ---
  // The soft iron matrix M corrects the shape of the ellipse.
  // The inverse of M*M^T is proportional to the quadratic part of the ellipse matrix.
  // We can use the Cholesky decomposition of the quadratic part of the ellipse matrix to get the correction matrix.
  
  // We'll calculate a 2x2 matrix for the quadratic part
  double det_Q = (4.0f * A * C - B * B) / 4.0f;
  double scale_factor = std::sqrt(std::abs(1.0f / det_Q));
  
  // This is a simplified approach, but should be a good starting point.
  // The correct soft iron matrix involves a Cholesky decomposition of the quadratic part
  // of the ellipsoid equation. For simplicity, we can construct the matrix directly.
  params.soft_iron_matrix.m[0][0] = A;
  params.soft_iron_matrix.m[0][1] = B / 2.0f;
  params.soft_iron_matrix.m[1][0] = B / 2.0f;
  params.soft_iron_matrix.m[1][1] = C;
  params.soft_iron_matrix.m[2][2] = 1.0f; // Keep Z-axis as identity.
  params.soft_iron_matrix = params.soft_iron_matrix * scale_factor;

  // We can also calculate the average field magnitude in the XY plane.
  double avg_mag = 0.0f;
  for (const auto& p : active_points)
  {
    DOUBLE_XYZ_MATRIX corrected_p = p - params.hard_iron_offset;
    corrected_p = params.soft_iron_matrix * corrected_p;
    avg_mag += std::sqrt(corrected_p.X*corrected_p.X + corrected_p.Y*corrected_p.Y);
  }
  params.average_field_magnitude = avg_mag / active_points.size();

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

  if (!fit_ellipse_and_get_calibration_matrix_2D(history, params)) 
  {
    INFORMATION_CALIBRATION += "2D Calibration failed or insufficient data. \n\tUsing default parameters.\n";
    // params already initialized to defaults (0 offset, identity matrix)

    COMPASS_CENTER_PSUDO_ACTIVE = true;
  }
  else
  {
    COMPASS_CENTER_PSUDO_ACTIVE = false;
  }
    
  return params;
}

// This function calculates the calibrated heading using a dampened soft-iron correction.
void CAL_LEVEL_3::calculate_calibrated_heading(const DOUBLE_XYZ_MATRIX& raw_point, const CalibrationParameters& params) 
{
  // Apply hard-iron offset to the raw magnetic data.
  // This removes the constant bias caused by the sensor's internal components.
  DOUBLE_XYZ_MATRIX hard_iron_corrected_point = raw_point - params.hard_iron_offset;

  // --- Calculate the heading with full soft-iron correction ---
  // This uses the soft-iron matrix to scale the magnetic field,
  // making the data more spherical and accurate for heading calculation.
  DOUBLE_XYZ_MATRIX full_corrected_point = params.soft_iron_matrix * hard_iron_corrected_point;
  double full_corrected_heading_rad = std::atan2(full_corrected_point.Y, full_corrected_point.X);
  double full_corrected_heading_deg = wrap_degrees(full_corrected_heading_rad * 180.0f / M_PI);

  // --- Calculate the heading with only hard-iron correction ---
  // This is the baseline heading, which is less precise but more stable.
  double hard_iron_heading_rad = std::atan2(hard_iron_corrected_point.Y, hard_iron_corrected_point.X);
  double hard_iron_heading_deg = wrap_degrees(hard_iron_heading_rad * 180.0f / M_PI);

  // --- Dampen the correction ---
  // The difference between the two headings is the correction provided by the soft-iron matrix.
  // We only apply a portion of this correction to the hard-iron heading to prevent
  // "overcorrection" and maintain a smoother, more stable result.
  double soft_iron_correction_deg = signed_angular_error(hard_iron_heading_deg, full_corrected_heading_deg);
  double dampened_heading = wrap_degrees(hard_iron_heading_deg - (soft_iron_correction_deg / 2.0f));

  // Store the results in the class's report variables.
  HEADING_DEGREES_REPORT = dampened_heading;
  HEADING_DEGREES_REPORT_NON_CALIBRATED = hard_iron_heading_deg;
}

/**
 * @brief Reports the compass heading in degrees.
 * Now uses the globally stored calibration parameters.
 * @param Raw_XYZ The current raw compass reading.
 */
void CAL_LEVEL_3::set_heading_degrees_report(const DOUBLE_XYZ_MATRIX& Raw_XYZ)
{
  calculate_calibrated_heading(Raw_XYZ, current_calibration_params);

  /*
  if (false)
  {
    //INFORMATION_CALIBRATION += std::fixed << std::setprecision(3); // Set precision for output

    INFORMATION_CALIBRATION += "Reported Heading (Calibrated): " + to_string(HEADING_DEGREES_REPORT) + " degrees";
    
    if (true)
    {  
      INFORMATION_CALIBRATION += "\tRaw Angle (from FAKE_INPUT): " + to_string(TRUE_FAKE_BEARING) + " degrees";
      // Calculate and display the difference for easier analysis
      float diff = HEADING_DEGREES_REPORT - FAKE_INPUT;
      // Normalize difference to be within -180 to 180 for easier interpretation
      if (diff > 180.0f) diff -= 360.0f;
      if (diff < -180.0f) diff += 360.0f;
      INFORMATION_CALIBRATION += "\tDiff: " << diff << std::endl;
    }

    // Also print raw and corrected XYZ values for detailed debugging
    INFORMATION_CALIBRATION += "\tRaw XYZ: (" + to_string(Raw_XYZ.X) + ", " + to_string(Raw_XYZ.Y) + ", " + to_string(Raw_XYZ.Z) + ")";
    
    // Apply hard iron correction to raw_XYZ to get intermediate point
    FLOAT_XYZ_MATRIX intermediate_point = Raw_XYZ - current_calibration_params.hard_iron_offset;
    // Apply soft iron correction to intermediate point to get final corrected point
    FLOAT_XYZ_MATRIX final_corrected_point = current_calibration_params.soft_iron_matrix * intermediate_point;

    INFORMATION_CALIBRATION += "\tCorrected XYZ: (" + to_string(final_corrected_point.X) + ", " + to_string(final_corrected_point.Y) + ", " + to_string(final_corrected_point.Z) + ")\n";
    // In a real application, you would store this heading, update a display, etc.
  }
  */
}

void CAL_LEVEL_3::clear()
{
  COMPASS_HISTORY.set_size(COMPASS_HISTORY_SIZE);
  active_points.clear();
  active_points.reserve(COMPASS_CALIBRATION_HISTORY_SIZE);
  //COMPASS_CALIBRATION_HISTORY.set_size(COMPASS_CALIBRATION_HISTORY_SIZE);
  COMPASS_CALIBRATION_HISTORY.clear(COMPASS_CALIBRATION_HISTORY_SIZE);

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

void CAL_LEVEL_3::calibration_level_3(unsigned long tmeFrame_Time, DOUBLE_XYZ_MATRIX &Raw_XYZ, bool Enable_Calibration_Routines)
{
  // Add point to history.  
  // Includes a simple noise filter.
  // Retain the last read value for future reference.
  bool successful_add = add_point(Raw_XYZ);

  // Analyze the points in the history. Only performed during timed interval.
  if (Enable_Calibration_Routines && 
      successful_add && 
      CALIBRATION_TIMER.is_ready(tmeFrame_Time))
  {
    CALIBRATION_TIMER.set(tmeFrame_Time, CALIBRATION_DELAY);

    clear_all_flags();

    if (true)
    {
      // This function will calculate the hard_iron_offset and soft_iron_matrix
      // based on the COMPASS_CALIBRATION_HISTORY.
      current_calibration_params = perform_hard_soft_iron_calibration(COMPASS_CALIBRATION_HISTORY);
      // COMPASS_CENTER is now conceptually replaced by current_calibration_params.hard_iron_offset

      INFORMATION_CALIBRATION += 
                      "Hard Iron Offset:\n\tX=" +  to_string_round_to_nth(current_calibration_params.hard_iron_offset.X, 1) + 
                      "\n\tY=" +                  to_string_round_to_nth(current_calibration_params.hard_iron_offset.Y, 1) +
                      "\n\tZ=" +                  to_string_round_to_nth(current_calibration_params.hard_iron_offset.Z, 1) +
                      "\n";
                      
      INFORMATION_CALIBRATION += "Soft Iron Matrix:\n";
      for (int i = 0; i < 3; ++i) 
      {
        INFORMATION_CALIBRATION += "\t[" +  to_string(current_calibration_params.soft_iron_matrix.m[i][0]) + ", " +
                                            to_string(current_calibration_params.soft_iron_matrix.m[i][1]) + ", " +
                                            to_string(current_calibration_params.soft_iron_matrix.m[i][2]) + "]" + 
                                            "\n";
      }
    }

    if (COMPASS_CENTER_PSUDO_ACTIVE)
    {
      if(PSUDO_CENTER_CLEAR_TIMER.is_ready(tmeFrame_Time))
      {
        // Clear the PSUDO_CENTER every so often if not working.
        PSUDO_CENTER_CLEAR_TIMER.set(tmeFrame_Time, 5 * 60 * 1000);
        PSUDO_CENTER_X.clear(tmeFrame_Time);
        PSUDO_CENTER_Y.clear(tmeFrame_Time);
        PSUDO_CENTER_Z.clear(tmeFrame_Time);
      }

      // Store values of XYZ
      PSUDO_CENTER_X.store_value(Raw_XYZ.X);
      PSUDO_CENTER_Y.store_value(Raw_XYZ.Y);
      PSUDO_CENTER_Z.store_value(Raw_XYZ.Z);

      // Calculate a center based on the means.
      COMPASS_CENTER.X = PSUDO_CENTER_X.mean();
      COMPASS_CENTER.Y = PSUDO_CENTER_Y.mean();
      COMPASS_CENTER.Z = PSUDO_CENTER_Z.mean();
    }
    else
    {
      // Real center found.  Set from Hard Iron Offset.
      COMPASS_CENTER = current_calibration_params.hard_iron_offset;
    }

    preservation_of_data();
  }
  
  // Calculate heading
  set_heading_degrees_report(Raw_XYZ);
}

// -------------------------------------------------------------------------------------

int PRESERVE_ANGLE_CALC::count()
{
  return COUNT;
}

void PRESERVE_ANGLE_CALC::add(DOUBLE_XYZ_MATRIX Add_Point)
{
  POINT_SUM = POINT_SUM + Add_Point;
  COUNT++;
}

DOUBLE_XYZ_MATRIX PRESERVE_ANGLE_CALC::average()
{
  return POINT_SUM / COUNT;
}

void PRESERVE_ANGLE_CALC::clear()
{
  COUNT = 0;
  POINT_SUM.X = 0.0f;
  POINT_SUM.Y = 0.0f;
  POINT_SUM.Z = 0.0f;
}

/**
 * @brief Helper function to perform a 5x5 matrix inversion.
 * A standard Gaussian elimination with partial pivoting is used.
 * @param matrix The 5x5 matrix to invert.
 * @return The inverted 5x5 matrix.
 */
std::vector<std::vector<double>> invert5x5(const std::vector<std::vector<double>>& matrix) 
{
  if (matrix.size() != 5 || matrix[0].size() != 5) 
  {
    std::cerr << "Error: Invalid matrix dimensions for 5x5 inversion." << std::endl;
    std::vector<std::vector<double>> identity(5, std::vector<double>(5, 0.0f));
    for (int i = 0; i < 5; ++i) identity[i][i] = 1.0f;
    return identity;
  }

  std::vector<std::vector<double>> A = matrix;
  std::vector<std::vector<double>> invA(5, std::vector<double>(5, 0.0f));

  // Initialize inverse matrix as an identity matrix
  for (int i = 0; i < 5; ++i) 
  {
    invA[i][i] = 1.0f;
  }

  // Gaussian elimination
  for (int i = 0; i < 5; ++i) 
  {
    // Find pivot for the current column
    double pivot_val = A[i][i];
    int pivot_row = i;
    for (int k = i + 1; k < 5; ++k) 
    {
      if (std::abs(A[k][i]) > std::abs(pivot_val)) 
      {
          pivot_val = A[k][i];
          pivot_row = k;
      }
    }

    // Swap rows if a better pivot is found
    if (pivot_row != i) 
    {
      std::swap(A[i], A[pivot_row]);
      std::swap(invA[i], invA[pivot_row]);
    }
    
    // Normalize the pivot row
    double div = A[i][i];
    if (std::abs(div) < std::numeric_limits<double>::epsilon()) 
    {
      std::cerr << "Error: Matrix is singular. Cannot invert." << std::endl;
      std::vector<std::vector<double>> identity(5, std::vector<double>(5, 0.0f));
      for (int j = 0; j < 5; ++j) identity[j][j] = 1.0f;
      return identity;
    }

    for (int j = 0; j < 5; ++j) 
    {
      A[i][j] /= div;
      invA[i][j] /= div;
    }

    // Eliminate other rows
    for (int k = 0; k < 5; ++k) 
    {
      if (k != i) 
      {
        double factor = A[k][i];
        for (int j = 0; j < 5; ++j) 
        {
          A[k][j] -= factor * A[i][j];
          invA[k][j] -= factor * invA[i][j];
        }
      }
    }
  }
  return invA;
}

void HMC5883L::load_history_and_settings()
{
  bool ret_success = false;

  JSON_INTERFACE configuration_json;

  string json_state_file = file_to_string(PROPS.OFFSET_HISTORY_FILE_NAME, ret_success);

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
          STRING_FLOAT mount_offset;
          STRING_FLOAT declination;
          STRING_INT force_xyz_center_offset;
          STRING_BOOL force_zero_z_axis;

          for (int settings = 0; 
              settings < (int)configuration_json.ROOT.DATA[root].DATA.size(); settings++)
          {
            if (configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("mount offset", mount_offset))
            {
              PROPS.CALIBRATION_MOUNT_OFFSET = mount_offset.get_float_value();
            }
            
            if (configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("declination", declination))
            {
              PROPS.CALIBRATION_LOCATION_DECLINATION = declination.get_float_value();
            }

            if (configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("force_xyz_center_offset", force_xyz_center_offset))
            {
              PROPS.FORCE_XYZ_CENTER_OFFSET = (double)force_xyz_center_offset.get_int_value();
            }

            if (configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("force_zero_z_axis", force_zero_z_axis))
            {
              PROPS.FORCE_ZERO_Z_AXIS = force_zero_z_axis.get_bool_value();
            }
          }

        }
        // load configuration points
        if (configuration_json.ROOT.DATA[root].label() == "calibration points")
        {
          for (int points = 0; 
              points < (int)configuration_json.ROOT.DATA[root].DATA.size(); points++)
          {
            int   angle = -1;
            float x     = -1.0f;
            float y     = -1.0f;
            float z     = -1.0f;

            for (int values = 0; 
                values < (int)configuration_json.ROOT.DATA[root].DATA[points].DATA.size(); values++)
            {
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("angle", angle);
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("X", x);
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Y", y);
              configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Z", z);
            }

            if (angle >= 0 && angle < 360)
            {
              COMPASS_POINT tmp_point;

              tmp_point.POINT.X = x;
              tmp_point.POINT.Y = y;
              tmp_point.POINT.Z = z;

              LEVEL_3.COMPASS_CALIBRATION_HISTORY.put_in_position(angle, tmp_point);
            }
          }
        }
      }
    }
  }
}

void HMC5883L::save_history_and_settings()
{
  bool ret_success = false;
  
  JSON_INTERFACE configuration_json;

  // Create base settings in JSON
  JSON_ENTRY settings;
  settings.create_label_value(quotify("mount offset"), quotify(to_string(PROPS.CALIBRATION_MOUNT_OFFSET)));
  settings.create_label_value(quotify("declination"), quotify(to_string(PROPS.CALIBRATION_LOCATION_DECLINATION)));
  settings.create_label_value(quotify("force_xyz_center_offset"), quotify(to_string((int)PROPS.FORCE_XYZ_CENTER_OFFSET)));
  settings.create_label_value(quotify("force_zero_z_axis"), quotify(to_string(PROPS.FORCE_ZERO_Z_AXIS)));
  configuration_json.ROOT.put_json_in_set(quotify("settings"), settings);

  // Create Point history in JSON
  if (LEVEL_3.COMPASS_CALIBRATION_HISTORY.count() > 10)
  {
    JSON_ENTRY compass_points;
    for (size_t pos = 0; pos < LEVEL_3.COMPASS_CALIBRATION_HISTORY.size(); pos++)
    {
      if (LEVEL_3.COMPASS_CALIBRATION_HISTORY.FLAGS[pos].HAS_DATA)
      {
        JSON_ENTRY single_point;
        single_point.create_label_value(quotify("angle"), quotify(to_string(pos)));
        single_point.create_label_value(quotify("X"), quotify(to_string(LEVEL_3.COMPASS_CALIBRATION_HISTORY[pos].POINT.X)));
        single_point.create_label_value(quotify("Y"), quotify(to_string(LEVEL_3.COMPASS_CALIBRATION_HISTORY[pos].POINT.Y)));
        single_point.create_label_value(quotify("Z"), quotify(to_string(LEVEL_3.COMPASS_CALIBRATION_HISTORY[pos].POINT.Z)));
        compass_points.put_json_in_list(single_point);
      }
    }
    configuration_json.ROOT.put_json_in_set(quotify("calibration points"), compass_points);
  }

  // Save JSON
  deque<string> json_deque;
  configuration_json.json_print_build_to_string_deque(json_deque);
  ret_success = deque_string_to_file(PROPS.OFFSET_HISTORY_FILE_NAME, json_deque, false);

  (void) ret_success;
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

bool HMC5883L::register_read(uint8_t Register, uint8_t &Value)
{
  bool ret_read_success = false;

  // Set register pointer
  if (write(DEVICE, &Register, 1) == 1)
  {
    // Read one byte from the register
    if (read(DEVICE, &Value, 1) == 1)
    {
      ret_read_success = true;
    }
  }

  return ret_read_success;
}

bool HMC5883L::create()
{
  bool ret_success = false;

  CALIBRATED_BEARINGS_SIZE = (int)(COMMS_COMPASS_HISTORY_TIME_SPAN_MS / COMMS_COMPASS_POLLING_RATE_MS); 

  CALIBRATED_BEARINGS.clear();
  CALIBRATED_BEARINGS.reserve(CALIBRATED_BEARINGS_SIZE + 1);
  
  LEVEL_3.clear();
  load_history_and_settings();

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
      /*
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
      */

      // Simple Setup
      {
        register_write(0x00, 0x10); // Configuration Register A
        // 1 sample average, 15 Hz data rate, normal measurement mode

        register_write(0x01, 0x00); // Configuration Register B
        // Gain
        
        /*
        Gain Settings via register_write(0x01, value)
          |  |  |  |  | 
          0  | register_write(0x01, 0x00); |  |  | (Max Range)
          1  | register_write(0x01, 0x20); |  |  | (Default)
          2  | register_write(0x01, 0x40); |  |  | 
          3  | register_write(0x01, 0x60); |  |  | 
          4  | register_write(0x01, 0x80); |  |  | 
          5  | register_write(0x01, 0xA0); |  |  | 
          6  | register_write(0x01, 0xC0); |  |  | 
          7  | register_write(0x01, 0xE0); |  |  | (Max sensitivity)

          Gain 2 = 1.3 Gauss (default), 1090 LSB/Gauss
        */

        register_write(0x02, 0x00); // Mode Register
        // Continuous measurement mode

        ret_success = true;
        CONNECTED = true;
      }

      /*
      // Alt Test
      {
        // Try Gain settings 5, 6, 7
        for (int gain = 5; gain <= 7; ++gain)
        {
            uint8_t crb_val = (gain << 5); // Gain bits are in bits 7:5

            // Step 1: CRA = 0x71 → 8-average, 15Hz, positive bias
            if (!register_write(0x00, 0x71)) {
                std::cerr << "Failed to write CRA\n";
                return 1;
            }

            // Step 2: CRB = gain setting
            if (!register_write(0x01, crb_val)) {
                std::cerr << "Failed to write CRB\n";
                return 1;
            }

            // Step 3: Mode = 0x00 → Continuous mode
            if (!register_write(0x02, 0x00)) {
                std::cerr << "Failed to write Mode\n";
                return 1;
            }

            // Step 4: Wait 6 ms
            usleep(6000);

            // Step 5: Read 6 bytes from data registers
            uint8_t reg = 0x03;
            if (write(DEVICE, &reg, 1) != 1) {
                std::cerr << "Failed to set register pointer\n";
                return 1;
            }

            uint8_t buffer[6];
            if (read(DEVICE, buffer, 6) != 6) {
                std::cerr << "Failed to read data\n";
                return 1;
            }

            int16_t x = (int16_t)((buffer[0] << 8) | buffer[1]);
            int16_t z = (int16_t)((buffer[2] << 8) | buffer[3]);
            int16_t y = (int16_t)((buffer[4] << 8) | buffer[5]);

            std::cout << "\nGain = " << gain << " (CRB = 0x" << std::hex << (int)crb_val << std::dec << ")\n";
            std::cout << "Self-test readings: X=" << x << " Y=" << y << " Z=" << z << "\n";

            // Step 6: Calculate limits based on gain
            int ref_gain = 390; // Gain 5 reference
            int actual_gain = (gain == 5) ? 390 : (gain == 6 ? 330 : 230);
            int low = (243 * actual_gain) / ref_gain;
            int high = (575 * actual_gain) / ref_gain;

            std::cout << "Expected range: " << low << " to " << high << "\n";

            bool x_ok = (x >= low && x <= high);
            bool y_ok = (y >= low && y <= high);
            bool z_ok = (z >= low && z <= high);

            // Step 7: Exit self-test mode
            register_write(0x00, 0x70); // Normal measurement mode

            if (x_ok && y_ok && z_ok) {
                std::cout << "✅ Self-test PASSED at Gain " << gain << "\n";
                return 0;
            } else {
                std::cerr << "❌ Self-test FAILED at Gain " << gain << "\n";
                if (!x_ok) std::cerr << "X out of range\n";
                if (!y_ok) std::cerr << "Y out of range\n";
                if (!z_ok) std::cerr << "Z out of range\n";
            }

            // Wait before next attempt
            usleep(10000);
        }

        std::cerr << "\n❌ Self-test FAILED at all gain levels (5–7)\n";
        exit(0);
      }
      */

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
  LEVEL_3.calibration_level_3(tmeFrame_Time, RAW_XYZ, PROPS.CALIBRATION_ENABLED);
  RAW_BEARING = LEVEL_3.HEADING_DEGREES_REPORT;

  // Adjust bearing for offsets and declination
  // Prepare for Jitter and normalize.
  double bearing = RAW_BEARING + 
                  PROPS.CALIBRATION_MOUNT_OFFSET + 
                  PROPS.CALIBRATION_LOCATION_DECLINATION - 
                  KNOWN_DEVICE_DEGREE_OFFSET; // Adjust to match original code's convention

  // Store uncalibrated bearing (Simple)
  BEARING_NON_CALIBRATED = LEVEL_3.HEADING_DEGREES_REPORT_NON_CALIBRATED + 
                PROPS.CALIBRATION_MOUNT_OFFSET + 
                PROPS.CALIBRATION_LOCATION_DECLINATION - 
                KNOWN_DEVICE_DEGREE_OFFSET; // Adjust to match original code's convention

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
    double bearing_start_value = CALIBRATED_BEARINGS[0];

    double bearing_value = 0.0f;
    double bearing_min = CALIBRATED_BEARINGS[0];
    double bearing_max = CALIBRATED_BEARINGS[0];

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
      BEARING = wrap_degrees(CALIBRATED_BEARINGS.back());

      BEARING_JITTER_MIN = bearing_min;
      BEARING_JITTER_MAX = bearing_max;
    }
  }

  // Calibration compass heading with GPS heading
  if (GPS_System.active(tmeFrame_Time) && GPS_System.CURRENT_POSITION.TRUE_HEADING.VALID) // Enable
  {
    // save error to error mean variable
    double error_current = signed_angular_error(bearing, GPS_System.CURRENT_POSITION.TRUE_HEADING.VALUE);
    
    // Reset the GPS_ERROR_MEAN every 30 minutes
    //  Rotation over 3 would be better, but this will work for now.
    if (GPS_ERROR_MEAN_RESET_TIMER.is_ready(tmeFrame_Time))
    {
      GPS_ERROR_MEAN_RESET_TIMER.set(tmeFrame_Time, 30 * 60 * 1000);
      if (GPS_ERROR_MEAN.samples() > 0)
      {
        double tmp_prv_value = GPS_ERROR_MEAN.mean();
        GPS_ERROR_MEAN.clear(tmeFrame_Time);
        GPS_ERROR_MEAN.store_value(tmp_prv_value);
      }
    }
    else
    {
      GPS_ERROR_MEAN.store_value(error_current);
    }
    
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

  // Save compass history and settings on timed inteval
  // Initial calibration at 10 minutes.  CALIBRATION_DATA_SAVE_DELAY afterwards.
  if (CALIBRATION_DATA_SAVE.is_ready(tmeFrame_Time))
  {
    CALIBRATION_DATA_SAVE.set(tmeFrame_Time, CALIBRATION_DATA_SAVE_DELAY);
    save_history_and_settings();
  }

  if (CALIBRATE)
  {
    double calibration_correction = signed_angular_error(bearing_calibrated(), bearing_non_calibrated());

    INFORMATION = "";

    if (PROPS.ENABLE_FAKE_COMPASS == false)
    {
      INFORMATION += "  BEARING CALIBRATED: " + to_string_round_to_nth(bearing_calibrated(), 1) + "\n";
      INFORMATION += "      NON_CALIBRATED: " + to_string_round_to_nth(bearing_non_calibrated(), 1) + "\n";
      INFORMATION += "          ADJUSTMENT: " + to_string_round_to_nth(calibration_correction, 1) + "\n";
    }
    else
    {
      double error_non_cali  = signed_angular_error(TRUE_FAKE_BEARING, bearing_non_calibrated());
      double error_cali      = signed_angular_error(TRUE_FAKE_BEARING, bearing_calibrated());
      INFORMATION += "        BEARING FAKE: " + to_string_round_to_nth(bearing_non_true_fake(), 1) + "\n";
      INFORMATION += "      NON_CALIBRATED: " + to_string_round_to_nth(bearing_non_calibrated(), 1) + "\n";
      INFORMATION += "  BEARING CALIBRATED: " + to_string_round_to_nth(bearing_calibrated(), 1) + "\n";
      INFORMATION += "          ADJUSTMENT: " + to_string_round_to_nth(calibration_correction, 1) + "\n";
      INFORMATION += "     TRUE FAKE ERROR: " + to_string_round_to_nth(error_non_cali, 1) + "\n";
      INFORMATION += "     TRUE CALI ERROR: " + to_string_round_to_nth(error_cali, 1) + "\n";
    }

    INFORMATION += "COMPASS HISTORY SIZE: " + to_string(LEVEL_3.COMPASS_HISTORY.count()) + "\n";
    INFORMATION += "COMPASS CALIBRA SIZE: " + to_string(LEVEL_3.COMPASS_CALIBRATION_HISTORY.count()) + "\n";
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

      // Set initial calibration save routine to wait 10 minutes 
      //  to allow it time before it potentially overwrites good data.
      if (ENABLE_TEST_COMPASS == false)
      {
        CALIBRATION_DATA_SAVE.set(tmeFrame_Time, 10 * 60 * 1000);
      }
      else
      {
        CALIBRATION_DATA_SAVE.set(tmeFrame_Time, 10 * 1000);
      }

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

        RAW_DEVICE_X = x;
        RAW_DEVICE_Y = y;
        RAW_DEVICE_Z = z;

        RAW_XYZ.X = (double)y;
        RAW_XYZ.Y = (double)x;
        RAW_XYZ.Z = (double)z;

        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 5000);   // Looking for live data

        // Set to true to use fake compass input for testing
        //  by overwriting the read valules.
        if (PROPS.ENABLE_FAKE_COMPASS)
        {
          RAW_XYZ = fake_compass_input(tmeFrame_Time, TRUE_FAKE_BEARING);

          TRUE_FAKE_BEARING -= (PROPS.CALIBRATION_MOUNT_OFFSET + 
                                PROPS.CALIBRATION_LOCATION_DECLINATION - 
                                KNOWN_DEVICE_DEGREE_OFFSET); // Adjust to match original code's convention
          LEVEL_3.TRUE_FAKE_BEARING_FROM_UPPER = TRUE_FAKE_BEARING;
        }

        // Force offsets
        RAW_XYZ.X += PROPS.FORCE_XYZ_CENTER_OFFSET;
        RAW_XYZ.Y += PROPS.FORCE_XYZ_CENTER_OFFSET;
        RAW_XYZ.Z += PROPS.FORCE_XYZ_CENTER_OFFSET;

        // Force 0 on z axis.
        if (PROPS.FORCE_ZERO_Z_AXIS)
        {
          RAW_XYZ.Z = 0;
        }

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

void HMC5883L::bearing_known_offset_calibration_to_gps()
{
  // Only set the from GPS mean if half a minute of data is accumulated
  if (GPS_ERROR_MEAN.samples() > 500)
  {
    KNOWN_DEVICE_DEGREE_OFFSET = GPS_ERROR_MEAN.mean();
  }
}

void HMC5883L::bearing_known_offset_clear()
{
  KNOWN_DEVICE_DEGREE_OFFSET = 0.0f;
  GPS_ERROR_MEAN.clear(0);
}

double HMC5883L::bearing_known_offset()
{
  return KNOWN_DEVICE_DEGREE_OFFSET;
}

double HMC5883L::accumulated_gps_to_compass_bearing_error()
{
  return GPS_ERROR_MEAN.mean();
}

double HMC5883L::bearing_calibrated()
{
  return wrap_degrees(BEARING);
}

double HMC5883L::bearing_non_calibrated()
{
  return wrap_degrees(BEARING_NON_CALIBRATED);
}

double HMC5883L::bearing_non_true_fake()
{
  return wrap_degrees(TRUE_FAKE_BEARING);
}
        
double HMC5883L::bearing_jitter_min()
{
  return wrap_degrees(BEARING_JITTER_MIN);
}

double HMC5883L::bearing_jitter_max()
{
  return wrap_degrees(BEARING_JITTER_MAX);
}


#endif
