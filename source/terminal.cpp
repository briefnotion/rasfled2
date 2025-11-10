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

// ---------------------------------------------------------------------
// --- UTF-8 DECODING IMPLEMENTATION (ROBUST, consumes 1 byte on early error) ---
// ---------------------------------------------------------------------

/**
 * @brief Decodes the next character from a UTF-8 string into a char32_t code point.
 * * In all error cases (incomplete or malformed), the index i is advanced by 1 byte
 * to ensure the calling loop terminates.
 * * @param text The input UTF-8 encoded string.
 * @param i Reference to the current byte index. It is advanced past the consumed bytes.
 * @return char32_t The decoded Unicode code point, or U'\ufffd' (Replacement Character) on error.
 */
char32_t decode_next_utf8_char(const std::string& text, size_t& i) 
{
  if (i >= text.size()) 
  {
    // Should not happen if called correctly, but safety first
    return U'\ufffd'; 
  }

  uint8_t byte1 = (uint8_t)text[i];
  char32_t code_point = 0;
  size_t consumed_bytes = 1;
  size_t start_index = i; // Store original index for logging

  // --- 1-byte sequence: 0xxxxxxx (ASCII) ---
  if (byte1 <= 0x7F) 
  {
    code_point = byte1;
  } 
  // --- 2-byte sequence: 110xxxxx 10xxxxxx ---
  else if ((byte1 & 0xE0) == 0xC0) 
  {
    if (i + 1 >= text.size()) 
    { 
        i++; return U'\ufffd'; 
    }
    uint8_t byte2 = (uint8_t)text[i + 1];
    if ((byte2 & 0xC0) != 0x80) 
    { 
        i++; return U'\ufffd'; 
    } 
    code_point = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
    consumed_bytes = 2;
  }
  // --- 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx ---
  else if ((byte1 & 0xF0) == 0xE0) 
  {
    if (i + 2 >= text.size()) 
    { 
        i++; return U'\ufffd'; 
    }
    uint8_t byte2 = (uint8_t)text[i + 1];
    uint8_t byte3 = (uint8_t)text[i + 2];
    
    if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80) 
    { 
        i++; return U'\ufffd'; 
    }
    code_point = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
    consumed_bytes = 3;
  }
  // --- 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx ---
  else if ((byte1 & 0xF8) == 0xF0) 
  {
    if (i + 3 >= text.size()) 
    { 
        i++; return U'\ufffd'; 
    }
    uint8_t byte2 = (uint8_t)text[i + 1];
    uint8_t byte3 = (uint8_t)text[i + 2];
    uint8_t byte4 = (uint8_t)text[i + 3];
    
    if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80 || (byte4 & 0xC0) != 0x80) 
    { 
        i++; return U'\ufffd'; 
    }
    code_point = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
    consumed_bytes = 4;
  } 
  // --- Invalid starting byte or trailing continuation byte (10xxxxxx) ---
  else 
  {
    i++; 
    return U'\ufffd';
  }

  // Check for overlong encodings or code points in the reserved range (D800-DFFF)
  if (code_point > 0x10FFFF || (code_point >= 0xD800 && code_point <= 0xDFFF)) 
  {
      
    // This is the fix: Advance index by the number of consumed bytes 
    // because we have already identified a full (but invalid) sequence.
    i = start_index + consumed_bytes; 
    return U'\ufffd'; // Return replacement character
  }

  // Advance index and return successfully decoded code point
  i += consumed_bytes;
  return code_point;
}

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val) 
{
	return index < params.size() ? params[index] : default_val;
}

