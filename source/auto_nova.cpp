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

void NOVA_ITEM::store_value(unsigned long tmeFrame_Time, int Id, 
                            unsigned char Byte_0, unsigned char Byte_1, 
                            unsigned char Byte_2, unsigned char Byte_3, 
                            unsigned char Byte_4, unsigned char Byte_5, 
                            unsigned char Byte_6, unsigned char Byte_7)
{
  NOVA_VALUE.ID = Id;

  // -------------------------
  // Byte_0

  if(set_bool_with_change_notify(bit_value(Byte_0, 7), NOVA_VALUE.NOVA_BITS[ 0]))
  {
    if(NOVA_VALUE.FREQUENCY[ 0].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 0].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 0].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 0].ping_up(tmeFrame_Time, 5000);
    }
  }

  if(set_bool_with_change_notify(bit_value(Byte_0, 6), NOVA_VALUE.NOVA_BITS[ 1]))
  {
    if(NOVA_VALUE.FREQUENCY[ 1].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 1].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 1].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 1].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 5), NOVA_VALUE.NOVA_BITS[ 2]))
  {
    if(NOVA_VALUE.FREQUENCY[ 2].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 2].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 2].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 2].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 4), NOVA_VALUE.NOVA_BITS[ 3]))
  {
    if(NOVA_VALUE.FREQUENCY[ 3].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 3].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 3].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 3].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 3), NOVA_VALUE.NOVA_BITS[ 4]))
  {
    if(NOVA_VALUE.FREQUENCY[ 4].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 4].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 4].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 4].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 2), NOVA_VALUE.NOVA_BITS[ 5]))
  {
    if(NOVA_VALUE.FREQUENCY[ 5].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 5].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 5].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 5].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 1), NOVA_VALUE.NOVA_BITS[ 6]))
  {
    if(NOVA_VALUE.FREQUENCY[ 6].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 6].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 6].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 6].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_0, 0), NOVA_VALUE.NOVA_BITS[ 7]))
  {
    if(NOVA_VALUE.FREQUENCY[ 7].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 7].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 7].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 7].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_1

  if(set_bool_with_change_notify(bit_value(Byte_1, 7), NOVA_VALUE.NOVA_BITS[ 8]))
  {
    if(NOVA_VALUE.FREQUENCY[ 8].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 8].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 8].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 8].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 6), NOVA_VALUE.NOVA_BITS[ 9]))
  {
    if(NOVA_VALUE.FREQUENCY[ 9].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[ 9].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[ 9].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[ 9].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 5), NOVA_VALUE.NOVA_BITS[10]))
  {
    if(NOVA_VALUE.FREQUENCY[10].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[10].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[10].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[10].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 4), NOVA_VALUE.NOVA_BITS[11]))
  {
    if(NOVA_VALUE.FREQUENCY[11].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[11].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[11].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[11].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 3), NOVA_VALUE.NOVA_BITS[12]))
  {
    if(NOVA_VALUE.FREQUENCY[12].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[12].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[12].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[12].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 2), NOVA_VALUE.NOVA_BITS[13]))
  {
    if(NOVA_VALUE.FREQUENCY[13].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[13].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[13].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[13].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 1), NOVA_VALUE.NOVA_BITS[14]))
  {
    if(NOVA_VALUE.FREQUENCY[14].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[14].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[14].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[14].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_1, 0), NOVA_VALUE.NOVA_BITS[15]))
  {
    if(NOVA_VALUE.FREQUENCY[15].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[15].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[15].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[15].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_2

  if(set_bool_with_change_notify(bit_value(Byte_2, 7), NOVA_VALUE.NOVA_BITS[16]))
  {
    if(NOVA_VALUE.FREQUENCY[16].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[16].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[16].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[16].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 6), NOVA_VALUE.NOVA_BITS[17]))
  {
    if(NOVA_VALUE.FREQUENCY[17].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[17].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[17].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[17].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 5), NOVA_VALUE.NOVA_BITS[18]))
  {
    if(NOVA_VALUE.FREQUENCY[18].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[18].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[18].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[18].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 4), NOVA_VALUE.NOVA_BITS[19]))
  {
    if(NOVA_VALUE.FREQUENCY[19].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[19].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[19].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[19].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 3), NOVA_VALUE.NOVA_BITS[20]))
  {
    if(NOVA_VALUE.FREQUENCY[20].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[20].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[20].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[20].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 2), NOVA_VALUE.NOVA_BITS[21]))
  {
    if(NOVA_VALUE.FREQUENCY[21].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[21].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[21].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[21].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 1), NOVA_VALUE.NOVA_BITS[22]))
  {
    if(NOVA_VALUE.FREQUENCY[22].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[22].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[22].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[22].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_2, 0), NOVA_VALUE.NOVA_BITS[23]))
  {
    if(NOVA_VALUE.FREQUENCY[23].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[23].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[23].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[23].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_3

  if(set_bool_with_change_notify(bit_value(Byte_3, 7), NOVA_VALUE.NOVA_BITS[24]))
  {
    if(NOVA_VALUE.FREQUENCY[24].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[24].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[24].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[24].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 6), NOVA_VALUE.NOVA_BITS[25]))
  {
    if(NOVA_VALUE.FREQUENCY[25].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[25].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[25].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[25].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 5), NOVA_VALUE.NOVA_BITS[26]))
  {
    if(NOVA_VALUE.FREQUENCY[26].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[26].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[26].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[26].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 4), NOVA_VALUE.NOVA_BITS[27]))
  {
    if(NOVA_VALUE.FREQUENCY[27].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[27].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[27].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[27].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 3), NOVA_VALUE.NOVA_BITS[28]))
  {
    if(NOVA_VALUE.FREQUENCY[28].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[28].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[28].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[28].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 2), NOVA_VALUE.NOVA_BITS[29]))
  {
    if(NOVA_VALUE.FREQUENCY[29].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[29].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[29].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[29].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 1), NOVA_VALUE.NOVA_BITS[30]))
  {
    if(NOVA_VALUE.FREQUENCY[30].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[30].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[30].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[30].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_3, 0), NOVA_VALUE.NOVA_BITS[31]))
  {
    if(NOVA_VALUE.FREQUENCY[31].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[31].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[31].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[31].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_4

  if(set_bool_with_change_notify(bit_value(Byte_4, 7), NOVA_VALUE.NOVA_BITS[32]))
  {
    if(NOVA_VALUE.FREQUENCY[32].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[32].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[32].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[32].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 6), NOVA_VALUE.NOVA_BITS[33]))
  {
    if(NOVA_VALUE.FREQUENCY[33].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[33].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[33].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[33].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 5), NOVA_VALUE.NOVA_BITS[34]))
  {
    if(NOVA_VALUE.FREQUENCY[34].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[34].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[34].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[34].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 4), NOVA_VALUE.NOVA_BITS[35]))
  {
    if(NOVA_VALUE.FREQUENCY[35].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[35].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[35].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[35].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 3), NOVA_VALUE.NOVA_BITS[36]))
  {
    if(NOVA_VALUE.FREQUENCY[36].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[36].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[36].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[36].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 2), NOVA_VALUE.NOVA_BITS[37]))
  {
    if(NOVA_VALUE.FREQUENCY[37].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[37].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[37].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[37].ping_up(tmeFrame_Time, 5000);
    }
  }
  if(set_bool_with_change_notify(bit_value(Byte_4, 1), NOVA_VALUE.NOVA_BITS[38]))
  {
    if(NOVA_VALUE.FREQUENCY[38].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[38].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[38].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[38].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_4, 0), NOVA_VALUE.NOVA_BITS[39]))
  {
    if(NOVA_VALUE.FREQUENCY[39].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[39].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[39].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[39].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_5

  if(set_bool_with_change_notify(bit_value(Byte_5, 7), NOVA_VALUE.NOVA_BITS[40]))
  {
    if(NOVA_VALUE.FREQUENCY[40].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[40].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[40].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[40].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 6), NOVA_VALUE.NOVA_BITS[41]))
  {
    if(NOVA_VALUE.FREQUENCY[41].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[41].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[41].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[41].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 5), NOVA_VALUE.NOVA_BITS[42]))
  {
    if(NOVA_VALUE.FREQUENCY[42].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[42].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[42].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[42].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 4), NOVA_VALUE.NOVA_BITS[43]))
  {
    if(NOVA_VALUE.FREQUENCY[43].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[43].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[43].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[43].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 3), NOVA_VALUE.NOVA_BITS[44]))
  {
    if(NOVA_VALUE.FREQUENCY[44].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[44].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[44].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[44].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 2), NOVA_VALUE.NOVA_BITS[45]))
  {
    if(NOVA_VALUE.FREQUENCY[45].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[45].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[45].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[45].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 1), NOVA_VALUE.NOVA_BITS[46]))
  {
    if(NOVA_VALUE.FREQUENCY[46].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[46].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[46].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[46].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_5, 0), NOVA_VALUE.NOVA_BITS[47]))
  {
    if(NOVA_VALUE.FREQUENCY[47].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[47].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[47].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[47].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_6

  if(set_bool_with_change_notify(bit_value(Byte_6, 7), NOVA_VALUE.NOVA_BITS[48]))
  {
    if(NOVA_VALUE.FREQUENCY[48].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[48].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[48].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[48].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 6), NOVA_VALUE.NOVA_BITS[49]))
  {
    if(NOVA_VALUE.FREQUENCY[49].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[49].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[49].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[49].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 5), NOVA_VALUE.NOVA_BITS[50]))
  {
    if(NOVA_VALUE.FREQUENCY[50].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[50].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[50].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[50].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 4), NOVA_VALUE.NOVA_BITS[51]))
  {
    if(NOVA_VALUE.FREQUENCY[51].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[51].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[51].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[51].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 3), NOVA_VALUE.NOVA_BITS[52]))
  {
    if(NOVA_VALUE.FREQUENCY[52].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[52].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[52].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[52].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 2), NOVA_VALUE.NOVA_BITS[53]))
  {
    if(NOVA_VALUE.FREQUENCY[53].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[53].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[53].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[53].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 1), NOVA_VALUE.NOVA_BITS[54]))
  {
    if(NOVA_VALUE.FREQUENCY[54].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[54].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[54].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[54].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_6, 0), NOVA_VALUE.NOVA_BITS[55]))
  {
    if(NOVA_VALUE.FREQUENCY[55].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[55].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[55].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[55].ping_up(tmeFrame_Time, 5000);
    }
  }

  // -------------------------
  // Byte_7

  if(set_bool_with_change_notify(bit_value(Byte_7, 7), NOVA_VALUE.NOVA_BITS[56]))
  {
    if(NOVA_VALUE.FREQUENCY[56].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[56].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[56].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[56].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 6), NOVA_VALUE.NOVA_BITS[57]))
  {
    if(NOVA_VALUE.FREQUENCY[57].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[57].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[57].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[57].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 5), NOVA_VALUE.NOVA_BITS[58]))
  {
    if(NOVA_VALUE.FREQUENCY[58].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[58].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[58].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[58].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 4), NOVA_VALUE.NOVA_BITS[59]))
  {
    if(NOVA_VALUE.FREQUENCY[59].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[59].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[59].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[59].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 3), NOVA_VALUE.NOVA_BITS[60]))
  {
    if(NOVA_VALUE.FREQUENCY[60].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[60].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[60].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[60].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 2), NOVA_VALUE.NOVA_BITS[61]))
  {
    if(NOVA_VALUE.FREQUENCY[61].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[61].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[61].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[61].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 1), NOVA_VALUE.NOVA_BITS[62]))
  {
    if(NOVA_VALUE.FREQUENCY[62].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[62].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[62].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[62].ping_up(tmeFrame_Time, 5000);
    }
  }
  
  if(set_bool_with_change_notify(bit_value(Byte_7, 0), NOVA_VALUE.NOVA_BITS[63]))
  {
    if(NOVA_VALUE.FREQUENCY[63].ping_down(tmeFrame_Time))
    {
      NOVA_VALUE.FREQUENCY[63].ping_up(tmeFrame_Time, 5000);
    }
    else
    {
      NOVA_VALUE.FREQUENCY[63].ping_up(tmeFrame_Time, 5000);
      NOVA_VALUE.HILIGHT[63].ping_up(tmeFrame_Time, 5000);
    }
  }
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