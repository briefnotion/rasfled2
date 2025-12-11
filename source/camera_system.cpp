// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: camera_system.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef CAMARA_SYSTEM_CPP
#define CAMARA_SYSTEM_CPP

#include "camera_system.h"

using namespace std;

// ---------------------------------------------------------------------------------------
/*

Notes:
  OpenCV does not support all camera  controls, and camera controls differ on cameras.

  To install v4l2-ctl command, if not already present:
  sudo apt install v4l-utils

  List available camera controls, labels, addreess, ranges, and defaults.
  v4l2-ctl -d /dev/video0 --list-ctrls

  code that may work in c:
  system("v4l2-ctl -d /dev/video0 --set-ctrl=focus_automatic_continuous=0");
  system("v4l2-ctl -d /dev/video0 --set-ctrl=focus_absolute=20");

  code that does work on command line, after camera is running:
  v4l2-ctl -d /dev/video0 --set-ctrl=focus_automatic_continuous=0
  v4l2-ctl -d /dev/video0 --set-ctrl=focus_absolute=20

*/
// ---------------------------------------------------------------------------------------

void resize_if_not_same_size(cv::Mat& Main_Frame, cv::Mat& Resize_Frame)
{
  if (Main_Frame.cols != Resize_Frame.cols || Main_Frame.rows != Resize_Frame.rows)
  {
    resize(Resize_Frame, Resize_Frame, cv::Size(Main_Frame.cols, Main_Frame.rows));
  }
}

// ---------------------------------------------------------------------------------------

// Helper function to create the coordinate mesh grid (unchanged from original intent)
void FAKE_FRAME::createMeshGrid(int rows, int cols, cv::Mat& mesh_x, cv::Mat& mesh_y)
{
  // Create 1D coordinate vectors
  cv::Mat x_coords = cv::Mat::zeros(1, cols, CV_32FC1);
  for (int i = 0; i < cols; ++i) { x_coords.at<float>(0, i) = (float)i; }

  cv::Mat y_coords = cv::Mat::zeros(rows, 1, CV_32FC1);
  for (int i = 0; i < rows; ++i) { y_coords.at<float>(i, 0) = (float)i; }

  // Expand to 2D mesh grids
  cv::repeat(x_coords, rows, 1, mesh_x);
  cv::repeat(y_coords, 1, cols, mesh_y);
}

// --- Initialization and Allocation (Slightly Cleaned, Functionally Same) ---
void FAKE_FRAME::re_int_vars(const cv::Mat& frame)
{
  PROCESS_WIDTH = frame.cols;
  PROCESS_HEIGHT = frame.rows;
  
  // Set flow size to 1/4th of process size (e.g., 640/4 = 160)
  FLOW_CALC_WIDTH = PROCESS_WIDTH / 4; 
  FLOW_CALC_HEIGHT = PROCESS_HEIGHT / 4; 

  cv::Size process_size(PROCESS_WIDTH, PROCESS_HEIGHT);
  cv::Size flow_calc_size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT);

  // Note: create() only re-allocates if size/type changes.
  current_gray_.create(process_size, CV_8UC1);
  prev_gray_.create(process_size, CV_8UC1);
  small_prev_gray_.create(flow_calc_size, CV_8UC1);
  small_current_gray_.create(flow_calc_size, CV_8UC1);
  flow_small_.create(flow_calc_size, CV_32FC2);
  flow_upsampled_.create(process_size, CV_32FC2);
  map_x_.create(process_size, CV_32FC1);
  map_y_.create(process_size, CV_32FC1);
  prev_frame_.create(process_size, frame.type());

  // 3. Pre-calculate the coordinate mesh grids
  createMeshGrid(PROCESS_HEIGHT, PROCESS_WIDTH, coords_x_, coords_y_);
}

void FAKE_FRAME::preprocess_initial_frame(const cv::Mat& initial_frame)
{
  // Check the initialization flag
  if (!is_initialized_)
  {
    // 1. Determine Dimensions, Allocate Buffers, and Create Mesh Grid
    re_int_vars(initial_frame); // Reuse the re_int_vars logic
    
    // Mark as initialized
    is_initialized_ = true;
  }
  
  // 4. Handle initial BGR frame state
  initial_frame.copyTo(prev_frame_);
  
  // Convert the initial frame to gray for the 'current' gray state
  cv::cvtColor(prev_frame_, current_gray_, cv::COLOR_BGR2GRAY);
  
  // The very first frame is the previous frame for the next calculation.
  // OPTIMIZATION: Use cv::swap to set prev_gray_ without data copy.
  cv::swap(current_gray_, prev_gray_); 
}

// --- Optimized Interpolation Function ---
cv::Mat FAKE_FRAME::interpolateFrame(const cv::Mat& current_frame) 
{
  // Check if the frame size has changed unexpectedly.
  if (current_frame.cols != PROCESS_WIDTH || current_frame.rows != PROCESS_HEIGHT) 
  {
    // Handle uninitialized case or size mismatch gracefully
    if (!is_initialized_ || PROCESS_WIDTH == 0) 
    {
      preprocess_initial_frame(current_frame);
      return current_frame;
    } 
    else 
    {
      // Re-initialize all variables for the new size
      re_int_vars(current_frame);
    }
  }

  // --- State Update OPTIMIZATION: Use cv::swap ---
  // 1. Current becomes previous (Zero-copy swap)
  // The data currently in current_gray_ (which was the previous frame's gray data) 
  // is now in prev_gray_. This is much faster than copyTo.
  cv::swap(prev_gray_, current_gray_);
  
  // 2. Process the new current BGR frame
  // Convert the new frame to gray, overwriting the old data now in current_gray_
  cv::cvtColor(current_frame, current_gray_, cv::COLOR_BGR2GRAY);

  // 3. Downsample for FAST Optical Flow Calculation
  // Ensure the interpolation method is explicitly set for downsampling for consistency.
  cv::resize(prev_gray_, small_prev_gray_, cv::Size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT), 0, 0, cv::INTER_AREA); // INTER_AREA is best for decimation
  cv::resize(current_gray_, small_current_gray_, cv::Size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT), 0, 0, cv::INTER_AREA);

  // 4. Calculate Optical Flow (Most expensive step, kept on downsampled image)
  //cv::calcOpticalFlowFarneback(small_prev_gray_, small_current_gray_, flow_small_, 0.5, 3, 10, 3, 5, 1.2, 0);
  cv::calcOpticalFlowFarneback(small_prev_gray_, small_current_gray_, flow_small_, 0.5, 2, 10, 3, 5, 1.2, 0);
  
  // 5. Upsample the flow field
  cv::resize(flow_small_, flow_upsampled_, cv::Size(PROCESS_WIDTH, PROCESS_HEIGHT), 0, 0, cv::INTER_NEAREST);

  // 6. Define the combined scaling constant
  // The flow needs to be scaled up by the ratio, and then halved for interpolation (dx/2)
  const float scale_factor = (float)PROCESS_WIDTH / FLOW_CALC_WIDTH; 
  const float scaling_constant = scale_factor * 0.5f; 
  
  // --- OPTIMIZATION: Vectorized Map Generation without intermediate Mats ---

  // 7. Split the 2-channel upsampled flow map into X and Y components
  std::vector<cv::Mat> flow_channels(2);
  cv::split(flow_upsampled_, flow_channels); // flow_channels[0] = dx, flow_channels[1] = dy
  
  // 8. Calculate map_x and map_y using cv::addWeighted (Eliminates flow_scaled, half_dx, half_dy matrices)
  // map_x = coords_x - dx * scaling_constant
  // map_x_ = 1.0 * coords_x_ + (-scaling_constant) * flow_channels[0] + 0.0
  cv::addWeighted(coords_x_, 1.0, flow_channels[0], -scaling_constant, 0.0, map_x_);
  
  // map_y = coords_y - dy * scaling_constant
  // map_y_ = 1.0 * coords_y_ + (-scaling_constant) * flow_channels[1] + 0.0
  cv::addWeighted(coords_y_, 1.0, flow_channels[1], -scaling_constant, 0.0, map_y_);

  // 9. Perform the geometric transformation (Interpolation)
  cv::Mat interpolated_frame = cv::Mat::zeros(prev_frame_.size(), prev_frame_.type());
  cv::remap(prev_frame_, interpolated_frame, map_x_, map_y_, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

  // 10. Update the BGR class state for the next iteration
  current_frame.copyTo(prev_frame_); 
  
  return interpolated_frame;
}

// ---------------------------------------------------------------------------------------

void FRAME_SUITE_EXTRA::advance_latest()
{
  if (LATEST_POS == 0)
  {
    LATEST_POS = 1;
  }
  else
  {
    LATEST_POS = 0;
  }
}

// ---------------------------------------------------------------------------------------

// Helper function implementation: checks the mean brightness of the frame
int CAMERA::is_low_light(const cv::Mat& Grey_Image_Full_Size) 
{
  // Calculate the mean intensity (0-255). If it's below the threshold, it's dark.
  cv::Scalar mean_intensity = cv::mean(Grey_Image_Full_Size);
  return (int)mean_intensity[0];
}

void  CAMERA::gray_enhance(cv::Mat& processed_frame, const cv::Mat& Grey_Image_Full_Size)
{
  // CLAHE settings: Clip Limit 2.0, Tile Size 8x8
  cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
  clahe->apply(Grey_Image_Full_Size, Grey_Image_Full_Size); 

  // Apply enhanced luminance back to the color frame (making it grayscale enhanced)
  cv::cvtColor(Grey_Image_Full_Size, processed_frame, cv::COLOR_GRAY2BGR);
}

/**
 * @brief Applies aggressive contrast enhancement (CLAHE) and Gamma Correction 
 * to the Luminance channel of a color image to simulate a powerful low-light filter.
 * * @param processed_frame The BGR image to be modified (input and output).
 * @param Grey_Image_Full_Size (Ignored for this color approach, included for signature compatibility).
 */
void CAMERA::low_light_filter(cv::Mat& processed_frame)
{
  // --- Configuration ---
  const double CLAHE_CLIP_LIMIT = 4.0; // Increased limit for stronger local contrast

  // 1. Convert BGR frame to YUV color space
  cv::Mat yuv_image;
  // We work on the existing 'processed_frame' (which should be the BGR input)
  cv::cvtColor(processed_frame, yuv_image, cv::COLOR_BGR2YUV); 

  // 2. Split the YUV image into its component channels
  std::vector<cv::Mat> yuv_channels;
  cv::split(yuv_image, yuv_channels);
  
  cv::Mat& luminance_channel = yuv_channels[0]; // Reference to the Y (Luminance) channel

  // 3. Apply Aggressive CLAHE (Contrast Limited Adaptive Histogram Equalization)
  cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(CLAHE_CLIP_LIMIT, cv::Size(8, 8));
  clahe->apply(luminance_channel, luminance_channel); 

  // 4. Apply GAMMA CORRECTION for overall brightness boost (focused on dark areas)
  
  // Create a lookup table (LUT) for the gamma transformation (Y_out = Y_in ^ gamma)
  cv::Mat lookup_table(1, 256, CV_8U);
  uchar* p = lookup_table.data;
  
  for (int i = 0; i < 256; ++i) 
  {
    // Compute new value: (i / 255.0) ^ gamma * 255.0
    p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, PROPS.ENH_LOW_LIGHT_GAMMA) * 255.0);
  }
  
  // Apply the gamma correction to the already CLAHE-enhanced luminance channel
  cv::LUT(luminance_channel, lookup_table, luminance_channel);

  // 5. Merge the enhanced Y channel back with the original U and V channels
  cv::merge(yuv_channels, yuv_image);
  
  // 6. Convert the enhanced YUV image back to BGR and overwrite the output frame
  cv::cvtColor(yuv_image, processed_frame, cv::COLOR_YUV2BGR);
}

