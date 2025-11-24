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

void TERMINAL_SCREEN::display_terminal(system_data &sdSysData, TERMINAL &Term)
{
  // Disable main program terminal input:
  sdSysData.PANEL_CONTROL.console_disable();

  // Gather keyboard input
  {
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
      bool app_cursor_mode = Term.APP_CURSOR_MODE;

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
        command_to_send = std::string("\r");
      }
      // Backspace
      else if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false))
      {
        // Send ASCII 127 (DEL), which acts as a backspace in a shell
        command_to_send = std::string("\x7F");
      }
      // Delete (sends Ctrl+D/EOF, which is common behavior)
      else if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
      {
        // Send Ctrl+D (EOF/Delete)
        command_to_send = std::string("\x04");
      }
      // ESCAPE KEY (Sends single byte \x1B)
      else if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
      {
        command_to_send = std::string("\x1B");
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
        Term.send_command(command_to_send);
      }

      // 2. Handle Printable Characters (Letters, Numbers, Symbols) - Text Input
      // This is run independently of the special key checks above.
      ImGuiIO& io = ImGui::GetIO();
      if (io.InputQueueCharacters.size() > 0)
      {
        // Lock the terminal's mutex before sending characters to ensure thread safety
        // if the terminal state relies on input order.
        std::lock_guard<std::mutex> lock(Term.get_mutex()); 

        for (char c : io.InputQueueCharacters)
        {
          // Send the exact character that ImGui resolved from the keyboard state
          std::string char_to_send(1, c);
          Term.send_command(char_to_send);
        }

        // Clear the queue for the next frame
        io.InputQueueCharacters.clear();
      }
    }
  }

  // Create terminal screen window
  //ImGui::BeginChild("Terminal", ImVec2(working_area.z, working_area.w), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (CURSOR_BLINK.is_ready(sdSysData.PROGRAM_TIME.current_frame_time()))
    {
      CURSOR_BLINK.set(sdSysData.PROGRAM_TIME.current_frame_time(), 250);
      CURSOR_ON = !CURSOR_ON;
    }

    // Draw Terminal Screen
    {
      // Use the public access method from the class (or BUF_MUTEX if you made it public)
      std::lock_guard<std::mutex> lock(Term.BUF_MUTEX);

      {
        ImVec2 start_position = ImVec2(0.0f,0.0f);
        ImVec2 position = start_position;
        float font_height = ImGui::GetFont()->FontSize;

        std::string line = "";
        std::string line_reverse_map = "";
        std::string line_reverse = "";

        ImGui::PushFont(sdSysData.PANEL_CONTROL.FONT_CONSOLE);

        for (int row = 0; row < TERMINAL::ROWS; row++)
        {
          Term.get_line_text(row, CURSOR_ON, line, line_reverse, line_reverse_map);

          // Text
          ImGui::SetCursorPos(position);
          ImGui::TextUnformatted(line.c_str());

          // Reverse Map
          ImGui::SetCursorPos(position);
          ImGui::TextUnformatted(line_reverse_map.c_str());

          // Reverse Text
          ImGui::SetCursorPos(position);
          ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
          ImGui::TextUnformatted(line_reverse.c_str());
          ImGui::PopStyleColor();

          // Addvance to next line
          position.y += font_height;
        }

        ImGui::PopFont();

      }
    }
    //ImGui::EndChild();
  }

  /*
  Extras:
      // limited cursor selection.
      //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '_';  // This works

      //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '■'; // U+25A0

      //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = static_cast<char>(219); // ASCII full block █

      // These both work.
      line.replace(sdSysData.TERMINAL_THREAD.CURRENT_COL, 1, std::string(reinterpret_cast<const char*>("\xE2\x96\x88")));


      //  or
      //line.replace(sdSysData.TERMINAL_THREAD.CURRENT_COL, 1, std::string(reinterpret_cast<const char*>(u8"█")));

      //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = static_cast<char>(164); // ¤
      //line[sdSysData.TERMINAL_THREAD.CURRENT_COL] = '¤';
      
      // Font test:
      //ImGui::TextUnformatted("- (─, │, ┌, ┐, etc.), block fills (█, ▓, ░");
      //ImGui::TextUnformatted("Symbols: [ ] { } █ ▓ ░ ┌ ┐ └ ┘ ─ │");
      //ImGui::TextUnformatted("Symbols: [ ] { } █ ▓ ░ ┌ ┐ └ ┘ ─ │ ⠁ ⠃ ⠇ ⠿");
  */

}

