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
      FRAME_DUMMY = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
      print_stream << "Could not open camera. Please check your camera connection." << std::endl;
      CAM_AVAILABLE = false;
    }

  }


  INFORMATION = print_stream.str();
}

void CAMERA::update_frame()
{
  // Capture the camera frame.
  if (CAM_AVAILABLE)
  {
    CAMERA_CAPTURE >> FRAME;
  }
  else
  {
    FRAME = FRAME_DUMMY;
  }
  NEW_FRAME_AVAILABLE = true;
}

void CAMERA::process_frame()
{
  // Only proceed if the frame is not empty.
  if (NEW_FRAME_AVAILABLE)
  {
    NEW_FRAME_AVAILABLE = false;

    if (!FRAME.empty())
    {
      // Flip the processed frame.
      if (PROPS.FLIP_HORIZONTAL && PROPS.FLIP_VERTICAL)
      {
        cv::flip(FRAME, PROCESSED_FRAME, -1);
      }
      else if (PROPS.FLIP_HORIZONTAL)
      {
        cv::flip(FRAME, PROCESSED_FRAME, 1);
      }
      else if (PROPS.FLIP_VERTICAL)
      {
        cv::flip(FRAME, PROCESSED_FRAME, 0);
      }
      else
      {
        PROCESSED_FRAME = FRAME;
      }

      // Convert the frame to an OpenGL texture.
      // We pass the member variable to reuse the same texture.
      TEXTURE_ID = matToTexture(PROCESSED_FRAME, TEXTURE_ID);
      CAM_VIDEO_AVAILABLE = true;
    }
    else
    {
      CAM_VIDEO_AVAILABLE = false;
    }
  }
}

cv::Mat CAMERA::get_current_frame()
{
  // Return a copy of the FRAME to the caller.
  return PROCESSED_FRAME;
}

void CAMERA::close_camera()
{
  CAMERA_CAPTURE.release();
  CAM_AVAILABLE = false;
  CAM_VIDEO_AVAILABLE = false;
}

// ---------------------------------------------------------------------------------------

#endif