// Rewritten function to perform Min-Max Contrast Stretching (Low Light Boost)
void CAMERA::apply_min_max_contrast(cv::Mat& processed_frame) 
{
  // Function rewritten to perform Min-Max Contrast Stretching (Low Light Boost).
  // This technique finds the current darkest (min) and brightest (max) pixels 
  // for each color channel and linearly scales all values so that min becomes 0 
  // and max becomes 255, maximizing contrast.
  
  // --- 1. Split BGR image into channels ---
  // We apply the stretch to Blue, Green, and Red channels independently.
  std::vector<cv::Mat> bgr_channels;
  cv::split(processed_frame, bgr_channels);
  
  // --- 2. Process each channel independently (Min-Max Stretching) ---
  for (size_t i = 0; i < bgr_channels.size(); ++i)
  {
    cv::Mat channel = bgr_channels[i];
    
    // Find the actual minimum and maximum pixel values in the current channel
    double min_val, max_val;
    cv::minMaxLoc(channel, &min_val, &max_val);
    
    // Critical: Check to prevent division by zero if the image channel is uniform (e.g., all one color)
    if (min_val == max_val)
    {
      // If min equals max, the channel is uniform, no stretching is needed.
      continue; 
    }
    
    // Calculate the scaling factor (alpha) and offset (beta) for the transformation:
    // Output = alpha * Input + beta
    
    // 1. Calculate Scaling Factor (alpha): This determines how much to multiply the range by
    // to stretch it from (max_val - min_val) to 255.
    double alpha = 255.0 / (max_val - min_val);
    
    // 2. Calculate Offset (beta): This shifts the minimum value (min_val) down to 0.
    double beta = -min_val * alpha;
    
    // Apply the transformation: O = alpha * I + beta. 
    // The result is an 8-bit image where the original min is 0 and max is 255.
    channel.convertTo(channel, -1, alpha, beta);
  }

  // --- 3. Merge channels back into the processed_frame ---
  cv::merge(bgr_channels, processed_frame);
}

/**
 * @brief RECOMMENDED: Detects and draws circles using the Hough Circle Transform.
 * * This method is far superior for finding circular shapes compared to general 
 * contour analysis, especially when dealing with noisy or low-contrast images.
 * * @param processed_frame The input/output BGR frame. Circles will be drawn on it.
 */
void CAMERA::detect_hough_circles(cv::Mat& processed_frame, const cv::Mat& Processed_Frame_Gaussian) 
{
  // --- 2. Find Circles using Hough Transform ---
  std::vector<cv::Vec3f> circles;
  
  // Parameters:
  // 1. image: Input image (8-bit, single-channel, grayscale)
  // 2. circles: Output vector of circles (x, y, radius)
  // 3. method: Detection method (HOUGH_GRADIENT is currently the only one)
  // 4. dp: Inverse ratio of the accumulator resolution (1 = same resolution as input)
  // 5. minDist: Minimum distance between the centers of the detected circles (prevents duplicate detections)
  // 6. param1: Canny edge detection high threshold (the low threshold is half of this)
  // 7. param2: Accumulator threshold for the Canny gradient output (smaller means more circles found, higher means stricter)
  // 8. minRadius: Minimum radius (in pixels) to search for
  // 9. maxRadius: Maximum radius (in pixels) to search for
  cv::HoughCircles(Processed_Frame_Gaussian, circles, cv::HOUGH_GRADIENT, 
                    1,         // dp: Default (accumulator same resolution as input)
                    30,        // minDist: Min distance between centers (e.g., 30 pixels)
                    100,       // param1: Canny high threshold 
                    30,        // param2: Accumulator threshold (Tune this! Lower if you're missing circles)
                    5,         // minRadius: Start checking from 5 pixels
                    100        // maxRadius: Stop checking at 100 pixels
  );

  // --- 3. Draw the detected circles ---
  for (size_t i = 0; i < circles.size(); i++)
  {
    cv::Vec3i c = circles[i];
    cv::Point center = cv::Point(c[0], c[1]);
    int radius = c[2];

    // Draw the circle center (Red dot)
    // cv::circle(processed_frame, center, 3, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);
    
    // Draw the circle outline (Green: B=0, G=255, R=0)
    cv::circle(processed_frame, center, radius, cv::Scalar(255, 255, 0), 2, cv::LINE_AA);
  }
}

cv::Mat CAMERA::canny_mask(cv::Mat& Processed_Frame_Canny)
{
  const int DILATE_ITERATIONS = 0;  // Number of times to thicken edges

  // --- Step 4: Explicitly thicken edges ---
  cv::Mat thickened;
  cv::dilate(Processed_Frame_Canny, thickened, cv::Mat(), cv::Point(-1, -1), DILATE_ITERATIONS);

  // --- Step 5: Convert to mask (255 where edges exist) ---
  cv::Mat mask;
  cv::compare(thickened, 0, mask, cv::CMP_GT); // mask = 255 where edges > 0

  return mask;
}

// Implementation of the image stylization filter: quantize_and_outline.
// It detects black outlines via adaptive thresholding and overlays them 
// directly onto the original image, preserving the original color palette.
// This version is significantly faster as it removes the Bilateral Filter.
//
// Arguments:
// - processed_frame: Input/Output image (cv::Mat, modified in place)
cv::Mat CAMERA::overlay_lines(cv::Mat& Processed_Frame_Gray_Downsized)
{
    // --- Fixed Parameters ---
    const int EDGE_BLUR_KSIZE = 7;    // Median Blur: Noise reduction and simplification for edges
    const int EDGE_BLOCK_SIZE = 5;    // Adaptive Threshold: Local neighborhood size
    const int EDGE_C = 2;             // Adaptive Threshold: Line thickness control (higher = thicker)

    // --- Step 1: Find Edges (Adaptive Thresholding) ---
    // 1a. Apply median blur for clean input edges 
    cv::Mat blurred;
    cv::medianBlur(Processed_Frame_Gray_Downsized, blurred, EDGE_BLUR_KSIZE); 

    // 1b. Use Adaptive Thresholding to find strong edges and make them black.
    cv::Mat edges;
    // 'edges' is created with 0 (black) for lines and 255 (white) for background
    cv::adaptiveThreshold(blurred, 
                          edges, 
                          255, 
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                          cv::THRESH_BINARY, 
                          EDGE_BLOCK_SIZE, 
                          EDGE_C);
    
    // Create a 1-channel mask where edge pixels (which are 0 in 'edges') are 255.
    cv::Mat line_mask;
    // line_mask is 255 where edges == 0 (the black line pixels)
    cv::compare(edges, 0, line_mask, cv::CMP_EQ); 

    return line_mask;
}

// It detects areas in the image that are close to pure white (glare/overexposure)
// and returns a mask that can be used to set these areas to black.
//
// Arguments:
// - processed_frame: Input image (cv::Mat, read-only)
// Returns:
// - cv::Mat: A 1-channel binary mask (CV_8UC1) where 255 represents a glare pixel.
cv::Mat CAMERA::suppress_glare_mask(cv::Mat& processed_frame)
{
  cv::Mat glare_mask;
  
  // Use cv::inRange to find pixels where the B, G, and R values are all 
  // within the range [GLARE_THRESHOLD, 255].
  cv::inRange(processed_frame, 
              cv::Scalar(PROPS.GLARE_THRESHOLD, PROPS.GLARE_THRESHOLD, PROPS.GLARE_THRESHOLD), 
              cv::Scalar(255, 255, 255), 
              glare_mask);

  // glare_mask is now a 1-channel mask where 255 indicates a glare pixel.
  return glare_mask;
}

  // Helper function 2: Extracts and returns ONLY the glare areas (3-channel BGR image).
  // This function is for visualization/analysis, or for processing the glare area specifically.
  //
  // Arguments:
  // - processed_frame: Input image (cv::Mat, read-write, though used as read-only here)
  // Returns:
  // - cv::Mat: A 3-channel BGR image containing only the masked glare pixels.
  cv::Mat CAMERA::extract_glare_area(cv::Mat& processed_frame)
  {
    // 1. Generate the binary glare mask (0 or 255).
    cv::Mat glare_mask;
    cv::inRange(processed_frame, 
                cv::Scalar(PROPS.GLARE_THRESHOLD, PROPS.GLARE_THRESHOLD, PROPS.GLARE_THRESHOLD), 
                cv::Scalar(255, 255, 255), 
                glare_mask);

    // 2. Isolate glare pixels using copyTo. This initializes glare_only_image to black 
    //    and copies pixels from processed_frame only where glare_mask is 255.
    cv::Mat glare_only_image(processed_frame.size(), processed_frame.type(), cv::Scalar(0, 0, 0));
    processed_frame.copyTo(glare_only_image, glare_mask);

    // --- ENHANCEMENT: Min-Max Contrast Stretching (Low Light Boost) ---
    
    // Split BGR image into channels for per-channel dynamic stretching
    std::vector<cv::Mat> bgr_channels;
    cv::split(glare_only_image, bgr_channels);
    
    // Declare min/max variables outside the loop (minor optimization)
    double min_val_ignored, max_val;
    const double min_val = PROPS.GLARE_THRESHOLD; // Fixed minimum stretching threshold

    // Process each channel independently (Min-Max Stretching)
    for (size_t i = 0; i < bgr_channels.size(); ++i)
    {
      cv::Mat channel = bgr_channels[i];
      
      // Find the maximum pixel values in the current channel's glare area.
      cv::minMaxLoc(channel, &min_val_ignored, &max_val);

      // Critical: Check if the stretching range is valid. 
      if (max_val <= min_val)
      {
        // If max is less than or equal to min, skip the enhancement.
        continue; 
      }
      
      // Calculate the scaling factor (alpha) and offset (beta) for the transformation:
      // Output = alpha * Input + beta
      double alpha = 255.0 / (max_val - min_val);
      double beta = -min_val * alpha;
      
      // --- OPTIMIZATION: Use Lookup Table (LUT) for faster per-pixel application ---
      cv::Mat lut(1, 256, CV_8UC1);
      uchar* p = lut.data;
      for (int j = 0; j < 256; ++j) 
      {
        // Apply the linear transformation O = alpha * I + beta
        p[j] = cv::saturate_cast<uchar>(round(alpha * j + beta));
      }
      
      // Apply the pre-calculated LUT to the entire channel image (in-place modification).
      cv::LUT(channel, lut, channel);
    }

    // Merge channels back into the glare_only_image
    cv::merge(bgr_channels, glare_only_image);
    
    return glare_only_image;
  }

// ---------------------------------------------------------------------------------------
  
bool CAMERA::set_control(uint32_t id, int32_t value)
{
  bool ret_success = false;

  // --- Step 1: open V4L2 device ---
  int fd = open(PROPS.CAMERA_DEVICE_NAME.c_str(), O_RDWR);
  if (fd != -1) 
  {
    struct v4l2_control control;
    std::memset(&control, 0, sizeof(control));
    control.id = id;
    control.value = value;
    if (ioctl(fd, VIDIOC_S_CTRL, &control) != -1)
    {
      ret_success = true;
    }
    close(fd);
  }

  return ret_success;
}

int CAMERA::get_control(uint32_t id) 
{
  int ret_value = -1;
  
  int fd = open(PROPS.CAMERA_DEVICE_NAME.c_str(), O_RDWR);
  if (fd != -1) 
  {
    struct v4l2_control control;
    std::memset(&control, 0, sizeof(control));
    control.id = id;
    if (ioctl(fd, VIDIOC_G_CTRL, &control) != -1) 
    {
      ret_value = control.value;
    }
    close(fd);
  }

  return ret_value;
}

cv::Mat CAMERA::generate_empty_frame(int width, int height)
{
  // This function creates a new OpenCV matrix (frame) that is entirely black.

  // --- Parameters ---
  // width: The desired width of the output frame in pixels.
  // height: The desired height of the output frame in pixels.
  
  // --- Implementation ---
  
  // 1. Define the dimensions and type for the new matrix.
  // CV_8UC3: 8-bit unsigned integer, 3 Channels (Standard BGR color format).
  
  // 2. Use cv::Mat::zeros to create a matrix initialized entirely to zero.
  // For a BGR image, initializing to 0, 0, 0 results in a black pixel.
  cv::Mat empty_frame = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
  
  return empty_frame;
}