// MANDATORY FIX: Define the constant default cell for use in std::fill
static const Cell DEFAULT_CELL = {}; 
// Global tracker for current cell attributes
Cell CURRENT_ATTRS = DEFAULT_CELL;

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
	PID = forkpty(&MASTER_FD, nullptr, nullptr, &ws); 

	if (PID == 0) 
	{
		// Child process: Execute the shell
		execl("/bin/bash", "bash", nullptr);
	} 
	else if (PID > 0) 
	{
		// Parent process: Immediately explicitly set the terminal size again 
		// using ioctl. This is a robust practice to ensure the PTY driver 
		// has the correct size before the shell starts executing and running 
		// programs like btop which rely on TIOCGWINSZ.
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

/**
 * @brief Inserts 'count' blank lines starting at CURRENT_ROW, shifting existing lines down.
 * Insertion only happens within the scrolling region [SCROLL_TOP, SCROLL_BOTTOM].
 * @param count The number of lines to insert.
 */
void TERMINAL::insert_line(int count)
{
  if (count <= 0 || CURRENT_ROW < SCROLL_TOP || CURRENT_ROW > SCROLL_BOTTOM) 
  {
    return;
  }

  int scroll_height = SCROLL_BOTTOM - SCROLL_TOP + 1;
  int actual_shift = std::min(count, scroll_height);
  int shift_start = CURRENT_ROW;

  // 1. Shift lines down, starting from the bottom of the scroll region
  // Stop shifting when we reach the insertion point (shift_start)
  for (int i = SCROLL_BOTTOM; i >= shift_start + actual_shift; --i) 
  {
    // Row i - actual_shift moves to row i
    std::copy(SCREEN[i - actual_shift], SCREEN[i - actual_shift] + COLS, SCREEN[i]);
  }
  
  // 2. Clear the newly inserted lines
  for (int i = shift_start; i < shift_start + actual_shift; ++i) 
  {
    std::fill(SCREEN[i], SCREEN[i] + COLS, DEFAULT_CELL);
  }
}

/**
 * @brief Deletes 'count' lines starting at CURRENT_ROW, shifting lines up to fill the gap.
 * Deletion only happens within the scrolling region [SCROLL_TOP, SCROLL_BOTTOM].
 * @param count The number of lines to delete.
 */
void TERMINAL::delete_line(int count)
{
  if (count <= 0 || CURRENT_ROW < SCROLL_TOP || CURRENT_ROW > SCROLL_BOTTOM) return;
  
  int scroll_height = SCROLL_BOTTOM - SCROLL_TOP + 1;
  // Calculate the maximum number of lines we can shift up from the current cursor position to the scroll bottom
  int actual_shift = std::min(count, scroll_height - (CURRENT_ROW - SCROLL_TOP));
  
  // 1. Shift lines up
  // Move rows from CURRENT_ROW + actual_shift up to CURRENT_ROW
  for (int i = CURRENT_ROW; i <= SCROLL_BOTTOM - actual_shift; ++i) 
  {
    // Row i + actual_shift moves to row i
    std::copy(SCREEN[i + actual_shift], SCREEN[i + actual_shift] + COLS, SCREEN[i]);
  }
  
  // 2. Clear the newly exposed lines at the bottom of the scrolling region
  for (int i = SCROLL_BOTTOM - actual_shift + 1; i <= SCROLL_BOTTOM; ++i) 
  {
    std::fill(SCREEN[i], SCREEN[i] + COLS, DEFAULT_CELL);
  }
}

/**
 * @brief Handles CSI sequences for cursor movement (A, B, C, D, H, f, G, d, E, F).
 * Note: 'E' (Next Line) and 'F' (Previous Line) are common cursor controls 
 * but were not in the original function, so they are not implemented here.
 * It's assumed CURRENT_ROW, SCROLL_TOP, COLS, etc., are member variables.
 */
bool TERMINAL::process_csi_cursor_movement(char final_char, const std::vector<int>& params)
{
  // Use the switch statement for cleaner dispatch based on the final character
  switch (final_char)
  {
    case 'A': // Cursor Up (CUU): ESC [ P A
    {
      int param = get_param(params, 0, 1);
      CURRENT_ROW = std::max(SCROLL_TOP, CURRENT_ROW - param);
      break;
    }

    case 'B': // Cursor Down (CUD): ESC [ P B
    {
      int param = get_param(params, 0, 1);
      CURRENT_ROW = std::min(SCROLL_BOTTOM, CURRENT_ROW + param);
      break;
    }

    case 'C': // Cursor Forward (CUF): ESC [ P C
    {
      int param = get_param(params, 0, 1);
      CURRENT_COL = std::min(COLS - 1, CURRENT_COL + param);
      break;
    }

    case 'D': // Cursor Back (CUB): ESC [ P D
    {
      int param = get_param(params, 0, 1);
      CURRENT_COL = std::max(0, CURRENT_COL - param);
      break;
    }

    case 'H': // Cursor Position (CUP: H) - Absolute Row/Col
    case 'f': // Horizontal and Vertical Position (HVP: f) - Absolute Row/Col
    case 'd': // Vertical Line Position Absolute (VPA: d) - Absolute Row
    {
      // Parameters are 1-based. Defaults to 1;1 for H/f or 1 for d.
      int requested_row_1_based = get_param(params, 0, 1);
      int requested_col_1_based = get_param(params, 1, 1); // Used only by H/f

      // VPA ('d') only moves the row, H/f move both.
      if (final_char == 'H' || final_char == 'f' || final_char == 'd') 
      {
        // Convert 1-based parameter to 0-based index relative to SCROLL_TOP
        int new_row_0_based = SCROLL_TOP + requested_row_1_based - 1;
        
        // Clamp the new row between SCROLL_TOP and SCROLL_BOTTOM
        CURRENT_ROW = std::min(SCROLL_BOTTOM, std::max(SCROLL_TOP, new_row_0_based));
      }
      
      // H/f also set the column
      if (final_char == 'H' || final_char == 'f') 
      {
        // Convert 1-based column to 0-based index
        int new_col_0_based = requested_col_1_based - 1;

        // Clamp the new column between 0 and COLS - 1
        CURRENT_COL = std::min(COLS - 1, std::max(0, new_col_0_based));
      }
      break;
    }

    case 'G': // Cursor Character Absolute (CHA): ESC [ P G - Absolute Column
    {
      // Parameter is 1-based column. Defaults to 1.
      int col_1_based = get_param(params, 0, 1);
      int new_col_0_based = col_1_based - 1;

      // Clamp the new column between 0 and COLS - 1
      CURRENT_COL = std::min(COLS - 1, std::max(0, new_col_0_based));
      break;
    }

    default:
    {
      return false; // Unhandled CSI final character
    }
  }
  
  return true; // Handled
}

/**
 * @brief Handles CSI sequences for screen/line editing (P, X, K, J, L, M, @).
 * Assumes SCREEN is a 2D array of CELL objects/structs and DEFAULT_CELL is a blank cell object.
 */
bool TERMINAL::process_csi_erase_and_edit(char final_char, const std::vector<int>& params)
{
  // Default count to 1 if the parameter is missing (P, X, @, L, M)
  int param = get_param(params, 0, 1); 
  
  // Helper lambda to clear a line range with DEFAULT_CELL
  auto clear_row_range = [&](int start_col, int end_col) 
  {
    std::fill(SCREEN[CURRENT_ROW] + start_col, 
              SCREEN[CURRENT_ROW] + end_col, 
              DEFAULT_CELL);
  };

  switch (final_char)
  {
    case 'P': // Delete Character (DCH): ESC [ P P
    {
      int count = param;
      if (CURRENT_COL < COLS) 
      {
        // Calculate the number of cells to shift. Don't shift past the end.
        int shift_len = std::max(0, COLS - (CURRENT_COL + count));
        
        // Shift the remaining characters left (overwriting the deleted ones)
        std::copy(SCREEN[CURRENT_ROW] + CURRENT_COL + count, 
                  SCREEN[CURRENT_ROW] + COLS, 
                  SCREEN[CURRENT_ROW] + CURRENT_COL);
        
        // Fill the freed space at the end with DEFAULT_CELL
        int fill_start_col = CURRENT_COL + shift_len;
        clear_row_range(fill_start_col, COLS);
      }
      break;
    }

    case '@': // Insert Character (ICH): ESC [ P @
    {
      int count = param;
      if (CURRENT_COL < COLS) 
      {
        // 1. Shift characters right to make space.
        // The shift stops at COLS - 1 (the last valid column).
        int shift_limit = std::min(COLS, CURRENT_COL + count);
        
        // Move characters from COLS - 1 down to shift_limit to their new location (right by `count`)
        std::copy_backward(SCREEN[CURRENT_ROW] + CURRENT_COL, // Start of source
                            SCREEN[CURRENT_ROW] + COLS - count, // End of source (one block before the end)
                            SCREEN[CURRENT_ROW] + COLS); // Destination end (one past the last element)

        // 2. Fill the newly inserted space with DEFAULT_CELL
        clear_row_range(CURRENT_COL, shift_limit);
      }
      break;
    }

    case 'X': // Erase Character (ECH): ESC [ P X
    {
      int count = param;
      int end_col = std::min(COLS, CURRENT_COL + count);
      clear_row_range(CURRENT_COL, end_col);
      break;
    }

    case 'K': // Erase in Line (EL): ESC [ P K
    {
      // Default param for K is 0, not 1, so we must re-evaluate.
      int param_k = get_param(params, 0, 0); 

      if (param_k == 0) 
      { 
        // Erase from cursor to end
        clear_row_range(CURRENT_COL, COLS);
      } 
      else if (param_k == 1) 
      { 
        // Erase from start to cursor (including cursor cell)
        clear_row_range(0, CURRENT_COL + 1);
      } 
      else if (param_k == 2) 
      { 
        // Erase entire line
        clear_row_range(0, COLS);
      }
      break;
    }

    case 'J': // Erase Display (ED): ESC [ P J (Clear Screen)
    {
      // Default param for J is 0, not 1, so we must re-evaluate.
      int param_j = get_param(params, 0, 0); 
      
      if (param_j == 2) 
      { 
        // Clear entire screen (within ROW boundaries) and home cursor
        for (int row = 0; row < ROWS; ++row) 
        {
          this->clear_row_range_full_line(row);
        }
        CURRENT_ROW = 0;
        CURRENT_COL = 0;
      } 
      else if (param_j == 0) 
      { 
        // Erase from cursor to end of screen (inclusive)
        clear_row_range(CURRENT_COL, COLS); // Clear remainder of current line
        for (int row = CURRENT_ROW + 1; row < ROWS; ++row) 
        {
          this->clear_row_range_full_line(row);
        }
      } 
      else if (param_j == 1) 
      { 
        // Erase from start of screen to cursor (inclusive)
        for (int row = 0; row < CURRENT_ROW; ++row) 
        {
          this->clear_row_range_full_line(row);
        }
        clear_row_range(0, CURRENT_COL + 1); // Clear up to cursor on current line
      }

      // NOTE: Full-screen clear (param 2) should respect SCROLL_TOP/SCROLL_BOTTOM limits
      // if the erase is supposed to only apply to the scroll region, but typically J2 means the whole screen.
      // I'll assume J2 means full screen (0 to ROWS-1) unless specified otherwise.
      break;
    }

    case 'L': // Insert Line (IL): ESC [ P L
    { 
      // Insert param lines at CURRENT_ROW, shifting lines down within the scroll region.
      int count = param;
      
      // Clamp count to prevent shifting past the scroll boundaries
      int effective_count = std::min(count, SCROLL_BOTTOM - CURRENT_ROW + 1);

      // 1. Move lines down: Iterate backward from the bottom of the scroll region.
      // Range to move: [CURRENT_ROW, SCROLL_BOTTOM - effective_count]
      // Destination: [CURRENT_ROW + effective_count, SCROLL_BOTTOM]
      for (int r = SCROLL_BOTTOM; r >= CURRENT_ROW + effective_count; --r) 
      {
        // Copy the row from 'effective_count' positions above
        std::copy(SCREEN[r - effective_count], SCREEN[r - effective_count] + COLS, SCREEN[r]);
      }
      
      // 2. Clear the inserted lines (which are now at [CURRENT_ROW, CURRENT_ROW + effective_count - 1])
      for (int r = CURRENT_ROW; r < CURRENT_ROW + effective_count; ++r) 
      {
        this->clear_row_range_full_line(r);
      }
      break;
    }

    case 'M': // Delete Line (DL): ESC [ P M
    { 
      // Delete param lines at CURRENT_ROW, shifting lines up within the scroll region.
      int count = param;
      
      // Clamp count to prevent shifting past the scroll boundaries
      int effective_count = std::min(count, SCROLL_BOTTOM - CURRENT_ROW + 1);

      // 1. Move lines up: Iterate forward from the deletion point (CURRENT_ROW).
      // Source: [CURRENT_ROW + effective_count, SCROLL_BOTTOM]
      // Destination: [CURRENT_ROW, SCROLL_BOTTOM - effective_count]
      for (int r = CURRENT_ROW; r <= SCROLL_BOTTOM - effective_count; ++r) 
      {
        // Copy the row from 'effective_count' positions below
        std::copy(SCREEN[r + effective_count], 
                  SCREEN[r + effective_count] + COLS, 
                  SCREEN[r]);
      }

      // 2. Clear the lines freed at the bottom of the scroll region.
      // Lines to clear: [SCROLL_BOTTOM - effective_count + 1, SCROLL_BOTTOM]
      for (int r = SCROLL_BOTTOM - effective_count + 1; r <= SCROLL_BOTTOM; ++r) 
      {
        this->clear_row_range_full_line(r);
      }
      
      break;
    }

    default:
    {
      return false; // Not an erase/edit sequence
    }
  }
  
  return true; // Handled
}

/**
 * @brief Handles CSI sequences for reporting, modes, and cursor save/restore (n, r, s, u, h, l, c, g, t).
 */
bool TERMINAL::process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private)
{
  switch (final_char)
  {
    case 'n': // Device Status Report (DSR): ESC [ 6 n
    {
      int param_n = get_param(params, 0, 0); // Default param for DSR is 0

      if (param_n == 6) // Request Cursor Position
      {
        // Respond with Cursor Position Report (CPR): ESC [ R ; C R (1-based, absolute)
        std::string response = "\x1B[" + 
                                std::to_string(CURRENT_ROW + 1) + 
                                ";" + 
                                std::to_string(CURRENT_COL + 1) + 
                                "R";
        
        if (MASTER_FD > 0) 
        {
          // Note: write() implementation is assumed to be available
          write(MASTER_FD, response.c_str(), response.size());
        }
      }
      break;
    }

    case 'c': // Device Attributes (DA): ESC [ P c
    {
      // Respond with Primary Device Attributes (PDA): ESC [ ? 1 ; 2 c (VT100 with advanced video option)
      std::string response = "\x1B[?1;2c"; 
      
      if (MASTER_FD > 0) 
      {
        // Note: write() implementation is assumed to be available
        write(MASTER_FD, response.c_str(), response.size());
      }
      break;
    }

    case 'r': // Set Scrolling Region (DECSTBM): ESC [ Pt ; Pb r
    {
      // Parameters are 1-based. Defaults to 1 (top) and ROWS (bottom).
      int top = get_param(params, 0, 1);
      int bottom = get_param(params, 1, ROWS);
      
      // Set SCROLL_TOP (0-based) and clamp to [0, ROWS-1]
      SCROLL_TOP = std::min(ROWS - 1, std::max(0, top - 1));

      // Set SCROLL_BOTTOM (0-based) and clamp, ensuring SCROLL_BOTTOM >= SCROLL_TOP
      SCROLL_BOTTOM = std::min(ROWS - 1, std::max(SCROLL_TOP, bottom - 1));

      // Cursor moves to home position (first column of the new top row).
      CURRENT_ROW = SCROLL_TOP;
      CURRENT_COL = 0;
      break;
    }

    case 's': // Save Cursor Position (DECSC alias)
    {
      SAVED_ROW = CURRENT_ROW;
      SAVED_COL = CURRENT_COL;
      break;
    }

    case 'u': // Restore Cursor Position (DECRC alias)
    {
      CURRENT_ROW = SAVED_ROW;
      CURRENT_COL = SAVED_COL;
      break;
    }

    case 'h': // Set Mode (SM)
    case 'l': // Reset Mode (RM)
    {
      if (is_dec_private && params.size() >= 1) 
      {
        int mode = params[0];
        bool set = (final_char == 'h');

        if (mode == 1) // DEC Private Mode 1: Application Cursor Keys (DECCKM)
        { 
          APP_CURSOR_MODE = set; 
        }
        else if (mode == 2) // DEC Private Mode 2: VT52 Application Keypad (DECKPAM)
        {
          APP_KEYPAD_MODE = set; 
        }
        else if (mode == 7) // DEC Private Mode 7: Auto Wrap Mode (DECAWM)
        {
          AUTO_WRAP_MODE = set;
        }
        else if (mode == 45) // DEC Private Mode 45: Reverse Wrap Around Mode (DECRLCM)
        {
          REVERSE_WRAP_MODE = set; 
        }
        // Modes 3, 4, 5, 6, 8, 40, 2004 are implicitly handled as sequence consumption (NO-OPs).
      }
      // If not a private mode, or if unhandled, the sequence is consumed.
      break;
    }

    case 't': // Window Manipulation (DTTERM)
    {
      // NO-OP: Consume the sequence.
      break;
    }

    case 'g': // Tabulation Clear (TBC): ESC [ P g
    {
      int param_g = get_param(params, 0, 0);
      (void)param_g; // Tabulation Clear is NO-OP since tab stops are fixed (Consume the sequence).
      break;
    }

    default:
    {
      return false; // Not a reporting/mode sequence
    }
  }

  return true; // Handled
}

