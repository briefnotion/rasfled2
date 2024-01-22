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

  string TEST = "aplay -q /etc/RasFLED/test.wav";
  string SND1 = "aplay -q /etc/RasFLED/snd1_up.wav";
  string SND2 = "aplay -q /etc/RasFLED/snd1_up_2.wav";

  public:

  void play_test();
  void play_snd1();
  void play_snd2();
};

// -------------------------------------------------------------------------------------


#endif