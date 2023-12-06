// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: auto_nova.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTO_NOVA_H
#define AUTO_NOVA_H

// Standard Header Files
#include <stdio.h>
//#include <string>
#include <vector>

//#include "fled_time.h"
//#include "stringthings.h"
//#include "helper.h"
//#include "alert_system_2.h"
//#include "screen4_helper.h"

//#include "comport.h"

using namespace std;

// -------------------------------------------------------------------------------------
//  Nova Class

class NOVA_ITEM
{
  public:

  int value = 0;
};

struct NOVA_MAP_ITEM
{
  int item = 0;
  int location = 0;
};

class AUTOMOBILE_NOVA
{
  private:

  vector <NOVA_MAP_ITEM> NOVA_MAP;

  int find_location(int Item);

  public:
  
  vector <NOVA_ITEM> NOVA_ITEMS;

  bool ENABLED = false;

  void process(int Pid, int Message[]);
  // Parse the message into Nova format.
  // Message should be only dim 8 in size.
  

};

#endif