// Assuming this is inside your CAMERA class or a namespace
cv::Mat CAMERA::generateDummyFrame(int width, int height) 
{
  cv::Mat dummy(height, width, CV_8UC3);
  
  // 1. Generate the initial color pattern
  for (int y = 0; y < dummy.rows; ++y)
  {
    for (int x = 0; x < dummy.cols; ++x)
    {
      // Original color pattern
      dummy.at<cv::Vec3b>(y, x) = cv::Vec3b(x % 256, y % 256, (x + y) % 256);
    }
  }
  
  // 2. Define text drawing properties
  int font = cv::FONT_HERSHEY_SIMPLEX;
  double scale = 1.5;
  int thickness = 2;
  // Use White color for text (B, G, R)
  cv::Scalar color(255, 255, 255); 
  
  // Calculate basic positions (adjust as needed for margins)
  int margin = 20;
  int centerX = width / 2;
  int centerY = height / 2;

  // A utility function to get text size for better placement
  auto getTextSize = [&](const std::string& text) 
  {
    int baseline = 0;
    return cv::getTextSize(text, font, scale, thickness, &baseline);
  };

  // 3. Draw "UP" (Top Center)
  std::string text_up = "UP";
  cv::Size size_up = getTextSize(text_up);
  cv::putText(dummy, text_up, 
      cv::Point(centerX - size_up.width / 2, margin + size_up.height), // Center aligned at top margin
      font, scale, color, thickness);

  // 4. Draw "DOWN" (Bottom Center)
  std::string text_down = "DOWN";
  cv::Size size_down = getTextSize(text_down);
  cv::putText(dummy, text_down, 
      cv::Point(centerX - size_down.width / 2, height - margin), // Center aligned at bottom margin
      font, scale, color, thickness);

  // 5. Draw "LEFT" (Mid Left)
  std::string text_left = "LEFT";
  cv::Size size_left = getTextSize(text_left);
  cv::putText(dummy, text_left, 
      cv::Point(margin, centerY + size_left.height / 2), // Vertical center aligned at left margin
      font, scale, color, thickness);

  // 6. Draw "RIGHT" (Mid Right)
  std::string text_right = "RIGHT";
  cv::Size size_right = getTextSize(text_right);
  cv::putText(dummy, text_right, 
      cv::Point(width - margin - size_right.width, centerY + size_right.height / 2), // Right aligned at right margin
      font, scale, color, thickness);
  
  return dummy;
}

/**
 * @brief Generates a dummy frame with a dynamic bouncing text element, a grid background, random noise, and spinning lines.
 * * This version uses a dark background and bright foreground elements for high contrast.
 */
cv::Mat CAMERA::generateDummyFrame_2(int width, int height, int frame_index) 
{
  cv::Mat dummy(height, width, CV_8UC3);
  
  // Set initial BLACK background
  dummy = cv::Scalar(0, 0, 0); 

  // --- ADD GRID BACKGROUND TO EXAGGERATE WARPING EFFECTS ---
  int grid_spacing = 30;
  cv::Scalar grid_color(255, 255, 255); // White grid lines
  
  // Draw vertical lines
  for (int x = 0; x < width; x += grid_spacing) {
      cv::line(dummy, cv::Point(x, 0), cv::Point(x, height), grid_color, 1, cv::LINE_AA);
  }

  // Draw horizontal lines
  for (int y = 0; y < height; y += grid_spacing) {
      cv::line(dummy, cv::Point(0, y), cv::Point(width, y), grid_color, 1, cv::LINE_AA);
  }
  // --- END GRID GENERATION ---
  
  // --- ADD STATIC (RANDOM GAUSSIAN NOISE) ---
  cv::Mat noise(height, width, CV_8UC3);
  cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(15));
  // Add noise to the dark background
  dummy += noise; 
  // --- END STATIC GENERATION ---

  // --- ADD SPINNING LINES ---
  cv::Point center(width / 2, height / 2);
  int num_lines = 8;
  int line_length = std::min(width, height) / 3;
  double angle_step = 2 * CV_PI / num_lines;
  double rotation_speed = CV_PI / 60.0; // Rotate one full circle in 120 frames 
  cv::Scalar line_color(255, 255, 255); // White lines
  int line_thickness = 1;

  for (int i = 0; i < num_lines; ++i) {
      double current_angle = (i * angle_step) + (frame_index * rotation_speed);
      cv::Point p1(center.x + static_cast<int>(line_length * std::cos(current_angle)),
                   center.y + static_cast<int>(line_length * std::sin(current_angle)));
      cv::Point p2(center.x - static_cast<int>(line_length * std::cos(current_angle)),
                   center.y - static_cast<int>(line_length * std::sin(current_angle)));
      cv::line(dummy, p1, p2, line_color, line_thickness, cv::LINE_AA);
  }
  // --- END SPINNING LINES ---


  // --- Dynamic Text Bouncing Logic (Drawn over everything) ---
  std::string bounce_text = "DVD Logo"; 
  int font = cv::FONT_HERSHEY_SIMPLEX;
  double scale = 1.0;
  int thickness = 2;
  cv::Scalar color(255, 255, 255); // White text
  int baseline = 0;
  cv::Size text_size = cv::getTextSize(bounce_text, font, scale, thickness, &baseline);

  // Define the boundaries for the bounce
  int margin = 5;
  //int minX = margin;
  int minY = margin;
  int maxX = width - text_size.width - margin;
  int maxY = height - text_size.height - margin;
  
  // Calculate Horizontal Bouncing Position (X) 
  int stepX = 5; 
  int bouncePeriodX = maxX / stepX;
  int fullCycleX = 2 * bouncePeriodX; 
  int currentStepX = frame_index % fullCycleX; 
  
  int offsetX = (currentStepX <= bouncePeriodX) 
                ? (currentStepX * stepX)                      
                : (maxX - (currentStepX - bouncePeriodX) * stepX); 

  // Calculate Vertical Bouncing Position (Y) 
  int bouncePeriodY = 80; 
  float sin_val = std::sin(frame_index * (CV_PI / bouncePeriodY));
  int midY = minY + (maxY - minY) / 2;
  int amplitudeY = (maxY - minY) / 2;
  int offsetY = midY + (int)(amplitudeY * sin_val);

  // Draw the Bouncing Text
  cv::putText(dummy, bounce_text, 
              cv::Point(offsetX, offsetY + text_size.height), 
              font, scale, color, thickness, cv::LINE_AA);

  // --- Add Frame Index for Debugging ---
  std::string index_text = "INDEX: " + std::to_string(frame_index);
  cv::putText(dummy, index_text, 
              cv::Point(10, 30), 
              cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 255, 255), 2, cv::LINE_AA); // White on black background
  
  cv::putText(dummy, "Frame Index: " + std::to_string(frame_index), 
              cv::Point(5, height - 5), 
              cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, cv::LINE_AA); // White on black background
  
  return dummy;
}

/**
 * @brief Generates a dummy color frame simulating a very low-light environment with a moving object.
 * * The background is set to a very dark gray/blue. A small, bright white circle moves 
 * horizontally across the frame, cycling back every 300 frames.
 * * @param width The desired width of the frame.
 * @param height The desired height of the frame.
 * @param frame_index A counter used to calculate the moving object's position.
 * @return cv::Mat The generated 3-channel BGR image.
 */
cv::Mat CAMERA::generateDummyLowLightFrame(int width, int height, int frame_index) 
{
  // 1. Create a 3-channel (BGR) Mat for the output frame
  cv::Mat frame(height, width, CV_8UC3);
  
  // 2. Set the background to a very dark color (simulating low light)
  // BGR values, e.g., (10, 10, 15) - a very dark, slightly bluish/purple hue
  // Max pixel value is 255. A value of 10 is very dim.
  //cv::Scalar low_light_color(10, 10, 15); 
  cv::Scalar low_light_color(0, 0, 0); 
  frame.setTo(low_light_color); 

  // 3. Define the moving object properties
  const int cycle_duration = 300; // Number of frames for the object to cross the screen
  const int object_radius = 25;
  const cv::Scalar object_color_1(1, 0, 0); 
  const cv::Scalar object_color_2(2, 0, 2); 
  const cv::Scalar object_color_3(0, 5, 0); 
  const cv::Scalar object_color_4(10, 10, 10); 
  const cv::Scalar object_color_bright(250, 250, 250); // Bright White (Visible!)
  
  // Calculate the object's horizontal position based on the frame index
  // The position cycles from 0 to width, normalized by the cycle duration
  int current_frame = frame_index % cycle_duration;
  
  // Linear interpolation for X position: (current_frame / cycle_duration) * width
  int center_x = static_cast<int>((static_cast<float>(current_frame) / cycle_duration) * width);
  
  // Y position (fixed in the center)
  int center_y = height / 2;
  
  // 4. Draw the moving object (a circle)
  cv::circle(
      frame,                          // Image to draw on
      cv::Point(center_x, center_y -200.0f),  // Center coordinates
      object_radius,                  // Radius
      object_color_1,                   // Color
      -1                              // Thickness: -1 means filled circle
  );
  cv::circle(
      frame,                          // Image to draw on
      cv::Point(center_x, center_y -100.0f),  // Center coordinates
      object_radius,                  // Radius
      object_color_2,                   // Color
      -1                              // Thickness: -1 means filled circle
  );
  cv::circle(
      frame,                          // Image to draw on
      cv::Point(center_x, center_y),  // Center coordinates
      object_radius,                  // Radius
      object_color_3,                   // Color
      -1                              // Thickness: -1 means filled circle
  );
  cv::circle(
      frame,                          // Image to draw on
      cv::Point(center_x, center_y +100.0f),  // Center coordinates
      object_radius,                  // Radius
      object_color_4,                   // Color
      -1                              // Thickness: -1 means filled circle
  );

  // Single bright point
  cv::circle(
      frame,                          // Image to draw on
      cv::Point(200.0f, 200.0f),      // Center coordinates
      5.0f,                           // Radius
      object_color_bright,            // Color
      -1                              // Thickness: -1 means filled circle
  );
  
  // 5. Add a small patch of slightly elevated light in the corner to test CLAHE locally
  cv::Rect bright_patch(width - 100, height - 100, 80, 80);
  cv::Mat patch = frame(bright_patch);
  // A slightly elevated dark gray color (40, 40, 40)
  patch.setTo(cv::Scalar(40, 40, 40)); 
  
  return frame;
}

/**
 * @brief Converts an OpenCV Mat object to an OpenGL texture ID.
 *
 * This function creates a new OpenGL texture or updates an existing one, 
 * uploading the pixel data from the OpenCV Mat object. It is designed to be 
 * robust against changes in the frame's resolution. The texture ID is passed 
 * and updated by reference.
 *
 * NOTE: This implementation assumes a 3-channel (color) image and uses 
 * glTexImage2D for both initial creation and updates to safely handle 
 * potential resolution changes.
 *
 * @param frame The OpenCV Mat object with image data (assumed to be BGR 8-bit).
 * @param textureID The existing OpenGL texture ID to update, or 0 to create a 
 * new one. Passed by reference (&), so it is modified directly.
 */
