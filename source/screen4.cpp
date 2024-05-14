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

void SCREEN4::door_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size)
{
  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(0).value)
  {
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, Window_Size.y / 2.0f), ImVec2(Window_Size.x / 2.0f, Window_Size.y), 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD, 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_white().STANDARD_V);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value)
  {
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), ImVec2(Window_Size.x / 2.0f, Window_Size.y / 2.0f), 
                    sdSysData.COLOR_SELECT.c_white().STANDARD_V, sdSysData.COLOR_SELECT.c_black().STANDARD, 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value)
  {
    Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x / 2.0f, Window_Size.y / 2.0f), ImVec2(Window_Size.x, Window_Size.y), 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD, 
                    sdSysData.COLOR_SELECT.c_white().STANDARD_V, sdSysData.COLOR_SELECT.c_black().STANDARD);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value)
  {
    Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x / 2.0f, 0.0f), ImVec2(Window_Size.x, Window_Size.y / 2.0f), 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_white().STANDARD_V, 
                    sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD);
  }
}

void SCREEN4::hazard_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size)
{
  if ((sdSysData.CAR_INFO.active() && sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards()) || sdSysData.booHazardRunning)
  {
    if (PING_BLINKER.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
    {
      BLINKER_BLINK = !BLINKER_BLINK;
      PING_BLINKER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), (790 / 2));
    }

    if (BLINKER_BLINK)
    {
      Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), ImVec2(Window_Size.x / 2.0f, Window_Size.y), 
                          sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_red().STANDARD, 
                          sdSysData.COLOR_SELECT.c_red().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD);

      Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x / 2.0f, 0.0f), ImVec2(Window_Size.x, Window_Size.y), 
                          sdSysData.COLOR_SELECT.c_red().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD, 
                          sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_red().STANDARD);
    }
  }
}

void SCREEN4::signal_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size)
{
  if (sdSysData.CAR_INFO.active() || sdSysData.booHazardRunning)
  {
    if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left())
    {
      if (PING_BLINKER.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
      {
        BLINKER_BLINK = !BLINKER_BLINK;
        PING_BLINKER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), (790 / 2));
      }

      if (BLINKER_BLINK)
      {
        Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), ImVec2(Window_Size.x / 2.0f, Window_Size.y), 
                      sdSysData.COLOR_SELECT.c_yellow().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD, 
                      sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_yellow().STANDARD);
      }
    }
    else if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right())
    {
      if (PING_BLINKER.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
      {
        BLINKER_BLINK = !BLINKER_BLINK;
        PING_BLINKER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), (790 / 2));
      }

      if (BLINKER_BLINK)
      {
        Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x / 2.0f, 0.0f), ImVec2(Window_Size.x, Window_Size.y), 
                      sdSysData.COLOR_SELECT.c_black().STANDARD, sdSysData.COLOR_SELECT.c_yellow().STANDARD, 
                      sdSysData.COLOR_SELECT.c_yellow().STANDARD, sdSysData.COLOR_SELECT.c_black().STANDARD);
      }
    }
    //else
    //{
    //  BLINKER_BLINK = true;
    //}
  }
}

void SCREEN4::set_screen_default_colors(system_data &sdSysData)
{
  sdSysData.COLOR_SELECT.init(1.0f);
  
  ImGuiStyle& style = ImGui::GetStyle();

  // Standard Default Colors on startup
  ImColor background = sdSysData.COLOR_SELECT.c_blue().BACKGROUND;
  ImColor dim = sdSysData.COLOR_SELECT.c_blue().DIM;
  ImColor standard = sdSysData.COLOR_SELECT.c_blue().STANDARD;
  ImColor hovered = sdSysData.COLOR_SELECT.c_blue().HOVERED;
  ImColor active = sdSysData.COLOR_SELECT.c_blue().ACTIVE;

  //style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  style.Colors[ImGuiCol_Text] = sdSysData.COLOR_SELECT.c_white().TEXT;

  style.Colors[ImGuiCol_TitleBg] = standard;
  style.Colors[ImGuiCol_TitleBgActive] = active;
  style.Colors[ImGuiCol_TitleBgCollapsed] = background;

  style.Colors[ImGuiCol_FrameBg] = background;
  style.Colors[ImGuiCol_FrameBgHovered] = hovered;
  style.Colors[ImGuiCol_FrameBgActive] = active;

  style.Colors[ImGuiCol_Border] = background;
  style.Colors[ImGuiCol_BorderShadow] = sdSysData.COLOR_SELECT.c_yellow().BACKGROUND;

  style.Colors[ImGuiCol_CheckMark] = standard;

  style.Colors[ImGuiCol_SliderGrab] = active;
  style.Colors[ImGuiCol_SliderGrabActive] = active;

  style.Colors[ImGuiCol_Button] = standard;
  style.Colors[ImGuiCol_ButtonHovered] = hovered;
  style.Colors[ImGuiCol_ButtonActive] = active;

  style.Colors[ImGuiCol_Header] = standard;
  style.Colors[ImGuiCol_HeaderActive] = active;

  style.Colors[ImGuiCol_TableHeaderBg] = background;
  style.Colors[ImGuiCol_TableBorderLight] = dim;
  style.Colors[ImGuiCol_TableBorderStrong] = standard;
  style.Colors[ImGuiCol_TableRowBg] = dim;
  style.Colors[ImGuiCol_TableRowBgAlt] = standard;

  style.Colors[ImGuiCol_Separator] = sdSysData.COLOR_SELECT.c_white().STANDARD;
  style.Colors[ImGuiCol_SeparatorHovered] = sdSysData.COLOR_SELECT.c_white().HOVERED;
  style.Colors[ImGuiCol_SeparatorActive] = sdSysData.COLOR_SELECT.c_white().ACTIVE;

  style.Colors[ImGuiCol_ResizeGrip] = standard;
  style.Colors[ImGuiCol_ResizeGripHovered] = hovered;
  style.Colors[ImGuiCol_ResizeGripActive] = active;

  //style.Colors[ImGuiCol_ScrollbarBg] = active;
  style.Colors[ImGuiCol_ScrollbarGrab] = dim;
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = hovered;
  style.Colors[ImGuiCol_ScrollbarGrabActive] = active;

  style.Colors[ImGuiCol_Tab] = standard;
  style.Colors[ImGuiCol_TabHovered] = hovered;
  style.Colors[ImGuiCol_TabActive] = active;
  style.Colors[ImGuiCol_TabUnfocused] = background;
  style.Colors[ImGuiCol_TabUnfocusedActive] = active;

  style.Colors[ImGuiCol_TextSelectedBg] = background;

  style.Colors[ImGuiCol_NavHighlight] = standard;
}

