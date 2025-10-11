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
bool CAMERA::is_low_light(const cv::Mat& frame, int threshold) 
{
  if (frame.empty()) return false;
  
  // Convert to grayscale to check overall luminance
  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

  // Calculate the mean intensity (0-255). If it's below the threshold, it's dark.
  cv::Scalar mean_intensity = cv::mean(gray);
  return mean_intensity[0] < threshold;
}

// NEW Helper function: Generates a trapezoidal mask focusing on the road ahead.
cv::Mat CAMERA::get_road_mask(const cv::Mat& frame) 
{
  cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
  int width = frame.cols;
  int height = frame.rows;

  // Define the vertices of the trapezoid (ROI). These points are tuned 
  // for a typical dashcam view, focusing on the bottom half and center.
  std::vector<cv::Point> vertices = 
  {
    cv::Point(0, height),             // Bottom left (full width)
    cv::Point(width, height),         // Bottom right (full width)
    cv::Point(width * 0.55, height * 0.6), // Top right (near horizon)
    cv::Point(width * 0.45, height * 0.6)  // Top left (near horizon)
  };

  const cv::Point* points[1] = { &vertices[0] };
  int npoints[] = { (int)vertices.size() };
  
  // Fill the trapezoid area on the mask with white (255)
  cv::fillPoly(mask, points, npoints, 1, cv::Scalar(255));
  
  return mask;
}

// NEW Helper function: Detects and draws general contours (outlines of objects).
void CAMERA::detect_and_draw_contours(cv::Mat& processed_frame) 
{
  if (processed_frame.empty()) return;

  // --- 1. Isolate Edges/Outlines ---
  cv::Mat gray_edges;
  cv::cvtColor(processed_frame, gray_edges, cv::COLOR_BGR2GRAY);
  
  // Use Canny Edge Detection to find sharp boundaries
  cv::Mat edges;
  cv::Canny(gray_edges, edges, 50, 150, 3);
  
  // --- 2. Find Contours ---
  // cv::RETR_EXTERNAL retrieves only the extreme outer contours.
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // --- 3. Filter and Draw Contours ---
  for (size_t i = 0; i < contours.size(); ++i) 
  {
    // Filter contours by area to ignore small noise
    double area = cv::contourArea(contours[i]);
    if (area > 500) 
    { 
      // Draw the contour (Cyan color: B=255, G=255, R=0)
      cv::drawContours(processed_frame, contours, (int)i, cv::Scalar(255, 255, 0), 2, cv::LINE_AA, hierarchy, 0);
    }
  }
}

// Implementation of the image stylization filter: quantize_and_outline.
// It detects black outlines via adaptive thresholding and overlays them 
// directly onto the original image, preserving the original color palette.
// This version is significantly faster as it removes the Bilateral Filter.
//
// Arguments:
// - processed_frame: Input/Output image (cv::Mat, modified in place)
cv::Mat CAMERA::overlay_lines(cv::Mat& processed_frame)
{
    // --- Fixed Parameters ---
    const int EDGE_BLUR_KSIZE = 7;    // Median Blur: Noise reduction and simplification for edges
    const int EDGE_BLOCK_SIZE = 5;    // Adaptive Threshold: Local neighborhood size
    const int EDGE_C = 2;             // Adaptive Threshold: Line thickness control (higher = thicker)

    // --- Step 1: Find Edges (Adaptive Thresholding) ---
    // 1a. Convert to grayscale and apply median blur for clean input edges
    cv::Mat gray, blurred;
    cv::cvtColor(processed_frame, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, blurred, EDGE_BLUR_KSIZE); 

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
    
    // Removed: cv::bitwise_not(edges, edges); - This inversion is no longer needed.
    
    // --- Step 2: Overlay Black Lines onto the Original Image ---
    
    // Create a 1-channel mask where edge pixels (which are 0 in 'edges') are 255.
    cv::Mat line_mask;
    // line_mask is 255 where edges == 0 (the black line pixels)
    cv::compare(edges, 0, line_mask, cv::CMP_EQ); 

    // Use the mask to set the line pixels (where line_mask is 255) in 'processed_frame' to pure black (0, 0, 0).
    //processed_frame.setTo(cv::Scalar(0, 0, 0), line_mask);

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
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_raw.jpg", FRAME_BUFFER_0);
      }
    }
    else
    if (LATEST_READY_FRAME == 1)
    {
      if (!FRAME_BUFFER_1.empty())
      {
        cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_raw.jpg", FRAME_BUFFER_1);
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
      cv::imwrite(PROPS.CAMERA_DIRECTORY + file_format_system_time() + "_prc.jpg", PROCESSED_FRAME);
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

    // Previous functions are fast.  
    // Generate Downsized Frame for difficult proccessing
    {
      cv::resize(PROCESSED_FRAME, PROCESSED_FRAME_DOWNSIZED, cv::Size(), 1.0 / DOWN_SCALE_FACTOR, 1.0 / DOWN_SCALE_FACTOR, cv::INTER_LINEAR);
    }

  }
}

