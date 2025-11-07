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

#include <iostream>

#include <string>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <pty.h>
#include <thread>
#include <regex>
#include <signal.h>   // For SIGKILL and other signal constants

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

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val);

class TERMINAL
{
  private:

  pid_t PID;
  std::thread T;

  int MASTER_FD = -1;

  void start_shell();

  void scroll_up(int count);
  void scroll_down(int count);
  void insert_line(int count);
  void delete_line(int count);

  bool process_csi_cursor_movement(char final_char, const std::vector<int>& params);
  bool process_csi_erase_and_edit(char final_char, const std::vector<int>& params);
  bool process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private);
  bool process_csi_sgr_and_misc(char final_char, const std::vector<int>& params);
  void process_output(const std::string& text);

  public:

  // Constants for screen dimensions
  static const int ROWS = 21;
  static const int COLS = 97;

  // Buffer containing the terminal screen
  char SCREEN[ROWS][COLS];
  int CURRENT_ROW;
  int CURRENT_COL;
  int SCROLL_TOP;
  int SCROLL_BOTTOM;
  int SAVED_ROW;    // New: Stored cursor row for DECSC/DECRC
  int SAVED_COL;    // New: Stored cursor column for DECSC/DECRC
  bool APP_CURSOR_MODE; // New state tracker for DEC Private Mode 1 (Application Cursor Keys)
  
  std::mutex BUF_MUTEX;

  TERMINAL();
  ~TERMINAL();

  std::mutex& get_mutex();

  void reader_thread();
  void create();
  void send_command(const std::string& charaters);
};

// ---------------------------------------------------------------------------------------

#endif