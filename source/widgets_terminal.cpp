// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_terminal.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_TERMINAL_CPP
#define WIDGETS_TERMINAL_CPP

#include "widgets_terminal.h"

// ---------------------------------------------------------------------------------------

void TERMINAL_SCREEN::display(system_data &sdSysData)
{
  ImVec4 working_area = get_working_area();

  ImGui::BeginChild("Terminal", ImVec2(working_area.z, working_area.w), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (CURSOR_BLINK.is_ready(sdSysData.PROGRAM_TIME.current_frame_time()))
    {
      CURSOR_BLINK.set(sdSysData.PROGRAM_TIME.current_frame_time(), 250);
      CURSOR_ON = !CURSOR_ON;
    }

    // Use the public access method from the class (or BUF_MUTEX if you made it public)
    std::lock_guard<std::mutex> lock(sdSysData.TERMINAL_THREAD.BUF_MUTEX);

    for (int row = 0; row < TERMINAL::ROWS; row++)
    {
      // 1. Construct the string directly from the char array slice.
      // This is much faster as it avoids the concatenation loop.
      std::string line(sdSysData.TERMINAL_THREAD.SCREEN[row], TERMINAL::COLS);

      if (CURSOR_ON)
      {
        if (row == sdSysData.TERMINAL_THREAD.CURRENT_ROW)
        {
          // limited cursor selection.
          line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '_';

          //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '■'; // U+25A0

          //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = static_cast<char>(219); // ASCII full block █

          //line.replace(sdSysData.TERMINAL_THREAD.CURRENT_COL, 1, u8"■");

          //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = static_cast<char>(164); // ¤
          //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '¤';
        }
      }

      ImGui::TextUnformatted(line.c_str());
    }
  }
  ImGui::EndChild();

  // ---

  static const int FKEY_PARAMS[] = 
  {
    11, // F1: ESC [ 1 1 ~
    12, // F2: ESC [ 1 2 ~
    13, // F3: ESC [ 1 3 ~
    14, // F4: ESC [ 1 4 ~
    15, // F5: ESC [ 1 5 ~
    17, // F6: ESC [ 1 7 ~
    18, // F7: ESC [ 1 8 ~
    19, // F8: ESC [ 1 9 ~
    20, // F9: ESC [ 2 0 ~
    21, // F10: ESC [ 2 1 ~
    23, // F11: ESC [ 2 3 ~
    24  // F12: ESC [ 2 4 ~
  };

  {
    // 1. Handle Special Keys (Arrows, F-Keys, Enter, Backspace, Control Sequences)

    // Use a string buffer to capture the command, ensuring only ONE special key is processed per frame.
    std::string command_to_send;

    // Determine the current cursor key mode to send the correct sequence
    bool app_cursor_mode = sdSysData.TERMINAL_THREAD.APP_CURSOR_MODE;

    // Sequences (Normal: CSI, Application: SS3)
    const char* up_seq    = app_cursor_mode ? "\x1BOA" : "\x1B[A";
    const char* down_seq  = app_cursor_mode ? "\x1BOB" : "\x1B[B";
    const char* right_seq = app_cursor_mode ? "\x1BOC" : "\x1B[C";
    const char* left_seq  = app_cursor_mode ? "\x1BOD" : "\x1B[D";

    // --- Arrow Keys for Navigation (Prioritized 1) ---
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false))
    {
      command_to_send = up_seq;
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))
    {
      command_to_send = down_seq;
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))
    {
      command_to_send = left_seq;
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false))
    {
      command_to_send = right_seq;
    }

    // --- Standard Control Characters (Prioritized 2) ---
    // Enter/Return
    else if (ImGui::IsKeyPressed(ImGuiKey_Enter, false))
    {
      // Send Carriage Return (\r), standard input for execution
      command_to_send = "\r";
    }
    // Backspace
    else if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false))
    {
      // Send ASCII 127 (DEL), which acts as a backspace in a shell
      command_to_send = "\x7F";
    }
    // Delete (sends Ctrl+D/EOF, which is common behavior)
    else if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
    {
      // Send Ctrl+D (EOF/Delete)
      command_to_send = "\x04";
    }
    // ESCAPE KEY (Sends single byte \x1B)
    else if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
    {
      command_to_send = "\x1B";
    }

    // --- Navigation/Editing Keys (CSI P ~) (Prioritized 3) ---
    // Home: ESC [ 1 ~
    else if (ImGui::IsKeyPressed(ImGuiKey_Home, false))
    {
      command_to_send = "\x1B[1~";
    }
    // Insert: ESC [ 2 ~
    else if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
    {
      command_to_send = "\x1B[2~";
    }
    // End: ESC [ 4 ~
    else if (ImGui::IsKeyPressed(ImGuiKey_End, false))
    {
      command_to_send = "\x1B[4~";
    }
    // Page Up: ESC [ 5 ~
    else if (ImGui::IsKeyPressed(ImGuiKey_PageUp, false))
    {
      command_to_send = "\x1B[5~";
    }
    // Page Down: ESC [ 6 ~
    else if (ImGui::IsKeyPressed(ImGuiKey_PageDown, false))
    {
      command_to_send = "\x1B[6~";
    }
    // Tab Key
    else if (ImGui::IsKeyPressed(ImGuiKey_Tab, false))
    {
      command_to_send = "\x09";
    }

    // --- GENERIC Control Key Sequences (Ctrl + Letter) (Prioritized 4) ---
    else if (ImGui::GetIO().KeyCtrl)
    {
      // Loop over all possible named keys
      for (int i = ImGuiKey_A; i <= ImGuiKey_Z; i++)
      {
        if (ImGui::IsKeyPressed((ImGuiKey)i, false))
        {
          // Calculate the corresponding ASCII control code:
          // ASCII_Key_Value - ASCII_A + 1
          char control_char = (char)(i - ImGuiKey_A + 1);

          command_to_send.assign(1, control_char);
          break;
        }
      }
    }

    // --- Function Keys (F1-F12) (Prioritized 5) ---
    // Check for F-keys ONLY if no other special key was found yet.
    if (command_to_send.empty())
    {
      for (int f_key_idx = 0; f_key_idx < 12; ++f_key_idx)
      {
        ImGuiKey key = (ImGuiKey)(ImGuiKey_F1 + f_key_idx);

        if (ImGui::IsKeyPressed(key, false))
        {
          int param = FKEY_PARAMS[f_key_idx];
          // Format: ESC [ P ~
          command_to_send = "\x1B[" + std::to_string(param) + "~";
          break; 
        }
      }
    }

    // --- Send Command ---
    if (!command_to_send.empty())
    {
      sdSysData.TERMINAL_THREAD.send_command(command_to_send);
    }

    // 2. Handle Printable Characters (Letters, Numbers, Symbols) - Text Input
    // This is run independently of the special key checks above.
    ImGuiIO& io = ImGui::GetIO();
    if (io.InputQueueCharacters.size() > 0)
    {
      // Lock the terminal's mutex before sending characters to ensure thread safety
      // if the terminal state relies on input order.
      std::lock_guard<std::mutex> lock(sdSysData.TERMINAL_THREAD.get_mutex()); 

      for (char c : io.InputQueueCharacters)
      {
        // Send the exact character that ImGui resolved from the keyboard state
        std::string char_to_send(1, c);
        sdSysData.TERMINAL_THREAD.send_command(char_to_send);
      }

      // Clear the queue for the next frame
      io.InputQueueCharacters.clear();
    }
  }

  // ----

}

// ---------------------------------------------------------------------------------------

#endif