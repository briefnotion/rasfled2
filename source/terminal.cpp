// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: terminal.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef TERMINAL_CPP
#define TERMINAL_CPP

#include "terminal.h"

// ---------------------------------------------------------------------------------------

std::string TERMINAL::strip_ansi(const std::string &input) 
{
  // Matches CSI (Control Sequence Introducer) escape sequences
  static const std::regex ansi_regex("\x1B\\[[0-9;?]*[ -/]*[@-~]");
  // Matches other escape sequences (like OSC, etc.)
  static const std::regex osc_regex("\x1B\\][^\\a]*\a");

  std::string out = std::regex_replace(input, ansi_regex, "");
  out = std::regex_replace(out, osc_regex, "");
  return out;
}

// Spawn shell inside PTY
void TERMINAL::start_shell() 
{
  //int slave_fd;
  PID = forkpty(&MASTER_FD, nullptr, nullptr, nullptr);
  if (PID == 0) 
  {
    execl("/bin/bash", "bash", nullptr);
  }
}

/**
 * @brief Handles vertical scrolling by shifting all rows up.
 */
void TERMINAL::scroll_up() 
{
  // Shift all rows up by one (row i becomes row i-1)
  for (int i = 1; i < ROWS; ++i) 
  {
    std::copy(SCREEN[i], SCREEN[i] + COLS, SCREEN[i - 1]);
  }
  // Clear the new last line
  std::fill(SCREEN[ROWS - 1], SCREEN[ROWS - 1] + COLS, ' ');
}


/**
 * @brief Processes incoming text and writes it to the screen buffer, handling
 * newlines, line wraps, and scrolling.
 * @param text The cleaned (ANSI-stripped) string to display.
 */
void TERMINAL::process_output(const std::string& text) 
{
  for (char c : text) 
  {
    if (c == '\n') 
    {
      // Newline: move to the start of the next line
      CURRENT_COL = 0;
      CURRENT_ROW++;
    } 
    else if (c == '\r') 
    {
      // Carriage Return: move to the start of the current line
      CURRENT_COL = 0;
    } 
    else if (c == '\t') 
    {
      // Simple Tab handling: move to the next 8-column boundary
      int new_col = (CURRENT_COL / 8 + 1) * 8;
      CURRENT_COL = (new_col < COLS) ? new_col : COLS - 1;
    } 
    else if (std::isprint(static_cast<unsigned char>(c))) 
    {
      // Handle Printable Character
      if (CURRENT_COL >= COLS) 
      {
        // Line Wrap: move to the start of the next line
        CURRENT_COL = 0;
        CURRENT_ROW++;
      }
      
      // Check for Scrolling
      if (CURRENT_ROW >= ROWS) 
      {
        scroll_up();
        CURRENT_ROW = ROWS - 1; // Stay on the last line
      }
      
      // Write character to SCREEN
      SCREEN[CURRENT_ROW][CURRENT_COL] = c;
      CURRENT_COL++;
    }
    // Note: Non-printable characters (other than \n, \r, \t) are ignored.
  }
}


// Read output from shell continuously
void TERMINAL::reader_thread()
{
  char buf[512];
  while (MASTER_FD > 0) // Check if the master FD is valid
  {
    ssize_t n = read(MASTER_FD, buf, sizeof(buf) - 1);
    if (n <= 0) break; // Shell closed or error
    buf[n] = '\0';
    
    // 1. Strip ANSI codes
    std::string cleaned_output = strip_ansi(std::string(buf));

    // 2. Lock the shared resource and process the output
    std::lock_guard<std::mutex> lock(BUF_MUTEX);
    process_output(cleaned_output);
  }
}

void TERMINAL::create()
{
  start_shell();
  T = std::thread(&TERMINAL::reader_thread, this);
  T.detach();
}

void TERMINAL::send_command(const std::string& cmd)
{
  if (MASTER_FD > 0) 
  {
    write(MASTER_FD, cmd.c_str(), cmd.size());
    write(MASTER_FD, "\n", 1);
  }
}
// ---------------------------------------------------------------------------------------

#endif