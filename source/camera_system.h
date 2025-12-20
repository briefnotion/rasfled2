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
#include <thread>
#include <algorithm>  // for std::max

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
//  Helpers

void resize_if_not_same_size(cv::Mat& Main_Frame, cv::Mat& Resize_Frame);
void createMeshGrid(int rows, int cols, cv::Mat& x_coords, cv::Mat& y_coords);

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

// Forward declaration of the class (assuming necessary headers are included)
class FAKE_FRAME
{
  private:

  // Dimensions
  int PROCESS_WIDTH = 0;
  int PROCESS_HEIGHT = 0;
  int FLOW_CALC_WIDTH = 0;
  int FLOW_CALC_HEIGHT = 0;
  bool is_initialized_ = false;

  // Buffers for Grayscale Frames (Swapped)
  cv::Mat current_gray_;
  cv::Mat prev_gray_;

  // Buffers for Optical Flow
  cv::Mat small_prev_gray_;
  cv::Mat small_current_gray_;
  cv::Mat flow_small_;
  cv::Mat flow_upsampled_;

  // Buffers for Remap Maps
  cv::Mat map_x_;
  cv::Mat map_y_;

  // Pre-calculated Mesh Grid
  cv::Mat coords_x_;
  cv::Mat coords_y_;

  // Previous Color Frame
  cv::Mat prev_frame_;

  // --- Configuration Constants for Raspberry Pi 5 Performance ---
  const int FLOW_SIZE = 12; // Increased from 4/8 to 12 for 15ms target
  const int FLOW_ITERATIONS = 3; // Reduced from 10 to 3 for massive speedup
  const int FLOW_LEVELS = 1; // Reduced to 1 to minimize pyramid overhead

  // Helper function to create the coordinate mesh grid (unchanged from original intent)
  void createMeshGrid(int rows, int cols, cv::Mat& mesh_x, cv::Mat& mesh_y);

  public:

  // Constructor (unchanged)
  FAKE_FRAME() = default;

  // Initialization methods
  void re_int_vars(const cv::Mat& frame);
  void preprocess_initial_frame(const cv::Mat& initial_frame);

  // Optimized Core Function
  cv::Mat interpolateFrame(const cv::Mat& current_frame);
};

// ---------------------------------------------------------------------------------------
class CAMERA_CONTROL_SETTING_LOADED
{
  public:
  std::string  NAME = "";
  int         VALUE = 0;
};

class CAMERA_SETTING_MENU_ITEM
{
  public:
  std::string  NAME = "";
  int            ID = 0;
};

class CAMERA_SETTING
{
  public:
  bool      ENABLED = false;
  std::string  NAME = "";
  uint32_t  ADDRESS = 0x0;
  int      VAR_TYPE = 0;  // 1 - int, 2 - bool, 3 - menu, 6 - class, else unknown
  int       MINIMUM = 0;
  int       MAXIMUM = 0;
  int       DEFAULT = 0;
  int          STEP = 0;
  int         VALUE = -1;
  int     SET_VALUE = -1;
  vector<CAMERA_SETTING_MENU_ITEM> MENU_LIST;
};

class CAMERA_PROPERTIES
{
  public:
  
  std::string CAMERA_NAME = "";
  std::string CAMERA_DEVICE_NAME = "";

  std::string CAMERA_DIRECTORY = "";     // e.g. .../rasfled-t/camera/
  std::string CAMERA_SETTINGS_DIR = "";  // e.g. .../rasfled-t/camera/settings/
  std::string CAMERA_TEST_FILE_NAME = "";  // e.g. .../rasfled-t/camera/settings/

  int WIDTH = 640;
  int HEIGHT = 480;

  int COMPRESSION = 1;
  // 1 - YUYV
  // 0 - MJPG

  bool FULL_FRAME_STYLE = 1;
  // 0 - Full Frame Center
  // 1 - Full Horizontal, Center

  float POST_PROCESS_SCALE = 1.0f;