// Be careful with this function. It is ran in its own thread.
void CAMERA::apply_ehancements()
{
  if (!PROCESSED_FRAME.empty())
  {
    if (PROPS.ENH_LOW_LIGHT)
    {
      if (is_low_light(PROCESSED_FRAME, 50))
      {
        // --- Step 2A: Low-Light Contrast Enhancement (CLAHE) ---
        // Grayscale conversion is required to isolate the luminance channel for CLAHE.
        cv::Mat gray_image;
        cv::cvtColor(PROCESSED_FRAME, gray_image, cv::COLOR_BGR2GRAY);

        // CLAHE settings: Clip Limit 2.0, Tile Size 8x8
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(gray_image, gray_image); 

        // Apply enhanced luminance back to the color frame (making it grayscale enhanced)
        cv::cvtColor(gray_image, PROCESSED_FRAME, cv::COLOR_GRAY2BGR);
      }
    }

    // Overlay lines
    if (PROPS.ENH_OVERLAY_LINES)
    {
      MASK_FRAME_OVERLAY_LINES = overlay_lines(PROCESSED_FRAME_DOWNSIZED);
    }

    //---

    // Glare Mask
    if (PROPS.ENH_GLARE_MASK)
    {
      MASK_FRAME_GLARE = suppress_glare_mask(PROCESSED_FRAME_DOWNSIZED);
    }

    //---

    // Contour and Shape Detection (General Curves)
    if (PROPS.ENH_CURVE_FIT)
    {
      // This block runs general contour detection to find outlines of objects (cars, signs, potholes).
      detect_and_draw_contours(PROCESSED_FRAME);
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
      if (CAM_AVAILABLE)
      {
        if (FRAME_TO_BUFFER == 0)
        {
          CAMERA_CAPTURE >> FRAME_BUFFER_0;
          LATEST_READY_FRAME = 0;
        }
        else if (FRAME_TO_BUFFER == 1)
        {
          CAMERA_CAPTURE >> FRAME_BUFFER_1;
          LATEST_READY_FRAME = 1;
        }
      }
      else
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
}

void CAMERA::process_enhancements_frame()
{  
  // Process captured image.
  if (CAM_BEING_VIEWED)
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
  }
  
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
      if (CAM_BEING_VIEWED)
      {
        // Convert the frame to an OpenGL texture.
        // We pass the member variable to reuse the same texture.
        TEXTURE_ID = matToTexture(PROCESSED_FRAME, TEXTURE_ID);

        // Create Thread safe mat frame to access
        PROCESSED_FRAME.copyTo(LIVE_FRAME);
      }
      CAM_VIDEO_AVAILABLE = true;
    }
    else
    {
      CAM_VIDEO_AVAILABLE = false;
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
void CAMERA::list_controls()
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

  // The file descriptor is automatically closed by FdCloser destructor here.
}

void CAMERA::create()
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
    }
    else
    {
      // If camera not found DUMMY images to FRAME_DUMMY and FRAME_DUMMY2
      // If multi frame test, gen image to FRAME_DUMMY2.
      FRAME_DUMMY = cv::imread(PROPS.CAMERA_TEST_FILE_NAME, cv::IMREAD_COLOR); // Load test image as color
      if (FRAME_DUMMY.empty())
      {
        FRAME_DUMMY = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
      }

      if (FRAME_DUMMY_MULTI_FRAME_TEST)
      {
        FRAME_DUMMY2 = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
      }
      else
      {
        FRAME_DUMMY.copyTo(FRAME_DUMMY2);
      }

      print_stream << "Could not open camera. Please check your camera connection." << std::endl;
      CAM_AVAILABLE = false;
    }

    // Signal thread that it should be safe to start.
    CAMERA_READ_THREAD_STOP = false;
  }

  INFORMATION = print_stream.str();
}

void CAMERA::process(unsigned long Frame_Time)
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

      // Start the camera update on a separate thread.
      // This call is non-blocking, so the main loop can continue immediately.
      THREAD_IMAGE_PROCESSING.start_render_thread([&]() 
                {  process_enhancements_frame();  });
    }
  }
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

void CAMERA::close_camera()
{
  CAMERA_READ_THREAD_STOP = true;
  CAMERA_CAPTURE.release();
  CAM_AVAILABLE = false;
  CAM_VIDEO_AVAILABLE = false;
}

// ---------------------------------------------------------------------------------------

#endif
