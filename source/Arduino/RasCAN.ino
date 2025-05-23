#include <Arduino.h>
#include <mcp_can.h>
//#include <mcp_can_dfs.h>

// -----------------------------------------------------

#define Revision "2.146_250325"

// -----------------------------------------------------
// Definitions

#define CANint 2
//#define LED2 8
//#define LED3 7

#define SPI_CS_PIN                        17      //  * 
// #define SPI_CS_PIN = 17;                       // CANBed V1
// #define SPI_CS_PIN = 3;                        // CANBed M0
// #define SPI_CS_PIN = 9;                        // CAN Bus Shield

#define CAN_ID_PID                        0x7DF   //  * CAN_ID

// LED Light
#define LED01                             13      //  * 
// #define LED01 = 23  Hobby Tronics: https://www.hobbytronics.co.uk/product/leonardo-can-bus
// #define LED01 = 13  CANBED V1    : https://docs.longan-labs.cc/1030008/

// -----------------------------------------------------

// Message

#define MESSAGES_IMMEDIATE_SIZE           13
#define MESSAGES_STANDARD_SIZE            35

// CAN
#define PID_PIDS_SUPPORTED_01_20          0x00  //  
#define PID_MONITOR                       0x01  //  
#define PID_FUEL_STATUS                   0x03  //  * 07 E8 04 41 03 02 00 00 00 00 0001D846
#define PID_CALC_ENGINE_LOAD              0x04  //  * 07 E9 03 41 04 80 00 00 00 00 0001DC1A
#define PID_COOLANT_TEMP                  0x05  //  * 07 E8 03 41 05 6F 00 00 00 00 0003E108
#define PID_SHORT_TERM_FUEL_TRIM_1        0x06  //  * 07 E8 03 41 06 7E 00 00 00 00 0002EF49
#define PID_LONG_TERM_FUEL_TRIM_1         0x07  //  * 07 E8 03 41 07 89 00 00 00 00 0003FE53
#define PID_SHORT_TERM_FUEL_TRIM_2        0x08  //  
#define PID_LONG_TERM_FUEL_TRIM_2         0x09  //  
#define PID_FUEL_PRESSURE                 0x0A  //  
#define PID_INTAKE_MANIFOLD_ABS_PRESSURE  0x0B  //  
#define PID_ENGIN_PRM                     0x0C  //  * 07 E8 04 41 0C 14 6B 00 00 00 0007338C
#define PID_VEHICLE_SPEED                 0x0D  //  * 07 E8 03 41 0D 36 00 00 00 00 000415C3
#define PID_INTAKE_AIR_TEMP               0x0F  //  * 07 E8 03 41 0F 4F 00 00 00 00 000419BF

#define PID_MASS_AIR_FLOW_RATE            0x10  //  * 07 E8 04 41 10 01 85 00 00 00 00030FBF
#define PID_THROTTLE_POSITION             0x11  //  * 07 E8 03 41 11 2A 00 00 00 00 000529D8
//#define PID_OXYGEN_SENSOR                 0x14 //  
#define PID_OBD_STANDARDS                 0x1C  //  * 07 E9 03 41 1C 09 00 00 00 00 00020C2C
                                                //  * 07 E8 03 41 1C 03 00 00 00 00 00020C30
#define PID_OXYGEN_SENSOR_PRESENT         0x1D  //  
#define PID_RUN_TIME_SINCE_START          0x1F  //  * 07 E8 04 41 1F 00 AA 00 00 00 0002140B

#define PID_PIDS_SUPPORTED_21_40          0x20  //  
#define PID_DISTANCE_TRAVELED_MIL_ON      0x21  //  * 07 E8 04 41 21 00 00 00 00 00 0003272F
#define PID_FUEL_RAIL_PRESSURE_MANIFOLD   0x22  //  
#define PID_FUEL_RAIL_PRESSURE_GAUGE      0x23  //  * 07 E8 04 41 23 00 E1 00 00 00 00032EEB
//#define PID_OXYGEN_SENSOR                 0x24  //  
#define PID_COMMANDED_ERG                 0x2C  //  
#define PID_FUEL_TANK_LEVEL               0x2F  //  * 07 E8 03 41 2F 75 00 00 00 00 00054D00

#define PID_DISTANCE_SINCE_CODES_CLEARED  0x31  //  * 07 E8 04 41 31 08 9E 00 00 00 00033AA3
#define PID_EVAP_SYSTEM_VAPOR_PRESSURE    0x32  //  * 07 E8 04 41 32 FF 77 00 00 00 000662D9
#define PID_BARAMETRIC_PRESSURE           0x33  //  * 07 E9 03 41 33 64 00 00 00 00 0002371D
//#define PID_OXYGEN_SENSOR                 0x34  //  
#define PID_CATALYST_TEMP_BANK_1_SENSOR_1 0x3C  //  * 07 E8 04 41 3C 19 A3 00 00 00 00066801
#define PID_CATALYST_TEMP_BANK_2_SENSOR_1 0x3D  //  
#define PID_CATALYST_TEMP_BANK_1_SENSOR_2 0x3E  //  
#define PID_CATALYST_TEMP_BANK_2_SENSOR_2 0x3F  //  

