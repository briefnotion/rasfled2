// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_CPP
#define SCREEN4_CPP

#include "screen4.h"

// ---------------------------------------------------------------------------------------

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// ---------------------------------------------------------------------------------------

struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

int SCREEN4::create()
{
  // Set colors
  COLOR_SELECT.init();

  // Init and prepare DISPLAY_SCREEN
  
  clear_color = ImVec4(0.00f, 0.55f, 0.00f, 0.00f);

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
  {
    printf("Graphics Set Error\n");
    return 2;
  }

  // Create window with graphics context
  int width = 1280;
  int height = 720;

  window = glfwCreateWindow(width, height, "OGL window", nullptr, nullptr);
  monitor = glfwGetPrimaryMonitor();
  mode = glfwGetVideoMode(monitor);

  glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);

  if (window == nullptr)
  {
    printf("Window Set To Monitor Error\n");
    return 2;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();
  // Adjust Styles
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 8;
  style.FrameRounding = 8;
  style.ChildRounding = 8;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL2_Init();

  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18.0f);
  ImFont* large_font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 48.0f);
  // Will track as "io.Fonts->Fonts.Data[1]" manually, for now.

  // Prepare Screens
  AUTOMOBILE.create(COLOR_SELECT);

  // Prepare Current Screen Widgets
  LIGHTS.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  LIGHTS.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  LIGHTS.update_text(0, "LIGHTS", "LIGHTS");

  DEBUG.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_RED;
  DEBUG.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DEBUG.update_text(0, "DEBUG", "     ");

  OVERHEAD.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  OVERHEAD.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  OVERHEAD.update_text(0, " OVERHEAD ", " OVERHEAD ");

  HAZARD.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_RED;
  HAZARD.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  HAZARD.update_text(0, "  HAZARD  ", "  HAZARD  ");

  DAY_NIGHT.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DAY_NIGHT.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  DAY_NIGHT.update_text(0, "  DAY  ", " NIGHT ");

  TIMER.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  TIMER.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  TIMER.update_text(0, " TIMER ", " TIMER ");

  DOOR1.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DOOR1.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  DOOR1.update_text(0, " DOOR1", " DOOR1");

  DOOR2.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DOOR2.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  DOOR2.update_text(0, " DOOR2", " DOOR2");

  DOOR3.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DOOR3.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  DOOR3.update_text(0, "DOOR3 ", "DOOR3 ");

  DOOR4.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  DOOR4.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  DOOR4.update_text(0, "DOOR4 ", "DOOR4 ");

  AUTO.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  AUTO.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  AUTO.update_text(0, " AUTO  ", " AUTO  ");

  ADSB_IND.PROPS.COLOR_TRUE = COLOR_SELECT.COLOR_COMB_DEFAULT;
  ADSB_IND.PROPS.COLOR_FALSE = COLOR_SELECT.COLOR_COMB_BLUE;
  ADSB_IND.update_text(0, " ADS-B ", " ADS-B ");

  string version = "v: ";
  version = version + Revision;
  VERSION.update_text(0, version);

  TEMP.update_text(0, "Temp: NA");

  return 0;
}

// ---------------------------------------------------------------------------------------

