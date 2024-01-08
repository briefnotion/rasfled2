// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: comport.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef COMPORT_CPP
#define COMPORT_CPP

#include "comport.h"

using namespace std;

// -------------------------------------------------------------------------------------
// 

/***************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received.                                           *
***************************************************************************/

void COMPORT::write_to_comm(string Command)
{
  Command = Command + '\n';

  int n_written = write(USB, Command.c_str(), Command.size());

  if (n_written == -1) 
  {
    // Handle error here
  } 
  else if (n_written != (int)Command.size()) 
  {
    // Not all characters were written
  }
}

bool COMPORT::read_from_comm()
{
  bool ret_data_read = false;

  char buf = '\0';

  while (read( USB, &buf, 1 ) > 0)
  {
    if (buf != PROPS.ENDING_CHAR)
    {
      if (PROPS.PRINTABLE_CHARACTERS_ONLY)
      {
        if (isprint(static_cast<unsigned char>(buf)))
        {
          RESPONSE = RESPONSE + buf;
        }
      }
      else
      {
        RESPONSE = RESPONSE + buf;
      }
    }
    else
    {
      
      if (RESPONSE.size() > 0)
      {
        READ_FROM_COMM.push_back(RESPONSE);

        if (PROPS.FLASH_DATA_RECORDER_ACTIVE == true)
        {
          FLASH_DATA.push_back(RESPONSE);
        }
        
        ret_data_read = true;
      }

      RESPONSE = "";
      buf = '\0';
    }
  }

  return ret_data_read;
}

