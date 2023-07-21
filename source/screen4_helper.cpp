// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4_helper.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_HELPER_CPP
#define SCREEN4_HELPER_CPP

#include "screen4_helper.h"

// ---------------------------------------------------------------------------------------

void CONSOLE_COMMUNICATION::carrage_return_set()
{
  CARRAGE_RETURN = true;
}

bool CONSOLE_COMMUNICATION::carrage_return_get()
{
  if (CARRAGE_RETURN == true)
  {
    CARRAGE_RETURN = false;
    return true;
  }
  else
  {
    return false;
  }
}

void CONSOLE_COMMUNICATION::command_text_set(string Text)
{
  if (COMMAND_TEXT != Text)
  {
    COMMAND_TEXT = Text;
    COMMAND_TEXT_CLEAR = false;
    CHANGED = true;
  }
}

bool CONSOLE_COMMUNICATION::command_text_changed()
{
  if (CHANGED == true)
  {
    CHANGED = false;
    return true;
  }
  else
  {
    return false;
  }
}

string CONSOLE_COMMUNICATION::command_text_get()
{
  return COMMAND_TEXT;
}

void CONSOLE_COMMUNICATION::command_text_clear()
{
  COMMAND_TEXT = "";
  CHANGED = false;
  COMMAND_TEXT_CLEAR = true;
}

bool CONSOLE_COMMUNICATION::printw_q_avail()
{
  return PRINT_Q_AVAIL;
}

string CONSOLE_COMMUNICATION::printw_q_get()
{
  string ret_q = PRINT_Q;
  PRINT_Q = "";
  PRINT_Q_AVAIL = false;
  return ret_q;
}

void CONSOLE_COMMUNICATION::printw(string Text)
{
  PRINT_Q_AVAIL = true;
  PRINT_Q = PRINT_Q + Text + "\n";
}

// ---------------------------------------------------------------------------------------

#endif