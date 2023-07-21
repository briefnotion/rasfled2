// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4_helper.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_HELPER_H
#define SCREEN4_HELPER_H

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------------------

class DEBUG
{
  public:
  
  //Debug On Off
  bool DEBUG = false;
  
  // Doors
  vector<bool> DOOR;
};


class CONSOLE_COMMUNICATION
// A smaller variable to facilitate communication to the console.
{
  private: 

  bool COMMAND_TEXT_CLEAR = false;
  bool CARRAGE_RETURN = false;

  string COMMAND_TEXT = "";
  bool CHANGED = false;

  string PRINT_Q = "";
  bool PRINT_Q_AVAIL = false;

  public:

  DEBUG DEBUG_STATUS;

  bool WINDOW_CLOSE = false;

  void carrage_return_set();
  bool carrage_return_get();

  void command_text_set(string Text);
  bool command_text_changed();
  string command_text_get();

  void command_text_clear();
  bool command_text_clear_get();
  
  bool printw_q_avail();
  string printw_q_get();
  void printw(string Text);
  
};

// ---------------------------------------------------------------------------------------
#endif