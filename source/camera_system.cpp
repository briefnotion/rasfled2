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

// Helper function to create the mesh grid (x and y coordinates)
// This is called once when the first frame is processed.
void createMeshGrid(int rows, int cols, cv::Mat& x_coords, cv::Mat& y_coords) 
{
  x_coords.create(rows, cols, CV_32FC1);
  y_coords.create(rows, cols, CV_32FC1);

  for (int y = 0; y < rows; y++) 
  {
    float* p_x = x_coords.ptr<float>(y);
    float* p_y = y_coords.ptr<float>(y);
    for (int x = 0; x < cols; x++) 
    {
      p_x[x] = (float)x;
      p_y[x] = (float)y;
    }
  }
}

// --- Constructor: Simple initialization, no pre-allocation yet ---
// Dimensions and buffers are set when the first frame size is known.
FAKE_FRAME::FAKE_FRAME()
{
  // is_initialized_ is default-initialized to false
}

// --- Dynamic Initialization and Preprocessing ---
// This method now handles configuration (setting sizes) and pre-allocation, 
// ensuring it only runs once with the dimensions of the initial_frame.
void FAKE_FRAME::preprocess_initial_frame(const cv::Mat& initial_frame)
{
  // Check the initialization flag
  if (!is_initialized_)
  {
    // 1. Determine Dimensions and Constants based on the first frame's size
    PROCESS_WIDTH = initial_frame.cols;
    PROCESS_HEIGHT = initial_frame.rows;
    
    // Set flow size to 1/4th of process size (e.g., 640/4 = 160)
    FLOW_CALC_WIDTH = PROCESS_WIDTH / 4; 
    FLOW_CALC_HEIGHT = PROCESS_HEIGHT / 4; 
    
    cv::Size process_size(PROCESS_WIDTH, PROCESS_HEIGHT);
    cv::Size flow_calc_size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT);

    // 2. Pre-allocate all buffers
    current_gray_.create(process_size, CV_8UC1);
    prev_gray_.create(process_size, CV_8UC1);
    small_prev_gray_.create(flow_calc_size, CV_8UC1);
    small_current_gray_.create(flow_calc_size, CV_8UC1);
    
    flow_small_.create(flow_calc_size, CV_32FC2);
    flow_upsampled_.create(process_size, CV_32FC2);
    map_x_.create(process_size, CV_32FC1);
    map_y_.create(process_size, CV_32FC1);

    // 3. Pre-calculate the coordinate mesh grids
    createMeshGrid(PROCESS_HEIGHT, PROCESS_WIDTH, coords_x_, coords_y_);

    // Mark as initialized
    is_initialized_ = true;
  }
  
  // 4. Handle initial BGR frame state
  
  // The initial_frame size matches the newly set PROCESS_WIDTH/HEIGHT
  initial_frame.copyTo(prev_frame_);
  
  // Convert the initial frame to gray for the 'current' gray state
  cv::cvtColor(prev_frame_, current_gray_, cv::COLOR_BGR2GRAY);
  // The very first frame is the previous frame for the next calculation
  current_gray_.copyTo(prev_gray_);
}


