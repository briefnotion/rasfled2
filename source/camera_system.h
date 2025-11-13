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
#include <opencv2/video.hpp>
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

class FAKE_FRAME
{
public:
  // Constructor is now default. Dimensions are set by the first frame in interpolateFrame.
  FAKE_FRAME();

  // Generates the interpolated frame between the previously stored frame and current_frame
  cv::Mat interpolateFrame(const cv::Mat& current_frame);

private:
  // Called by interpolateFrame with the very first input frame. 
  // This method now performs configuration, buffer allocation, and initial state setup.
  void preprocess_initial_frame(const cv::Mat& initial_frame);
  
  // --- Configuration Variables (No longer const, initialized when the first frame arrives) ---
  int PROCESS_WIDTH = 0;
  int PROCESS_HEIGHT = 0;
  int FLOW_CALC_WIDTH = 0;
  int FLOW_CALC_HEIGHT = 0;
  
  // Flag to ensure configuration and buffer allocation only happens once
  bool is_initialized_ = false;

  // --- State Variables ---
  // Stores the previous BGR frame used for interpolation. This is the frame we will remap.
  cv::Mat prev_frame_;

  // --- Intermediate Reusable Buffers (Pre-allocated in preprocess_initial_frame) ---

  // Gray frames for optical flow calculation (PROCESS_SIZE)
  cv::Mat current_gray_, prev_gray_;

  // Downsampled gray frames (FLOW_CALC_SIZE)
  cv::Mat small_prev_gray_, small_current_gray_;

  // Flow field at the small resolution (FLOW_CALC_SIZE, 2-channel float)
  cv::Mat flow_small_;

  // Flow field upsampled back to the processing size (PROCESS_SIZE, 2-channel float)
  cv::Mat flow_upsampled_;

  // Pre-calculated mesh grids for vectorized map generation (PROCESS_SIZE, 1-channel float)
  cv::Mat coords_x_, coords_y_; 

  // Remapping matrices (PROCESS_SIZE, 1-channel float)
  cv::Mat map_x_, map_y_;
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
  
  std::string CAMERA_NAME = "";
  std::string CAMERA_DEVICE_NAME = "";
  int    CAMERA_DEVICE_ID   = -1;

  std::string CAMERA_DIRECTORY = "";     // e.g. .../rasfled-t/camera/
  std::string CAMERA_SETTINGS_DIR = "";  // e.g. .../rasfled-t/camera/settings/
  std::string CAMERA_TEST_FILE_NAME = "";  // e.g. .../rasfled-t/camera/settings/

  int WIDTH = 640;
  int HEIGHT = 480;

  bool FLIP_HORIZONTAL = false; // (horizontal flip, around Y-axis)
  bool FLIP_VERTICAL = false;   // (vertical flip, around X-axis)

  int FORCED_FRAME_LIMIT_MS  = 30;

  bool TEST             = false;
  bool TEST_IMAGE       = false;
  bool TEST_MULTI_FRAME = false;

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

  // ---
  // Camera Enhancements

  // Tested and working well enhancements

  // When low light triggered, turns on grayscale image 
  //  and booste brightness
  bool ENH_LOW_LIGHT      = false;

  // Finds contrast image lines and draws mask.
  //bool ENH_OVERLAY_LINES  = false;

  // Finds max brightness area and mask them out.
  bool ENH_GLARE_MASK     = true;

  // Boost all colors to min max.  Very noisy at 
  //  no light.
  bool ENH_COLOR          = true;

  // Finds contrast image lines and draws mask.
  // Works better than ENH_OVERLAY_LINES
  bool ENH_CANNY_MASK     = true;

  // Frame Generation, aka Frame Interpolation, aka Fake Frames
  bool ENH_FAKE_FRAMES    = true;

  // Experimental Enhancements

  // Repeats the previous frames camera mask.
  bool ENH_DOUBLE_MASK    = true;

  // May or not work. Never had a full car on screen to 
  //  see.
  bool ENH_CAR_DETECTION  = false;

  // Draws a circle around a curved object. Use case not found.
  bool ENH_HOUGH          = false;

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

  bool NEW_FRAME_AVAILABLE = false; // Delete?

  double DOWN_SCALE_FACTOR = 2.0;