void SCREEN4::draw(system_data &sdSysData)
{
  // sdSysData.tmeCURRENT_FRAME_TIME

  // Handle Console Iputs
  if (SCREEN_COMMS.printw_q_avail() == true)
  {
    CONSOLE.add_line(SCREEN_COMMS.printw_q_get());
  }
  
  /*
  Does not work yet.
  if (SCREEN_COMMS.command_text_clear_get() == true)
  {
    //COMMAND_TEXT = "";
  }
  */

  DISPLAY_TIMER = sdSysData.cdTIMER.is_active();

  if (SCREEN_COMMS.WINDOW_CLOSE == true)
  {
    WINDOW_CLOSE = true;
  }

  // ---------------------------------------------------------------------------------------
  // ---------------------------------------------------------------------------------------
  // Handle Console Outputs
  ImGuiIO &io = ImGui::GetIO();
  //(void)io;
  
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
  ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

  if (ImGui::Begin("Window", &show_test_window, DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
  {
    ImGui::BeginChild("Main", ImVec2(ImGui::GetContentRegionAvail().x - 85, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
    {
      // ---------------------------------------------------------------------------------------
      // Status Sub Window

      ImGui::BeginChild("Status", ImVec2(ImGui::GetContentRegionAvail().x, 60), true, DEFAULTS.flags_c);
      {

        float region_div_4 = ImGui::GetContentRegionAvail().x / 4;

        // Divide sub window into 4
        // Left
        ImGui::BeginChild("Status Left", ImVec2(region_div_4, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
        {
          
          if (RESET_KEYBOARD_FOCUS == true)
          {
            ImGui::SetKeyboardFocusHere();
            RESET_KEYBOARD_FOCUS = false;
          }

          ImGui::PushFont(io.Fonts->Fonts.Data[1]);
          ImGui::Text("CMD:");
          ImGui::PopFont();

          ImGui::SameLine();
          // Command Line
          {
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
            ImGui::PushItemWidth(100);
            if (ImGui::InputText(" ", &COMMAND_TEXT_TEMP, flags))
            {
              COMMAND_TEXT = COMMAND_TEXT_TEMP;
              COMMAND_TEXT_TEMP = "";
              SCREEN_COMMS.command_text_set(COMMAND_TEXT);
              //SCREEN_COMMS.carrage_return_set();
              RESET_KEYBOARD_FOCUS = true;
            }
            ImGui::PopItemWidth();
          }       
        }
        ImGui::EndChild();

        // Mid
        ImGui::SameLine();

        ImGui::BeginChild("Status Mid", ImVec2(region_div_4 * 2, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
        {
          // Display Lights Off mode toggle.
          ImGui::BeginGroup();
          {
            LIGHTS.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.Lights_On.value());
            LIGHTS.draw(sdSysData.tmeCURRENT_FRAME_TIME);

            DEBUG.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, SCREEN_COMMS.DEBUG_STATUS.DEBUG);
            DEBUG.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
          ImGui::EndGroup();
          
          ImGui::SameLine();

          // Status Group
          ImGui::BeginGroup();
          {
            OVERHEAD.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.booOverheadRunning);
            OVERHEAD.draw(sdSysData.tmeCURRENT_FRAME_TIME);

            HAZARD.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.booHazardRunning);
            HAZARD.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
          ImGui::EndGroup();

          ImGui::SameLine();

          // Status Group
          ImGui::BeginGroup();
          {
            // Lights
            DAY_NIGHT.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.Day_On_With_Override.value());
            DAY_NIGHT.draw(sdSysData.tmeCURRENT_FRAME_TIME);

            // Timer
            TIMER.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.cdTIMER.is_active());
            TIMER.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
          ImGui::EndGroup();

          ImGui::SameLine();
          
          // Door Group
          ImGui::BeginGroup();
          {
            DOOR2.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value);
            DOOR2.draw(sdSysData.tmeCURRENT_FRAME_TIME);
            
            ImGui::SameLine();
            
            DOOR4.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value);
            DOOR4.draw(sdSysData.tmeCURRENT_FRAME_TIME);


            DOOR1.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(0).value);
            DOOR1.draw(sdSysData.tmeCURRENT_FRAME_TIME);
            
            ImGui::SameLine();
            
            DOOR3.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value);
            DOOR3.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
          ImGui::EndGroup();

          ImGui::SameLine();
          
          // Auto ADSB
          ImGui::BeginGroup();
          {
            AUTO.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.CAR_INFO.active());
            AUTO.draw(sdSysData.tmeCURRENT_FRAME_TIME);

            ADSB_IND.update_tf(sdSysData.tmeCURRENT_FRAME_TIME, sdSysData.AIRCRAFT_COORD.is_active());
            ADSB_IND.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
          ImGui::EndGroup();
          
        }
        ImGui::EndChild();

        // Mid Right
        ImGui::SameLine();

        ImGui::BeginChild("Status Right", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
        {
          VERSION.draw(sdSysData.tmeCURRENT_FRAME_TIME);

          if (sdSysData.hsHardware_Status.enabled() == true)
          {
            TEMP.update_text(sdSysData.tmeCURRENT_FRAME_TIME, ("Temp: " + to_string((int)sdSysData.hsHardware_Status.get_temperature()) + "c").c_str());
            TEMP.draw(sdSysData.tmeCURRENT_FRAME_TIME);
          }
        }
        ImGui::EndChild();
       
      }
      ImGui::EndChild();

      // ---------------------------------------------------------------------------------------
      // Console Sub Window

      ImGui::BeginChild("DISPLAY_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 48), false, DEFAULTS.flags_c);
      {
        
        ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

        if (DISPLAY_SCREEN == 0)
        {
          CONSOLE.display("Console", NULL, DEFAULTS.flags_w);
        }

        else if (DISPLAY_SCREEN == 1)
        {
          AUTOMOBILE.display(sdSysData.tmeCURRENT_FRAME_TIME, SCREEN_COMMS, "Automobile", NULL, DEFAULTS.flags_w);
        }

        else if (DISPLAY_SCREEN == 2)
        {
          ADSB.display(sdSysData.tmeCURRENT_FRAME_TIME, SCREEN_COMMS, COLOR_SELECT, "ADSB", NULL, DEFAULTS.flags_w);
        }

        else if (DISPLAY_SCREEN == 4)
        {
          DAEMON_LOG.display("System Logs", NULL, DEFAULTS.flags_w);
        }

        else
        {
          ImGui::Begin(" ", NULL, DEFAULTS.flags_w);
          {
            // Show Nothing
          }
          ImGui::End();
        }
      }
      ImGui::EndChild();

      // ---------------------------------------------------------------------------------------
      // Tabs Sub Window

      ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
      {

        if (button_simple_toggle_color("Console", "Console", DISPLAY_SCREEN == 0, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 0;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("Automobile", "Automobile", DISPLAY_SCREEN == 1, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 1;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("ADSB", "ADSB", DISPLAY_SCREEN == 2, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 2;
        }

        ImGui::SameLine();

        /*
        if (button_simple_toggle_color("Hello\nWorld", "Hello\nWorld", DISPLAY_SCREEN == 3, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 3;
        }
        
        ImGui::SameLine();
        */

        if (button_simple_toggle_color("LOGS", "LOGS", DISPLAY_SCREEN == 4, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 4;
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // ---------------------------------------------------------------------------------------
    // Menus Sub Window

    ImGui::BeginChild("System Menu", ImVec2(0, ImGui::GetContentRegionAvail().y), false, DEFAULTS.flags_c);
    {
      // Main Menu
      if (DISPLAY_MENU == 0)
      {
        if (button_simple_toggle_color("STOP\nTIMER", "START\nTIMER", sdSysData.cdTIMER.is_active(), COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_GREEN, DEFAULTS.SIZE_BUTTON))
        {
          if (sdSysData.cdTIMER.is_active() == false)
          {
            SCREEN_COMMS.command_text_set("  ");
          }
          else
          {
            SCREEN_COMMS.command_text_set(" `");
          }
        }

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        if (button_simple_toggle_color("OVER\nHEAD\nLIGHTS", "OVER\nHEAD\nLIGHTS", sdSysData.booOverheadRunning, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          if (sdSysData.booOverheadRunning == true)
          {
            SCREEN_COMMS.command_text_set("o`");
          }
          else
          {
            if (DISPLAY_OVERHEAD_COLOR == true)
            {
              SCREEN_COMMS.command_text_set("oo");
              DISPLAY_OVERHEAD_COLOR = false;
            }
            else
            {
              DISPLAY_OVERHEAD_COLOR = true;
            }
          }
        }

        if (button_simple_color("FLASH", COLOR_SELECT.COLOR_COMB_GREEN, DEFAULTS.SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set("ff");
        }

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        if (button_simple_toggle_color("LIGHTS\n(On)", "LIGHTS\n(Off)", sdSysData.Lights_On.value(), COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON))
        {
          if (sdSysData.Lights_On.value() == true)
          {
            SCREEN_COMMS.command_text_set(" lightsoff");
          }
          else
          {
            SCREEN_COMMS.command_text_set(" lightson");
          }
        }

        if (button_simple_color("...", COLOR_SELECT.COLOR_COMB_BLUE, DEFAULTS.SIZE_BUTTON))
        {
          DISPLAY_MENU = 1;
        } 
      }
      else if (DISPLAY_MENU == 1)
      {
        if (button_simple_toggle_color("HAZARD\n(On)", "HAZARD\n(Off)", sdSysData.booHazardRunning, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          if (sdSysData.booHazardRunning == true)
          {
            SCREEN_COMMS.command_text_set("h`");
          }
          else
          {
            SCREEN_COMMS.command_text_set("hh");
          }
        }

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        if (button_simple_color("CLEAR\nANIMS", COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set("``");
        }

        if (button_simple_color("SYSTEM", COLOR_SELECT.COLOR_COMB_BLUE, DEFAULTS.SIZE_BUTTON))
        {
          DISPLAY_MENU = 2;
        }

        if (button_simple_color("<-", COLOR_SELECT.COLOR_COMB_BLUE, DEFAULTS.SIZE_BUTTON))
        {
          DISPLAY_MENU = 0;
        }
      }
      else if (DISPLAY_MENU == 2)
      {
        if (button_simple_color("EXIT", COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          WINDOW_CLOSE = true;
        }

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        if (button_simple_color("SYSTEM\nSHUT\nDOWN", COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set(" comshutd");
        }

        if (button_simple_color("SYSTEM\nREBOOT", COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set(" reboot");
        }

        button_simple_enabled(" ", false, DEFAULTS.SIZE_BUTTON);

        if (button_simple_color("DEBUG", COLOR_SELECT.COLOR_COMB_YELLOW, DEFAULTS.SIZE_BUTTON))
        {
          DISPLAY_DEBUG = !DISPLAY_DEBUG;
        }

        if (button_simple_color("<-", COLOR_SELECT.COLOR_COMB_BLUE, DEFAULTS.SIZE_BUTTON))
        {
          DISPLAY_MENU = 0;
        }
      }
      else
      {
        DISPLAY_MENU = 0;
      }
    }
    ImGui::EndChild();

  }
  ImGui::End();

  // ---------------------------------------------------------------------------------------
  // Debug Window
  
  if (DISPLAY_DEBUG == true)
  {
    ImGui::SetNextWindowSize(ImVec2(270, 152));
    if (ImGui::Begin("Debug", &DISPLAY_DEBUG, DEFAULTS.flags_w_pop)) 
    {
      ImGui::Text("%.3f ms/frame  %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
      //ImGui::Text("%.1f FPS", io.Framerate);

      if (button_simple_toggle_color("Dug", "Dbug", SCREEN_COMMS.DEBUG_STATUS.DEBUG, COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON_SMALL))
      {
        SCREEN_COMMS.DEBUG_STATUS.DEBUG = ! SCREEN_COMMS.DEBUG_STATUS.DEBUG;
      }

      ImGui::SameLine();

      button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(1)).c_str(), false, DEFAULTS.SIZE_BUTTON_SMALL);

      ImGui::SameLine();

      if (button_simple_toggle_color("2", "2", SCREEN_COMMS.DEBUG_STATUS.DOOR[1], COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_SMALL))
      {
        SCREEN_COMMS.DEBUG_STATUS.DOOR[1] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
      }

      ImGui::SameLine();

      if (button_simple_toggle_color("4", "4", SCREEN_COMMS.DEBUG_STATUS.DOOR[3], COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_SMALL))
      {
        SCREEN_COMMS.DEBUG_STATUS.DOOR[3] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[3];
      }

      ImGui::SameLine();

      button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(3)).c_str(), false, DEFAULTS.SIZE_BUTTON_SMALL);

      //-

      if (ImGui::Button("X", DEFAULTS.SIZE_BUTTON_SMALL))
      {
        DISPLAY_DEBUG = false;
      }

      ImGui::SameLine();

      button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(0)).c_str(), false, DEFAULTS.SIZE_BUTTON_SMALL);

      ImGui::SameLine();

      if (button_simple_toggle_color("1", "1", SCREEN_COMMS.DEBUG_STATUS.DOOR[0], COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_SMALL))
      {
        SCREEN_COMMS.DEBUG_STATUS.DOOR[0] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
      }

      ImGui::SameLine();

        ImGui::TableNextColumn();
        if (button_simple_toggle_color("3", "3", SCREEN_COMMS.DEBUG_STATUS.DOOR[2], COLOR_SELECT.COLOR_COMB_WHITE, COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[2] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
        }

      ImGui::SameLine();

      button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(2)).c_str(), false, DEFAULTS.SIZE_BUTTON_SMALL);
    }
    ImGui::End();
  }

  // ---------------------------------------------------------------------------------------
  // Timer Window

  if (DISPLAY_TIMER == true)
  {
    ImGui::SetNextWindowSize(ImVec2(200, 60));
    if (ImGui::Begin("Timer", &DISPLAY_TIMER, DEFAULTS.flags_w_pop)) 
    {
      long elaped_time = 0;
      unsigned long duration_time = 0;
      long remaining_time = 0;

      // Calculate
      duration_time = sdSysData.cdTIMER.duration();
      elaped_time = sdSysData.cdTIMER.elapsed_time(sdSysData.tmeCURRENT_FRAME_TIME);
      remaining_time = duration_time - elaped_time;


      string timer_dsp =  linemerge_right_justify(2, "00", to_string(millis_to_time_minutes(remaining_time))) + ":" + 
                          linemerge_right_justify(2, "00", to_string(millis_to_time_seconds(remaining_time)));

      // Display Timer

      ImGui::ProgressBar((((float)remaining_time) / (float)duration_time), ImVec2(-1.0f,0.0f), timer_dsp.c_str());
    }
    ImGui::End();
  }

  // ---------------------------------------------------------------------------------------
  // Overhead Color Window
  
  if (DISPLAY_OVERHEAD_COLOR == true)
  {
    ImGui::SetNextWindowSize(ImVec2(143, 290));
    if (ImGui::Begin("Overhead Color", &DISPLAY_DEBUG, DEFAULTS.flags_w_pop)) 
    {
      if (button_simple_color("Red", COLOR_SELECT.COLOR_COMB_RED, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("or");
        DISPLAY_OVERHEAD_COLOR = false;
      }

      ImGui::SameLine();

      if (button_simple_color("Green", COLOR_SELECT.COLOR_COMB_GREEN, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("og");
        DISPLAY_OVERHEAD_COLOR = false;
      }

      if (button_simple_color("Blue", COLOR_SELECT.COLOR_COMB_BLUE, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("ob");
        DISPLAY_OVERHEAD_COLOR = false;
      }

      ImGui::SameLine();

      if (button_simple_color("Purple", COLOR_SELECT.COLOR_COMB_PURPLE, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("ou");
        DISPLAY_OVERHEAD_COLOR = false;
      }
      
      if (button_simple_color("Yellow", COLOR_SELECT.COLOR_COMB_YELLOW, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("oy");
        DISPLAY_OVERHEAD_COLOR = false;
      }

      ImGui::SameLine();

      if (button_simple_color("Cyan", COLOR_SELECT.COLOR_COMB_CYAN, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("oc");
        DISPLAY_OVERHEAD_COLOR = false;
      }
      
      if (button_simple_color("Orange", COLOR_SELECT.COLOR_COMB_DEFAULT, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("on");
        DISPLAY_OVERHEAD_COLOR = false;
      }

      ImGui::SameLine();

      if (button_simple_color("White", COLOR_SELECT.COLOR_COMB_WHITE, DEFAULTS.SIZE_BUTTON_MEDIUM))
      {
        SCREEN_COMMS.command_text_set("ow");
        DISPLAY_OVERHEAD_COLOR = false;
      }
    }
    ImGui::End();
  }

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
  //glUseProgram(last_program);

  glfwMakeContextCurrent(window);
  glfwSwapBuffers(window);

  // ---------------------------------------------------------------------------------------
  // ---------------------------------------------------------------------------------------
  // Handle Console Post Process
  //SCREEN_COMMS.command_text_set(COMMAND_TEXT);
}

void SCREEN4::shutdown()
{
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void SCREEN4::update_daemon_log(string Text)
{
  DAEMON_LOG.add_line(Text);
}

void SCREEN4::update_automobile_gadgets(unsigned long &tmeCurrentMillis, system_data &sdSysData)
{
  if(sdSysData.CAR_INFO.CHANGED == true)
  {
    AUTOMOBILE.update(sdSysData, tmeCurrentMillis);
  }
}

void SCREEN4::update_ADS_B_gadgets(unsigned long &tmeCurrentMillis, system_data &sdSysData)
{
  if(sdSysData.AIRCRAFT_COORD.DATA.CHANGED == true)
  {
    ADSB.update(sdSysData, tmeCurrentMillis);
  }
}

// ---------------------------------------------------------------------------------------
#endif