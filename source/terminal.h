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

#include <iomanip>

#include <string>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <pty.h>
#include <thread>
#include <regex>
#include <signal.h>   // For SIGKILL and other signal constants
#include <sys/wait.h> // For waitpid

using namespace std;

// ---------------------------------------------------------------------------------------

// ANSI escape codes start with 0x1B (ESC).
const char ESC = 0x1B;
// Control Sequence Introducer (CSI) is ESC followed by [
const std::string CSI = "\033[";

/**
 * @brief Tracks the state of ANSI escape sequence parsing in a buffer.
 * * This is necessary to ensure we only process a buffer chunk that does not
 * end in the middle of a control sequence (e.g., ESC[32;1).
 */
class AnsiStateTracker 
{
public:
  enum State 
  {
    NORMAL,           // Not in an escape sequence.
    IN_ESC_SEQUENCE,  // Received ESC (0x1B), expecting [ or other code.
    IN_CSI_SEQUENCE   // Received ESC[, expecting parameters and a final character.
  };

private:
  State current_state_ = NORMAL;

public:
  AnsiStateTracker() = default;

  bool parse_and_check_completion(const std::string& data);
  
  // Getter for debugging
  State get_state() const 
  {
    return current_state_;
  }
};

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
  bool is_underline = false;
  bool is_reverse = false; // NEW REVERSE FLAG
  // Other attributes like underline, italic, etc. can be added here
};

// -----------------------------------------------------

char32_t decode_next_utf8_char(const std::string& text, size_t& i);

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val);

// -----------------------------------------------------

/*
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

  bool process_csi_cursor_movement(char final_char, const std::vector<int>& params);
  bool process_csi_erase_and_edit(char final_char, const std::vector<int>& params);
  bool process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private);
  bool process_csi_sgr_and_misc(char final_char, const std::vector<int>& params);
  char32_t map_graphics_char(char32_t ascii_char) const;
  
  void process_output(const std::string& raw_text);

// ---------------------------------------------------------------------

  bool process_output_2_enable = true;

  void reset();

  void screen_scoll_down();
  void screen_scoll_up();
  void screen_clear();
  void screen_erase_to_end();
  void screen_erase_line_to_end();
  
  void cursor_up();
  void cursor_down();
  void cursor_left();
  void cursor_right();

  void control_BS();
  void control_HT();
  void control_LF();
  void control_CR();
  
  void control_RI();
  void control_HTS();

  void bracket_device_attribute();
  void bracket_cursor_home();
  void bracket_erase_in_display(int mode);
  void bracket_erase_in_line(int mode);
  void bracket_delete_character(int count);
  void bracket_set_reset_mode(std::string params, bool set);
  void bracket_set_margins(std::string params);
  void bracket_select_graphic_rendition(std::string params);

  void control_characters(uint8_t Character);

  void handle_simple_escape(uint8_t command);
  void handle_bracket_escape(std::string sequence);
  void handle_parameterized_escape(std::string Raw_Text);

  bool escape_characters(std::string Raw_Text, int &End_Position);

  void write_to_screen(char32_t Character);

  string PROCESS_OUTPUT_2_BUFFER = "";
  void process_output_2(std::string& raw_text);


// ---------------------------------------------------------------------


  public:

  // Constants for screen dimensions
  static const int ROWS = 26;
  //static const int COLS = 99;
  static const int COLS = 87;

  // Buffer containing the terminal screen
  Cell SCREEN[ROWS][COLS]; 
  bool TAB_STOPS[COLS];
  int CURRENT_ROW;
  int CURRENT_COL;
  int SCROLL_TOP;
  int SCROLL_BOTTOM;
  bool APP_CURSOR_MODE; 
  bool AUTO_WRAP_MODE;
  bool REVERSE_WRAP_MODE;
  bool APP_KEYPAD_MODE;
  bool IS_GRAPHICS_MODE;
  bool ANSI_MODE;
  bool WRAP_PENDING = false;
  bool BRACKETED_PASTE_MODE = false;
  bool CURSOR_VISIBLE = true;
  bool CURSOR_BLINK = true;
  bool USE_ALT_SCREEN = false;
  
  int SAVED_ROW; 
  int SAVED_COL;
  bool SAVED_AUTO_WRAP_MODE;
  Cell SAVED_CURRENT_ATTRS;
  
  const Cell DEFAULT_CELL; 
  // Global tracker for current cell attributes
  Cell CURRENT_ATTRS = DEFAULT_CELL;

  string ID_RESPONSE;

  std::mutex BUF_MUTEX;

  //deque<std::string> MESSAGES;
  bool SHELL_EXITED = false;

  TERMINAL();
  ~TERMINAL();

  std::mutex& get_mutex();

  void get_line_text(int row, bool cursor_on, 
                      std::string &line, 
                      std::string &line_reverse, 
                      std::string &line_reverse_map);

  void reader_thread();
  void create();
  void send_command(const std::string& charaters);
};
*/

class TERMINAL 
{
private:
    /** --- Shell & Process Management --- */
    pid_t PID;                          // Process ID of the child shell (e.g., /bin/bash)
    std::thread T;                      // Thread dedicated to reading from the master PTY
    int MASTER_FD = -1;                 // File descriptor for the PTY master side

    void start_shell();                 // Fork and exec the shell process

    /** --- Internal Screen Manipulation --- */
    void clear_row_range_full_line(int row);
    void scroll_up(int count);          // General scroll up by N lines
    void scroll_down(int count);        // General scroll down by N lines

    /** --- CSI (Control Sequence Introducer) Specialized Parsers --- */
    // Handles Cursor Position, Up, Down, Forward, Back (CSI A, B, C, D, H, f, G, d)
    bool process_csi_cursor_movement(char final_char, const std::vector<int>& params);
    