void CAMERA::matToTexture(const cv::Mat& frame, GLuint &textureID)
{
  // 1. Error Check
  if (frame.empty())
  {
    std::cerr << "Error: The OpenCV Mat is empty." << std::endl;
    return; // Return type is void now, so we just return.
  }

  // 2. Color Space Conversion (BGR to RGB)
  // OpenCV Mat is BGR by default; OpenGL texture format will be GL_RGB.
  cv::Mat rgbFrame;
  if (frame.channels() == 3) 
  {
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
  } 
  else if (frame.channels() == 1) 
  {
    // Handle grayscale: Convert to a 3-channel BGR for consistency, then RGB.
    cv::cvtColor(frame, rgbFrame, cv::COLOR_GRAY2RGB);
  } 
  else 
{
    std::cerr << "Error: Unsupported number of channels in Mat: " << frame.channels() << std::endl;
    return;
  }

  // 3. Texture Binding and Creation/Update Logic
  if (textureID == 0) 
  {
    // Generate a new texture ID and store it directly in the referenced variable
    glGenTextures(1, &textureID);
  } 
  
  // Bind the texture (necessary for all operations)
  // The bind operation uses the ID that was either passed in or just generated.
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Set parameters (these only need to be set once, but it's harmless to repeat)
  // We only set them if a valid ID was found/created.
  if (textureID != 0) 
  {
    // Set texture parameters for rendering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  // 4. Upload/Reallocate Pixel Data
  // We use glTexImage2D for reallocation, which is safer when resolution 
  // changes and avoids the memory issue associated with glTexSubImage2D 
  // if the size changes.
  
  // InternalFormat: GL_RGB (3 components)
  // Format: GL_RGB (3 components in data buffer)
  // Type: GL_UNSIGNED_BYTE (8-bit per channel)
  glTexImage2D(
      GL_TEXTURE_2D, 
      0,                 // Mipmap level
      GL_RGB,            // Internal format
      rgbFrame.cols,     // Width
      rgbFrame.rows,     // Height
      0,                 // Border (must be 0)
      GL_RGB,            // Format of the pixel data
      GL_UNSIGNED_BYTE,  // Data type of the pixel data
      rgbFrame.data      // Pointer to the image data
  );

  // 5. Unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);

  // No return statement needed, as the ID was updated via reference.
}

void CAMERA::init(stringstream &Print_Stream)
{
  // Initial Camera Setup
  // disable continuous autofocus
  // set_camera_control(PROPS.CTRL_FOCUS_AUTO, 0);
  // set manual focus value (depends on your camera’s supported range)
  // set_camera_control(PROPS.CTRL_FOCUS_ABSOLUTE, 0);

  // Verify settings

  Print_Stream << "Init not yet coded.";

  // When CAMERA_SETTING is matured (vectorized, tested) enable for common control set.
  /*
  Print_Stream << "               Auto Focus: " << get_camera_control_value(PROPS.CTRL_FOCUS_AUTO) << endl;
  Print_Stream << "          Abslolute Focus: " << get_camera_control_value(PROPS.CTRL_FOCUS_ABSOLUTE) << endl;
  Print_Stream << "            Exposure Auto: " << get_camera_control_value(PROPS.CTRL_EXPOSURE_AUTO) << endl;
  Print_Stream << "   Absolute Exposure Time: " << get_camera_control_value(PROPS.CTRL_EXPOSURE_TIME_ABSOLUTE) << endl;
  Print_Stream << "   Backlight Compensation: " << get_camera_control_value(PROPS.CTRL_BACKLIGHT_COMPENSATION) << endl;
  Print_Stream << "       White Balance Auto: " << get_camera_control_value(PROPS.CTRL_WHITE_BALANCE_AUTOMATIC) << endl;
  Print_Stream << "White Balance Temperature: " << get_camera_control_value(PROPS.CTRL_WHITE_BALANCE_TEMP) << endl;
  Print_Stream << "                     Gain: " << get_camera_control_value(PROPS.CTRL_GAIN) << endl;
  Print_Stream << "                     Gama: " << get_camera_control_value(PROPS.CTRL_GAMA) << endl;
  Print_Stream << "                      Hue: " << get_camera_control_value(PROPS.CTRL_HUE) << endl;
  Print_Stream << "                Sharpness: " << get_camera_control_value(PROPS.CTRL_SHARPNESS) << endl;
  */
}

void CAMERA::save_settings()
{
  JSON_INTERFACE settings;
    JSON_ENTRY camera_settings;
      JSON_ENTRY backup_camera_settings;
      JSON_ENTRY camera_control_settings;

  // Standard Settings
  camera_settings.create_label_value(quotify("CAMERA_DEVICE_NAME"), quotify(PROPS.CAMERA_DEVICE_NAME));
  camera_settings.create_label_value(quotify("WIDTH"), quotify(to_string(PROPS.WIDTH)));
  camera_settings.create_label_value(quotify("HEIGHT"), quotify(to_string(PROPS.HEIGHT)));
  camera_settings.create_label_value(quotify("COMPRESSION"), quotify(to_string(PROPS.COMPRESSION)));
  camera_settings.create_label_value(quotify("FULL_FRAME_STYLE"), quotify(to_string(PROPS.FULL_FRAME_STYLE)));
  camera_settings.create_label_value(quotify("POST_PROCESS_SCALE"), quotify(to_string(PROPS.POST_PROCESS_SCALE)));
  camera_settings.create_label_value(quotify("FLIP_HORIZONTAL"), quotify(to_string(PROPS.FLIP_HORIZONTAL)));
  camera_settings.create_label_value(quotify("FLIP_VERTICAL"), quotify(to_string(PROPS.FLIP_VERTICAL)));
  camera_settings.create_label_value(quotify("TEST"), quotify(to_string(PROPS.TEST)));
  camera_settings.create_label_value(quotify("TEST_IMAGE"), quotify(to_string(PROPS.TEST_IMAGE)));
  camera_settings.create_label_value(quotify("TEST_MULTI_FRAME"), quotify(to_string(PROPS.TEST_MULTI_FRAME)));

  camera_settings.create_label_value(quotify("ENH_LOW_LIGHT"), quotify(to_string(PROPS.ENH_LOW_LIGHT)));
  camera_settings.create_label_value(quotify("ENH_LOW_LIGHT_THRESHOLD"), quotify(to_string(PROPS.ENH_LOW_LIGHT_THRESHOLD)));
  camera_settings.create_label_value(quotify("ENH_LOW_LIGHT_GAMMA"), quotify(to_string(PROPS.ENH_LOW_LIGHT_GAMMA)));
  camera_settings.create_label_value(quotify("ENH_GLARE_MASK"), quotify(to_string(PROPS.ENH_GLARE_MASK)));
  camera_settings.create_label_value(quotify("ENH_COLOR"), quotify(to_string(PROPS.ENH_COLOR)));
  camera_settings.create_label_value(quotify("ENH_CANNY_MASK"), quotify(to_string(PROPS.ENH_CANNY_MASK)));
  camera_settings.create_label_value(quotify("ENH_FAKE_FRAMES"), quotify(to_string(PROPS.ENH_FAKE_FRAMES)));
  camera_settings.create_label_value(quotify("ENH_DOUBLE_MASK"), quotify(to_string(PROPS.ENH_DOUBLE_MASK)));

  // Backup Camera Settings
  backup_camera_settings.create_label_value(quotify("SHOW_PATH"), quotify(to_string(PROPS.SHOW_PATH)));
  backup_camera_settings.create_label_value(quotify("ANGLE_MULTIPLIER"), quotify(to_string(PROPS.ANGLE_MULTIPLIER)));

  backup_camera_settings.create_label_value(quotify("Y0"), quotify(to_string(PROPS.Y0)));
  backup_camera_settings.create_label_value(quotify("XL0"), quotify(to_string(PROPS.XL0)));
  backup_camera_settings.create_label_value(quotify("XR0"), quotify(to_string(PROPS.XR0)));
  backup_camera_settings.create_label_value(quotify("Y1"), quotify(to_string(PROPS.Y1)));
  backup_camera_settings.create_label_value(quotify("XL1"), quotify(to_string(PROPS.XL1)));
  backup_camera_settings.create_label_value(quotify("XR1"), quotify(to_string(PROPS.XR1)));
  backup_camera_settings.create_label_value(quotify("Y2"), quotify(to_string(PROPS.Y2)));
  backup_camera_settings.create_label_value(quotify("XL2"), quotify(to_string(PROPS.XL2)));
  backup_camera_settings.create_label_value(quotify("XR2"), quotify(to_string(PROPS.XR2)));
  backup_camera_settings.create_label_value(quotify("Y3"), quotify(to_string(PROPS.Y3)));
  backup_camera_settings.create_label_value(quotify("XL3"), quotify(to_string(PROPS.XL3)));
  backup_camera_settings.create_label_value(quotify("XR3"), quotify(to_string(PROPS.XR3)));

  camera_settings.put_json_in_set(quotify("backup camera settings"), backup_camera_settings);

  // Camera Control Settings
  for (size_t setting = 0; setting < SETTINGS.size(); setting++)
  {
    if (SETTINGS[setting].VAR_TYPE ==  1 || SETTINGS[setting].VAR_TYPE ==  2 || SETTINGS[setting].VAR_TYPE ==  3)
    {
      camera_control_settings.create_label_value(quotify(SETTINGS[setting].NAME), quotify(to_string(SETTINGS[setting].VALUE)));
    }
  }

  camera_settings.put_json_in_set(quotify("camera control settings"), camera_control_settings);

  // All Settings
  settings.ROOT.put_json_in_set(quotify("camera settings"), camera_settings);


  // Save to disk
  deque<std::string> camera_settings_json_deque;
  settings.json_print_build_to_string_deque(camera_settings_json_deque);

  threaded_deque_string_to_file(PROPS.CAMERA_SETTINGS_DIR + PROPS.CAMERA_NAME + "_camera_settings.json", camera_settings_json_deque);
}

void CAMERA::load_settings_json(vector<CAMERA_CONTROL_SETTING_LOADED> &Camera_Control)
{
  bool tmp_success = false;

  std::string camera_settings_json = file_to_string(PROPS.CAMERA_SETTINGS_DIR + PROPS.CAMERA_NAME + "_camera_settings.json", tmp_success);

  if (tmp_success == false)
  {
    save_settings();
  }
  else
  {
    // Load Settings
    JSON_INTERFACE settings;

    // parse file
    tmp_success = settings.load_json_from_string(camera_settings_json);

    if (tmp_success)
    {
      // root
      for(size_t root = 0; root < settings.ROOT.DATA.size(); root++)
      {
        if (settings.ROOT.DATA[root].label() == "camera settings")
        {
          STRING_STRING ss_camera_device_name;
          STRING_INT    si_width;
          STRING_INT    si_height;
          STRING_INT    si_compression;
          STRING_INT    si_full_frame_style;
          STRING_FLOAT  sf_post_process_scale;
          STRING_BOOL   sb_flip_horizontal;
          STRING_BOOL   sb_flip_vertical;
          STRING_BOOL   sb_test;
          STRING_BOOL   sb_test_image;
          STRING_BOOL   sb_test_multi_frame;

          STRING_BOOL   sb_enh_low_light;
          STRING_INT    si_enh_low_light_threshold;
          STRING_DOUBLE sd_enh_low_light_gamma;
          STRING_BOOL   sb_enh_grare_mask;
          STRING_BOOL   sb_enh_color;
          STRING_BOOL   sb_enh_canny_mask;
          STRING_BOOL   sb_enh_fake_frames;
          STRING_BOOL   sb_enh_double_mask;

          //root/camera settings
          for (size_t entry_list = 0;
                      entry_list < settings.ROOT.DATA[root].DATA.size(); entry_list++)
          {
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("CAMERA_DEVICE_NAME", ss_camera_device_name);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("WIDTH", si_width);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("HEIGHT", si_height);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("COMPRESSION", si_compression);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FULL_FRAME_STYLE", si_full_frame_style);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("POST_PROCESS_SCALE", sf_post_process_scale);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_HORIZONTAL", sb_flip_horizontal);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_VERTICAL", sb_flip_vertical);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST", sb_test);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST_IMAGE", sb_test_image);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST_MULTI_FRAME", sb_test_multi_frame);

            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_LOW_LIGHT", sb_enh_low_light);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_LOW_LIGHT_THRESHOLD", si_enh_low_light_threshold);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_LOW_LIGHT_GAMMA", sd_enh_low_light_gamma);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_GLARE_MASK", sb_enh_grare_mask);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_COLOR", sb_enh_color);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_CANNY_MASK", sb_enh_canny_mask);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_FAKE_FRAMES", sb_enh_fake_frames);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("ENH_DOUBLE_MASK", sb_enh_double_mask);

            // Backup Camera Settings
            if (settings.ROOT.DATA[root].DATA[entry_list].label() == "backup camera settings")
            {
              STRING_BOOL   sb_show_path;
              STRING_FLOAT  sb_angle_multiplier;

              STRING_FLOAT  sf_y0;
              STRING_FLOAT  sf_xl0;
              STRING_FLOAT  sf_rl0;

              STRING_FLOAT  sf_y1;
              STRING_FLOAT  sf_xl1;
              STRING_FLOAT  sf_rl1;

              STRING_FLOAT  sf_y2;
              STRING_FLOAT  sf_xl2;
              STRING_FLOAT  sf_rl2;

              STRING_FLOAT  sf_y3;
              STRING_FLOAT  sf_xl3;
              STRING_FLOAT  sf_rl3;

              //root/camera settings/backup camera settings
              for (size_t backup_cam_entry_list = 0;
              backup_cam_entry_list < settings.ROOT.DATA[root].DATA[entry_list].DATA.size(); backup_cam_entry_list++)
              {
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("SHOW_PATH", sb_show_path);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("ANGLE_MULTIPLIER", sb_angle_multiplier);

                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("Y0", sf_y0);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XL0", sf_xl0);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XR0", sf_rl0);

                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("Y1", sf_y1);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XL1", sf_xl1);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XR1", sf_rl1);

                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("Y2", sf_y2);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XL2", sf_xl2);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XR2", sf_rl2);

                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("Y3", sf_y3);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XL3", sf_xl3);
                settings.ROOT.DATA[root].DATA[entry_list].DATA[backup_cam_entry_list].get_if_is("XR3", sf_rl3);
              }

              if (sb_show_path.conversion_success())
              {
                PROPS.SHOW_PATH = sb_show_path.get_bool_value();
              }            
              if (sb_angle_multiplier.conversion_success())
              {
                PROPS.ANGLE_MULTIPLIER = sb_angle_multiplier.get_float_value();
              }

              if (sf_y0.conversion_success())
              {
                PROPS.Y0 = sf_y0.get_float_value();
              }
              if (sf_xl0.conversion_success())
              {
                PROPS.XL0 = sf_xl0.get_float_value();
              }
              if (sf_rl0.conversion_success())
              {
                PROPS.XR0 = sf_rl0.get_float_value();
              }

              if (sf_y1.conversion_success())
              {
                PROPS.Y1 = sf_y1.get_float_value();
              }
              if (sf_xl1.conversion_success())
              {
                PROPS.XL1 = sf_xl1.get_float_value();
              }
              if (sf_rl1.conversion_success())
              {
                PROPS.XR1 = sf_rl1.get_float_value();
              }

              if (sf_y2.conversion_success())
              {
                PROPS.Y2 = sf_y2.get_float_value();
              }
              if (sf_xl2.conversion_success())
              {
                PROPS.XL2 = sf_xl2.get_float_value();
              }
              if (sf_rl2.conversion_success())
              {
                PROPS.XR2 = sf_rl2.get_float_value();
              }

              if (sf_y3.conversion_success())
              {
                PROPS.Y3 = sf_y3.get_float_value();
              }
              if (sf_xl3.conversion_success())
              {
                PROPS.XL3 = sf_xl3.get_float_value();
              }
              if (sf_rl3.conversion_success())
              {
                PROPS.XR3 = sf_rl3.get_float_value();
              }
            }

            // Camera Control Settings
            if (settings.ROOT.DATA[root].DATA[entry_list].label() == "camera control settings")
            {
              //root/camera settings/camera control settings
              for (size_t setting = 0;
              setting < settings.ROOT.DATA[root].DATA[entry_list].DATA.size(); setting++)
              {
                STRING_INT tmp_value;
                CAMERA_CONTROL_SETTING_LOADED tmp_control_setting;
                
                tmp_control_setting.NAME = settings.ROOT.DATA[root].DATA[entry_list].DATA[setting].label();
                tmp_value.store(settings.ROOT.DATA[root].DATA[entry_list].DATA[setting].value());

                if (tmp_value.conversion_success())
                {
                  tmp_control_setting.VALUE = tmp_value.get_int_value();
                  Camera_Control.push_back(tmp_control_setting);
                }
              }
            }

            // ---
            if (ss_camera_device_name.conversion_success())
            {
              PROPS.CAMERA_DEVICE_NAME = ss_camera_device_name.get_str_value();
            }
            if (si_width.conversion_success())
            {
              PROPS.WIDTH = si_width.get_int_value();
            }            
            if (si_height.conversion_success())
            {
              PROPS.HEIGHT = si_height.get_int_value();
            }            
            if (si_compression.conversion_success())
            {
              PROPS.COMPRESSION = si_compression.get_int_value();
            }               
            if (si_full_frame_style.conversion_success())
            {
              PROPS.FULL_FRAME_STYLE = si_full_frame_style.get_int_value();
            }         
            if (sf_post_process_scale.conversion_success())
            {
              PROPS.POST_PROCESS_SCALE = sf_post_process_scale.get_float_value();
            }      
            if (sb_flip_horizontal.conversion_success())
            {
              PROPS.FLIP_HORIZONTAL = sb_flip_horizontal.get_bool_value();
            }            
            if (sb_flip_vertical.conversion_success())
            {
              PROPS.FLIP_VERTICAL = sb_flip_vertical.get_bool_value();
            }            
            if (sb_test.conversion_success())
            {
              PROPS.TEST = sb_test.get_bool_value();
            }            
            if (sb_test_image.conversion_success())
            {
              PROPS.TEST_IMAGE = sb_test_image.get_bool_value();
            }            
            if (sb_test_multi_frame.conversion_success())
            {
              PROPS.TEST_MULTI_FRAME = sb_test_multi_frame.get_bool_value();
            }

            // ---

            if (sb_enh_low_light.conversion_success())
            {
              PROPS.ENH_LOW_LIGHT = sb_enh_low_light.get_bool_value();
            }
            if (si_enh_low_light_threshold.conversion_success())
            {
              PROPS.ENH_LOW_LIGHT_THRESHOLD = si_enh_low_light_threshold.get_int_value();
            }
            if (sd_enh_low_light_gamma.conversion_success())
            {
              PROPS.ENH_LOW_LIGHT_GAMMA = sd_enh_low_light_gamma.get_double_value();
            }
            if (sb_enh_grare_mask.conversion_success())
            {
              PROPS.ENH_GLARE_MASK = sb_enh_grare_mask.get_bool_value();
            }            
            if (sb_enh_color.conversion_success())
            {
              PROPS.ENH_COLOR = sb_enh_color.get_bool_value();
            }            
            if (sb_enh_canny_mask.conversion_success())
            {
              PROPS.ENH_CANNY_MASK = sb_enh_canny_mask.get_bool_value();
            }            
            if (sb_enh_fake_frames.conversion_success())
            {
              PROPS.ENH_FAKE_FRAMES = sb_enh_fake_frames.get_bool_value();
            }            
            if (sb_enh_double_mask.conversion_success())
            {
              PROPS.ENH_DOUBLE_MASK = sb_enh_double_mask.get_bool_value();
            }

            // ---
            
          }
        }
      }
    }
  }
}

