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

void COLOR_COMBO::set(float H, float S, float V)
{
  TEXT = ImColor::HSV(H, .7, V + .3);
  BACKGROUND = ImColor::HSV(H, S - .3, V - .3);
  STANDARD = ImColor::HSV(H, S, V);
  HOVERED = ImColor::HSV(H, S + .1, V + .1);
  ACTIVE = ImColor::HSV(H, S + .2, V + .2);
  DEFAULT = false;
  }

void COLOR_COMBOS::init()
{
  COLOR_COMB_BLACK.set(0.0f, 0.0f, 0.0f);
  COLOR_COMB_WHITE.set(0.0f, 0.0f, .6f);
  COLOR_COMB_ORANGE.set(0.074f, 0.8f, 0.6f);
  COLOR_COMB_YELLOW.set(0.162, 0.8f, 0.6f);
  COLOR_COMB_GREEN.set(0.333f, 0.8f, 0.6f);
  COLOR_COMB_CYAN.set(0.5f, 0.8f, 0.6f);
  COLOR_COMB_PURPLE.set(0.768f, 0.8f, 0.7f);
  COLOR_COMB_PINK.set(0.838f, 0.8f, 0.6f);

  // Exceptions

  COLOR_COMB_RED.set(0.0f, 0.8f, 0.6f);
  COLOR_COMB_RED.TEXT = ImColor::HSV(0.0f, 0.6f, 1.0f);
  
  COLOR_COMB_BLUE.set(0.667f, 0.8f, 0.6f);
  COLOR_COMB_BLUE.TEXT = ImColor::HSV(0.667f, 0.6f, 1.0f);

  COLOR_COMB_GREY.set(0.0f, 0.0f, 0.4f);
  COLOR_COMB_GREY.TEXT = ImColor::HSV(0.0f, 0.0f, 0.6f);
}

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