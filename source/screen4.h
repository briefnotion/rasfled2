// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_H
#define SCREEN4_H

#include <stdio.h>
#include <string>
#include <sys/ioctl.h>

// Rasfled Includes
#include "system.h"
#include "screen4_helper.h"
#include "widgets.h"
#include "widgets_automobile.h"
#include "widgets_adsb.h"
#include "widgets_drawing.h"
#include "widgets_advertisement.h"
#include "widgets_alerts.h"
#include "widgets_dot_dot_dot.h"
#include "stringthings.h"
#include "fled_time.h"
#include "animations_handler.h"

// IMGui Includes
#include "../../imgui/imgui.h"
#include "../../imgui/misc/cpp/imgui_stdlib.h"
#include "../../imgui/backends/imgui_impl_opengl2.h"
#include "../../imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

using namespace std;

// ---------------------------------------------------------------------------------------

class SCREEN4
{
  private:
  // TTY things
  struct winsize TERMINAL_WINDOW;

  // Default Colors
  bool DEFAULT_COLORS_CHANGED = false;

  // ImGui things
  bool use_work_area = true;

  bool show_another_window = true;
  bool show_test_window = false;

  ImVec4 clear_color;
  GLFWwindow *  window = nullptr;

  GLFWmonitor * monitor = nullptr;
  const GLFWvidmode * mode = nullptr;
  //ImGuiIO& io = ImGui::GetIO(); //(void)io;
  
  int width = 1280;
  int height = 720;

  // Windows Screen Variables

  // CORNFIRM DIAG
  CONFIRM_DIALOG CONFIRM;

  // BUTTON_TOGGLE_COLOR
  BUTTON_TOGGLE_COLOR BTC_TAB_CONSOLE;
  BUTTON_TOGGLE_COLOR BTC_TAB_AUTOMOBILE;
  BUTTON_TOGGLE_COLOR BTC_TAB_ADSB;
  BUTTON_TOGGLE_COLOR BTC_TAB_LOGS;
  BUTTON_TOGGLE_COLOR BTC_TAB_DOTDOTDOT;

  BUTTON_TOGGLE_COLOR BTC_TIMER;
  BUTTON_TOGGLE_COLOR BTC_OVER_HEAD_LIGHTS;
  BUTTON_TOGGLE_COLOR BTC_LIGHTS;
  BUTTON_TOGGLE_COLOR BTC_HAZARD;
  BUTTON_TOGGLE_COLOR BTC_NIGHT_MODE;
  BUTTON_TOGGLE_COLOR BTC_RUNNING_COLOR;

  BUTTON_TOGGLE_COLOR BTC_DEBUG;
  BUTTON_TOGGLE_COLOR BTC_DEBUG_2;
  BUTTON_TOGGLE_COLOR BTC_DEBUG_4;
  BUTTON_TOGGLE_COLOR BTC_DEBUG_1;
  BUTTON_TOGGLE_COLOR BTC_DEBUG_3;
  
  // BUTTON_COLOR
  BUTTON_COLOR BT_DISPLAY_ALERTS;
  BUTTON_COLOR BT_FLASH;
  BUTTON_COLOR BT_CARDS;
  BUTTON_COLOR BT_DOTDOTDOT;
  BUTTON_COLOR BT_CLEAR_ANIMS;
  BUTTON_COLOR BT_SYSTEM;
  BUTTON_COLOR BT_BACK_1;
  BUTTON_COLOR BT_EXIT;
  BUTTON_COLOR BT_SYSTEM_SHUT_DOWN;
  BUTTON_COLOR BT_REBOOT;
  BUTTON_COLOR BT_DEBUG;
  BUTTON_COLOR BT_BACK_2;
  
  BUTTON_COLOR BT_OVER_RED;
  BUTTON_COLOR BT_OVER_GREEN;
  BUTTON_COLOR BT_OVER_BLUE;
  BUTTON_COLOR BT_OVER_PURPLE;
  BUTTON_COLOR BT_OVER_YELLOW;
  BUTTON_COLOR BT_OVER_CYAN;
  BUTTON_COLOR BT_OVER_ORANGE;
  BUTTON_COLOR BT_OVER_WHITE;