/**
 * @brief Handles CSI sequences for Select Graphic Rendition (SGR: m) and other presentation commands.
 */
bool TERMINAL::process_csi_sgr_and_misc(char final_char, const std::vector<int>& params)
{
  if (final_char == 'm') // Select Graphic Rendition (SGR: m)
  {
    if (params.empty()) 
    {
      CURRENT_ATTRS = DEFAULT_CELL;
      return true; 
    }

    for (size_t i = 0; i < params.size(); ++i)
    {
      int code = params[i];

      if (code == 0) // Reset/Normal: ESC [ 0 m
      {
        // Reset ALL attributes, including the new flag
        CURRENT_ATTRS = DEFAULT_CELL;
      }
      else if (code == 1) // Bold/Bright: ESC [ 1 m
      {
        CURRENT_ATTRS.is_bold = true;
      }
      else if (code == 22) // Normal intensity (neither bold nor faint): ESC [ 22 m
      {
        CURRENT_ATTRS.is_bold = false;
      }
      else if (code == 7) // Reverse (Set Reverse Flag): ESC [ 7 m
      {
        // *** CRITICAL CHANGE: SET THE FLAG INSTEAD OF SWAPPING COLORS ***
        CURRENT_ATTRS.is_reverse = true;
      }
      else if (code == 27) // Not Reverse (Unset Reverse Flag): ESC [ 27 m
      {
        // *** CRITICAL CHANGE: CLEAR THE FLAG ***
        CURRENT_ATTRS.is_reverse = false;
      }
      // --- ANSI 3/4-bit Colors (30-37, 40-47, 90-97, 100-107) ---
      else if (code >= 30 && code <= 37) // Standard Foreground (30-37)
      { 
        CURRENT_ATTRS.foreground_color = ANSI_COLORS[code - 30];
        CURRENT_ATTRS.is_reverse = false; // Reset reverse state implicitly if color changes (some terminals do this)
      }
      else if (code >= 90 && code <= 97) // Bright Foreground (90-97)
      {
        CURRENT_ATTRS.foreground_color = BRIGHT_ANSI_COLORS[code - 90];
        CURRENT_ATTRS.is_reverse = false;
      }
      else if (code >= 40 && code <= 47) // Standard Background (40-47)
      { 
        CURRENT_ATTRS.background_color = ANSI_COLORS[code - 40];
        CURRENT_ATTRS.is_reverse = false;
      }
      else if (code >= 100 && code <= 107) // Bright Background (100-107)
      {
        CURRENT_ATTRS.background_color = BRIGHT_ANSI_COLORS[code - 100];
        CURRENT_ATTRS.is_reverse = false;
      }
      // --- Extended Color Modes (256-color or True Color) ---
      else if (code == 38 || code == 48)
      {
        // Extended color logic (as before, consumes up to 4 params)
        if (i + 1 < params.size())
        { 
          if (params[i+1] == 5) // 256 Color Mode: 38;5;N or 48;5;N
          { 
            i += 2; // Consume mode selector (5) and color index (N)
          } 
          else if (params[i+1] == 2) // 24-bit True Color Mode: 38;2;R;G;B or 48;2;R;G;B
          {
            if (i + 4 < params.size()) 
            {
              uint8_t r = (uint8_t)std::min(255, std::max(0, params[i+2]));
              uint8_t g = (uint8_t)std::min(255, std::max(0, params[i+3]));
              uint8_t b = (uint8_t)std::min(255, std::max(0, params[i+4]));
              
              RgbColor true_color = {r, g, b};
              
              if (code == 38) { CURRENT_ATTRS.foreground_color = true_color; } 
              else { CURRENT_ATTRS.background_color = true_color; }
              
              i += 4; // Consume mode selector (2), R, G, B
            } 
            else 
            { 
              i += 1; 
            }
          } 
          else 
          { 
            i += 1; 
          }
        }
      }
      // Reset reverse flag for any explicit color change (common terminal behavior)
      if (code >= 30 && code <= 107) 
      {
        CURRENT_ATTRS.is_reverse = false;
      }
    }
    return true; 
  }
  
  return false; // Not handled
}