#define PID_PIDS_SUPPORTED_41_60          0x40  //  
#define PID_MONITOR_STATUS_DRIVE_CYCLE    0x41  //  
#define PID_CONTROL_VOLTAGE               0x42  //  * 07 E8 04 41 42 36 17 00 00 00 00070902
#define PID_ABSOLUTE_LOAD                 0x43  //  * 07 E8 04 41 43 00 1C 00 00 00 00046644
#define PID_COMMANDED_AIR_FUEL_RATIO      0x44  //  * 07 E8 04 41 44 70 A3 00 00 00 00046A22
#define PID_RELATIVE_TROTTLE_POS          0x45  //  
#define PID_AMBIENT_AIR_TEMPERATURE       0x46  //  * 07 E8 03 41 46 4C 00 00 00 00 00057D0B
#define PID_ABSOLOUTE_TROTTLE_POSITION_B  0x47  //  * 07 E8 03 41 47 2B 00 00 00 00 00068C53
#define PID_ABSOLOUTE_TROTTLE_POSITION_C  0x48  //  
#define PID_ACCELERATORE_PEDAL_POSITION_D 0x49  //  * 07 E9 03 41 49 27 00 00 00 00 00026486
#define PID_ACCELERATORE_PEDAL_POSITION_E 0x4A  //  * 07 E8 03 41 4A 14 00 00 00 00 0006980A
#define PID_ACCELERATORE_PEDAL_POSITION_F 0x4B  //  
#define PID_COMMANDED_THROTTLE_ACTUATOR_E 0x4C  //  * 07 E8 03 41 4C 14 00 00 00 00 00069FE5
#define PID_TIME_RUN_WITH_MIL_ON          0x4D  //  
#define PID_TIME_SINCE_TROUBLE_CODES_CLEARED                0x4E  //  
#define PID_MAX_VAL_FUEL_AIR_RATIO_OV_OCUR_MAN_ABS_PRESS    0x4F  //  

#define PID_MAX_VAL_AIR_FLOW_RATE_SENSOR  0x50  //  
#define PID_ABS_EVAP_PRESSURE             0x53  //  
#define PID_EVAP_PRESSURE                 0x54  //  
#define PID_FUEL_RAIL_ABS_PRESSURE        0x59  //  
#define PID_REL_ACCELERATORE_PEDAL_POS    0X5A  //  
#define PID_ENGINE_OIL_TEMP               0x5C  //  
#define PID_ENGINE_FUEL_RATE              0x5E  //  

#define PID_PIDS_SUPPORTED_61_80          0x60  //  
#define PID_DRIVERS_DEMAND_ENGINE_TORQUE  0x61  //  
#define PID_ACTURAL_DEMAND_ENGINE_TORQUE  0x62  //  
#define PID_REFERENCE_ENGINE_TORQUE       0x63  //  
#define PID_ENGINE_PERCENT_TORQUE_DATA    0x64  //  
#define PID_MASS_AIR_FLOW_SENSOR          0x66  //  
#define PID_ENGINE_COOLANT_TEMPERATURE    0x67  //  
#define PID_INTAKE_AIR_TEMPERATURE_SENSOR 0x68  //  
#define PID_EXHAST_GAS_RECIRC_TEMP         0x6B  // 

#define PID_PIDS_SUPPORTED_81_A0          0x80  //  
#define PID_MANIFOLD_SURFACE_TEMP         0x84  //  

#define PID_ENGINE_FUEL_RATE              0x9D  //  
#define PID_ENGINE_EXHAST_FLOW_RATE       0x9E  //  
#define PID_FUEL_SYSTEM_PERCENTAGE_USED   0x9F  //  

#define PID_PIDS_SUPPORTED_A1_C0          0xA0  //  
#define PID_TRANS_ACUTAL_GEAR             0xA4  //  
#define PID_ODOMETER                      0xA6  //  
#define PID_ABS_DISABLE_SWITCH_STATE      0xA9  //  

// ---

// Reducing Can Message Count
// Normal opperation is 165msg/sec with max queue size of 15.

// Format Groupings:

// Immediate - Messages where the most recent value is important

// Tachometer
// Doors
// Transmission
// Lights
// Ignition Switch
// High Beam
// Cruis Control
// Signal Lights

// Priority - Messages that are immediate, but with stored history.

// Tire Speed
// Steering wheel angle
// Hazards

// Standard - Any other message type.


struct CAM_MESSAGE
{
  public:

  unsigned long Message_ID = 0;
  byte Message_len = 0;
  byte Message_buf[8] = {0};
  unsigned long Message_timestamp = 0;
  bool CHANGED = false;
  // 18 bytes?
};

