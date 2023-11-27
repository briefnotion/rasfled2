// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: interface.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef INTERFACE_CPP
#define INTERFACE_CPP

#include "interface.h"

// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

void command_desc(CONSOLE_COMMUNICATION &cons, string strDescription)
{
  //cons.printw(linemerge_left_justify("________________________________________________", 
  //                                      "__" + strDescription));
  cons.printw("  " + strDescription);
}

// -------------------------------------------------------------------------------------
// CONSOLE_COMMUNICATION Commands

// -------------------------------------------------------------------------------------
// Procedures related to the Command Line.

// Display the help screen.
void consoleprinthelp(CONSOLE_COMMUNICATION &cons)
{
  cons.printw("");
  cons.printw("HELP SCREEN -------------------------------");
  cons.printw("");
  cons.printw("'X' or 'exit' - Safely exits the RasFLED.");
  cons.printw("' restart'    - Restart RasFLED.");
  cons.printw("");
  cons.printw("'help'    - Prints this help screen.");
  cons.printw("' events' - Prints all active events.");
  cons.printw("' config' - Prints some configuration data.");
  cons.printw("");
  cons.printw("     hh - Hazard Lights      '  ' - Start Timer");
  cons.printw("     h` - Hazard Lights Off  ' '' - End Timer");
  cons.printw("");
  cons.printw("     `` - End Most Repeating Lights");
  cons.printw("");
  cons.printw(" dd - Toggle Day/Night Mode");
  cons.printw("");
  cons.printw("Colors:");
  cons.printw(" r - Red    u - Purple  n - Orange");
  cons.printw(" g - Green  y - Yellow  w - White");
  cons.printw(" b - Blue   c - Cyan    ` - End");
  cons.printw("");
  cons.printw("rX  - Set Running Color  (X is the color)");
  cons.printw("");
  cons.printw("pX  - Pulse Anim         (X is the color)");
  cons.printw("oX  - Overhead Anim      (X is the color)");
  cons.printw("fX  - Flash Anim      (X is the color)");
  cons.printw("X`  - End Command (X is the Animation)");
  cons.printw("  Double animations will be with Running Color.");
  cons.printw("");
  //cons.printw("  Not all colors implemented for all commands.");
  //cons.printw("");
  cons.printw("'\' - Turn on and off diagnosis mode.");
  cons.printw("t - Cycle Strips  c - Test LEDs   a - not specified");
  cons.printw("");

  cons.printw("SERIAL PORT COMMANDS ----------------------");
  cons.printw("");
  cons.printw("' commo' - Start Serial Communications Recording");
  cons.printw("' commf' - Start Serial Communications Recording");
  cons.printw("");
  cons.printw("']]' - Flash Recording. Store most recent communications");
  cons.printw("");
  cons.printw("Commands are sent to the serial port by preceeding with 'q'");
  cons.printw("    Format: 'qXYZ' + ENTER - send 'XYZ' to serial");
  cons.printw("");
  cons.printw("'qp' - Toggle pause and unpause communications");
  cons.printw("'qt' - Toggle test data");
  cons.printw("'qf' - Toggle send only KNOWN PIDs or ALL PIDs");
  cons.printw("");
  cons.printw("'qv3' - Swithch to v3 system");
  cons.printw("'qv4' - Swithch to v4 system");
  cons.printw("");
  cons.printw("PID are sent in same format if prepared");
  cons.printw("  e.g. 'q0B' for 'PID_INTAKE_MANIFOLD_ABS_PRESSURE request'");
}

// Display all running events.
void consoleprintevents(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, ANIMATION_HANDLER &Animations)
{
  cons.printw("Channel Count:" + std::to_string(sdSysData.CONFIG.iNUM_CHANNELS));
  for (int channel = 0; channel < sdSysData.CONFIG.iNUM_CHANNELS; channel++)
  {
    cons.printw("Channel " + std::to_string(channel));
    if (Animations.EVENTS[channel].teDATA.size() == 0)
    {
      cons.printw("  No Events");
    }
    else
    {
      for (int event = 0; event < (int)Animations.EVENTS[channel].teDATA.size(); event++)
      {
        cons.printw("  C" + to_string(channel)
                        + " ID:\"" + Animations.EVENTS[channel].teDATA[event].strIdent
                        + "\" Anim:" + std::to_string(Animations.EVENTS[channel].teDATA[event].bytANIMATION)  
                        + " LEDanim:" + std::to_string(Animations.EVENTS[channel].teDATA[event].bytLEDANIMATION) 
                        + " Strt:" + std::to_string(Animations.EVENTS[channel].teDATA[event].intSTARTPOS) 
                        + " End:" + std::to_string(Animations.EVENTS[channel].teDATA[event].intENDPOS));
      }
    }
  }
}

