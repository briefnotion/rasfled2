// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: interface.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef INTERFACE_H
#define INTERFACE_H

// Standard Header Files
#include <string>

// RASFled related header files
#include "stringthings.h"
#include "helper.h"
#include "screen4_helper.h"
#include "commands.h"
#include "animations_handler.h"

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

void command_desc(CONSOLE_COMMUNICATION &cons, string strDescription);

// -------------------------------------------------------------------------------------
// Console Commands

// -------------------------------------------------------------------------------------
// Procedures related to the Command Line.

// Display the help screen.
void consoleprinthelp(CONSOLE_COMMUNICATION &cons);

// Display all running events.
void consoleprintevents(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, ANIMATION_HANDLER &Animations);

// Display all running events.
void consoleprintconfig(CONSOLE_COMMUNICATION &cons, system_data &sdSysData);

// -------------------------------------------------------------------------------------
void process_power_animation(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations, CRGB cRGBpulsecolor);

// -------------------------------------------------------------------------------------
// Pulses

// Set To End All Pulses
void processcommandpulseend(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// Flash Color All Channels
void processcommandflash(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// Pulse Color All Channels
void processcommandpulse(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations, CRGB cRGBpulsecolor);

// Pulse Color All Channels
void processcommandpulsecountdown(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// -------------------------------------------------------------------------------------
// Overhead Illum

// Set To End All Overhead Illumination
void processcommandoverheadillumend(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// -------------------------------------------------------------------------------------
// Overhead Illumination Color
void processcommandoverheadillum(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// -------------------------------------------------------------------------------------
// Hazard

// Set To End All Hazard
void processcommandhazardend(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// Hazard
void processcommandhazard(system_data &sdSysData, CONSOLE_COMMUNICATION &cons, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

bool check_command(CONSOLE_COMMUNICATION &cons, string command, string description);

bool check_command_with_num_param(CONSOLE_COMMUNICATION &cons, string command, string description, int num_start, int num_len, int &Parameter);

// Test Routine Area
void run_test(CONSOLE_COMMUNICATION &cons);

void process_slow_commands(CONSOLE_COMMUNICATION &cons, system_data &sdSysData);

// Process and call routines as entered on the command line.
void processcommandlineinput(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, 
            unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);

// If a door is opened and DOORAWARE is on, we want to end these animations when the door
//  has been opened.
void extraanimationdoorcheck2(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations);



#endif