// -----------------------------------------------------
class MESSAGE_STORAGE
// Simple message storeage system
//  30 messages
{
  public:

  int MESSAGES_RECEIVED_COUNT_TO_SEND = 0;

  CAM_MESSAGE MESSAGES_IMMEDIATE[MESSAGES_IMMEDIATE_SIZE];  // Dedicated Message Storage
  int MESSAGE_COUNT_IMMEDIATE = 0;
  // 0  - (AD_80)  - Power
  // 1  - (AD_90)  - Tachometer
  // 2  - (AD_C0)  - Fuel Percentage
  // 3  - (AD_C8)  - Lights, Ignition Switch
  // 4  - (AD_D0)  - Gear Selection
  // 5  - (AD_F0)  - Transmission
  // 6  - (AD_130) - Dash Indicators
  // 7  - (AD_200) - Cruise Control
  // 8  - (AD_252) - Brake pos pressure
  // 9  - (AD_260) - Signal Lights
  // 10 - (AD_360) - Doors, High Beam
  // 11 - (AD_380) - Fuel Level

  CAM_MESSAGE MESSAGES_STANDARD[MESSAGES_STANDARD_SIZE];
  int MESSAGE_COUNT_STANDARD = 0;
  // (AD_10)  - Steering wheel angle
  // (AD_190) - Tire Speed
  // (AD_310) - Hazards
  // (AD_3E8) - Requested PID
  // (AD_3E9) - Requested PID
  // (AD_3EA) - Requested PID
  // (AD_3EB) - Requested PID
  // Any other message type.

  int message_count()
  {
    return MESSAGE_COUNT_IMMEDIATE + MESSAGE_COUNT_STANDARD;
  }

  void message_clear()
  {
    MESSAGE_COUNT_IMMEDIATE = 0;
    MESSAGE_COUNT_STANDARD = 0;
  }

  void store(unsigned long Message_ID, byte Message_len, byte Message_buf[], unsigned long Message_timestamp)
  {
    switch (Message_ID)
    {
      case (0x80):
      {
        // 0  - (AD_80)  - Power
        MESSAGES_IMMEDIATE[0].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[0].Message_len = Message_len;

        MESSAGES_IMMEDIATE[0].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[0].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[0].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[0].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[0].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[0].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[0].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[0].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[0].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[0].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }

        MESSAGES_IMMEDIATE[0].CHANGED = true;
        
        break;
      }
      case (0x90):
      {
        // 1  - (AD_90)  - Tachometer
        MESSAGES_IMMEDIATE[1].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[1].Message_len = Message_len;

        MESSAGES_IMMEDIATE[1].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[1].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[1].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[1].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[1].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[1].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[1].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[1].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[1].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[1].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[1].CHANGED = true;

        break;
      }
      case (0xc0):
      {
        // 2  - (AD_C0)  - Fuel Percentage
        MESSAGES_IMMEDIATE[2].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[2].Message_len = Message_len;

        MESSAGES_IMMEDIATE[2].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[2].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[2].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[2].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[2].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[2].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[2].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[2].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[2].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[2].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }

        MESSAGES_IMMEDIATE[2].CHANGED = true;

        break;
      }
      case (0xc8):
      {
        // 3  - (AD_C8)  - Lights, Ignition Switch
        MESSAGES_IMMEDIATE[3].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[3].Message_len = Message_len;

        MESSAGES_IMMEDIATE[3].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[3].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[3].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[3].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[3].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[3].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[3].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[3].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[3].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[3].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[3].CHANGED = true;

        break;
      }
      case (0xD0):
      {
        // 4  - (AD_D0)  - Gear Selection
        MESSAGES_IMMEDIATE[4].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[4].Message_len = Message_len;

        MESSAGES_IMMEDIATE[4].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[4].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[4].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[4].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[4].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[4].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[4].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[4].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[4].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[4].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }

        MESSAGES_IMMEDIATE[4].CHANGED = true;
        
        break;
      }
      case (0xF0):
      {
        // 5  - (AD_F0)  - Transmission
        MESSAGES_IMMEDIATE[5].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[5].Message_len = Message_len;

        MESSAGES_IMMEDIATE[5].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[5].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[5].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[5].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[5].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[5].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[5].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[5].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[5].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[5].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }

        MESSAGES_IMMEDIATE[5].CHANGED = true;
        
        break;
      }
      case (0x130):
      {
        // 6  - (AD_130) - Dash Indicators
        MESSAGES_IMMEDIATE[6].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[6].Message_len = Message_len;

        MESSAGES_IMMEDIATE[6].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[6].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[6].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[6].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[6].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[6].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[6].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[6].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[6].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[6].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[6].CHANGED = true;

        break;
      }
      case (0x200):
      {
        // 7  - (AD_200) - Cruise Control
        MESSAGES_IMMEDIATE[7].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[7].Message_len = Message_len;

        MESSAGES_IMMEDIATE[7].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[7].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[7].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[7].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[7].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[7].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[7].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[7].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[7].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[7].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[7].CHANGED = true;

        break;
      }
      case (0x252):
      {
        // 8  - (AD_252) - Brake pos pressure
        MESSAGES_IMMEDIATE[8].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[8].Message_len = Message_len;

        MESSAGES_IMMEDIATE[8].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[8].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[8].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[8].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[8].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[8].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[8].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[8].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[8].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[8].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[8].CHANGED = true;

        break;
      }
      case (0x260):
      {
        // 9  - (AD_260) - Signal Lights
        MESSAGES_IMMEDIATE[9].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[9].Message_len = Message_len;

        MESSAGES_IMMEDIATE[9].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[9].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[9].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[9].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[9].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[9].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[9].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[9].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[9].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[9].CHANGED == true)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[9].CHANGED = true;

        break;
      }
      case (0x360):
      {
        // 10 - (AD_360) - Doors, High Beam
        MESSAGES_IMMEDIATE[10].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[10].Message_len = Message_len;

        MESSAGES_IMMEDIATE[10].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[10].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[10].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[10].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[10].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[10].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[10].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[10].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[10].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[10].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[10].CHANGED = true;

        break;
      }
      case (0x380):
      {
        // 11 - (AD_380) - Fuel Level
        MESSAGES_IMMEDIATE[11].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[11].Message_len = Message_len;

        MESSAGES_IMMEDIATE[11].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[11].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[11].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[11].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[11].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[11].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[11].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[11].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[11].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[11].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[11].CHANGED = true;
        
        break;
      }
      case (0x140):
      {
        // 12  - (AD_140) - Dash Screens
        MESSAGES_IMMEDIATE[12].Message_ID = Message_ID;
        MESSAGES_IMMEDIATE[12].Message_len = Message_len;

        MESSAGES_IMMEDIATE[12].Message_buf[0] = Message_buf[0];
        MESSAGES_IMMEDIATE[12].Message_buf[1] = Message_buf[1];
        MESSAGES_IMMEDIATE[12].Message_buf[2] = Message_buf[2];
        MESSAGES_IMMEDIATE[12].Message_buf[3] = Message_buf[3];
        MESSAGES_IMMEDIATE[12].Message_buf[4] = Message_buf[4];
        MESSAGES_IMMEDIATE[12].Message_buf[5] = Message_buf[5];
        MESSAGES_IMMEDIATE[12].Message_buf[6] = Message_buf[6];
        MESSAGES_IMMEDIATE[12].Message_buf[7] = Message_buf[7];
        
        MESSAGES_IMMEDIATE[12].Message_timestamp = Message_timestamp;

        if (MESSAGES_IMMEDIATE[12].CHANGED == false)
        {
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_IMMEDIATE++;
        }
        
        MESSAGES_IMMEDIATE[12].CHANGED = true;

        break;
      }

      default:
      {
        // (AD_10)  - Steering wheel angle
        // (AD_190) - Tire Speed
        // (AD_310) - Hazards
        if (MESSAGE_COUNT_STANDARD < MESSAGES_STANDARD_SIZE)
        {
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_ID = Message_ID;
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_len = Message_len;

          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[0] = Message_buf[0];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[1] = Message_buf[1];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[2] = Message_buf[2];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[3] = Message_buf[3];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[4] = Message_buf[4];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[5] = Message_buf[5];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[6] = Message_buf[6];
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_buf[7] = Message_buf[7];
          
          MESSAGES_STANDARD[MESSAGE_COUNT_STANDARD].Message_timestamp = Message_timestamp;
          
          MESSAGES_RECEIVED_COUNT_TO_SEND++;
          MESSAGE_COUNT_STANDARD++;
        }
        break;
      }
    }
  }
};