// Display all running events.
void consoleprintconfig(CONSOLE_COMMUNICATION &cons, system_data &sdSysData)
{
  cons.printw("Configuration");
  cons.printw("");

  cons.printw("Switch Pin Information");
  cons.printw("  Number of Switches - " + to_string(sdSysData.CONFIG.iNUM_SWITCHES));
  for(int x=0; x<sdSysData.CONFIG.iNUM_SWITCHES; x++)
  {
    cons.printw("    Switch Id (" + to_string(x) + ") - " + to_string(sdSysData.CONFIG.vSWITCH_PIN_MAP.at(x).pin));
  }
  cons.printw("");

  cons.printw("LED Information and Strtip Order");
  cons.printw("  Unit(" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).intID) + "): "
                  + sdSysData.CONFIG.LED_MAIN.at(0).strNAME);
  cons.printw("  Number of Groups - " + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.size()));
  cons.printw("  LED Count - " + to_string(sdSysData.CONFIG.LED_MAIN.at(0).led_count()));


  for(int group=0; group<sdSysData.CONFIG.LED_MAIN.at(0).g_size(); group++)
  {
    cons.printw("    Group(" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).intID) + "): "
                    + sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).strNAME
                    + "   Status: \"" + sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).Status + "\"");
    cons.printw("    Number of LED Strips - " + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.size()));
    cons.printw("    LED Count:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).led_count()));

    for(int strip=0; strip<sdSysData.CONFIG.LED_MAIN.at(0).s_size(group); strip++)
    {
      cons.printw("      C" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).intCHANNEL) 
                      + " G" + to_string(group) + " S" + to_string(strip) 
                      + "  Strip(" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).intID) + "): "
                      + sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).strNAME + " "
                      + sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).strPOSITION );

      cons.printw("        LED: Count:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count()) 
                      + "   Start:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).fs(0))
                      + "   End:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).fe(0) )
                      + "   forward:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).booFORWARD)
                      + "   standing:" + to_string(sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).booBOT_AT_START)
                      + "   Status: \"" + sdSysData.CONFIG.LED_MAIN.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).Status + "\"");
    }
  }
}

// -------------------------------------------------------------------------------------

void process_power_animation(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations, CRGB cRGBpulsecolor)
{
  sdSysData.TMP_PASSTHRU_COLOR = cRGBpulsecolor;
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "POWER");
  sdSysData.booPulsesRunning = true;
}

// -------------------------------------------------------------------------------------
// Pulses

// Set To End All Pulses
void processcommandpulseend(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "PULSE COLOR STOP");
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "COUNTDOWN TIMER STOP");
  sdSysData.booPulsesRunning = false;
}

// Flash Color All Channels
void processcommandflash(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "FLASH");
}

// Pulse Color All Channels
void processcommandpulse(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations, CRGB cRGBpulsecolor)
{
  sdSysData.PULSE_COLOR = cRGBpulsecolor;
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "PULSE COLOR");
  sdSysData.booPulsesRunning = true;
}

// Pulse Color All Channels
void processcommandpulsecountdown(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "COUNTDOWN TIMER");
  sdSysData.booPulsesRunning = true;
}

// -------------------------------------------------------------------------------------
// Overhead Illum

// Set To End All Overhead Illumination
void processcommandoverheadillumend(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "All Doors Closed - Overhead");
  sdSysData.booOverheadRunning = false;
}

// Overhead Illumination Color
void processcommandoverheadillum(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "A Door Opened - Overhead");
  sdSysData.booOverheadRunning = true;
}

// -------------------------------------------------------------------------------------
// Hazard

// Set To End All Hazard
void processcommandhazardend(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "HAZARD STOP");
  sdSysData.booHazardRunning = false;
}

