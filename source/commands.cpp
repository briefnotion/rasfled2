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

void COMMAND_THREAD::run_command(string Command, unsigned long current_time_frame, 
                                  int one_instance_slot, unsigned long delay)
{
  if (one_instance_slot == -1)
  {
    COMMAND_STRING tmp_command;

    tmp_command.COMMAND_STR = Command;
    tmp_command.DELAY.ping_up(current_time_frame, delay);
    tmp_command.INSTANCE_ID = one_instance_slot;
    tmp_command.DELETE_FLAG = false;

    PENDING_COMMANDS.push_back(tmp_command);
  }
  else
  {
    // search for instances of the same
    int instance_location = -1;
    
    if (PENDING_COMMANDS.size() != 0)
    {
      for (int pos = 0; (pos < (int)PENDING_COMMANDS.size()) && (instance_location == -1); pos++)
      {
        if (PENDING_COMMANDS[pos].INSTANCE_ID == one_instance_slot)
        {
          instance_location = pos;
        }
      }
    }

    if (instance_location == -1)
    {
      COMMAND_STRING tmp_command;

      tmp_command.COMMAND_STR = Command;
      tmp_command.DELAY.ping_up(current_time_frame, delay);
      tmp_command.INSTANCE_ID = one_instance_slot;
      tmp_command.DELETE_FLAG = false;

      PENDING_COMMANDS.push_back(tmp_command);
    }
    else
    {
      PENDING_COMMANDS[instance_location].COMMAND_STR = Command;
      PENDING_COMMANDS[instance_location].DELAY.ping_up(current_time_frame, delay);
      PENDING_COMMANDS[instance_location].INSTANCE_ID = one_instance_slot;
      PENDING_COMMANDS[instance_location].DELETE_FLAG = false;
    }
  }
}

void COMMAND_THREAD::run_command(string Command)
{
  run_command(Command, 0, -1, 0);
}

void COMMAND_THREAD::execute(unsigned long current_time_frame)
{
  // Check to execute commands
  if (PENDING_COMMANDS.size() > 0)
  {
    for (int command = 0; command < (int)PENDING_COMMANDS.size(); command++)
    {
      // check time
      if (PENDING_COMMANDS[command].DELAY.ping_down(current_time_frame) == false ||
          PENDING_COMMANDS[command].DELAY.start_time() == 0)
      {
        // find vacant slot
        int vacant_slot = -1;

        for (int thread = 0; (thread < DEF_COMMANDS_MAX) && (vacant_slot == -1); thread++)
        {
          if (COMMAND_THREADS[thread].running() == false)
          {
            vacant_slot = thread;
          }
        }

        if (vacant_slot != -1)
        {
          COMMAND_THREADS[vacant_slot].run_command(PENDING_COMMANDS[command].COMMAND_STR);
        }

        PENDING_COMMANDS[command].DELETE_FLAG = true;
      }
    }
    
    // clear called commands from vector
    for (int pos = (int)PENDING_COMMANDS.size() -1; pos >= 0; pos--)
    {
      if (PENDING_COMMANDS[pos].DELETE_FLAG)
      {
        PENDING_COMMANDS.erase(PENDING_COMMANDS.begin() + pos);
      }
    }
  }
}

#endif