// -----------------------------------------------------
// Control Class
class CONTROL
{
  // Read Byte Characteristics
  // p = Stop Read
  // s = Continue Read
  // r = Send Can Data
  // t = Send Test Data

  public:
  int version = 0;
  
  bool test = false;                // Send (toggle) test data: t
  bool start = true;
  bool filter = true;               // Filter (toggle) with known PIDs: f
  bool wait_for_request = true;     // Only send data when "r" received (toggle): wait 

  bool restart = false;             // Restart: q

  //unsigned long send_delay = 0;     // Delay after send: del0 del1 del3 del10

  // ---

  bool send_test_response = false;

  bool send_command = false;
  unsigned char command;

  bool send_service_command = false;
  unsigned char service_command;
  unsigned char service_command_data_00;

  bool send_service_command_2 = false;
  unsigned char service_command_2_data_00;
  unsigned char service_command_2_data_01;
  unsigned char service_command_2_data_02;
  unsigned char service_command_2_data_03;
  unsigned char service_command_2_data_04;
  unsigned char service_command_2_data_05;
  unsigned char service_command_2_data_06;
  unsigned char service_command_2_data_07;

    // ---

  bool read(String read_string)
  {
    read_string.trim();

    if (read_string == "r")
    {
      // Immediatly exit the function routine, this is a send data request.
      return true;
    }

    // version 3 and version == 4 and version == 5commands
    // 00 - 0F
    else if (read_string == "00")
    {
      command = PID_PIDS_SUPPORTED_01_20;
      send_command = true;
    }
    else if (read_string == "01")
    {
      command = PID_MONITOR;
      send_command = true;
    }
    else if (read_string == "03")
    {
      command = PID_FUEL_STATUS;
      send_command = true;
    }
    else if (read_string == "04")
    {
      command = PID_CALC_ENGINE_LOAD;
      send_command = true;
    }
    else if (read_string == "05" || read_string == "temp")
    {
      command = PID_COOLANT_TEMP;
      send_command = true;
    }
    else if (read_string == "06")
    {
      command = PID_SHORT_TERM_FUEL_TRIM_1;
      send_command = true;
    }
    else if (read_string == "07")
    {
      command = PID_LONG_TERM_FUEL_TRIM_1;
      send_command = true;
    }
    else if (read_string == "08")
    {
      command = PID_SHORT_TERM_FUEL_TRIM_2;
      send_command = true;
    }
    else if (read_string == "09")
    {
      command = PID_LONG_TERM_FUEL_TRIM_2;
      send_command = true;
    }
    else if (read_string == "0A")
    {
      command = PID_FUEL_PRESSURE;
      send_command = true;
    }
    else if (read_string == "0B")
    {
      command = PID_INTAKE_MANIFOLD_ABS_PRESSURE;
      send_command = true;
    }
    else if (read_string == "0C" || read_string == "temp")
    {
      command = PID_ENGIN_PRM;
      send_command = true;
    }
    else if (read_string == "0D" || read_string == "speed")
    {
      command = PID_VEHICLE_SPEED;
      send_command = true;
    }
    else if (read_string == "0F")
    {
      command = PID_INTAKE_AIR_TEMP;
      send_command = true;
    }

    // 10 - 1F
    else if (read_string == "10")
    {
      command = PID_MASS_AIR_FLOW_RATE;
      send_command = true;
    }
    else if (read_string == "11")
    {
      command = PID_THROTTLE_POSITION;
      send_command = true;
    }
    else if (read_string == "1C")
    {
      command = PID_OBD_STANDARDS;
      send_command = true;
    }
    else if (read_string == "1D")
    {
      command = PID_OXYGEN_SENSOR_PRESENT;
      send_command = true;
    }
    else if (read_string == "1F")
    {
      command = PID_RUN_TIME_SINCE_START;
      send_command = true;
    }

    // 20 - 2F
    else if (read_string == "20")
    {
      command = PID_PIDS_SUPPORTED_21_40;
      send_command = true;
    }
    else if (read_string == "21")
    {
      command = PID_DISTANCE_TRAVELED_MIL_ON;
      send_command = true;
    }
    else if (read_string == "22")
    {
      command = PID_FUEL_RAIL_PRESSURE_MANIFOLD;
      send_command = true;
    }
    else if (read_string == "23")
    {
      command = PID_FUEL_RAIL_PRESSURE_GAUGE;
      send_command = true;
    }
    else if (read_string == "2C")
    {
      command = PID_COMMANDED_ERG;
      send_command = true;
    }
    else if (read_string == "2F")
    {
      command = PID_FUEL_TANK_LEVEL;
      send_command = true;
    }

    // 30 - 3F
    else if (read_string == "31")
    {
      command = PID_DISTANCE_SINCE_CODES_CLEARED;
      send_command = true;
    }
    else if (read_string == "32")
    {
      command = PID_EVAP_SYSTEM_VAPOR_PRESSURE;
      send_command = true;
    }
    else if (read_string == "33")
    {
      command = PID_BARAMETRIC_PRESSURE;
      send_command = true;
    }
    else if (read_string == "3C")
    {
      command = PID_CATALYST_TEMP_BANK_1_SENSOR_1;
      send_command = true;
    }
    else if (read_string == "3D")
    {
      command = PID_CATALYST_TEMP_BANK_2_SENSOR_1;
      send_command = true;
    }
    else if (read_string == "3E")
    {
      command = PID_CATALYST_TEMP_BANK_1_SENSOR_2;
      send_command = true;
    }
    else if (read_string == "3F")
    {
      command = PID_CATALYST_TEMP_BANK_2_SENSOR_2;
      send_command = true;
    }

    // 40 - 4F
    else if (read_string == "40")
    {
      command = PID_PIDS_SUPPORTED_41_60;
      send_command = true;
    }
    else if (read_string == "41")
    {
      command = PID_MONITOR_STATUS_DRIVE_CYCLE;
      send_command = true;
    }
    else if (read_string == "42")
    {
      command = PID_CONTROL_VOLTAGE;
      send_command = true;
    }
    else if (read_string == "43")
    {
      command = PID_ABSOLUTE_LOAD;
      send_command = true;
    }
    else if (read_string == "44")
    {
      command = PID_COMMANDED_AIR_FUEL_RATIO;
      send_command = true;
    }
    else if (read_string == "45")
    {
      command = PID_RELATIVE_TROTTLE_POS;
      send_command = true;
    }
    else if (read_string == "46")
    {
      command = PID_AMBIENT_AIR_TEMPERATURE;
      send_command = true;
    }
    else if (read_string == "47")
    {
      command = PID_ABSOLOUTE_TROTTLE_POSITION_B;
      send_command = true;
    }
    else if (read_string == "48")
    {
      command = PID_ABSOLOUTE_TROTTLE_POSITION_C;
      send_command = true;
    }
    else if (read_string == "49")
    {
      command = PID_ACCELERATORE_PEDAL_POSITION_D;
      send_command = true;
    }
    else if (read_string == "4A")
    {
      command = PID_ACCELERATORE_PEDAL_POSITION_E;
      send_command = true;
    }
    else if (read_string == "4B")
    {
      command = PID_ACCELERATORE_PEDAL_POSITION_F;
      send_command = true;
    }
    else if (read_string == "4C")
    {
      command = PID_COMMANDED_THROTTLE_ACTUATOR_E;
      send_command = true;
    }
    else if (read_string == "4D")
    {
      command = PID_TIME_RUN_WITH_MIL_ON;
      send_command = true;
    }
    else if (read_string == "4E")
    {
      command = PID_TIME_SINCE_TROUBLE_CODES_CLEARED;
      send_command = true;
    }
    else if (read_string == "4F")
    {
      command = PID_MAX_VAL_FUEL_AIR_RATIO_OV_OCUR_MAN_ABS_PRESS;
      send_command = true;
    }

    // 50 - 5F
    else if (read_string == "50")
    {
      command = PID_MAX_VAL_AIR_FLOW_RATE_SENSOR;
      send_command = true;
    }
    else if (read_string == "53")
    {
      command = PID_ABS_EVAP_PRESSURE;
      send_command = true;
    }
    else if (read_string == "54")
    {
      command = PID_EVAP_PRESSURE;
      send_command = true;
    }
    else if (read_string == "59")
    {
      command = PID_FUEL_RAIL_ABS_PRESSURE;
      send_command = true;
    }
    else if (read_string == "5A")
    {
      command = PID_REL_ACCELERATORE_PEDAL_POS;
      send_command = true;
    }
    else if (read_string == "5C")
    {
      command = PID_ENGINE_OIL_TEMP;
      send_command = true;
    }
    else if (read_string == "5E")
    {
      command = PID_ENGINE_FUEL_RATE;
      send_command = true;
    }

    // 60 - 6F
    else if (read_string == "60")
    {
      command = PID_PIDS_SUPPORTED_61_80;
      send_command = true;
    }
    else if (read_string == "61")
    {
      command = PID_DRIVERS_DEMAND_ENGINE_TORQUE;
      send_command = true;
    }
    else if (read_string == "62")
    {
      command = PID_ACTURAL_DEMAND_ENGINE_TORQUE;
      send_command = true;
    }
    else if (read_string == "63")
    {
      command = PID_REFERENCE_ENGINE_TORQUE;
      send_command = true;
    }
    else if (read_string == "64")
    {
      command = PID_ENGINE_PERCENT_TORQUE_DATA;
      send_command = true;
    }
    else if (read_string == "66")
    {
      command = PID_MASS_AIR_FLOW_SENSOR;
      send_command = true;
    }
    else if (read_string == "67")
    {
      command = PID_ENGINE_COOLANT_TEMPERATURE;
      send_command = true;
    }
    else if (read_string == "68")
    {
      command = PID_INTAKE_AIR_TEMPERATURE_SENSOR;
      send_command = true;
    }
    else if (read_string == "6B")
    {
      command = PID_EXHAST_GAS_RECIRC_TEMP;
      send_command = true;
    }

    // 70 - 7F

    // 80 - 8F
    else if (read_string == "80")
    {
      command = PID_PIDS_SUPPORTED_81_A0;
      send_command = true;
    }
    else if (read_string == "84")
    {
      command = PID_MANIFOLD_SURFACE_TEMP;
      send_command = true;
    }
    
    // 90 - 9F
    else if (read_string == "9D")
    {
      command = PID_ENGINE_FUEL_RATE;
      send_command = true;
    }
    else if (read_string == "9E")
    {
      command = PID_ENGINE_EXHAST_FLOW_RATE;
      send_command = true;
    }
    else if (read_string == "9F")
    {
      command = PID_FUEL_SYSTEM_PERCENTAGE_USED;
      send_command = true;
    }

    // A0 - AF
    else if (read_string == "A0")
    {
      command = PID_PIDS_SUPPORTED_A1_C0;
      send_command = true;
    }
    else if (read_string == "A4")
    {
      command = PID_TRANS_ACUTAL_GEAR;
      send_command = true;
    }
    else if (read_string == "A6")
    {
      command = PID_ODOMETER;
      send_command = true;
    }
    else if (read_string == "A9")
    {
      command = PID_ABS_DISABLE_SWITCH_STATE;
      send_command = true;
    }

    // B0 - BF

    // request mode 3 Show Diagnostic Codes
    else if (read_string == "diags")
    {
      // 0x02, 0x01, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00
      // partially works. cant read responses

      // Request pid list in mode 9?
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x03;
      service_command_2_data_02 = 0x00;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Diagnostic List Code");
    }

    // request mode 3 Clear Diagnostic Codes
    else if (read_string == "diagc")
    {
      // 0x02, 0x01, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00
      // worked once, cant get it to work again

      // Request pid list in mode 9?
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x04;
      service_command_2_data_02 = 0x00;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Diagnostic Clear Code");
    }

    // request mode 9 supported list
    else if (read_string == "ser")
    {
      // 0x02, 0x01, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00
      // works- returns list of supported mode 9 pids

      // Request pid list in mode 9?
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x09;
      service_command_2_data_02 = 0x00;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Services List Code");
    }

    // request mode 9 vin list
    else if (read_string == "vin")
    {
      // 0x02, 0x01, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00
      // doesnt work - may not be supported. returns nothing.
      // Request pid list in mode 9?
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x09;
      service_command_2_data_02 = 0x02;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:VIN Request Code");
    }

    // Tire Pressure
    else if (read_string == "t1")
    // doesnt work
    {
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x22;
      service_command_2_data_02 = 0x00;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Tire 1 Unknown");
    }
    else if (read_string == "t2")
    // doesnt work
    {
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x22;
      service_command_2_data_02 = 0x01;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Tire 2 Unknown");
    }
    else if (read_string == "t3")
    // doesnt work
    {
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x22;
      service_command_2_data_02 = 0x02;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Tire 3 Unknown");
    }
    else if (read_string == "t4")
    // doesnt work
    {
      service_command_2_data_00 = 0x02;
      service_command_2_data_01 = 0x22;
      service_command_2_data_02 = 0x03;
      service_command_2_data_03 = 0x00;
      service_command_2_data_04 = 0x00;
      service_command_2_data_05 = 0x00;
      service_command_2_data_06 = 0x00;
      service_command_2_data_07 = 0x00;

      send_service_command_2 = true;

      Serial.println("CAN:Tire 4 Unknown");
    }

    else if (read_string == "test")
    {
      send_test_response = true;
    }
    
    else if (read_string == "p")
    {
      // Command to pause and unpause input
      if (start == false)
      {
        start = true;
        Serial.println("CAN:Pause Off");
      }
      else
      {
        start = false;
        Serial.println("CAN:Pause On");
      }
    }
    else if (read_string == "t")
    {
      // Command to send test or stop sending test data
      if (test == true)
      {
        test = false;
        Serial.println("CAN:Test Data Off");
      }
      else
      {
        test = true;
        Serial.println("CAN:Test Data On");
      }
    }
    else if (read_string == "f")
    {
      // Command to filter pids on or off
      if (filter == true)
      {
        filter = false;
        Serial.println("CAN:Filter Off");
      }
      else
      {
        filter = true;
        Serial.println("CAN:Filter On");
      }
    }
    else if (read_string == "q")
    {
      // Command to restart
      restart = true;
    }
    else if (read_string == "v5")
    {
      version = 5;
      restart = true;
    }

    // Wait for response
    else if (read_string == "wait")
    {
      if (wait_for_request == true)
      {
        wait_for_request = false;
        Serial.println("CAN:Wait Off");
      }
      else
      {
        wait_for_request = true;
        Serial.println("CAN:Wait On");
      }
    }

    return false;
  }

