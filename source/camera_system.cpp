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

// Helper function implementation: checks the mean brightness of the frame
bool CAMERA::is_low_light(const cv::Mat& Grey_Image_Full_Size, int threshold) 
{
  // Calculate the mean intensity (0-255). If it's below the threshold, it's dark.
  cv::Scalar mean_intensity = cv::mean(Grey_Image_Full_Size);
  return mean_intensity[0] < threshold;
}

void  CAMERA::gray_enhance(cv::Mat& processed_frame, const cv::Mat& Grey_Image_Full_Size)
{
  // --- Step 2A: Low-Light Contrast Enhancement (CLAHE) ---
  // Grayscale conversion is required to isolate the luminance channel for CLAHE.
  cv::cvtColor(processed_frame, Grey_Image_Full_Size, cv::COLOR_BGR2GRAY);

  // CLAHE settings: Clip Limit 2.0, Tile Size 8x8
  cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
  clahe->apply(Grey_Image_Full_Size, Grey_Image_Full_Size); 

  // Apply enhanced luminance back to the color frame (making it grayscale enhanced)
  cv::cvtColor(Grey_Image_Full_Size, processed_frame, cv::COLOR_GRAY2BGR);
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
  const int DILATE_ITERATIONS = 1;  // Number of times to thicken edges

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
 * @brief Converts an OpenCV Mat object to an OpenGL texture ID.
 *
 * This function handles the creation of a new OpenGL texture or updates an
 * existing one, then uploads the pixel data from the OpenCV Mat object.
 *
 * @param frame The OpenCV Mat object with image data.
 * @param textureID The existing OpenGL texture ID to update, or 0 to create a new one.
 * @return The OpenGL texture ID (GLuint).
 */
GLuint CAMERA::matToTexture(const cv::Mat& frame, GLuint textureID)
{
  // Check if the frame is empty. If it is, return 0 (an invalid texture ID).
  if (frame.empty())
  {
    std::cerr << "Error: The OpenCV Mat is empty." << std::endl;
    return 0;
  }

  // Convert the image to RGB format.
  // The GL_RGB format expects the channels in R-G-B order,
  // but OpenCV loads them in B-G-R order by default.
  cv::Mat rgbFrame;
  cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

  // If the texture hasn't been created yet, create it.
  if (textureID == 0) 
  {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters for the first time.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the pixel data from the OpenCV Mat to the OpenGL texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgbFrame.cols, rgbFrame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbFrame.data);
  } 
  else 
  {
    // If the texture already exists, just update its data.
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rgbFrame.cols, rgbFrame.rows, GL_RGB, GL_UNSIGNED_BYTE, rgbFrame.data);
  }

  // Unbind the texture to prevent accidental modifications.
  glBindTexture(GL_TEXTURE_2D, 0);

  return textureID;
}