cv::Mat FAKE_FRAME::interpolateFrame(const cv::Mat& current_frame) 
{
  // If prev_frame_ is empty, it means this is the very first call.
  // We run preprocess_initial_frame to initialize dimensions and buffers 
  // based on current_frame's size, and set up the initial state.
  if (prev_frame_.empty())
  {
    // INITIALIZATION STEP: Set dimensions and allocate buffers based on current_frame
    preprocess_initial_frame(current_frame);
    // Return the current frame, as there is no previous frame to interpolate from.
    return current_frame;
  }
  else
  {
    // 1. Update grayscale state: Current becomes previous
    current_gray_.copyTo(prev_gray_);
    
    // 2. Process the new current BGR frame
    cv::Mat processed_current_frame;
    // Note: Subsequent frames MUST match the size of the first frame (PROCESS_WIDTH/HEIGHT)
    if (current_frame.cols != PROCESS_WIDTH || current_frame.rows != PROCESS_HEIGHT) 
    {
      // Resize BGR frame if necessary
      cv::resize(current_frame, processed_current_frame, cv::Size(PROCESS_WIDTH, PROCESS_HEIGHT));
    } 
    else 
    {
      // Use a reference/shallow copy if sizes match to avoid BGR copy
      processed_current_frame = current_frame; 
    }

    // Convert the new frame to gray
    cv::cvtColor(processed_current_frame, current_gray_, cv::COLOR_BGR2GRAY);

    // 3. Downsample for FAST Optical Flow Calculation
    cv::resize(prev_gray_, small_prev_gray_, cv::Size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT));
    cv::resize(current_gray_, small_current_gray_, cv::Size(FLOW_CALC_WIDTH, FLOW_CALC_HEIGHT));

    // 4. Calculate Optical Flow 
    cv::calcOpticalFlowFarneback(small_prev_gray_, small_current_gray_, flow_small_, 0.5, 3, 10, 3, 5, 1.2, 0);
    
    // 5. Upsample the flow field
    cv::resize(flow_small_, flow_upsampled_, cv::Size(PROCESS_WIDTH, PROCESS_HEIGHT), 0, 0, cv::INTER_NEAREST);

    // 6. Apply Scaling to the flow vectors
    const float scale_factor = (float)PROCESS_WIDTH / FLOW_CALC_WIDTH; 
    cv::Mat flow_scaled = flow_upsampled_ * scale_factor;
    
    // --- OPTIMIZED REMAP MAP GENERATION (Vectorized) ---

    // 7. Split the 2-channel scaled flow map into X and Y components
    std::vector<cv::Mat> flow_channels;
    cv::split(flow_scaled, flow_channels); // flow_channels[0] = dx, flow_channels[1] = dy
    
    // 8. Calculate map_x and map_y using vectorized subtraction
    cv::Mat half_dx = flow_channels[0] * 0.5f;
    cv::Mat half_dy = flow_channels[1] * 0.5f;

    cv::subtract(coords_x_, half_dx, map_x_);
    cv::subtract(coords_y_, half_dy, map_y_);

    // 9. Perform the geometric transformation (Interpolation)
    cv::Mat interpolated_frame = cv::Mat::zeros(prev_frame_.size(), prev_frame_.type());
    cv::remap(prev_frame_, interpolated_frame, map_x_, map_y_, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

    // 10. Update class state for the next iteration
    processed_current_frame.copyTo(prev_frame_); 
    
    return interpolated_frame;
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
  // --- Glare Detection Parameter ---
  // Pixels where all B, G, and R channels are above this threshold will be masked.
  // 230 is a good starting point for bright white light (0-255 scale).
  const int GLARE_THRESHOLD = 230;

  cv::Mat glare_mask;
  
  // Use cv::inRange to find pixels where the B, G, and R values are all 
  // within the range [GLARE_THRESHOLD, 255].
  cv::inRange(processed_frame, 
              cv::Scalar(GLARE_THRESHOLD, GLARE_THRESHOLD, GLARE_THRESHOLD), 
              cv::Scalar(255, 255, 255), 
              glare_mask);

  // glare_mask is now a 1-channel mask where 255 indicates a glare pixel.
  return glare_mask;
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
  camera_settings.create_label_value(quotify("WIDTH"), quotify(to_string(PROPS.WIDTH)));
  camera_settings.create_label_value(quotify("HEIGHT"), quotify(to_string(PROPS.HEIGHT)));
  camera_settings.create_label_value(quotify("COMPRESSION"), quotify(to_string(PROPS.COMPRESSION)));
  camera_settings.create_label_value(quotify("FULL_FRAME_STYLE"), quotify(to_string(PROPS.FULL_FRAME_STYLE)));
  camera_settings.create_label_value(quotify("FLIP_HORIZONTAL"), quotify(to_string(PROPS.FLIP_HORIZONTAL)));
  camera_settings.create_label_value(quotify("FLIP_VERTICAL"), quotify(to_string(PROPS.FLIP_VERTICAL)));
  camera_settings.create_label_value(quotify("FORCED_FRAME_LIMIT_MS"), quotify(to_string(PROPS.FORCED_FRAME_LIMIT_MS)));
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
          STRING_INT    si_width;
          STRING_INT    si_height;
          STRING_INT    si_compression;
          STRING_INT    si_full_frame_style;
          STRING_FLOAT  sf_post_process_scale;
          STRING_BOOL   sb_flip_horizontal;
          STRING_BOOL   sb_flip_vertical;
          STRING_INT    si_forced_frame_limit;
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
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("WIDTH", si_width);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("HEIGHT", si_height);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("COMPRESSION", si_compression);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FULL_FRAME_STYLE", si_full_frame_style);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("POST_PROCESS_SCALE", sf_post_process_scale);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_HORIZONTAL", sb_flip_horizontal);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_VERTICAL", sb_flip_vertical);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FORCED_FRAME_LIMIT_MS", si_forced_frame_limit);
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
              PROPS.POST_PROCESS_SCALE = sf_post_process_scale.get_int_value();
            }      
            if (sb_flip_horizontal.conversion_success())
            {
              PROPS.FLIP_HORIZONTAL = sb_flip_horizontal.get_bool_value();
            }            
            if (sb_flip_vertical.conversion_success())
            {
              PROPS.FLIP_VERTICAL = sb_flip_vertical.get_bool_value();
            }            
            if (si_forced_frame_limit.conversion_success())
            {
              PROPS.FORCED_FRAME_LIMIT_MS = si_forced_frame_limit.get_int_value();
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

// Be careful with this function. It is ran in its own thread.
void CAMERA::check_for_save_image_buffer_frame()
{
  // If snapshot requested.
  if (SAVE_IMAGE_BUFFER_FRAME)
  {
    SAVE_IMAGE_BUFFER_FRAME = false;

    // Its possible the most buffer frame that was process was overwritten,
    //  so save the most recent buffer frame thats not locked out.

    if (LATEST_READY_FRAME == 0)
    {
      if (!FRAME_BUFFER_0.empty())
      {
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_raw.jpg", FRAME_BUFFER_0);
      }
    }
    else
    if (LATEST_READY_FRAME == 1)
    {
      if (!FRAME_BUFFER_1.empty())
      {
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_raw.jpg", FRAME_BUFFER_1);
      }
    }
  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::check_for_save_image_buffer_processed()
{
  // If snapshot requested.
  if (SAVE_IMAGE_PROCESSED_FRAME)
  {
    SAVE_IMAGE_PROCESSED_FRAME = false;

    if (!PROCESSED_FRAME.empty())
    {
      cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_" + PROPS.CAMERA_NAME + "_prc.jpg", PROCESSED_FRAME);
    }
  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::run_preprocessing(cv::Mat &Frame, unsigned long Frame_Time)
{
  if (!Frame.empty())
  {
    if (PROPS.POST_PROCESS_SCALE == 1.0f)
    {
      // 1. Initial Frame Preparation (Flip logic)
      if (PROPS.FLIP_HORIZONTAL && PROPS.FLIP_VERTICAL)
      {
        cv::flip(Frame, PROCESSED_FRAME, -1);
      }
      else if (PROPS.FLIP_HORIZONTAL)
      {
        cv::flip(Frame, PROCESSED_FRAME, 1);
      }
      else if (PROPS.FLIP_VERTICAL)
      {
        cv::flip(Frame, PROCESSED_FRAME, 0);
      }
      else
      {
        // Use clone to prevent FRAME from being modified by subsequent operations on PROCESSED_FRAME
        PROCESSED_FRAME = Frame.clone(); 
      }
    }
    else
    {
      


      // 1. Initial Frame Preparation (Flip logic)
      if (PROPS.FLIP_HORIZONTAL && PROPS.FLIP_VERTICAL)
      {
        cv::resize(Frame, FRAME_BUFFER_RESIZE, POST_PROCESS_SIZE);
        cv::flip(Frame, PROCESSED_FRAME, -1);
      }
      else if (PROPS.FLIP_HORIZONTAL)
      {
        cv::resize(Frame, FRAME_BUFFER_RESIZE, POST_PROCESS_SIZE);
        cv::flip(Frame, PROCESSED_FRAME, 1);
      }
      else if (PROPS.FLIP_VERTICAL)
      {
        cv::resize(Frame, FRAME_BUFFER_RESIZE, POST_PROCESS_SIZE);
        cv::flip(Frame, PROCESSED_FRAME, 0);
      }
      else
      {
        cv::resize(Frame, PROCESSED_FRAME, POST_PROCESS_SIZE);
      }

    }

  }

  // CV Code Here: Conditional and Controllable Image Enhancement Pipeline
  // The entire processing block is now conditional on the user controls.
  if (!PROCESSED_FRAME.empty())
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
      cv::cvtColor(PROCESSED_FRAME, PROCESSED_FRAME_GRAY, cv::COLOR_BGR2GRAY);

      // Manage Low Light Filter
      if (PROPS.ENH_LOW_LIGHT)
      {
        // Get Low Light Value
        LOW_LIGHT_VALUE = is_low_light(PROCESSED_FRAME_GRAY);

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
          low_light_filter(PROCESSED_FRAME);
        }
      }

      // Boost or reduce all color brightness
      if (PROPS.ENH_COLOR)
      {
        apply_min_max_contrast(PROCESSED_FRAME);
      }

      //cv::GaussianBlur(PROCESSED_FRAME_GRAY, PROCESSED_FRAME_GAUSSIAN, cv::Size(BLUR_KSIZE, BLUR_KSIZE), 0);
      //cv::Canny(PROCESSED_FRAME_GAUSSIAN, PROCESSED_FRAME_CANNY, CANNY_THRESH_LOW, CANNY_THRESH_HIGH, CANNY_APERTURE);

      cv::Canny(PROCESSED_FRAME_GRAY, PROCESSED_FRAME_CANNY, CANNY_THRESH_LOW, CANNY_THRESH_HIGH, CANNY_APERTURE);
    }
  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::apply_ehancements()
{
  if (!PROCESSED_FRAME.empty())
  {
    if (PROPS.ENH_DOUBLE_MASK)
    {
      if (DOUBLE_MASK_LATEST == 0)
      {
        DOUBLE_MASK_LATEST = 1;
      }
      else
      {
        DOUBLE_MASK_LATEST = 0;
      }
    }

    // Canny Mask
    if (PROPS.ENH_CANNY_MASK)
    {
      if (DOUBLE_MASK_LATEST == 0)
      {
        MASK_FRAME_CANNY_0 = canny_mask(PROCESSED_FRAME_CANNY);
      }
      else
      {
        MASK_FRAME_CANNY_1 = canny_mask(PROCESSED_FRAME_CANNY);
      }
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
      if (DOUBLE_MASK_LATEST == 0)
      {
        MASK_FRAME_GLARE_0 = suppress_glare_mask(PROCESSED_FRAME);
      }
      else
      {
        MASK_FRAME_GLARE_1 = suppress_glare_mask(PROCESSED_FRAME);
      }
    }

    //---

    if (PROPS.ENH_HOUGH)
    {
      //detect_hough_circles(PROCESSED_FRAME, PROCESSED_FRAME_GAUSSIAN);
      detect_hough_circles(PROCESSED_FRAME, PROCESSED_FRAME_GRAY);
    }

    //---

    // Car Detection (Haar Cascade)
    if (PROPS.ENH_CAR_DETECTION && CAR_CASCADE_LOADED)
    {
      // Cascade classifiers are faster on smaller images and grayscale.
      cv::Mat gray_for_detection;
      cv::cvtColor(PROCESSED_FRAME, gray_for_detection, cv::COLOR_BGR2GRAY);
      
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
        cv::rectangle(PROCESSED_FRAME, original_rect, cv::Scalar(0, 0, 255), 2);
        cv::putText(PROCESSED_FRAME, "CAR", cv::Point(original_rect.x, original_rect.y - 10), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
      }
    }
  }
}

void CAMERA::apply_masks_to_processed_frame(cv::Mat &Mask_0, cv::Mat &Mask_1)
{
  if (PROPS.ENH_DOUBLE_MASK)
  {
    if (!Mask_0.empty())
    {
      PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), Mask_0);
    }
    if (!Mask_1.empty())
    {
      PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), Mask_1);
    }
  }
  else
  {
    if (DOUBLE_MASK_LATEST == 0)
    {
      if (!Mask_0.empty())
      {
        PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), Mask_0);
      }
    }
    else
    {
      if (!Mask_1.empty())
      {
        PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), Mask_1);
      }
    }
  }
}

