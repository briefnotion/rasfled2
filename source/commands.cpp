// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: commands.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef COMMANDS_CPP
#define COMMANDS_CPP

#include "commands.h"

using namespace std;

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

// Source: https://raspberry-projects.com/pi/programming-in-c/console/using-console-commands-in-code

void COMMANDS::shutdown_now()
// Shut down the entire system.
{
  system(SHUTDOWN_NOW.c_str());
}

void COMMANDS::reboot()
// Shut down the entire system.
{
  system(REBOOT.c_str());
}

bool COMMAND_T::running()
{
  if(THREAD_RUNNING == true)
  // Check to see if output thread was started before checking the completion status.
  {
    if(THREAD.wait_for(0ms) == future_status::ready)
    // Check to verify thte thread is complete before allowing the console to be updated again. 
    {
      THREAD_RUNNING = false;
    }
  }
  
  return THREAD_RUNNING;
}

void COMMAND_T::run_command(string Command)
{
  COMMAND = Command;

  if (running() == false)
  {
    THREAD = async(system, COMMAND.c_str());
    THREAD_RUNNING = true;
  }
}

void COMMAND_THREAD::run_command(string Command, int one_instance_slot)
{
  if (one_instance_slot == -1)
  {
    // find first empty slot above 0

    int empty_slot = -1;

    for (int slot = 1; slot < DEF_COMMANDS_MAX && empty_slot != -1; slot++)
    {
      if (PENDING_COMMANDS[slot].COMMAND_STR.size() == 0)
      {
        empty_slot = slot;
      }
    }

    if (empty_slot == -1)
    {
      // no empty slots found
      // maybe handle this
    }
    else
    {
      // define a max size - temp solution
      if (PENDING_COMMANDS[one_instance_slot].COMMAND_STR.size() < 20)
      {
        PENDING_COMMANDS[one_instance_slot].COMMAND_STR.push_back(Command);
      }
    }
  }
  else
  {
    // put in slot asked for, including 0 if necessary.

    if (one_instance_slot < DEF_COMMANDS_MAX)
    {
      // define a max size - temp solution
      if (PENDING_COMMANDS[one_instance_slot].COMMAND_STR.size() < 20)
      {
        PENDING_COMMANDS[one_instance_slot].COMMAND_STR.push_back(Command);
      }
    }
  }
}

void COMMAND_THREAD::run_command(string Command)
{
  run_command(Command, -1);
}

void COMMAND_THREAD::execute()
{
  // step through theads to see if command is pending
  for (int thread_slot = 0; thread_slot < DEF_COMMANDS_MAX; thread_slot++)
  {
    // execute command if pending and thread not busy
    if (PENDING_COMMANDS[thread_slot].COMMAND_STR.size() > 0)
    {
      if (COMMAND_THREADS[thread_slot].running() == false)
      {
        // run the command and delete it from the queue
        COMMAND_THREADS[thread_slot].run_command(PENDING_COMMANDS[thread_slot].COMMAND_STR[0]);
        PENDING_COMMANDS[thread_slot].COMMAND_STR.erase(PENDING_COMMANDS[thread_slot].COMMAND_STR.begin());
      }
    }
  }
}

#endif