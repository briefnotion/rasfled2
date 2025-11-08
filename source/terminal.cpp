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
char32_t decode_next_utf8_char(const std::string& text, size_t& i) {
    if (i >= text.size()) {
        // Should not happen if called correctly, but safety first
        return U'\ufffd'; 
    }

    uint8_t byte1 = (uint8_t)text[i];
    char32_t code_point = 0;
    size_t consumed_bytes = 1;
//    size_t start_index = i; // Store original index for logging

    // --- 1-byte sequence: 0xxxxxxx (ASCII) ---
    if (byte1 <= 0x7F) {
        code_point = byte1;
    } 
    // --- 2-byte sequence: 110xxxxx 10xxxxxx ---
    else if ((byte1 & 0xE0) == 0xC0) {
        if (i + 1 >= text.size()) { 
            //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Incomplete 2-byte sequence): Start byte 0x" 
            //          << std::hex << (int)byte1 << std::dec << " at index " << start_index << std::endl;
            i++; return U'\ufffd'; 
        }
        uint8_t byte2 = (uint8_t)text[i + 1];
        if ((byte2 & 0xC0) != 0x80) { 
            //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Malformed 2-byte sequence): Expected continuation byte, got 0x" 
            //          << std::hex << (int)byte2 << std::dec << " at index " << start_index + 1 << std::endl;
            i++; return U'\ufffd'; 
        } 
        code_point = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
        consumed_bytes = 2;
    }
    // --- 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx ---
    else if ((byte1 & 0xF0) == 0xE0) {
        if (i + 2 >= text.size()) { 
            //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Incomplete 3-byte sequence): Start byte 0x" 
            //          << std::hex << (int)byte1 << std::dec << " at index " << start_index << std::endl;
            i++; return U'\ufffd'; 
        }
        uint8_t byte2 = (uint8_t)text[i + 1];
        uint8_t byte3 = (uint8_t)text[i + 2];
        
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80) { 
             //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Malformed 3-byte sequence): Invalid continuation bytes (0x" 
             //          << std::hex << (int)byte2 << ", 0x" << (int)byte3 << std::dec << ") starting at index " << start_index << std::endl;
            i++; return U'\ufffd'; 
        }
        code_point = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
        consumed_bytes = 3;
    }
    // --- 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx ---
    else if ((byte1 & 0xF8) == 0xF0) {
        if (i + 3 >= text.size()) { 
            //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Incomplete 4-byte sequence): Start byte 0x" 
            //          << std::hex << (int)byte1 << std::dec << " at index " << start_index << std::endl;
            i++; return U'\ufffd'; 
        }
        uint8_t byte2 = (uint8_t)text[i + 1];
        uint8_t byte3 = (uint8_t)text[i + 2];
        uint8_t byte4 = (uint8_t)text[i + 3];
        
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80 || (byte4 & 0xC0) != 0x80) { 
             //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Malformed 4-byte sequence): Invalid continuation bytes starting at index " << start_index << std::endl;
            i++; return U'\ufffd'; 
        }
        code_point = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
        consumed_bytes = 4;
    } 
    // --- Invalid starting byte or trailing continuation byte (10xxxxxx) ---
    else {
         //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Invalid start byte/Trailing continuation): Got 0x" 
            //          << std::hex << (int)byte1 << std::dec << " at index " << start_index << std::endl;
        i++; 
        return U'\ufffd';
    }

    // Check for overlong encodings or code points in the reserved range (D800-DFFF)
    if (code_point > 0x10FFFF || (code_point >= 0xD800 && code_point <= 0xDFFF)) {
        i += consumed_bytes; // Already consumes the full sequence
         // FIX: Cast char32_t to uint32_t for proper hex printing
         //std::cerr << "DEBUG: UTF-8 DECODE FAILED (Overlong/Reserved Code Point): 0x" 
            //          << std::hex << (uint32_t)code_point << std::dec << " starting at index " << start_index << std::endl;
        return U'\ufffd';
    }

    // Advance index and return successfully decoded code point
    i += consumed_bytes;
    return code_point;
}



// ---------------------------------------------------------------------------------------

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val) 
{
	return index < params.size() ? params[index] : default_val;
}

static const Cell DEFAULT_CELL = {}; 