  void reset()
  {
    test = false;
    start = true;
    filter = true;
    restart = false;
    wait_for_request = true;
  }
};


// -----------------------------------------------------
// Global Variables

unsigned char len = 0;
unsigned char buf[8];
unsigned long ID = 0;
unsigned long CAN_ID = 0;
unsigned long MESSAGE_TIME = 0;

MCP_CAN CAN0(SPI_CS_PIN); // Set CS to pin 17

CONTROL ctrl;

// -----------------------------------------------------
// Setup Variables

unsigned long time;
void setup() 
{
  // Set Default Version
  ctrl.version = 5;

  // Prep Serial
  Serial.begin(115200);
  
  while (!Serial) 
  {
    Serial.print("CAN:I will wait for serial connect");
    delay(1000);
  };
  
  // Prep Pins
  pinMode(23, OUTPUT);
  digitalWrite(LED01, HIGH); // led

  //pinMode(LED2, OUTPUT);
  //pinMode(LED3, OUTPUT);
  pinMode(CANint, INPUT);
  //digitalWrite(LED2, LOW);
    
  // Prep CAN Bus
  Serial.println("CAN:CAN init:");

  if (CAN0.begin(CAN_500KBPS) == CAN_OK) 
  {
    Serial.println("CAN:Can Init Success");
  } 
  else 
  {
    Serial.println("CAN:Can Init Failed");
    while (1) 
    {
      Serial.print("CAN:I will wait here forever...");
      delay(1000);
    }
  }

  Serial.println("CAN:Good to go!");

  digitalWrite(LED01, LOW); // led
}


