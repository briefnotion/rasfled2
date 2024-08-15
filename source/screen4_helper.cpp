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

ImVec2 operator+(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x + V2.x, V1.y + V2.y);
}

ImVec2 operator-(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x - V2.x, V1.y - V2.y);
}

ImVec2 operator*(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x * V2.x, V1.y * V2.y);
}

ImVec2 operator*(ImVec2 V1, float Number)
{
  return ImVec2(V1.x * Number, V1.y * Number);
}

// ---------------------------------------------------------------------------------------

void COLOR_COMBO::set_rgb(float R, float G, float B, float A, float Intensity)
{
  TEXT = ImColor((R *0.8f + 0.5f) * Intensity, (G *0.8f + 0.5f) * Intensity, (B *0.8f + 0.5f) * Intensity, A);

  BACKGROUND  = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.2f);
  DIM         = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.5f);
  STANDARD    = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  STANDARD_V  = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  HOVERED     = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.8f);
  ACTIVE      = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.9f);
  SIMPLE_RGB  = CRGB(static_cast<unsigned char>(R * 255.0f), static_cast<unsigned char>(G * 255.0f), static_cast<unsigned char>(B * 255.0f));
}

void COLOR_COMBO::set_rgb_v(float R, float G, float B, float A, float Intensity)
{
  TEXT = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);

  BACKGROUND  = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.2f);
  DIM         = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.5f);
  STANDARD    = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  STANDARD_V  = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);
  HOVERED     = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.8f);
  ACTIVE      = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.9f);
  SIMPLE_RGB  = CRGB(static_cast<unsigned char>(R * 255.0f), static_cast<unsigned char>(G * 255.0f), static_cast<unsigned char>(B * 255.0f));
}


void COLOR_COMBOS::init(float Intensity)
{
  if (ALREADY_INITIALIZED == false)
  {
    ALREADY_INITIALIZED = true; //prevent another routine from running again

    COLOR_COMBO COLOR_COMB_BLACK; 
    COLOR_COMBO COLOR_COMB_WHITE;
    COLOR_COMBO COLOR_COMB_GREY;
    COLOR_COMBO COLOR_COMB_RED;
    COLOR_COMBO COLOR_COMB_ORANGE;
    COLOR_COMBO COLOR_COMB_YELLOW;
    COLOR_COMBO COLOR_COMB_GREEN;
    COLOR_COMBO COLOR_COMB_CYAN;
    COLOR_COMBO COLOR_COMB_BLUE;
    COLOR_COMBO COLOR_COMB_PURPLE;
    COLOR_COMBO COLOR_COMB_PINK;
    COLOR_COMBO COLOR_COMB_CORPO;

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

    // Specific Colors
    //COLOR_COMB_CORPO.set_rgb(1.0f, 0.0f, 0.0f, 1.0f, Intensity);
    //COLOR_COMB_CORPO.STANDARD_V = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);
    //COLOR_COMB_CORPO.TEXT = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);

    // Put colors in vector list for reference
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
    //COLOR_COMBINATIONS.push_back(COLOR_COMB_CORPO);

    // Base Colors V
    COLOR_COMB_BLACK.set_rgb_v(0.0f, 0.0f, 0.0f, 1.0f, Intensity);
    COLOR_COMB_WHITE.set_rgb_v(1.0f, 1.0f, 1.0f, 1.0f, Intensity);
    COLOR_COMB_GREY.set_rgb_v(0.2f, 0.2f, 0.2f, 1.0f, Intensity);
    COLOR_COMB_RED.set_rgb_v(1.0f, 0.0f, 0.0f, 1.0f, Intensity);
    COLOR_COMB_ORANGE.set_rgb_v(1.0f, 0.75f, 0.0f, 1.0f, Intensity);
    COLOR_COMB_YELLOW.set_rgb_v(1.0f, 1.0f, 0.0f, 1.0f, Intensity);
    COLOR_COMB_GREEN.set_rgb_v(0.0f, 1.0f, 0.0f, 1.0f, Intensity);
    COLOR_COMB_CYAN.set_rgb_v(0.0f, 1.0f, 1.0f, 1.0f, Intensity);
    COLOR_COMB_BLUE.set_rgb_v(0.0f, 0.0f, 1.0f, 1.0f, Intensity);
    COLOR_COMB_PURPLE.set_rgb_v(1.0f, 0.0f, 1.0f, 1.0f, Intensity);
    COLOR_COMB_PINK.set_rgb_v(1.0f, 0.0f, 0.5f, 1.0f, Intensity);

    // Put colors in vector list for reference
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_BLACK);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_WHITE);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_GREY);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_RED);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_ORANGE);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_YELLOW);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_GREEN);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_CYAN);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_BLUE);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_PURPLE);
    COLOR_COMBINATIONS_V.push_back(COLOR_COMB_PINK);
  }
}

COLOR_COMBO COLOR_COMBOS::color(int Color)
{
  if (void_color)
  {
    if (Color == 0)
    {
      return COLOR_COMBINATIONS[0];
    }
    else
    {
      return COLOR_COMBINATIONS_V[void_color_value];
    }
  }
  else
  {
    return COLOR_COMBINATIONS[Color];
  }
}

void COLOR_COMBOS::toggle_void_color(unsigned long Time)
{
  if (void_color_fade_timer.ping_down(Time) == false)
  {
    CHANGED = true;
    void_color = !void_color;
    void_color_fade_timer.ping_up(Time, 5000);
  }
}

COLOR_COMBO COLOR_COMBOS::c_black()
{
  return color(0);
}

COLOR_COMBO COLOR_COMBOS::c_white()
{
  return color(1);
}

COLOR_COMBO COLOR_COMBOS::c_grey()
{
  return color(2);
}

COLOR_COMBO COLOR_COMBOS::c_red()
{
  return color(3);
}

COLOR_COMBO COLOR_COMBOS::c_orange()
{
  return color(4);
}

COLOR_COMBO COLOR_COMBOS::c_yellow()
{
  return color(5);
}

COLOR_COMBO COLOR_COMBOS::c_green()
{
  return color(6);
}

COLOR_COMBO COLOR_COMBOS::c_cyan()
{
  return color(7);
}

COLOR_COMBO COLOR_COMBOS::c_blue()
{
  return color(8);
}

COLOR_COMBO COLOR_COMBOS::c_purple()
{
  return color(9);
}

COLOR_COMBO COLOR_COMBOS::c_pink()
{
  return color(10);
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

int COLOR_COMBOS::void_colr()
{
  return void_color_value;
}

void COLOR_COMBOS::void_color_set(int Color)
{
  void_color_value = Color;
  CHANGED = true;
}

bool COLOR_COMBOS::changed_no_reset()
{
  return CHANGED;
}

bool COLOR_COMBOS::changed()
{
  if (CHANGED)
  {
    CHANGED = false;
    return true;
  }
  else
  {
    return false;
  }
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

void CONSOLE_COMMUNICATION::command_pending_set(string Text)
{
  PENDING_CONFIRM_COMMAND = Text;
  PENDING_CONFIRM_WITH_CR = false;
}

void CONSOLE_COMMUNICATION::command_pending_set_cr(string Text)
{
  PENDING_CONFIRM_COMMAND = Text;
  PENDING_CONFIRM_WITH_CR = true;
}

void CONSOLE_COMMUNICATION::command_pending_send()
{
  command_text_set(PENDING_CONFIRM_COMMAND);
  if (PENDING_CONFIRM_WITH_CR)
  {
    carrage_return_set();
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
  CHANGED = true;
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