// ---------------------------------------------------------------------------------------



/**
 * @brief Constructor: Initializes the screen buffer and cursor state.
 */
TERMINAL::TERMINAL() : 
    CURRENT_ATTRS(DEFAULT_CELL), // <-- FIX: Initialize non-static member here
    CURRENT_ROW(0), 
    CURRENT_COL(0),
    SCROLL_TOP(0), // Default top scrolling boundary (0-based)
    SCROLL_BOTTOM(ROWS - 1), // Default bottom scrolling boundary (0-based)
    SAVED_ROW(0),
    SAVED_COL(0),
    APP_CURSOR_MODE(false) // Default state for Application Cursor Keys mode
{
    // Initialize the entire screen buffer by filling with the DEFAULT_CELL object.
    for (int i = 0; i < ROWS; ++i) {
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






// ---------------------------------------------------------------------
// --- LINE MANIPULATION IMPLEMENTATIONS ---
// ---------------------------------------------------------------------

/**
 * @brief Scrolls lines up within the scrolling region [SCROLL_TOP, SCROLL_BOTTOM] by 'count' rows.
 * @param count The number of rows to scroll up.
 */
void TERMINAL::scroll_up(int count) 
{
    if (count <= 0 || SCROLL_TOP >= SCROLL_BOTTOM) return;

    // Determine how many lines actually need to shift
    int scroll_height = SCROLL_BOTTOM - SCROLL_TOP + 1;
    int actual_shift = std::min(count, scroll_height);

    // 1. Shift existing rows up
    // Move rows from SCROLL_TOP + actual_shift to SCROLL_BOTTOM
    for (int i = SCROLL_TOP; i <= SCROLL_BOTTOM - actual_shift; ++i) 
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
 * @brief Scrolls lines down within the scrolling region [SCROLL_TOP, SCROLL_BOTTOM] by 'count' rows (Reverse Scroll).
 * @param count The number of rows to scroll down.
 */
void TERMINAL::scroll_down(int count) 
{
    if (count <= 0 || SCROLL_TOP >= SCROLL_BOTTOM) return;

    int scroll_height = SCROLL_BOTTOM - SCROLL_TOP + 1;
    int actual_shift = std::min(count, scroll_height);

    // 1. Shift existing rows down
    // Iterate from bottom up: Row i moves to Row i + actual_shift
    for (int i = SCROLL_BOTTOM; i >= SCROLL_TOP + actual_shift; --i) 
    {
        // Row i - actual_shift moves to row i
        std::copy(SCREEN[i - actual_shift], SCREEN[i - actual_shift] + COLS, SCREEN[i]);
    }

    // 2. Clear the newly exposed lines at the top of the scrolling region
    for (int i = SCROLL_TOP; i < SCROLL_TOP + actual_shift; ++i) 
    {
        std::fill(SCREEN[i], SCREEN[i] + COLS, DEFAULT_CELL);
    }
}

/**
 * @brief Inserts 'count' blank lines starting at CURRENT_ROW, shifting existing lines down.
 * Insertion only happens within the scrolling region [SCROLL_TOP, SCROLL_BOTTOM].
 * @param count The number of lines to insert.
 */
void TERMINAL::insert_line(int count)
{
    if (count <= 0 || CURRENT_ROW < SCROLL_TOP || CURRENT_ROW > SCROLL_BOTTOM) return;

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


// ---------------------------------------------------------------------
// --- CSI HELPER FUNCTIONS (CURSOR, ERASE/EDIT, REPORT/MODE) ---
// ---------------------------------------------------------------------

/**
 * @brief Handles CSI sequences for cursor movement (A, B, C, D, H, f, G, d, E).
 */
bool TERMINAL::process_csi_cursor_movement(char final_char, const std::vector<int>& params)
{
    int param = get_param(params, 0, 1);
    
    if (final_char == 'A') // Cursor Up (CUU): ESC [ P A
    { 
        CURRENT_ROW = std::max(SCROLL_TOP, CURRENT_ROW - param);
    }
    else if (final_char == 'B') // Cursor Down (CUD): ESC [ P B
    { 
        CURRENT_ROW = std::min(SCROLL_BOTTOM, CURRENT_ROW + param);
    }
    else if (final_char == 'D') // Cursor Left (CUF): ESC [ P D
    { 
        CURRENT_COL = std::max(0, CURRENT_COL - param);
    }
    else if (final_char == 'C') // Cursor Right (CUB): ESC [ P C
    { 
        CURRENT_COL = std::min(COLS - 1, CURRENT_COL + param);
    }
    else if (final_char == 'H' || final_char == 'f') // Cursor Position (CUP: H) or HVP (f)
    {
        // Parameters are 1-based (row; col). Defaults to 1;1 if no params.
        int row = get_param(params, 0, 1);
        int col = get_param(params, 1, 1);

        // Clamp to 0-based indexing for internal use, relative to SCROLL_TOP
        CURRENT_ROW = std::min(SCROLL_BOTTOM, std::max(SCROLL_TOP, SCROLL_TOP + row - 1));
        CURRENT_COL = std::min(COLS - 1, std::max(0, col - 1));
    }
    else if (final_char == 'G') // Cursor Character Absolute (CHA): ESC [ P G
    {
        int col = get_param(params, 0, 1);
        CURRENT_COL = std::min(COLS - 1, std::max(0, col - 1));
    }
    else if (final_char == 'd') // Vertical Line Position Absolute (VPA): ESC [ P d
    {
        int row = get_param(params, 0, 1);
        CURRENT_ROW = std::min(SCROLL_BOTTOM, std::max(SCROLL_TOP, SCROLL_TOP + row - 1));
    }
    else 
    {
        return false; // Not a cursor movement sequence
    }
    return true; // Handled
}

/**
 * @brief Handles CSI sequences for screen/line editing (P, X, K, J, L, M, @).
 */
bool TERMINAL::process_csi_erase_and_edit(char final_char, const std::vector<int>& params)
{
    int param = get_param(params, 0, 1); // Default to 1 for edits/insertions

    if (final_char == 'P') // Delete Character (DCH): ESC [ P P
    {
        // Shift characters left from the deletion point
        if (CURRENT_COL < COLS) 
        {
            int count = param;
            int shift_start = CURRENT_COL;
            int shift_end = COLS - count;
            
            if (shift_end > shift_start) 
            {
                // Shift the block of characters
                std::copy(SCREEN[CURRENT_ROW] + shift_start + count, 
                          SCREEN[CURRENT_ROW] + COLS, 
                          SCREEN[CURRENT_ROW] + shift_start);
            }
            
            // Fill the freed space at the end with spaces
            std::fill(SCREEN[CURRENT_ROW] + std::max(shift_start, shift_end), 
                      SCREEN[CURRENT_ROW] + COLS, DEFAULT_CELL);
        }
    }
    else if (final_char == '@') // Insert Character (ICH): ESC [ P @
    {
        // Shifts characters right at CURRENT_ROW, starting at CURRENT_COL
        if (CURRENT_COL < COLS) 
        {
            int count = param;
            int shift_end = COLS - 1;
            int shift_start = CURRENT_COL;
            
            // 1. Shift existing rows right, stopping at the shift_start point
            // Iterate from right to left: Move character j - count to character j
            for (int j = shift_end; j >= shift_start + count; --j) 
            {
                if (j - count >= shift_start) 
                {
                    SCREEN[CURRENT_ROW][j] = SCREEN[CURRENT_ROW][j - count];
                }
            }
            
            // 2. Fill the newly inserted space with spaces
            int fill_end = std::min(COLS, shift_start + count);
            std::fill(SCREEN[CURRENT_ROW] + shift_start, 
                      SCREEN[CURRENT_ROW] + fill_end, DEFAULT_CELL);
        }
    }
    else if (final_char == 'X') // Erase Character (ECH): ESC [ P X
    {
        int count = param;
        int start_col = CURRENT_COL;
        int end_col = std::min(COLS, CURRENT_COL + count);
        std::fill(SCREEN[CURRENT_ROW] + start_col, SCREEN[CURRENT_ROW] + end_col, DEFAULT_CELL);
    }
    else if (final_char == 'K') // Erase in Line (EL): ESC [ P K
    {
        int param_k = get_param(params, 0, 0);
        if (param_k == 0) 
        { // Erase from cursor to end
            std::fill(SCREEN[CURRENT_ROW] + CURRENT_COL, SCREEN[CURRENT_ROW] + COLS, DEFAULT_CELL);
        } 
        else if (param_k == 1) 
        { // Erase from start to cursor
            std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + CURRENT_COL + 1, DEFAULT_CELL);
        } 
        else if (param_k == 2) 
        { // Erase entire line
            std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + COLS, DEFAULT_CELL);
        }
    }
    else if (final_char == 'J') // Erase Display (ED): ESC [ P J (Clear Screen)
    {
        int param_j = get_param(params, 0, 0);
        if (param_j == 2) 
        { // Clear entire screen and home cursor
            for (int row = 0; row < ROWS; ++row) 
            {
                std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL);
            }
            CURRENT_ROW = 0;
            CURRENT_COL = 0;
        } 
        else if (param_j == 0) 
        { // Erase from cursor to end of screen
            std::fill(SCREEN[CURRENT_ROW] + CURRENT_COL, SCREEN[CURRENT_ROW] + COLS, DEFAULT_CELL);
            for (int row = CURRENT_ROW + 1; row < ROWS; ++row) 
            {
                std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL);
            }
        } 
        else if (param_j == 1) 
        { // Erase from start of screen to cursor
            std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + CURRENT_COL + 1, DEFAULT_CELL);
            for (int row = 0; row < CURRENT_ROW; ++row) 
            {
                std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL);
            }
        }
    }
    else if (final_char == 'L') // Insert Line (IL): ESC [ P L
    { 
        this->insert_line(param); // Calls the new method
    }
    else if (final_char == 'M') // Delete Line (DL): ESC [ P M
    { 
        this->delete_line(param); // Calls the new method
    }
    else 
    {
        return false; // Not an erase/edit sequence
    }
    return true; // Handled
}

