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
//#include <stdio.h>
//#include <string>
//#include <deque>

// RASFled related header files
//#include "system.h"
#include "commands.h"

// -------------------------------------------------------------------------------------

class SOUNDS
{
  private:

  COMMAND_THREAD Command_Thread;

  string C6 = "aplay -q /etc/rasfled/c6.wav";
  string D6 = "aplay -q /etc/rasfled/c6.wav";
  string E6 = "aplay -q /etc/rasfled/c6.wav";
  string F6 = "aplay -q /etc/rasfled/c6.wav";
  string G6 = "aplay -q /etc/rasfled/c6.wav";
  string A6 = "aplay -q /etc/rasfled/c6.wav";
  string B6 = "aplay -q /etc/rasfled/c6.wav";

  string ALERT_LEVEL_1 = "aplay -q /etc/rasfled/mid_low.wav";
  string ALERT_LEVEL_2 = "aplay -q /etc/rasfled/hi_mid.wav";
  string ALERT_LEVEL_3 = "aplay -q /etc/rasfled/low_mid_hi.wav";

  public:

  void play_c6(bool Mute);
  void play_d6(bool Mute);
  void play_e6(bool Mute);
  void play_f6(bool Mute);
  void play_g6(bool Mute);
  void play_a6(bool Mute);
  void play_b6(bool Mute);

  void play_alert_level_1(bool Mute);
  void play_alert_level_2(bool Mute);
  void play_alert_level_3(bool Mute);
};

// -------------------------------------------------------------------------------------


#endif