void CAMERA::prepare()
{
  PROPS.CTRL_FOCUS_AUTO.ADDRESS = 0x009a090c;
  PROPS.CTRL_FOCUS_AUTO.MINIMUM = 0;
  PROPS.CTRL_FOCUS_AUTO.MAXIMUM = 1;
  PROPS.CTRL_FOCUS_AUTO.DEFAULT = 1;

  PROPS.CTRL_FOCUS_ABSOLUTE.ADDRESS = 0x009a090a;
  PROPS.CTRL_FOCUS_ABSOLUTE.MINIMUM = 0;
  PROPS.CTRL_FOCUS_ABSOLUTE.MAXIMUM = 21;
  PROPS.CTRL_FOCUS_ABSOLUTE.DEFAULT = 16;

  PROPS.CTRL_EXPOSURE_AUTO.ADDRESS = 0x009a0901;
  PROPS.CTRL_EXPOSURE_AUTO.MINIMUM = 0;
  PROPS.CTRL_EXPOSURE_AUTO.MAXIMUM = 3;
  PROPS.CTRL_EXPOSURE_AUTO.DEFAULT = 3;

  PROPS.CTRL_EXPOSURE_TIME_ABSOLUTE.ADDRESS = 0x009a0902;
  PROPS.CTRL_EXPOSURE_TIME_ABSOLUTE.MINIMUM = 4;
  PROPS.CTRL_EXPOSURE_TIME_ABSOLUTE.MAXIMUM = 5000;
  PROPS.CTRL_EXPOSURE_TIME_ABSOLUTE.DEFAULT = 625;

  PROPS.CTRL_BACKLIGHT_COMPENSATION.ADDRESS = 0x0098091c;
  PROPS.CTRL_BACKLIGHT_COMPENSATION.MINIMUM = 0;
  PROPS.CTRL_BACKLIGHT_COMPENSATION.MAXIMUM = 1;
  PROPS.CTRL_BACKLIGHT_COMPENSATION.DEFAULT = 1;

  PROPS.CTRL_SHARPNESS.ADDRESS = 0x0098091b;
  PROPS.CTRL_SHARPNESS.MINIMUM = 0;
  PROPS.CTRL_SHARPNESS.MAXIMUM = 15;
  PROPS.CTRL_SHARPNESS.DEFAULT = 6;

  PROPS.CTRL_WHITE_BALANCE_AUTOMATIC.ADDRESS = 0x0098090c;
  PROPS.CTRL_WHITE_BALANCE_AUTOMATIC.MINIMUM = 0;
  PROPS.CTRL_WHITE_BALANCE_AUTOMATIC.MAXIMUM = 1;
  PROPS.CTRL_WHITE_BALANCE_AUTOMATIC.DEFAULT = 1;

  PROPS.CTRL_WHITE_BALANCE_TEMP.ADDRESS = 0x0098091a;
  PROPS.CTRL_WHITE_BALANCE_TEMP.MINIMUM = 2800;
  PROPS.CTRL_WHITE_BALANCE_TEMP.MAXIMUM = 6500;
  PROPS.CTRL_WHITE_BALANCE_TEMP.DEFAULT = 2800;

  PROPS.CTRL_GAIN.ADDRESS = 0x00980913;
  PROPS.CTRL_GAIN.MINIMUM = 0;
  PROPS.CTRL_GAIN.MAXIMUM = 0;
  PROPS.CTRL_GAIN.DEFAULT = 0;

  PROPS.CTRL_GAMA.ADDRESS = 0x00980910;
  PROPS.CTRL_GAMA.MINIMUM = 1;
  PROPS.CTRL_GAMA.MAXIMUM = 10;
  PROPS.CTRL_GAMA.DEFAULT = 7;

  PROPS.CTRL_HUE.ADDRESS = 0x00980903;
  PROPS.CTRL_HUE.MINIMUM = 10;
  PROPS.CTRL_HUE.MAXIMUM = 10;
  PROPS.CTRL_HUE.DEFAULT = 10;
}

void CAMERA::init(stringstream &Print_Stream)
{
  // Initial Camera Setup
  // disable continuous autofocus
  set_camera_control(PROPS.CTRL_FOCUS_AUTO, 0);
  // set manual focus value (depends on your camera’s supported range)
  set_camera_control(PROPS.CTRL_FOCUS_ABSOLUTE, 0);

  // Verify settings
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
}

void CAMERA::save_settings()
{
  JSON_INTERFACE settings;
  JSON_ENTRY camera_settings;
  JSON_ENTRY backup_camera_settings;

  // Standar Settings
  camera_settings.create_label_value(quotify("WIDTH"), quotify(to_string(PROPS.WIDTH)));
  camera_settings.create_label_value(quotify("HEIGHT"), quotify(to_string(PROPS.HEIGHT)));
  camera_settings.create_label_value(quotify("FLIP_HORIZONTAL"), quotify(to_string(PROPS.FLIP_HORIZONTAL)));
  camera_settings.create_label_value(quotify("FLIP_VERTICAL"), quotify(to_string(PROPS.FLIP_VERTICAL)));
  camera_settings.create_label_value(quotify("FORCED_FRAME_LIMIT_MS"), quotify(to_string(PROPS.FORCED_FRAME_LIMIT_MS)));
  camera_settings.create_label_value(quotify("TEST"), quotify(to_string(PROPS.TEST)));
  camera_settings.create_label_value(quotify("TEST_IMAGE"), quotify(to_string(PROPS.TEST_IMAGE)));
  camera_settings.create_label_value(quotify("TEST_MULTI_FRAME"), quotify(to_string(PROPS.TEST_MULTI_FRAME)));

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
  settings.ROOT.put_json_in_set(quotify("camera settings"), camera_settings);

  deque<std::string> camera_settings_json_deque;
  settings.json_print_build_to_string_deque(camera_settings_json_deque);

  threaded_deque_string_to_file(PROPS.CAMERA_SETTINGS_DIR + PROPS.CAMERA_NAME + "_camera_settings.json", camera_settings_json_deque);
}