int SCREEN4::create(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    // Set colors
    sdSysData.COLOR_SELECT.init(1.0f);

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
    //glfwSwapInterval(0); // No Cap Frame Rate

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls //lols

    // For keyboard input redirect
    io.WantCaptureKeyboard = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // Adjust Styles
    ImGuiStyle& style = ImGui::GetStyle();

    set_screen_default_colors(sdSysData);

    style.WindowRounding = 8.0f;
    style.FrameRounding = 8.0f;
    style.ChildRounding = 8.0f;

    style.ScrollbarSize = 20.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 57.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 28.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 100.0f);

    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 57.0f);
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 28.0f);
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 100.0f);
    // Will track as "io.Fonts->Fonts.Data[1]" manually, for now.

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Generate an empty from on create so Fonts, and whatever else is needed, can load.
    //  Necessary for ImGui::CalcTextSize(hzxjhfzxjhxz)
    ImGui::EndFrame();

    // QR_Image
    QR_CODE.create(sdSysData.FILE_NAMES.QR_CODE_FILE, 2.0f);
    REFERENCE_CARD.create(sdSysData.FILE_NAMES.REFERENCE_CARD_FILE, 1.0f);

    // Prepare Screens
    AUTOMOBILE.create(sdSysData);
    ADSB.create(sdSysData);

    // Prepare Current Screen Widgets
    LIGHTS.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    LIGHTS.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    LIGHTS.update_text("LED ILLUM", "LED ILLUM");

    DEBUG.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.red();
    DEBUG.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.white();
    DEBUG.update_text("  DEBUG  ", "         ");

    OVERHEAD.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    OVERHEAD.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    OVERHEAD.update_text(" OVERHEAD ", " OVERHEAD ");

    HAZARD.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.red();
    HAZARD.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    HAZARD.update_text("  HAZARD  ", "  HAZARD  ");

    DAY_NIGHT.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    DAY_NIGHT.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    DAY_NIGHT.update_text("  DAY  ", " NIGHT ");

    TIMER.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    TIMER.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    TIMER.update_text(" TIMER ", " TIMER ");

    GPS.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    GPS.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    GPS.update_text("  GPS  ", "  GPS  ");

    COMPASS.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    COMPASS.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    COMPASS.update_text("COMPASS", "COMPASS");

    /*

    DOOR2.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    DOOR2.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    DOOR2.update_text(" DOOR2", " DOOR2");

    DOOR3.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    DOOR3.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    DOOR3.update_text("DOOR3 ", "DOOR3 ");

    DOOR4.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    DOOR4.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    DOOR4.update_text("DOOR4 ", "DOOR4 ");
    */

    AUTO.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    AUTO.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    AUTO.update_text(" AUTO  ", " AUTO  ");

    ADSB_IND.PROPS.COLOR_TRUE = sdSysData.COLOR_SELECT.white();
    ADSB_IND.PROPS.COLOR_FALSE = sdSysData.COLOR_SELECT.blue();
    ADSB_IND.update_text(" ADS-B ", " ADS-B ");

    string version = "v: ";
    version = version + Revision;
    VERSION.update_text(sdSysData, version);
    VERSION.PROPS.COLOR = sdSysData.COLOR_SELECT.white();

    TEMP.update_text(sdSysData, "Temp: NA");
    TEMP.PROPS.COLOR = sdSysData.COLOR_SELECT.white();

    {
      BAR_TIMER.PROPS.LABEL = "Timer";
      BAR_TIMER.PROPS.BAR_HEIGHT = 20.0f;
      BAR_TIMER.PROPS.MARKER_SIZE = 15.0f;
      BAR_TIMER.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      BAR_TIMER.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      BAR_TIMER.PROPS.DRAW_MIN_MAX_ON_TOP = false;
      BAR_TIMER.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      BAR_TIMER.PROPS.DRAW_MIN_MAX = false;
      BAR_TIMER.PROPS.MAX = 1.0f;
      BAR_TIMER.PROPS.DRAW_RULER = true;
      BAR_TIMER.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      BAR_TIMER.PROPS.MAX_TICK_LEVEL = 3;
      BAR_TIMER.create();
    }

    // Load Advertisements
    ADVERTISEMENTS.create(sdSysData);

    // Prepare Compass
    COMPASS_WINDOW_COMPASS.set_size(32, (15 / 2));
    // set at frame rate for slow and jitter size for fast.
  }
  else
  {
    // TTY
    struct termios terminal;

    tcgetattr(0, &terminal);

    terminal.c_lflag &= ~ICANON;  // set conotical mod.
    terminal.c_lflag &= ~ECHO;    // turn echo off.

    tcsetattr(0, TCSANOW, &terminal);

    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &TERMINAL_WINDOW);
  }

  return 0;
}