void CAMERA::apply_all_masks()
{
  if (!PROCESSED_FRAME.empty())
  {
    if (PROPS.ENH_CANNY_MASK)
    {
      apply_masks_to_processed_frame(MASK_FRAME_CANNY_0, MASK_FRAME_CANNY_1);
    }

    /*
    if (PROPS.ENH_OVERLAY_LINES)
    {
      cv::resize(MASK_FRAME_OVERLAY_LINES, MASK_FRAME_OVERLAY_LINES, PROCESSED_FRAME.size(), 0, 0, cv::INTER_NEAREST);
      PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), MASK_FRAME_OVERLAY_LINES);
    }
    */

    if (PROPS.ENH_GLARE_MASK)
    {
      apply_masks_to_processed_frame(MASK_FRAME_GLARE_0, MASK_FRAME_GLARE_1);
    }
  }
}



/**
 * @brief Configures the V4L2 device directly using ioctl. This explicit, raw setup 
 * is used to force the driver to read the full sensor field-of-view (FOV) and 
 * ensures the Codec is set before the Resolution.
 * This function attempts all settings and only returns false if the critical format 
 * (codec) setting fails.
 * @param print_stream Stream to log messages to.
 * @return True on successful configuration, false otherwise.
 */
bool CAMERA::configure_v4l2_roi_and_format(std::stringstream& print_stream)
{
    // --- 1. MANUALLY OPEN THE DEVICE WITH POSIX ---
    int fd = open(PROPS.CAMERA_DEVICE_NAME.c_str(), O_RDWR);
    if (fd < 0) {
        print_stream << "ERROR: Cannot open " << PROPS.CAMERA_DEVICE_NAME << " for raw V4L2 configuration." << std::endl;
        return false;
    }

    // Flag to track the most critical step: whether the requested codec was actually set.
    bool format_set_successfully = true; 

    // --- 2. DETERMINE PIXEL FORMAT ---
    __u32 pixelformat;
    std::string requested_format_name;
    if (PROPS.COMPRESSION == 1) {
        pixelformat = V4L2_PIX_FMT_MJPEG;
        requested_format_name = "MJPG";
    } else if (PROPS.COMPRESSION == 2) {
        pixelformat = V4L2_PIX_FMT_H264;
        requested_format_name = "H264";
    } else {
        pixelformat = V4L2_PIX_FMT_YUYV; // V4L2 uses YUYV for YUY2
        requested_format_name = "YUYV";
    }
    
    __u32 requested_pixelformat = pixelformat; // Store the requested format for validation
    
    // --- 3. RESET ROI / CROP TO FULL SENSOR (CRITICAL FIX) ---
    // The IMX291 has a maximum resolution of 1920x1080. We force the ROI to the full sensor at (0,0).
    struct v4l2_selection sel {};
    sel.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    sel.target = V4L2_SEL_TGT_CROP;
    sel.r.left = 0;
    sel.r.top = 0;
    sel.r.width  = 1920; 
    sel.r.height = 1080;

    if (ioctl(fd, VIDIOC_S_SELECTION, &sel) < 0) {
        print_stream << "WARNING: VIDIOC_S_SELECTION failed. Driver may not support explicit ROI reset." << std::endl;
        // Continue, as this is a non-critical setting for stream operation
    } else {
        print_stream << " > Successfully set V4L2 full sensor ROI to 1920x1080 at (0,0)." << std::endl;
    }


    // --- 4. FIRST PASS: SET FULL RESOLUTION + CODEC (To initialize compression correctly) ---
    struct v4l2_format fmt_full {};
    fmt_full.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt_full.fmt.pix.pixelformat = pixelformat;
    fmt_full.fmt.pix.width  = 1920;
    fmt_full.fmt.pix.height = 1080;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt_full) < 0) {
        print_stream << "ERROR: Could not set full-sensor format/resolution (VIDIOC_S_FMT 1920x1080) - IOCTL failed in first pass." << std::endl;
        format_set_successfully = false; // CRITICAL: Mark failure but CONTINUE to second pass
    } else {
        print_stream << " > First pass: Set full-res (" << requested_format_name << " 1920x1080)." << std::endl;
    }
    
    
    // --- 5. SECOND PASS: SET ACTUAL DESIRED RESOLUTION ---
    struct v4l2_format fmt_actual {};
    fmt_actual.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt_actual.fmt.pix.pixelformat = pixelformat; // Keep the requested format
    fmt_actual.fmt.pix.width  = PROPS.WIDTH;
    fmt_actual.fmt.pix.height = PROPS.HEIGHT;
    
    if (ioctl(fd, VIDIOC_S_FMT, &fmt_actual) < 0) {
        print_stream << "ERROR: Could not set actual resolution (VIDIOC_S_FMT " 
                     << PROPS.WIDTH << "x" << PROPS.HEIGHT << ") - IOCTL failed in second pass." << std::endl;
        format_set_successfully = false; // Mark failure but CONTINUE
    } else {
        // --- Validation Check: Did the driver accept the requested format on the second pass? ---
        if (fmt_actual.fmt.pix.pixelformat != requested_pixelformat) {
            // Decode the actual coerced format for reporting
            uint32_t actual_f = fmt_actual.fmt.pix.pixelformat;
            char actual_codec[] = {(char)(actual_f & 0xFF), (char)((actual_f >> 8) & 0xFF), (char)((actual_f >> 16) & 0xFF), (char)((actual_f >> 24) & 0xFF), 0};
            
            print_stream << "ERROR: Requested format (" << requested_format_name 
                         << ") rejected after second pass. Driver coerced to " << actual_codec << "." << std::endl;
            format_set_successfully = false; // CRITICAL FAILURE: Mark failure but CONTINUE
        }

        // Check if the requested resolution was actually set
        bool resolution_coerced = false;
        if (fmt_actual.fmt.pix.width != (__u32)PROPS.WIDTH || fmt_actual.fmt.pix.height != (__u32)PROPS.HEIGHT) {
            print_stream << "WARNING: Driver coerced final resolution to " << fmt_actual.fmt.pix.width << "x" << fmt_actual.fmt.pix.height << std::endl;
            resolution_coerced = true; // This is a WARNING, not a CRITICAL FAILURE
        }
        
        // Log final success summary
        if (format_set_successfully && !resolution_coerced) {
            print_stream << " > Successfully set final V4L2 format and resolution." << std::endl;
        } else if (format_set_successfully) {
            print_stream << " > Format set successfully, but resolution was coerced." << std::endl;
        }
    }


    // --- 6. SET FPS ---
    struct v4l2_streamparm parm {};
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = (__u32)PROPS.FPS;

    if (ioctl(fd, VIDIOC_S_PARM, &parm) < 0) {
        print_stream << "WARNING: Could not set FPS (VIDIOC_S_PARM). Driver may ignore this or use a default." << std::endl;
        // Continue, as this is a non-critical setting for stream operation
    } else {
         print_stream << " > Successfully set V4L2 target FPS to " << PROPS.FPS << "." << std::endl;
    }
    
    // Close the raw file descriptor. OpenCV will re-open and attach.
    close(fd); 
    
    // Final check: Only return true if the codec (the primary goal) was set successfully.
    if (!format_set_successfully) {
        print_stream << "CRITICAL FAILURE: V4L2 configuration failed to set required codec/format." << std::endl;
        return false;
    }

    return true;
}