/**
 * @brief Handles CSI sequences for reporting, modes, and cursor save/restore (n, r, s, u, h, l, c, g).
 */
bool TERMINAL::process_csi_reporting_and_mode(char final_char, const std::vector<int>& params, bool is_dec_private)
{
    if (final_char == 'n') // Device Status Report (DSR): ESC [ 6 n
    {
        int param_n = get_param(params, 0, 0);
        if (param_n == 6) // Request Cursor Position
        {
            // Respond with Cursor Position Report (CPR): ESC [ R ; C R (1-based)
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
    }
    else if (final_char == 'c') // Device Attributes (DA): ESC [ P c
    {
        // Respond with Primary Device Attributes (PDA)
        // ESC [ ? 1 ; 2 c: VT100 with advanced video option, a common default.
        std::string response = "\x1B[?1;2c"; 
        
        if (MASTER_FD > 0) 
        {
            // Note: write() implementation is assumed to be available
            write(MASTER_FD, response.c_str(), response.size());
        }
    }
    else if (final_char == 'r') // Set Scrolling Region (DECSTBM): ESC [ Pt ; Pb r
    {
        int top = get_param(params, 0, 1);
        int bottom = get_param(params, 1, ROWS);
        
        SCROLL_TOP = std::min(ROWS - 1, std::max(0, top - 1));
        SCROLL_BOTTOM = std::min(ROWS - 1, std::max(0, bottom - 1));

        // Cursor moves to home position (first column of the new top row).
        CURRENT_ROW = SCROLL_TOP;
        CURRENT_COL = 0;
    }
    else if (final_char == 's') // Save Cursor Position (DECSC alias)
    {
        SAVED_ROW = CURRENT_ROW;
        SAVED_COL = CURRENT_COL;
    }
    else if (final_char == 'u') // Restore Cursor Position (DECRC alias)
    {
        CURRENT_ROW = SAVED_ROW;
        CURRENT_COL = SAVED_COL;
    }
    else if (final_char == 'h' || final_char == 'l') // SM/RM (Set/Reset Mode)
    {
        if (is_dec_private && params.size() >= 1) 
        {
            if (params[0] == 1) 
            {
                // DEC Private Mode 1: Application Cursor Keys (DECCKM)
                if (final_char == 'h') { APP_CURSOR_MODE = true; } 
                else { APP_CURSOR_MODE = false; }
            }
            // Log unhandled private modes
            else 
            {
                //std::cerr << "DEBUG CSI: Ignoring unhandled DEC Private Mode " 
                //          << params[0] << " (" << final_char << ")" << std::endl;
            }
        }
        // NO-OP for other modes for now: Consume the sequence.
        return true;
    }
    else if (final_char == 't') // Window Manipulation (DTTERM)
    {
        // NO-OP: Consume the sequence.
    }
    else if (final_char == 'g') // Tabulation Clear (TBC): ESC [ P g
    {
        int param_g = get_param(params, 0, 0);
        // TBC (Tabulation Clear) is used to clear tab stops. 
        // Since fixed 8-column tab stops are currently used, this is a NO-OP.
        (void)param_g; // Silence unused variable warning
    }
    else 
    {
        return false; // Not a reporting/mode sequence
    }
    return true; // Handled
}

/**_c
 * @brief Handles CSI sequences for Select Graphic Rendition (SGR: m) and other presentation commands.
 */
bool TERMINAL::process_csi_sgr_and_misc(char final_char, const std::vector<int>& params)
{
    if (final_char == 'm') // Select Graphic Rendition (SGR): ESC [ ... m
    {
        // If no parameters, default is [0m (Reset/Normal)
        if (params.empty() || (params.size() == 1 && params[0] == 0))
        {
            // Reset all attributes
            CURRENT_ATTRS = DEFAULT_CELL;
        }

        // Iterate through all parameters
        for (size_t i = 0; i < params.size(); ++i)
        {
            int code = params[i];

            if (code == 0) // Reset/Normal
            {
                CURRENT_ATTRS = DEFAULT_CELL;
            }
            else if (code == 1) // Bold/Bright
            {
                CURRENT_ATTRS.is_bold = true;
            }
            else if (code == 22) // Normal (neither bold nor faint)
            {
                CURRENT_ATTRS.is_bold = false;
            }
            else if (code == 7) // Reverse (Swap FG/BG)
            {
                // Swap the colors
                RgbColor temp_fg = CURRENT_ATTRS.foreground_color; 
                CURRENT_ATTRS.foreground_color = CURRENT_ATTRS.background_color;
                CURRENT_ATTRS.background_color = temp_fg;
            }
            else if (code == 27) // Not Reverse
            {
                // Reset to default terminal colors
                CURRENT_ATTRS.foreground_color = DEFAULT_FG_COLOR;
                CURRENT_ATTRS.background_color = DEFAULT_BG_COLOR;
            }
            // --- ANSI 3/4-bit Colors (30-37, 40-47, 90-97, 100-107) ---
            else if (code >= 30 && code <= 37) // Standard FG
            { 
                CURRENT_ATTRS.foreground_color = ANSI_COLORS[code - 30];
            }
            else if (code >= 90 && code <= 97) // Bright FG
            {
                CURRENT_ATTRS.foreground_color = BRIGHT_ANSI_COLORS[code - 90];
            }
            else if (code >= 40 && code <= 47) // Standard BG
            { 
                CURRENT_ATTRS.background_color = ANSI_COLORS[code - 40];
            }
            else if (code >= 100 && code <= 107) // Bright BG
            {
                 CURRENT_ATTRS.background_color = BRIGHT_ANSI_COLORS[code - 100];
            }
            // --- 24-bit True Color (38;2;R;G;B or 48;2;R;G;B) ---
            else if (code == 38 || code == 48)
            {
                // Check for 256 color (38;5;N or 48;5;N) - Skipping 256 color implementation for brevity
                if (i + 1 < params.size() && params[i+1] == 5)
                { 
                    i += 2; // Consume 5 and the color index N
                } 
                // Check for True color (38;2;R;G;B or 48;2;R;G;B)
                else if (i + 4 < params.size() && params[i+1] == 2) 
                {
                    uint8_t r = std::min(255, std::max(0, params[i+2]));
                    uint8_t g = std::min(255, std::max(0, params[i+3]));
                    uint8_t b = std::min(255, std::max(0, params[i+4]));
                    
                    RgbColor true_color = {r, g, b};
                    
                    if (code == 38) { // Foreground
                        CURRENT_ATTRS.foreground_color = true_color;
                    } else { // Background (code == 48)
                        CURRENT_ATTRS.background_color = true_color;
                    }
                    
                    i += 4; // Consume 2, R, G, B
                }
                // If it wasn't a recognized multi-parameter mode, consume the code and continue
            }
            else {
                // Ignore other unhandled codes (e.g., underline 4, blink 5)
            }
        }
        return true; 
    }
    
    return false; // Not handled
}


// ---------------------------------------------------------------------
// --- MAIN PROCESS OUTPUT FUNCTION (Using CURRENT_ATTRS member) ---
// ---------------------------------------------------------------------

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
                size_t csi_start_i = i; // Store the index of ESC
                i += 2; // Move index past ESC and [
                
                // 0. Check for optional intermediate characters (e.g., '?' for DEC Private Modes)
                bool is_dec_private = false;
                while (i < raw_text.size() && (raw_text[i] == '?' || raw_text[i] == '>')) 
                {
                    if (raw_text[i] == '?') 
                    {
                        is_dec_private = true;
                    }
                    i++; 
                }

                // 1. Extract optional numerical parameters (P1;P2;...)
                std::vector<int> params;
                std::string current_param_str;
                
                while (i < raw_text.size()) 
                {
                    if (raw_text[i] >= '0' && raw_text[i] <= '9') 
                    {
                        current_param_str += raw_text[i];
                    } 
                    else if (raw_text[i] == ';') 
                    {
                        // Handle empty parameter case (e.g., in "ESC[;H")
                        int p = current_param_str.empty() ? 0 : std::stoi(current_param_str);
                        params.push_back(p);
                        current_param_str.clear();
                    } 
                    else 
                    {
                        break; // Found the final character
                    }
                    i++;
                }
                
                // Push the last parameter (or default 0 if empty)
                int p_last = current_param_str.empty() ? 0 : std::stoi(current_param_str);
                params.push_back(p_last);

                // 2. Process final command character using helpers
                if (i < raw_text.size()) 
                {
                    char final_char = raw_text[i];
                    
                    // --- NEW DEBUG LOGGING: Show the full sequence and parameters ---
                    std::string seq_str = raw_text.substr(csi_start_i, i - csi_start_i + 1);
                    //std::cerr << "DEBUG CSI: Seq: '" << seq_str << "'"
                    //          << " | Params: [";
                    for (size_t p_idx = 0; p_idx < params.size(); ++p_idx) {
                        //std::cerr << params[p_idx] << (p_idx == params.size() - 1 ? "" : ";");
                    }
                    //std::cerr << "], Final: '" << final_char << "'" << std::endl;
                    // --- END NEW DEBUG LOGGING ---


                    // NOTE: Helper signatures do not need to change since they access CURRENT_ATTRS directly.
                    bool handled_csi = process_csi_cursor_movement(final_char, params) ||
                                         process_csi_erase_and_edit(final_char, params) ||
                                         process_csi_reporting_and_mode(final_char, params, is_dec_private) ||
                                         process_csi_sgr_and_misc(final_char, params); 

                    if (!handled_csi) 
                    {
                        // Use a clearer message if unhandled
                        //std::cerr << "DEBUG CSI: UNHANDLED SEQUENCE: " << seq_str << std::endl;
                    }

                    i++; // Consume the final character of the CSI sequence
                } else {
                    // Incomplete sequence (e.g. ESC[123 and end of string)
                    std::string seq_str = raw_text.substr(csi_start_i);
                    //std::cerr << "DEBUG CSI: INCOMPLETE SEQUENCE (EOD): " << seq_str << std::endl;
                }
                continue; // Move to the next byte index
            }
            
            // --- Generic Escape Sequence Handler ---
            else if (i + 1 < raw_text.size())
            {
                char next_char = raw_text[i + 1];
                
                // Handling 3-character G0/G1/G2/G3 designation sequences
                if (next_char == '(' || next_char == ')' || next_char == '*' || next_char == '+')
                {
                    if (i + 2 < raw_text.size()) 
                    {
                        i += 3; // ESC X C, consume ESC, X, and C
                    } 
                    else 
                    {
                        i += 2; // Incomplete sequence (e.g., ESC (), consume ESC and X.
                    }
                }
                else if (next_char == '#') // DEC Special Functions
                {
                    if (i + 2 < raw_text.size()) 
                    {
                        char third_char = raw_text[i + 2];
                        if (third_char == '8') 
                        { // DECALN: ESC # 8 (Screen Alignment Display)
                            // Fill entire screen with 'E' using current attributes
                            Cell fill_cell = CURRENT_ATTRS; // Use persistent attributes
                            fill_cell.character = U'E';

                            for (int row = 0; row < ROWS; ++row) 
                            {
                                // Use std::fill for efficiency and consistency
                                std::fill(SCREEN[row], SCREEN[row] + COLS, fill_cell);
                            }
                            i += 3; // Consume ESC, #, and '8'
                        } 
                        else 
                        {
                            //std::cerr << "DEBUG: UNHANDLED ESC # SEQUENCE (Third Char): '" << third_char 
                          //          << "' (0x" << std::hex << (int)(unsigned char)third_char << std::dec << ")" 
                          //          << std::endl;
                            i += 3; // Consume ESC, #, and the unhandled third char
                        }
                    } 
                    else 
                    {
                        // Error: Incomplete ESC # sequence
                        i += 2; // Consume ESC and #
                    }
                }
                else if (next_char == '7' || next_char == '8' || next_char == 'M' || next_char == 'D' || next_char == 'E' || next_char == 'J' || next_char == 'c' || next_char == '>' || next_char == '=' || next_char == 'H' || next_char == 'Y' || next_char == 'I' || next_char == '*') 
                { 
                    // Handle single-character sequences (like DECSC, DECRC, RI, IND, NEL, ED, etc.)
                    
                    // ESC 7/8 (Save/Restore)
                    if (next_char == '7') { SAVED_ROW = CURRENT_ROW; SAVED_COL = CURRENT_COL; }
                    else if (next_char == '8') { CURRENT_ROW = SAVED_ROW; CURRENT_COL = SAVED_COL; }
                    
                    // ESC M (Reverse Index)
                    else if (next_char == 'M') {
                        if (CURRENT_ROW == SCROLL_TOP) { this->scroll_down(1); } 
                        else { CURRENT_ROW = std::max(0, CURRENT_ROW - 1); }
                    }
                    
                    // ESC D (Index)
                    else if (next_char == 'D') {
                        if (CURRENT_ROW == SCROLL_BOTTOM) { this->scroll_up(1); } 
                        else { CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); }
                    }
                    
                    // ESC E (Next Line)
                    else if (next_char == 'E') {
                        CURRENT_COL = 0;
                        if (CURRENT_ROW == SCROLL_BOTTOM) { this->scroll_up(1); } 
                        else { CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); }
                    }
                    
                    // ESC J (Erase Display)
                    else if (next_char == 'J') {
                        // Clear entire screen (same as CSI 2 J) and home cursor
                        for (int row = 0; row < ROWS; ++row) {
                            // Use std::fill with the defined constant for clean screen initialization
                            std::fill(SCREEN[row], SCREEN[row] + COLS, DEFAULT_CELL);
                        }
                        CURRENT_ROW = 0;
                        CURRENT_COL = 0;
                    }
                    
                    // All other 2-character ESC sequences (c, >, =, H, Y, I, *) consume 2 bytes (ESC + char)
                    i += 2; 
                }
                else if (next_char == 'O' && (i + 2 < raw_text.size())) // SS3 (Single Shift 3): ESC O P, ESC O Q, etc. (3 chars total)
                {
                    i += 3; // Consume ESC, 'O', and the command character
                }
                else 
                {
                    // Sequence not handled, log the error directly
                    //std::cerr << "DEBUG: UNHANDLED GENERIC ESC SEQUENCE (Next Char): '" << next_char 
                  //          << "' (0x" << std::hex << (int)(unsigned char)next_char << std::dec << ")" 
                  //          << std::endl;
                    i += 2; // Consume ESC and the unhandled character
                }
                continue;
            }
            
            // If ESC wasn't followed by a recognized sequence, it's typically ignored
            i++; 
            continue;
        } 
        
        // --- CONTROL CHARACTER HANDLING ---
        else if (c == '\0') { i++; continue; } // NUL
        else if (c == '\r') { CURRENT_COL = 0; i++; continue; } // CR
        else if (c == '\b' || c == '\x08') { // BS
            if (CURRENT_COL > 0) { CURRENT_COL--; }
            i++; continue;
        }
        else if (c == '\t') { // HT
            int new_col = (CURRENT_COL / 8 + 1) * 8;
            CURRENT_COL = (new_col < COLS) ? new_col : COLS - 1; 
            i++; continue;
        }
        else if (c == '\n' || c == '\v' || c == '\f') { // LF, VT, FF (all treated as NEL/IND)
            CURRENT_COL = 0; 
            CURRENT_ROW++; 
            i++; 
        } 
        
        // --- UTF-8 DECODING AND PRINTABLE CHARACTER HANDLING ---
        else 
        {
            // At this point, the byte is either a printable ASCII character or the start of a multi-byte UTF-8 sequence.
            
            // 1. Decode the next Unicode character (code point)
            size_t start_i = i;
            char32_t code_point = decode_next_utf8_char(raw_text, i);
            
            // Check if decoding failed (i.e., returned replacement char, and index was only advanced by 1 byte)
            // The improved decode function ensures i is always advanced by at least 1 byte on error.
            if (code_point == U'\ufffd' && i == start_i + 1) {
                // The specific error is now logged inside decode_next_utf8_char.
                continue;
            }
            
            // If the code point is still a control character (which shouldn't happen if decoding is robust), skip.
            if (code_point < 0x20) {
                 // The decode function handles the index advancement, so we just continue
                 continue;
            }
            
            // 2. Handle Line Wrap
            if (CURRENT_COL >= COLS) 
            {
                CURRENT_COL = 0;
                CURRENT_ROW++;
            }
            
            // 3. Write character to SCREEN
            if (CURRENT_ROW < ROWS && CURRENT_COL < COLS) 
            {
                // UPDATED: Write the decoded char32_t to the Cell's character field
                SCREEN[CURRENT_ROW][CURRENT_COL].character = code_point;
                // Use the persistent CURRENT_ATTRS for styling
                SCREEN[CURRENT_ROW][CURRENT_COL].foreground_color = CURRENT_ATTRS.foreground_color;
                SCREEN[CURRENT_ROW][CURRENT_COL].background_color = CURRENT_ATTRS.background_color;
                SCREEN[CURRENT_ROW][CURRENT_COL].is_bold = CURRENT_ATTRS.is_bold;
            }
            CURRENT_COL++;
        }

        // --- Post-Character Scroll Check ---
        if (CURRENT_ROW > SCROLL_BOTTOM) 
        {
            this->scroll_up(CURRENT_ROW - SCROLL_BOTTOM);
            CURRENT_ROW = SCROLL_BOTTOM; 
        }
    }
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
    if (row < 0 || row >= ROWS) {
        return ""; // Return empty string for out-of-bounds row
    }

    std::string result;
    result.reserve(COLS); // Optimization: Reserve capacity based on maximum ASCII size

    // Iterate through all cells in the specified row
    for (int col = 0; col < COLS; ++col)
    {
        char32_t code_point = SCREEN[row][col].character;

        // Manually encode char32_t (Unicode code point) to UTF-8 bytes
        if (code_point <= 0x7F) {
            // 1-byte sequence (ASCII)
            result += (char)code_point;
        } else if (code_point <= 0x7FF) {
            // 2-byte sequence
            result += (char)(0xC0 | (code_point >> 6));
            result += (char)(0x80 | (code_point & 0x3F));
        } else if (code_point <= 0xFFFF) {
            // 3-byte sequence (most common non-BMP characters)
            result += (char)(0xE0 | (code_point >> 12));
            result += (char)(0x80 | ((code_point >> 6) & 0x3F));
            result += (char)(0x80 | (code_point & 0x3F));
        } else if (code_point <= 0x10FFFF) {
            // 4-byte sequence (supplementary characters)
            result += (char)(0xF0 | (code_point >> 18));
            result += (char)(0x80 | ((code_point >> 12) & 0x3F));
            result += (char)(0x80 | ((code_point >> 6) & 0x3F));
            result += (char)(0x80 | (code_point & 0x3F));
        }
        // Invalid or unhandled code points are skipped
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
		if (n <= 0) break; // Shell closed or error
		
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