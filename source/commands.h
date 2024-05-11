// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: commands.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef COMMANDS_H
#define COMMANDS_H

#define DEF_COMMANDS_MAX  15

// Standard Header Files
#include <stdio.h>
#include <string>
#include <future>
#include <vector>
//#include <deque>

// Rasfled includes
#include "fled_time.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

// Source: https://raspberry-projects.com/pi/programming-in-c/console/using-console-commands-in-code

class COMMANDS
// Commands that can be executed as if from a shell prompt.
{
  private:
  string SHUTDOWN_NOW = "sudo shutdown now";
  //string SHUTDOWN_NOW = "sudo shutdown -h -t 5";
                    //  "sleep 5 && sudo shutdown now &"

  string REBOOT = "sudo reboot";
                    //  "sleep 5 && sudo shutdown now &"

  public:

  void shutdown_now();

  void reboot();
};

class COMMAND_STRING
{
  public: 

  vector<string> COMMAND_STR;
};

class COMMAND_T
{
  private:

  future<int> THREAD;
  bool THREAD_RUNNING = false;

  string COMMAND;

  public:

  bool running();

  void run_command(string Command);
};

class COMMAND_THREAD
{
  private:

  string PLAY_COMMAND = "aplay -q";

  COMMAND_STRING PENDING_COMMANDS[DEF_COMMANDS_MAX];
  COMMAND_T COMMAND_THREADS[DEF_COMMANDS_MAX];

  void push_back_command(string Command, int Thread_Slot);

  public:

  // reserve slot 0 for sounds.

  void run_command(string Command, int one_instance_slot);
  // Run command Command at next execute cycle,
  //  one_instance_slot: an arbitrary user defined id, e.g. 0 for "aplay", 
  //  commands are qued in, so two commands in same slot will execute after 
  //    the first has completed.
  void run_command(string Command);
  //  Run command immediatly, no instance or delay checks.

  void que_command(string Command, unsigned long current_time_frame, 
                    int one_instance_slot, unsigned long delay);

  void execute();
  //  Run whatever commands are queued in the Pending Commands list.

};


#endif