/*
// Be careful with this function. It is ran in its own thread.
void CAMERA::check_for_save_image_buffer_frame(cv::Mat &Frame)
{
  // If snapshot requested.
  if (SAVE_IMAGE_BUFFER_FRAME)
  {
    SAVE_IMAGE_BUFFER_FRAME = false;

    // Its possible the most buffer frame that was process was overwritten,
    //  so save the most recent buffer frame thats not locked out.

    if (LATEST_READY_FRAME == 0)
    {
      if (!Frame.empty())
      {
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_raw.jpg", Frame);
      }
    }
    else
    if (LATEST_READY_FRAME == 1)
    {
      if (!Frame.empty())
      {
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_raw.jpg", Frame);
      }
    }
  }
}
*/

/*
// Be careful with this function. It is ran in its own thread.
void CAMERA::check_for_save_image_buffer_processed(cv::Mat &Frame)
{
  // If snapshot requested.
  if (SAVE_IMAGE_PROCESSED_FRAME)
  {
    SAVE_IMAGE_PROCESSED_FRAME = false;

    if (!Frame.empty())
    {
      cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_prc.jpg", Frame);
    }
  }
}
*/

void CAMERA::run_preprocessing_inner(cv::Mat &Frame, cv::Mat &Frame_Output)
{
  if (!Frame.empty())
  {
    // 1. Initial Frame Preparation (Flip logic)
    if (PROPS.FLIP_HORIZONTAL && PROPS.FLIP_VERTICAL)
    {
      cv::flip(Frame, Frame_Output, -1);
    }
    else if (PROPS.FLIP_HORIZONTAL)
    {
      cv::flip(Frame, Frame_Output, 1);
    }
    else if (PROPS.FLIP_VERTICAL)
    {
      cv::flip(Frame, Frame_Output, 0);
    }
    else
    {
      Frame.copyTo(Frame_Output); 
    }
  } 
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::run_preprocessing_outer(FRAME_SUITE &Suite, unsigned long Frame_Time)
{
  // CV Code Here: Conditional and Controllable Image Enhancement Pipeline
  // The entire processing block is now conditional on the user controls.
  if (!Suite.PROCESSED_FRAME.empty())
  {
    /*
    // Always Apply Routines
    {
      // --- Stronger Denoising with Median Blur (5x5 kernel) ---
      int ksize = 5; 
      cv::medianBlur(PROCESSED_FRAME, PROCESSED_FRAME, ksize);

      // Sharpen
      cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
          0, -1, 0,
          -1, 5, -1,
          0, -1, 0);
      cv::filter2D(PROCESSED_FRAME, PROCESSED_FRAME, -1, kernel);
    }
    */

    // First Level of enhancements and preprocessing
    // Generate Processing Frames for enhancements.
    {
      cv::cvtColor(Suite.PROCESSED_FRAME, Suite.PROCESSED_FRAME_GRAY, cv::COLOR_BGR2GRAY);

      // Manage Low Light Filter
      if (PROPS.ENH_LOW_LIGHT)
      {
        // Get Low Light Value
        LOW_LIGHT_VALUE = is_low_light(Suite.PROCESSED_FRAME_GRAY);

        // Check timer, if expired flip IS_LOW_LIGHT
        if (LOW_LIGHT_DEBOUNCE_TIMER_LL.is_ready(Frame_Time))
        {
          IS_LOW_LIGHT = !IS_LOW_LIGHT;

          if (LOW_LIGHT_VALUE < PROPS.ENH_LOW_LIGHT_THRESHOLD)
          {
            IS_LOW_LIGHT = true;
            LOW_LIGHT_DEBOUNCE_TIMER_LL.set(Frame_Time, 5000);
          }
          else
          {
            IS_LOW_LIGHT = false;
            LOW_LIGHT_DEBOUNCE_TIMER_LL.set(Frame_Time, 1000);
          }
        }
        else  // Timer not expired, reset timer if still applies.
        {
          if (IS_LOW_LIGHT) // Low Light On
          {
            if (LOW_LIGHT_VALUE < PROPS.ENH_LOW_LIGHT_THRESHOLD)
            {
              LOW_LIGHT_DEBOUNCE_TIMER_LL.set(Frame_Time, 5000);
            }
          }
          else              // Low Light Off
          {
            if (LOW_LIGHT_VALUE >= PROPS.ENH_LOW_LIGHT_THRESHOLD)
            {
              LOW_LIGHT_DEBOUNCE_TIMER_LL.set(Frame_Time, 1000);
            }
          }
        }

        // Turn low light filter on or off
        if (IS_LOW_LIGHT)
        {
          low_light_filter(Suite.PROCESSED_FRAME);
        }
      }

      // Boost or reduce all color brightness
      if (PROPS.ENH_COLOR)
      {
        apply_min_max_contrast(Suite.PROCESSED_FRAME);
      }

      //cv::GaussianBlur(PROCESSED_FRAME_GRAY, PROCESSED_FRAME_GAUSSIAN, cv::Size(BLUR_KSIZE, BLUR_KSIZE), 0);
      //cv::Canny(PROCESSED_FRAME_GAUSSIAN, PROCESSED_FRAME_CANNY, CANNY_THRESH_LOW, CANNY_THRESH_HIGH, CANNY_APERTURE);

      cv::Canny(Suite.PROCESSED_FRAME_GRAY, Suite.PROCESSED_FRAME_CANNY, CANNY_THRESH_LOW, CANNY_THRESH_HIGH, CANNY_APERTURE);
    }
  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::apply_ehancements(FRAME_SUITE &Suite, FRAME_SUITE_EXTRA &Extra)
{
  if (!Suite.PROCESSED_FRAME.empty())
  {
    Extra.advance_latest();

    // Canny Mask
    if (PROPS.ENH_CANNY_MASK)
    {
      Extra.MASK_FRAME_CANNY[Extra.LATEST_POS] = canny_mask(Suite.PROCESSED_FRAME_CANNY);
    }

    /*
    // Overlay lines
    if (PROPS.ENH_OVERLAY_LINES)
    {
      MASK_FRAME_OVERLAY_LINES = overlay_lines(PROCESSED_FRAME_GRAY_DOWNSIZED);
    }
    */

    //---

    // Glare Mask
    if (PROPS.ENH_GLARE_MASK)
    {
      Extra.MASK_FRAME_GLARE[Extra.LATEST_POS] = extract_glare_area(Suite.PROCESSED_FRAME);
    }

    //---

    if (PROPS.ENH_HOUGH)
    {
      //detect_hough_circles(PROCESSED_FRAME, PROCESSED_FRAME_GAUSSIAN);
      detect_hough_circles(Suite.PROCESSED_FRAME, Suite.PROCESSED_FRAME_GRAY);
    }

    //---

    // Car Detection (Haar Cascade)
    if (PROPS.ENH_CAR_DETECTION && CAR_CASCADE_LOADED)
    {
      // Cascade classifiers are faster on smaller images and grayscale.
      cv::Mat gray_for_detection;
      cv::cvtColor(Suite.PROCESSED_FRAME, gray_for_detection, cv::COLOR_BGR2GRAY);
      
      // Reduce the size of the frame before detection to significantly boost speed.
      cv::Mat small_frame;
      float scale_factor = 0.5f; // Detect on half-size image (4x faster)
      cv::resize(gray_for_detection, small_frame, cv::Size(), scale_factor, scale_factor, cv::INTER_LINEAR);

      // Perform the detection
      std::vector<cv::Rect> cars;
      CAR_CASCADE.detectMultiScale(small_frame, cars, 
                                    1.1, // Scale factor
                                    5,   // Minimum neighbors
                                    0,   // Flags
                                    cv::Size(30, 30)); // Minimum object size (in scaled image)

      // Draw bounding boxes on the original size PROCESSED_FRAME
      for (const auto& rect : cars)
      {
        // Rescale the coordinates back to the original frame size
        cv::Rect original_rect(
            (int)(rect.x / scale_factor),
            (int)(rect.y / scale_factor),
            (int)(rect.width / scale_factor),
            (int)(rect.height / scale_factor)
        );
        
        // Draw a red rectangle (BGR: 0, 0, 255)
        cv::rectangle(Suite.PROCESSED_FRAME, original_rect, cv::Scalar(0, 0, 255), 2);
        cv::putText(Suite.PROCESSED_FRAME, "CAR", cv::Point(original_rect.x, original_rect.y - 10), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
      }
    }
  }
}

void CAMERA::apply_masks_to_processed_frame(cv::Mat &Frame, cv::Mat &Mask_0, cv::Mat &Mask_1, int Latest)
{
  if (PROPS.ENH_DOUBLE_MASK)
  {
    if (!Mask_0.empty())
    {
      resize_if_not_same_size(Frame, Mask_0);
      Frame.setTo(cv::Scalar(0, 0, 0), Mask_0);
    }
    if (!Mask_1.empty())
    {
      resize_if_not_same_size(Frame, Mask_1);
      Frame.setTo(cv::Scalar(0, 0, 0), Mask_1);
    }
  }
  else
  {
    if (Latest == 0)
    {
      if (!Mask_0.empty())
      {
        resize_if_not_same_size(Frame, Mask_0);
        Frame.setTo(cv::Scalar(0, 0, 0), Mask_0);
      }
    }
    else
    {
      if (!Mask_1.empty())
      {
        resize_if_not_same_size(Frame, Mask_1);
        Frame.setTo(cv::Scalar(0, 0, 0), Mask_1);
      }
    }
  }
}

// Function to combine two isolated, processed 3-channel image sections (Reconstruction).
// Frame is assumed to contain one component (e.g., non-glare part) and is patched
// with the component specified by 'Latest' (e.g., the glare part).
void CAMERA::apply_image_masks_to_processed_frame(cv::Mat &Frame, cv::Mat &Mask_0, cv::Mat &Mask_1, int Latest)
{
  if (Latest == 0)
  {
    if (!Mask_0.empty())
    {
      resize_if_not_same_size(Frame, Mask_0);
      
      // 1. Convert the 3-channel Mask_0 (colored patch on black background) 
      //    into a 1-channel non-black mask (CV_8UC1).
      cv::Mat mask_1ch;
      cv::cvtColor(Mask_0, mask_1ch, cv::COLOR_BGR2GRAY);
      cv::threshold(mask_1ch, mask_1ch, 1, 255, cv::THRESH_BINARY);

      // REPLACEMENT LOGIC: Use cv::copyTo.
      // copyTo(src, mask, dst) copies data from src (Mask_0) to dst (Frame)
      // ONLY where mask_1ch is non-zero (i.e., the colored area of the patch).
      // This cleanly replaces the area in Frame with the content of Mask_0.
      Mask_0.copyTo(Frame, mask_1ch);
    }
  }
  else // Latest == 1
  {
    if (!Mask_1.empty())
    {
      resize_if_not_same_size(Frame, Mask_1);
      
      // 1. Convert the 3-channel Mask_1 (colored patch on black background) 
      //    into a 1-channel non-black mask (CV_8UC1).
      cv::Mat mask_1ch;
      cv::cvtColor(Mask_1, mask_1ch, cv::COLOR_BGR2GRAY);
      cv::threshold(mask_1ch, mask_1ch, 1, 255, cv::THRESH_BINARY);

      // REPLACEMENT LOGIC: Use cv::copyTo.
      // copyTo(src, mask, dst) copies data from src (Mask_1) to dst (Frame)
      // ONLY where mask_1ch is non-zero (i.e., the colored area of the patch).
      Mask_1.copyTo(Frame, mask_1ch);
    }
  }
}

void CAMERA::apply_all_masks(FRAME_SUITE &Suite, FRAME_SUITE_EXTRA &Extra)
{
  if (!Suite.PROCESSED_FRAME.empty())
  {
    if (PROPS.ENH_CANNY_MASK)
    {
      apply_masks_to_processed_frame(Suite.PROCESSED_FRAME, Extra.MASK_FRAME_CANNY[0], Extra.MASK_FRAME_CANNY[1], Extra.LATEST_POS);
    }

    if (PROPS.ENH_GLARE_MASK)
    {
      //apply_masks_to_processed_frame(Suite.PROCESSED_FRAME, Extra.MASK_FRAME_GLARE[0], Extra.MASK_FRAME_GLARE[1], Extra.LATEST_POS);
      apply_image_masks_to_processed_frame(Suite.PROCESSED_FRAME, Extra.MASK_FRAME_GLARE[0], Extra.MASK_FRAME_GLARE[1], Extra.LATEST_POS);
    }
  }
}

void CAMERA::close_camera()
{
  std::stringstream print_stream;
  save_settings();

  // 1. Check if the camera capture object is open and release it
  if (CAMERA_CAPTURE.isOpened())
  {
    CAMERA_CAPTURE.release();

    PROPS.WIDTH = RESTART_WIDTH;
    PROPS.HEIGHT = RESTART_HEIGHT;
    PROPS.COMPRESSION = RESTART_COMPRESSION;

    print_stream << "Camera capture released.";
  }
  else
  {
    print_stream << "Camera close call when not open.";
  }

  PRINTW_QUEUE.push_back(print_stream.str());
}

void CAMERA::open_camera()
{
  std::stringstream print_stream;

  // Set Restart Variables
  RESTART_WIDTH = PROPS.WIDTH;
  RESTART_HEIGHT = PROPS.HEIGHT;
  RESTART_COMPRESSION = PROPS.COMPRESSION;

  LATEST_FRAME_READY = -1;
  BUFFER_FRAME_HANDOFF_POSITION  = -1;
  VIEWING_FRAME_POS = -1;

  if (CAMERA_CAPTURE.isOpened())
  {
    print_stream << "Camera already open. Needs to be closed first." << endl;
  }
  else
  {
    release_all_frames();

    // Load Cascades
    // --- Object Detection Setup ---
    // NOTE: In a real environment, you must have the XML file available 
    // (e.g., 'haarcascade_car.xml' or 'haarcascade_fullbody.xml')
    std::string cascade_path = "/home/delmane/rasfled/test/cars.xml"; // Mock path
    if (!CAR_CASCADE.load(cascade_path)) 
    {
      std::cerr << "WARNING: Could not load car cascade classifier from " << cascade_path << std::endl;
      std::cerr << "Car detection will be disabled." << std::endl;
      CAR_CASCADE_LOADED = false;
    } 
    else 
    {
      CAR_CASCADE_LOADED = true;
      //std::cout << "Car cascade loaded successfully." << std::endl;
    }

    // Start the camera 
    if (PROPS.TEST == false)
    {
      CAMERA_CAPTURE.open(PROPS.CAMERA_DEVICE_NAME, cv::CAP_V4L2);
      CAM_AVAILABLE = CAMERA_CAPTURE.isOpened();

      if (CAMERA_CAPTURE.isOpened()) 
      {
        // Set Compression
        if (PROPS.COMPRESSION == 1)
        {
          CAMERA_CAPTURE.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
          print_stream << " > Requesting Codec to MJPG" << std::endl;
        }
        else if (PROPS.COMPRESSION == 2)
        {
          // Some drivers use AVC1 for H.264
          CAMERA_CAPTURE.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('A', 'V', 'C', '1'));
          print_stream << " > Requesting Codec to AVC1 (H.264 alternative)." << std::endl;
        }
        else
        {
          CAMERA_CAPTURE.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', '2'));
          print_stream << " > Requesting Codec to YUY2." << std::endl;
        }

        // Set Resolution
        CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_WIDTH, (double)PROPS.WIDTH);
        CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_HEIGHT, (double)PROPS.HEIGHT);

        // VERIFICATION
        print_stream << "Camera Results:" << std::endl;

        double w = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_WIDTH);
        double h = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_HEIGHT);

        print_stream << "  > Resolution: " << w << "x" << h << " (Requested: " << PROPS.WIDTH << "x" << PROPS.HEIGHT << ")" << std::endl;

        PROPS.WIDTH = (int)w;
        PROPS.HEIGHT = (int)h;

        // Manual Decode stream method
        double fourcc = CAMERA_CAPTURE.get(cv::CAP_PROP_FOURCC);
        {
          uint32_t f = (uint32_t)fourcc;
          char codec[] = 
          {
            (char)(f & 0xFF),
            (char)((f >> 8) & 0xFF),
            (char)((f >> 16) & 0xFF),
            (char)((f >> 24) & 0xFF),
            0
          };
          print_stream << "  > Codec Str:  " << codec << std::endl;  
        }

        // Initialize camera via backend.  First set normal operation mode, then gather all properties.
        init(print_stream);
        
        CAM_AVAILABLE = true;

      }
      else
      {
        print_stream << "Could not open camera. Please check your camera connection." << std::endl;
        CAM_AVAILABLE = false;
        ENABLED = false;
      }
    }
    else // TEST CAMERA = true
    {
      // If camera not found DUMMY images to FRAME_DUMMY and FRAME_DUMMY2
      // If multi frame test, gen image to FRAME_DUMMY2.
      if (PROPS.TEST_IMAGE)
      {
        FRAME_DUMMY = cv::imread(PROPS.CAMERA_TEST_FILE_NAME, cv::IMREAD_COLOR); // Load test image as color
        if (!FRAME_DUMMY.empty())
        {
          if (PROPS.TEST_MULTI_FRAME)
          {
            FRAME_DUMMY2 = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
            print_stream << "Test camera loaded with test image for frame 1 and generated for frame 2." << std::endl;
            CAM_AVAILABLE = true;
          }
          else
          {
            print_stream << "Test camera loaded with test image." << std::endl;
            FRAME_DUMMY.copyTo(FRAME_DUMMY2);
            CAM_AVAILABLE = true;
          }
        }
        else
        {
          print_stream << "Test failed to load test image." << std::endl;
          CAM_AVAILABLE = false;
        }
      }
      else
      {
        FRAME_DUMMY = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
        FRAME_DUMMY2 = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
        print_stream << "Test camera loaded with generated frames." << std::endl;
        CAM_AVAILABLE = true;
      }
    }
  }

  PRINTW_QUEUE.push_back(print_stream.str());
}

