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

// Read output from shell continuously
void TERMINAL::reader_thread()
{
  char buf[512];
  while (true) 
  {
    ssize_t n = read(MASTER_FD, buf, sizeof(buf) - 1);
    if (n <= 0) break; // shell closed or error
    buf[n] = '\0';
    std::lock_guard<std::mutex> lock(BUF_MUTEX);
    TERM_BUF.push_back(strip_ansi(std::string(buf)));
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