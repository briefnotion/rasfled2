// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_advertisement.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************

#ifndef WIDGETS_ADVERTISEMENT_H
#define WIDGETS_ADVERTISEMENT_H

// Standard Header Files
//#include <fstream>
//#include <iostream>
#include <string>
//#include <time.h>
#include <vector>

// Rasfled Includes
#include "system.h"
#include "rasapi.h"
#include "fled_time.h"
#include "widgets.h"
#include "widgets_drawing.h"
#include "screen4_helper.h"

// -------------------------------------------------------------------------------------

class ADVERTISEMENT_SCREENS
{
  private:

  TIMED_PING PING_NEXT_AD;

  int ADVERT_POS = -1;

  vector<string> AD_IMAGE_LIST;
  //TEXTURE_IMAGE ADVERT_IMAGE;
  vector<TEXTURE_IMAGE> ADVERT_IMAGES;

  bool ACTIVE = false;

  int STOP_PAUSE_PLAY = 1;
  // -1 Stop
  //  0 Pause
  //  1 Play

  // SCAN
  int SCAN_LINE = 0;

  bool CHANGED = true;

  public:

  void stop();
  void pause();
  void play(system_data &sdSysData);

  void create(system_data &sdSysData);
  // Loads Advertisement List
  // Sets ACTIVE if sucessful.

  void draw(system_data &sdSysData);

};

// -------------------------------------------------------------------------------------

#endif