  // Object detection member
  bool CAR_CASCADE_LOADED = false;
  cv::CascadeClassifier CAR_CASCADE;

  MEASURE_TIME_START_END TIME_SE_CAMERA_FPS;
  MEASURE_TIME_START_END TIME_SE_FRAME_RETRIEVAL;
  MEASURE_TIME_START_END TIME_SE_FRAME_PROCESSING;
  MEASURE_TIME_START_END TIME_SE_DISPLAYED_FRAME_RATE;

  bool INTERPOLATION_DISPLAY = true;

  bool SAVE_IMAGE_BUFFER_FRAME = false;
  bool SAVE_IMAGE_PROCESSED_FRAME = false;

  //bool CAM_BEING_VIEWED = false;
  // Turn off high level processing if 
  //  camera display is not shown.  Manually set until 
  //  later.

  // ---------------------------------------------------------------------------------------
    
  // Class wide enhancement settings
  const int BLUR_KSIZE = 7;         // Gaussian blur kernel size for noise reduction  (5)
  const int CANNY_THRESH_LOW = 10;  // Lower threshold for edge detection (50)
  const int CANNY_THRESH_HIGH = 100;// Upper threshold for edge detection (150)
  const int CANNY_APERTURE = 3;     // Aperture size for Sobel operator   (3) 3 5 or 7

  // Camera CV Helper
  bool is_low_light(const cv::Mat& Grey_Image_Full_Size, int threshold);
  void gray_enhance(cv::Mat& processed_frame, const cv::Mat& Grey_Image_Full_Size);
  void low_light_filter(cv::Mat& processed_frame);
  void apply_min_max_contrast(cv::Mat& processed_frame);
  void detect_hough_circles(cv::Mat& processed_frame, const cv::Mat& Processed_Frame_Gaussian);
  cv::Mat canny_mask(cv::Mat& Processed_Frame_Canny);
  cv::Mat overlay_lines(cv::Mat& Processed_Frame_Gray_Downsized);
  cv::Mat suppress_glare_mask(cv::Mat& processed_frame);

  // Change Settings
  bool set_control(uint32_t id, int32_t value); // returns true on success
  int get_control(uint32_t id);                 // returns -1 on failure, otherwise returns value.

  cv::Mat generate_empty_frame(int width, int height);
  cv::Mat generateDummyFrame(int width, int height);
  cv::Mat generateDummyFrame_2(int width, int height, int frame_index);
  cv::Mat generateDummyLowLightFrame(int width, int height, int frame_index);
  GLuint matToTexture(const cv::Mat& frame, GLuint textureID);

  void prepare();
  // Until a load from json is created, manually keyt this routine up.

  void init(stringstream &Print_Stream);
  // Until camera properties are in a vector, manually set and get each control
  //  as necessary for first run.

  // ---------------------------------------------------------------------------------------
  //Multithreaded routines and vaiables. Access with caution.

  /*

  process

  update_frame
    // Trigger buffer 0 or 1 ready
    BUFFER_FRAME_HANDOFF_READY

  if (BUFFER_FRAME_HANDOFF_READY && WORKING_FRAME_FULLY_PROCESSED)
  process_enhancements_frame
    // clear trigger
    BUFFER_FRAME_HANDOFF_READY = false
    // Lock enhancements from starting
    WORKING_FRAME_FULLY_PROCESSED = false

      Completes with
      FRAME_TO_TEXTURE_TRACK = 1
  

  if (FRAME_TO_TEXTURE_TRACK > 0)
  generate_imgui_texture_frame
    // Relese process_enhancements_frame
    WORKING_FRAME_FULLY_PROCESSED = true

        FRAME_TO_TEXTURE_TRACK = 0 || 2
        FRAME_TO_TEXTURE_TRACK = 0

  */
  
  // All Threads
  vector<std::string>    PRINTW_QUEUE;

  // Thread Update
  
  FLED_TIME         CAMERA_READ_THREAD_TIME;            // Thread gets its own Time 
                                                        // Variable.
  bool              CAMERA_READ_THREAD_STOP   = true;   // Keep reading camera in thread 
                                                        // until told to stop
  bool              CAMERA_ONLINE             = false;  // Reports to outside camera running. 

  cv::VideoCapture  CAMERA_CAPTURE;
  int               FRAME_TO_BUFFER = 0;

