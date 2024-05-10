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

void SOUNDS::play_c6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(C6.c_str(), 0);
  }
}

void SOUNDS::play_d6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(D6.c_str(), 0);
  }
}

void SOUNDS::play_e6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(E6.c_str(), 0);
  }
}

void SOUNDS::play_f6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(F6.c_str(), 0);
  }
}

void SOUNDS::play_g6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(G6.c_str(), 0);
  }
}

void SOUNDS::play_a6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(A6.c_str(), 0);
  }
}

void SOUNDS::play_b6(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    Thread.run_command(B6.c_str(), 0);
  }
}

void SOUNDS::play_alert_level_1(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    play_f6(Thread, Mute);
    play_c6(Thread, Mute);
  }
}

void SOUNDS::play_alert_level_2(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    play_b6(Thread, Mute);
    play_f6(Thread, Mute);
  }
}

void SOUNDS::play_alert_level_3(COMMAND_THREAD &Thread, bool Mute)
{
  if (Mute == false)
  {
    play_c6(Thread, Mute);
    play_f6(Thread, Mute);
    play_b6(Thread, Mute);
  }
}

// -------------------------------------------------------------------------------------


#endif