    // Handles Erase in Display/Line, Delete, Insert (CSI J, K, P, L, M)
    bool process_csi_erase_and_edit(char final_char, const std::vector<int>& params);
    
    // Handles DEC Private Modes, Reporting, and Cursor Visibility (CSI ?...h/l, CSI c, CSI n)
    bool process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private);
    
    // Handles Colors, Bold, Reverse, and specialized VT modes (CSI m, CSI q)
    bool process_csi_sgr_and_misc(char final_char, const std::vector<int>& params);
    
    // Maps ASCII to DEC Special Graphics characters (for line drawing)
    char32_t map_graphics_char(char32_t ascii_char) const;
    
    // Original output processor (v1)
    void process_output(const std::string& raw_text);

    /** --- Output Processor v2: Refined ANSI/VT100 Logic --- */
    bool process_output_2_enable = false;

    void reset();                       // Hard reset of terminal state

    void screen_scoll_down();           // Single row scroll within margins (Reverse Index logic)
    void screen_scoll_up();             // Single row scroll within margins (Line Feed logic)
    void screen_clear();                // Clears entire buffer and resets cursor
    void screen_erase_to_end();         // Clears from cursor to end of screen
    void screen_erase_line_to_end();    // Clears from cursor to end of line
    
    void cursor_up();                   // Move cursor up 1 (capped)
    void cursor_down();                 // Move cursor down 1 (capped)
    void cursor_left();                 // Move cursor left 1 (capped)
    void cursor_right();                // Move cursor right 1 (capped)

    void control_BS();                  // Backspace (0x08)
    void control_HT();                  // Horizontal Tab (0x09)
    void control_LF();                  // Line Feed (0x0A) - handles scrolling
    void control_CR();                  // Carriage Return (0x0D)
    
    void control_RI();                  // Reverse Index (ESC M)
    void control_HTS();                 // Horizontal Tab Set (ESC H)

    void bracket_device_attribute();    // CSI c (Terminal ID)
    void bracket_cursor_home();         // CSI H (Reset to 0,0)
    void bracket_erase_in_display(int mode); // CSI J
    void bracket_erase_in_line(int mode);    // CSI K
    void bracket_delete_character(int count); // CSI P
    void bracket_set_reset_mode(std::string params, bool set); // CSI h/l
    void bracket_set_margins(std::string params);              // CSI r
    void bracket_select_graphic_rendition(std::string params); // CSI m (SGR)

    void control_characters(uint8_t Character); // Handles C0 codes (0x00-0x1F)

    void handle_simple_escape(uint8_t command); // ESC followed by 1 byte
    void handle_bracket_escape(std::string sequence); // Full CSI sequence
    void handle_parameterized_escape(std::string Raw_Text);

    // High-level escape sequence identifying logic
    bool escape_characters(std::string Raw_Text, int &End_Position);

    void write_to_screen(char32_t Character); // The "Pen": writes data to buffer

    std::string PROCESS_OUTPUT_2_BUFFER = ""; // Fragment buffer for ESC sequences
    void process_output_2(std::string& raw_text); // v2 Entry point

public:
    static const int ROWS = 26;
    static const int COLS = 87;

    /** --- Terminal State & Buffers --- */
    Cell SCREEN[ROWS][COLS];            // Primary grid of characters/attributes
    bool TAB_STOPS[COLS];               // Column-indexed tab stop flags
    int CURRENT_ROW;                    // Cursor Row (0 to ROWS-1)
    int CURRENT_COL;                    // Cursor Column (0 to COLS-1)
    int SCROLL_TOP;                     // Top margin for scrolling (usually 0)
    int SCROLL_BOTTOM;                  // Bottom margin for scrolling (usually ROWS-1)
    
    /** --- DEC/ANSI Modes --- */
    bool APP_CURSOR_MODE;               // Send specialized sequences for arrows
    bool AUTO_WRAP_MODE;                // Wrap to next line at COLS
    bool REVERSE_WRAP_MODE;             // Backspace wraps to previous line
    bool APP_KEYPAD_MODE;               // Specialized keypad behavior
    bool IS_GRAPHICS_MODE;              // Use line-drawing character set
    bool ANSI_MODE;                     // VT100 (true) vs VT52 (false)
    bool WRAP_PENDING = false;          // True if cursor is at edge waiting for next char
    bool BRACKETED_PASTE_MODE = false;  // Wrap pasted text in start/end markers
    bool CURSOR_VISIBLE = true;         // DECTCEM
    bool CURSOR_BLINK = true;
    bool USE_ALT_SCREEN = false;        // Main buffer vs Alternate buffer

    /** --- Saved States (ESC 7 / ESC 8) --- */
    int SAVED_ROW; 
    int SAVED_COL;
    bool SAVED_AUTO_WRAP_MODE;
    Cell SAVED_CURRENT_ATTRS;
    
    const Cell DEFAULT_CELL;            // Baseline cell (empty, default colors)
    Cell CURRENT_ATTRS = DEFAULT_CELL;  // Active pen attributes for new characters

    std::string ID_RESPONSE;            // Terminal ID string to send back to shell

    std::mutex BUF_MUTEX;               // Protects screen buffers during thread access
    bool SHELL_EXITED = false;          // Flag for thread termination

    TERMINAL();
    ~TERMINAL();

    /** --- External Interface for UI --- */
    std::mutex& get_mutex();

    // Extracts text and attribute maps for UI rendering
    void get_line_text(int row, bool cursor_on, 
                       std::string &line, 
                       std::string &line_reverse, 
                       std::string &line_reverse_map);

    void reader_thread();               // Background process reading from PTY
    void create();                      // Initialization and shell launch
    void send_command(const std::string& characters); // Send user input to PTY
};

// ---------------------------------------------------------------------------------------

#endif