// ---------------------------------------------------------------------
// --- NEW: CHARACTER MAPPING FOR GRAPHICS MODE ---
// ---------------------------------------------------------------------

/**
 * @brief Maps an ASCII character to a Unicode line drawing character 
 * based on the VT100/VT52 Special Graphics Character Set (G0/G1).
 * @param ascii_char The decoded char32_t code point (expected to be in ASCII range).
 * @return char32_t The mapped Unicode character or the original character.
 */
char32_t TERMINAL::map_graphics_char(char32_t ascii_char) const 
{
  if (!IS_GRAPHICS_MODE) return ascii_char;
  
  // This mapping covers the common line drawing characters from the ASCII range 
  // 0x60-0x7E (96-126) when the Special Graphics Character Set is active.
  switch (ascii_char) 
  {
    case U'`': return U'◆'; // Diamond
    case U'a': return U'▒'; // Medium Shade
    case U'b': return U'\u2409'; // HT Symbol (U+2409)
    case U'c': return U'\u240C'; // FF Symbol (U+240C)
    case U'd': return U'\u240D'; // CR Symbol (U+240D)
    case U'e': return U'\u240A'; // LF Symbol (U+240A)
    case U'f': return U'°'; // Degree Sign
    case U'g': return U'±'; // Plus-Minus Sign
    case U'h': return U'\u2408'; // BS Symbol (U+2408)
    case U'i': return U'\u240B'; // VT Symbol (U+240B)
    case U'j': return U'┘'; // Corner bottom right
    case U'k': return U'┐'; // Corner top right
    case U'l': return U'┌'; // Corner top left
    case U'm': return U'└'; // Corner bottom left
    case U'n': return U'┼'; // Crossing lines
    case U'o': return U'⎺'; // Top scan line (DECSL1)
    case U'p': return U'⎻'; // Bottom scan line (DECSL9)
    case U'q': return U'─'; // Horizontal line
    case U'r': return U'⎼'; // Mid scan line (DECSL3)
    case U's': return U'⎽'; // Mid scan line (DECSL7)
    case U't': return U'├'; // T-junction left
    case U'u': return U'┤'; // T-junction right
    case U'v': return U'┴'; // T-junction bottom
    case U'w': return U'┬'; // T-junction top
    case U'x': return U'│'; // Vertical line
    case U'y': return U'≤'; // Less-than or equal to
    case U'z': return U'≥'; // Greater-than or equal to
    case U'{': return U'π'; // Pi
    case U'|': return U'≠'; // Not equal to
    case U'}': return U'£'; // British pound sign
    case U'~': return U'·'; // Middle dot
    default: return ascii_char;
  }
}

