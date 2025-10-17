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
#include <vector>

#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp> 


// Rasfled Includes
#include "helper.h"
#include "fled_time.h"
#include "threading.h"
#include "screen4_helper.h"
#include "json_interface.h"

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

  std::string CAMERA_DIRECTORY = "";     // e.g. .../rasfled-t/camera/
  std::string CAMERA_SETTINGS_DIR = "";  // e.g. .../rasfled-t/camera/settings/
  std::string CAMERA_TEST_FILE_NAME = "";  // e.g. .../rasfled-t/camera/settings/
  std::string CAMERA_SETTINGS_FILE_NAME = "";  // e.g. .../rasfled-t/camera/settings/

  int WIDTH = 640;
  int HEIGHT = 480;

  bool FLIP_HORIZONTAL = false; // (horizontal flip, around Y-axis)
  bool FLIP_VERTICAL = false;   // (vertical flip, around X-axis)

  int FORCED_FRAME_LIMIT_MS  = 30;

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
  
  //bool ENH_SHARPEN        = true;

  //bool ENH_MEDIAN_BLUR    = true;
  bool ENH_LOW_LIGHT      = true;

  bool ENH_OVERLAY_LINES  = true;

  bool ENH_GLARE_MASK     = true;

  //bool ENH_LINE_DETECTION = false;
  bool ENH_COLOR          = false;
  bool ENH_HOUGH          = false;
  bool ENH_CURVE_IMPROVED = false;
  bool ENH_CURVE_FIT      = false;

  //bool ENH_ROAD_MASK      = false;  // Doesnt work

  bool ENH_CAR_DETECTION  = false;

  // ---------------------------------------------------------------------------------------
  
  // Adjustments for the backup camera
    // Level 0: Closest to the car (Bottom of Green)
  bool SHOW_PATH = false;
  
  // Angle Muliplier
  float ANGLE_MULTIPLIER = 2.5f;

  // Level 0: Closest Green boundary
  float Y0 = 1.000f;
  float XL0 = -0.100f;
  float XR0 = 1.160f;
  
  // Level 1: Green/Yellow boundary
  float Y1 = 0.800f;
  float XL1 = 0.00f;
  float XR1 = 1.020f;
  
  // Level 2: Yellow/Red boundary
  float Y2 = 0.330f;
  float XL2 = 0.227f;
  float XR2 = 0.700f;

  // Level 3: Farthest point (Top of Red)
  float Y3 = 0.200f;
  float XL3 = 0.295f;
  float XR3 = 0.610f;

};

class CAMERA
{
  private:
  cv::Mat FRAME_DUMMY;
  cv::Mat FRAME_DUMMY2; // for testing double buffer
  bool    FRAME_DUMMY_MULTI_FRAME_TEST = false;

  cv::Mat LIVE_FRAME;

  bool NEW_FRAME_AVAILABLE = false; // Delete?

  double DOWN_SCALE_FACTOR = 2.0;

  // Object detection member
  bool CAR_CASCADE_LOADED = false;
  cv::CascadeClassifier CAR_CASCADE;

  MEASURE_TIME_START_END TIME_SE_MAX_FPS;
  MEASURE_TIME_START_END TIME_SE_FRAME_RETRIEVAL;
  MEASURE_TIME_START_END TIME_SE_FRAME_PROCESSING;

  bool SAVE_IMAGE_BUFFER_FRAME = false;
  bool SAVE_IMAGE_PROCESSED_FRAME = false;

  // ---------------------------------------------------------------------------------------
  
  // Camera CV Helper
  bool is_low_light(const cv::Mat& frame, int threshold);
  cv::Mat get_road_mask(const cv::Mat& frame);

  void detect_and_draw_contours_basic_fixed(cv::Mat& processed_frame);
  void detect_hough_circles(cv::Mat& processed_frame);
  void detect_and_draw_contours_improved(cv::Mat& processed_frame);

  void detect_and_draw_contours(cv::Mat& processed_frame);
  cv::Mat overlay_lines(cv::Mat& processed_frame);
  cv::Mat suppress_glare_mask(cv::Mat& processed_frame);

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

