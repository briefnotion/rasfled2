// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: rasapi.cpp
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

#ifndef RASAPI_CPP
#define RASAPI_CPP

#include "rasapi.h"

using namespace std;

// -------------------------------------------------------------------------------------

bool FILE_WATCH::open()
// Open the file for watching.
{
  LOG_FILE.open(FILENAME, ios::in);

  if(LOG_FILE.is_open() == true)
  {
    PROP.ACTIVE = true;
  }
  else
  {
    PROP.ACTIVE = false;
    PROP.ENABLED = false;
  }
  return PROP.ACTIVE;
}

bool FILE_WATCH::close()
// Close the file.
{
  if(LOG_FILE.is_open())
  {
    LOG_FILE.close();
  }

  PROP.ACTIVE = false;

  return true;
}

void FILE_WATCH::close_then_reopen()
// Simple close the stream then reopen, effectivly clearing stram errors.
{
  close();
  open();
}

bool FILE_WATCH::check_file_status()
{
  if (boost::filesystem::exists(FILENAME) == false)
  {
    if (PROP.FILE_EXIST == true)
    {
      CHANGED = true;
    }

    PROP.FILE_EXIST = false;
    close();
  }
  else if(LOG_FILE.is_open() != true)
  {
    if (PROP.FILE_EXIST == false)
    {
      CHANGED = true;
    }

    PROP.FILE_EXIST = true;
    open();
  }

  return PROP.ACTIVE;
  
}

void FILE_WATCH::reset()
// Simple close the stream then reopen, effectivly clearing stram errors.
// Also, clears the positon variable so the next attempt to get line or
//  check line avail will cause the routine to start reading from the 
//  beginning.
{
  close();
  LINE_AVAIL = false;
  POSITION = 0;
  open();
}

