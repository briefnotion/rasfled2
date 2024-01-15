// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: hmc5883l.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef HMC5883L_CPP
#define HMC5883L_CPP

#include "hmc5883l.h"

using namespace std;

// -------------------------------------------------------------------------------------
// 

bool HMC5883L::register_write(char Register, char Value)
{
  bool ret_write_success = false;
  char buffer[2];

  buffer[0] = Register;
  buffer[1] = Value;

  if (write(DEVICE, buffer, 2) == 2)
  {
    ret_write_success = true;
  }

  return ret_write_success;
}

bool HMC5883L::create()
{
  bool ret_success = false;

  RAW_POINTS.clear();
  RAW_POINTS.reserve(1000);  
  MIN_MAX_HAS_DATA = false;
  X_OFFSET = 0;
  Y_OFFSET = 0;
  Z_OFFSET = 0;

  CONNECTED = false;

  if (PROPS.AUTOSTART == false)
  {
    // Disable Cycles
    CYCLE = -1;
  }

  DEVICE = open(PROPS.PORT.c_str(), O_RDWR);
  if (DEVICE >= 0)
  {
    if (ioctl(DEVICE, I2C_SLAVE, PROPS.I2C_ID) >= 0)
    {
      // default settings
      register_write(0x01, 0x20); // Still figuring out register writes
      register_write(0x02, 0x00); // Still figuring out register writes
      
      ret_success = true;
      CONNECTED = true;
    }
  }

  return ret_success;
}

void HMC5883L::stop()
{
  CONNECTED = false;
}

void HMC5883L::process()
{
  RAW_POINTS.push_back(RAW_XYZ);

  // Maintain point history size
  if (RAW_POINTS.size() > 990)
  {
    RAW_POINTS.erase(RAW_POINTS.begin(), RAW_POINTS.begin() + 190);
  }

  // Spot check Calibration
  //  code not yet written.


  // Run Calibration Routines if necessary.
  if (CALIBRATE)
  {
    if (MIN_MAX_HAS_DATA == false)
    {
      X_MIN = (float)RAW_XYZ.X;
      X_MAX = (float)RAW_XYZ.X;
      Y_MIN = (float)RAW_XYZ.Y;
      Y_MAX = (float)RAW_XYZ.Y;
      Z_MIN = (float)RAW_XYZ.Z;
      Z_MAX = (float)RAW_XYZ.Z;

      MIN_MAX_HAS_DATA = true;
    }
    else
    {
      if (RAW_XYZ.X < X_MIN)
      {
        X_MIN = RAW_XYZ.X;
      }
      if (RAW_XYZ.X > X_MAX)
      {
         X_MAX = RAW_XYZ.X;
      }
      if (RAW_XYZ.Y < Y_MIN)
      {
         Y_MIN = RAW_XYZ.Y;
      }
      if (RAW_XYZ.Y > Y_MAX)
      {
         Y_MAX = RAW_XYZ.Y;
      }
      if (RAW_XYZ.Z < Z_MIN)
      {
         Z_MIN = RAW_XYZ.Z;
      }
      if (RAW_XYZ.Z > Z_MAX)
      {
         Z_MAX = RAW_XYZ.Z;
      }

      X_OFFSET = (X_MAX + X_MIN) / 2.0f;
      Y_OFFSET = (Y_MAX + Y_MIN) / 2.0f;
      Z_OFFSET = (Z_MAX + Z_MIN) / 2.0f;
    }
  }
}

int HMC5883L::raw_points_size()
{
  return RAW_POINTS.size();
}

COMPASS_XYZ HMC5883L::raw_xyz()
{
  if (RAW_POINTS.size() >0)
  {
    return RAW_POINTS[RAW_POINTS.size() -1];
  }
  else
  {
    COMPASS_XYZ tmp_point;
    return tmp_point;
  }
}

COMPASS_XYZ HMC5883L::raw_xyz(int Position)
{
  return RAW_POINTS[Position];
}

COMPASS_XYZ HMC5883L::calibrated_xyz()
{
  if (RAW_POINTS.size() >0)
  {
    COMPASS_XYZ tmp_point;
    tmp_point.X = RAW_POINTS[RAW_POINTS.size() -1].X - X_OFFSET;
    tmp_point.Y = RAW_POINTS[RAW_POINTS.size() -1].Y - Y_OFFSET;
    tmp_point.Z = RAW_POINTS[RAW_POINTS.size() -1].Z - Z_OFFSET;
    return tmp_point;
  }
  else
  {
    COMPASS_XYZ tmp_point;
    return tmp_point;
  }
}

