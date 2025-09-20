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

// Rasfled Includes
#include "system.h"
//#include "LEDstuff.h"
#include "screen4_helper.h"
#include "widgets.h"
//#include "widgets_drawing.h"
//#include "animations_handler.h"

//#include "stringthings.h"
//#include "fled_time.h"

// IMGui Includes
#include "../../imgui/imgui.h"
//#include "../../imgui/misc/cpp/imgui_stdlib.h"

using namespace std;

// ---------------------------------------------------------------------------------------

class TERMINAL_SCREEN
{
  private:  
  
  char input[256];

  public:  
  
  void display(system_data &sdSysData);
  // Draw all changes to Panel.
  // Returns true if panel was redrawn.

};
// ---------------------------------------------------------------------------------------

#endif