// -----------------------------------------------------
// Blink and time
void blink(bool Blink)
{
  // -----------------------------------------------------
  // If system is paused then blink the lights
  if(Blink)
  {
    digitalWrite(LED01, HIGH);  // LED OFF
    delay(100); 
    digitalWrite(LED01, LOW); // LED ON
    delay(900); 
  }
}

// -----------------------------------------------------
// Read from host
String read_com()
{
  String read_string = Serial.readStringUntil('\n');
  read_string.trim();
  return read_string;
}

// -----------------------------------------------------
// Convert char to string hex

void print_hex(unsigned char value)
{
  if(value<16)
  {
    Serial.print('0');
    Serial.print(value,HEX);
  }
  else
  {
    Serial.print(value,HEX);    
  }
  Serial.print(" ");
}


String format_hex(unsigned char value)
{
  String ret_hex = "";
  
  if(value<16)
  {
    ret_hex = ret_hex + "0";
    ret_hex = ret_hex + String(value,HEX);
  }
  else
  {
    ret_hex = ret_hex + String(value,HEX);    
  }

  ret_hex = ret_hex + " ";
  return ret_hex;
}

void print_hex_UL(unsigned long value)
{
  if(value == 0)
  {
    Serial.print("00000000");
  }
  else if(value < 16)
  {
    Serial.print("0000000");
    Serial.print(value, HEX);
  }
  else if (value < 256)
  {
    Serial.print("000000");
    Serial.print(value, HEX);    
  }
  else if (value < 4096)
  {
    Serial.print("00000");
    Serial.print(value, HEX);    
  }
  else if (value < 65536)
  {
    Serial.print("0000");
    Serial.print(value, HEX);    
  }
  else if (value < 1048576)
  {
    Serial.print("000");
    Serial.print(value, HEX);    
  }
  else if (value < 16777216)
  {
    Serial.print("00");
    Serial.print(value, HEX);    
  }
  else if (value < 268435456)
  {
    Serial.print("0");
    Serial.print(value, HEX);    
  }
  else
  {
    Serial.print(value, HEX);    
  }
  
  Serial.print(" ");
}

