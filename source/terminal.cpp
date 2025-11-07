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

// Helper to safely get parameters (default to 0 if not present)
int get_param(const std::vector<int>& params, size_t index, int default_val) 
{
	return index < params.size() ? params[index] : default_val;
}

// ---------------------------------------------------------------------------------------

/**
 * @brief Constructor: Initializes the screen buffer and cursor state.
 */
TERMINAL::TERMINAL() : 
    CURRENT_ROW(0), 
    CURRENT_COL(0),
    SCROLL_TOP(0), // New: Default top scrolling boundary (0-based)
    SCROLL_BOTTOM(ROWS - 1), // New: Default bottom scrolling boundary (0-based)
    // Initialize saved cursor state
    SAVED_ROW(0),
    SAVED_COL(0)
{
	// Initialize the entire screen buffer to spaces
	for (int i = 0; i < ROWS; ++i) {
		std::fill(SCREEN[i], SCREEN[i] + COLS, ' ');
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
		std::fill(SCREEN[i], SCREEN[i] + COLS, ' ');
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
		std::fill(SCREEN[i], SCREEN[i] + COLS, ' ');
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
		std::fill(SCREEN[i], SCREEN[i] + COLS, ' ');
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
		std::fill(SCREEN[i], SCREEN[i] + COLS, ' ');
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
								SCREEN[CURRENT_ROW] + COLS, ' ');
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
								SCREEN[CURRENT_ROW] + fill_end, ' ');
		}
	}
	else if (final_char == 'X') // Erase Character (ECH): ESC [ P X
	{
		int count = param;
		int start_col = CURRENT_COL;
		int end_col = std::min(COLS, CURRENT_COL + count);
		std::fill(SCREEN[CURRENT_ROW] + start_col, SCREEN[CURRENT_ROW] + end_col, ' ');
	}
	else if (final_char == 'K') // Erase in Line (EL): ESC [ P K
	{
		int param_k = get_param(params, 0, 0);
		if (param_k == 0) 
		{ // Erase from cursor to end
			std::fill(SCREEN[CURRENT_ROW] + CURRENT_COL, SCREEN[CURRENT_ROW] + COLS, ' ');
		} 
		else if (param_k == 1) 
		{ // Erase from start to cursor
			std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + CURRENT_COL + 1, ' ');
		} 
		else if (param_k == 2) 
		{ // Erase entire line
			std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + COLS, ' ');
		}
	}
	else if (final_char == 'J') // Erase Display (ED): ESC [ P J (Clear Screen)
	{
		int param_j = get_param(params, 0, 0);
		if (param_j == 2) 
		{ // Clear entire screen and home cursor
			for (int row = 0; row < ROWS; ++row) 
			{
				std::fill(SCREEN[row], SCREEN[row] + COLS, ' ');
			}
			CURRENT_ROW = 0;
			CURRENT_COL = 0;
		} 
		else if (param_j == 0) 
		{ // Erase from cursor to end of screen
			std::fill(SCREEN[CURRENT_ROW] + CURRENT_COL, SCREEN[CURRENT_ROW] + COLS, ' ');
			for (int row = CURRENT_ROW + 1; row < ROWS; ++row) 
			{
				std::fill(SCREEN[row], SCREEN[row] + COLS, ' ');
			}
		} 
		else if (param_j == 1) 
		{ // Erase from start of screen to cursor
			std::fill(SCREEN[CURRENT_ROW], SCREEN[CURRENT_ROW] + CURRENT_COL + 1, ' ');
			for (int row = 0; row < CURRENT_ROW; ++row) 
			{
				std::fill(SCREEN[row], SCREEN[row] + COLS, ' ');
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
				if (final_char == 'h') 
				{ // Set Mode
					APP_CURSOR_MODE = true; 
					std::cerr << "DEBUG: SET APP CURSOR MODE" << std::endl;
				} 
				else 
				{ // Reset Mode
					APP_CURSOR_MODE = false; 
					std::cerr << "DEBUG: RESET APP CURSOR MODE" << std::endl;
				}
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

/**
 * @brief Handles CSI sequences for Select Graphic Rendition (SGR: m) and other presentation commands.
 */
bool TERMINAL::process_csi_sgr_and_misc(char final_char, const std::vector<int>& params)
{
	(void)params; // Silence "unused parameter" warning until SGR processing is implemented.

	if (final_char == 'm') // Select Graphic Rendition (SGR): ESC [ ... m
	{
		// NO-OP: SGR sequences are critical for text appearance (color, bold, etc.) 
		// but for now, we just consume the sequence to prevent errors.
		return true; 
	}
	
	return false; // Not handled
}


// ---------------------------------------------------------------------
// --- MAIN PROCESS OUTPUT FUNCTION (Refactored) ---
// ---------------------------------------------------------------------

void TERMINAL::process_output(const std::string& raw_text) 
{
	for (size_t i = 0; i < raw_text.size(); ++i) 
	{
		char c = raw_text[i];
		
		// --- ANSI ESCAPE SEQUENCE PARSING (CSI: ESC [ ...) ---
		if (c == '\x1B') // ESC character
		{
			// Check for CSI (Control Sequence Introducer): ESC [
			if (i + 1 < raw_text.size() && raw_text[i + 1] == '[') 
			{
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
					
					// We need to pass is_dec_private to the mode handler
					bool handled_csi = process_csi_cursor_movement(final_char, params) ||
															process_csi_erase_and_edit(final_char, params) ||
															process_csi_reporting_and_mode(final_char, params, is_dec_private) ||
															process_csi_sgr_and_misc(final_char, params); 

					if (!handled_csi) 
					{
						// FIX: If the final character is a C0 control code (like \b=0x08, VT=0x0B, etc.), 
						// it's a malformed sequence, so we silently ignore it.
						if (final_char < 0x20) 
						{
							// Consumed the malformed sequence without error.
						} 
						else 
						{
							std::cerr << "DEBUG: UNHANDLED CSI SEQUENCE (Final Char): '" << final_char 
											<< "' (0x" << std::hex << (int)(unsigned char)final_char << std::dec << ")" 
											<< std::endl;
						}
					}
					continue; // Skip consumption of the final char, as it's part of the sequence, and move to next char
				}
			}
			// --- Generic Escape Sequence Handler (e.g., ESC c, ESC 7, ESC 8, ESC O, ESC M, ESC H, ESC J, ESC Y, ESC I) ---
			else if (i + 1 < raw_text.size())
			{
				char next_char = raw_text[i + 1];
				
				// FIX: Handle 3-character G0/G1/G2/G3 designation sequences (ESC ( C, ESC * C, etc.)
				// This block now handles incomplete sequences gracefully (e.g., just ESC () without the third character)
				if (next_char == '(' || next_char == ')' || next_char == '*' || next_char == '+')
				{
					if (i + 2 < raw_text.size()) 
					{
						i += 2; // ESC X C, consume the designation char and the set identifier
					} 
					else 
					{
						i += 1; // Incomplete sequence (e.g., ESC (), consume only the second character.
					}
				}
				else if (next_char == '#') // DEC Special Functions
				{
					if (i + 2 < raw_text.size()) 
					{
						char third_char = raw_text[i + 2];
						if (third_char == '8') 
						{ // DECALN: ESC # 8 (Screen Alignment Display)
							// Fill entire screen with 'E'
							for (int row = 0; row < ROWS; ++row) 
							{
									std::fill(SCREEN[row], SCREEN[row] + COLS, 'E');
							}
							i += 2; // Consume '#' and '8'
						} 
						else 
						{
							// Unhandled 3-character ESC # sequence
							std::cerr << "DEBUG: UNHANDLED ESC # SEQUENCE (Third Char): '" << third_char 
											<< "' (0x" << std::hex << (int)(unsigned char)third_char << std::dec << ")" 
											<< std::endl;
							i += 2; // Consume '#' and the unhandled third char
						}
					} 
					else 
					{
						// Error: Incomplete ESC # sequence (just consume #)
						i++; 
					}
				}
				else if (next_char == '7') // DEC Save Cursor (DECSC)
				{ 
					SAVED_ROW = CURRENT_ROW; SAVED_COL = CURRENT_COL; i++; 
				}
				else if (next_char == '8') // DEC Restore Cursor (DECRC)
				{ 
					CURRENT_ROW = SAVED_ROW; CURRENT_COL = SAVED_COL; i++; 
				}
				else if (next_char == 'M') // Reverse Index (RI): ESC M
				{
					if (CURRENT_ROW == SCROLL_TOP) 
					{
						this->scroll_down(1); // Perform reverse scroll (shift down)
					} 
					else 
					{
						CURRENT_ROW = std::max(0, CURRENT_ROW - 1); // Move up one row
					}
					i++; 
				}
				else if (next_char == 'D') // Index (IND): ESC D
				{
						if (CURRENT_ROW == SCROLL_BOTTOM) 
						{
							this->scroll_up(1); // Scroll the region up
						} 
						else 
						{
							// Move down one line, ensuring we don't move past the physical screen end.
							CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); 
						}
						i++;
				}
				else if (next_char == 'E') // Next Line (NEL): ESC E
				{
					CURRENT_COL = 0; // Reset column
					if (CURRENT_ROW == SCROLL_BOTTOM) 
					{
						this->scroll_up(1); // Scroll the region up
					} 
					else 
					{
						// Move down one line, ensuring we don't move past the physical screen end.
						CURRENT_ROW = std::min(ROWS - 1, CURRENT_ROW + 1); 
					}
					i++;
				}
				else if (next_char == 'J') // Erase Display (ED): ESC J (Clear Screen)
				{
					// Clear entire screen (same as CSI 2 J) and home cursor
					for (int row = 0; row < ROWS; ++row) 
					{
						std::fill(SCREEN[row], SCREEN[row] + COLS, ' ');
					}
					CURRENT_ROW = 0;
					CURRENT_COL = 0;
					i++; 
				}
				// Handle ESC \b (Backspace) which is not a standard sequence but sometimes appears
				else if (next_char == '\b' || next_char == 0x08)
				{
					i++; // Consume the \b character as a NO-OP when preceded by ESC
				}
				// Combined handlers for simpler one-character ESC sequences (HTS, Reset, etc.)
				else if (next_char == 'c' || next_char == '>' || next_char == '=' || next_char == 'H' || next_char == 'Y' || next_char == 'I' || next_char == '*') 
				{ 
					i++; 
				}
				else if (next_char == 'O' && (i + 2 < raw_text.size())) // SS3 (Single Shift 3): ESC O P, ESC O Q, etc. (3 chars total)
				{
					// We consume the 'O' and the following command character.
					i += 2; 
				}
				else 
				{
					// Sequence not handled, log the error directly
					std::cerr << "DEBUG: UNHANDLED GENERIC ESC SEQUENCE (Next Char): '" << next_char 
											<< "' (0x" << std::hex << (int)(unsigned char)next_char << std::dec << ")" 
											<< std::endl;
					// Crucial fix: Consume the character after ESC even if unhandled,
					// otherwise it gets treated as a printable character.
					i++; 
				}
				continue;
			}
			
			// If ESC wasn't followed by a recognized sequence, skip only the ESC char.
			continue;
		} 
			
		// --- CONTROL CHARACTER HANDLING ---
		else if (c == '\0') // Null Character (NUL, 0x00)
		{
			// Null characters are ignored but consume a byte. This helps with nano desync.
			continue;
		}
		else if (c == '\n' || c == '\v' || c == '\f') 
		{ 
			// Handle Line Feed (LF, 0x0A), Vertical Tab (VT, 0x0B), and Form Feed (FF, 0x0C)
			// All typically behave as LF: move cursor to next line, start of line.
			CURRENT_COL = 0; 
			CURRENT_ROW++; 
		} 
		else if (c == '\r') 
		{ 
			CURRENT_COL = 0; 
		} 
		else if (c == '\t') 
		{
			// Simple tab to next 8-column stop
			int new_col = (CURRENT_COL / 8 + 1) * 8;
			CURRENT_COL = (new_col < COLS) ? new_col : COLS; 
		}
		else if (c == '\b' || c == '\x08')
		{
			if (CURRENT_COL > 0) 
			{ 
				CURRENT_COL--; 
			}
		}
		
		// --- PRINTABLE CHARACTER HANDLING ---
		else if (std::isprint(static_cast<unsigned char>(c))) 
		{
			// 1. Handle Line Wrap
			if (CURRENT_COL >= COLS) 
			{
				CURRENT_COL = 0;
				CURRENT_ROW++;
			}
			
			// 2. Write character to SCREEN
			if (CURRENT_ROW < ROWS && CURRENT_COL < COLS) 
			{
				SCREEN[CURRENT_ROW][CURRENT_COL] = c;
			}
			CURRENT_COL++;
		}

		// --- Post-Character Scroll Check ---
		if (CURRENT_ROW > SCROLL_BOTTOM) 
		{
				this->scroll_up(CURRENT_ROW - SCROLL_BOTTOM); // Calls the new method
				CURRENT_ROW = SCROLL_BOTTOM; 
		}
	}
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