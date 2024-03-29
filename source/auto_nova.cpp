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
//  Nova Item Class


void NOVA_ITEM::set_bits(unsigned long tmeFrame_Time, unsigned char Byte_Num, int &Bit_Num)
{
  for (int bit = 0; bit < 8; bit++)
  {
    if(set_bool_with_change_notify(bit_value(Byte_Num, 7 - bit), NOVA_VALUE.NOVA_BITS[Bit_Num]))
    {
      if(NOVA_VALUE.FREQUENCY[Bit_Num].ping_down(tmeFrame_Time))
      {
        NOVA_VALUE.FREQUENCY[Bit_Num].ping_up(tmeFrame_Time, 10000);
      }
      else
      {
        NOVA_VALUE.FREQUENCY[Bit_Num].ping_up(tmeFrame_Time, 10000);
        NOVA_VALUE.HILIGHT[Bit_Num].ping_up(tmeFrame_Time, 10000);
      }
    }

    Bit_Num++;
  }
}

void NOVA_ITEM::store_value(unsigned long tmeFrame_Time, int Id, 
                            unsigned char Byte_0, unsigned char Byte_1, 
                            unsigned char Byte_2, unsigned char Byte_3, 
                            unsigned char Byte_4, unsigned char Byte_5, 
                            unsigned char Byte_6, unsigned char Byte_7)
{
  NOVA_VALUE.ID = Id;

  NOVA_VALUE.NOVA_BYTES[0] = Byte_0;
  NOVA_VALUE.NOVA_BYTES[1] = Byte_1;
  NOVA_VALUE.NOVA_BYTES[2] = Byte_2;
  NOVA_VALUE.NOVA_BYTES[3] = Byte_3;
  NOVA_VALUE.NOVA_BYTES[4] = Byte_4;
  NOVA_VALUE.NOVA_BYTES[5] = Byte_5;
  NOVA_VALUE.NOVA_BYTES[6] = Byte_6;
  NOVA_VALUE.NOVA_BYTES[7] = Byte_7;

  int bit_num = 0;

  set_bits(tmeFrame_Time, Byte_0, bit_num);
  set_bits(tmeFrame_Time, Byte_1, bit_num);
  set_bits(tmeFrame_Time, Byte_2, bit_num);
  set_bits(tmeFrame_Time, Byte_3, bit_num);
  set_bits(tmeFrame_Time, Byte_4, bit_num);
  set_bits(tmeFrame_Time, Byte_5, bit_num);
  set_bits(tmeFrame_Time, Byte_6, bit_num);
  set_bits(tmeFrame_Time, Byte_7, bit_num);

}

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

void AUTOMOBILE_NOVA::process(unsigned long tmeFrame_Time,int Pid, int Message[])
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

      tmp_nova_item.store_value(tmeFrame_Time, Pid, Message[0], Message[1], Message[2], Message[3], 
                                                    Message[4], Message[5], Message[6], Message[7]);

      // Filter
      if (Pid == 0x7e8 || Pid == 0x7e9 || Pid == 0x7ea || Pid == 0x7eb)
      {
        tmp_nova_item.NOVA_VALUE.NON_CONSISTANT = true;
      }

      NOVA_ITEMS.push_back(tmp_nova_item);
    }
    else
    // found
    {
      NOVA_ITEMS[location].store_value(tmeFrame_Time, Pid, Message[0], Message[1], Message[2], Message[3], 
                                                            Message[4], Message[5], Message[6], Message[7]);
    }
  }

  // dummy
  Message[0] = 0;

}


#endif