void TERMINAL_SCREEN::display(system_data &sdSysData)
{
  /*
  Darn - this is pointless
  // Print Messages (placed here because terminals have no process points.  maybe add later)
  for (size_t term_num = 0; term_num < sdSysData.TERMINAL_THREADS.size(); term_num++)
  {
    if (sdSysData.TERMINAL_THREADS[term_num] -> MESSAGES.size() > 0)
    {
      for (size_t message = 0; message < sdSysData.TERMINAL_THREADS[term_num] -> MESSAGES.size(); message++)
      {
        sdSysData.SCREEN_COMMS.printw(sdSysData.TERMINAL_THREADS[term_num] -> MESSAGES[message]);
      }
      sdSysData.TERMINAL_THREADS[term_num] -> MESSAGES.clear();
    }
  }
  */

  // Screen
  ImVec4 working_area = get_working_area();
  
  // Disable other buttons from getting accidentally clicked.
  ImGui::PushAllowKeyboardFocus(false);

  ImGui::BeginChild("Terminal", ImVec2(working_area.z, working_area.w), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    ImGui::SetCursorPos(ImVec2(0,0));
    if (SHOW_BUTTONS == false)
    {
      if (ImGui::InvisibleButton("Hide Buttons", ImVec2(working_area.z, working_area.w)))
      {
        SHOW_BUTTONS = true;
        SHOW_BUTTONS_TIMER.set(sdSysData.PROGRAM_TIME.current_frame_time(), 30000);
      }
    }
    else
    {
      if (SHOW_BUTTONS_TIMER.is_ready(sdSysData.PROGRAM_TIME.current_frame_time()))
      {
        SHOW_BUTTONS = false;
      }
    }

    if (SHOW_BUTTONS)
    {
      while (BC_TERMINAL_SELECT_BUTTONS.size() > sdSysData.TERMINAL_THREADS.size() + 1)
      {
        BC_TERMINAL_SELECT_BUTTONS.pop_back();
      }

      while (BC_TERMINAL_SELECT_BUTTONS.size() < sdSysData.TERMINAL_THREADS.size() + 1)
      {
        BUTTON_TOGGLE_COLOR tmp_terminal_button;
        BC_TERMINAL_SELECT_BUTTONS.push_back(tmp_terminal_button);
      }

      ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
      for (int pos = 0; pos < (int)sdSysData.TERMINAL_THREADS.size(); pos++)
      {
        if (BC_TERMINAL_SELECT_BUTTONS[pos].button_toggle_color(sdSysData, to_string(pos + 1), to_string(pos + 1), 
                                                    sdSysData.PANEL_CONTROL.PANELS.TERMINAL_SELECTED == pos, 
                                                    RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          sdSysData.PANEL_CONTROL.PANELS.TERMINAL_SELECTED = pos;
        }
        ImGui::SameLine();
      }
      
      if (sdSysData.TERMINAL_THREADS.size() < 9)
      {
        if (BC_TERMINAL_SELECT_BUTTONS[sdSysData.TERMINAL_THREADS.size()].button_toggle_color(sdSysData, "+", "+", 
                                          false, 
                                          RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          auto new_terminal_ptr = std::make_unique<TERMINAL>();
          new_terminal_ptr->create();
          sdSysData.TERMINAL_THREADS.push_back(std::move(new_terminal_ptr));
          sdSysData.PANEL_CONTROL.PANELS.TERMINAL_SELECTED = sdSysData.TERMINAL_THREADS.size() - 1;
        }
      }
    }

    if (sdSysData.TERMINAL_THREADS.size() > 0)
    {
      display_terminal(sdSysData, *sdSysData.TERMINAL_THREADS[sdSysData.PANEL_CONTROL.PANELS.TERMINAL_SELECTED]);
    }

    ImGui::EndChild();
  }

  ImGui::PopAllowKeyboardFocus();

}

// ---------------------------------------------------------------------------------------

#endif