  bool FLIP_HORIZONTAL = false; // (horizontal flip, around Y-axis)
  bool FLIP_VERTICAL = false;   // (vertical flip, around X-axis)

  // Does not set frame rate of camera. Instead sets the minimum 
  //  speed of the while loop for the camera read, a delay.
  //  0 means that the next read will be attempte imediately, 
  //  30 means that the camera read will only be attempted 
  //  30 ms after the previous.
  
  bool TEST             = false;
  bool TEST_IMAGE       = false;
  bool TEST_MULTI_FRAME = false;

  // ---
  // Camera Enhancements

  // Tested and working well enhancements

  // When low light triggered, turns on grayscale image 
  //  and booste brightness
  bool    ENH_LOW_LIGHT      = false;
  int     ENH_LOW_LIGHT_THRESHOLD  = 15;
  double  ENH_LOW_LIGHT_GAMMA      = 0.5; // Gamma < 1.0 brightens shadows significantly

  // Finds contrast image lines and draws mask.
  //bool ENH_OVERLAY_LINES  = false;

  // Finds max brightness area and mask them out.
  bool ENH_GLARE_MASK     = true;

  // --- Glare Detection Parameter ---
  // Pixels where all B, G, and R channels are above this threshold will be masked.
  // 230 is a good starting point for bright white light (0-255 scale).
  int GLARE_THRESHOLD = 224;

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

// ---------------------------------------------------------------------------------------

class FRAME_SUITE_EXTRA
{
  public:

  int LATEST_POS = 0;

  // Mask frames and doubles for rotation
  cv::Mat MASK_FRAME_CANNY[2];
  cv::Mat MASK_FRAME_GLARE[2];

  void advance_latest();
};

class FRAME_SUITE
{
  public:

  // Thread Update and process_enhancements_frame
  cv::Mat FRAME_BUFFER;
  cv::Mat FRAME_BUFFER_RESIZE;  // tmp
  cv::Mat PROCESSED_FRAME;

  // Single line post process
  cv::Mat FRAME_BUFFER_FAKE;

  // Thread process_enhancements_frames

  cv::Mat PROCESSED_FRAME_GRAY;
  cv::Mat PROCESSED_FRAME_CANNY;

  //cv::Mat MASK_FRAME_OVERLAY_LINES;

  // Display Frames
  cv::Mat LIVE_FRAME_0; // Live frame buffer to LIVE_FRAME.
  cv::Mat LIVE_FRAME_1; // Live frame buffer to LIVE_FRAME.
};

class CAMERA
{
  private:
  cv::Mat FRAME_DUMMY;
  cv::Mat FRAME_DUMMY2; // for testing double buffer

  cv::Size POST_PROCESS_SIZE;

  // Object detection member
  bool CAR_CASCADE_LOADED = false;
  cv::CascadeClassifier CAR_CASCADE;

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
  const int CANNY_THRESH_LOW = 100;  // Lower threshold for edge detection (50)
  const int CANNY_THRESH_HIGH = 200;// Upper threshold for edge detection (150)
  const int CANNY_APERTURE = 3;     // Aperture size for Sobel operator   (3) 3 5 or 7

  // Camera CV Helper
  int  is_low_light(const cv::Mat& Grey_Image_Full_Size);
  void gray_enhance(cv::Mat& processed_frame, const cv::Mat& Grey_Image_Full_Size);
  void low_light_filter(cv::Mat& processed_frame);
  void apply_min_max_contrast(cv::Mat& processed_frame);
  void detect_hough_circles(cv::Mat& processed_frame, const cv::Mat& Processed_Frame_Gaussian);
  cv::Mat canny_mask(cv::Mat& Processed_Frame_Canny);
  cv::Mat overlay_lines(cv::Mat& Processed_Frame_Gray_Downsized);
  cv::Mat suppress_glare_mask(cv::Mat& processed_frame);
  cv::Mat extract_glare_area(cv::Mat& processed_frame);

