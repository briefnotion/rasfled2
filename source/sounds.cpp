// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: sounds.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef SOUNDS_CPP
#define SOUNDS_CPP

#include "sounds.h"

using namespace std;

// -------------------------------------------------------------------------------------

void SOUNDS::play_test()
{
  Command_Thread.run_command(TEST.c_str());
  //system(TEST.c_str());
}

void SOUNDS::play_snd1()
{
  system(SND1.c_str());
}

void SOUNDS::play_snd2()
{
  system(SND2.c_str());
}

// -------------------------------------------------------------------------------------


#endif