void CAMERA::close_camera()
{
  std::stringstream print_stream;

  // 1. Check if the camera capture object is open and release it
  if (CAMERA_CAPTURE.isOpened())
  {
    CAMERA_CAPTURE.release();
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

  if (CAMERA_CAPTURE.isOpened())
  {
    print_stream << "Camera already open. Needs to be closed first." << endl;
  }
  else
  {
    // Create Thread (safe to recreate if already created)
    THREAD_CAMERA.create(5);           // Longest wait im main process
    THREAD_IMAGE_PROCESSING.create(5); // Longest wait im main process

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
      std::cout << "Car cascade loaded successfully." << std::endl;
    }

    // Start the camera 
    if (PROPS.TEST == false)
    {
      // --- STEP A: Optionally Perform Raw V4L2 Configuration ---
      if (PROPS.FORCE_V4L2_CONFIG) { // Controlled by the new flag
          print_stream << "Attempting raw V4L2 configuration..." << std::endl;
          configure_v4l2_roi_and_format(print_stream);
      } else {
            print_stream << "Using standard OpenCV configuration sequence." << std::endl;
            // NOTE: If FORCE_V4L2_CONFIG is false, you might want to add 
            // the original OpenCV 'set' calls back here if you still want
            // to attempt configuration without the raw ioctls.
            // For now, we assume raw V4L2 is the primary fix path.
      }

      CAMERA_CAPTURE.open(PROPS.CAMERA_DEVICE_NAME, cv::CAP_V4L2);
      CAM_AVAILABLE = CAMERA_CAPTURE.isOpened();

      if (CAMERA_CAPTURE.isOpened()) 
      {
        if (PROPS.FORCE_V4L2_CONFIG == false)
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
          CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_WIDTH, PROPS.WIDTH);
          CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_HEIGHT, PROPS.HEIGHT);
        }
        
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
          std::cout << "Codec: " << codec << std::endl;
          print_stream << "  > Codec Str:  " << codec << std::endl;  
        }

        // Set Post Processing Size
        POST_PROCESS_SIZE = {(int)(PROPS.POST_PROCESS_SCALE * (float)PROPS.WIDTH), 
                                        (int)(PROPS.POST_PROCESS_SCALE * (float)PROPS.HEIGHT)};

        // Initialize camera via backend.  First set normal operation mode, then gather all properties.
        init(print_stream);
        
        CAM_AVAILABLE = true;
      }
      else
      {
        print_stream << "Could not open camera. Please check your camera connection." << std::endl;
        CAM_AVAILABLE = false;
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
  //  Its sleep time should either be limited to the FORCED_FRAME_LIMIT_MS or the 
  //  hardware io limits of the camera read.
  // BUFFER_FRAME_HANDOFF_READY will signal to enhancement proccessing when 
  //  a new frame is ready, then continue the loop.
  // BEING_PROCESSED_FRAME is checked to ensure the new frame isnt placed in a 
  //  frame that is being processed.
  // Considering multiple new frames can be captured durng the processing phrase, 
  //  LATEST_READY_FRAME should report the newest frame.
  // FRAME_BUFFER_0, FRAME_BUFFER_1, BUFFER_FRAME_HANDOFF_READY, LATEST_READY_FRAME
  //  and BEING_PROCESSED_FRAME should be MUTEXed (mutex) to prevent data races, 
  //  but as is, the variables are playing nicely in their respective threads. 
void CAMERA::update_frame()
{
  // --- CONFIGURATION FOR ROBUSTNESS ---
  const int max_consecutive_errors = 50; // Allow 50 bad frames/loops before giving up
  int consecutive_connection_error_count = 0;
  int consecutive_frame_error_count = 0;

  // Create Process Time
  CAMERA_READ_THREAD_TIME.create();

  // Start the camera
  open_camera();

  CAMERA_ONLINE = true;

  while (CAMERA_READ_THREAD_STOP == false)
  {
    //  Get current time.  This will be our timeframe to work in.
    CAMERA_READ_THREAD_TIME.setframetime();

    // Enforce hard frame limit time.
    if (FORCED_FRAME_LIMIT.is_ready(CAMERA_READ_THREAD_TIME.current_frame_time()))
    {
      FORCED_FRAME_LIMIT.set(CAMERA_READ_THREAD_TIME.current_frame_time(), PROPS.FORCED_FRAME_LIMIT_MS); 

      // Grab start to loop times to compute fps.
      TIME_SE_CAMERA_FPS.end_clock();
      TIME_CAMERA_FPS = TIME_SE_CAMERA_FPS.duration_fps();
      TIME_CAMERA_FRAME_TIME = TIME_SE_CAMERA_FPS.duration_ms();
      TIME_SE_CAMERA_FPS.start_clock();
      
      // Save image to disk and get captured frame from camera.
      check_for_save_image_buffer_frame();

      // Check for restart
      if (APPLY_RESTART)
      {
        APPLY_RESTART = false;
        close_camera();
        PROPS.WIDTH = RESTART_WIDTH;
        PROPS.HEIGHT = RESTART_HEIGHT;
        PROPS.COMPRESSION = RESTART_COMPRESSION;
        open_camera();
      }

      if (CAMERA_BEING_VIEWED)
      {
        // Camera Open Error Checking.  Reconnect if possible.
        if (PROPS.TEST == false && CAMERA_CAPTURE.isOpened() == false)
        {
          consecutive_connection_error_count++;
          PRINTW_QUEUE.push_back("Camera signal lost. Attempting reconnect...");
          close_camera(); // Ensure old handle is gone

          bool not_connected = true;
          while (consecutive_connection_error_count <= max_consecutive_errors && not_connected)
          {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            open_camera(); // Try to reopen
            consecutive_connection_error_count++;
            not_connected = !CAMERA_CAPTURE.isOpened();
          }
        }

        // Stop Thread if too many errors
        if (consecutive_connection_error_count + consecutive_frame_error_count > max_consecutive_errors)
        {
          CAMERA_READ_THREAD_STOP = true;
          PRINTW_QUEUE.push_back("Camera Error - Connection Glitch");
          PRINTW_QUEUE.push_back("  Connection Error Count: " + to_string(consecutive_connection_error_count));
          PRINTW_QUEUE.push_back("  Frame Error Count: = " + to_string(consecutive_frame_error_count));
        }
        else
        {
          // Determine which buffer to put frame in.
          if (BEING_PROCESSED_FRAME == -1)
          {
            FRAME_TO_BUFFER = 0;
          }
          else
          {
            if (BEING_PROCESSED_FRAME == 0)
            {
              FRAME_TO_BUFFER = 1;
            }
            else
            if (BEING_PROCESSED_FRAME == 1)
            {
              FRAME_TO_BUFFER = 0;
            }
          }

          // Measure the time to run the routine.
          TIME_SE_FRAME_RETRIEVAL.start_clock();

          // Capture the camera frame.
          if (PROPS.TEST)
          {
            if (FRAME_TO_BUFFER == 0)
            {
              if (PROPS.TEST_IMAGE)
              {
                FRAME_DUMMY.copyTo(FRAME_BUFFER_0);
              }
              else
              {
                //FRAME_BUFFER_0_PRE = generateDummyFrame_2(POST_PROCESS_SIZE.width, POST_PROCESS_SIZE.height, (int)CAMERA_READ_THREAD_TIME.current_frame_time() / 100);
                FRAME_BUFFER_0 = generateDummyLowLightFrame(POST_PROCESS_SIZE.width, POST_PROCESS_SIZE.height, (int)CAMERA_READ_THREAD_TIME.current_frame_time() / 100);
              }
              LATEST_READY_FRAME = 0;
              BUFFER_FRAME_HANDOFF_READY = true;
            }
            else if (FRAME_TO_BUFFER == 1)
            {
              if (PROPS.TEST_IMAGE)
              {
                FRAME_DUMMY2.copyTo(FRAME_BUFFER_1);
              }
              else
              {
                //FRAME_BUFFER_1_PRE = generateDummyFrame_2(POST_PROCESS_SIZE.width, POST_PROCESS_SIZE.height, (int)CAMERA_READ_THREAD_TIME.current_frame_time() / 100);
                FRAME_BUFFER_1 = generateDummyLowLightFrame(POST_PROCESS_SIZE.width, POST_PROCESS_SIZE.height, (int)CAMERA_READ_THREAD_TIME.current_frame_time() / 100);
              }
              LATEST_READY_FRAME = 1;
              BUFFER_FRAME_HANDOFF_READY = true;
            }
          }
          else
          {
            if (FRAME_TO_BUFFER == 0)
            {
              CAMERA_CAPTURE >> FRAME_BUFFER_0;
              if (FRAME_BUFFER_0.empty())
              {
                consecutive_frame_error_count++;
              }
              else
              {
                LATEST_READY_FRAME = 0;
                consecutive_frame_error_count = 0;
                // Signal the enhancement processor that a new frame is available
                BUFFER_FRAME_HANDOFF_READY = true;
              }
            }
            else if (FRAME_TO_BUFFER == 1)
            {
              CAMERA_CAPTURE >> FRAME_BUFFER_1;
              if (FRAME_BUFFER_1.empty())
              {
                consecutive_frame_error_count++;
              }
              else
              {
                LATEST_READY_FRAME = 1;
                consecutive_frame_error_count = 0;
                // Signal the enhancement processor that a new frame is available
                BUFFER_FRAME_HANDOFF_READY = true;
              }
            }

            // Verification output
            if (FIRST_RUN == true) 
            {
              if (consecutive_frame_error_count == 0 && consecutive_connection_error_count == 0)
              {
                FIRST_RUN = false;

                size_t expected_step;
                size_t current_step;
                if (FRAME_TO_BUFFER == 0)
                {
                  expected_step = FRAME_BUFFER_0.cols * FRAME_BUFFER_0.channels() * FRAME_BUFFER_0.elemSize1();
                  current_step = FRAME_BUFFER_0.step[0];
                }
                else
                {
                  expected_step = FRAME_BUFFER_1.cols * FRAME_BUFFER_1.channels() * FRAME_BUFFER_0.elemSize1();
                  current_step = FRAME_BUFFER_1.step[0];
                }

                if (expected_step != current_step)
                {
                  PRINTW_QUEUE.push_back("Camera Error - Stride Mismatch");
                  PRINTW_QUEUE.push_back("   Current Mat Step : " + to_string(current_step));
                  PRINTW_QUEUE.push_back("  Expected Mat Step : = " + to_string(expected_step));
                }
                else
                {
                  PRINTW_QUEUE.push_back("Camera Stride Test Passed.");
                }

              }
              
            }


          }
        
          // Save the amout of time it took to read the frame.
          TIME_SE_FRAME_RETRIEVAL.end_clock();
          TIME_FRAME_RETRIEVAL = TIME_SE_FRAME_RETRIEVAL.duration_ms();
          
        }

      } // Cam Being Viewed
      else
      {
        // camera not being viewed. sleep theread on next pass
        FORCED_FRAME_LIMIT.set(CAMERA_READ_THREAD_TIME.current_frame_time(), 100);
      }
    } // Frame Limit Cap

    // Thread will need to sleep, governed by the FORCED_FRAME_LIMIT.
    CAMERA_READ_THREAD_TIME.request_ready_time(FORCED_FRAME_LIMIT.get_ready_time());
    CAMERA_READ_THREAD_TIME.sleep_till_next_frame();
  }

  CAMERA_ONLINE = false;

  // Close Camera if thread stops.
  close_camera();
  CAM_AVAILABLE = false;

  save_settings();
  PRINTW_QUEUE.push_back("Camera Closed");
}

void CAMERA::process_enhancements_frame()
{  
  TIME_SE_FRAME_PROCESSING.start_clock();

  check_for_save_image_buffer_processed();

  BEING_PROCESSED_FRAME = LATEST_READY_FRAME;

  if (BEING_PROCESSED_FRAME == 0)
  {
    run_preprocessing(FRAME_BUFFER_0, PROCESS_ENHANCEMENTS_FRAME_TIME);
    // No longer dependant on Buffer frame.  Release.
    BEING_PROCESSED_FRAME = -1;
    apply_ehancements();
    apply_all_masks();
  }
  else
  if (BEING_PROCESSED_FRAME == 1)
  {
    run_preprocessing(FRAME_BUFFER_1, PROCESS_ENHANCEMENTS_FRAME_TIME);
    // No longer dependant on Buffer frame.  Release.
    BEING_PROCESSED_FRAME = -1;
    apply_ehancements();
    apply_all_masks();
  }

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

    FRAME_BUFFER_FAKE = FAKE_FRAME_GENERATOR.interpolateFrame(PROCESSED_FRAME);
  }
  
  TIME_SE_FRAME_PROCESSING.end_clock();
  TIME_FRAME_PROCESSING = TIME_SE_FRAME_PROCESSING.duration_ms();

  NEW_FRAME_AVAILABLE = true;
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
    Frame.copyTo(LIVE_FRAME);
  }
}

