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

void SOUNDS::create(string Audio_Directory)
{
  C6 = Audio_Directory + C6;
  D6 = Audio_Directory + D6;
  E6 = Audio_Directory + E6;
  F6 = Audio_Directory + F6;
  G6 = Audio_Directory + G6;
  A6 = Audio_Directory + A6;
  B6 = Audio_Directory + B6;
  
  C7 = Audio_Directory + C7;
  D7 = Audio_Directory + D7;
  E7 = Audio_Directory + E7;
  F7 = Audio_Directory + F7;
  G7 = Audio_Directory + G7;
  A7 = Audio_Directory + A7;
  B7 = Audio_Directory + B7;
}

void SOUNDS::set_mute(bool Mute)
{
  MUTE = Mute;
}

void SOUNDS::add_note_to_queue(COMMAND_THREAD &Thread, string Note)
{
  string note_filename = "";

  if (Note == "c6")
  {
    note_filename = C6;
  }
  else if (Note == "d6")
  {
    note_filename = D6;
  }
  else if (Note == "e6")
  {
    note_filename = E6;
  }
  else if (Note == "f6")
  {
    note_filename = F6;
  }
  else if (Note == "g6")
  {
    note_filename = G6;
  }
  else if (Note == "a6")
  {
    note_filename = A6;
  }
  else if (Note == "b6")
  {
    note_filename = B6;
  }

  else if (Note == "c7")
  {
    note_filename = C7;
  }
  else if (Note == "d7")
  {
    note_filename = D7;
  }
  else if (Note == "e7")
  {
    note_filename = E7;
  }
  else if (Note == "f7")
  {
    note_filename = F7;
  }
  else if (Note == "g7")
  {
    note_filename = G7;
  }
  else if (Note == "a7")
  {
    note_filename = A7;
  }
  else if (Note == "b7")
  {
    note_filename = B7;
  }

  if (note_filename != "" && MUTE == false)
  {
    Thread.run_command(note_filename.c_str(), 0);
  }
}

void SOUNDS::play_alert_level_1(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "d7");
  add_note_to_queue(Thread, "c7");
}

void SOUNDS::play_alert_level_2(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "e7");
  add_note_to_queue(Thread, "d7");
}

void SOUNDS::play_alert_level_3(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "c7");
  add_note_to_queue(Thread, "d7");
  add_note_to_queue(Thread, "e7");
}

void SOUNDS::play_gear_park(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "c6");
  add_note_to_queue(Thread, "c6");
  add_note_to_queue(Thread, "c6");
}
void SOUNDS::play_gear_reverse(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "d6");
  add_note_to_queue(Thread, "d6");
}
void SOUNDS::play_gear_neutral(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "e6");
}
void SOUNDS::play_gear_drive(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "f6");
  add_note_to_queue(Thread, "f6");
}
void SOUNDS::play_gear_low(COMMAND_THREAD &Thread)
{
  add_note_to_queue(Thread, "g7");
  add_note_to_queue(Thread, "g6");
  add_note_to_queue(Thread, "f6");
}

// -------------------------------------------------------------------------------------


#endif