// ---------------------------------------------------------------------------------------

void SCREEN4::character_enter(unsigned char Character, 
                bool Shift_Pressed, bool Backspace_Pressed, bool Enter_Pressed)
{
    // Letters
    if (Character >= 65 && Character <= 90)
    {
      // All Upper Case Letters, but with shift not pressed to lower the case.
      if (Shift_Pressed)
      {
        COMMAND_TEXT += Character;
        SCREEN_COMMS.command_text_set(COMMAND_TEXT);
      }
      else
      {
        COMMAND_TEXT += Character + 32;
        SCREEN_COMMS.command_text_set(COMMAND_TEXT);
      }
    }

    // Symbols
    else if ((Character >= 32 && Character <= 64) || 
              (Character >= 91 && Character <= 96) || 
              (Character >= 123 && Character <= 126))
    {
      COMMAND_TEXT += Character;
      SCREEN_COMMS.command_text_set(COMMAND_TEXT);
    }

    // Backspace 
    else if (Backspace_Pressed)
    {
      if (COMMAND_TEXT.size() > 0)
      {
        COMMAND_TEXT.resize(COMMAND_TEXT.size() -1);
      }
    }

    // CR
    else if (Enter_Pressed)
    {
      SCREEN_COMMS.carrage_return_set();
    }
}

// ---------------------------------------------------------------------------------------