long FILE_WATCH::get_file_size()
// Get the size of file with name and path defined by the class var named 
//  FILENAME.
{
  if (check_file_status() == true)
  {
    struct stat stat_buf;
    int rc = stat(FILENAME.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
  }
  else
  {
    return 0;
  }
}

bool FILE_WATCH::file_time_changed()
// Returns true if file write time has changed from 
//  previous check.
{
  bool boo_return = false;
  time_t write_time;

  //double seconds;

  if (check_file_status() == true)
  {
    boost::filesystem::path path_and_filename{FILENAME};

    write_time = last_write_time(path_and_filename);

    if(LAST_WRITE_TIME == write_time)
    {
      boo_return = false;
    }
    else
    {
      LAST_WRITE_TIME = write_time;
      boo_return = true;
    }
  }

  return boo_return;
}

bool FILE_WATCH::file_changed()
// Returns true if file size has changed.
// Seperate behavior for Watch Size and or Watch Time Write.

// If Watch Time:
//  Stores time.

// If Watch Size:
// Sets the new FILE_SIZE if changed.
// Complete reset of the entire FILE_WATCH if 
//  the read position is past the end of file because 
//  the file has had content removed.
{
  bool boo_return = false;

  // Watch for File Write Change.
  if(PROP.WATCH_TIME_CHANGE == true)
  {
    if(file_time_changed() == true)
    {
      CHANGED = true;
    }
  }

  // Watch for File Size Change.
  if(PROP.WATCH_SIZE_CHANGE == true && CHANGED == false)
  {
    // Get file size;
    long new_file_size = get_file_size();

    // Reset if position is outside file or file read has failed.
    if (POSITION > new_file_size || (LOG_FILE.fail() == true))
    {
      reset();
      //return false; 
    }
    
    // Compair file size to history.
    if (FILE_SIZE == new_file_size)
    {
      //return false;
    }
    else
    {
      FILE_SIZE = new_file_size;
      CHANGED =  true;
    }
  }

  boo_return = CHANGED;

  CHANGED = false;

  return boo_return;
}

void FILE_WATCH::start(string Filename)
{
  FILENAME = Filename;
}

void FILE_WATCH::stop()
{
  close();
}

bool FILE_WATCH::line_avail()
// Returns true if a line is available to be read.
{
  if (LINE_AVAIL == true || file_changed() == true)
  {
    LINE_AVAIL = true;
  }
  
  return LINE_AVAIL;
}

bool FILE_WATCH::changed()
// Returns true if file changed.
{
  if (file_changed() == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

string FILE_WATCH::get_next_line()
// Returns the newly added line of the watch file.
{
  //string containing new line.
  string str_read_line = "";

  if(check_file_status() == true)
  {
    if(LINE_AVAIL == true && PROP.WATCH_TIME_CHANGE == true)
    {
      if(LOG_FILE.is_open() == false)
      {
        LOG_FILE.open(FILENAME);
      }

      getline(LOG_FILE, str_read_line);

      if (LOG_FILE.eof())
      {
        LINE_AVAIL = false;
        LOG_FILE.close();
      }
    }
    else if (LINE_AVAIL == true && PROP.WATCH_SIZE_CHANGE == true)
    // if line available read line to return.
    {
      // New Position var for comparison.
      streampos new_position;

      // Insure cursor position is at correct spot.
      LOG_FILE.seekg(POSITION);

      // Load next line.
      getline(LOG_FILE, str_read_line);

      // Read new cursor position.
      new_position = LOG_FILE.tellg();

      if (LOG_FILE.eof())
      // No more lines available at EOF.
      {
        // Set class var showing no more lines are available.
        LINE_AVAIL = false;

        // The file will be errored out because of the EOF, 
        //  so properly close it and open again.  There will 
        //  be no more read attempts until the size changes. 
        close_then_reopen();

        // Set the new cursor position to one position back 
        // from the file size, or, one position prior to the 
        // END_OF_FILE marker, so the cursor will be at the 
        // correct posion when the file is read again.
        POSITION = FILE_SIZE -1;
      }
      else
      {
        // More lines are available, but store the new positon 
        //  for safe keeping, which is necessary becaues of the 
        //  next get line call.
        POSITION = new_position;

        // Check and stop grabbing next lines if the program 
        //  is at the END OF FILE.
        if (POSITION >= FILE_SIZE -1)
        {
          LINE_AVAIL = false;
        }
      }
    }
  }
  
  // Return the read line.
  return str_read_line;
}

bool check_create_working_dir(string Directory, bool Create)
{
  bool ret_exist = false;
  namespace fs = boost::filesystem;

  boost::filesystem::path path(Directory);

  if (boost::filesystem::exists(Directory))
  {
    ret_exist = true;
    //printf("Exist: %s\n", Directory.c_str());
  }
  else
  {
    if (Create)
    {
      ret_exist = boost::filesystem::create_directory(path);
      //printf("Create: %s\n", Directory.c_str());
    }
    else
    {
      ret_exist = false;
      //printf("Not Created: %s\n", Directory.c_str());
    }
  }

  return ret_exist;
}

bool get_files_list(string Directory, vector<string> &List, string Only_Type)
{
  bool ret_sucess = false;
  List.clear();

  // Using Boost
  {
    boost::filesystem::path path (Directory);

    if (boost::filesystem::exists(path))
    {
      if (boost::filesystem::is_directory(path))
      {
        for (boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(path))
        {
          if (Only_Type == "")
          {
            List.push_back(x.path().string());
          }
          else
          {
            if (x.path().string().size() > Only_Type.size())
            {
              if (x.path().string().substr(x.path().string().size() - Only_Type.size()) == Only_Type)
              {
                List.push_back(x.path().string());
              }
            }
          }
        }
      
        ret_sucess = true;
      }
      else
      {
        // Directory exist, but it is not a directory.
      }
    }
    else
    {
      // Directory does not exist.
    }
  }

  return ret_sucess;
}


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
//std::string file_to_string(const std::string& Dir_Filename, bool& Success, bool strip_special_chars = false)
std::string file_to_string(const std::string& Dir_Filename, bool& Success)
{
  // Start with the assumption of failure.
  Success = false;

  bool strip_special_chars = true;

  // Use std::ifstream for reading from a file.
  // std::fstream is for both reading and writing.
  std::ifstream inputFile(Dir_Filename);

  // Check if the file was successfully opened. This is a crucial first step.
  if (inputFile.is_open()) 
  {
    // Use a stringstream to efficiently build the string from the file's content.
    // This is much faster than repeated string concatenation.
    std::stringstream buffer;
    
    // Read the entire content of the file into the stringstream buffer.
    // The rdbuf() function returns a pointer to the stream's streambuf object,
    // which can be used to copy the entire file content at once.
    buffer << inputFile.rdbuf();

    // Close the file stream. It's good practice to close streams explicitly.
    inputFile.close();

    // Get the string from the stringstream.
    std::string file_content = buffer.str();

    // Check if we need to strip special characters.
    if (strip_special_chars) 
    {
      // Use the erase-remove idiom to efficiently filter out unwanted characters.
      // This lambda function specifically targets and removes carriage returns,
      // newlines, and tabs, which are common culprits for parsing issues.
      file_content.erase(std::remove_if(file_content.begin(), file_content.end(),
                                        [](unsigned char c) 
                                        {
                                          return c == '\r' || c == '\n' || c == '\t'; 
                                        }),
                          file_content.end());
    }

    // If we reach this point, the operation was successful.
    Success = true;
    
    return file_content;
  }

  // If the file could not be opened, return an empty string and keep Success as false.
  return "";
}

string file_to_string(string Dir_Filename)
{
  bool success = false;
  return file_to_string(Dir_Filename, success);
}

/**
 * @brief Writes the contents of a deque of strings to a file.
 * @param Dir_Filename The path to the file.
 * @param qFile A reference to the deque of strings to be written.
 * @param Append If true, appends to the file. If false, overwrites the file.
 * @return True on success, false on failure.
 */
bool deque_string_to_file(const std::string& Dir_Filename, std::deque<std::string>& qFile, bool Append)
{
  // Use an RAII approach to handle file streams. The fstream object will
  // automatically close when it goes out of scope, even if an error occurs.
  // This makes the code cleaner and less prone to resource leaks.
  std::fstream fsFile;

  // Determine the open mode based on the 'Append' flag.
  std::ios_base::openmode openMode = std::ios::out;
  if (Append) 
  {
    openMode = std::ios::app;
  }

  // Open the file.
  fsFile.open(Dir_Filename, openMode);

  // Check if the file opened successfully.
  if (!fsFile.is_open()) 
  {
    std::cerr << "Error: Could not open file " << Dir_Filename << std::endl;
    return false; // Return early on failure.
  }

  // Iterate through the deque and write each string to the file.
  // A range-based for loop is more modern and readable.
  for (const auto& line : qFile) 
  {
    fsFile << line << '\n'; // Write the line and a newline character.
  }

  // The fstream destructor will automatically close the file.
  // No need for an explicit fsFile.close(); call unless you need to
  // close it for some reason before the function returns.

  // If we reached this point, the write operation was a success.
  return true;
}

bool file_to_deque_string(string Dir_Filename, deque<string> &qFile)
{
  fstream fsFile;

  bool booSuccess = false;
  bool booActive = false;

  fsFile.open(Dir_Filename, ios::in);

  if (!fsFile)
  {
    booActive = false;
  }
  else 
  {
    booActive = true;
  }

  if (booActive == true)
  {
    string strRead = "";

    while(booActive == true)
    {
      getline(fsFile,strRead);

      qFile.push_back(strRead);

      if(fsFile.eof())
      {
        booActive = false;
        booSuccess = true;
      }
    }
  }

  return booSuccess;
}

// -------------------------------------------------------------------------------------

/**
 * @brief The core routine that performs the file saving.
 * * It takes the filename and the content as arguments to avoid any data races.
 * @param dir_filename The path to the file to save.
 * @param file_contents The content to write to the file.
 */
void save_routine(const std::string& dir_filename, const std::string& file_contents)
{
  // Use an RAII approach to handle file streams.
  std::fstream fsFile;
  std::ios_base::openmode openMode = std::ios::out;
  fsFile.open(dir_filename, openMode);

  if (fsFile.is_open()) 
  {
    fsFile << file_contents;
  }
}

/**
 * @brief A public, standalone function to save a deque of strings to a file in a detached thread.
 * * This function encapsulates the logic for preparing the data and offloading the
 * I/O operation to a separate thread, returning immediately.
 * @param dir_filename The path to the file to save.
 * @param qFile The deque of strings to save.
 */
void threaded_deque_string_to_file(const std::string& dir_filename, const std::deque<std::string>& qFile)
{
  if (!qFile.empty())
  {
    std::string file_contents;
    for (const auto& line : qFile)
    {
      file_contents += line + "\n";
    }

    // We now pass a copy of the data and the filename to the detached thread.
    thread_and_forget(save_routine, dir_filename, file_contents);
  }
}

// -------------------------------------------------------------------------------------


#endif