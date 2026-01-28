// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: terminal.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef TERMINAL_CPP
#define TERMINAL_CPP

#include "terminal.h"

// ---------------------------------------------------------------------------------------

/**
 * @brief Helper to parse CSI parameters (e.g., "31;1" -> {31, 1})
 */
std::vector<int> parse_csi_params(const std::string& sequence) 
{
  std::vector<int> params;
  if (sequence.size() < 3) return params;

  // Remove ESC [ and the final character
  std::string content = sequence.substr(2, sequence.size() - 3);
  if (content.empty()) return params;

  // Handle DEC Private Mode prefix '?'
  if (content[0] == '?') 
  {
    content.erase(0, 1);
  }

  std::stringstream ss(content);
  std::string segment;
  while (std::getline(ss, segment, ';')) 
  {
    try 
    {
      if (!segment.empty()) params.push_back(std::stoi(segment));
      else params.push_back(0); // Treat empty segments as 0
    } 
    catch (...) 
    {
      params.push_back(0);
    }
  }
  return params;
}

/**
 * @brief Helper to safely get parameters with a default value
 */
int get_param(const std::vector<int>& params, size_t index, int default_val) 
{
  if (index >= params.size()) return default_val;
  return params[index] == 0 ? default_val : params[index];
}

// ---------------------------------------------------------------------------------------

/**
 * @brief Parses a chunk of data and updates the internal state.
 * * @param data The new chunk of data to parse.
 * @return true if the buffer *ends* in a NORMAL state (i.e., no
 * unterminated escape sequence).
 */
bool AnsiStateTracker::parse_and_check_completion(const std::string& data) 
{
  for (char c : data) 
  {
    switch (current_state_) 
    {
      case NORMAL:
        if (c == ESC) 
        {
          current_state_ = IN_ESC_SEQUENCE;
        }
        break;
      
      case IN_ESC_SEQUENCE:
        if (c == '[') 
        {
          current_state_ = IN_CSI_SEQUENCE;
        } 
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) 
        {
          // Single-character ESC sequences (like ESC c for reset)
          current_state_ = NORMAL; 
        } 
        else 
        {
          // Unrecognized or complex ESC sequence starter, reset or stay
          // For simplicity, we assume any final byte ends it if not '['.
          // A more robust implementation handles intermediate bytes 0x30-0x3F.
          current_state_ = NORMAL;
        }
        break;
    
      case IN_CSI_SEQUENCE:
        // CSI sequences end when a character from 0x40 to 0x7E is received.
        if (c >= '@' && c <= '~') 
        {
          current_state_ = NORMAL;
        }
        // Intermediate bytes (0x30-0x3F) are consumed but do not change state.
        break;
    }
  }
  return current_state_ == NORMAL;
}

// ---------------------------------------------------------------------------------------

/**
 * Maps ASCII characters to their Unicode equivalents when in DEC Special Graphics mode.
 */
char32_t translate_dec_graphics(char32_t cp) {
  switch (cp) 
  {
    case 'q': return U'─'; // Horizontal line
    case 'x': return U'│'; // Vertical line
    case 'l': return U'┌'; // Top left corner
    case 'm': return U'└'; // Bottom left corner
    case 'k': return U'┐'; // Top right corner
    case 'j': return U'┘'; // Bottom right corner
    case 'u': return U'┤'; // Right tee
    case 't': return U'├'; // Left tee
    case 'v': return U'┴'; // Bottom tee
    case 'w': return U'┬'; // Top tee
    case 'n': return U'┼'; // Plus / crossing
    case 'a': return U'▒'; // Checkerboard
    case '_': return U' '; // Blank
    case '`': return U'◆'; // Diamond
    case 'h': return U'#'; // Board
    case 'f': return U'°'; // Degree
    case 'g': return U'±'; // Plus-minus
    case '~': return U'·'; // Bullet
    default: return cp;
  }
}

/**
 * Enhanced UTF-8 decoder that detects incomplete sequences.
 */