  bool CAM_AVAILABLE = false;

  // Thread Update and process_enhancements_frame
  cv::Mat FRAME_BUFFER_0;
  cv::Mat FRAME_BUFFER_1;
  cv::Mat FRAME_BUFFER_FAKE;

  bool IS_LOW_LIGHT = false;

  // Thread process_enhancements_frames
  cv::Mat PROCESSED_FRAME;
  cv::Mat PROCESSED_FRAME_GRAY;
  //cv::Mat PROCESSED_FRAME_DOWNSIZED;
  //cv::Mat PROCESSED_FRAME_GRAY_DOWNSIZED;
  //cv::Mat PROCESSED_FRAME_GAUSSIAN;
  cv::Mat PROCESSED_FRAME_CANNY;

  //cv::Mat MASK_FRAME_OVERLAY_LINES;

  // Mask frames and doubles for rotation
  cv::Mat MASK_FRAME_GLARE_0;
  cv::Mat MASK_FRAME_GLARE_1;

  cv::Mat MASK_FRAME_CANNY_0;
  cv::Mat MASK_FRAME_CANNY_1;

  int DOUBLE_MASK_LATEST = 0;

  // Display Frames
  cv::Mat LIVE_FRAME;   // Frame tied to Texture for display
  cv::Mat LIVE_FRAME_0; // Live frame buffer to LIVE_FRAME.
  cv::Mat LIVE_FRAME_1; // Live frame buffer to LIVE_FRAME.

  FAKE_FRAME FAKE_FRAME_GENERATOR;
  
  int             FRAME_TO_TEXTURE_TRACK = 0;
  TIMED_IS_READY  FRAME_TO_TEXTURE_TIMER;

  bool    BUFFER_FRAME_HANDOFF_READY = false;  // Needs Lock
  int     LATEST_READY_FRAME         = -1;
  int     BEING_PROCESSED_FRAME      = -1;

  // Thread process_enhancements_frame and generate_imgui_texture_frame
  bool    WORKING_FRAME_FULLY_PROCESSED = true; // Needs Lock

  bool    CAMERA_BEING_VIEWED       = false;
  
  // Load and Save settings
  void save_settings();
  void load_settings_json();

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

  void apply_masks_to_processed_frame(cv::Mat &Mask_0, cv::Mat &Mask_1);
  // A simple helper routine to apply double or single frame mask.

  void apply_all_masks();
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

  void generate_imgui_texture_frame(cv::Mat& Frame);
  // Converts PROCESSED_FRAME into ImGui Texture to be rendered
  //  into program display.
  // Copies PROCESSED_FRAME to LIVE_FRAME for thread safe access.

  public:

  GLuint TEXTURE_ID = 0;
  
  std::string INFORMATION_COMMAND_LIST = "Not Available";

  double TIME_CAMERA_FPS;
  double TIME_CAMERA_FRAME_TIME;
  double TIME_FRAME_RETRIEVAL;
  double TIME_FRAME_PROCESSING;
  double TIME_ACTUAL_FPS;
  double TIME_ACTUAL_FRAME_TIME;

  bool FRAME_GEN = false;

  TIMED_IS_READY  FORCED_FRAME_LIMIT;
  
  THREADING_INFO  THREAD_CAMERA;
  THREADING_INFO  THREAD_IMAGE_PROCESSING;

  CAMERA_PROPERTIES PROPS;
  
  bool set_camera_control(CAMERA_SETTING &Setting, int Value);
  int get_camera_control_value(CAMERA_SETTING &Setting);
  
  void list_controls(CONSOLE_COMMUNICATION &cons);

  // Manually print output stream
  void print_stream(CONSOLE_COMMUNICATION &cons);

  void process(CONSOLE_COMMUNICATION &cons, unsigned long Frame_Time, bool Camera_Being_Viewed);

  // Public method to get a thread-safe copy of the current frame.
  cv::Mat get_current_frame();

  void take_snapshot();

  // Load all camera settings
  void load_settings();

  // Public method to start and stop the camera capture by 
  //  truning off and on the thread.
  void camera_start();
  void camera_stop();

  // Report if camera is opened.
  bool camera_online();
};


// ---------------------------------------------------------------------------------------

#endif
