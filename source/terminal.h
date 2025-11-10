// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: terminal.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef TERMINAL_H
#define TTERMINAL_H

#include <iostream>

#include <string>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <pty.h>
#include <thread>
#include <regex>
#include <signal.h>   // For SIGKILL and other signal constants

using namespace std;

// ---------------------------------------------------------------------------------------

// --- ASSUMED STRUCTS/ENUMS FROM TERMINAL.H (Necessary for SGR logic) ---
enum Color 
{
  DEFAULT_C = 0,
  BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37,
  // Bright colors (commonly used when SGR 1 (Bold) is set)
  BRIGHT_BLACK = 90, BRIGHT_RED = 91, BRIGHT_GREEN = 92, BRIGHT_YELLOW = 93, BRIGHT_BLUE = 94, BRIGHT_MAGENTA = 95, BRIGHT_CYAN = 96, BRIGHT_WHITE = 97,
  // Background colors start at 40
  BG_BLACK = 40, BG_RED = 41, BG_GREEN = 42, BG_YELLOW = 43, BG_BLUE = 44, BG_MAGENTA = 45, BG_CYAN = 46, BG_WHITE = 47,
  // Bright background colors start at 100
  BG_BRIGHT_BLACK = 100, BG_BRIGHT_RED = 101, BG_BRIGHT_GREEN = 102, BG_BRIGHT_YELLOW = 103, BG_BRIGHT_BLUE = 104, BG_BRIGHT_MAGENTA = 105, BG_BRIGHT_CYAN = 106, BG_BRIGHT_WHITE = 107
};

// --- RGB Color Struct for Terminal Overlays and True Color Support ---
struct RgbColor 
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  // Default constructor: black (0,0,0)
  RgbColor() : r(0), g(0), b(0) {}

  // Parameterized constructor
  RgbColor(uint8_t red, uint8_t green, uint8_t blue)
      : r(red), g(green), b(blue) {}
};

// --- Comparison Operators for RgbColor ---
inline bool operator==(const RgbColor& lhs, const RgbColor& rhs) 
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

inline bool operator!=(const RgbColor& lhs, const RgbColor& rhs) 
{
  return !(lhs == rhs);
}

// Define standard/default ANSI colors in RGB format
// This mapping is necessary to handle standard SGR codes (30-37, 40-47)
// The "default" terminal FG is typically light gray, BG is black.
const RgbColor DEFAULT_FG_COLOR = {204, 204, 204};
const RgbColor DEFAULT_BG_COLOR = {0, 0, 0};

// Map ANSI 30-37 (FG) and 40-47 (BG) codes to specific RGB values for 
// backward compatibility.
// (R, G, B)
static const RgbColor ANSI_COLORS[8] = 
{
  {0, 0, 0},         // Black
  {205, 0, 0},       // Red
  {0, 205, 0},       // Green
  {205, 205, 0},     // Yellow
  {0, 0, 238},       // Blue
  {205, 0, 205},     // Magenta
  {0, 205, 205},     // Cyan
  {229, 229, 229}    // White
};

// Map Bright ANSI 90-97 (FG) and 100-107 (BG) codes
static const RgbColor BRIGHT_ANSI_COLORS[8] = 
{
  {127, 127, 127},   // Bright Black
  {255, 0, 0},       // Bright Red
  {0, 255, 0},       // Bright Green
  {255, 255, 0},     // Bright Yellow
  {92, 92, 255},     // Bright Blue
  {255, 0, 255},     // Bright Magenta
  {0, 255, 255},     // Bright Cyan
  {255, 255, 255}    // Bright White
};

// Update the Cell structure to use RgbColor
struct Cell 
{
  char32_t character = U' ';
  RgbColor foreground_color = DEFAULT_FG_COLOR;
  RgbColor background_color = DEFAULT_BG_COLOR;
  bool is_bold = false;
  bool is_reverse = false; // NEW REVERSE FLAG
  // Other attributes like underline, italic, etc. can be added here
};

// -----------------------------------------------------

char32_t decode_next_utf8_char(const std::string& text, size_t& i);

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val);

// -----------------------------------------------------

class TERMINAL
{
  private:

  pid_t PID;
  std::thread T;
  
  int MASTER_FD = -1;

  void start_shell();

  void clear_row_range_full_line(int row);
  void scroll_up(int count);
  void scroll_down(int count);
  void insert_line(int count);
  void delete_line(int count);

  bool process_csi_cursor_movement(char final_char, const std::vector<int>& params);
  bool process_csi_erase_and_edit(char final_char, const std::vector<int>& params);
  bool process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private);
  bool process_csi_sgr_and_misc(char final_char, const std::vector<int>& params);
  char32_t map_graphics_char(char32_t ascii_char) const;
  
  void process_output(const std::string& raw_text);

  public:

  // Constants for screen dimensions
  static const int ROWS = 26;
  //static const int COLS = 99;
  static const int COLS = 87;

  // Buffer containing the terminal screen
  Cell SCREEN[ROWS][COLS]; 
  int CURRENT_ROW;
  int CURRENT_COL;
  int SCROLL_TOP;
  int SCROLL_BOTTOM;
  int SAVED_ROW; 
  int SAVED_COL;
  bool APP_CURSOR_MODE; 
  bool AUTO_WRAP_MODE;
  bool REVERSE_WRAP_MODE;
  bool APP_KEYPAD_MODE;
  bool IS_GRAPHICS_MODE;

  std::mutex BUF_MUTEX;

  TERMINAL();
  ~TERMINAL();

  std::mutex& get_mutex();

  std::string get_line_text(int row) const;
  std::string get_line_text_reverse_map(int row) const;
  std::string get_line_text_reverse(int row) const;

  void reader_thread();
  void create();
  void send_command(const std::string& charaters);
};

// ---------------------------------------------------------------------------------------

#endif