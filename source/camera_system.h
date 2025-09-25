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
};

class CAMERA
{
private:

  GLuint matToTexture(const cv::Mat& frame, GLuint textureID);
  cv::Mat FRAME;
  cv::Mat PROCESSED_FRAME;

  bool NEW_FRAME_AVAILABLE = false;

public:
  cv::VideoCapture CAMERA_CAPTURE;
  GLuint TEXTURE_ID = 0;
  bool CAMERA_AVAILABLE = false;
  bool VIDEO_AVAILABLE = false;

  CAMERA_PROPERTIES PROPS;
  
  // Public method to create the camera capture.
  void create();

  // Public method to update the frame and texture in a thread-safe manner.
  void update_frame();

  void process_frame();

  // Public method to get a thread-safe copy of the current frame.
  cv::Mat get_current_frame();

  void close();
};


// ---------------------------------------------------------------------------------------

#endif
