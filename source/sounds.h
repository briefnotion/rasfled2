// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: sounds.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef SOUNDS_H
#define SOUNDS_H

// Standard Header Files
#include <string>
//#include <deque>

// RASFled related header files
#include "commands.h"

// -------------------------------------------------------------------------------------

class SOUNDS
{
  private:

  string C6 = "/etc/rasfled/c6.wav";
  string D6 = "/etc/rasfled/d6.wav";
  string E6 = "/etc/rasfled/e6.wav";
  string F6 = "/etc/rasfled/f6.wav";
  string G6 = "/etc/rasfled/g6.wav";
  string A6 = "/etc/rasfled/a6.wav";
  string B6 = "/etc/rasfled/b6.wav";
  
  string C7 = "/etc/rasfled/c7.wav";
  string D7 = "/etc/rasfled/d7.wav";
  string E7 = "/etc/rasfled/e7.wav";
  string F7 = "/etc/rasfled/f7.wav";
  string G7 = "/etc/rasfled/g7.wav";
  string A7 = "/etc/rasfled/a7.wav";
  string B7 = "/etc/rasfled/b7.wav";

  bool MUTE = false;

  public:

  void set_mute(bool Mute);

  void add_note_to_queue(COMMAND_THREAD &Thread, string Note);

  void play_alert_level_1(COMMAND_THREAD &Thread);
  void play_alert_level_2(COMMAND_THREAD &Thread);
  void play_alert_level_3(COMMAND_THREAD &Thread);

  void play_gear_park(COMMAND_THREAD &Thread);
  void play_gear_reverse(COMMAND_THREAD &Thread);
  void play_gear_neutral(COMMAND_THREAD &Thread);
  void play_gear_drive(COMMAND_THREAD &Thread);
  void play_gear_low(COMMAND_THREAD &Thread);
};

// -------------------------------------------------------------------------------------


#endif