// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: terminal.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef TERMINAL_H
#define TTERMINAL_H

#include <string>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <pty.h>
#include <thread>
#include <regex>

// Rasfled Includes
//#include "stringthings.h"
//#include "system.h"
//#include "helper.h"
//#include "fled_time.h"
//#include "nmea.h"
//#include "hmc5883l.h"
//#include "aircraft_coordinator.h"
//#include "screen4_helper.h"
//#include "widgets.h"
//#include "widgets_drawing.h"
//#include "globe_helper.h"
//#include "map.h"

using namespace std;

class TERMINAL
{
  private:

  std::string strip_ansi(const std::string &input);

  public:
  pid_t PID;
  std::thread T;

  int MASTER_FD = -1;
  std::vector<std::string> TERM_BUF;
  std::mutex BUF_MUTEX;

  void start_shell();
  void reader_thread();
  void create();
  void send_command(const std::string& cmd);
};


// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------

#endif