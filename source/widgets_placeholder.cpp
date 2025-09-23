// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_placeholder.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_PLACEHOLDER_CPP
#define WIDGETS_PLACEHOLDER_CPP

#include "widgets_placeholder.h"

// ---------------------------------------------------------------------------------------

void PLACEHOLDER::display(system_data &sdSysData)
{
  cout << "placeholder time: " << sdSysData.PROGRAM_TIME.current_frame_time() << endl;
}

// ---------------------------------------------------------------------------------------

#endif