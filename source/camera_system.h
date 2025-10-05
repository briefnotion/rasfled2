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
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp> 

#include <GLFW/glfw3.h>

// Rasfled Includes

using namespace std;

// ---------------------------------------------------------------------------------------

// --- Helper class for RAII (Resource Acquisition Is Initialization) ---
// This ensures the file descriptor is closed automatically when the function exits.
class FdCloser 
{
  public:
  int fd;
  // Constructor initializes with the file descriptor
  FdCloser(int descriptor) : fd(descriptor) {}
  // Destructor automatically calls close()
  ~FdCloser() 
  {
    if (fd != -1) 
    {
        close(fd);
    }
  }
  // Prevent copying (since file descriptors should not be copied)
  FdCloser(const FdCloser&) = delete;
  FdCloser& operator=(const FdCloser&) = delete;
};

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

  std::string CAMERA_DEVICE_NAME = "";
  int    CAMERA_DEVICE_ID   = -1;

  int WIDTH = 640;
  int HEIGHT = 480;
  //int AUTO_FOCUS = 0;  // 0 off
  //double FOCUS = 100.0;      // 0 - 255

  bool FLIP_HORIZONTAL = false; // (horizontal flip, around Y-axis)
  bool FLIP_VERTICAL = false;   // (vertical flip, around X-axis)

  // Controls:

  CAMERA_SETTING CTRL_FOCUS_AUTO;
  CAMERA_SETTING CTRL_FOCUS_ABSOLUTE;

  CAMERA_SETTING CTRL_EXPOSURE_AUTO;
  CAMERA_SETTING CTRL_EXPOSURE_TIME_ABSOLUTE;
  
  CAMERA_SETTING CTRL_BACKLIGHT_COMPENSATION;

  CAMERA_SETTING CTRL_WHITE_BALANCE_AUTOMATIC;
  CAMERA_SETTING CTRL_WHITE_BALANCE_TEMP;

  CAMERA_SETTING CTRL_GAIN;
  CAMERA_SETTING CTRL_GAMA;
  CAMERA_SETTING CTRL_HUE;
  CAMERA_SETTING CTRL_SHARPNESS;

  // Camera Enhancements

  // Median blur is excellent for removing "salt-and-pepper" noise.
  bool ENH_MEDIAN_BLUR    = true;
  bool ENH_LOW_LIGHT      = true;
  bool ENH_SHARPEN        = true;
  bool ENH_LINE_DETECTION = true;
  bool ENH_CURVE_FIT      = false;
  bool ENH_ROAD_MASK      = false;
  bool ENH_CAR_DETECTION  = false;

};

class CAMERA
{
  private:
  cv::Mat FRAME;
  cv::Mat FRAME_DUMMY;
  cv::Mat PROCESSED_FRAME;

  bool NEW_FRAME_AVAILABLE = false;

  // Object detection member
  bool CAR_CASCADE_LOADED = false;
  cv::CascadeClassifier CAR_CASCADE;

  // Camera CV Helper
  bool is_low_light(const cv::Mat& frame, int threshold);
  cv::Mat get_road_mask(const cv::Mat& frame);
  void detect_and_draw_contours(cv::Mat& processed_frame);

  // Change Settings
  bool set_control(uint32_t id, int32_t value); // returns true on success
  int get_control(uint32_t id);                 // returns -1 on failure, otherwise returns value.

  cv::Mat generateDummyFrame(int width, int height);
  GLuint matToTexture(const cv::Mat& frame, GLuint textureID);

  void prepare();
  // Until a load from json is created, manually keyt this routine up.

  void init(stringstream &Print_Stream);
  // Until camera properties are in a vector, manually set and get each control
  //  as necessary for first run.

  public:
  bool CAM_BEING_VIEWED = false;
  // Temporary fix to turn off high level processing if 
  //  camera display is not shown.  Manually set until 
  //  later.

  cv::VideoCapture CAMERA_CAPTURE;
  GLuint TEXTURE_ID = 0;
  
  bool CAM_AVAILABLE = false;
  bool CAM_VIDEO_AVAILABLE = false;

  std::string INFORMATION              = "Not Available";
  std::string INFORMATION_COMMAND_LIST = "Not Available";

  CAMERA_PROPERTIES PROPS;
  
  bool set_camera_control(CAMERA_SETTING &Setting, int Value);
  int get_camera_control_value(CAMERA_SETTING &Setting);
  
  void list_controls();

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
