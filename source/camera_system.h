// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: camera_system.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef CAMARA_SYSTEM_H
#define CAMARA_SYSTEM_H

//#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>

// Rasfled Includes

using namespace std;

// ---------------------------------------------------------------------------------------

class CAMERA_SETTING
{
  public:
  uint32_t  ADDRESS = 0x0;
  int       MINIMUM = 0;
  int       MAXIMUM = 0;
  int       DEFAULT = 0;
  int     SET_VALUE = -1;
};

class CAMERA_PROPERTIES
{
  public:

  string CAMERA_DEVICE_NAME = "";
  int    CAMERA_DEVICE_ID   = -1;

  int WIDTH = 640;
  int HEIGHT = 480;
  //int AUTO_FOCUS = 0;  // 0 off
  //double FOCUS = 100.0;      // 0 - 255

  bool FLIP_HORIZONTAL = false;

  // Controls:

  CAMERA_SETTING CTRL_FOCUS_AUTO;
  CAMERA_SETTING CTRL_FOCUS_ABSOLUTE;

  CAMERA_SETTING CTRL_EXPOSURE_AUTO;
  CAMERA_SETTING CTRL_EXPOSURE_TIME_ABSOLUTE;
  
  CAMERA_SETTING CTRL_BACKLIGHT_COMENSATION;
  CAMERA_SETTING CTRL_SHARPNESS;

  CAMERA_SETTING CTRL_WHITE_BALANCE_AUTOMATIC;
  CAMERA_SETTING CTRL_WHITE_BALANCE_TEMP;

  CAMERA_SETTING CTRL_GAIN;
  CAMERA_SETTING CTRL_GAMA;
  CAMERA_SETTING CTRL_HUE;
};

class CAMERA
{
private:
  cv::Mat FRAME;
  cv::Mat FRAME_DUMMY;
  cv::Mat PROCESSED_FRAME;

  bool NEW_FRAME_AVAILABLE = false;
  
  bool set_control(uint32_t id, int32_t value); // returns true on success
  int get_control(uint32_t id);                 // returns -1 on failure, otherwise returns value.

  GLuint matToTexture(const cv::Mat& frame, GLuint textureID);
  cv::Mat generateDummyFrame(int width, int height);

public:
  cv::VideoCapture CAMERA_CAPTURE;
  GLuint TEXTURE_ID = 0;
  
  bool CAM_AVAILABLE = false;
  bool CAM_VIDEO_AVAILABLE = false;

  string INFORMATION              = "Not Available";
  string INFORMATION_COMMAND_LIST = "Not Available";

  CAMERA_PROPERTIES PROPS;
  
  void list_controls();

  bool set_camera_control(CAMERA_SETTING &Setting, int Value);
  int get_camera_control_value(CAMERA_SETTING &Setting);

  // Public method to create the camera capture.
  void create();

  // Public method to update the frame and texture in a thread-safe manner.
  void update_frame();

  void process_frame();

  // Public method to get a thread-safe copy of the current frame.
  cv::Mat get_current_frame();

  void close_camera();
};


// ---------------------------------------------------------------------------------------

#endif