// Hazard
void processcommandhazard(system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "HAZARD");
  sdSysData.booHazardRunning = true;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

bool check_command(CONSOLE_COMMUNICATION &cons, string command, string description)
// Check the cons command with the parameter command
//  If matched, print command entered, description, and clears cons.command.
//  Only print description if one is provideded, not equal to "".
//  Else, just return false.
{
  if (cons.command_text_get() == command)
  {
    cons.printw(linemerge_left_justify("________________________________________________", 
                    "CMD: " + cons.command_text_get()));

    if (description == "")
    {
      // do nothing
    }
    else
    {
      command_desc(cons, description);
    }

    cons.command_text_clear();
    return true;
  }
  else
  {
    return false;
  }

}

bool check_command_with_num_param(CONSOLE_COMMUNICATION &cons, string command, string description, int num_start, int num_len, int &Parameter)
{
  bool boo_return = false;
  if (cons.command_text_get().length() == command.length())
  {
    if(cons.command_text_get().substr(0,num_start) == command.substr(0,num_start))
    {
      try
      {
        Parameter = stoi(cons.command_text_get().substr(num_start, num_len));
        boo_return = check_command(cons, cons.command_text_get(), description);
      }
      catch(const std::exception& e)
      {
        cons.command_text_clear();
      }
    }
  }

  return boo_return;  
}

void run_test(CONSOLE_COMMUNICATION &cons)
{
  cons.printw("Test ...");

  cons.printw("------------------");
  cons.printw("no test code prepared.");
  cons.printw("------------------");
}

void process_slow_commands(CONSOLE_COMMUNICATION &cons, system_data &sdSysData)
  {
    if (cons.command_text_get().size() > 1)
    {
      // Check for comm commands
      if (cons.command_text_get()[0] == 'q')
      {
        string comm_command = cons.command_text_get().substr(1, cons.command_text_get().size()-1);
        sdSysData.COMMS.send(comm_command);
      }
    }
  }