char32_t decode_utf8_safe(const std::string& text, size_t& i, bool& incomplete) 
{
  incomplete = false;
  if (i >= text.size()) return U'\ufffd';

  uint8_t b1 = (uint8_t)text[i];
  size_t needed = 0;

  if (b1 <= 0x7F) 
  {
    return (char32_t)text[i++];
  } 
  else if ((b1 & 0xE0) == 0xC0) 
  {
    needed = 2;
  } 
  else if ((b1 & 0xF0) == 0xE0) 
  {
    needed = 3;
  } 
  else if ((b1 & 0xF8) == 0xF0) 
  {
    needed = 4;
  } 
  else 
  {
    i++;
    return U'\ufffd';
  }

  if (i + needed > text.size()) 
  {
    incomplete = true;
    return 0;
  }

  char32_t cp = 0;
  bool valid = true;

  if (needed == 2) 
  {
    uint8_t b2 = (uint8_t)text[i + 1];
    if ((b2 & 0xC0) != 0x80) valid = false;
    cp = ((b1 & 0x1F) << 6) | (b2 & 0x3F);
  } 
  else if (needed == 3) 
  {
    uint8_t b2 = (uint8_t)text[i + 1];
    uint8_t b3 = (uint8_t)text[i + 2];
    if ((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) valid = false;
    cp = ((b1 & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
  } 
  else if (needed == 4) 
  {
    uint8_t b2 = (uint8_t)text[i + 1];
    uint8_t b3 = (uint8_t)text[i + 2];
    uint8_t b4 = (uint8_t)text[i + 3];
    if ((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80 || (b4 & 0xC0) != 0x80) valid = false;
    cp = ((b1 & 0x07) << 18) | ((b2 & 0x3F) << 12) | ((b3 & 0x3F) << 6) | (b4 & 0x3F);
  }

  if (!valid || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) 
  {
    i++; 
    return U'\ufffd';
  }

  i += needed;
  return cp;
}


// ---------------------------------------------------------------------------------------

// Spawn shell inside PTY
void TERMINAL::start_shell() 
{
  // Define the terminal window size structure based on our assumed ROWS and COLS
  struct winsize ws;
  ws.ws_row = ROWS; 
  ws.ws_col = COLS;
  ws.ws_xpixel = 0;
  ws.ws_ypixel = 0;

  // Use forkpty, passing the winsize structure pointer as the fourth argument.
  // MASTER_FD is populated with the master side FD (for the parent/emulator).
  PID = forkpty(&MASTER_FD, nullptr, nullptr, &ws); 

  if (PID == 0) 
  {
    // ------------------------------------------------------------------
    // CHILD PROCESS: CRITICAL FIX FOR GLIBC/STDIO HANDLE ERROR
    // The child process inherits a copy of the MASTER_FD. 
    // It must close this descriptor before exec() to prevent I/O conflicts.
    if (MASTER_FD > 0) {
        close(MASTER_FD);
    }
    // ------------------------------------------------------------------
    
    // Child process: Execute the shell
    execl("/bin/bash", "bash", nullptr);
    
    // This line should only be reached if execl fails
    perror("execl failed");
    exit(1); 
  } 
  else if (PID > 0) 
  {
    // Parent process: Immediately explicitly set the terminal size again 
    // using ioctl. (This is correct in the parent.)
    ioctl(MASTER_FD, TIOCSWINSZ, &ws);
  }
}

/**
 * @brief Helper function to clear an entire line within the terminal screen.
 * It resets all cells in the specified row to DEFAULT_CELL.
 */
void TERMINAL::clear_row_range_full_line(int row)
{
  // Clear the entire row from column 0 up to COLS
  std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL);
}

// ---------------------------------------------------------------------
// --- CORE SCROLLING MECHANISMS ---
// ---------------------------------------------------------------------

/**
 * @brief Scrolls lines up within the scroll region (SCROLL_TOP to SCROLL_BOTTOM).
 * The lines at the top of the region are discarded, and new empty lines appear at the bottom.
 * This is used for standard line feed/overflow behavior.
 */
void TERMINAL::scroll_up(int count)
{
  if (count <= 0 || SCROLL_TOP > SCROLL_BOTTOM) 
  {
    return;
  }

  // Clamp count to the size of the scroll region
  count = std::min(count, SCROLL_BOTTOM - SCROLL_TOP + 1);

  // 1. Move lines up: Iterate forward.
  // Source: [SCROLL_TOP + count, SCROLL_BOTTOM]
  // Destination: [SCROLL_TOP, SCROLL_BOTTOM - count]
  for (int r = SCROLL_TOP; r <= SCROLL_BOTTOM - count; ++r) 
  {
    // Copy contents of SCREEN[r + count] to SCREEN[r]
    std::copy(SCREEN[r + count], 
              SCREEN[r + count] + COLS, 
              SCREEN[r]);
  }

  // 2. Clear the newly opened lines at the bottom of the region.
  // Lines to clear: [SCROLL_BOTTOM - count + 1, SCROLL_BOTTOM]
  for (int r = SCROLL_BOTTOM - count + 1; r <= SCROLL_BOTTOM; ++r) 
  {
    this->clear_row_range_full_line(r);
  }
}

/**
 * @brief Scrolls lines down within the scroll region (SCROLL_TOP to SCROLL_BOTTOM).
 * The lines at the bottom of the region are discarded, and new empty lines appear at the top.
 * This is used for Reverse Index (ESC M) and Line Insert (ESC [ L) sequences.
 */
void TERMINAL::scroll_down(int count)
{
  if (count <= 0 || SCROLL_TOP > SCROLL_BOTTOM) 
  {
    return;
  }

  // Clamp count to the size of the scroll region
  count = std::min(count, SCROLL_BOTTOM - SCROLL_TOP + 1);

  // 1. Move lines down: Iterate backward.
  // Source Range: [SCROLL_TOP, SCROLL_BOTTOM - count]
  // Target Range: [SCROLL_TOP + count, SCROLL_BOTTOM]
  for (int r = SCROLL_BOTTOM; r >= SCROLL_TOP + count; --r) 
  {
    // Copy contents of SCREEN[r - count] to SCREEN[r]
    std::copy(SCREEN[r - count], 
              SCREEN[r - count] + COLS, 
              SCREEN[r]);
  }

  // 2. Clear the newly opened lines at the top of the region.
  // Lines to clear: [SCROLL_TOP, SCROLL_TOP + count - 1]
  for (int r = SCROLL_TOP; r < SCROLL_TOP + count; ++r) 
  {
    this->clear_row_range_full_line(r);
  }
}

bool TERMINAL::process_csi_cursor_movement(char final_char, const std::vector<int>& params) 
{
  switch (final_char) 
  {
    case 'A': // Cursor Up (CUU)
      CURRENT_ROW = std::max(SCROLL_TOP, CURRENT_ROW - get_param(params, 0, 1));
      WRAP_PENDING = false;
      break;

    case 'B': // Cursor Down (CUD)
      CURRENT_ROW = std::min(SCROLL_BOTTOM, CURRENT_ROW + get_param(params, 0, 1));
      WRAP_PENDING = false;
      break;

    case 'C': // Cursor Forward (CUF)
      CURRENT_COL = std::min(COLS - 1, CURRENT_COL + get_param(params, 0, 1));
      WRAP_PENDING = false;
      break;

    case 'D': // Cursor Back (CUB)
      CURRENT_COL = std::max(0, CURRENT_COL - get_param(params, 0, 1));
      WRAP_PENDING = false;
      break;

    case 'H': // Cursor Position (CUP)
    case 'f': // HVP
    case 'd': // VPA (Vertical Line Position Absolute)
    {
      int req_row = get_param(params, 0, 1);
      int req_col = get_param(params, 1, 1);

      if (final_char == 'H' || final_char == 'f' || final_char == 'd') 
      {
        int new_row = SCROLL_TOP + req_row - 1;
        CURRENT_ROW = std::min(SCROLL_BOTTOM, std::max(SCROLL_TOP, new_row));
      }
      if (final_char == 'H' || final_char == 'f') 
      {
        CURRENT_COL = std::min(COLS - 1, std::max(0, req_col - 1));
      }
      WRAP_PENDING = false;
      break;
    }

    case 'G': // Cursor Horizontal Absolute (CHA)
      CURRENT_COL = std::min(COLS - 1, std::max(0, get_param(params, 0, 1) - 1));
      WRAP_PENDING = false;
      break;

    default:
      return false;
  }
  return true;
}

/**
 * @brief Handles CSI sequences for screen/line editing (P, X, K, J, L, M, @).
 * Assumes SCREEN is a 2D array of CELL objects/structs and DEFAULT_CELL is a blank cell object.
 */
bool TERMINAL::process_csi_erase_and_edit(char final_char, const std::vector<int>& params)
{
  int param = get_param(params, 0, 1); 
  
  auto clear_row_range = [&](int start_col, int end_col) 
  {
    if (CURRENT_ROW >= 0 && CURRENT_ROW < ROWS) {
      std::fill(SCREEN[CURRENT_ROW] + start_col, 
            SCREEN[CURRENT_ROW] + end_col, 
            DEFAULT_CELL);
    }
  };

  switch (final_char)
  {
    case 'b': // Repeat Character (REP): ESC [ P b
    {
      // Repeat the last printed character 'param' times.
      // 1. Identify the character to repeat. 
      // Traditionally, this is the character at the cell immediately to the left of the cursor.
      int count = std::max(0, param);
      int source_col = CURRENT_COL - 1;

      if (source_col >= 0 && CURRENT_ROW >= 0 && CURRENT_ROW < ROWS)
      {
        Cell char_to_repeat = SCREEN[CURRENT_ROW][source_col];
        
        // 2. Insert 'count' instances of that character
        for (int i = 0; i < count; ++i)
        {
          // We use the terminal's standard printing logic (or simplified version here)
          // Handle wrapping if AUTO_WRAP_MODE is on
          if (CURRENT_COL >= COLS)
          {
            if (AUTO_WRAP_MODE)
            {
              CURRENT_COL = 0;
              control_LF(); // Move down/scroll
            }
            else
            {
              CURRENT_COL = COLS - 1; // Stay at edge
              break; 
            }
          }

          SCREEN[CURRENT_ROW][CURRENT_COL] = char_to_repeat;
          CURRENT_COL++;
        }
        WRAP_PENDING = false;
      }
      break;
    }

    case 'P': // Delete Character (DCH): ESC [ P P
    {
      int count = std::max(0, param); 
      if (CURRENT_COL < COLS) 
      {
        int src_start_col = CURRENT_COL + count;
        int shift_cells = std::max(0, COLS - src_start_col);
        if (shift_cells > 0)
        {
          std::copy(SCREEN[CURRENT_ROW] + src_start_col, 
                    SCREEN[CURRENT_ROW] + COLS, 
                    SCREEN[CURRENT_ROW] + CURRENT_COL);
        }
        int fill_start_col = CURRENT_COL + shift_cells;
        clear_row_range(fill_start_col, COLS);
      }
      break;
    }

    case '@': // Insert Character (ICH): ESC [ P @
    {
      int count = std::max(0, param);
      if (CURRENT_COL < COLS) 
      {
        auto src_start = SCREEN[CURRENT_ROW] + CURRENT_COL;
        int cells_to_shift = COLS - CURRENT_COL - count;
        if (cells_to_shift > 0)
        {
          auto actual_src_end = src_start + cells_to_shift; 
          auto dst_start = src_start + count;
          std::copy_backward(actual_src_end - cells_to_shift, 
                               actual_src_end, 
                               dst_start + cells_to_shift);
        }
        int fill_limit = std::min(COLS, CURRENT_COL + count);
        clear_row_range(CURRENT_COL, fill_limit);
      }
      break;
    }

    case 'X': // Erase Character (ECH): ESC [ P X
    {
      int count = std::max(0, param);
      int end_col = std::min(COLS, CURRENT_COL + count);
      clear_row_range(CURRENT_COL, end_col);
      break;
    }

    case 'K': // Erase in Line (EL)
    {
      int param_k = get_param(params, 0, 0); 
      if (param_k == 0) clear_row_range(CURRENT_COL, COLS);
      else if (param_k == 1) clear_row_range(0, CURRENT_COL + 1);
      else if (param_k == 2) clear_row_range(0, COLS);
      break;
    }

    case 'J': // Erase Display (ED)
    {
      int param_j = get_param(params, 0, 0); 
      if (param_j == 2) 
      { 
        for (int row = 0; row < ROWS; ++row) this->clear_row_range_full_line(row);
        CURRENT_ROW = 0; CURRENT_COL = 0;
      } 
      else if (param_j == 0) 
      { 
        clear_row_range(CURRENT_COL, COLS);
        for (int row = CURRENT_ROW + 1; row < ROWS; ++row) this->clear_row_range_full_line(row);
      } 
      else if (param_j == 1) 
      { 
        for (int row = 0; row < CURRENT_ROW; ++row) this->clear_row_range_full_line(row);
        clear_row_range(0, CURRENT_COL + 1);
      }
      break;
    }

    case 'L': // Insert Line (IL)
    { 
      int count = std::max(0, param);
      int effective_count = std::min(count, SCROLL_BOTTOM - CURRENT_ROW + 1);
      for (int r = SCROLL_BOTTOM; r >= CURRENT_ROW + effective_count; --r) 
        std::copy(SCREEN[r - effective_count], SCREEN[r - effective_count] + COLS, SCREEN[r]);
      for (int r = CURRENT_ROW; r < CURRENT_ROW + effective_count; ++r) 
        this->clear_row_range_full_line(r);
      break;
    }

    case 'M': // Delete Line (DL)
    { 
      int count = std::max(0, param);
      int effective_count = std::min(count, SCROLL_BOTTOM - CURRENT_ROW + 1);
      for (int r = CURRENT_ROW; r <= SCROLL_BOTTOM - effective_count; ++r) 
        std::copy(SCREEN[r + effective_count], SCREEN[r + effective_count] + COLS, SCREEN[r]);
      for (int r = SCROLL_BOTTOM - effective_count + 1; r <= SCROLL_BOTTOM; ++r) 
        this->clear_row_range_full_line(r);
      break;
    }

    default: return false;
  }
  return true;
}

bool TERMINAL::process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private) 
{
  switch (final_char) 
  {
    case 'n': // DSR
      if (get_param(params, 0, 0) == 6 && MASTER_FD > 0) 
      {
        std::string res = "\x1B[" + std::to_string(CURRENT_ROW + 1) + ";" + std::to_string(CURRENT_COL + 1) + "R";
        write(MASTER_FD, res.c_str(), res.size());
      }
      break;

    case 'c': // DA
      if (MASTER_FD > 0) 
      {
        std::string res = "\x1B[?1;2c"; 
        write(MASTER_FD, res.c_str(), res.size());
      }
      break;

    case 'r': // DECSTBM
    {
      int top = get_param(params, 0, 1);
      int bottom = get_param(params, 1, ROWS);
      SCROLL_TOP = std::min(ROWS - 1, std::max(0, top - 1));
      SCROLL_BOTTOM = std::min(ROWS - 1, std::max(SCROLL_TOP, bottom - 1));
      CURRENT_ROW = SCROLL_TOP;
      CURRENT_COL = 0;
      WRAP_PENDING = false;
      break;
    }

    case 's': SAVED_ROW = CURRENT_ROW; SAVED_COL = CURRENT_COL; break;
    case 'u': CURRENT_ROW = SAVED_ROW; CURRENT_COL = SAVED_COL; WRAP_PENDING = false; break;

    case 'h': 
    case 'l': 
    {
      if (is_dec_private && !params.empty()) 
      {
        int mode = params[0];
        bool set = (final_char == 'h');
        if (mode == 1) APP_CURSOR_MODE = set;
        else if (mode == 2) APP_KEYPAD_MODE = set;
        else if (mode == 7) AUTO_WRAP_MODE = set;
        else if (mode == 45) REVERSE_WRAP_MODE = set;
      }
      break;
    }

    case 't': // Window Manipulation
    case 'g': // Tab Clear
      break;

    default:
      return false;
  }
  return true;
}

bool TERMINAL::process_csi_sgr_and_misc(char final_char, const std::vector<int>& params) 
{
  if (final_char != 'm') return false;

  if (params.empty()) 
  {
    CURRENT_ATTRS = DEFAULT_CELL;
    return true; 
  }

  for (size_t i = 0; i < params.size(); ++i) 
  {
    int code = params[i];
    if (code == 0) CURRENT_ATTRS = DEFAULT_CELL;
    else if (code == 1) CURRENT_ATTRS.is_bold = true;
    else if (code == 22) CURRENT_ATTRS.is_bold = false;
    else if (code == 3) CURRENT_ATTRS.is_italic = true;
    else if (code == 23) CURRENT_ATTRS.is_italic = false;
    else if (code == 4) CURRENT_ATTRS.is_underline = true;
    else if (code == 24) CURRENT_ATTRS.is_underline = false;
    else if (code == 5) CURRENT_ATTRS.is_blink = true;
    else if (code == 25) CURRENT_ATTRS.is_blink = false;
    else if (code == 7) CURRENT_ATTRS.is_reverse = true;
    else if (code == 27) CURRENT_ATTRS.is_reverse = false;
    else if (code == 8) CURRENT_ATTRS.is_hidden = true;
    else if (code == 28) CURRENT_ATTRS.is_hidden = false;
    else if (code == 9) CURRENT_ATTRS.is_strikethrough = true;
    else if (code == 29) CURRENT_ATTRS.is_strikethrough = false;
    else if (code >= 30 && code <= 37) { CURRENT_ATTRS.foreground_color = ANSI_COLORS[code - 30]; CURRENT_ATTRS.is_reverse = false; }
    else if (code == 39) { CURRENT_ATTRS.foreground_color = DEFAULT_CELL.foreground_color; }
    else if (code >= 90 && code <= 97) { CURRENT_ATTRS.foreground_color = BRIGHT_ANSI_COLORS[code - 90]; CURRENT_ATTRS.is_reverse = false; }
    else if (code >= 40 && code <= 47) { CURRENT_ATTRS.background_color = ANSI_COLORS[code - 40]; CURRENT_ATTRS.is_reverse = false; }
    else if (code == 49) { CURRENT_ATTRS.background_color = DEFAULT_CELL.background_color; }
    else if (code >= 100 && code <= 107) { CURRENT_ATTRS.background_color = BRIGHT_ANSI_COLORS[code - 100]; CURRENT_ATTRS.is_reverse = false; }
    else if (code == 38 || code == 48) 
    {
      if (i + 1 < params.size()) 
      {
        if (params[i+1] == 5) i += 2; // 256 color (stubbed index consumption)
        else if (params[i+1] == 2 && i + 4 < params.size()) 
        {
          RgbColor tc = {(uint8_t)params[i+2], (uint8_t)params[i+3], (uint8_t)params[i+4]};
          if (code == 38) CURRENT_ATTRS.foreground_color = tc; else CURRENT_ATTRS.background_color = tc;
          i += 4;
        }
      }
    }
  }
  return true;
}

// ---------------------------------------------------------------------
// --- process_output_2 ---
// ---------------------------------------------------------------------

void TERMINAL::reset()
{
  // 1. Reset Cursor Position
  CURRENT_ROW = 0;
  CURRENT_COL = 0;
  SAVED_ROW = 0;
  SAVED_COL = 0;
  SAVED_AUTO_WRAP_MODE = true;
  SAVED_CURRENT_ATTRS = DEFAULT_CELL;

  // 2. Reset Scroll Margins to full screen
  SCROLL_TOP = 0;
  SCROLL_BOTTOM = ROWS - 1;

  // 3. Reset Modes to Power-on Defaults
  APP_CURSOR_MODE = false;
  AUTO_WRAP_MODE = true;
  REVERSE_WRAP_MODE = false;
  APP_KEYPAD_MODE = false;
  IS_GRAPHICS_MODE = false;
  ANSI_MODE = true;
  WRAP_PENDING = false;
  BRACKETED_PASTE_MODE = false;
  CURSOR_VISIBLE = true;
  CURSOR_BLINK = true;
  USE_ALT_SCREEN = false;
  CURRENT_ATTRS = DEFAULT_CELL;

  // VT52 Identity response is traditionally fixed as ESC / Z
  // VT100 Identity (Device Attributes) response:
  // ESC [ ? 1 ; 2 c  indicates a VT100 with Advanced Video Option.
  ID_RESPONSE = "\033[?1;2c";

  // 4. Clear the screen buffer
  for (int i = 0; i < ROWS; ++i) 
  {
      std::fill(SCREEN[i], SCREEN[i] + COLS, DEFAULT_CELL);
  }

  // 5. Initialize Tab Stops: Set a stop every 8th column starting at column 8
  for (int j = 0; j < COLS; j++)
  {
      TAB_STOPS[j] = (j > 0 && j % 8 == 0);
  }
}

void TERMINAL::screen_scoll_down()
{
  for (int row_pos = SCROLL_BOTTOM; row_pos > SCROLL_TOP; row_pos--)
  {
    for (int col_pos = 0; col_pos < COLS; col_pos++)
    {
      SCREEN[row_pos][col_pos] = SCREEN[row_pos -1][col_pos];
    }
  }

  for (int col_pos = 0; col_pos < COLS; col_pos++)
  {
    SCREEN[SCROLL_TOP][col_pos] = CURRENT_ATTRS;
  }
}

void TERMINAL::screen_scoll_up()
{
  for (int row_pos = SCROLL_TOP + 1; row_pos <= SCROLL_BOTTOM; row_pos++)
  {
    for (int col_pos = 0; col_pos < COLS; col_pos++)
    {
      SCREEN[row_pos - 1][col_pos] = SCREEN[row_pos][col_pos];
    }
  }

  for (int col_pos = 0; col_pos < COLS; col_pos++)
  {
    SCREEN[SCROLL_BOTTOM][col_pos] = DEFAULT_CELL;
  }
}

void TERMINAL::screen_clear()
{
  Cell tmp_blank_cell;
  for (int row_pos = 0; row_pos < ROWS; row_pos++)
  {
    for (int col_pos = 0; col_pos < COLS; col_pos++)
    {
      SCREEN[row_pos][col_pos] = tmp_blank_cell;
    }
  }
  CURRENT_ROW = 0;
  CURRENT_COL = 0;
}

void TERMINAL::screen_erase_to_end()
{
  for (int r = CURRENT_ROW; r < ROWS; r++) 
  {
    int start_c = (r == CURRENT_ROW) ? CURRENT_COL : 0;
    for (int c = start_c; c < COLS; c++) 
    {
      SCREEN[r][c] = CURRENT_ATTRS;
    }
  }
}

void TERMINAL::screen_erase_line_to_end()
{
  // Clear only from the current column to the end of the current row
  for (int c = CURRENT_COL; c < COLS; c++) 
  {
    SCREEN[CURRENT_ROW][c] = CURRENT_ATTRS;
  }
}

void TERMINAL::cursor_up()
{
  if (CURRENT_ROW > 0)
  {
    CURRENT_ROW--;
  }
  WRAP_PENDING = false;
}

void TERMINAL::cursor_down()
{
  if (CURRENT_ROW < ROWS - 1)
  {
    CURRENT_ROW++;
  }
  WRAP_PENDING = false;
}

void TERMINAL::cursor_left()
{
  if (CURRENT_COL > 0)
  {
    CURRENT_COL--;
  }
  WRAP_PENDING = false;
}

void TERMINAL::cursor_right()
{
  if (CURRENT_COL < COLS - 1)
  {
    CURRENT_COL++;
  }
  WRAP_PENDING = false;
}

void TERMINAL::control_BS()
{
  CURRENT_COL --;
  if (CURRENT_COL < 0)
  {
    CURRENT_COL = 0;
  }
  WRAP_PENDING = false;
}

void TERMINAL::control_HT() 
{
  WRAP_PENDING = false;

  // Move to next tab stop
  for (int i = CURRENT_COL + 1; i < COLS; i++) 
  {
    if (TAB_STOPS[i]) 
    {
      CURRENT_COL = i;
      return;
    }
  }
  // If no more tab stops, move to the last column
  CURRENT_COL = COLS - 1;
}

void TERMINAL::control_LF()
{
  // Index: Move cursor down, scroll text UP if at bottom margin
  if (CURRENT_ROW == SCROLL_BOTTOM) 
  {
    screen_scoll_up(); 
  } 
  else if (CURRENT_ROW < ROWS - 1) 
  {
    CURRENT_ROW++;
  }
  WRAP_PENDING = false;
}

void TERMINAL::control_CR()
{
  CURRENT_COL = 0;
}

void TERMINAL::control_RI()
{
  // Reverse Index: Move cursor up, scroll if at top margin
  if (CURRENT_ROW == SCROLL_TOP) 
  {
    screen_scoll_down();
  } 
  else if (CURRENT_ROW > 0) 
  {
    CURRENT_ROW--;
  }
  WRAP_PENDING = false; // Manual movement clears pending wrap
}

void TERMINAL::control_HTS()
{
  if (CURRENT_COL < COLS) 
  {
    TAB_STOPS[CURRENT_COL] = true;
  }
}

void TERMINAL::control_characters(uint8_t Character) 
{
  switch (Character) 
  {
    //case 0x07: // Bell (BEL)
      // Logic for terminal beep/visual flash
    //  break;

    case 0x08: // Backspace (BS)
      control_BS();
      break;

    case 0x09: // Horizontal Tab (HT)
      control_HT();
      break;

    case 0x0A: // Line Feed (LF)
      control_LF();
      break;

    case 0x0D: // Carriage Return (CR)
      control_CR();
      break;

    case 0x0C: // Form Feed (FF) 
      screen_clear();
      break;

    case 0x0B: // Vertical Tab (VT)
      // VT is often treated exactly like LF (Line Feed)
      control_LF();
      break;

    case 0x0E: // Shift Out (SO)
      // Invoke G1 character set (Special Graphics)
      IS_GRAPHICS_MODE = true;
      break;

    case 0x0F: // Shift In (SI)
      // Invoke G0 character set (ASCII)
      IS_GRAPHICS_MODE = false;
      break;

    //case 0x00: // NUL
      // Not Coded
    //  break;

    default:
      // Log the hex code of the skipped control character for debugging
      std::cout << "[DEBUG] Skipped Control Character: 0x" 
                << std::hex << std::setw(2) << std::setfill('0') 
                << (int)Character << std::dec << std::endl;
      break;
  }
}


// Handler for ESC + Character (2-byte sequences)
void TERMINAL::handle_simple_escape(uint8_t command)
{
  switch (command) 
  {    
    case 'A': // Cursor Up (VT52 compatibility)
      cursor_up();
      break;
    case 'B': // Cursor Down (VT52 compatibility)
      cursor_down();
      break;
    case 'C': // Cursor Right (VT52 compatibility)
      cursor_right();
      break;
    case 'D': // 0x0A - IND - Index  or Cursor Left(VT52 compatibility)
      if (ANSI_MODE)
      {
        control_LF(); // Move down, scroll if at bottom
      }
      else
      {
        cursor_left();           // VT52 behavior
      }
      break;
    case 'F': // 0x46 - Enter Graphics Mode (VT52)
      IS_GRAPHICS_MODE = true;
      break;
    case 'G': // 0x47 - Exit Graphics Mode (VT52)
      IS_GRAPHICS_MODE = false;
      break;
    case 'I': // 0x49 - Reverse Line Feed (VT52 compatibility)
      control_RI();
      break;
    case 'J': // 0x4A - Erase to End of Screen (VT52 compatibility)
      screen_erase_to_end();
      break;
    case 'K': // 0x4B - Erase to End of Line
      screen_erase_line_to_end();
      break;
    case 'M': // 0x4D - RI - Reverse Index
      control_RI();
      break;
    case 'E': // 0x45 - NEL - Next Line
      control_CR(); 
      control_LF(); 
      break;
    case 'H': // 0x4B - HTS - Horizontal Tab Set
      control_HTS();
      break;    
    case 'Z': // 0x5A - Identify (VT52)
      // Standard VT52 response is "ESC / Z"
      // needs to respond with ID_RESPONSE
      break;
    case '<': // 0x3C - Enter ANSI Mode (VT52)
      ANSI_MODE = true;
      break;
    case 'c': // 0x63 -  RIS - Reset to Initial State
      reset();
      break;
    case '7': // 0x37 - DECSC - Save Cursor
        SAVED_ROW = CURRENT_ROW;
        SAVED_COL = CURRENT_COL;
        SAVED_AUTO_WRAP_MODE = AUTO_WRAP_MODE;
        SAVED_CURRENT_ATTRS = CURRENT_ATTRS;
        break;
    case '8': // 0x38 - DECRC - Restore Cursor
        CURRENT_ROW = SAVED_ROW;
        CURRENT_COL = SAVED_COL;
        AUTO_WRAP_MODE = SAVED_AUTO_WRAP_MODE;
        CURRENT_ATTRS = SAVED_CURRENT_ATTRS;
        WRAP_PENDING = false;
        break;
    case '=': // DECKPAM - Application Keypad
        APP_KEYPAD_MODE = true;
        break;
    case '>': // DECKPNM - Numeric Keypad
        APP_KEYPAD_MODE = false;
        break;
    default:
      // Log the hex code of the skipped control character for debugging
      std::cout << "[DEBUG] Skipped Control Character: 0x" 
        << std::hex << std::setw(2) << std::setfill('0') 
        << (int)command << std::dec << std::endl;
        break;
  }
}

void TERMINAL::handle_bracket_escape(std::string sequence) 
{
  char command = sequence.back();
  bool is_dec_private = (sequence.size() > 2 && sequence[2] == '?');
  std::vector<int> params = parse_csi_params(sequence);

  // --- DISPATCH LOGIC ---
  
  // 1. Cursor Movement & Positioning
  if (process_csi_cursor_movement(command, params)) return;

  // 2. Erasing, Deleting, and Editing
  if (process_csi_erase_and_edit(command, params)) return;

  // 3. Modes, Reporting, and State
  if (process_csi_reporting_and_mode(command, params, is_dec_private)) return;

  // 4. Graphics Rendition (Colors/Styles) and Misc
  if (process_csi_sgr_and_misc(command, params)) return;

  // Fallback for Debugging
  std::string printable_seq = "";
  for (unsigned char c : sequence) 
  {
    if (c == 0x1B) printable_seq += "ESC";
    else if (c < 32 || c > 126) 
    {
      std::stringstream ss;
      ss << "<0x" << std::hex << std::setw(2) << std::setfill('0') << (int)c << ">";
      printable_seq += ss.str();
    }
    else printable_seq += (char)c;
  }
  std::cout << "[DEBUG] Unhandled CSI: " << printable_seq << std::endl;
}

// Handler for ESC + Char + Params (e.g., VT52 Direct Cursor Addressing)
void TERMINAL::handle_parameterized_escape(std::string Raw_Text)
{
  uint8_t command = (uint8_t)Raw_Text[1];
  
  if (command == 'Y') // VT52 Direct Cursor Addressing
  {
    int row = (uint8_t)Raw_Text[2] - 31;
    int col = (uint8_t)Raw_Text[3] - 31;

    if (row >= 0 && row < ROWS) CURRENT_ROW = row;
    if (col >= 0 && col < COLS) CURRENT_COL = col;
  }
}


bool TERMINAL::escape_characters(std::string Raw_Text, int &End_Position) 
{
  if (Raw_Text.size() < 2) return false;
  uint8_t next_byte = (uint8_t)Raw_Text[1];

  if (next_byte == '[') 
  {
    for (size_t j = 2; j < Raw_Text.size(); j++) 
    {
      uint8_t c = (uint8_t)Raw_Text[j];
      if (c >= 0x40 && c <= 0x7E) 
      {
        handle_bracket_escape(Raw_Text.substr(0, j + 1));
        End_Position = (int)j;
        return true;
      }
    }
    return false;
  } 

  // Character Set Designations (ESC ( char)
  if (next_byte == '(' || next_byte == ')' || next_byte == '*' || next_byte == '+') 
  {
    if (Raw_Text.size() < 3) return false;
    
    // Logic to switch character sets
    if (next_byte == '(') 
    { // Designate G0
      uint8_t charset_code = (uint8_t)Raw_Text[2];
      if (charset_code == '0') current_g0_charset = CharSet::DEC_SPECIAL_GRAPHICS;
      else if (charset_code == 'B') current_g0_charset = CharSet::ASCII;
    }

    End_Position = 2;
    return true;
  } 

  if (next_byte == '#') 
  {
    if (Raw_Text.size() < 3) return false;
    End_Position = 2;
    return true;
  } 

  if (next_byte == 'Y') 
  {
    if (Raw_Text.size() < 4) return false;
    handle_parameterized_escape(Raw_Text.substr(0, 4));
    End_Position = 3;
    return true;
  }

  handle_simple_escape(next_byte);
  End_Position = 1;
  return true;
}

void TERMINAL::write_to_screen(char32_t Character)
{
  if (WRAP_PENDING && AUTO_WRAP_MODE)
  {
    CURRENT_COL = 0;
    control_LF(); // Move down or scroll
    WRAP_PENDING = false;
  }

  Cell new_cell = CURRENT_ATTRS;
  new_cell.character = Character;
  SCREEN[CURRENT_ROW][CURRENT_COL] = new_cell;

  // Advance logic
  if (CURRENT_COL < COLS - 1) 
  {
    CURRENT_COL++;
    WRAP_PENDING = false;
  } 
  else 
  {
    // We hit the last column. Don't move cursor yet.
    // Set flag so the NEXT character triggers the wrap.
    WRAP_PENDING = true;
  }
}

void TERMINAL::process_output_2(std::string& raw_text) 
{
  PROCESS_OUTPUT_2_BUFFER += raw_text;
  raw_text.clear();

  size_t i = 0;
  bool stop_processing = false;

  while (i < PROCESS_OUTPUT_2_BUFFER.size() && !stop_processing) 
  {
    uint8_t input_byte = (uint8_t)PROCESS_OUTPUT_2_BUFFER[i];

    if (input_byte == 0x1B) 
    {
      int end_pos = 0;
      if (escape_characters(PROCESS_OUTPUT_2_BUFFER.substr(i), end_pos)) 
      {
        i += end_pos + 1;
      } 
      else 
      {
        stop_processing = true; 
      }
    } 
    else if (input_byte < 0x20) 
    {
      control_characters(input_byte);
      i++;
    } 
    else 
    {
      bool incomplete = false;
      size_t temp_i = i;
      char32_t cp = decode_utf8_safe(PROCESS_OUTPUT_2_BUFFER, temp_i, incomplete);

      if (incomplete) 
      {
        stop_processing = true; 
      } 
      else 
      {
        i = temp_i;
        // Only translate if cp is within the ASCII range where DEC Graphics apply
        if (current_g0_charset == CharSet::DEC_SPECIAL_GRAPHICS && cp < 128) 
        {
          cp = translate_dec_graphics(cp);
        }
        write_to_screen(cp);
      }
    }
  }
  PROCESS_OUTPUT_2_BUFFER.erase(0, i);
}

// ---------------------------------------------------------------------

/**
 * @brief Constructor: Initializes the screen buffer, cursor state, and redirects stderr for logging.
 */
TERMINAL::TERMINAL()
{
  // Use the reset routine to establish the initial state
  reset();
}

/**
 * @brief Destructor: Cleans up the PTY and joins the reader thread.
 * * CRITICAL ORDERING: The child process and file descriptor must be closed/killed 
 * BEFORE joining the thread, otherwise the thread may block indefinitely on read().
 */
TERMINAL::~TERMINAL() 
{
  std::cout << "TERMINAL destructor called." << std::endl;

  // 1. Terminate the child process (the shell) first.
  if (PID > 0) 
  {
    std::cout << "Terminating child process (PID: " << PID << ")..." << std::endl;
    kill(PID, SIGKILL); // Ensure the shell is forcefully terminated
  }
  
  // 2. Close the master file descriptor.
  // This action should unblock the reader thread if it was blocked on read().
  // We check for > 0, as 0, 1, 2 are stdin, stdout, stderr.
  if (MASTER_FD > 0) 
  {
    std::cout << "Closing MASTER_FD (" << MASTER_FD << ")..." << std::endl;
    close(MASTER_FD);
    // Note: Resetting to -1 is good practice after closing
    MASTER_FD = -1; 
  }

  // 3. Join the thread.
  // Since the thread should now be unblocked, join() will complete quickly.
  if (T.joinable()) 
  {
    std::cout << "Joining terminal reader thread..." << std::endl;
    T.join();
  }
  
  std::cout << "TERMINAL object destruction complete." << std::endl;
}

/**
 * @brief Exposes the mutex for external synchronization.
 */
std::mutex& TERMINAL::get_mutex() 
{
	return BUF_MUTEX;
}

// ---------------------------------------------------------------------
// --- NEW: TEXT EXTRACTION METHOD ---
// ---------------------------------------------------------------------

void TERMINAL::get_line_text(int row, bool cursor_on, 
                              string &line, 
                              string &line_reverse, 
                              string &line_reverse_map)
{
  line = "";
  line_reverse = "";
  line_reverse_map = "";

  if (row < 0 || row >= ROWS) 
  {
    return; // Return empty string for out-of-bounds row
  }

  line.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size
  line_reverse.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size
  line_reverse_map.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size

  bool revers_reverse = false;

  // Iterate through all cells in the specified row
  for (int col = 0; col < COLS; ++col)
  {
    char32_t code_point = SCREEN[row][col].character;

    revers_reverse = SCREEN[row][col].is_reverse;
    if (CURRENT_COL == col && CURRENT_ROW == row && cursor_on)
    {
      revers_reverse = !revers_reverse;
    }

    if (revers_reverse || SCREEN[row][col].background_color != DEFAULT_BG_COLOR)
    {
      // Manually encode char32_t (Unicode code point) to UTF-8 bytes
      if (code_point <= 0x7F) 
      {
        // 1-byte sequence (ASCII)
        line_reverse += (char)code_point;
      } 
      else if (code_point <= 0x7FF) 
      {
        // 2-byte sequence
        line_reverse += (char)(0xC0 | (code_point >> 6));
        line_reverse += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0xFFFF) 
      {
        // 3-byte sequence (most common non-BMP characters)
        line_reverse += (char)(0xE0 | (code_point >> 12));
        line_reverse += (char)(0x80 | ((code_point >> 6) & 0x3F));
        line_reverse += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0x10FFFF) 
      {
        // 4-byte sequence (supplementary characters)
        line_reverse += (char)(0xF0 | (code_point >> 18));
        line_reverse += (char)(0x80 | ((code_point >> 12) & 0x3F));
        line_reverse += (char)(0x80 | ((code_point >> 6) & 0x3F));
        line_reverse += (char)(0x80 | (code_point & 0x3F));
      }
      // Invalid or unhandled code points are skipped

      line += ' ';
      line_reverse_map += reinterpret_cast<const char*>("\xE2\x96\x88");
    }
    else
    {
      // Manually encode char32_t (Unicode code point) to UTF-8 bytes
      if (code_point <= 0x7F) 
      {
        // 1-byte sequence (ASCII)
        line += (char)code_point;
      } 
      else if (code_point <= 0x7FF) 
      {
        // 2-byte sequence
        line += (char)(0xC0 | (code_point >> 6));
        line += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0xFFFF) 
      {
        // 3-byte sequence (most common non-BMP characters)
        line += (char)(0xE0 | (code_point >> 12));
        line += (char)(0x80 | ((code_point >> 6) & 0x3F));
        line += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0x10FFFF) 
      {
        // 4-byte sequence (supplementary characters)
        line += (char)(0xF0 | (code_point >> 18));
        line += (char)(0x80 | ((code_point >> 12) & 0x3F));
        line += (char)(0x80 | ((code_point >> 6) & 0x3F));
        line += (char)(0x80 | (code_point & 0x3F));
      }
      // Invalid or unhandled code points are skipped

      line_reverse += ' ';
      line_reverse_map += ' ';
    }
  }
}

/**
 * @brief Continuously reads output from the shell's master file descriptor.
 * * Buffers data until a complete block, free of unterminated ANSI escape sequences,
 * is received. This prevents display corruption from partial escape codes.
 */
void TERMINAL::reader_thread()
{
  std::string raw_text = "";
  AnsiStateTracker state_tracker; // Tracks if the current buffer ends mid-escape sequence

  char buf[4096];
  
  // Loop until the master file descriptor becomes invalid or an error occurs
  while (MASTER_FD > 0)
  {
    // We are assuming MASTER_FD is a blocking file descriptor here.
    ssize_t n = read(MASTER_FD, buf, sizeof(buf) - 1);
    
    if (n < 0) 
    {
      // Error reading (e.g., ECONNRESET, EIO). Check errno for details.
      // If the FD was closed by another thread, this will fire an error.
      if (errno != EINTR) 
      {
        //std::stringstream ss; 
        //ss << "TERMINAL reader_thread: Read error. Code: " << errno << std::endl;
        std::cerr << "TERMINAL reader_thread: Read error. Code: " << errno << std::endl;
        //MESSAGES.push_back(ss.str());
      }
      break; 
    }
    
    if (n == 0) 
    {
      // End-of-file (shell process exited and pipe/pty slave end closed)
      
      //std::stringstream ss; 
      //ss << "TERMINAL reader_thread: EOF detected (Shell exited)." << std::endl;
      std::cerr << "TERMINAL reader_thread: EOF detected (Shell exited)." << std::endl;
      //MESSAGES.push_back(ss.str());
      break; 
    }
    
    // Convert read bytes to a C++ string
    std::string raw_output(buf, n); 
    raw_text += raw_output;

    // Use the state tracker to determine if the *entire* accumulated buffer 
    // is currently in a NORMAL state.
    bool is_complete = state_tracker.parse_and_check_completion(raw_output);

    if (is_complete)
    {
      // The shell has sent a complete chunk of data. It is safe to process.
      std::lock_guard<std::mutex> lock(BUF_MUTEX);
      process_output_2(raw_text);
    }
  }
  
  // After the loop breaks, if there's any remaining text, process it as a final chunk.
  if (!raw_text.empty()) 
  {
    std::lock_guard<std::mutex> lock(BUF_MUTEX);
    process_output_2(raw_text);
  }

  //std::stringstream ss; 
  //ss << "TERMINAL reader_thread exiting." << std::endl;
  std::cerr << "TERMINAL reader_thread exiting." << std::endl;
  //MESSAGES.push_back(ss.str());

  // --- CRITICAL STEP: CLEANUP CHILD PROCESS (Zombie Prevention) ---
  // The shell process (PID) is guaranteed to have exited if we hit EOF (n=0).
  int status;
  // We use WNOHANG just in case, but after EOF, waitpid should generally return the status immediately.
  if (PID > 0 && waitpid(PID, &status, WNOHANG) > 0) \
  {
    //std::stringstream ss; 
    //ss << "TERMINAL reader_thread: Successfully collected exit status for PID " << PID << "." << std::endl;
    std::cerr << "TERMINAL reader_thread: Successfully collected exit status for PID " << PID << "." << std::endl;
    //MESSAGES.push_back(ss.str());
  } 
  else if (PID > 0) 
  {
    // If waitpid fails, it might mean the process was already waited for, or the 
    // process hasn't fully cleaned up yet (less likely after EOF).
    //std::stringstream ss; 
    //ss << "TERMINAL reader_thread: Warning: waitpid did not return status for PID " << PID << "." << std::endl;
    std::cerr << "TERMINAL reader_thread: Warning: waitpid did not return status for PID " << PID << "." << std::endl;
    //MESSAGES.push_back(ss.str());
  }

  // Set the flag for the main application to clean up this TERMINAL object.
  SHELL_EXITED = true;
}

void TERMINAL::create()
{
	start_shell();
	T = std::thread(&TERMINAL::reader_thread, this);
	T.detach();
}

/**
 * @brief Sends a command to the shell over the PTY master file descriptor.
 * @param charaters The command string to send.
 */
void TERMINAL::send_command(const std::string& charaters)
{
	if (MASTER_FD > 0) 
	{
		// Commands like Backspace (\x7F) or Delete (\x04) are sent here.
		write(MASTER_FD, charaters.c_str(), charaters.size());
	}
}
// ---------------------------------------------------------------------------------------

#endif