COMPASS_XYZ HMC5883L::calibrated_xyz(int Position)
{
  COMPASS_XYZ tmp_point;

  tmp_point.X = RAW_POINTS[Position].X - X_OFFSET;
  tmp_point.Y = RAW_POINTS[Position].Y - Y_OFFSET;
  tmp_point.Z = RAW_POINTS[Position].Z - Z_OFFSET;

  return tmp_point;
}

void HMC5883L::calibrateion_reset()
{
  RAW_POINTS.clear();
  MIN_MAX_HAS_DATA = false;
  X_OFFSET = 0;
  Y_OFFSET = 0;
  Z_OFFSET = 0;
}

void HMC5883L::calibrate_toggle()
{
  CALIBRATE = !CALIBRATE;
}

bool HMC5883L::calibrate_on()
{
  return CALIBRATE;
}

bool HMC5883L::connected()
{
  return CONNECTED;
}

bool HMC5883L::cycle(unsigned long tmeFrame_Time)
{
  //bool data_received = true;
  bool ret_cycle_changed = false;

  if (CYCLE == 99)
  {
    // First Run
    DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 2000);

    CYCLE_CHANGE = 99;
    ret_cycle_changed = true;

    if (PROPS.AUTOSTART)
    {
      CYCLE = 2;  // Start connection cycle.
    }
    else
    {
      CYCLE = -1; // Read as if connected
    }
  }
  else if (CYCLE == -1)
  {
    // Ignore all cycles if Cycle is -1.
  }
  else if (CYCLE == 0)
  {
    // Keep active with checking for live data
    if (PROPS.AUTOSTART && PROPS.CONTINUOUS_DATA)
    {
      if (DATA_RECIEVED_TIMER.ping_down(tmeFrame_Time) == false)
      {
        CYCLE = 1;
      }
    }
  }
  else if (CYCLE == 1)
  {
    // Check for cycle close. Need to ensure all comm sent before closing connection and reconnecting
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 1;
      ret_cycle_changed = true;

      // Close the connection.
      //  Don't start reconnect if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        CYCLE_TIMER.ping_up(tmeFrame_Time, 1000); // Wait before starting next cycle.
        CYCLE = 2;

        stop();
      }
      else
      {
        CYCLE = -1;
      }
    }
  }
  else if (CYCLE == 2)
  {
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 2;
      ret_cycle_changed = true;

      //  Open a new connection at current baud rate.
      //  Don't prepare check baud rate if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        CYCLE = 0; // Go into normal cycle.
        create();
      }
      else
      {
        CYCLE = -1;
        create();
      }
    }
  }

  if (CONNECTED == true)
  {
    // get raw x, y, z compass data
    BUFFER[0] = 0x03;   // Set to request xyz.

    // Write buffer
    if ((write(DEVICE, BUFFER, 1)) != 1)
    {
      // stop comms.
      CYCLE_CHANGE = -1;
      ret_cycle_changed = true;
      CYCLE = -1;
      stop();
    }
    else
    {
      // Read buffer
      if (read(DEVICE, BUFFER, 6) != 6)
      {
        // stop comms.
        CYCLE_CHANGE = -1;
        ret_cycle_changed = true;
        CYCLE = -1;
        stop();
      }
      else
      {
        short x = 0;
        short y = 0;
        short z = 0;

        x = (BUFFER[0] << 8) | BUFFER[1];
        y = (BUFFER[4] << 8) | BUFFER[5];
        z = (BUFFER[2] << 8) | BUFFER[3];

        RAW_XYZ.X = y;
        RAW_XYZ.Y = x;
        RAW_XYZ.Z = z;

        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 5000);   // Looking for live data

        // Process
        process();
      }
    }
  }

  return ret_cycle_changed;
}

int HMC5883L::cycle_change()
{
  return CYCLE_CHANGE;
}

void HMC5883L::open_port()
{
  CYCLE = 2;
}

void HMC5883L::close_port()
{
  CYCLE = -1;
  stop();
}

//bool HMC5883L::active(unsigned long tmeFrame_Time)
//{
//  return DATA_RECIEVED_TIMER.ping_down(tmeFrame_Time);
//}

float HMC5883L::bearing()
{
  float ret_bearing = (atan2(calibrated_xyz().Y, calibrated_xyz().X) * 180 / M_PI) + 90.0f - KNOWN_DEVICE_DEGREE_OFFSET;
  if (ret_bearing < -0.0f)
  {
    return 360.0f + ret_bearing;
  }
  else
  {
    return ret_bearing;
  }
}


#endif