bool CAMERA::set_camera_control(CAMERA_SETTING &Setting)
{
  bool ret_success = false;

  //if (CAM_AVAILABLE && Setting.SET_VALUE != Setting.VALUE)
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
  //else
  //{
  //  ret_success = false;
  //}

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
void CAMERA::list_controls(CONSOLE_COMMUNICATION &cons)
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

  cons.printw(INFORMATION_COMMAND_LIST);

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
  if (CAMERA_BEING_VIEWED != Camera_Being_Viewed)
  {
    CAMERA_BEING_VIEWED = Camera_Being_Viewed;

    if (CAMERA_BEING_VIEWED == false)
    {
      FRAME_BUFFER_0.release();
      FRAME_BUFFER_1.release();
      FRAME_BUFFER_FAKE.release();
      LIVE_FRAME.release();
      LIVE_FRAME_1.release();

      PROCESSED_FRAME.release();
      PROCESSED_FRAME_GRAY.release();
      //PROCESSED_FRAME_GAUSSIAN.release();
      PROCESSED_FRAME_CANNY.release();

      MASK_FRAME_GLARE_0.release();
      MASK_FRAME_GLARE_1.release();

      MASK_FRAME_CANNY_0.release();
      MASK_FRAME_CANNY_1.release();
    }
  }
  
  // ---------------------------------------------------------------------------------------
  // Check Thread Completions
  
  // Check to see if THREAD_CAMERA has stopped for any reason. 
  THREAD_CAMERA.check_for_completition();

  // ---------------------------------------------------------------------------------------
  // Check Thread Completions
  
  // When the working frame has been fully process, render the 
  //  texture to be drawn in opengl.
  if (THREAD_IMAGE_PROCESSING.check_for_completition())
  {
    // Converts PROCESSED_FRAME into ImGui Texture to be rendered
    //  into program display.
    // Copies PROCESSED_FRAME to LIVE_FRAME for thread safe access.

    // Only proceed if the frame is not empty.
    if (NEW_FRAME_AVAILABLE)
    {
      NEW_FRAME_AVAILABLE = false;
      FRAME_TO_TEXTURE_TRACK = 1;

      if (PROPS.ENH_FAKE_FRAMES == false)
      {
        PROCESSED_FRAME.copyTo(LIVE_FRAME_1);
      }
      else
      {
        FRAME_BUFFER_FAKE.copyTo(LIVE_FRAME_0);
        PROCESSED_FRAME.copyTo(LIVE_FRAME_1);
      }
    }
  }

  // ---------------------------------------------------------------------------------------

  // Starts the THREAD_CAMERA unless CAMERA_READ_THREAD_STOP is true. 
  //  THREAD_CAMERA will continue running until the camera is closed.
  {
    if (CAMERA_READ_THREAD_STOP == false)
    {
      if (THREAD_CAMERA.check_to_run_routine_on_thread(Frame_Time)) 
      {
        // Start the camera update on a separate thread.
        // This call is non-blocking, so the main loop can continue immediately.
        THREAD_CAMERA.start_render_thread([&]() 
                  {  update_frame();  });
      }
    }
  }

  // ---------------------------------------------------------------------------------------

  // Only try to process frames when a handoff is complete and 
  //  the texture is created.
  if (BUFFER_FRAME_HANDOFF_READY && WORKING_FRAME_FULLY_PROCESSED)
  {
    // Do not start thread if already running.
    if (THREAD_IMAGE_PROCESSING.check_to_run_routine_on_thread(Frame_Time)) 
    {
      BUFFER_FRAME_HANDOFF_READY = false;
      WORKING_FRAME_FULLY_PROCESSED = false;

      if (CAMERA_BEING_VIEWED)
      {
        //  In Place here because weird process_enhancements_frame didnt safely
        //    accept an argument.
        PROCESS_ENHANCEMENTS_FRAME_TIME = Frame_Time;

        // Start the camera update on a separate thread.
        // This call is non-blocking, so the main loop can continue immediately.
        THREAD_IMAGE_PROCESSING.start_render_thread([&]() 
                  {  process_enhancements_frame();  });
      }
    }
  }

  // ---------------------------------------------------------------------------------------

  if (CAMERA_BEING_VIEWED == false)
  {
    FRAME_TO_TEXTURE_TRACK = 0;
    WORKING_FRAME_FULLY_PROCESSED = true;
  }
  else if (FRAME_TO_TEXTURE_TRACK != 0)
  {
    if (PROPS.ENH_FAKE_FRAMES == false || INTERPOLATION_DISPLAY == false)
    {
      FRAME_GEN = false;

      FRAME_TO_TEXTURE_TRACK = 0;
      generate_imgui_texture_frame(LIVE_FRAME_1);
      WORKING_FRAME_FULLY_PROCESSED = true;
    }
    else  // PROPS.ENH_FAKE_FRAMES == true
    {
      FRAME_GEN = true;

      if (FRAME_TO_TEXTURE_TRACK == 1)
      {
        FRAME_TO_TEXTURE_TRACK = 2;
        FRAME_TO_TEXTURE_TIMER.set(Frame_Time, (int)TIME_CAMERA_FRAME_TIME / 2);
        generate_imgui_texture_frame(LIVE_FRAME_0);
        WORKING_FRAME_FULLY_PROCESSED = true;
      }
      else if (FRAME_TO_TEXTURE_TRACK == 2)
      {
        if (FRAME_TO_TEXTURE_TIMER.is_ready(Frame_Time))
        {
          FRAME_TO_TEXTURE_TRACK = 0;
          generate_imgui_texture_frame(LIVE_FRAME_1);
          //WORKING_FRAME_FULLY_PROCESSED = true;
        }
      }
    }
  }

  // Print whatever is in the console print_wait queue.
  print_stream(cons);
}

cv::Mat CAMERA::get_current_frame()
{
  // Return a copy of the FRAME to the caller.
  return LIVE_FRAME;
}

void CAMERA::take_snapshot()
{
  SAVE_IMAGE_BUFFER_FRAME = true;
  SAVE_IMAGE_PROCESSED_FRAME = true;
}

void CAMERA::load(CONSOLE_COMMUNICATION &cons)
{
  vector<CAMERA_CONTROL_SETTING_LOADED> loaded_camera_control;
  load_settings_json(loaded_camera_control);
  list_controls(cons);
  apply_loaded_camera_controls(loaded_camera_control, SETTINGS);
  APPLY_CHANGES = true;
}

void CAMERA::camera_start()
{
  if (CAMERA_READ_THREAD_STOP == true)
  {
    // Signal stop thread.
    CAMERA_READ_THREAD_STOP = false;
  }
}

void CAMERA::camera_stop()
{
  if (CAMERA_READ_THREAD_STOP == false)
  {
    // Signal stop thread.
    CAMERA_READ_THREAD_STOP = true;
  }
}

bool CAMERA::camera_online()
{
  return CAMERA_ONLINE;
}

// ---------------------------------------------------------------------------------------

#endif
