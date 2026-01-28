// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_terminal.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_TERMINAL_H
#define WIDGETS_TERMINAL_H

//#include <stdio.h>
#include <string>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <pty.h>
#include <thread>
#include <vector>
#include <filesystem> // For std::filesystem::exists

// Rasfled Includes
#include "system.h"
#include "screen4_helper.h"
#include "widgets.h"

// IMGui Includes
#include "../../imgui/imgui.h"

using namespace std;

// ---------------------------------------------------------------------------------------

class TERMINAL_SCREEN
{
  private:  
  
  char input[256];

  TIMED_IS_READY  CURSOR_BLINK;
  bool            CURSOR_ON = false;

  // Buttons
  TIMED_IS_READY SHOW_BUTTONS_TIMER;
  bool SHOW_BUTTONS = false;
  vector<BUTTON_TOGGLE_COLOR> BC_TERMINAL_SELECT_BUTTONS;

  void display_terminal(system_data &sdSysData, TERMINAL &Term);

  public:  

  // Draw all changes to Panel.
  void display(system_data &sdSysData);

};
// ---------------------------------------------------------------------------------------

#endif