void CAMERA::load_settings_json()
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
      for(size_t root = 0; root < settings.ROOT.DATA.size(); root++)        //root
      {
        if (settings.ROOT.DATA[root].label() == "camera settings")
        {
          STRING_INT    si_width;
          STRING_INT    si_height;
          STRING_BOOL   sb_flip_horizontal;
          STRING_BOOL   sb_flip_vertical;
          STRING_INT    si_forced_frame_limit;
          STRING_BOOL   sb_test;
          STRING_BOOL   sb_test_image;
          STRING_BOOL   sb_test_multi_frame;

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

          for (size_t entry_list = 0;                                        //root/marker_list
                      entry_list < settings.ROOT.DATA[root].DATA.size(); entry_list++)
          {
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("WIDTH", si_width);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("HEIGHT", si_height);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_HORIZONTAL", sb_flip_horizontal);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FLIP_VERTICAL", sb_flip_vertical);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("FORCED_FRAME_LIMIT_MS", si_forced_frame_limit);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST", sb_test);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST_IMAGE", sb_test_image);
            settings.ROOT.DATA[root].DATA[entry_list].get_if_is("TEST_MULTI_FRAME", sb_test_multi_frame);

            if (settings.ROOT.DATA[root].DATA[entry_list].label() == "backup camera settings")
            {
              for (size_t backup_cam_entry_list = 0;                                        //root/marker_list
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
void CAMERA::run_preprocessing(cv::Mat &Frame)
{
  if (!Frame.empty())
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

  // CV Code Here: Conditional and Controllable Image Enhancement Pipeline
  // The entire processing block is now conditional on the user controls.
  if (!PROCESSED_FRAME.empty())
  {
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

    // First Level of enhancements and preprocessing
    // Generate Processing Frames for enhancements.
    {
      // Boost or reduce all color brightness
      if (PROPS.ENH_COLOR)
      {
        apply_min_max_contrast(PROCESSED_FRAME);
      }

      // If still low light after boost, grey scale and increase contrast.
      // Put in both PROCESSED_FRAME and PROCESSED_FRAME_GRAY
      if (PROPS.ENH_LOW_LIGHT && is_low_light(PROCESSED_FRAME_GRAY, 50))
      {
        cv::cvtColor(PROCESSED_FRAME, PROCESSED_FRAME_GRAY, cv::COLOR_BGR2GRAY);
        gray_enhance(PROCESSED_FRAME, PROCESSED_FRAME_GRAY);
        cv::cvtColor(PROCESSED_FRAME, PROCESSED_FRAME_GRAY, cv::COLOR_BGR2GRAY);
      }
      else
      {
        cv::cvtColor(PROCESSED_FRAME, PROCESSED_FRAME_GRAY, cv::COLOR_BGR2GRAY);
      }

      cv::resize(PROCESSED_FRAME, PROCESSED_FRAME_DOWNSIZED, cv::Size(), 1.0 / DOWN_SCALE_FACTOR, 1.0 / DOWN_SCALE_FACTOR, cv::INTER_LINEAR);
      cv::resize(PROCESSED_FRAME_GRAY, PROCESSED_FRAME_GRAY_DOWNSIZED, cv::Size(), 1.0 / DOWN_SCALE_FACTOR, 1.0 / DOWN_SCALE_FACTOR, cv::INTER_LINEAR);
      cv::GaussianBlur(PROCESSED_FRAME_GRAY, PROCESSED_FRAME_GAUSSIAN, cv::Size(BLUR_KSIZE, BLUR_KSIZE), 0);
      cv::Canny(PROCESSED_FRAME_GAUSSIAN, PROCESSED_FRAME_CANNY, CANNY_THRESH_LOW, CANNY_THRESH_HIGH, CANNY_APERTURE);
    }
  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::apply_ehancements()
{
  if (!PROCESSED_FRAME.empty())
  {
    // Canny Mask
    if (PROPS.ENH_CANNY_MASK)
    {
      MASK_FRAME_CANNY = canny_mask(PROCESSED_FRAME_CANNY);
    }

    // Overlay lines
    if (PROPS.ENH_OVERLAY_LINES)
    {
      MASK_FRAME_OVERLAY_LINES = overlay_lines(PROCESSED_FRAME_GRAY_DOWNSIZED);
    }

    //---

    // Glare Mask
    if (PROPS.ENH_GLARE_MASK)
    {
      MASK_FRAME_GLARE = suppress_glare_mask(PROCESSED_FRAME_DOWNSIZED);
    }

    //---

    if (PROPS.ENH_HOUGH)
    {
      detect_hough_circles(PROCESSED_FRAME, PROCESSED_FRAME_GAUSSIAN);
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

  if (PROPS.ENH_CANNY_MASK)
  {
    PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), MASK_FRAME_CANNY);
  }

  if (PROPS.ENH_OVERLAY_LINES)
  {
    cv::resize(MASK_FRAME_OVERLAY_LINES, MASK_FRAME_OVERLAY_LINES, PROCESSED_FRAME.size(), 0, 0, cv::INTER_NEAREST);
    PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), MASK_FRAME_OVERLAY_LINES);
  }

  if (PROPS.ENH_GLARE_MASK)
  {
    cv::resize(MASK_FRAME_GLARE, MASK_FRAME_GLARE, PROCESSED_FRAME.size(), 0, 0, cv::INTER_NEAREST);
    PROCESSED_FRAME.setTo(cv::Scalar(0, 0, 0), MASK_FRAME_GLARE);
  }
}


void CAMERA::open_camera()
{
  std::stringstream print_stream;

  if (CAMERA_CAPTURE.isOpened())
  {
    print_stream << "Camera already open. Needs to be closed first." << endl;
  }
  else
  {
    // Create Thread (safe to recreate if already created)
    THREAD_CAMERA.create(60);           // Longest wait im main process
    THREAD_IMAGE_PROCESSING.create(15); // Longest wait im main process

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
      // Set up all camera properties.
      prepare();
      
      // Attempt to open the default camera (index 0).
      CAMERA_CAPTURE.open(PROPS.CAMERA_DEVICE_ID);

      // Check if the camera was successfully opened.
      CAM_AVAILABLE = CAMERA_CAPTURE.isOpened();
      if (CAM_AVAILABLE)
      {
        // If the camera is opened, set its frame width and height.
        CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_WIDTH, PROPS.WIDTH);
        CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_HEIGHT, PROPS.HEIGHT);

        print_stream << "Camera successfully opened and properties set." << std::endl;

        // Now, probe the camera for its actual capabilities and print them.
        double actual_width = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_WIDTH);
        double actual_height = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_HEIGHT);
        double actual_fps = CAMERA_CAPTURE.get(cv::CAP_PROP_FPS);

        // Show some of the cv settings
        print_stream << "Actual camera resolution: " << actual_width << "x" << actual_height << std::endl;
        print_stream << "Actual camera FPS: " << actual_fps << std::endl;

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
void CAMERA::update_frame()
{
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

  CAMERA_READ_THREAD_TIME.create();

  open_camera();

  while (CAMERA_READ_THREAD_STOP == false)
  {
    //  Get current time.  This will be our timeframe to work in.
    CAMERA_READ_THREAD_TIME.setframetime();

    // Enforce hard frame limit time.
    if (FORCED_FRAME_LIMIT.is_ready(CAMERA_READ_THREAD_TIME.current_frame_time()))
    {
      FORCED_FRAME_LIMIT.set(CAMERA_READ_THREAD_TIME.current_frame_time(), PROPS.FORCED_FRAME_LIMIT_MS); 

      // Grab start to loop times to compute fps.
      TIME_SE_MAX_FPS.end_clock();
      TIME_MAX_FPS = TIME_SE_MAX_FPS.duration_fps();
      TIME_SE_MAX_FPS.start_clock();
      
      // Save image to disk and get captured frame from camera.
      check_for_save_image_buffer_frame();

      // Determine which buffer to put frame in.
      if (BEING_PROCESSED_FRAME == -1)
      {
        if (FRAME_TO_BUFFER == 0)
        {
          FRAME_TO_BUFFER = 1;
        }
        else
        if (FRAME_TO_BUFFER == 1)
        {
          FRAME_TO_BUFFER = 0;
        }
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
          FRAME_DUMMY.copyTo(FRAME_BUFFER_0);
          LATEST_READY_FRAME = 0;
        }
        else if (FRAME_TO_BUFFER == 1)
        {
          FRAME_DUMMY2.copyTo(FRAME_BUFFER_1);
          LATEST_READY_FRAME = 1;
        }
      }
      else
      {
        if (FRAME_TO_BUFFER == 0)
        {
          CAMERA_CAPTURE >> FRAME_BUFFER_0;
          LATEST_READY_FRAME = 0;
          
          // Check for errors.  close camera if empty frame or not connected.
          if (CAMERA_CAPTURE.isOpened() == false || FRAME_BUFFER_0.empty())
          {
            PRINTW_QUEUE.push_back("Camera Error");
            PRINTW_QUEUE.push_back("Is opened = " + to_string(CAMERA_CAPTURE.isOpened()) +
                                    " Frame Buffer Empty = " + to_string(FRAME_BUFFER_0.empty()));
            CAMERA_READ_THREAD_STOP = true;
          }
        }
        else if (FRAME_TO_BUFFER == 1)
        {
          CAMERA_CAPTURE >> FRAME_BUFFER_1;
          LATEST_READY_FRAME = 1;
          
          // Check for errors.  close camera if empty frame or not connected.
          if (CAMERA_CAPTURE.isOpened() == false || FRAME_BUFFER_1.empty())
          {
            PRINTW_QUEUE.push_back("Camera Error");
            PRINTW_QUEUE.push_back("Is opened = " + to_string(CAMERA_CAPTURE.isOpened()) +
                                    " Frame Buffer Empty = " + to_string(FRAME_BUFFER_1.empty()));
            CAMERA_READ_THREAD_STOP = true;
          }
        }
      }

      // Save the amout of time it took to read the frame.
      TIME_SE_FRAME_RETRIEVAL.end_clock();
      TIME_FRAME_RETRIEVAL = TIME_SE_FRAME_RETRIEVAL.duration_ms();

      // Signal the enhancement processor that a new frame is available
      BUFFER_FRAME_HANDOFF_READY = true;
    }

    // Thread will need to sleep, governed by the FORCED_FRAME_LIMIT.
    CAMERA_READ_THREAD_TIME.request_ready_time(FORCED_FRAME_LIMIT.get_ready_time());
    CAMERA_READ_THREAD_TIME.sleep_till_next_frame();
  }

  // Close Camera if thread stops.
  CAMERA_CAPTURE.release();
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
    run_preprocessing(FRAME_BUFFER_0);
    // No longer dependant on Buffer frame.  Release.
    BEING_PROCESSED_FRAME = -1;
    apply_ehancements();
  }
  else
  if (BEING_PROCESSED_FRAME == 1)
  {
    run_preprocessing(FRAME_BUFFER_1);
    // No longer dependant on Buffer frame.  Release.
    BEING_PROCESSED_FRAME = -1;
    apply_ehancements();
  }

  TIME_SE_FRAME_PROCESSING.end_clock();
  TIME_FRAME_PROCESSING = TIME_SE_FRAME_PROCESSING.duration_ms();
  
  NEW_FRAME_AVAILABLE = true;

}

