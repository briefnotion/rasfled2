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

int SCREEN4::create(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    // Set colors
    sdSysData.COLOR_SELECT.init();

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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls //lols

    // For keyboard input redirect
    io.WantCaptureKeyboard = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // Adjust Styles
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8;
    style.FrameRounding = 8;
    style.ChildRounding = 8;

    style.ScrollbarSize = 20;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18.0f);
    //ImFont* large_font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 48.0f);
    ImFont* large_font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 57.0f);
    ImFont* medium_font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 28.0f);
    // Will track as "io.Fonts->Fonts.Data[1]" manually, for now.

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Generate an empty from on create so Fonts, and whatever else is needed, can load.
    //  Necessary for ImGui::CalcTextSize(hzxjhfzxjhxz)
    ImGui::EndFrame();

    // QR_Image
    QR_CODE.create(FILES_DIRECTORY, FILES_QR_CODE);

    // Prepare Screens
    AUTOMOBILE.create(sdSysData);
    ADSB.create(sdSysData);

    // Prepare Current Screen Widgets
    LIGHTS.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    LIGHTS.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    LIGHTS.update_text(sdSysData, "LED ILLUM", "LED ILLUM");

    DEBUG.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_RED;
    DEBUG.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DEBUG.update_text(sdSysData, "  DEBUG  ", "         ");

    OVERHEAD.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    OVERHEAD.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    OVERHEAD.update_text(sdSysData, " OVERHEAD ", " OVERHEAD ");

    HAZARD.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_RED;
    HAZARD.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    HAZARD.update_text(sdSysData, "  HAZARD  ", "  HAZARD  ");

    DAY_NIGHT.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DAY_NIGHT.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    DAY_NIGHT.update_text(sdSysData, "  DAY  ", " NIGHT ");

    TIMER.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    TIMER.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    TIMER.update_text(sdSysData, " TIMER ", " TIMER ");

    DOOR1.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DOOR1.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    DOOR1.update_text(sdSysData, " DOOR1", " DOOR1");

    DOOR2.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DOOR2.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    DOOR2.update_text(sdSysData, " DOOR2", " DOOR2");

    DOOR3.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DOOR3.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    DOOR3.update_text(sdSysData, "DOOR3 ", "DOOR3 ");

    DOOR4.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    DOOR4.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    DOOR4.update_text(sdSysData, "DOOR4 ", "DOOR4 ");

    AUTO.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    AUTO.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    AUTO.update_text(sdSysData, " AUTO  ", " AUTO  ");

    ADSB_IND.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    ADSB_IND.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    ADSB_IND.update_text(sdSysData, " ADS-B ", " ADS-B ");

    string version = "v: ";
    version = version + Revision;
    VERSION.update_text(sdSysData, version);

    TEMP.update_text(sdSysData, "Temp: NA");
  }

  return 0;
}

// ---------------------------------------------------------------------------------------

