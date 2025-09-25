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

void CAMERA::create()
{
  // Attempt to open the default camera (index 0).
  CAMERA_CAPTURE.open(0);

  // Check if the camera was successfully opened.
  bool useDummy = !CAMERA_CAPTURE.isOpened();
  if (!useDummy)
  {
    // If the camera is opened, set its frame width and height.
    CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_WIDTH, PROPS.WIDTH);
    CAMERA_CAPTURE.set(cv::CAP_PROP_FRAME_HEIGHT, PROPS.HEIGHT);
    //CAMERA_CAPTURE.set(cv::CAP_PROP_AUTOFOCUS, PROPS.AUTO_FOCUS);
    //CAMERA_CAPTURE.set(cv::CAP_PROP_FOCUS, PROPS.FOCUS);

    std::cout << "Camera successfully opened and properties set." << std::endl;

    // Now, probe the camera for its actual capabilities and print them.
    double actual_width = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_WIDTH);
    double actual_height = CAMERA_CAPTURE.get(cv::CAP_PROP_FRAME_HEIGHT);
    double actual_fps = CAMERA_CAPTURE.get(cv::CAP_PROP_FPS);

    std::cout << "Actual camera resolution: " << actual_width << "x" << actual_height << std::endl;
    std::cout << "Actual camera FPS: " << actual_fps << std::endl;

    CAMERA_AVAILABLE = true;
  }
  else
  {
    std::cout << "Could not open camera. Please check your camera connection." << std::endl;
    CAMERA_AVAILABLE = false;
  }
}

void CAMERA::update_frame()
{
  // Capture the camera frame.
  CAMERA_CAPTURE >> FRAME;
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
      VIDEO_AVAILABLE = true;
    }
    else
    {
      VIDEO_AVAILABLE = false;
    }
  }
}

cv::Mat CAMERA::get_current_frame()
{
  // Return a copy of the FRAME to the caller.
  return PROCESSED_FRAME;
}

void CAMERA::close()
{
  CAMERA_CAPTURE.release();
}

// ---------------------------------------------------------------------------------------

#endif