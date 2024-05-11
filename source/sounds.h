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

  public:

  void play_c6(COMMAND_THREAD &Thread, bool Mute);
  void play_d6(COMMAND_THREAD &Thread, bool Mute);
  void play_e6(COMMAND_THREAD &Thread, bool Mute);
  void play_f6(COMMAND_THREAD &Thread, bool Mute);
  void play_g6(COMMAND_THREAD &Thread, bool Mute);
  void play_a6(COMMAND_THREAD &Thread, bool Mute);
  void play_b6(COMMAND_THREAD &Thread, bool Mute);

  void play_alert_level_1(COMMAND_THREAD &Thread, bool Mute);
  void play_alert_level_2(COMMAND_THREAD &Thread, bool Mute);
  void play_alert_level_3(COMMAND_THREAD &Thread, bool Mute);
};

// -------------------------------------------------------------------------------------


#endif