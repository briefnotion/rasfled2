// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: rasapi.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
// *
// ***************************************************************************************
// Note: I dont like the way boost filesystem and standard file system is mixed.  Needs 
//        a few rewrites.

#ifndef RASAPI_H
#define RASAPI_H

// Standard Header Files
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <time.h>
#include <deque>
#include <fstream>
#include <sstream>
#include <algorithm> // Required for std::remove_if
#include <cctype>    // Required for std::isalnum and std::isspace

// Boost libraries
#include <boost/filesystem.hpp>

using namespace std;

// -------------------------------------------------------------------------------------
class FILE_WATCH_Properties
// Properties (duh)
{
  public:
  bool READ_FROM_BEGINNING = false; // Set behavior to return next line from previous 
                                    //  read last line read on file change or start 
                                    //  next line from beginning on file change.

  bool FILE_EXIST = false;  // Indicates existance of file on last file check.
  bool ACTIVE = true;       // Indicates if routine is in good status and working.
  bool ENABLED = true;      // turn on and off routine.  (Not Yet Implemented)

  bool WATCH_SIZE_CHANGE = true;  // New line indicated if File Size changed.
  bool WATCH_TIME_CHANGE = false; // New line indicated if File Write Time changed.
};

class FILE_WATCH
// Watches a log file and returns lines added through the get_next_line function.
{
  private:
  ifstream LOG_FILE;

  string FILENAME = "";

  long  FILE_SIZE   = 0;
  bool  LINE_AVAIL  = false;

  // Next cursor position of Read_Line
  streampos POSITION;

  // File Properties
  time_t LAST_WRITE_TIME;

  bool CHANGED = false;

  public:
  FILE_WATCH_Properties PROP;

  private:

  bool open();

  bool close();

  void close_then_reopen();

  bool check_file_status();

  void reset();

  long get_file_size();

  bool file_time_changed();

  bool file_changed();

  public:

  void start(string Filename);
  
  void stop();

  bool line_avail();

  bool changed();

  string get_next_line();
};

bool check_create_working_dir(string Directory, bool Create);
// Not Coded

bool get_files_list(string Directory, vector<string> &List, string Only_Type);
// returns false if no files found.

//std::string file_to_string(const std::string& Dir_Filename, bool& Success, bool strip_special_chars = false)
std::string file_to_string(const std::string& Dir_Filename, bool& Success);
// This function reads the entire content of a file into a single string.
// It is more efficient and robust than the original version.
//
// Dir_Filename: The path and name of the file to read.
// Success: A reference to a boolean variable that will be set to true if the
//          file was read successfully, and false otherwise.
// strip_special_chars: A boolean parameter that, if true, will remove any
//                      character that is not a letter, number, or space from
//                      the returned string.
// Returns: A string containing the entire content of the file.

string file_to_string(string Dir_Filename);
// Opens and loads a file to return as a string.

bool deque_string_to_file(const std::string& Dir_Filename, std::deque<std::string>& qFile, bool Append);
/**
 * @param Dir_Filename The path to the file.
 * @param qFile A reference to the deque of strings to be written.
 * @param Append If true, appends to the file. If false, overwrites the file.
 * @return True on success, false on failure.
 */

bool file_to_deque_string(string Dir_Filename, deque<string> &qFile);
// Opens and saves a dequed string into a file with append option
// Reurns false if failed.

#endif