// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: auto_nova.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef AUTO_NOVA_CPP
#define AUTO_NOVA_CPP

#include "auto_nova.h"

// -------------------------------------------------------------------------------------
//  Nova Class

int AUTOMOBILE_NOVA::find_location(int Item)
{
  int ret_value = -1;

  for (int pos = 0; pos < ((int)NOVA_MAP.size()) && (ret_value == -1); pos++)
  {
    if (Item == NOVA_MAP[pos].item)
    {
      ret_value =  NOVA_MAP[pos].location;
    }
  }

  return ret_value;
}

void AUTOMOBILE_NOVA::process(int Pid, int Message[])
{
  if (ENABLED)
  {
    int location = find_location(Pid);

    // not found
    if (location == -1)
    {
      // Map
      NOVA_MAP_ITEM tmp_map_location;
      tmp_map_location.item = Pid;
      tmp_map_location.location = NOVA_ITEMS.size();
      
      NOVA_MAP.push_back(tmp_map_location);

      // Item
      NOVA_ITEM tmp_nova_item;

      tmp_nova_item.value = 1;

      NOVA_ITEMS.push_back(tmp_nova_item);
    }
    else
    // found
    {
      NOVA_ITEMS[location].value = 2;
    }
  }

  // dummy
  Message[0] = 0;

}


#endif