void CAMERA::generate_imgui_texture_frame()
{
  // Only proceed if the frame is not empty.
  if (NEW_FRAME_AVAILABLE)
  {
    NEW_FRAME_AVAILABLE = false;

    if (!PROCESSED_FRAME.empty())
    {
      // Convert the frame to an OpenGL texture.
      // We pass the member variable to reuse the same texture.
      TEXTURE_ID = matToTexture(PROCESSED_FRAME, TEXTURE_ID);

      // Create Thread safe mat frame to access
      PROCESSED_FRAME.copyTo(LIVE_FRAME);
    }
    else
    {
    }
  }

  WORKING_FRAME_FULLY_PROCESSED = true;
}

bool CAMERA::set_camera_control(CAMERA_SETTING &Setting, int Value)
{
  bool ret_success = false;

  if (CAM_AVAILABLE && Value != Setting.SET_VALUE)
  {
    if (Value < Setting.MINIMUM)
    {
      if (set_control(Setting.ADDRESS, (int32_t)Setting.MINIMUM))
      {
        Setting.SET_VALUE = Setting.MINIMUM;
        ret_success = true;
      }
    }
    else if (Value > Setting.MAXIMUM)
    {
      if (set_control(Setting.ADDRESS, (int32_t)Setting.MAXIMUM))
      {
        Setting.SET_VALUE = Setting.MAXIMUM;
        ret_success = true;
      }
    }
    else 
    {
      if (set_control(Setting.ADDRESS, (int32_t)Value))
      {
        Setting.SET_VALUE = Value;
        ret_success = true;
      }
    }
  }

  return ret_success;
}