  BUTTON_COLOR BT_RUNNING_RED;
  BUTTON_COLOR BT_RUNNING_GREEN;
  BUTTON_COLOR BT_RUNNING_BLUE;
  BUTTON_COLOR BT_RUNNING_PURPLE;
  BUTTON_COLOR BT_RUNNING_YELLOW;
  BUTTON_COLOR BT_RUNNING_CYAN;
  BUTTON_COLOR BT_RUNNING_ORANGE;
  BUTTON_COLOR BT_RUNNING_BLACK;
  BUTTON_COLOR BT_RUNNING_GREY;
  BUTTON_COLOR BT_RUNNING_WHITE;
  BUTTON_COLOR BT_RUNNING_PINK;

  BUTTON_COLOR BT_QR_CODE;
  BUTTON_COLOR BT_REFER;

  //bool DISPLAY_MAIN = true;
  bool DISPLAY_DEBUG = false;
  bool DISPLAY_TIMER = false;
  bool DISPLAY_OVERHEAD_COLOR = false;
  bool DISPLAY_RUNNING_COLOR = false;
  bool DISPLAY_QR_CODE = false;
  bool DISPLAY_REFERENCE_CARD = false;
  bool DISPLAY_COMPASS_WINDOW = false;
  bool DISPLAY_CARDS_WINDOW = false;

  bool DISPLAY_DNFWTS_SHOW = true;

  bool DISPLAY_CONFIRM = false;

  int DISPLAY_MENU = 0;
  int DISPLAY_SCREEN = 0;
  // Screen:
  //  0 - Console
  //  1 - Automobile
  //  2 - ADSB
  //  3 - not coded
  //  4 - Logs
  //  5 - Dot Dot Dot

  bool RESET_KEYBOARD_FOCUS = true;

  bool CONSOLE_SCROLL_TO_BOTTOM = false;
  
  // ---
  // Screen Vars

  TEXTURE_IMAGE QR_CODE;
  TEXTURE_IMAGE REFERENCE_CARD;

  bool COMMAND_TEXT_CHANGED = false;  // for TTY only redraw
  string COMMAND_TEXT = "";
  int PREV_FRAME_KEY_DOWN = 0;

  // ---
  // Screen Widgets
  TEXT_CONSOLE CONSOLE;
  AUTOMOBILE_SCREEN AUTOMOBILE;
  ADSB_SCREEN ADSB;
  //TEXT_CONSOLE GPS_CONSOLE;
  TEXT_CONSOLE DAEMON_LOG;
  ADVERTISEMENT_SCREENS ADVERTISEMENTS;
  DOT_DOT_DOT_SCREEN DOT_DOT_DOT;

  // --- 

  bool RESTACK_WINDOWS = false;

  // --- 
  // Widgets
  ALERT_WIDGET AUTOMOBILE_ALERTS;
  ALERT_WIDGET ADSB_ALERTS;

  W_TEXT_TF LIGHTS;
  W_TEXT_TF DEBUG;
  W_TEXT_TF OVERHEAD;
  W_TEXT_TF HAZARD;
  W_TEXT_TF DAY_NIGHT;
  W_TEXT_TF TIMER;

  W_TEXT_TF GPS;
  W_TEXT_TF COMPASS;

  W_TEXT_TF AUTO;
  W_TEXT_TF ADSB_IND;

  W_TEXT VERSION;
  W_TEXT TEMP;

  ONE_CHAR_LINE_GRAPH OCG;

  BAR_TECH BAR_TIMER;

  COMPASS_WIDGET COMPASS_WINDOW_COMPASS;

  // Blinkers
  TIMED_PING PING_BLINKER;
  bool BLINKER_BLINK = true;

  //bool CHANGED = false;

  void door_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size);
  void hazard_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size);
  void signal_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size);

  void set_screen_default_colors(system_data &sdSysData);

  public:

  int create(system_data &sdSysData);
  // Create Screen and set variables
  // Return 0 OK
  // Return 2 error

  void character_enter(system_data &sdSysData, unsigned char Character, 
                bool Shift_Pressed, bool Backspace_Pressed, bool Enter_Pressed);

  void draw(system_data &sdSysData, ANIMATION_HANDLER &Animations);
  // Render window to terminal

  void shutdown(system_data &sdSysData);
  // Proper close of imgui

  // ----
  // Data Update Paths

  //int get_current_screen();
  //  Returns ID of screen being displayed (DISPLAY_SCREEN)

  //void update_gps_gadgets(system_data &sdSysData, string Text);

  void update_daemon_log(system_data &sdSysData, string Text);

  void update_automobile_gadgets(system_data &sdSysData);

  void update_ADS_B_gadgets(system_data &sdSysData);

  //void update_GPS_gadgets(system_data &sdSysData);

};

// ---------------------------------------------------------------------------------------
#endif