void SCREEN4::draw(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    // Handle Console Iputs
    if (SCREEN_COMMS.printw_q_avail() == true)
    {
      CONSOLE.add_line(SCREEN_COMMS.printw_q_get());
    }
    
    //Does not work yet?
    if (SCREEN_COMMS.command_text_clear_get() == true)
    {
      COMMAND_TEXT = "";
    }

    DISPLAY_TIMER = sdSysData.cdTIMER.is_active();

    if (SCREEN_COMMS.WINDOW_CLOSE == true)
    {
      WINDOW_CLOSE = true;
    }

    // ---------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------
    // Handle Console Outputs
    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    glfwPollEvents();

    // Check for keybaord input
    {
      char character_pressed = 0;
      bool shift_pressed = false;
      bool backspace_pressed = false;
      bool enter_pressed = false;

      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
      {
        if (io.KeysDown[i])
        {
          if ((i == 259 || i == ImGuiKey_Backspace) && COMMAND_TEXT.length() > 0)
          {
            character_pressed = i;
            backspace_pressed = true;
          }
          if ( i == 257 || i == ImGuiKey_Enter || i == ImGuiKey_KeypadEnter)
          {
            character_pressed = i;
            enter_pressed = true;
          }
          if (i == 340 || i == ImGuiKey_ModShift || i == ImGuiKey_LeftShift || i == ImGuiKey_RightShift)
          {
            shift_pressed = true;
          }
          if (i >= 32 && i < 128)
          {
            character_pressed = i;
          }
        }
      }

      if (character_pressed > 0 && character_pressed != PREV_FRAME_KEY_DOWN)
      {
        PREV_FRAME_KEY_DOWN = character_pressed;

        // Letters
        if (character_pressed >= 65 && character_pressed <= 90)
        {
          if (shift_pressed)
          {
            COMMAND_TEXT += character_pressed;
            SCREEN_COMMS.command_text_set(COMMAND_TEXT);
          }
          else
          {
            COMMAND_TEXT += character_pressed + 32;
            SCREEN_COMMS.command_text_set(COMMAND_TEXT);
          }
        }

        // Symbols
        else if (character_pressed >= 32 && character_pressed <= 64 || 
                  character_pressed >= 91 && character_pressed <= 96 || 
                  character_pressed >= 123 && character_pressed <= 127)
        {
          COMMAND_TEXT += character_pressed;
          SCREEN_COMMS.command_text_set(COMMAND_TEXT);
        }

        // Backspace 
        else if (backspace_pressed)
        {
          COMMAND_TEXT.resize(COMMAND_TEXT.size() -1);
        }

        // CR
        else if (enter_pressed)
        {
          SCREEN_COMMS.carrage_return_set();
          COMMAND_TEXT = "";
        }
      }
      else if (character_pressed == 0)
      {
        PREV_FRAME_KEY_DOWN = 0;
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (ImGui::Begin("Window", &show_test_window, sdSysData.SCREEN_DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    {
      ImGui::BeginChild("Main", ImVec2(ImGui::GetContentRegionAvail().x - 85, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
        // ---------------------------------------------------------------------------------------
        // Status Sub Window

        ImGui::BeginChild("Status", ImVec2(ImGui::GetContentRegionAvail().x, 60), true, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          float region_div_4 = ImGui::GetContentRegionAvail().x / 4;

          // Divide sub window into 4
          // Left
          ImGui::BeginChild("Status Left", ImVec2(region_div_4, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (RESET_KEYBOARD_FOCUS == true)
            {
              ImGui::SetKeyboardFocusHere();
              RESET_KEYBOARD_FOCUS = false;
            }

            ImGui::PushFont(io.Fonts->Fonts.Data[2]);
            ImGui::Text("CMD:");

            ImGui::SameLine();

            ImGui::Text(COMMAND_TEXT.c_str());
            simple_wrap_text_box();
            ImGui::PopFont(); 
          }
          ImGui::EndChild();

          // Mid
          ImGui::SameLine();

          ImGui::BeginChild("Status Mid", ImVec2(region_div_4 * 2, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            // Display Lights Off mode toggle.
            ImGui::BeginGroup();
            {
              LIGHTS.update_tf(sdSysData, sdSysData.Lights_On.value());
              LIGHTS.draw(sdSysData);

              DEBUG.update_tf(sdSysData, SCREEN_COMMS.DEBUG_STATUS.DEBUG);
              DEBUG.draw(sdSysData);
            }
            ImGui::EndGroup();
            
            ImGui::SameLine();

            // Status Group
            ImGui::BeginGroup();
            {
              OVERHEAD.update_tf(sdSysData, sdSysData.booOverheadRunning);
              OVERHEAD.draw(sdSysData);

              HAZARD.update_tf(sdSysData, sdSysData.booHazardRunning);
              HAZARD.draw(sdSysData);
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            // Status Group
            ImGui::BeginGroup();
            {
              // Lights
              DAY_NIGHT.update_tf(sdSysData, sdSysData.Day_On_With_Override.value());
              DAY_NIGHT.draw(sdSysData);

              // Timer
              TIMER.update_tf(sdSysData, sdSysData.cdTIMER.is_active());
              TIMER.draw(sdSysData);
            }
            ImGui::EndGroup();

            ImGui::SameLine();
            
            // Door Group
            ImGui::BeginGroup();
            {
              DOOR2.update_tf(sdSysData, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value);
              DOOR2.draw(sdSysData);
              
              ImGui::SameLine();
              
              DOOR4.update_tf(sdSysData, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value);
              DOOR4.draw(sdSysData);


              DOOR1.update_tf(sdSysData, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(0).value);
              DOOR1.draw(sdSysData);
              
              ImGui::SameLine();
              
              DOOR3.update_tf(sdSysData, sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value);
              DOOR3.draw(sdSysData);
            }
            ImGui::EndGroup();

            ImGui::SameLine();
            
            // Auto ADSB
            ImGui::BeginGroup();
            {
              AUTO.update_tf(sdSysData, sdSysData.CAR_INFO.active());
              AUTO.draw(sdSysData);

              ADSB_IND.update_tf(sdSysData, sdSysData.AIRCRAFT_COORD.is_active());
              ADSB_IND.draw(sdSysData);
            }
            ImGui::EndGroup();
            
          }
          ImGui::EndChild();

          // Mid Right
          ImGui::SameLine();

          ImGui::BeginChild("Status Right", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            VERSION.draw(sdSysData);

            if (sdSysData.hsHardware_Status.enabled() == true)
            {
              TEMP.update_text(sdSysData, ("Temp: " + to_string((int)sdSysData.hsHardware_Status.get_temperature()) + "c").c_str());
              TEMP.draw(sdSysData);
            }
          }
          ImGui::EndChild();
        
        }
        ImGui::EndChild();

        // ---------------------------------------------------------------------------------------
        // Console Sub Window

        ImGui::BeginChild("DISPLAY_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 48), false, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          
          ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
          ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

          if (DISPLAY_SCREEN == 0)
          {
            CONSOLE.display("Console", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
          }

          else if (DISPLAY_SCREEN == 1)
          {
            AUTOMOBILE.display(sdSysData, SCREEN_COMMS, "Automobile", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
          }

          else if (DISPLAY_SCREEN == 2)
          {
            ADSB.display(sdSysData, SCREEN_COMMS, "ADSB", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
          }

          else if (DISPLAY_SCREEN == 4)
          {
            DAEMON_LOG.display("System Logs", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
          }

          else
          {
            ImGui::Begin(" ", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
            {
              // Show Nothing
            }
            ImGui::End();
          }
        }
        ImGui::EndChild();

        // ---------------------------------------------------------------------------------------
        // Tabs Sub Window

        ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
        {

          if (button_simple_toggle_color("Console", "Console", DISPLAY_SCREEN == 0, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
          {
            DISPLAY_SCREEN = 0;
          }

          ImGui::SameLine();

          if (button_simple_toggle_color("Automobile", "Automobile", DISPLAY_SCREEN == 1, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
          {
            DISPLAY_SCREEN = 1;
          }

          ImGui::SameLine();

          if (button_simple_toggle_color("ADSB", "ADSB", DISPLAY_SCREEN == 2, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
          {
            DISPLAY_SCREEN = 2;
          }

          ImGui::SameLine();

          if (button_simple_toggle_color("LOGS", "LOGS", DISPLAY_SCREEN == 4, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
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

      ImGui::BeginChild("System Menu", ImVec2(0, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
        // Main Menu
        if (DISPLAY_MENU == 0)
        {
          if (button_simple_toggle_color("STOP\nTIMER", "START\nTIMER", sdSysData.cdTIMER.is_active(), sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

          button_simple_enabled(" ", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

          if (button_simple_toggle_color("OVER\nHEAD\nLIGHTS", "OVER\nHEAD\nLIGHTS", sdSysData.booOverheadRunning, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

          if (button_simple_color("FLASH", sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            SCREEN_COMMS.command_text_set("ff");
          }

          button_simple_enabled(" ", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

          if (button_simple_toggle_color("LIGHTS\n(On)", "LIGHTS\n(Off)", sdSysData.Lights_On.value(), sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

          if (button_simple_color("...", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            DISPLAY_MENU = 1;
          } 
        }
        else if (DISPLAY_MENU == 1)
        {
          if (button_simple_toggle_color("HAZARD\n(On)", "HAZARD\n(Off)", sdSysData.booHazardRunning, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

          button_simple_enabled(" ", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

          if (button_simple_toggle_color("NIGHT\nMODE", "DAY\nMODE", sdSysData.Day_On_With_Override.value(), sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            SCREEN_COMMS.command_text_set("dd");
          }

          if (button_simple_toggle_color("SET\nRUNNING\nCOLOR", "SET\nRUNNING\nCOLOR", DISPLAY_RUNNING_COLOR, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
              DISPLAY_RUNNING_COLOR = !DISPLAY_RUNNING_COLOR;
          }

          if (button_simple_color("CLEAR\nANIMS", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            SCREEN_COMMS.command_text_set("``");
          }

          if (button_simple_color("SYSTEM", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            DISPLAY_MENU = 2;
          }

          if (button_simple_color("<-", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            DISPLAY_MENU = 0;
          }
        }
        else if (DISPLAY_MENU == 2)
        {
          if (button_simple_color("EXIT", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            PENDING_CONFIRM_COMMAND = "X";
            DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
          }

          button_simple_enabled(" ", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

          if (button_simple_color("SYSTEM\nSHUT\nDOWN", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            PENDING_CONFIRM_COMMAND = " comshutd";
            DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
          }

          if (button_simple_color("SYSTEM\nREBOOT", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            PENDING_CONFIRM_COMMAND = " reboot";
            DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
          }

          button_simple_enabled(" ", false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

          if (button_simple_color("DEBUG", sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
          {
            DISPLAY_DEBUG = !DISPLAY_DEBUG;
          }

          if (button_simple_color("<-", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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
      if (ImGui::Begin("Debug", &DISPLAY_DEBUG, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        ImGui::Text("%.3f ms/frame  %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
        //ImGui::Text("%.1f FPS", io.Framerate);

        if (button_simple_toggle_color("Dug", "Dbug", SCREEN_COMMS.DEBUG_STATUS.DEBUG, sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DEBUG = ! SCREEN_COMMS.DEBUG_STATUS.DEBUG;
        }

        ImGui::SameLine();

        button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(1)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

        ImGui::SameLine();

        if (button_simple_toggle_color("2", "2", SCREEN_COMMS.DEBUG_STATUS.DOOR[1], sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[1] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("4", "4", SCREEN_COMMS.DEBUG_STATUS.DOOR[3], sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[3] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[3];
        }

        ImGui::SameLine();

        button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(3)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

        //-

        if (ImGui::Button("X", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
        {
          DISPLAY_DEBUG = false;
        }

        ImGui::SameLine();

        button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(0)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

        ImGui::SameLine();

        if (button_simple_toggle_color("1", "1", SCREEN_COMMS.DEBUG_STATUS.DOOR[0], sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[0] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
        }

        ImGui::SameLine();

          ImGui::TableNextColumn();
          if (button_simple_toggle_color("3", "3", SCREEN_COMMS.DEBUG_STATUS.DOOR[2], sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            SCREEN_COMMS.DEBUG_STATUS.DOOR[2] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
          }

        ImGui::SameLine();

        button_simple_enabled(to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(2)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);
      }
      ImGui::End();
    }

    // ---------------------------------------------------------------------------------------
    // Timer Window

    if (DISPLAY_TIMER == true)
    {
      ImGui::SetNextWindowSize(ImVec2(200, 60));
      if (ImGui::Begin("Timer", &DISPLAY_TIMER, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
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
      ImGui::SetNextWindowSize(ImVec2(143, 292));
      if (ImGui::Begin("Overhead Color", &DISPLAY_DEBUG, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        if (button_simple_color("Red", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("or");
          DISPLAY_OVERHEAD_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Green", sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("og");
          DISPLAY_OVERHEAD_COLOR = false;
        }

        if (button_simple_color("Blue", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("ob");
          DISPLAY_OVERHEAD_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Purple", sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("ou");
          DISPLAY_OVERHEAD_COLOR = false;
        }
        
        if (button_simple_color("Yellow", sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("oy");
          DISPLAY_OVERHEAD_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Cyan", sdSysData.COLOR_SELECT.COLOR_COMB_CYAN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("oc");
          DISPLAY_OVERHEAD_COLOR = false;
        }
        
        if (button_simple_color("Orange", sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("on");
          DISPLAY_OVERHEAD_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("White", sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("ow");
          DISPLAY_OVERHEAD_COLOR = false;
        }
      }
      ImGui::End();
    }

    // ---------------------------------------------------------------------------------------
    // Overhead Color Window
    
    if (DISPLAY_RUNNING_COLOR == true)
    {
      ImGui::SetNextWindowSize(ImVec2(143, 292));
      if (ImGui::Begin("Running Color", &DISPLAY_DEBUG, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {
        if (button_simple_color("Red", sdSysData.COLOR_SELECT.COLOR_COMB_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rr");
          DISPLAY_RUNNING_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Green", sdSysData.COLOR_SELECT.COLOR_COMB_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rg");
          DISPLAY_RUNNING_COLOR = false;
        }

        if (button_simple_color("Blue", sdSysData.COLOR_SELECT.COLOR_COMB_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rb");
          DISPLAY_RUNNING_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Purple", sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("ru");
          DISPLAY_RUNNING_COLOR = false;
        }
        
        if (button_simple_color("Yellow", sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("ry");
          DISPLAY_RUNNING_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("Cyan", sdSysData.COLOR_SELECT.COLOR_COMB_CYAN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rc");
          DISPLAY_RUNNING_COLOR = false;
        }
        
        if (button_simple_color("Orange", sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rn");
          DISPLAY_RUNNING_COLOR = false;
        }

        ImGui::SameLine();

        if (button_simple_color("White", sdSysData.COLOR_SELECT.COLOR_COMB_WHITE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
        {
          SCREEN_COMMS.command_text_set("rw");
          DISPLAY_RUNNING_COLOR = false;
        }
      }
      ImGui::End();
    }

    // ---------------------------------------------------------------------------------------
    // Overhead Color Window
    
    if (DISPLAY_CONFIRM == true)
    {
      ImGui::SetNextWindowSize(ImVec2(90, 195));
      if (ImGui::Begin("CONFIRM", &DISPLAY_CONFIRM, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
      {

        if (button_simple_color("CONFIRM", sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set(PENDING_CONFIRM_COMMAND);
        }
        
        if (button_simple_color("DENY", sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
        {
          PENDING_CONFIRM_COMMAND = "";
          DISPLAY_CONFIRM = false;
        }

      }

      ImGui::End();
    }

    // ---------------------------------------------------------------------------------------

    {
      //style.ScrollbarSize + style.WindowPadding.y * 2.0f and fontsize for titlebar
      ImGui::SetNextWindowSize(ImVec2(QR_CODE.widtht() + style.WindowPadding.x * 2.0f, QR_CODE.height() +18 + style.WindowPadding.y * 2.0f));
      ImGui::Begin("About", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
      QR_CODE.draw();
      ImGui::End();
    }

    // ---------------------------------------------------------------------------------------

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

  }
  else
  {
    // Handle Console Iputs
    if (SCREEN_COMMS.printw_q_avail() == true)
    {
      printf("%s", SCREEN_COMMS.printw_q_get().c_str());
    }
  }

  // ---------------------------------------------------------------------------------------
  // ---------------------------------------------------------------------------------------
  // Handle Console Post Process
  //SCREEN_COMMS.command_text_set(COMMAND_TEXT);
}

void SCREEN4::shutdown(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

void SCREEN4::update_daemon_log(system_data &sdSysData, string Text)
{
  if (sdSysData.TTY_ONLY == false)
  {
    DAEMON_LOG.add_line(Text);
  }
}

void SCREEN4::update_automobile_gadgets(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    if(sdSysData.CAR_INFO.CHANGED == true)
    {
      AUTOMOBILE.update(sdSysData);
    }
  }
}

void SCREEN4::update_ADS_B_gadgets(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    if(sdSysData.AIRCRAFT_COORD.DATA.CHANGED == true)
    {
      ADSB.update(sdSysData);
    }
  }
}

// ---------------------------------------------------------------------------------------
#endif