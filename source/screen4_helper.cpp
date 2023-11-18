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

void COLOR_COMBO::set_rgb(float R, float G, float B, float A, float Intensity)
{
  TEXT = ImColor((R *0.8f + 0.5f) * Intensity, (G *0.8f + 0.5f) * Intensity, (B *0.8f + 0.5f) * Intensity, A);

  BACKGROUND  = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.2f);
  DIM         = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.5f);
  STANDARD    = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  HOVERED     = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.8f);
  ACTIVE      = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.9f);

}

void COLOR_COMBOS::init(float Intensity)
{
  // Base Colors
  COLOR_COMB_BLACK.set_rgb(0.0f, 0.0f, 0.0f, 1.0f, Intensity);
  COLOR_COMB_WHITE.set_rgb(1.0f, 1.0f, 1.0f, 1.0f, Intensity);
  COLOR_COMB_GREY.set_rgb(0.2f, 0.2f, 0.2f, 1.0f, Intensity);
  COLOR_COMB_RED.set_rgb(1.0f, 0.0f, 0.0f, 1.0f, Intensity);
  COLOR_COMB_ORANGE.set_rgb(1.0f, 0.75f, 0.0f, 1.0f, Intensity);
  COLOR_COMB_YELLOW.set_rgb(1.0f, 1.0f, 0.0f, 1.0f, Intensity);
  COLOR_COMB_GREEN.set_rgb(0.0f, 1.0f, 0.0f, 1.0f, Intensity);
  COLOR_COMB_CYAN.set_rgb(0.0f, 1.0f, 1.0f, 1.0f, Intensity);
  COLOR_COMB_BLUE.set_rgb(0.0f, 0.0f, 1.0f, 1.0f, Intensity);
  COLOR_COMB_PURPLE.set_rgb(1.0f, 0.0f, 1.0f, 1.0f, Intensity);
  COLOR_COMB_PINK.set_rgb(1.0f, 0.0f, 0.5f, 1.0f, Intensity);

  COLOR_COMBINATIONS.push_back(COLOR_COMB_BLACK);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_WHITE);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_GREY);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_RED);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_ORANGE);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_YELLOW);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_GREEN);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_CYAN);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_BLUE);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_PURPLE);
  COLOR_COMBINATIONS.push_back(COLOR_COMB_PINK);
}

COLOR_COMBO COLOR_COMBOS::color(int Color)
{
  return COLOR_COMBINATIONS[Color];
}

int COLOR_COMBOS::black()
{
  return 0;
}

int COLOR_COMBOS::white()
{
  return 1;
}

int COLOR_COMBOS::grey()
{
  return 2;
}

int COLOR_COMBOS::red()
{
  return 3;
}

int COLOR_COMBOS::orange()
{
  return 4;
}

int COLOR_COMBOS::yellow()
{
  return 5;
}

int COLOR_COMBOS::green()
{
  return 6;
}

int COLOR_COMBOS::cyan()
{
  return 7;
}

int COLOR_COMBOS::blue()
{
  return 8;
}

int COLOR_COMBOS::purple()
{
  return 9;
}

int COLOR_COMBOS::pink()
{
  return 10;
}


// ---------------------------------------------------------------------------------------

void CONSOLE_COMMUNICATION::carrage_return_set()
{
  CARRAGE_RETURN = true;
  CHANGED = true;
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
  CARRAGE_RETURN = false;
  CHANGED = false;
  COMMAND_TEXT_CLEAR = true;
}

bool CONSOLE_COMMUNICATION::command_text_clear_get()
{
  if (COMMAND_TEXT_CLEAR == true)
  {
    COMMAND_TEXT_CLEAR = false;
    return true;
  }
  else
  {
    return false;
  }
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