String format_hex_UL(unsigned long value)
{
  String ret_hex = "";

  if(value == 0)
  {
    ret_hex = "00000000";
  }
  else if(value < 16)
  {
    ret_hex = "0000000";
    ret_hex = ret_hex + String(value, HEX);
  }
  else if (value < 256)
  {
    ret_hex = "000000";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else if (value < 4096)
  {
    ret_hex = "00000";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else if (value < 65536)
  {
    ret_hex = "0000";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else if (value < 1048576)
  {
    ret_hex = "000";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else if (value < 16777216)
  {
    ret_hex = "00";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else if (value < 268435456)
  {
    ret_hex = "0";
    ret_hex = ret_hex + String(value, HEX);    
  }
  else
  {
    ret_hex = ret_hex + String(value, HEX);    
  }
  
  ret_hex = ret_hex + " ";
  return ret_hex;
}

// sendMsgBuf(unsigned long id, byte ext, byte rtr, byte len, byte *buf)

void sendPid(unsigned char __pid) 
{
  // 2 bytes
  // service 1
  // pid

  unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
  CAN0.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

void send_Service_Pid_2(  unsigned char data_0, unsigned char data_1, unsigned char data_2, unsigned char data_3, 
                          unsigned char data_4, unsigned char data_5, unsigned char data_6, unsigned char data_7) 
{
  // data_0 - number of bytes
  // data_1 - mode
  // data_2 - param ...
  unsigned char tmp[8] = {data_0, data_1, data_2, data_3, data_4, data_5, data_6, data_7};

  // 0x7DF is the OBD-II broadcast ID
  // 0 - standard message
  // 8 - message size
  CAN0.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);

  // message should be recreived on 0x7E8
}

String serial_send_format(CAM_MESSAGE Message)
{
  String ret_send_string = "";
  unsigned char IDl = Message.Message_ID % 256;
  unsigned char IDu = (Message.Message_ID - IDl) / 256;

  ret_send_string = ret_send_string + format_hex(IDu);
  ret_send_string = ret_send_string + format_hex(IDl);

  ret_send_string = ret_send_string + format_hex(Message.Message_buf[0]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[1]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[2]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[3]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[4]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[5]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[6]);
  ret_send_string = ret_send_string + format_hex(Message.Message_buf[7]);

  ret_send_string = ret_send_string + format_hex_UL(Message.Message_timestamp);

  ret_send_string = ret_send_string + "\0";

  return ret_send_string;
}

bool filter(unsigned long ID)
{
  bool ret_pass = false;

  switch (ID)
  {
    case (0x10):
      ret_pass = true;
      break;

    //case (0x70):
    //  ret_pass = true;
    //  break;

    case (0x80):
      ret_pass = true;
      break;

    case (0x90):
      ret_pass = true;
      break;

    case (0xC0):
      ret_pass = true;
      break;

    case (0xC8):
      ret_pass = true;
      break;

    case (0xD0):
      ret_pass = true;
      break;

    case (0xF0):
      ret_pass = true;
      break;

    case (0x130):
      ret_pass = true;
      break;

    case (0x140):
      ret_pass = true;
      break;


    case (0x190):
      ret_pass = true;
      break;

    case (0x200):
      ret_pass = true;
      break;

    case (0x252):
      ret_pass = true;
      break;

    case (0x260):
      ret_pass = true;
      break;

    case (0x310):
      ret_pass = true;
      break;

    case (0x360):
      ret_pass = true;
      break;

    case (0x380):
      ret_pass = true;
      break;

    // Messages on PIDs with:
    case (0x7E8):
      ret_pass = true;
      break;

    case (0x7E9):
      ret_pass = true;
      break;

    case (0x7EA):
      ret_pass = true;
      break;

    case (0x7EB):
      ret_pass = true;
      break;
  }

  return ret_pass;
}


// -----------------------------------------------------
// Version 5

void version_5()
{
  unsigned long recv_canid = 0;

  unsigned long fake_two_byte_counter = 0;
  char fake_one_byte_counter = 0;

  MESSAGE_STORAGE messages_to_send;

  ctrl.reset();
  
  Serial.print("CAN:Version 5 - ");
  Serial.print("Revision ");
  Serial.println(Revision);

  // Statistics
  unsigned long send_statistics_time = millis() + 1000;
  unsigned int messages_max_queue = 0; // Max number of messages in one queue.

  while (ctrl.restart == false)
  {
    if (ctrl.start == true && CAN_MSGAVAIL == CAN0.checkReceive() ||
          ctrl.start == true && ctrl.test == true)
    {
      // Check to see whether data is read
      if (ctrl.test == false)
      {
        // Get Can Data
        MESSAGE_TIME = millis();
        CAN0.readMsgBufID(&ID, &len, buf);    // Read data
      }
      else
      {
        // Get Test Data
        MESSAGE_TIME = millis();
        //MESSAGE_TIME = micro();  // Returns the number of microseconds since the Arduino board began running the current program. Data type: unsigned long.
                                 // 1 millisecond is equal to 1000
        ID = fake_two_byte_counter;
        len = 8;
        buf[0] = fake_one_byte_counter;
        buf[1] = fake_one_byte_counter;
        buf[2] = fake_one_byte_counter;
        buf[3] = fake_one_byte_counter;
        buf[4] = fake_one_byte_counter;
        buf[5] = fake_one_byte_counter;
        buf[6] = fake_one_byte_counter;
        buf[7] = fake_one_byte_counter;
        
        fake_two_byte_counter++;
        fake_one_byte_counter++;

        if (fake_two_byte_counter > 0x0f00)
        {
          fake_two_byte_counter = 0;
        }

        if (fake_two_byte_counter > (256*256)-1)
        {
          fake_two_byte_counter = 0;                    
        }
        
        if (fake_one_byte_counter > 255)
        {
          fake_one_byte_counter = 0;                    
        }

        delayMicroseconds(50);
      }

      // Store message to queue
      if (ctrl.filter == true)
      {
        if (filter(ID) == true)
        {
          digitalWrite(LED01, HIGH); // LED ON

          messages_to_send.store(ID, len, buf, MESSAGE_TIME);

          digitalWrite(LED01, LOW);  // LED OFF

          if (messages_to_send.message_count() > messages_max_queue)
          {
            messages_max_queue = messages_to_send.message_count();
          }
        }
      }
      else
      {
        digitalWrite(LED01, HIGH); // LED ON

        messages_to_send.store(ID, len, buf, MESSAGE_TIME);

        digitalWrite(LED01, LOW);  // LED OFF

        if (messages_to_send.message_count() > messages_max_queue)
        {
          messages_max_queue = messages_to_send.message_count();
        }
      }
    }

    // Read from host
    if((Serial.available() > 0 && ctrl.read(read_com()) == true) || ctrl.wait_for_request == false)
    {
      // Received "receive" message, send queue messages

      // Process Statistics
      if ((messages_to_send.MESSAGES_RECEIVED_COUNT_TO_SEND > 0) && (millis() > send_statistics_time))
      {
        send_statistics_time = millis() + 1000;

        buf[0] = highByte(messages_to_send.MESSAGES_RECEIVED_COUNT_TO_SEND);
        buf[1] = lowByte(messages_to_send.MESSAGES_RECEIVED_COUNT_TO_SEND);
        buf[2] = lowByte(messages_max_queue);
        buf[3] = 0;
        buf[4] = 0;
        buf[5] = 0;
        buf[6] = 0;
        buf[7] = 0;

        messages_to_send.store(0xffff, 0x8, buf, millis());

        messages_to_send.MESSAGES_RECEIVED_COUNT_TO_SEND = 0;
        messages_max_queue = 0;
      }

      //Immediate Message
      for (int pos = 0; pos < MESSAGES_IMMEDIATE_SIZE; pos++)
      {
        if (messages_to_send.MESSAGES_IMMEDIATE[pos].CHANGED == true)
        {
          Serial.println(serial_send_format(messages_to_send.MESSAGES_IMMEDIATE[pos]));
          messages_to_send.MESSAGES_IMMEDIATE[pos].CHANGED = false;
        }
      }

      //Standard Message
      for (int pos = 0; pos < messages_to_send.MESSAGE_COUNT_STANDARD; pos++)
      {
        Serial.println(serial_send_format(messages_to_send.MESSAGES_STANDARD[pos]));
      }
      
      messages_to_send.message_clear();
    }

    // send command to CAN
    if (ctrl.send_command == true)
    {
      digitalWrite(LED01, HIGH); // LED ON

      ctrl.send_command = false;
      sendPid(ctrl.command);
      
      digitalWrite(LED01, LOW);  // LED OFF
    }

    // send command to CAN
    if (ctrl.send_service_command_2 == true)
    {
      digitalWrite(LED01, HIGH); // LED ON

      ctrl.send_service_command_2 = false;

      send_Service_Pid_2( ctrl.service_command_2_data_00, ctrl.service_command_2_data_01, 
                          ctrl.service_command_2_data_02, ctrl.service_command_2_data_03, 
                          ctrl.service_command_2_data_04, ctrl.service_command_2_data_05, 
                          ctrl.service_command_2_data_06, ctrl.service_command_2_data_07);

      // clear vars
      ctrl.service_command_2_data_00 = 0x00;
      ctrl.service_command_2_data_01 = 0x00;
      ctrl.service_command_2_data_02 = 0x00;
      ctrl.service_command_2_data_03 = 0x00;
      ctrl.service_command_2_data_04 = 0x00;
      ctrl.service_command_2_data_05 = 0x00;
      ctrl.service_command_2_data_06 = 0x00;
      ctrl.service_command_2_data_07 = 0x00;
      
      digitalWrite(LED01, LOW);  // LED OFF
    }

    // send test resonse
    if (ctrl.send_test_response == true)
    {
      digitalWrite(LED01, HIGH); // LED ON

      ctrl.send_test_response = false;

      Serial.println("07 e9 06 41 00 98 18 00 13 00 018e398b");
      
      digitalWrite(LED01, LOW);  // LED OFF
    }

    // Blink and Time
    blink(ctrl.start == false);
  }
}

// -----------------------------------------------------
// Main loop

void loop() 
{
  int delay_time = 250;
  
  while(true)
  {
    // pause if in main loop
    delay(delay_time);

    if (ctrl.version == 5)
    {
      version_5();
    }
    else
    {
      Serial.println("CAN:Version Not Supported");
      Serial.println("CAN:Changing to Version 5");
      ctrl.version = 5;
    }

    // pause if in main loop
    delay(delay_time);
  }
}