bool COMPORT::create()
{
  bool ret_success = true;

  // for BAUD rate target cycle changing
  if (ACTIVE_BAUD_RATE == 0)
  {
    ACTIVE_BAUD_RATE = PROPS.BAUD_RATE;
  }

  if (PROPS.AUTOSTART == false)
  {
    // Disable Cycles
    CYCLE = -1;
  }

  // Needs Restart an autostart cycle.

  if (PROPS.PORT == "")
  {
    ret_success = false;
    READ_FROM_COMM.push_back("COM:Test file failed to load.");
  }
  if (PROPS.RECEIVE_TEST_DATA == true)
  {
    file_to_deque_string(PROPS.TEST_DATA_FILENAME, TEST_DATA);
    READ_FROM_COMM.push_back("COM:Test file loaded.");
    ret_success = true;
  }
  else
  {
    memset (&tty, 0, sizeof tty);

    // WARNING:
    // Method #2: Allocate memory on stack and copy the contents of the
    // original string. Keep in mind that once a current function returns,
    // the memory is invalidated.
    char *port;
    port = (char *)alloca(PROPS.PORT.size() + 1);
    memcpy(port, PROPS.PORT.c_str(), PROPS.PORT.size() + 1);

    USB = open(port, O_RDWR| O_NOCTTY| O_NONBLOCK );

    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 ) 
    {
      //   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
      ret_success = false;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    // Choices: B0,  B50,  B75,  B110,  B134,  B150,  B200, B300, B600, B1200, B1800, B2400, 
    //          B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800
    if (ACTIVE_BAUD_RATE == 300)
    {
      cfsetospeed (&tty, (speed_t)B300);
      cfsetispeed (&tty, (speed_t)B300);
    }
    else if (ACTIVE_BAUD_RATE == 2400)
    {
      cfsetospeed (&tty, (speed_t)B2400);
      cfsetispeed (&tty, (speed_t)B2400);
    }
    else if (ACTIVE_BAUD_RATE == 4800)
    {
      cfsetospeed (&tty, (speed_t)B4800);
      cfsetispeed (&tty, (speed_t)B4800);
    }
    else if (ACTIVE_BAUD_RATE == 9600)
    {
      cfsetospeed (&tty, (speed_t)B9600);
      cfsetispeed (&tty, (speed_t)B9600);
    }
    else if (ACTIVE_BAUD_RATE == 19200)
    {
      cfsetospeed (&tty, (speed_t)B19200);
      cfsetispeed (&tty, (speed_t)B19200);
    }
    else if (ACTIVE_BAUD_RATE == 38400)
    {
      cfsetospeed (&tty, (speed_t)B38400);
      cfsetispeed (&tty, (speed_t)B38400);
    }
    else if (ACTIVE_BAUD_RATE == 57600)
    {
      cfsetospeed (&tty, (speed_t)B57600);
      cfsetispeed (&tty, (speed_t)B57600);
    }
    else if (ACTIVE_BAUD_RATE == 115200)
    {
      cfsetospeed (&tty, (speed_t)B115200);
      cfsetispeed (&tty, (speed_t)B115200);
    }
    else if (ACTIVE_BAUD_RATE == 230400)
    {
      cfsetospeed (&tty, (speed_t)B230400);
      cfsetispeed (&tty, (speed_t)B230400);
    }
    else if (ACTIVE_BAUD_RATE == 460800)
    {
      cfsetospeed (&tty, (speed_t)B460800);
      cfsetispeed (&tty, (speed_t)B460800);
    }

    /* Setting other Port Stuff */
    if (PROPS.PARITY == false)          // Set Parity
    {
      tty.c_cflag &= ~PARENB;           // No Parity
    }
    else
    {
      tty.c_cflag |= PARENB;            // Parity
    }

    if (PROPS.STOP_BITS == 1)           // Set Stop Bits
    {
      tty.c_cflag &=  ~CSTOPB;          // 1 Stop Bit
    }
    else
    {
      tty.c_cflag |= CSTOPB;            // 2 Stop Bit    
    }

    tty.c_cflag &=  ~CSIZE;             // Clear Bits per Byte
    if (PROPS.BIT_COUNT == 5)           // 5 Bits per Byte
    {
      tty.c_cflag |=  CS5;
    }
    else if (PROPS.BIT_COUNT == 6)      // 6 Bits per Byte
    {
      tty.c_cflag |=  CS6;
    }
    else if (PROPS.BIT_COUNT == 7)      // 7 Bits per Byte
    {
      tty.c_cflag |=  CS7;
    }
    else //(PROPS.BIT_COUNT == 8)       // 8 Bits per Byte
    {
      tty.c_cflag |=  CS8;
    }

    if (PROPS.HARDWARE_FLOW_CONTROL == false) // Hardware Flow Control
    {
      tty.c_cflag &=  ~CRTSCTS;         // no flow control
    }
    else
    {
      tty.c_cflag |= CRTSCTS;           // flow control
    }

    tty.c_cc[VMIN]  = PROPS.READ_BYTE_MIN;  // default 1
    tty.c_cc[VTIME] = PROPS.READ_MIN_TIME;  // default 0.5 seconds


    tty.c_cflag |=  CREAD | CLOCAL; // turn on READ & ignore ctrl lines

    if (PROPS.DISABLE_CANONICAL_MODE == true)
    {
      tty.c_cflag &= ~ICANON;
    }

    if (PROPS.XONXOFF == true)
    {
      tty.c_iflag |= IXON | IXOFF;            // enable XON/XOFF flow control
    }

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );

    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) 
    {
      //   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
      ret_success = false;
    }
  }

  CONNECTED = ret_success;
  return ret_success;
}

void COMPORT::stop()
{
  // i dont know how to close this thing.
  tty = tty_old;
  CONNECTED = false;
}

bool COMPORT::record_in_progress()
{
  return SAVE_TO_LOG;
}

void COMPORT::log_file_on()
{
  SAVE_TO_LOG_FILENAME = PROPS.SAVE_LOG_FILENAME + "_data_" + file_format_system_time() + ".txt";
  SAVE_TO_LOG = true;
}

void COMPORT::log_file_off()
{
  SAVE_TO_LOG = false;
}

void COMPORT::device_baud_rate_change_to_target_string(string Device_Baud_Rate_Change_String)
{
  BAUD_RATE_TARGET_DEVICE_CHANGE_BAUD_RATE_STRING = Device_Baud_Rate_Change_String;
}

void COMPORT::write_flash_data()
{
  FLASH_DATA_WRITE = true;
}