// Note: This code assumes the existence of:
// - Cell, DEFAULT_CELL, CURRENT_ATTRS structures/variables.
// - ROWS, COLS, SCROLL_TOP, SCROLL_BOTTOM constants/variables.
// - CURRENT_ROW, CURRENT_COL, SAVED_ROW, SAVED_COL state variables.
// - APP_CURSOR_MODE, AUTO_WRAP_MODE, REVERSE_WRAP_MODE, APP_KEYPAD_MODE, IS_GRAPHICS_MODE boolean flags.
// - SCREEN (a 2D array of Cell).
// - MASTER_FD (file descriptor).
// - Helper functions: scroll_up, scroll_down, decode_next_utf8_char, map_graphics_char.
// - write() function for sending responses.

void TERMINAL::process_output(const std::string& raw_text) 
{
  for (size_t i = 0; i < raw_text.size();) 
  {
    char c = raw_text[i];
    
    // --- ANSI ESCAPE SEQUENCE PARSING (CSI: ESC [ ...) ---
    if (c == '\x1B') // ESC character
    {
      if (i + 1 < raw_text.size() && raw_text[i + 1] == '[') 
      {
        // This is a CSI sequence: ESC [
        const size_t csi_start_i = i;
        i += 2; // Consume ESC and [
        
        // --- 0. Check for optional DEC Private Mode Indicator ('?' or '>') ---
        bool is_dec_private = false;
        if (i + 1 < raw_text.size() && (raw_text[i] == '?' || raw_text[i] == '>')) 
        {
          if (raw_text[i] == '?') 
          {
            is_dec_private = true;
          }
          i++; // Consume indicator
        }

        // --- 1. Extract optional numerical parameters (P1;P2;...) ---
        std::vector<int> params;
        std::string current_param_str;

        while (i < raw_text.size()) 
        {
          char next_char = raw_text[i];
          
          if (next_char >= '0' && next_char <= '9') 
          {
            current_param_str += next_char;
          } 
          else if (next_char == ';') 
          {
            // Handle empty parameter (e.g., ESC[;1H -> first param is 0)
            int p = current_param_str.empty() ? 0 : std::stoi(current_param_str);
            params.push_back(p);
            current_param_str.clear();
          } 
          // Stop parameter processing on Intermediate (0x20-0x2F) or Final (0x40-0x7E)
          else if ((next_char >= 0x20 && next_char <= 0x2F) || (next_char >= 0x40 && next_char <= 0x7E))
          {
            break; 
          }
          else 
          {
            // Abort: Found an unexpected character. Reset pointer to ESC.
            i = csi_start_i; 
            goto continue_next_char; 
          }
          i++;
        }

        // Push the final collected parameter (or default 0)
        int p_last = current_param_str.empty() ? 0 : std::stoi(current_param_str);
        params.push_back(p_last);


        // --- 2. Check for optional intermediate bytes (0x20-0x2F) and consume them ---
        // The intermediate character is consumed here, but its value is not currently used by the processing functions.
        while (i < raw_text.size() && raw_text[i] >= 0x20 && raw_text[i] <= 0x2F)
        {
          i++; // Consume intermediate
        }

        // --- 3. Check for the final command character (0x40-0x7E) and process ---
        if (i < raw_text.size()) 
        {
          char final_char = raw_text[i];
          
          if (final_char >= 0x40 && final_char <= 0x7E)
          {
            // Process CSI Command (ACTIVATED HANDLERS)
            bool handled_csi = process_csi_cursor_movement(final_char, params)
                || process_csi_erase_and_edit(final_char, params) 
                || process_csi_reporting_and_mode(final_char, params, is_dec_private)
                || process_csi_sgr_and_misc(final_char, params);
            
            (void)handled_csi; // Prevent unused variable warning
            
            i++; // Consume the final command character
            goto continue_next_char;
          }
        } 
        // If the sequence was aborted or incomplete, we fall through.
        // i is left pointing at the character that broke the sequence (to be processed in the main loop).
        goto continue_next_char; // Sequence was handled or aborted, continue main loop iteration.
      }
      
      // --- Generic Escape Sequence Handler (Non-CSI) ---
      else if (i + 1 < raw_text.size())
      {
        char next_char = raw_text[i + 1];
        bool handled_esc = false;
        
        // 1. Handling 3-character G0/G1/G2/G3 designation sequences (ESC X C)
        if ((next_char == '(' || next_char == ')' || next_char == '*' || next_char == '+') && (i + 2 < raw_text.size()))
        {
          i += 3; // Consume ESC, X, and C (3 bytes total)
          handled_esc = true;
        }
        
        // 2. Handling 3-character DEC Special Functions (ESC # X)
        else if (next_char == '#') 
        {
          if (i + 2 < raw_text.size()) 
          {
            char third_char = raw_text[i + 2];
            if (third_char == '8') 
            { 
              // DECALN: ESC # 8 (Screen Alignment Display)
              Cell fill_cell = CURRENT_ATTRS; 
              fill_cell.character = U'E';

              for (int row = 0; row < ROWS; ++row) 
              {
                  std::fill(SCREEN[row], SCREEN[row] + COLS, fill_cell);
              }
            } 
            i += 3; // Consume ESC, #, and the third char
            handled_esc = true;
          } 
          else 
          {
            i += 2; // Incomplete sequence: Consume ESC and #
            handled_esc = true;
          }
        }
        
        // 3. Handling 2-character and 4-character sequences (ESC X or ESC Y R C)
        else if (next_char == 'Y' || next_char == '7' || next_char == '8' || next_char == 'M' || next_char == 'D' || next_char == 'E' || next_char == 'J' || next_char == 'c' || next_char == '>' || next_char == '=' || next_char == 'H' || next_char == 'Z' || next_char == 'I' || next_char == 'F' || next_char == 'G' || next_char == 'A' || next_char == 'B' || next_char == 'C' || next_char == 'K' || next_char == 'O')
        {
          // --- Handle 4-character VT52 Direct Cursor Addressing (ESC Y R C) ---
          if (next_char == 'Y') 
          {
            // ESC Y needs two more bytes for coordinates. Requires 4 total bytes.
            if (i + 3 < raw_text.size()) 
            {
              int row_byte = (unsigned char)raw_text[i + 2];
              int col_byte = (unsigned char)raw_text[i + 3];

              // Calculate 0-based coordinates (1-based from host, offset by 32)
              int target_row = row_byte - 32;
              int target_col = col_byte - 32;

              // Clamp and set 0-based coordinates
              CURRENT_ROW = std::min(SCROLL_BOTTOM, std::max(0, target_row));
              CURRENT_COL = std::min(COLS - 1, std::max(0, target_col));

              i += 4; // Consume ESC, Y, Row Byte, Col Byte
              handled_esc = true;
            } 
            else 
            {
              i += 2; // Incomplete: Consume ESC and Y only
              handled_esc = true;
            }
          }
          // --- END ESC Y HANDLER ---
          
          // --- Handle 2-character sequences ---
          else if (next_char == '7') 
          { 
            // ESC 7 (Save Cursor)
            SAVED_ROW = CURRENT_ROW; 
            SAVED_COL = CURRENT_COL; 
          }
          else if (next_char == '8') 
          { 
            // ESC 8 (Restore Cursor)
            CURRENT_ROW = SAVED_ROW; 
            CURRENT_COL = SAVED_COL; 
          }
          else if (next_char == 'M') 
          { 
            // ESC M (Reverse Index)
            if (CURRENT_ROW == SCROLL_TOP) 
            { 
              this->scroll_down(1); 
            } 
            else 
            { 
              CURRENT_ROW = std::max(0, CURRENT_ROW - 1); 
            }
          }
          else if (next_char == 'D') 
          { 
            // ESC D (Index)
            if (CURRENT_ROW == SCROLL_BOTTOM) 
            { 
              this->scroll_up(1); 
            } 
            else 
            { 
              CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); 
            }
          }
          else if (next_char == 'E') 
          { 
            // ESC E (Next Line)
            CURRENT_COL = 0;
            if (CURRENT_ROW == SCROLL_BOTTOM) 
            { 
              this->scroll_up(1); 
            } 
            else 
            { 
              CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); 
            }
          }
          else if (next_char == 'J') 
          { 
            // ESC J (Erase Display - VT52)
            CURRENT_ROW = 0;
            CURRENT_COL = 0;
            for (int r = 0; r < ROWS; ++r) 
            {
              std::fill(SCREEN[r], SCREEN[r] + COLS, DEFAULT_CELL);
            }
          }
          else if (next_char == 'I') 
          { 
            // ESC I (VT52 Reverse Index)
            if (CURRENT_ROW == SCROLL_TOP) 
            {
              scroll_down(1); 
            } 
            else 
            {
              CURRENT_ROW = std::max(SCROLL_TOP, CURRENT_ROW - 1);
            }
          }
          else if (next_char == 'H') 
          { 
            // ESC H (Set Tab Stop) - NO-OP
          }
          else if (next_char == 'Z') 
          { 
            // ESC Z (DECID - Identify Terminal)
            std::string response = "\x1B[?1;2c"; // VT100/VT102 ID
            if (MASTER_FD > 0) 
            { 
              /* write(MASTER_FD, response.c_str(), response.size()); */ 
            }
          }
          else if (next_char == 'c') 
          { 
            // ESC c (RIS - Reset to Initial State)
            // Full Terminal Reset Logic
            for (int row = 0; row < ROWS; ++row) 
            { 
              std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL); 
            }
            CURRENT_ROW = 0; CURRENT_COL = 0; SCROLL_TOP = 0; SCROLL_BOTTOM = ROWS - 1;
            CURRENT_ATTRS = DEFAULT_CELL; APP_CURSOR_MODE = false; AUTO_WRAP_MODE = true; 
            REVERSE_WRAP_MODE = false; APP_KEYPAD_MODE = false; IS_GRAPHICS_MODE = false; 
          }
          else if (next_char == 'F') 
          { 
            // ESC F (Select Graphics Character Set - VT52)
            IS_GRAPHICS_MODE = true; 
          }
          else if (next_char == 'G') 
          { 
            // ESC G (Select Standard Character Set - VT52)
            IS_GRAPHICS_MODE = false; 
          }
          // ESC A, B, C, K, O sequences are consumed but otherwise ignored/NO-OP for these handlers
          
          if (next_char != 'Y') 
          { 
            // If it wasn't the 4-byte ESC Y sequence, consume 2 bytes (ESC + char)
            i += 2; 
            handled_esc = true;
          }
        }
          
        if (handled_esc)
        {
          goto continue_next_char;
        }
        else 
        {
          // Unhandled 2-character sequence: consume ESC and the unhandled character to avoid infinite loops
          i += 2; 
          goto continue_next_char;
        }
      }
        
      // If ESC wasn't followed by a recognized sequence or if the string ended right after ESC
      i++; 
      goto continue_next_char;
    } 
    
    // --- CONTROL CHARACTER HANDLING ---
    else if (c == '\0') 
    { 
      i++; goto continue_next_char; 
    } // NUL (Ignore)
    else if (c == '\r') 
    { 
      CURRENT_COL = 0; i++; 
      goto continue_next_char; 
    } // CR
    else if (c == '\b' || c == '\x08') 
    { 
      // BS
        if (CURRENT_COL > 0) 
        { 
          CURRENT_COL--; 
        }
        i++; goto continue_next_char;
    }
    else if (c == '\t') 
    { 
      // HT (Horizontal Tab)
        // Move to next 8-column tab stop (or end of line)
        int new_col = (CURRENT_COL / 8 + 1) * 8;
        CURRENT_COL = (new_col < COLS) ? new_col : COLS - 1; 
        i++; goto continue_next_char;
    }
    else if (c == '\n' || c == '\v' || c == '\f') 
    { 
      // LF, VT, FF (Enforce CR+LF behavior)
        CURRENT_COL = 0; 
        CURRENT_ROW++; 
        i++; 
    } 
    
    // --- UTF-8 DECODING AND PRINTABLE CHARACTER HANDLING ---
    else 
    {
      // 1. Decode the next Unicode character (code point)
      size_t start_i = i;
      char32_t code_point = decode_next_utf8_char(raw_text, i);
      
      // Check if decoding failed or if it decoded to a control char (which should be handled above)
      if (code_point == U'\ufffd' || code_point < 0x20) 
      {
        // Ensure 'i' is advanced at least by 1 if decode_next_utf8_char failed to advance it
        if (i == start_i) i++; 
        goto continue_next_char;
      }

      // 2. APPLY GRAPHICS MODE MAPPING
      if (IS_GRAPHICS_MODE && code_point <= 0x7E && code_point >= 0x60) 
      {
        code_point = map_graphics_char(code_point);
      }
      
      // 3. Handle Line Wrap
      if (CURRENT_COL >= COLS) 
      {
        if (AUTO_WRAP_MODE) 
        {
          CURRENT_COL = 0;
          CURRENT_ROW++;
        } 
        else 
        {
          // If wrap mode is off, cursor stays at the last column
          CURRENT_COL = COLS - 1;
        }
      }
      
      // 4. Write character to SCREEN
      if (CURRENT_ROW < ROWS && CURRENT_COL < COLS) 
      {
        // Write the decoded char32_t to the Cell's character field
        SCREEN[CURRENT_ROW][CURRENT_COL].character = code_point;
        
        // Copy current attributes
        SCREEN[CURRENT_ROW][CURRENT_COL].foreground_color = CURRENT_ATTRS.foreground_color;
        SCREEN[CURRENT_ROW][CURRENT_COL].background_color = CURRENT_ATTRS.background_color;
        SCREEN[CURRENT_ROW][CURRENT_COL].is_bold = CURRENT_ATTRS.is_bold;
        SCREEN[CURRENT_ROW][CURRENT_COL].is_reverse = CURRENT_ATTRS.is_reverse;
      }
      
      // Advance cursor
      if (CURRENT_COL < COLS - 1 || AUTO_WRAP_MODE) 
      {
        CURRENT_COL++;
      }
    }

    // --- Post-Character Scroll Check ---
    if (CURRENT_ROW > SCROLL_BOTTOM) 
    {
      // Scroll up enough to bring the cursor back to the scroll bottom
      this->scroll_up(CURRENT_ROW - SCROLL_BOTTOM);
      CURRENT_ROW = SCROLL_BOTTOM; 
    }
    
    continue_next_char:;
  }
}

