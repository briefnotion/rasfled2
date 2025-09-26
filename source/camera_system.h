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

class CAMERA_PROPERTIES
{
  public:
  int WIDTH = 640;
  int HEIGHT = 480;
  //int AUTO_FOCUS = 0;  // 0 off
  //double FOCUS = 100.0;      // 0 - 255

  bool FLIP_HORIZONTAL = false;

  // Control Addresses:
  uint32_t CTRL_ADDR_FOCUS_AUTO     = 0x009a090c;
  uint32_t CTRL_ADDR_FOCUS_ABSOLUTE = 0x009a090a;
};

class CAMERA
{
private:
  cv::Mat FRAME;
  cv::Mat FRAME_DUMMY;
  cv::Mat PROCESSED_FRAME;

  bool NEW_FRAME_AVAILABLE = false;
  
  int set_control(int fd, uint32_t id, int32_t value);
  int get_control(int fd, uint32_t id);

  GLuint matToTexture(const cv::Mat& frame, GLuint textureID);
  cv::Mat generateDummyFrame(int width, int height);

public:
  cv::VideoCapture CAMERA_CAPTURE;
  GLuint TEXTURE_ID = 0;
  
  bool CAM_AVAILABLE = false;
  bool CAM_VIDEO_AVAILABLE = false;
  
  // Camera Settings
  int CAM_FOCUS_AUTO      = 0;
  int CAM_FOCUS_ABSOLUTE  = 0;

  string INFORMATION              = "Not Available";
  string INFORMATION_COMMAND_LIST = "Not Available";

  CAMERA_PROPERTIES PROPS;
  
  void list_controls(const char* device);

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