  // ---------------------------------------------------------------------------------------
  //Multithreaded routines and vaiables. Access with caution.
  
  // All Threads
  vector<std::string>    PRINTW_QUEUE;

  // Thread Update
  
  FLED_TIME         CAMERA_READ_THREAD_TIME;              // Thread gets its own Time 
                                                          // Variable.
  bool              CAMERA_READ_THREAD_STOP     = true;  // Keep reading camera in thread 
                                                          // until told to stop
  cv::VideoCapture  CAMERA_CAPTURE;
  int               FRAME_TO_BUFFER = 0;

  bool CAM_AVAILABLE = false;
  bool CAM_VIDEO_AVAILABLE = false;

  // Thread Update and process_enhancements_frame
  cv::Mat FRAME_BUFFER_0;
  cv::Mat FRAME_BUFFER_1;
  bool    BUFFER_FRAME_HANDOFF_READY = false;  // Needs Lock
  int     LATEST_READY_FRAME         = -1;
  int     BEING_PROCESSED_FRAME      = -1;
  
  // Thread process_enhancements_frame
  cv::Mat PROCESSED_FRAME_DOWNSIZED;
  cv::Mat MASK_FRAME_OVERLAY_LINES;
  cv::Mat MASK_FRAME_GLARE;

  // Thread process_enhancements_frame and generate_imgui_texture_frame
  cv::Mat PROCESSED_FRAME;
  bool    WORKING_FRAME_FULLY_PROCESSED = true; // Needs Lock

  // Load and Save settings
  void save_settings();
  void load_settings();

  void check_for_save_image_buffer_frame();
  // check to see if current buffer should be saved to disk.

  void check_for_save_image_buffer_processed();
  // check to see if current buffer should be saved to disk.

  void run_preprocessing(cv::Mat &Frame);
  // Apply orientation (flip logic)
  // Apply Denoising
  // Apply Sharpening
  // Create Downsized image

  void apply_ehancements();
  // Apply all prop enable enhancements.
  // PROCESSED_FRAME created upon completion.

  // Public method to create the camera capture.
  void open_camera();
  // Step through the process of starting the camera.

  void update_frame();
  // Pull in latest frame from camera in a non thread-safe manner.

  void process_enhancements_frame();
  // Process all enhancements to working frame in a non thread-safe manner.
  
  // ---------------------------------------------------------------------------------------

  void generate_imgui_texture_frame();
  // Converts PROCESSED_FRAME into ImGui Texture to be rendered
  //  into program display.
  // Copies PROCESSED_FRAME to LIVE_FRAME for thread safe access.

  public:

  bool CAM_BEING_VIEWED = false;
  // Temporary fix to turn off high level processing if 
  //  camera display is not shown.  Manually set until 
  //  later.

  GLuint TEXTURE_ID = 0;
  
  std::string INFORMATION_COMMAND_LIST = "Not Available";

  double TIME_MAX_FPS;
  double TIME_FRAME_RETRIEVAL;
  double TIME_FRAME_PROCESSING;

  TIMED_IS_READY  FORCED_FRAME_LIMIT;
  
  THREADING_INFO  THREAD_CAMERA;
  THREADING_INFO  THREAD_IMAGE_PROCESSING;

  CAMERA_PROPERTIES PROPS;
  
  bool set_camera_control(CAMERA_SETTING &Setting, int Value);
  int get_camera_control_value(CAMERA_SETTING &Setting);
  
  void list_controls(CONSOLE_COMMUNICATION &cons);

  // Manually print output stream
  void print_stream(CONSOLE_COMMUNICATION &cons);

  void process(CONSOLE_COMMUNICATION &cons, unsigned long Frame_Time);

  // Public method to get a thread-safe copy of the current frame.
  cv::Mat get_current_frame();

  void take_snapshot();

  // Public method to start and stop the camera capture by 
  //  truning off and on the thread.
  void camera_start();
  void camera_stop();

  // Report if camera is opened.
  bool camera_avalable();
  bool video_avalable();
};


// ---------------------------------------------------------------------------------------

#endif