/**
 * @brief Constructor: Initializes the screen buffer, cursor state, and redirects stderr for logging.
 */
TERMINAL::TERMINAL() : 
  CURRENT_ROW(0), 
  CURRENT_COL(0),
  SCROLL_TOP(0), // Default top scrolling boundary (0-based)
  SCROLL_BOTTOM(ROWS - 1), // Default bottom scrolling boundary (0-based)
  SAVED_ROW(0),
  SAVED_COL(0),
  APP_CURSOR_MODE(false), // Default state for Application Cursor Keys mode
  // NEW: Text Wrapping is usually ON by default (DEC private mode 7)
  AUTO_WRAP_MODE(true),
  // NEW: Reverse Wrap Mode (DEC Private Mode 45) is OFF by default
  REVERSE_WRAP_MODE(false),
  // NEW: Application Keypad Mode (DEC Private Mode 2) is OFF by default
  APP_KEYPAD_MODE(false),
  // NEW: Graphics Mode (VT52 ESC F/G) is OFF by default
  IS_GRAPHICS_MODE(false) // <--- ADDED Graphics Mode State
{
  // Initialize the entire screen buffer by filling with the DEFAULT_CELL object.
  for (int i = 0; i < ROWS; ++i) 
  {
      std::fill(SCREEN[i], SCREEN[i] + COLS, DEFAULT_CELL);
  }
}