// Be careful with this function. It is ran in its own thread.
  // The THREAD_CAMERA should manage itself, similare to the main.cpp main loop.
  // BUFFER_FRAME_HANDOFF_READY will signal to enhancement proccessing when 
  //  a new frame is ready, then continue the loop.
  // BEING_PROCESSED_FRAME is checked to ensure the new frame isnt placed in a 
  //  frame that is being processed.
  // Considering multiple new frames can be captured durng the processing phrase, 
  //  LATEST_READY_FRAME should report the newest frame.
  // FRAME_BUFFER_0, FRAME_BUFFER_1, BUFFER_FRAME_HANDOFF_READY, LATEST_READY_FRAME
  //  and BEING_PROCESSED_FRAME should be MUTEXed (mutex) to prevent data races, 
  //  but as is, the variables are playing nicely in their respective threads. 
void CAMERA::update_frame_thread()
{
  THREAD_CAMERA_ACTIVE = true;
  PRINTW_QUEUE.push_back("Starting Camera Read Thread");

  FLED_TIME thread_time;        // Thread gets its own Time 
  TIMED_IS_READY  forced_frame_limit;
  MEASURE_TIME_START_END time_se_frame_retrieval;
  MEASURE_TIME_START_END time_se_camera_fps;

  // --- CONFIGURATION FOR ROBUSTNESS ---
  const int max_consecutive_errors = 50; // Allow 50 bad frames/loops before giving up
  int consecutive_connection_error_count = 0;
  int consecutive_frame_error_count = 0;
  bool error = false;
  int new_frame_pos = -1;
  int current_buffer_frame_pos = -1;
  int frame_to_buffer = -1;

  // Create Process Time
  thread_time.create();

  load();

  open_camera();

  CAMERA_READ_THREAD_STOP = false;
  while (CAMERA_READ_THREAD_STOP == false)
  {
    //  Get current time.  This will be our timeframe to work in.
    thread_time.setframetime();

    // Enforce hard frame limit time.
    if (forced_frame_limit.is_ready(thread_time.current_frame_time()))
    {
      // Set wait times
      if (CAMERA_BEING_VIEWED)
      {
        forced_frame_limit.set(thread_time.current_frame_time(), 1); 
      }
      else
      {
        forced_frame_limit.set(thread_time.current_frame_time(), 100); 
      }

      // Grab start to loop times to compute fps.
      time_se_camera_fps.end_clock();
      TIME_CAMERA_FPS = time_se_camera_fps.duration_fps();
      TIME_CAMERA_FRAME_TIME = time_se_camera_fps.duration_ms();
      time_se_camera_fps.start_clock();

      // Check for restart
      if (APPLY_RESTART)
      {
        PRINTW_QUEUE.push_back("Camera restarting ...");
        APPLY_RESTART = false;

        // stop processing thread
        int retry_counter = 0;
        while (THREAD_PROCESSING_ACTIVE && retry_counter <= 10)
        {
          ENABLED = false;
          retry_counter++;
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Full Close
        close_camera();

        // allow start processing thread 
        ENABLED = true;

        // Open again
        open_camera();
      }

      if (CAMERA_CAPTURE.isOpened() || PROPS.TEST == true)
      {
        if (CAMERA_BEING_VIEWED)
        {
          // Inline error check. pos select, reset ftb.
          error = false;
          new_frame_pos = -1;
          current_buffer_frame_pos = BUFFER_FRAME_HANDOFF_POSITION % 3;

          // Determine next new frame position
          for (int pos = 0; pos < 3 && new_frame_pos < 0; pos++)
          {
            if (pos != current_buffer_frame_pos && pos != frame_to_buffer)
            {
              new_frame_pos = pos;
            }
          }
          frame_to_buffer = new_frame_pos;

          // Set Post Processing Size
          if (is_within(PROPS.POST_PROCESS_SCALE, 0.0f, 1.0f))
          {
            POST_PROCESS_SIZE = cv::Size(
                          (int)(PROPS.POST_PROCESS_SCALE * (float)PROPS.WIDTH),
                          (int)(PROPS.POST_PROCESS_SCALE * (float)PROPS.HEIGHT)
                                        );
          }

          // Capture the camera frame.
          if (PROPS.TEST)
          {
            if (PROPS.TEST_IMAGE)
            {
              FRAME_DUMMY.copyTo(PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER);
            }
            else
            {
              PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER = generateDummyLowLightFrame(POST_PROCESS_SIZE.width, POST_PROCESS_SIZE.height, (int)thread_time.current_frame_time() / 100);
            }
          }
          else
          {
            CAMERA_CAPTURE >> PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER;
            if (PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER.empty())
            {
              error = true;
            }
          }

          // Start Processing
          if (error)
          {
            consecutive_frame_error_count++;
          }
          else
          {
            // Save image to disk and get captured frame from camera.
            //check_for_save_image_buffer_frame(PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER);

            // Measure the time to run the routine.
            time_se_frame_retrieval.start_clock();

            consecutive_frame_error_count = 0;

            // Start generating PROCESSED_FRAME
            if (PROPS.POST_PROCESS_SCALE == 1.0f)
            {
              run_preprocessing_inner(PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER, PROCESS_FRAMES_ARRAY[frame_to_buffer].PROCESSED_FRAME);
            }
            else
            {
              cv::resize(PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER, PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER_RESIZE, POST_PROCESS_SIZE);
              run_preprocessing_inner(PROCESS_FRAMES_ARRAY[frame_to_buffer].FRAME_BUFFER_RESIZE, PROCESS_FRAMES_ARRAY[frame_to_buffer].PROCESSED_FRAME);
            }

            // Signal the enhancement processor that a new frame is available
            LATEST_FRAME_READY = frame_to_buffer;

            // Save the amout of time it took to read the frame.
            time_se_frame_retrieval.end_clock();
            TIME_FRAME_RETRIEVAL = time_se_frame_retrieval.duration_ms();
          }
        }
      }
      else
      {
        consecutive_connection_error_count++;
      }

      // Stop Thread if too many errors
      if (consecutive_connection_error_count + consecutive_frame_error_count > max_consecutive_errors)
      {
        ENABLED = false;
        PRINTW_QUEUE.push_back("Camera Error - Connection Glitch");
        PRINTW_QUEUE.push_back("  Connection Error Count: " + to_string(consecutive_connection_error_count));
        PRINTW_QUEUE.push_back("  Frame Error Count: = " + to_string(consecutive_frame_error_count));
      }

    } //  Enforce hard frame limit time.

    // Thread will need to sleep, governed by the FORCED_FRAME_LIMIT.
    thread_time.request_ready_time(forced_frame_limit.get_ready_time());
    thread_time.sleep_till_next_frame();

    // Only continue running if enabled
    CAMERA_READ_THREAD_STOP = !ENABLED;
    
  } // Main While Loop

  LATEST_FRAME_READY = -1;

  // Close Camera if thread stops.
  close_camera();
  CAM_AVAILABLE = false;

  PRINTW_QUEUE.push_back("Ending Camera Read Thread");
  THREAD_CAMERA_ACTIVE = false;
}

void CAMERA::process_enhancements_thread()
{
  THREAD_PROCESSING_ACTIVE = true;
  PRINTW_QUEUE.push_back("Starting Image Process Thread");

  FLED_TIME thread_time;        // Thread gets its own Time 
  TIMED_IS_READY  forced_frame_limit;
  MEASURE_TIME_START_END time_se_frame_processing;

  // --- CONFIGURATION FOR ROBUSTNESS --- 
  int frame_number = -1;

  // Create Process Time
  thread_time.create();

  CAMERA_PROCESSING_THREAD_STOP = false;
  while (CAMERA_PROCESSING_THREAD_STOP == false)
  {
    //  Get current time.  This will be our timeframe to work in.
    thread_time.setframetime();

    // Enforce hard frame limit time.
    if (forced_frame_limit.is_ready(thread_time.current_frame_time()))
    {
      // Set wait times
      if (CAMERA_BEING_VIEWED)
      {
        forced_frame_limit.set(thread_time.current_frame_time(), 1); 
      }
      else
      {
        forced_frame_limit.set(thread_time.current_frame_time(), 100); 
      }

      // Process enhancements if frame ready
      if (LATEST_FRAME_READY != -1 &&  BUFFER_FRAME_HANDOFF_POSITION == -1)
      {
        // Establish latest frame and lock
        BUFFER_FRAME_HANDOFF_POSITION = LATEST_FRAME_READY;
        frame_number = LATEST_FRAME_READY;
        LATEST_FRAME_READY = -1;

        // Start measuring how long it takes to procecc the enhancements
        time_se_frame_processing.start_clock();

        // Run enancements processing.  Store data back in itself.
        run_preprocessing_outer(PROCESS_FRAMES_ARRAY[frame_number], thread_time.current_frame_time());
        
        apply_ehancements(PROCESS_FRAMES_ARRAY[frame_number], PROCESS_FRAMES_EXTRA);
        apply_all_masks(PROCESS_FRAMES_ARRAY[frame_number], PROCESS_FRAMES_EXTRA);

        if (PROPS.ENH_FAKE_FRAMES)
        {
          if (TIME_FRAME_PROCESSING < TIME_CAMERA_FRAME_TIME)
          {
            INTERPOLATION_DISPLAY = true;
          }
          else
          {
            INTERPOLATION_DISPLAY = false;
          }

          PROCESS_FRAMES_ARRAY[frame_number].FRAME_BUFFER_FAKE = FAKE_FRAME_GENERATOR.interpolateFrame(PROCESS_FRAMES_ARRAY[frame_number].PROCESSED_FRAME);
        }

        // Store times
        time_se_frame_processing.end_clock();
        TIME_FRAME_PROCESSING = time_se_frame_processing.duration_ms();

        // Advance Buffer
        BUFFER_FRAME_HANDOFF_POSITION = frame_number + 3;
      }
      else if (BUFFER_FRAME_HANDOFF_POSITION >= 0 && BUFFER_FRAME_HANDOFF_POSITION <= 2)
      {
        // Something went wrong. Avoid lockout.
        BUFFER_FRAME_HANDOFF_POSITION = -1;
      }
    } // Frame Limit Cap

    // Thread will need to sleep, governed by the FORCED_FRAME_LIMIT.
    thread_time.request_ready_time(forced_frame_limit.get_ready_time());
    thread_time.sleep_till_next_frame();

    // Only continue running if enabled
    CAMERA_PROCESSING_THREAD_STOP = !ENABLED;
  }

  PRINTW_QUEUE.push_back("Ending Image Process Thread");
  THREAD_PROCESSING_ACTIVE = false;
}

void CAMERA::generate_imgui_texture_frame(cv::Mat& Frame)
{
  // Get Acual Frame Rate
  TIME_SE_DISPLAYED_FRAME_RATE.end_clock();
  TIME_ACTUAL_FPS         = TIME_SE_DISPLAYED_FRAME_RATE.duration_fps();
  TIME_ACTUAL_FRAME_TIME  = TIME_SE_DISPLAYED_FRAME_RATE.duration_ms();
  TIME_SE_DISPLAYED_FRAME_RATE.start_clock();

  if (!Frame.empty())
  {
    // Convert the frame to an OpenGL texture.
    // We pass the member variable to reuse the same texture.
    matToTexture(Frame, TEXTURE_ID);
  }
}

bool CAMERA::set_camera_control(CAMERA_SETTING &Setting)
{
  bool ret_success = false;

  if (CAM_AVAILABLE && Setting.SET_VALUE != Setting.VALUE)
  {
    if (Setting.SET_VALUE < Setting.MINIMUM)
    {
      Setting.SET_VALUE = Setting.MINIMUM;
    }
    else if (Setting.SET_VALUE > Setting.MAXIMUM)
    {
      Setting.SET_VALUE = Setting.MAXIMUM;
    }

    ret_success = set_control((int32_t)Setting.ADDRESS, (int32_t)Setting.SET_VALUE);
    Setting.VALUE = Setting.SET_VALUE;
  }
  else
  {
    ret_success = false;
  }

  return ret_success;
}

int CAMERA::get_camera_control_value(CAMERA_SETTING &Setting)
{
  int ret_value = -1;

  if (CAM_AVAILABLE && Setting.ENABLED)
  {
    ret_value = get_control(Setting.ADDRESS);
    if (ret_value != -1)
    {
      Setting.SET_VALUE = ret_value;
    }
  }

  return ret_value;
}

void CAMERA::apply_camera_control_changes()
{
  for (size_t pos = 0; pos < SETTINGS.size(); pos++)
  {
    if (SETTINGS[pos].SET_VALUE != SETTINGS[pos].VALUE)
    {
      set_camera_control(SETTINGS[pos]);
    }
  }
}

void CAMERA::apply_camera_control_defaults()
{
  for (size_t pos = 0; pos < SETTINGS.size(); pos++)
  {
    SETTINGS[pos].SET_VALUE = SETTINGS[pos].DEFAULT;
    set_camera_control(SETTINGS[pos]);
  }
}

// Assuming CAMERA is a class and PROPS/INFORMATION_COMMAND_LIST are members
void CAMERA::list_controls(vector<std::string> &String_Vector)
{
  // 1. Clear the output buffer
  INFORMATION_COMMAND_LIST = "";

  // 2. Open the device and use RAII for safe closing
  int device_fd = open(PROPS.CAMERA_DEVICE_NAME.c_str(), O_RDWR);
  if (device_fd == -1) 
  {
    // Use strerror(errno) for detailed error information
    INFORMATION_COMMAND_LIST = "Error: Opening video device failed. Reason: " + 
                                std::string(strerror(errno)) + "\n";
    return;
  }
  
  // The file descriptor will be closed when 'fd_closer' goes out of scope.
  FdCloser fd_closer(device_fd); 

  // Define the table header with aligned columns
  INFORMATION_COMMAND_LIST = "Command List:\n\n"
                              "| ID         | Control Name                   |Type  |Min  |Max  |Dflt |Step |\n"
                              "|------------|--------------------------------|------|-----|-----|-----|-----|\n";

  struct v4l2_queryctrl queryctrl;
  std::memset(&queryctrl, 0, sizeof(queryctrl));

  // Start with the first control ID
  queryctrl.id = V4L2_CID_BASE;

  // Loop through controls until ioctl fails (i.e., returns -1)
  while (ioctl(device_fd, VIDIOC_QUERYCTRL, &queryctrl) == 0) 
  {
    // 3. Skip disabled controls
    if (!(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)) 
    {
      CAMERA_SETTING tmp_camera_setting;
      std::stringstream print_stream;
      
      // Start row and set alignment to left
      print_stream << std::left << "| ";

      // ID (Hex) - 8 chars for hex, plus "0x" prefix
      print_stream << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(' ') << queryctrl.id << std::dec << " | ";
      tmp_camera_setting.ADDRESS = queryctrl.id;

      // Reset fill/case and use appropriate widths for text/decimal
      print_stream << std::setfill(' ') << std::left;

      // Control Name - max 32 chars
      print_stream << std::setw(30) << queryctrl.name << " |";
      tmp_camera_setting.NAME = reinterpret_cast<char*>(queryctrl.name);
      
      // Type
      tmp_camera_setting.VAR_TYPE = queryctrl.type;
      if (queryctrl.type == V4L2_CTRL_TYPE_INTEGER)
      {
        print_stream << std::setw(6) << "int" << "|";
      }
      else if (queryctrl.type == V4L2_CTRL_TYPE_BOOLEAN)
      {
        print_stream << std::setw(6) << "bool" << "|";
      }
      else if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
      {
        print_stream << std::setw(6) << "menu" << "|";
      }
      else if (queryctrl.type == V4L2_CTRL_TYPE_CTRL_CLASS)
      {
        print_stream << std::setw(6) << "clas" << "|";
      }
      else 
      {
        print_stream << std::setw(6) << queryctrl.type << "|";
      }

      // Min, Max, Default, Step - right aligned numbers
      print_stream << std::right;
      
      print_stream << std::setw(5) << queryctrl.minimum << "|";
      tmp_camera_setting.MINIMUM = queryctrl.minimum;

      print_stream << std::setw(5) << queryctrl.maximum << "|";
      tmp_camera_setting.MAXIMUM = queryctrl.maximum;

      print_stream << std::setw(5) << queryctrl.default_value << "|";
      tmp_camera_setting.DEFAULT = queryctrl.default_value;
      // for now, assume value and set value is default value
      tmp_camera_setting.VALUE = queryctrl.default_value;
      tmp_camera_setting.SET_VALUE = queryctrl.default_value;

      print_stream << std::setw(5) << queryctrl.step << "|\n"; // End of row
      tmp_camera_setting.STEP = queryctrl.step;

      INFORMATION_COMMAND_LIST += print_stream.str();

      // Extra Lines
      if (queryctrl.type == V4L2_CTRL_TYPE_MENU) 
      {
        struct v4l2_querymenu querymenu;
        std::memset(&querymenu, 0, sizeof(querymenu));
        
        querymenu.id = queryctrl.id;

        // Loop from minimum index to maximum index to query the label for each option
        for (querymenu.index = queryctrl.minimum; 
            querymenu.index <= (unsigned int)queryctrl.maximum; 
            querymenu.index++) 
        {
          // Call VIDIOC_QUERYMENU for each index
          if (ioctl(device_fd, VIDIOC_QUERYMENU, &querymenu) == 0) 
          {
            const char* menu_name = reinterpret_cast<char*>(querymenu.name);

            // Append the menu item to the command list, indented for clarity
            std::stringstream menu_stream;
            menu_stream << "|            |                                | " << querymenu.index
                        << ": " << std::left << std::setw(26) << menu_name << "|" << "\n";
            INFORMATION_COMMAND_LIST += menu_stream.str();

            // Store menu item
            CAMERA_SETTING_MENU_ITEM tmp_menu_item;
            tmp_menu_item.NAME = menu_name; // Safely assigned
            tmp_menu_item.ID = querymenu.index;
            tmp_camera_setting.MENU_LIST.push_back(tmp_menu_item);
          }
        }
      }

      SETTINGS.push_back(tmp_camera_setting);
    }

    // 4. Set the flag to query the *next* control. This is the V4L2 iteration pattern.
    queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
  }

  // 5. Check ioctl failure reason
  // The loop breaks when ioctl returns -1. We must check if it was the expected end condition (EINVAL).
  if (errno != EINVAL) 
  {
    // If the error is anything other than EINVAL, it's a real IO error during control enumeration.
    std::string error_msg = "\nError during control enumeration: " + 
                            std::string(strerror(errno)) + "\n";
    INFORMATION_COMMAND_LIST += error_msg;
  }

  String_Vector.push_back(INFORMATION_COMMAND_LIST);

  // The file descriptor is automatically closed by FdCloser destructor here.
}

void CAMERA::apply_loaded_camera_controls(vector<CAMERA_CONTROL_SETTING_LOADED> &Camera_Control, 
                                          deque<CAMERA_SETTING> &Settings)
{
  for (size_t cam_con = 0; cam_con < Camera_Control.size(); cam_con++)
  {
    for (size_t setting = 0; setting < Settings.size(); setting++)
    {
      if (Camera_Control[cam_con].NAME == Settings[setting].NAME)
      {
        Settings[setting].SET_VALUE = Camera_Control[cam_con].VALUE;
      }
    }
  }
}

void CAMERA::release_all_frames()
{
  for (int pos = 0; pos < 3; pos++)
  {
    PROCESS_FRAMES_ARRAY[pos].FRAME_BUFFER.release();
    PROCESS_FRAMES_ARRAY[pos].FRAME_BUFFER_RESIZE.release();
    PROCESS_FRAMES_ARRAY[pos].FRAME_BUFFER_FAKE.release();
    PROCESS_FRAMES_ARRAY[pos].PROCESSED_FRAME.release();
    PROCESS_FRAMES_ARRAY[pos].PROCESSED_FRAME_GRAY.release();
    PROCESS_FRAMES_ARRAY[pos].PROCESSED_FRAME_CANNY.release();
    PROCESS_FRAMES_ARRAY[pos].LIVE_FRAME_0.release();
    PROCESS_FRAMES_ARRAY[pos].LIVE_FRAME_1.release();
    FRAME_DUMMY.release();
    FRAME_DUMMY2.release();
  }
  for (int pos = 0; pos < 2; pos++)
  {    
    PROCESS_FRAMES_EXTRA.MASK_FRAME_GLARE[pos].release();
    PROCESS_FRAMES_EXTRA.MASK_FRAME_CANNY[pos].release();
  }
}

int CAMERA::post_process_height()
{
  return POST_PROCESS_SIZE.height;
}

int CAMERA::post_process_width()
{
  return POST_PROCESS_SIZE.width;
}

int CAMERA::bfhp()
{
  return BUFFER_FRAME_HANDOFF_POSITION;
}

void CAMERA::print_stream(CONSOLE_COMMUNICATION &cons)
{
  if (PRINTW_QUEUE.size() > 0)
  {
    std::string print_buffer = "";
    for (size_t pos = 0; pos < PRINTW_QUEUE.size(); pos++)
    {
      print_buffer += PRINTW_QUEUE[pos] + "\n";
    }
    PRINTW_QUEUE.clear();
    cons.printw(print_buffer);
  }
}

void CAMERA::process(CONSOLE_COMMUNICATION &cons, unsigned long Frame_Time, bool Camera_Being_Viewed)
{
  if (ENABLED)
  {
    if (APPLY_DEFAULTS)
    {
      apply_camera_control_defaults();
      APPLY_DEFAULTS = false;
      APPLY_CHANGES = false;
    }
    else if (APPLY_CHANGES)
    {
      apply_camera_control_changes();
      APPLY_DEFAULTS = false;
      APPLY_CHANGES = false;
    }

    // Check and set Camera Being Viewed
    if (Camera_Being_Viewed != CAMERA_BEING_VIEWED)
    {
      CAMERA_BEING_VIEWED = Camera_Being_Viewed;

      if (CAMERA_BEING_VIEWED == false)
      {
        release_all_frames();
      }
    }
    
    // ---------------------------------------------------------------------------------------
    // Check Thread Completions
    
    // Check to see if THREAD_CAMERA has stopped for any reason. 
    THREAD_CAMERA.check_for_completition();
    THREAD_PROCESSING.check_for_completition();

    // ---------------------------------------------------------------------------------------
    // Check Thread Completions

    // Calculate average frame time
    if (AVERAGE_FRAME_RATE_TIMER.is_ready(Frame_Time))
    {
      AVERAGE_FRAME_RATE_TIMER.set(Frame_Time, 1000);
      TIME_AVERAGE_FPS = AVERAGE_FRAME_RATE_COUNTER;
      AVERAGE_FRAME_RATE_COUNTER = 0;
    }

    // ---------------------------------------------------------------------------------------

    // Only attempt to start thread if camera is enabled
    if (THREAD_CAMERA.check_to_run_routine_on_thread(Frame_Time)) 
    {
      THREAD_CAMERA.create(5);
      // Start the camera update on a separate thread.
      // This call is non-blocking, so the main loop can continue immediately.
      THREAD_CAMERA.start_render_thread([&]() 
                {  update_frame_thread();  });
    }
    
    // ---------------------------------------------------------------------------------------

    // Only attempt to start thread if camera is enabled
    if (THREAD_PROCESSING.check_to_run_routine_on_thread(Frame_Time)) 
    {
      THREAD_PROCESSING.create(5);
      // Start the camera update on a separate thread.
      // This call is non-blocking, so the main loop can continue immediately.
      THREAD_PROCESSING.start_render_thread([&]() 
                {  process_enhancements_thread();  });
    }

    // ---------------------------------------------------------------------------------------

    // When the working frame has been fully process, render the 
    //  texture to be drawn in opengl.
    // Governed by the proccess call of the main program
    {
      if (BUFFER_FRAME_HANDOFF_POSITION >= 3 && 
          BUFFER_FRAME_HANDOFF_POSITION <= 5)
      {
        int current_buffer_frame_pos = BUFFER_FRAME_HANDOFF_POSITION % 3;

        // Generate Fake and Real
        if (PROPS.ENH_FAKE_FRAMES)
        {
          PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].FRAME_BUFFER_FAKE.copyTo(PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].LIVE_FRAME_0);
        }
        PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].PROCESSED_FRAME.copyTo(PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].LIVE_FRAME_1);

        // Place real or fake frame live.
        if (PROPS.ENH_FAKE_FRAMES == false)
        {
          FRAME_GEN = false;
          generate_imgui_texture_frame(PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].LIVE_FRAME_1);
          AVERAGE_FRAME_RATE_COUNTER++;
        }
        else
        {
          FRAME_GEN = false;
          FRAME_TO_TEXTURE_TIMER.set(Frame_Time, (int)TIME_CAMERA_FRAME_TIME / 2);
          generate_imgui_texture_frame(PROCESS_FRAMES_ARRAY[current_buffer_frame_pos].LIVE_FRAME_0);
          FRAME_TO_TEXTURE_TIMER_CHECK = true;
          AVERAGE_FRAME_RATE_COUNTER++;
        }

        VIEWING_FRAME_POS = current_buffer_frame_pos;
        BUFFER_FRAME_HANDOFF_POSITION = -1;
      }
      else if (BUFFER_FRAME_HANDOFF_POSITION > 5 || BUFFER_FRAME_HANDOFF_POSITION < -1)
      {
        // Something went wrong. Avoid lockout.
        BUFFER_FRAME_HANDOFF_POSITION = -1;
      }
    }

    if (FRAME_TO_TEXTURE_TIMER_CHECK)
    {
      if (FRAME_TO_TEXTURE_TIMER.is_ready(Frame_Time))
      {
        FRAME_TO_TEXTURE_TIMER_CHECK = false;
        if (camera_online() && CAMERA_BEING_VIEWED)
        {
          FRAME_GEN = true;
          generate_imgui_texture_frame(PROCESS_FRAMES_ARRAY[VIEWING_FRAME_POS].LIVE_FRAME_1);
          AVERAGE_FRAME_RATE_COUNTER++;
        }
      }
    }

    // ---------------------------------------------------------------------------------------

    // Print whatever is in the console print_wait queue.
    print_stream(cons);
  }
}

void CAMERA::take_snapshot()
{
  SAVE_IMAGE_BUFFER_FRAME = true;
  SAVE_IMAGE_PROCESSED_FRAME = true;
}

void CAMERA::load()
{
  vector<CAMERA_CONTROL_SETTING_LOADED> loaded_camera_control;
  load_settings_json(loaded_camera_control);
  list_controls(PRINTW_QUEUE);
  apply_loaded_camera_controls(loaded_camera_control, SETTINGS);
  APPLY_CHANGES = true;
}

void CAMERA::enable(bool Enable)
{
  ENABLED = Enable;
}

bool CAMERA::camera_online()
{
  return THREAD_PROCESSING_ACTIVE && THREAD_CAMERA_ACTIVE;
}

// ---------------------------------------------------------------------------------------

#endif
