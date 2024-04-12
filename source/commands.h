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

  string COMMAND_STR = "";
  TIMED_PING DELAY;
  int INSTANCE_ID = -1;
  bool DELETE_FLAG = true;
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

  vector<COMMAND_STRING> PENDING_COMMANDS;

  COMMAND_T COMMAND_THREADS[DEF_COMMANDS_MAX];

  public:

  void run_command(string Command, unsigned long current_time_frame, 
                    int one_instance_slot, unsigned long delay);
  // Run command Command at next execute cycle,
  //  one_instance_slot: an arbitrary user defined id, e.g. 0 for "aplay", 
  //    for commands that fail when running more than one instance at the 
  //      same time.
  //    In the case of two of the same instance is being called, if the command 
  //      has not already started, the most recent will replace the previous.
  //    if value is -1, instance id is not observed.
  //    if command with the same instance has already started, it will likely
  //      try to run.
  //  delay is the amout of time in ms to wait to execute the command.
  void run_command(string Command);
  //  Run command immediatly, no instance or delay checks.

  void execute(unsigned long current_time_frame);
  //  Run whatever commands are queued in the Pending Commands list.

};


#endif