void SCREEN4::draw(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    //Check for changes in style
    if (CHANGED)
    {
      set_screen_default_colors(sdSysData);
      CHANGED = false;
    }

    // Handle Console Iputs
    if (SCREEN_COMMS.printw_q_avail() == true)
    {
      CONSOLE.add_line(SCREEN_COMMS.printw_q_get());
    }
    
    // Clear
    if (SCREEN_COMMS.command_text_clear_get() == true)
    {
      COMMAND_TEXT = "";
    }

    DISPLAY_TIMER = sdSysData.cdTIMER.is_active();

    // ---------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------
    // Handle Console Outputs
    ImGuiIO &io = ImGui::GetIO();
    //ImGuiStyle& style = ImGui::GetStyle();

    // reduce call backs.
    unsigned long current_frame_time = sdSysData.PROGRAM_TIME.current_frame_time();
    
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
          // Check for backspace
          {
            character_pressed = i;
            backspace_pressed = true;
          }
          if ( i == 257 || i == ImGuiKey_Enter || i == ImGuiKey_KeypadEnter)
          // Check for enter key
          {
            character_pressed = i;
            enter_pressed = true;
          }
          if (i == 340 || i == ImGuiKey_ModShift || i == ImGuiKey_LeftShift || i == ImGuiKey_RightShift)
          // Check for shift key and characters
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

        character_enter(character_pressed, shift_pressed, backspace_pressed, enter_pressed);

      }
      else if (character_pressed == 0)
      {
        PREV_FRAME_KEY_DOWN = 0;
      }
    }

    // ---------------------------------------------------------------------------------------

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (sdSysData.DNFWTS.active_update(current_frame_time))
    {
      if (ImGui::Begin("Window", &show_test_window, sdSysData.SCREEN_DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      {
        ImVec4 working_area = get_working_area();

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 line_start_1;
        ImVec2 line_end_1;
        ImVec2 line_start_2;
        ImVec2 line_end_2;

        line_start_1 = ImVec2(working_area.x + 50.0f, working_area.y + 50.0f);
        line_end_1 = ImVec2(working_area.x + working_area.z - 50.0f, working_area.y + working_area.w - 50.0f);
        
        line_start_2 = ImVec2(working_area.x + working_area.z - 50.0f, working_area.y + 50.0f);
        line_end_2 = ImVec2(working_area.x + 50.0f, working_area.y + working_area.w - 50.0f);

        draw_list->AddLine(line_start_1, line_end_1, sdSysData.COLOR_SELECT.c_red().STANDARD, 50.0f);
        draw_list->AddLine(line_start_2, line_end_2, sdSysData.COLOR_SELECT.c_red().STANDARD, 50.0f);

        ImGui::PushFont(io.Fonts->Fonts.Data[2]);
        ImGui::SetCursorScreenPos(ImVec2(working_area.x + 50.0f, working_area.y + 150.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
        ImGui::Text("DO \nNOT FUCK \nWITH THE SYSTEM");
        ImGui::PopStyleColor();
        ImGui::PopFont();

        ImGui::End();
      }
    }
    else
    {
      if (ImGui::Begin("Window", &show_test_window, sdSysData.SCREEN_DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      {
        // draw list
        ImDrawList* draw_list_window_background = ImGui::GetWindowDrawList();

        ImGui::BeginChild("Main", ImVec2(ImGui::GetContentRegionAvail().x - 85.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          // ---------------------------------------------------------------------------------------
          // Status Sub Window

          //ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
          ImGui::BeginChild("Status", ImVec2(ImGui::GetContentRegionAvail().x, 60.0f), true, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            float region_div_4 = ImGui::GetContentRegionAvail().x / 4.0f;

            // Divide sub window into 4
            // Left
            ImGui::BeginChild("Status Left", ImVec2(region_div_4, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              ImVec4 working_area_command = get_working_area();

              // Assign Draw List
              ImDrawList* draw_list_status_left = ImGui::GetWindowDrawList();

              if (RESET_KEYBOARD_FOCUS == true)
              {
                ImGui::SetKeyboardFocusHere();
                RESET_KEYBOARD_FOCUS = false;
              }

              ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.c_white().TEXT));
              ImGui::PushFont(io.Fonts->Fonts.Data[2]);
              ImGui::Text("CMD:");

              ImGui::SameLine();

              ImGui::Text(COMMAND_TEXT.c_str());
              simple_wrap_text_box(draw_list_status_left, sdSysData);

              ImGui::PopFont();
              ImGui::PopStyleColor();

              if (button_area(working_area_command))
              {
                ADVERTISEMENTS.play(sdSysData);
              }
            }
            ImGui::EndChild();

            // Mid
            ImGui::SameLine();

            ImGui::BeginChild("Status Mid", ImVec2((region_div_4 * 2.0f) - 45.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              ImVec4 working_area = get_working_area();

              // Display Lights Off mode toggle.
              ImGui::BeginGroup();
              {
                LIGHTS.update_tf(sdSysData.Lights_On.value());
                LIGHTS.draw(sdSysData);

                DEBUG.update_tf(SCREEN_COMMS.DEBUG_STATUS.DEBUG);
                DEBUG.draw(sdSysData);
              }
              ImGui::EndGroup();
              
              ImGui::SameLine();

              // Status Group
              ImGui::BeginGroup();
              {
                OVERHEAD.update_tf(sdSysData.booOverheadRunning);
                OVERHEAD.draw(sdSysData);

                HAZARD.update_tf(sdSysData.booHazardRunning);
                HAZARD.draw(sdSysData);
              }
              ImGui::EndGroup();

              ImGui::SameLine();

              // Status Group
              ImGui::BeginGroup();
              {
                // Lights
                if (DAY_NIGHT.update_tf(sdSysData.Day_On_With_Override.value()))
                {
                  //set_screen_default_colors(sdSysData);
                }
                DAY_NIGHT.draw(sdSysData);

                // Timer
                TIMER.update_tf(sdSysData.cdTIMER.is_active());
                TIMER.draw(sdSysData);
              }
              ImGui::EndGroup();

              ImGui::SameLine();
              
              // Group
              ImGui::BeginGroup();
              {
                GPS.update_tf(sdSysData.GPS_SYSTEM.active(current_frame_time));
                GPS.draw(sdSysData);

                COMPASS.update_tf(sdSysData.COMMS_COMPASS.connected());
                COMPASS.draw(sdSysData);
              }
              ImGui::EndGroup();

              ImGui::SameLine();
              
              // Auto ADSB
              ImGui::BeginGroup();
              {
                AUTO.update_tf(sdSysData.CAR_INFO.active());
                AUTO.draw(sdSysData);

                ADSB_IND.update_tf(sdSysData.AIRCRAFT_COORD.is_active());
                ADSB_IND.draw(sdSysData);
              }
              ImGui::EndGroup();

              // Change Screens
              if (button_area(working_area))
              {
                sdSysData.DNFWTS.turn_on(current_frame_time);
              }
              
            }
            ImGui::EndChild();

            // Mid Right
            ImGui::SameLine();

            ImGui::BeginChild("Status Right", ImVec2(ImGui::GetContentRegionAvail().x - 90.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              ImVec2 start_position = ImGui::GetCursorScreenPos();
              VERSION.draw(sdSysData);

              if (sdSysData.hsHardware_Status.enabled() == true)
              {
                TEMP.update_text(sdSysData, ("Temp: " + to_string((int)sdSysData.hsHardware_Status.get_temperature()) + "c").c_str());
                TEMP.draw(sdSysData);
              }

              ImGui::SetCursorScreenPos(start_position);
              if (ImGui::InvisibleButton("CORPO VOID", ImGui::GetContentRegionAvail()))
              {
                sdSysData.COLOR_SELECT.toggle_void_color(current_frame_time);
                CHANGED = true;
              }
            }
            ImGui::EndChild();

            // Mid Right
            ImGui::SameLine();

            ImGui::BeginChild("Status Compass", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              // Assign Draw List
              ImDrawList* draw_list_status_compass = ImGui::GetWindowDrawList();

              if (sdSysData.GPS_SYSTEM.active(current_frame_time) ||
                  sdSysData.COMMS_COMPASS.connected())
              {
                ImVec4 working_area = get_working_area();

                // Draw North Direction Compass
                draw_compass(draw_list_status_compass, sdSysData, 1, ImVec2((working_area.x + working_area.z / 2.0f),(working_area.y + working_area.w / 2.0f)), 
                              (working_area.z / 6.0f), false, sdSysData.GPS_SYSTEM.current_position().VALID_GPS_FIX, 
                              sdSysData.GPS_SYSTEM.current_position().VALID_TRACK, (sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING), 
                              sdSysData.COMMS_COMPASS.connected(), (sdSysData.COMMS_COMPASS.bearing()), true);

                // Change Screens
                if (button_area(working_area))
                {
                  DISPLAY_COMPASS_WINDOW = !DISPLAY_COMPASS_WINDOW;
                }
              }
            }
            ImGui::EndChild();
          }
          ImGui::EndChild();

          // ---------------------------------------------------------------------------------------
          // Console Sub Window

          ImGui::BeginChild("DISPLAY_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 48), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (DISPLAY_SCREEN == 0)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("CONSOLE_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                  
                  CONSOLE.display(sdSysData, "Console", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
                    
                }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::BeginChild("Automobile Sidebar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  AUTOMOBILE.display_sidebar(sdSysData, false, RESTACK_WINDOWS);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
              }
              else
              {
                ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                CONSOLE.display(sdSysData, "Console", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              }
            }

            else if (DISPLAY_SCREEN == 1)
            {
              ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
              AUTOMOBILE.display(sdSysData, SCREEN_COMMS, DISPLAY_CONFIRM);
              ImGui::PopStyleColor();
              
              ImGui::SameLine();

              ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
              ImGui::BeginChild("Automobile Sidebar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
              {
                AUTOMOBILE.display_sidebar(sdSysData, false, RESTACK_WINDOWS);
              }
              ImGui::EndChild();
              ImGui::PopStyleColor();
            }

            else if (DISPLAY_SCREEN == 2)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("ADSB_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                  ADSB.display(sdSysData, SCREEN_COMMS);
                  ImGui::PopStyleColor();
                }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::BeginChild("Automobile Sidebar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  AUTOMOBILE.display_sidebar(sdSysData, false, RESTACK_WINDOWS);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
              }
              else
              {
                ADSB.display(sdSysData, SCREEN_COMMS);
              }
            }

            /*
            else if (DISPLAY_SCREEN == 3)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("GPS_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                  GPS_CONSOLE.display(sdSysData, "GPS Logs", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::BeginChild("Automobile Sidebar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  AUTOMOBILE.display_sidebar(sdSysData, false, RESTACK_WINDOWS);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
              }
              else
              {
                ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                GPS_CONSOLE.display(sdSysData, "GPS Logs", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              }
            }
            */

            else if (DISPLAY_SCREEN == 4)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("LOGS_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

                  DAEMON_LOG.display(sdSysData, "System Logs", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
                  
                }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::BeginChild("Automobile Sidebar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  AUTOMOBILE.display_sidebar(sdSysData, false, RESTACK_WINDOWS);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
              }
              else
              {
                ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                DAEMON_LOG.display(sdSysData, "System Logs", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              }
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

          // Restack windows placed here to complete cycle.
          RESTACK_WINDOWS = false;

          ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x - 215.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {

            if (button_simple_toggle_color(sdSysData, "Console", "Console", DISPLAY_SCREEN == 0, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 0;
              RESTACK_WINDOWS = true;
            }

            ImGui::SameLine();

            if (button_simple_toggle_color(sdSysData, "Automobile", "Automobile", DISPLAY_SCREEN == 1, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 1;
            }

            ImGui::SameLine();

            if (button_simple_toggle_color(sdSysData, "ADSB", "ADSB", DISPLAY_SCREEN == 2, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 2;
            }

            ImGui::SameLine();

            /*
            if (button_simple_toggle_color(sdSysData, "GPS", "GPS", DISPLAY_SCREEN == 3, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 3;
            }

            ImGui::SameLine();
            */

            if (button_simple_toggle_color(sdSysData, "LOGS", "LOGS", DISPLAY_SCREEN == 4, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 4;
              RESTACK_WINDOWS = true;
            }
          }
          ImGui::EndChild();

          if (sdSysData.ALERTS_2.alert_count() > 0)
          {
            ImGui::SameLine();

            if (button_simple_color(sdSysData, "DISPLAY\nALERTS", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.ALERTS_2.display_active_alerts();
            }
          }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // ---------------------------------------------------------------------------------------
        // Menus Sub Window

        ImGui::BeginChild("System Menu", ImVec2(0.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          // Main Menu
          if (DISPLAY_MENU == 0)
          {
            if (button_simple_toggle_color(sdSysData, "STOP\nTIMER", "START\nTIMER", sdSysData.cdTIMER.is_active(), sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

            ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (button_simple_toggle_color(sdSysData, "OVER\nHEAD\nLIGHTS", "OVER\nHEAD\nLIGHTS", sdSysData.booOverheadRunning, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

            if (button_simple_color(sdSysData, "FLASH", sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_text_set("ff");
            }

            if (button_simple_color(sdSysData, "CARDS", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_CARDS_WINDOW = !DISPLAY_CARDS_WINDOW;
            } 

            if (button_simple_toggle_color(sdSysData, "LIGHTS\n(On)", "LIGHTS\n(Off)", sdSysData.Lights_On.value(), sdSysData.COLOR_SELECT.red(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

            if (button_simple_color(sdSysData, "...", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 1;
            } 
          }
          else if (DISPLAY_MENU == 1)
          {
            if (button_simple_toggle_color(sdSysData, "HAZARD\n(On)", "HAZARD\n(Off)", sdSysData.booHazardRunning, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

            ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (button_simple_toggle_color(sdSysData, "NIGHT\nMODE", "DAY\nMODE", sdSysData.Day_On_With_Override.value(), sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_text_set("dd");
            }

            if (button_simple_toggle_color(sdSysData, "SET\nRUNNING\nCOLOR", "SET\nRUNNING\nCOLOR", DISPLAY_RUNNING_COLOR, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.yellow(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
                DISPLAY_RUNNING_COLOR = !DISPLAY_RUNNING_COLOR;
            }

            if (button_simple_color(sdSysData, "CLEAR\nANIMS", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_text_set("``");
            }

            if (button_simple_color(sdSysData, "SYSTEM", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 2;
            }

            if (button_simple_color(sdSysData, "<-", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 0;
            }
          }
          else if (DISPLAY_MENU == 2)
          {
            if (button_simple_color(sdSysData, "EXIT", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_pending_set("X");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (button_simple_color(sdSysData, "SYSTEM\nSHUT\nDOWN", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_pending_set(" shutdown");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            if (button_simple_color(sdSysData, "SYSTEM\nREBOOT", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              SCREEN_COMMS.command_pending_set(" reboot");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            ImGui::InvisibleButton("noshow5", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (button_simple_color(sdSysData, "DEBUG", sdSysData.COLOR_SELECT.yellow(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_DEBUG = !DISPLAY_DEBUG;
            }

            if (button_simple_color(sdSysData, "<-", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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

        // Auto Signal Lights
        door_lights(draw_list_window_background, sdSysData, viewport->Size);
        signal_lights(draw_list_window_background, sdSysData, viewport->Size);
        hazard_lights(draw_list_window_background, sdSysData, viewport->Size);
        
        ImGui::End();
      }
      else
      {
        SCREEN_COMMS.WINDOW_CLOSE = true;
      }


      // ---------------------------------------------------------------------------------------
      // Debug Window
      
      if (DISPLAY_DEBUG == true)
      {
        ImGui::SetNextWindowSize(ImVec2(266, 266));
        if (ImGui::Begin("Debug", &DISPLAY_DEBUG, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          ImGui::Text("%.3f ms/frame  %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
          //ImGui::Text("%.1f FPS", io.Framerate);

          if (button_simple_toggle_color(sdSysData, "Dug", "Dbug", SCREEN_COMMS.DEBUG_STATUS.DEBUG, sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            SCREEN_COMMS.DEBUG_STATUS.DEBUG = ! SCREEN_COMMS.DEBUG_STATUS.DEBUG;
          }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(1)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          ImGui::SameLine();

          if (button_simple_toggle_color(sdSysData, "2", "2", SCREEN_COMMS.DEBUG_STATUS.DOOR[1], sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            SCREEN_COMMS.DEBUG_STATUS.DOOR[1] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
          }

          ImGui::SameLine();

          if (button_simple_toggle_color(sdSysData, "4", "4", SCREEN_COMMS.DEBUG_STATUS.DOOR[3], sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            SCREEN_COMMS.DEBUG_STATUS.DOOR[3] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[3];
          }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(3)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          //-

          if (ImGui::Button("X", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            DISPLAY_DEBUG = false;
          }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(0)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          ImGui::SameLine();

          if (button_simple_toggle_color(sdSysData, "1", "1", SCREEN_COMMS.DEBUG_STATUS.DOOR[0], sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            SCREEN_COMMS.DEBUG_STATUS.DOOR[0] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
          }

          ImGui::SameLine();

            ImGui::TableNextColumn();
            if (button_simple_toggle_color(sdSysData, "3", "3", SCREEN_COMMS.DEBUG_STATUS.DOOR[2], sdSysData.COLOR_SELECT.white(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
            {
              SCREEN_COMMS.DEBUG_STATUS.DOOR[2] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
            }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(2)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          // Rasfled Stats
          ImGui::Text("   Compute Time: %6.2lf ms", sdSysData.dblCOMPUTETIME.get_data());
          ImGui::Text("     Cycle Time: %6.2lf ms", sdSysData.dblCYCLETIME.get_data());
          ImGui::Text("Prev Sleep Time: %6.2lf ms", sdSysData.dblPREVSLEEPTIME.get_data());
          ImGui::Text("     Comms Time: %6.2lf ms", sdSysData.dblCOMMS_TRANSFER_TIME.get_data());
          ImGui::Text("    Screen Time: %6.2lf ms", sdSysData.dblSCREEN_RENDER_TIME.get_data());
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Timer Window

      if (DISPLAY_TIMER == true && RESTACK_WINDOWS == false)
      {
        ImGui::SetNextWindowSize(ImVec2(250, 90));
        if (ImGui::Begin("Timer", &DISPLAY_TIMER, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          ImDrawList* draw_list_timer = ImGui::GetWindowDrawList();

          long elaped_time = 0;
          float duration_time = 0.0f;
          float remaining_time = 0.0f;

          // Calculate
          duration_time = (float)sdSysData.cdTIMER.duration();
          elaped_time = sdSysData.cdTIMER.elapsed_time(current_frame_time);
          remaining_time = (float)duration_time - (float)elaped_time;


          string timer_dsp =  linemerge_right_justify(2, "00", to_string(millis_to_time_minutes(remaining_time))) + ":" + 
                              linemerge_right_justify(2, "00", to_string(millis_to_time_seconds(remaining_time)));

          // Display Timer

          ImGui::Text("Remaining Time: %s", timer_dsp.c_str());
          BAR_TIMER.update_value(sdSysData, remaining_time / duration_time);
          BAR_TIMER.draw(draw_list_timer, sdSysData);
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Overhead Color Window
      
      if (DISPLAY_OVERHEAD_COLOR == true && RESTACK_WINDOWS == false)
      {
        ImGui::SetNextWindowSize(ImVec2(143, 292));
        if (ImGui::Begin("Overhead Color", &DISPLAY_OVERHEAD_COLOR, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          if (button_simple_color(sdSysData, "Red", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("or");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Green", sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("og");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          if (button_simple_color(sdSysData, "Blue", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("ob");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Purple", sdSysData.COLOR_SELECT.purple(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("ou");
            DISPLAY_OVERHEAD_COLOR = false;
          }
          
          if (button_simple_color(sdSysData, "Yellow", sdSysData.COLOR_SELECT.yellow(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("oy");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Cyan", sdSysData.COLOR_SELECT.cyan(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("oc");
            DISPLAY_OVERHEAD_COLOR = false;
          }
          
          if (button_simple_color(sdSysData, "Orange", sdSysData.COLOR_SELECT.orange(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("on");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "White", sdSysData.COLOR_SELECT.white(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("ow");
            DISPLAY_OVERHEAD_COLOR = false;
          }
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Overhead Color Window
      
      if (DISPLAY_RUNNING_COLOR == true && RESTACK_WINDOWS == false)
      {
        ImGui::SetNextWindowSize(ImVec2(143, 292));
        if (ImGui::Begin("Running Color", &DISPLAY_RUNNING_COLOR, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          if (button_simple_color(sdSysData, "Red", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rr");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Green", sdSysData.COLOR_SELECT.green(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rg");
            DISPLAY_RUNNING_COLOR = false;
          }

          if (button_simple_color(sdSysData, "Blue", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rb");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Purple", sdSysData.COLOR_SELECT.purple(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("ru");
            DISPLAY_RUNNING_COLOR = false;
          }
          
          if (button_simple_color(sdSysData, "Yellow", sdSysData.COLOR_SELECT.yellow(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("ry");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "Cyan", sdSysData.COLOR_SELECT.cyan(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rc");
            DISPLAY_RUNNING_COLOR = false;
          }
          
          if (button_simple_color(sdSysData, "Orange", sdSysData.COLOR_SELECT.orange(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rn");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (button_simple_color(sdSysData, "White", sdSysData.COLOR_SELECT.white(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            SCREEN_COMMS.command_text_set("rw");
            DISPLAY_RUNNING_COLOR = false;
          }
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Overhead Color Window
      
      if (DISPLAY_CONFIRM == true && RESTACK_WINDOWS == false)
      {
        bool choice = false;

        if (confirm_dialog(sdSysData, choice))
        {
          if (choice)
          {
            SCREEN_COMMS.command_pending_send();
            DISPLAY_CONFIRM = false;
          }
          else
          {
            DISPLAY_CONFIRM = false;
          }
        }
      }

      // ---------------------------------------------------------------------------------------
      // Alert Windows

      if (RESTACK_WINDOWS == false)
      {
        // Go through and display all reserve
        for (int alert_num = 0; alert_num < RESERVE_ALERT_LIST_SIZE; alert_num++)
        {
          //if (sdSysData.ALERTS_2.ALERTS_RESERVE[alert_num].active())
          //{
            if (sdSysData.ALERTS_2.res_display(alert_num))
            {
              ImGui::SetNextWindowSize(ImVec2(300, 100));

              if (sdSysData.ALERTS_2.res_warning(alert_num))
              {
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
              }
              else if(sdSysData.ALERTS_2.res_active(alert_num))
              {
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
              }
              else
              {
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_green().STANDARD));
              }
              
              if (ImGui::Begin(("ALERT " + to_string(alert_num)).c_str(), nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
              {
                ImVec2 screen_pos = ImGui::GetCursorScreenPos();
                ImGui::Text(sdSysData.ALERTS_2.res_alert_text_line_1(alert_num).c_str());
                ImGui::Text(sdSysData.ALERTS_2.res_alert_text_line_2(alert_num).c_str());

                ImGui::SetCursorScreenPos(screen_pos);
                if (ImGui::InvisibleButton(("Acknowlege Alert" + to_string(alert_num)).c_str(), ImGui::GetContentRegionAvail()))
                {
                  sdSysData.ALERTS_2.res_acknowlege(alert_num);
                }
              }
              ImGui::End();

              ImGui::PopStyleColor(3);
            }
          //}
        }
      
        // Go through and display all generic alerts
        if (sdSysData.ALERTS_2.gen_size() > 0)
        {
          for (int alert_num = 0; alert_num < (int)sdSysData.ALERTS_2.gen_size(); alert_num++)
          {
            ImGui::SetNextWindowSize(ImVec2(300, 100));
            
            if (sdSysData.ALERTS_2.gen_display(alert_num))
            {
              if (sdSysData.ALERTS_2.gen_warning(alert_num))
              {
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_yellow().DIM));
              }
              else
              {
                ImGui::PushStyleColor(ImGuiCol_TitleBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(sdSysData.COLOR_SELECT.c_red().STANDARD));
              }
              
              if (ImGui::Begin(("ALERT " + to_string(sdSysData.ALERTS_2.gen_alert_id(alert_num))).c_str(), nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
              {
                ImVec2 screen_pos = ImGui::GetCursorScreenPos();
                ImGui::Text(sdSysData.ALERTS_2.gen_alert_text_line_1(alert_num).c_str());
                ImGui::Text(sdSysData.ALERTS_2.res_alert_text_line_2(alert_num).c_str());

                ImGui::SetCursorScreenPos(screen_pos);
                if (ImGui::InvisibleButton(("Acknowlege Alert" + to_string(alert_num)).c_str(), ImGui::GetContentRegionAvail()))
                {
                  sdSysData.ALERTS_2.gen_acknowlege(alert_num);
                }
              }
              ImGui::End();

              ImGui::PopStyleColor(3);
            }
          }
        }
      }

      // ---------------------------------------------------------------------------------------

      if (DISPLAY_QR_CODE == true)
      {
        ImGui::SetNextWindowSize(QR_CODE.get_should_be_window_size());
        ImGui::Begin("About", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
        {
          ImVec4 working_area = get_working_area();
          QR_CODE.draw();
            
          if (button_area(working_area))
          {
            DISPLAY_QR_CODE = false;
          }

          ImGui::End();
        }
      }

      // ---------------------------------------------------------------------------------------

      if (DISPLAY_REFERENCE_CARD == true)
      {
        ImGui::SetNextWindowSize(REFERENCE_CARD.get_should_be_window_size());
        ImGui::Begin("Reference Card", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
        {
          ImVec4 working_area = get_working_area();
          REFERENCE_CARD.draw();
            
          if (button_area(working_area))
          {
            DISPLAY_REFERENCE_CARD = false;
          }

          ImGui::End();
        }
      }

      // ---------------------------------------------------------------------------------------
      
      if (DISPLAY_COMPASS_WINDOW == true)
      {
        if (sdSysData.COMMS_COMPASS.connected())
        {
          ImGui::SetNextWindowSize(ImVec2(140.0f, 210.0f));

          ImGui::Begin("Compass", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
          {
            ImVec4 working_area = get_working_area();

            ImDrawList* draw_list_status_compass_window = ImGui::GetWindowDrawList();

            COMPASS_WINDOW_COMPASS.draw(draw_list_status_compass_window, sdSysData, 2, 
                        ImVec2(working_area.x + (working_area.z / 2.0f), 
                        working_area.y + (working_area.w / 2.0f)), 
                        working_area.z / 2.0f * 0.6f, true, sdSysData.GPS_SYSTEM.current_position().VALID_GPS_FIX, 
                        sdSysData.GPS_SYSTEM.current_position().VALID_TRACK, sdSysData.GPS_SYSTEM.current_position().TRUE_HEADING, 
                        sdSysData.COMMS_COMPASS.connected(), sdSysData.COMMS_COMPASS.bearing(), true, 
                        true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max());

            if (button_area(working_area))
            {
              DISPLAY_COMPASS_WINDOW = false;
            }
          }
          ImGui::End();
        }
      }

      // ---------------------------------------------------------------------------------------
      
      if (DISPLAY_CARDS_WINDOW == true)
      {
          ImGui::SetNextWindowSize(ImVec2(90.0f, 195.0f));

          ImGui::Begin("Cards", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
          {
            if (button_simple_color(sdSysData, "QR\nCODE", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_QR_CODE = !DISPLAY_QR_CODE;
              DISPLAY_CARDS_WINDOW = false;
            } 

            if (button_simple_color(sdSysData, "REFER", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_REFERENCE_CARD = !DISPLAY_REFERENCE_CARD;
              DISPLAY_CARDS_WINDOW = false;
            } 


            ImGui::End();
          }
      }

      // ---------------------------------------------------------------------------------------
      // Display Advert

      ADVERTISEMENTS.draw(sdSysData);

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
    // TTY SCREEN
    // Handle Console Iputs
    if (SCREEN_COMMS.printw_q_avail() == true)
    {
      printf("%s", SCREEN_COMMS.printw_q_get().c_str());
      
      if (COMMAND_TEXT != "")
      {
        COMMAND_TEXT_CHANGED = true;
      }
    }

    // Clear
    if (SCREEN_COMMS.command_text_clear_get() == true)
    {
      COMMAND_TEXT = "";
      COMMAND_TEXT_CHANGED = true;
    }

    if (COMMAND_TEXT_CHANGED)
    {
      string command_display = left_justify((int)TERMINAL_WINDOW.ws_col, "CMD: " + COMMAND_TEXT) + "\n" + 
                               left_justify((int)TERMINAL_WINDOW.ws_col, " ") + "\n";

      printf ("\033[1;1H%s\033[%d;1H", command_display.c_str(), TERMINAL_WINDOW.ws_row);

      COMMAND_TEXT_CHANGED = false;
    }

    // Read and write to and from only TTY.
    {
      char character_pressed = 0;
      read (0, &character_pressed, 1);

      if (character_pressed > 0)
      {
        bool shift_pressed = false;
        bool backspace_pressed = false;
        bool enter_pressed = false;

        // check for backspace
        if (character_pressed == 127)
        {
          backspace_pressed = true;
        }

        // check for enter
        if (character_pressed == 10)
        {
          enter_pressed = true;
        }

        // Because gui doesnt handle upper case letters, only key presses ...
        // convert to upper case if lower case then set shift to off
        if (character_pressed >= 65 + 32  && character_pressed <= 90 + 32)
        {
          character_pressed -= 32;
          shift_pressed = false;
        }
        else if (character_pressed >= 65 && character_pressed <= 90)
        // leave at upper case and set shift to on
        {
          shift_pressed = true;
        }

        character_enter(character_pressed, shift_pressed, backspace_pressed, enter_pressed);
        COMMAND_TEXT_CHANGED = true;
      }
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

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  }
  else
  {
    struct termios terminal;

    tcgetattr(0, &terminal);

    terminal.c_lflag |=ICANON;  // set conotical mod.
    terminal.c_lflag |= ECHO;    // turn echo off.

    tcsetattr(0, TCSANOW, &terminal);
  }

}

/*
void SCREEN4::update_gps_gadgets(system_data &sdSysData, string Text)
{
  if (sdSysData.TTY_ONLY == false)
  {
    GPS_CONSOLE.add_line(Text);
  }
}
*/

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

/*
void SCREEN4::update_GPS_gadgets(system_data &sdSysData)
{
  if (sdSysData.TTY_ONLY == false)
  {
    // Update GPS Console
    for (int pos = 0; pos < (int)sdSysData.GPS_SYSTEM.RECIEVE_HISTORY.size(); pos++)
    {
      GPS_CONSOLE.add_line(trim(sdSysData.GPS_SYSTEM.RECIEVE_HISTORY[pos]) + "\n");
    }
  }
  
  sdSysData.GPS_SYSTEM.RECIEVE_HISTORY.clear();
}
*/

// ---------------------------------------------------------------------------------------
#endif