/**
 * @brief Destructor: Cleans up the PTY and joins the reader thread.
 */
TERMINAL::~TERMINAL() 
{
  if (MASTER_FD > 0) 
	{
		close(MASTER_FD);
  }
  if (T.joinable()) 
	{
		T.join();
  }
  if (PID > 0) 
	{
		kill(PID, SIGKILL); // Ensure the child process is terminated
  }
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

/**
 * @brief Extracts the raw character text from a screen row and encodes it to UTF-8.
 * * @param row The 0-based row index to extract.
 * @return std::string UTF-8 encoded text content of the line.
 */
std::string TERMINAL::get_line_text(int row) const
{
  if (row < 0 || row >= ROWS) 
  {
    return ""; // Return empty string for out-of-bounds row
  }

  std::string result;
  result.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size

  // Iterate through all cells in the specified row
  for (int col = 0; col < COLS; ++col)
  {
    char32_t code_point = SCREEN[row][col].character;

    if (SCREEN[row][col].is_reverse || SCREEN[row][col].background_color != DEFAULT_BG_COLOR)
    {
      result += ' ';
    }
    else
    {
      // Manually encode char32_t (Unicode code point) to UTF-8 bytes
      if (code_point <= 0x7F) 
      {
          // 1-byte sequence (ASCII)
          result += (char)code_point;
      } 
      else if (code_point <= 0x7FF) 
      {
          // 2-byte sequence
          result += (char)(0xC0 | (code_point >> 6));
          result += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0xFFFF) 
      {
          // 3-byte sequence (most common non-BMP characters)
          result += (char)(0xE0 | (code_point >> 12));
          result += (char)(0x80 | ((code_point >> 6) & 0x3F));
          result += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0x10FFFF) 
      {
          // 4-byte sequence (supplementary characters)
          result += (char)(0xF0 | (code_point >> 18));
          result += (char)(0x80 | ((code_point >> 12) & 0x3F));
          result += (char)(0x80 | ((code_point >> 6) & 0x3F));
          result += (char)(0x80 | (code_point & 0x3F));
      }
      // Invalid or unhandled code points are skipped
    }
  }
  
  return result;
}

std::string TERMINAL::get_line_text_reverse_map(int row) const
{
  if (row < 0 || row >= ROWS) 
  {
    return ""; // Return empty string for out-of-bounds row
  }

  std::string result;
  result.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size

  // Iterate through all cells in the specified row
  for (int col = 0; col < COLS; ++col)
  {
    if (SCREEN[row][col].is_reverse || SCREEN[row][col].background_color != DEFAULT_BG_COLOR)
    {
      result += reinterpret_cast<const char*>("\xE2\x96\x88");
    }
    else
    {
      result += ' ';
    }
  }
  
  return result;
}

std::string TERMINAL::get_line_text_reverse(int row) const
{
  if (row < 0 || row >= ROWS) 
  {
    return ""; // Return empty string for out-of-bounds row
  }

  std::string result;
  result.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size

  // Iterate through all cells in the specified row
  for (int col = 0; col < COLS; ++col)
  {
    char32_t code_point = SCREEN[row][col].character;

    if (!(SCREEN[row][col].is_reverse || SCREEN[row][col].background_color != DEFAULT_BG_COLOR))
    {
      result += ' ';
    }
    else
    {
      // Manually encode char32_t (Unicode code point) to UTF-8 bytes
      if (code_point <= 0x7F) 
      {
        // 1-byte sequence (ASCII)
        result += (char)code_point;
      } 
      else if (code_point <= 0x7FF) 
      {
        // 2-byte sequence
        result += (char)(0xC0 | (code_point >> 6));
        result += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0xFFFF) 
      {
        // 3-byte sequence (most common non-BMP characters)
        result += (char)(0xE0 | (code_point >> 12));
        result += (char)(0x80 | ((code_point >> 6) & 0x3F));
        result += (char)(0x80 | (code_point & 0x3F));
      } 
      else if (code_point <= 0x10FFFF) 
      {
        // 4-byte sequence (supplementary characters)
        result += (char)(0xF0 | (code_point >> 18));
        result += (char)(0x80 | ((code_point >> 12) & 0x3F));
        result += (char)(0x80 | ((code_point >> 6) & 0x3F));
        result += (char)(0x80 | (code_point & 0x3F));
      }
      // Invalid or unhandled code points are skipped
    }
  }
  
  return result;
}

// Read output from shell continuously
void TERMINAL::reader_thread()
{
	char buf[512];
	while (MASTER_FD > 0) // Check if the master FD is valid
	{
		ssize_t n = read(MASTER_FD, buf, sizeof(buf) - 1);
		if (n <= 0) 
    {
      break; // Shell closed or error
    }
		
		// 1. Pass RAW output to process_output, as it now handles escape sequences.
		std::string raw_output(buf, n); // Use 'n' to ensure we only capture valid bytes

		// 2. Lock the shared resource and process the output
		std::lock_guard<std::mutex> lock(BUF_MUTEX);
		process_output(raw_output);
	}
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