int CAMERA::get_camera_control_value(CAMERA_SETTING &Setting)
{
  int ret_value = -1;

  if (CAM_AVAILABLE)
  {
    ret_value = get_control(Setting.ADDRESS);
    if (ret_value != -1)
    {
      Setting.SET_VALUE = ret_value;
    }
  }

  return ret_value;
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
      std::stringstream print_stream;
      
      // Start row and set alignment to left
      print_stream << std::left << "| ";

      // ID (Hex) - 8 chars for hex, plus "0x" prefix
      print_stream << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(' ') << queryctrl.id << std::dec << " | ";
      
      // Reset fill/case and use appropriate widths for text/decimal
      print_stream << std::setfill(' ') << std::left;

      // Control Name - max 32 chars
      print_stream << std::setw(30) << queryctrl.name << " |";
      
      // Type
      if (queryctrl.type == 1)
      {
        print_stream << std::setw(6) << "int" << "|";
      }
      else if (queryctrl.type == 2)
      {
        print_stream << std::setw(6) << "bool" << "|";
      }
      else if (queryctrl.type == 3)
      {
        print_stream << std::setw(6) << "menu" << "|";
      }
      else 
      {
        print_stream << std::setw(6) << queryctrl.type << "|";
      }

      // Min, Max, Default, Step - right aligned numbers
      print_stream << std::right;
      print_stream << std::setw(5) << queryctrl.minimum << "|";
      print_stream << std::setw(5) << queryctrl.maximum << "|";
      print_stream << std::setw(5) << queryctrl.default_value << "|";
      print_stream << std::setw(5) << queryctrl.step << "|\n"; // End of row

      INFORMATION_COMMAND_LIST += print_stream.str();
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

void CAMERA::print_stream(CONSOLE_COMMUNICATION &cons)
{
  if (PRINTW_QUEUE.size() > 0)
  {
    std::string print_buffer = "";
    for (size_t pos = 0; pos < PRINTW_QUEUE.size(); pos++)
    {
      print_buffer += PRINTW_QUEUE[pos];
    }
    PRINTW_QUEUE.clear();
    cons.printw(print_buffer);
  }
}

void CAMERA::process(CONSOLE_COMMUNICATION &cons, unsigned long Frame_Time, bool Camera_Being_Viewed)
{
  // Check to see if THREAD_CAMERA has stopped for any reason. 
  THREAD_CAMERA.check_for_completition();

  // When the working frame has been fully process, render the 
  //  texture to be drawn in opengl.
  if (THREAD_IMAGE_PROCESSING.check_for_completition())
  {
    // Converts PROCESSED_FRAME into ImGui Texture to be rendered
    //  into program display.
    // Copies PROCESSED_FRAME to LIVE_FRAME for thread safe access.
    generate_imgui_texture_frame();
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

  // Only try to process frames when a haldoff is complete and 
  //  the texture is created.
  if (BUFFER_FRAME_HANDOFF_READY && WORKING_FRAME_FULLY_PROCESSED)
  {
    if (THREAD_IMAGE_PROCESSING.check_to_run_routine_on_thread(Frame_Time)) 
    {
      BUFFER_FRAME_HANDOFF_READY = false;
      WORKING_FRAME_FULLY_PROCESSED = false;

      if (Camera_Being_Viewed)
      {
        // Start the camera update on a separate thread.
        // This call is non-blocking, so the main loop can continue immediately.
        THREAD_IMAGE_PROCESSING.start_render_thread([&]() 
                  {  process_enhancements_frame();  });
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

void CAMERA::load_settings()
{
  load_settings_json();
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

bool CAMERA::camera_avalable()
{
  return !CAMERA_READ_THREAD_STOP;
}

// ---------------------------------------------------------------------------------------

#endif
