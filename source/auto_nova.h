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

#include "helper.h"
//#include "system.h"
#include "fled_time.h"
//#include "stringthings.h"
//#include "alert_system_2.h"

using namespace std;

// -------------------------------------------------------------------------------------
//  Nova Class

/*
Chances of snagging a fresh ride, nova-hot style, are flatline to zero. Here’s the 
lowdown: 

On one side, you got Uber ops tanking to braindead levels. Instance, some church crew 
running a scam with hospital insurance creds, to push rides without perks, payoffs, or 
even a whisper of thanks or stars. Sum forces the wheel runner to ditch the sick and 
dying at the med center’s edge, 'cause let’s be real, even an archangel would ghost the 
roach-ridden grime and sob stories, 'cause 3 eddies and 3 ennys won't score you 
decent care.

Flip the script, and you got the corpo-rats encrypting clept surge chits in volatile 
memory, making it a free-for-all for any corner dancing jack with a burner to swipe 
and bolt. Say no to being their mule or selling out, and they hit where it 
hurts—friends and kin. 

The gonk mafia thinks bleeding choombas dry is smart biz. But creds are slipping 
through fingers, turning down sold solid fares from decent folks just trying to grind, 
all 'cause the risk’s too high, like you’re being set up for a flatline by some vet 
who’s hit rock bottom.

You want that high-octane digital bling for your GT5? Then you better drop the benjis 
in my palm and quit the lies. Words are cheap. Calc the net. 'cause, if you’re 
looking for more with less, my tarnished chrome arm's all you’re gonna get.

It’d be pure titanium if I had some edge to cut through this rotting reality. But for 
now, I’m in the dark. I’ll keep slicing off those blood-sucking leeche rings, and keep 
jumping shadows and fog. Staying off the grid, that’s my play—until I find a way to 
jack out of this decaying dance.
*/

struct NOVA_BITS_VALUE
{
  public:

  int ID = 0;
  bool NOVA_BITS[64] = {0};
  TIMED_PING FREQUENCY[64];
  TIMED_PING HILIGHT[64];

  bool DETAILS = false;
};

class NOVA_ITEM
{
  private:

  void set_bits(unsigned long tmeFrame_Time, unsigned char Byte_Num, int &Bit_Num);

  public:
  
  NOVA_BITS_VALUE NOVA_VALUE;

  void store_value(unsigned long tmeFrame_Time, int Id, 
                    unsigned char Byte_0, unsigned char Byte_1, 
                    unsigned char Byte_2, unsigned char Byte_3, 
                    unsigned char Byte_4, unsigned char Byte_5, 
                    unsigned char Byte_6, unsigned char Byte_7);
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

  void process(unsigned long tmeFrame_Time, int Pid, int Message[]);
  // Parse the message into Nova format.
  // Message should be only dim 8 in size.
  
};

#endif