  // Change Settings
  bool set_control(uint32_t id, int32_t value); // returns true on success
  int get_control(uint32_t id);                 // returns -1 on failure, otherwise returns value.

  cv::Mat generate_empty_frame(int width, int height);
  cv::Mat generateDummyFrame(int width, int height);
  cv::Mat generateDummyFrame_2(int width, int height, int frame_index);
  cv::Mat generateDummyLowLightFrame(int width, int height, int frame_index);
  void matToTexture(const cv::Mat& frame, GLuint &textureID);

  void init(stringstream &Print_Stream);
  // Until camera properties are in a vector, manually set and get each control
  //  as necessary for first run.

  // ---------------------------------------------------------------------------------------
  //Multithreaded routines and vaiables. Access with caution.

  /*

  BUFFER_FRAME_HANDOFF_READY ... WORKING_FRAME_FULLY_PROCESSED ... LIVE_FRAME

  ------------------------process(--------------------------
  update_frame_thread().............process_enhancements_frame().....generate_imgui_texture_frame()
  FRAME_BUFFER[3]      PROCESSED_FRAME[2]                 LIVE_FRAME[2]
          FRAME_BUFFER_RESIZE[3]                              
                      V                                       V
  FRAME_TO_BUFFER = #
  LATEST_READY_FRAME  BUFFER_FRAME_HANDOFF_POSITION = +3         VIEWING_FRAME_POS +3 -- -1


  */
  
  // All Threads
  vector<std::string>    PRINTW_QUEUE;

  // Thread Update
  cv::VideoCapture  CAMERA_CAPTURE;

  bool CAM_AVAILABLE = false;

  FAKE_FRAME FAKE_FRAME_GENERATOR;
  
  TIMED_IS_READY  FRAME_TO_TEXTURE_TIMER;
  bool            FRAME_TO_TEXTURE_TIMER_CHECK = false;

  int LATEST_FRAME_READY = -1;
  FRAME_SUITE PROCESS_FRAMES_ARRAY[3];
  int BUFFER_FRAME_HANDOFF_POSITION  = -1;

  FRAME_SUITE_EXTRA PROCESS_FRAMES_EXTRA;

  // ---
  TIMED_IS_READY LOW_LIGHT_DEBOUNCE_TIMER_LL;
  int VIEWING_FRAME_POS = -1;

  bool CAMERA_BEING_VIEWED = false;

  // Turn on or off the camera
  bool ENABLED = false;
  
  // Load and Save settings
  void save_settings();
  void load_settings_json(vector<CAMERA_CONTROL_SETTING_LOADED> &Camera_Control);

  //void check_for_save_image_buffer_frame(cv::Mat &Frame);
  // check to see if current buffer should be saved to disk.

  //void check_for_save_image_buffer_processed(cv::Mat &Frame);
  // check to see if current buffer should be saved to disk.

  void run_preprocessing_inner(cv::Mat &Frame, cv::Mat &Frame_Output);
  // Apply orientation (flip logic)
  // Create Downsized image

  void run_preprocessing_outer(FRAME_SUITE &Suite, unsigned long Frame_Time);
  // Apply Denoising
  // Apply Sharpening
  // Frames and Vars not passed in parameter:
  //  PROCESSED_FRAME_GRAY
  //  PROCESSED_FRAME_CANNY
  //  CANNY_THRESH_LOW
  //  CANNY_THRESH_HIGH
  //  CANNY_APERTURE

  void apply_ehancements(FRAME_SUITE &Suite, FRAME_SUITE_EXTRA &Extra);
  // Apply all prop enable enhancements.
  // PROCESSED_FRAME created upon completion.

  void apply_masks_to_processed_frame(cv::Mat &Frame, cv::Mat &Mask_0, cv::Mat &Mask_1, int Latest);
  // A simple helper routine to apply double or single frame mask.