void processcommandlineinput(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, 
              unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  COMMANDS command;
  //int parameter = 0;
          
  if(cons.command_text_changed() == true)
  {
    // Carrage Return
    if (cons.carrage_return_get() == true)
    {
      process_slow_commands(cons, sdSysData);

      // Clear field regardless of command sent
      cons.command_text_clear();
    }
    else
    {
      // Process Quick Commands

      // Color Palettes
      CRGB crgbWhite  = CRGB(32,32,32); // W
      CRGB crgbRed    = CRGB(64,0,0);   // R
      CRGB crgbGreen  = CRGB(0,64,0);   // G
      CRGB crgbBlue   = CRGB(0,0,64);   // B

      CRGB crgbPurple = CRGB(32,0,64);  // U
      CRGB crgbYellow = CRGB(48,48,0);  // Y
      CRGB crgbCyan   = CRGB(0,48,48);  // C

      CRGB crgbOrange = CRGB(64,16,0);  // N

      // Call routines that match the info on the command line.

      // Start Comm Port
      if (check_command(cons," startcomm", "Start Comms Port"))
      {

        if (sdSysData.COMMS.create() == true)
        {
          cons.printw("Comm Port Started.");
        }
        else
        {
          cons.printw("Comm Port Failed To Start.");
        }
      }
      
      // Stop Comm Port
      if (check_command(cons," stopcomm", "Start Comms Port"))
      {
        sdSysData.COMMS.close_port();
        cons.printw("Comm Port Stop Command Sent.");
      }
      
      if (check_command(cons," commo", "Start Comms Log"))
      {
        sdSysData.COMMS.log_file_on();
        sdSysData.COMMS.send("-- STARTING COMMS LOG --");
      }

      if (check_command(cons," commf", "Stop Comms Log"))
      {
        sdSysData.COMMS.log_file_off();
        sdSysData.COMMS.send("-- STOPPING COMMS LOG --");
      }

      if (check_command(cons,"]]", "Store Comms Flash Data"))
      {
        sdSysData.COMMS.write_flash_data();
      }

      // Test Routine
      if(cons.command_text_get() == " test")
      {
        // Keep values below 128
        cons.printw("CMD: " + cons.command_text_get());
        run_test(cons);
        cons.command_text_clear();
      }

      if (check_command(cons," animt", "Event System 2 Test Animation"))
      {
        Animations.call_animation(sdSysData, tmeCurrentTime, "Car", "test1");
      }
      
      // Program Exit
      if (check_command(cons,"X", "Program Exit") || check_command(cons, "exit", "Program Exit"))
      {
        cons.WINDOW_CLOSE = true;
      }

      // Program Restart
      if (check_command(cons, " restart", "Restart Program"))
      {
        sdSysData.booREBOOT = true;
        cons.WINDOW_CLOSE = true;
      }

      // print help
      if (check_command(cons, "help", "Help and Instructions"))
      {
        consoleprinthelp(cons);
      }

      // print event list
      if (check_command(cons, " events", "All Currently Running Events"))
      {
        consoleprintevents(cons, sdSysData, Animations);
      }

      // print configuration data
      if (check_command(cons, " config", "Current Configuration and Settings"))
      {
        //consoleprintconfig(cons, sdSysData, Animations);
        consoleprintconfig(cons, sdSysData);
      }

      // End All Extra Repeating Lights and Countdown Timer
      if (check_command(cons, "``", "End Most Pulse Animations"))
      {
        // end Countdown Timer
        sdSysData.cdTIMER.end();

        processcommandpulseend(sdSysData, tmeCurrentTime, Animations);
        processcommandoverheadillumend(sdSysData, tmeCurrentTime, Animations);
        processcommandhazardend(sdSysData, tmeCurrentTime, Animations);
      }

      // -------------------------------------------------------------------------------------
      // Animations Load or Reload

      if (check_command(cons, " animld", "Reload Animations"))
      {
        string Working_Directory = FILES_DIRECTORY;
        string Animations_Library_JSON = FILES_ANIMATIONS;
        string Running_State_Filename = Working_Directory + FILES_RUNNING_STATE_SAVE;

        if (Animations.load_collections(Working_Directory, Animations_Library_JSON) == true)
        {
          cons.printw("  Animations file loaded.");
        }
        else
        {
          cons.printw("    Animations file not loaded.");
        }
      }

      // -------------------------------------------------------------------------------------
      // DAY NIGHT MODE

      // Toggle Day Night
      if (check_command(cons, "dd", ""))
      {
        if (sdSysData.Day_On == true)
        {
          command_desc(cons, "Toggle DAY mode OFF.");
          sdSysData.Day_On = false;
        }
        else
        {
          command_desc(cons, "Toggle DAY mode ON.");
          sdSysData.Day_On = true;
        }
      }

      // Toggle Lights Off
      if (check_command(cons, " lightsoff", "Lights Off."))
      {
        sdSysData.Lights_On.set(false);
      }

      // Toggle Lights On
      if (check_command(cons, " lightson", "Lights ON."))
      {
        sdSysData.Lights_On.set(true);
      }

      // Toggle Day Mode On
      if (check_command(cons, "dayon", "DAY mode ON."))
      {
        sdSysData.Day_On = true;
      }

      // Toggle Day Mode Off
      if (check_command(cons, "dayoff", "DAY mode OFF."))
      {
        sdSysData.Day_On = false;
      }
      
      // -------------------------------------------------------------------------------------
      // TERMINAL COMMANDS
  
      // Command Line (sudo shutdown now)
      if (check_command(cons, " comshutd", "Shutdown Started"))
      {
        command_desc(cons, "Shutting System Down.");
        
        // Call Shutdown command.
        command.shutdown_now();

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));

        // Set system to exit
        cons.WINDOW_CLOSE = true;
      }

      // Command Line (sudo reboot)
      if (check_command(cons, " reboot", "Reboot Started"))
      {
        command_desc(cons, "Rebooting System.");
        
        // Call Shutdown command.
        command.reboot();

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));

        // Set system to exit
        cons.WINDOW_CLOSE = true;
      }

      // -------------------------------------------------------------------------------------
      // Threadable Commands with from command prompt   

      // Radio -------------
      // Command Line (load air_m)
      if (check_command(cons, " airstop", "Airband Stop"))
      { 
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_airstop.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      // Command Line (load air_m)
      if (check_command(cons, " lafm", "Airband Lafayette Multi"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_lafm.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      // Radio Call Airband Lafayette Scan Script Command
      if (check_command(cons, " lafs", "Airband Lafayette Scan"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_lafs.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      // Radio Call CB Channel Scan Script Command
      if (check_command(cons, " cbs", "CB Channel Scan"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_cbs.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      // Radio Call NOAA Channel Scan Script Command
      if (check_command(cons, " rnoaa", "NOAA Channel Scan"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_noaa.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      // Radio Call Emergency Channel Scan Script Command
      if (check_command(cons, " remergenc", "Emergency Channel Scan"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/rtlsdr/ras_emergency.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(25, 0, 0));
      }

      /*
      // -------------------------------------------------------------------------------------
      // For Deguging Gadgets
      // Debug Counter On
      if (check_command(cons, " debugcon", "Debug Counter On"))
      {
        sdSysData.DEBUG_COUNTER = true;
      }

      // Debug Counter Off
      if (check_command(cons, " debugcoff", "Debug Counter On"))
      {
        sdSysData.DEBUG_COUNTER = false;
      }
      */

      // -------------------------------------------------------------------------------------
      // ABS-B -------------
      // Command Line (load fastart.sh)
      if (check_command(cons, " adsbon", "ABS-B On"))
      { 
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/flightaware/fastart.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(15, 15, 0));
      }

      // Command Line (load fastop.sh)
      if (check_command(cons, " adsboff", "ABS-B Off"))
      {
        // Call command.
        sdSysData.Command_Thread.run_command("/home/pi/flightaware/fastop.sh");

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(15, 15, 0));
      }
      
      // Command Line (load copy direcory)
      if (check_command(cons, " adsbsnap", "ADS-B Snapshot"))
      {
        // Call command.
        //cons.printw("cp -r /run/dump1090-fa/ ~/flightaware/snapshot." + to_string(tmeCurrentTime));
        sdSysData.Command_Thread.run_command("cp -r /run/dump1090-fa/ /home/pi/flightaware/snapshot." + sdSysData.AIRCRAFT_COORD.DATA.NOW.get_str_value());

        // Start Power Down Animation
        process_power_animation(sdSysData, tmeCurrentTime, Animations, CRGB(0, 30, 0));
      }
      // -------------------------------------------------------------------------------------
      // PLAYLIST

      /*
      // Print out the current playlist.
      if (check_command(cons, " playlist", "Current Playlist:"))
      {
        for (int x=0; x < cons.the_player.Play_List.size(); x++)
        {
          cons.printw("  " + cons.the_player.Play_List.MOVIE_LIST[x]);
        }
      }

      // Skip to the next film in the playlist.
      if (check_command(cons, " skip", "Play Next in Playlist:"))
      {
        cons.the_player.booSkip = true;
      }
      */

      // -------------------------------------------------------------------------------------
      // FLASH

      // flash Running
      if (check_command(cons, "ff", "Flash All LEDs with Running Color"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash White
      if (check_command(cons, "fw", "Flash White All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash Red
      if (check_command(cons, "fr", "Flash Red All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash Green
      if (check_command(cons, "fg", "Flash Green All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash Blue
      if (check_command(cons, "fb", "Flash Blue All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash Purple
      if (check_command(cons, "fu", "Flash Purple All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }
    
      // flash Yellow
      if (check_command(cons, "fy", "Flash Yellow All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }
      
      // flash Cyan
      if (check_command(cons, "fc", "Flash Cyan All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // flash Orange
      if (check_command(cons, "fn", "Flash Orange All LEDs"))
      {
        processcommandflash(sdSysData, tmeCurrentTime, Animations);
      }

      // -------------------------------------------------------------------------------------
      // PULSES

      // pulse end
      if (check_command(cons, "p`", "End Most Pulse Animations"))
      {
        processcommandpulseend(sdSysData, tmeCurrentTime, Animations);
      }

      // pulse Running Timer
      if (check_command(cons, "  ", "5 minute Pulse Timer Started with Running Color"))
      {
        sdSysData.start_timer(DEFAULTTIMER * 60);
        processcommandpulsecountdown(sdSysData, tmeCurrentTime, Animations);
      }

      // pulse Running Timer end
      if (check_command(cons, " `", "Pulse Timer Stop"))
      {
        sdSysData.cdTIMER.end();
        processcommandpulseend(sdSysData, tmeCurrentTime, Animations);
      }

      // pulse Running Color
      if (check_command(cons, "pp", "Pulse Running Color All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, sdSysData.get_running_color());
      }

      // pulse White
      if (check_command(cons, "pw", "Pulse White All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbWhite);
      }

      // pulse Red
      if (check_command(cons, "pr", "Pulse Red All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbRed);
      }

      // pulse Green
      if (check_command(cons, "pg", "Pulse Green All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbGreen);
      }

      // pulse Blue
      if (check_command(cons, "pb", "Pulse Blue All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbBlue);
      }

      // pulse Purple
      if (check_command(cons, "pu", "Pulse Purple All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbPurple);
      }

      // pulse Yellow
      if (check_command(cons, "py", "Pulse Yellow All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbYellow);
      }

      // pulse Cyan
      if (check_command(cons, "pc", "Pulse Cyan All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbCyan);
      }

      // pulse Orange
      if (check_command(cons, "pn", "Pulse Orange All LEDs"))
      {
        processcommandpulse(sdSysData, tmeCurrentTime, Animations, crgbOrange);
      }

      // -------------------------------------------------------------------------------------
      // Overhead Illumination
      
      // pulse end overhead illum
      if (check_command(cons, "o`", "Turn Off Overhead Illumination Lights"))
      {
        processcommandoverheadillumend(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Running
      if (check_command(cons,"oo", "Turn On Overhead Illumination Lights with Running Color") || 
          check_command(cons,"zz", "Turn On Overhead Illumination Lights with Running Color"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead White
      if (check_command(cons, "ow", "Turn On White Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Red
      if (check_command(cons, "or", "Turn On Red Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Green
      if (check_command(cons, "og", "Turn On Green Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Blue
      if (check_command(cons, "ob", "Turn On Blue Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Purple
      if (check_command(cons, "ou", "Turn On Purple Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Yellow
      if (check_command(cons, "oy", "Turn On Yellow Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Cyan
      if (check_command(cons, "oc", "Turn On Cyan Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Overhead Orange
      if (check_command(cons, "on", "Turn On Orange Overhead Illumination Lights"))
      {
        processcommandoverheadillum(sdSysData, tmeCurrentTime, Animations);
      }

      // Set Running Color
      if (check_command(cons, "rw", "Set Running Color to White"))
      {
        sdSysData.set_running_color(crgbWhite, "White");
      }

      if (check_command(cons, "rr", "Set Running Color to Red"))
      {
        sdSysData.set_running_color(crgbRed, "Red");
      }

      if (check_command(cons, "rg", "Set Running Color to Green"))
      {
        sdSysData.set_running_color(crgbGreen, "Green");
      }

      if (check_command(cons, "rb", "Set Running Color to Blue"))
      {
        sdSysData.set_running_color(crgbBlue, "Blue");
      }

      if (check_command(cons, "ru", "Set Running Color to Purple"))
      {
        sdSysData.set_running_color(crgbPurple, "Purple");
      }

      if (check_command(cons, "ry", "Set Running Color to Yellow"))
      {
        sdSysData.set_running_color(crgbYellow, "Yellow");
      }

      if (check_command(cons, "rc", "Set Running Color to Cyan"))
      {
        sdSysData.set_running_color(crgbCyan, "Cyan");
      }

      if (check_command(cons, "rn", "Set Running Color to Orange"))
      {
        sdSysData.set_running_color(crgbOrange, "Orange");
      }

      // -------------------------------------------------------------------------------------
      // Hazard

      // Hazard illum end
      if (check_command(cons, "h`", "HAZARD LIGHTS OFF"))
      {
        processcommandhazardend(sdSysData, tmeCurrentTime, Animations);
      }

      // Hazard
      if (check_command(cons, "hh", "HAZARD LIGHTS ON ('h`' to turn off"))
      {
        processcommandhazard(sdSysData, tmeCurrentTime, Animations);
      }

      /*
      // -------------------------------------------------------------------------------------
      // Debug Characters only active when debug mode is on
      // debug
      if (check_command(cons, "/", "Debug mode On/Off Toggle"))
      {
        cons.keywatch.in(KEYDEBUG);
      }

      // Only accept debug keys if debug is on.
      if (cons.keywatch.getnoreset(KEYDEBUG) == 1)
      {
        // LED DOOR CYCLE
        if(cons.command_text_get()[0] == KEYLEDDRCYCL)
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in(KEYLEDDRCYCL);
          cons.command_text_clear();
        }

        // LED TEST toggle all lights on to static value.
        if(cons.command_text_get()[0] == KEYLEDTEST)
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in(KEYLEDTEST);
          cons.command_text_clear();
        }

        // Toggle door open or closed.
        if(cons.command_text_get()[0] == '1')
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in('1');
          cons.command_text_clear();
        }

        // Toggle door open or closed.
        if(cons.command_text_get()[0] == '2')
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in('2');
          cons.command_text_clear();
        }

        // Toggle door open or closed.
        if(cons.command_text_get()[0] == '3')
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in('3');
          cons.command_text_clear();
        }

        // Toggle door open or closed.
        if(cons.command_text_get()[0] == '4')
        {
          cons.printw("CMD: " + cons.command_text_get());
          cons.keywatch.in('4');
          cons.command_text_clear();
        }
      }

      // -------------------------------------------------------------------------------------
      // Turn on and off debug. Deactivate debug keys when off.
      // Store behavior values for debug info.
      if (cons.keywatch.pressed(KEYDEBUG) == true)
      {
        if (cons.keywatch.getnoreset(KEYDEBUG) == 0)
        {
          // Draw values for debug LED CYCLE through displayed range (all, Door #)
          cons.keywatch.Chars[KEYLEDDRCYCL].VALUE = 0;
          cons.keywatch.Chars[KEYLEDDRCYCL].ACTIVE = false;

          // Draw values for debug LED TEST toggle all lights on to static value.
          cons.keywatch.Chars[KEYLEDTEST].VALUE = 0;
          cons.keywatch.Chars[KEYLEDTEST].ACTIVE = false;

          // Draw values for toggle door open or closed.
          cons.keywatch.Chars['1'].VALUE = 0;
          cons.keywatch.Chars['1'].ACTIVE = false;
          
          // Draw values for toggle door open or closed.
          cons.keywatch.Chars['2'].VALUE = 0;
          cons.keywatch.Chars['2'].ACTIVE = false;

          // Draw values for toggle door open or closed.
          cons.keywatch.Chars['3'].VALUE = 0;
          cons.keywatch.Chars['3'].ACTIVE = false;

          // Draw values for toggle door open or closed.
          cons.keywatch.Chars['4'].VALUE = 0;
          cons.keywatch.Chars['4'].ACTIVE = false;
        }
        else
        {
          // Reset CONSOLE_COMMUNICATION debug values to default values if debug turned off.
          cons.keywatch.Chars[KEYLEDDRCYCL].ACTIVE = true;
          cons.keywatch.Chars[KEYLEDTEST].ACTIVE = true;
          cons.keywatch.Chars['1'].ACTIVE = true;
          cons.keywatch.Chars['2'].ACTIVE = true;
          cons.keywatch.Chars['3'].ACTIVE = true;
          cons.keywatch.Chars['4'].ACTIVE = true;
        }
      }
      */
    }
  }
}

// If a door is opened and DOORAWARE is on, we want to end these animations when the door
//  has been opened.
void extraanimationdoorcheck2(CONSOLE_COMMUNICATION &cons, system_data &sdSysData, unsigned long tmeCurrentTime, ANIMATION_HANDLER &Animations)
{
  if(sdSysData.cdTIMER.is_active() == true)
  {
    // Check for newly expired timer
    if (sdSysData.cdTIMER.is_triggered() == true)
    {
      processcommandflash(sdSysData, tmeCurrentTime, Animations);
    }

    if (sdSysData.intDoorsOpen > 0)
    {
      cons.printw(to_string(sdSysData.intDoorsOpen));
      // End pulses when door is opened and end countdown timer.
      sdSysData.cdTIMER.end();
      processcommandpulseend(sdSysData, tmeCurrentTime, Animations);
      //processcommandoverheadillumend(cons, sdSysData, tmeCurrentTime, Animations);

      // Check if Overhead Lights are on, then if a door is opened turn off button and turn off 
      //  illumination only if door is closed.
    }
  }
}




#endif