void COMPORT::flash_data_check()
{
  if (PROPS.FLASH_DATA_RECORDER_ACTIVE == true && FLASH_DATA_WRITE == true)
  {
    // write flash data to disk
    deque_string_to_file(PROPS.SAVE_LOG_FILENAME + "_flash_" + file_format_system_time() + ".txt", FLASH_DATA, true);
    FLASH_DATA_WRITE = false;
  }

  if ((int)FLASH_DATA.size() > PROPS.FLASH_DATA_SIZE + 500)
  {
    FLASH_DATA.erase(FLASH_DATA.begin(), FLASH_DATA.begin() + FLASH_DATA.size() - PROPS.FLASH_DATA_SIZE);
  }
}

void COMPORT::send(string Text)
{
  WRITE_TO_COMM.push_back(Text);
}

string COMPORT::recieve()
{
  string ret_string = "";

  if (READ_FROM_COMM.size() > 0)
  {
    ret_string = READ_FROM_COMM.front();
    READ_FROM_COMM.pop_front();
  }

  return ret_string;  
}

int COMPORT::recieve_size()
{
  return READ_FROM_COMM.size();
}

void COMPORT::request_to_send()
{
  if (CONNECTED == true)
  {
    write_to_comm("r");
  }
}

bool COMPORT::cycle(unsigned long tmeFrame_Time)
{
  bool data_received = true;
  bool ret_cycle_changed = false;

  if (CYCLE == 99)
  {
    // First Run
    DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 2000);

    CYCLE_CHANGE = 99;
    ret_cycle_changed = true;

    if (PROPS.AUTOSTART)
    {
      CYCLE = 2;  // Start connection cycle.
    }
    else
    {
      CYCLE = -1; // Read as if connected
    }
  }
  else if (CYCLE == -1)
  {
    // Ignore all cycles if Cycle is -1.
  }
  else if (CYCLE == 0)
  {
    // Keep active with checking for live data
    if (PROPS.AUTOSTART && PROPS.CONTINUOUS_DATA)
    {
      if (DATA_RECIEVED_TIMER.ping_down(tmeFrame_Time) == false)
      {
        // Start shutdown cycle
        ACTIVE_BAUD_RATE = PROPS.BAUD_RATE;

        CYCLE = 1;
      }
    }
  }
  else if (CYCLE == 1)
  {
    // Check for cycle close. Need to ensure all comm sent before closing connection and reconnecting
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 1;
      ret_cycle_changed = true;

      // Close the connection.
      //  Don't start reconnect if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        CYCLE_TIMER.ping_up(tmeFrame_Time, 1000); // Wait before starting next cycle.
        CYCLE = 2;

        stop();
      }
      else
      {
        CYCLE = -1;
      }
    }
  }
  else if (CYCLE == 2)
  {
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 2;
      ret_cycle_changed = true;

      //  Open a new connection at current baud rate.
      //  Don't prepare check baud rate if autoconnect if not on
      if (PROPS.AUTOSTART == true)
      {
        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 2000);   // Refresh the data recieve timer.

        // Dont go into cycle 3 if BAUD_RATE_CHANGE_TO is off
        if (PROPS.BAUD_RATE_CHANGE_TO)
        {
          CYCLE_TIMER.ping_up(tmeFrame_Time, 2000);           // Wait before starting next cycle.
          CYCLE = 3; // Prepare to check for proper baud rate
          }
        else
        {
          CYCLE = 0; // Go into normal cycle.
        }

        create();
      }
      else
      {
        CYCLE = -1;
      }
    }
  }
  else if (CYCLE == 3)
  {
    // Target Baud Rate
    if (CYCLE_TIMER.ping_down(tmeFrame_Time) == false)
    {
      CYCLE_CHANGE = 3;
      ret_cycle_changed = true;

      if (PROPS.BAUD_RATE_CHANGE_TO)
      {
        if (CONNECTED)
        {
          if (ACTIVE_BAUD_RATE == PROPS.BAUD_RATE_TARGET)
          {
            CYCLE_CHANGE = 0;
            ret_cycle_changed = true;
            CYCLE = 0;
          }
          else
          {
            // Connected on non target, baud rate
            // Send command to device to change its rate. Then change rate.
            send(BAUD_RATE_TARGET_DEVICE_CHANGE_BAUD_RATE_STRING.c_str());

            CYCLE_TIMER.ping_up(tmeFrame_Time, 2000); // Start timer to disconnect
            CYCLE = 1;

            // Cycle Original and Target
            if (ACTIVE_BAUD_RATE == PROPS.BAUD_RATE)
            {
              ACTIVE_BAUD_RATE = PROPS.BAUD_RATE_TARGET;
            }
            else
            {
              ACTIVE_BAUD_RATE = PROPS.BAUD_RATE;
            }
          }
        }
        else
        {
          CYCLE_TIMER.ping_up(tmeFrame_Time, 2000); // Start timer to disconnect
          CYCLE = 1;

          // Cycle Original and Target
          if (ACTIVE_BAUD_RATE == PROPS.BAUD_RATE)
          {
            ACTIVE_BAUD_RATE = PROPS.BAUD_RATE_TARGET;
          }
          else
          {
            ACTIVE_BAUD_RATE = PROPS.BAUD_RATE;
          }
        }
      }
      else
      {
        if (CONNECTED)
        {            
          CYCLE_CHANGE = 0;
          ret_cycle_changed = true;
          CYCLE = 0;
        }
        else
        {
          CYCLE_TIMER.ping_up(tmeFrame_Time, 2000); // Start timer to disconnect
          CYCLE = 1;
        }
      }
    }
  }
  else
  {
    // Unknown Cycle Number
    CYCLE_TIMER.ping_up(tmeFrame_Time, 2000); // Shutdown no autoconnect
    CYCLE = -1;
  }

  // Do not access comm port if it is shut down.
  if (CONNECTED == true)
  {
    // Send Data
    if (WRITE_TO_COMM.size() > 0)
    // Sending data to comm port.
    {
      //while (WRITE_TO_COMM.size() > 0)
      // Dont send more than one command per cycle. Recieving side isn't checking 
      //  multiple commands yet.
      {
        if ((SAVE_TO_LOG_SENT_DATA == true) && 
            (SAVE_TO_LOG == true) && 
            (WRITE_TO_COMM.size() > 0) && 
            (PROPS.RECEIVE_TEST_DATA == false))
        {
          WRITE_TO_COMM.push_front("- Send");
          deque_string_to_file(SAVE_TO_LOG_FILENAME, WRITE_TO_COMM, true);
          WRITE_TO_COMM.pop_front();
        }

        // put data into send to comm port queue
        write_to_comm(WRITE_TO_COMM.front());
        WRITE_TO_COMM.pop_front();
      }
    }

    // -----

    // Receive data.
    if(PROPS.RECEIVE_TEST_DATA == false)
    {
      // get data from comm port
      // exit loop when no data available.
      while (data_received == true)
      {
        data_received = read_from_comm();
      }

      if (READ_FROM_COMM.size() > 0)
      {
        DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 5000);   // Looking for live data
        
        if (SAVE_TO_LOG == true && READ_FROM_COMM.size() >0)
        {
          deque_string_to_file(SAVE_TO_LOG_FILENAME, READ_FROM_COMM, true);
        }
      }
    }
    else
    {
      // get test data from file
      for (int count = 0; count < 6; count++)   // Count is the number of messages
      {                                         //  sent per cycle.
        if (TEST_DATA.size() > 0)
        {
          DATA_RECIEVED_TIMER.ping_up(tmeFrame_Time, 5000);   // Looking for live data
        
          READ_FROM_COMM.push_back(TEST_DATA.front());
          
          TEST_DATA.pop_front();
        }
      }
    }
  }

  // Comms flash data check and cleanup.
  flash_data_check();

  return ret_cycle_changed;
}

int COMPORT::cycle_change()
{
  return CYCLE_CHANGE;
}

void COMPORT::open_port()
{
  ACTIVE_BAUD_RATE = 0;
  CYCLE = 99;
}

void COMPORT::close_port()
{
  CYCLE = -1;
  stop();
}



#endif