  void apply_image_masks_to_processed_frame(cv::Mat &Frame, cv::Mat &Mask_0, cv::Mat &Mask_1, int Latest);
  // A simple helper routine to apply double or single frame mask.

  void apply_all_masks(FRAME_SUITE &Suite, FRAME_SUITE_EXTRA &Extra);
  // Apply all prop enable enhancements.
  // PROCESSED_FRAME created upon completion.

  bool configure_v4l2_roi_and_format(std::stringstream& print_stream);
  // Alternative open_camera to force settings before openining.

  void close_camera();
  void open_camera();
  // Mmethod to close or create the camera capture.
  // Only to be called by the "update_frame" function, 
  //  within its own thread.

  void update_frame_thread();
  // Pull in latest frame from camera in a non thread-safe manner.

  void process_enhancements_thread();
  // Process enhancements on images as they arrive.
  
  // ---------------------------------------------------------------------------------------

  void generate_imgui_texture_frame(cv::Mat& Frame);
  // Converts PROCESSED_FRAME into ImGui Texture to be rendered
  //  into program display.
  // Copies PROCESSED_FRAME to LIVE_FRAME for thread safe access.

  bool set_camera_control(CAMERA_SETTING &Setting);
  int get_camera_control_value(CAMERA_SETTING &Setting);
  void apply_camera_control_changes();
  void apply_camera_control_defaults();
  
  void list_controls(vector<std::string> &String_Vector);
  void apply_loaded_camera_controls(vector<CAMERA_CONTROL_SETTING_LOADED> &Camera_Control, 
                                    deque<CAMERA_SETTING> &Settings);

  void release_all_frames();
  
  public:

  GLuint TEXTURE_ID = 0;

  std::string INFORMATION_COMMAND_LIST = "Not Available";

  double  TIME_CAMERA_FPS;
  double  TIME_CAMERA_FRAME_TIME;
  double  TIME_FRAME_RETRIEVAL;
  double  TIME_FRAME_PROCESSING;
  double  TIME_ACTUAL_FPS;
  int  TIME_AVERAGE_FPS;
  double  TIME_ACTUAL_FRAME_TIME;
  bool    IS_LOW_LIGHT = false;
  int     LOW_LIGHT_VALUE = 0;
  
  THREADING_INFO  THREAD_CAMERA;  // Controls: update_frame_thread()
  bool CAMERA_READ_THREAD_STOP   = true;            // Keep reading camera in thread 
                                                    // until told to stop 
  bool THREAD_CAMERA_ACTIVE = false;  

  TIMED_IS_READY  AVERAGE_FRAME_RATE_TIMER;
  int             AVERAGE_FRAME_RATE_COUNTER = 0;
                                                    // Variable.

  THREADING_INFO  THREAD_PROCESSING;  // Controls: process_enhancements_thread()
  bool CAMERA_PROCESSING_THREAD_STOP   = true;      // Keep processing images in thread 
                                                    // until told to stop
  bool THREAD_PROCESSING_ACTIVE = false;                                                     

  CAMERA_PROPERTIES PROPS;
  deque<CAMERA_SETTING> SETTINGS;

  // Changes that need to restart the camera
  bool APPLY_DEFAULTS = false;
  bool APPLY_CHANGES = false;
  bool APPLY_RESTART = false;
  
  int RESTART_WIDTH = 640;
  int RESTART_HEIGHT = 480;
  int RESTART_COMPRESSION = 1;
  
  bool       FRAME_GEN = false;

  int post_process_height();
  int post_process_width();
  int bfhp();

  // Manually print output stream
  void print_stream(CONSOLE_COMMUNICATION &cons);

  void process(CONSOLE_COMMUNICATION &cons, unsigned long Frame_Time, bool Camera_Being_Viewed);

  void take_snapshot();

  // Load all camera settings
  void load();

  // Turn on or off the camera
  void enable(bool Enable);

  // Report if camera is opened.
  bool camera_online();
};


// ---------------------------------------------------------------------------------------

#endif
