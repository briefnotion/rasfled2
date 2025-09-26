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

cv::Mat CAMERA::generateDummyFrame(int width, int height) 
{
  cv::Mat dummy(height, width, CV_8UC3);
  for (int y = 0; y < dummy.rows; ++y)
  {
    for (int x = 0; x < dummy.cols; ++x)
    {
      dummy.at<cv::Vec3b>(y, x) = cv::Vec3b(x % 256, y % 256, (x + y) % 256);
    }
  }
  return dummy;
}

int CAMERA::set_control(int fd, uint32_t id, int32_t value)
{
  struct v4l2_control control;
  std::memset(&control, 0, sizeof(control));
  control.id = id;
  control.value = value;
  if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1)
  {
    perror("Setting control failed");
    return -1;
  }
  return 0;
}

int CAMERA::get_control(int fd, uint32_t id) 
{
  struct v4l2_control control;
  std::memset(&control, 0, sizeof(control));
  control.id = id;
  if (ioctl(fd, VIDIOC_G_CTRL, &control) == -1) 
  {
    perror("Getting control failed");
    return -1;
  }
  return control.value;
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

void CAMERA::list_controls(const char* device)
{
  INFORMATION_COMMAND_LIST = "";
  
  int fd = open(device, O_RDWR);
  if (fd == -1) 
  {
    //perror("Opening video device failed");
    INFORMATION_COMMAND_LIST = "Opening video device failed\n";
    return;
  }

  INFORMATION_COMMAND_LIST = "Command List:\n\n";

  struct v4l2_queryctrl queryctrl;
  std::memset(&queryctrl, 0, sizeof(queryctrl));

  // Start with the first control ID
  queryctrl.id = V4L2_CID_BASE;

  while (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) 
  {
    if (!(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)) 
    {
      std::stringstream ss;
      ss << "Control: " << queryctrl.name
        << "  ID: 0x" << std::hex << queryctrl.id
        << "  Type: " << std::dec << queryctrl.type
        << "\n";

      INFORMATION_COMMAND_LIST += ss.str();
    }
    queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
  }

  close(fd);
}

void CAMERA::create()
{
  INFORMATION = ""; 
  std::stringstream ss;
  
  // Attempt to open the default camera (index 0).
  CAMERA_CAPTURE.open(0);

  // Check if the camera was successfully opened.
  CAM_AVAILABLE = CAMERA_CAPTURE.isOpened();
  if (CAM_AVAILABLE)
  {
    // If the camera is opened, set its frame width and height.
    CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_WIDTH, PROPS.WIDTH);
    CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_HEIGHT, PROPS.HEIGHT);
    //CAMERA_CAPTURE.set(cv::CAP_PROP_AUTOFOCUS, PROPS.AUTO_FOCUS);
    //CAMERA_CAPTURE.set(cv::CAP_PROP_FOCUS, PROPS.FOCUS);

    // Hardcoding settings for my camera. May change in the future.
    {
      const char* device = "/dev/video0";

      // --- Step 1: open V4L2 device ---
      int fd = open(device, O_RDWR);
      if (fd == -1) 
      {
        //perror("Opening video device failed");
        ss << "Opening video device failed" << endl;
      }

      // --- Step 2: set focus controls ---
      // disable continuous autofocus
      set_control(fd, PROPS.CTRL_ADDR_FOCUS_AUTO, 0);   // V4L2_CID_FOCUS_AUTO_CONTINUOUS
      // set manual focus value (depends on your camera’s supported range)
      set_control(fd, PROPS.CTRL_ADDR_FOCUS_ABSOLUTE, 0);  // V4L2_CID_FOCUS_ABSOLUTE

      // Get settings
      CAM_FOCUS_AUTO = get_control(fd, PROPS.CTRL_ADDR_FOCUS_AUTO); // focus_absolute
      CAM_FOCUS_ABSOLUTE = get_control(fd, PROPS.CTRL_ADDR_FOCUS_ABSOLUTE); // focus_absolute

      ss << "Auto Focus: " << CAM_FOCUS_AUTO << endl;
      ss << "Absl Focus: " << CAM_FOCUS_ABSOLUTE << endl;

      // --- Step 3: close fd (controls persist in the driver) ---
      close(fd);
    }

    ss << "Camera successfully opened and properties set." << std::endl;

    // Now, probe the camera for its actual capabilities and print them.
    double actual_width = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_WIDTH);
    double actual_height = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_HEIGHT);
    double actual_fps = CAMERA_CAPTURE.get(cv::CAP_PROP_FPS);

    ss << "Actual camera resolution: " << actual_width << "x" << actual_height << std::endl;
    ss << "Actual camera FPS: " << actual_fps << std::endl;
  }
  else
  {
    FRAME_DUMMY = generateDummyFrame(PROPS.WIDTH, PROPS.HEIGHT);
    ss << "Could not open camera. Please check your camera connection." << std::endl;
    CAM_AVAILABLE = false;
  }

  INFORMATION += ss.str();
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
      PROCESSED_FRAME = FRAME;

      // Flip the processed frame horizontally for a mirror effect.
      if (PROPS.FLIP_HORIZONTAL)
      {
        cv::flip(PROCESSED_FRAME, PROCESSED_FRAME, 1);
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
}

// ---------------------------------------------------------------------------------------

#endif