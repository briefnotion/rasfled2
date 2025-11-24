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
    DOOR_INDICATOR_RD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE));
  }
  else
  {
    DOOR_INDICATOR_RD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value)
  {
    DOOR_INDICATOR_FD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE));
  }
  else
  {
    DOOR_INDICATOR_FD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value)
  {
    DOOR_INDICATOR_RP.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE));
  }
  else
  {
    DOOR_INDICATOR_RP.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value)
  {
    DOOR_INDICATOR_FP.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE));
  }
  else
  {
    DOOR_INDICATOR_FP.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
  }


  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(0).value || DOOR_INDICATOR_RD.is_changing())
  {    
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), Window_Size, 
                BLACK_QPAQUE, BLACK_QPAQUE, 
                BLACK_QPAQUE, DOOR_INDICATOR_RD.color(sdSysData.PROGRAM_TIME.current_frame_time()));
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value || DOOR_INDICATOR_FD.is_changing())
  {    
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), Window_Size, 
                DOOR_INDICATOR_FD.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE, 
                BLACK_QPAQUE, BLACK_QPAQUE);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value || DOOR_INDICATOR_RP.is_changing())
  {    
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), Window_Size, 
                BLACK_QPAQUE, BLACK_QPAQUE, 
                DOOR_INDICATOR_RP.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE);
  }

  if (sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value || DOOR_INDICATOR_FP.is_changing())
  {    
    Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), Window_Size, 
                BLACK_QPAQUE, DOOR_INDICATOR_FP.color(sdSysData.PROGRAM_TIME.current_frame_time()), 
                BLACK_QPAQUE, BLACK_QPAQUE);
  }
}

void SCREEN4::signal_lights(ImDrawList *Draw_List, system_data &sdSysData, ImVec2 Window_Size)
{
  if (sdSysData.CAR_INFO.active() || sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
  {
    // Set Blinker Color Value
    if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left() || 
        sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right() || 
        sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards() || 
        sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
    {
      if (PING_BLINKER.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()) == false)
      {
        BLINKER_BLINK = !BLINKER_BLINK;
        PING_BLINKER.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), ((790 * 2) / 2));
        
        if (BLINKER_BLINK)
        {
          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left())
          {
            BLINKER_INDICATOR_LEFT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_ORANGE));
          }

          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right())
          {
            BLINKER_INDICATOR_RIGHT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_ORANGE));
          }

          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards() || sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
          {
            BLINKER_INDICATOR_HAZARD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_RED));
          }
        }
        else
        {
          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left())
          {
            BLINKER_INDICATOR_LEFT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
          }

          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right())
          {
            BLINKER_INDICATOR_RIGHT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
          }

          if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards() || sdSysData.PANEL_CONTROL.AUTO_HAZARDS)
          {
            BLINKER_INDICATOR_HAZARD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
          }
        }
      }
    }
    else if (BLINKER_BLINK)
    {
      BLINKER_BLINK = false;
      BLINKER_INDICATOR_LEFT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
      BLINKER_INDICATOR_RIGHT.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
      BLINKER_INDICATOR_HAZARD.set_color(sdSysData.PROGRAM_TIME.current_frame_time(), BLACK_QPAQUE);
    }

    // Draw Blinker

    // Left Blinker
    if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_left() || BLINKER_INDICATOR_LEFT.is_changing())
    {
      Draw_List->AddRectFilledMultiColor(ImVec2(0.0f, 0.0f), ImVec2(Window_Size.x * 1.0f, Window_Size.y), 
          BLINKER_INDICATOR_LEFT.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE, 
          BLACK_QPAQUE, BLINKER_INDICATOR_LEFT.color(sdSysData.PROGRAM_TIME.current_frame_time()));
    }

    // Right Blinker
    if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_sinal_right() || BLINKER_INDICATOR_RIGHT.is_changing())
    {
      Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x * 0.0f, 0.0f), ImVec2(Window_Size.x, Window_Size.y), 
          BLACK_QPAQUE, BLINKER_INDICATOR_RIGHT.color(sdSysData.PROGRAM_TIME.current_frame_time()), 
          BLINKER_INDICATOR_RIGHT.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE);
    }

    // Hazard Blinker
    if (sdSysData.CAR_INFO.STATUS.INDICATORS.val_hazards() || sdSysData.PANEL_CONTROL.AUTO_HAZARDS || BLINKER_INDICATOR_HAZARD.is_changing())
    {
      Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x * 0.0f, 0.0f), ImVec2(Window_Size.x * 0.5f, Window_Size.y), 
          BLACK_QPAQUE, BLINKER_INDICATOR_HAZARD.color(sdSysData.PROGRAM_TIME.current_frame_time()), 
          BLINKER_INDICATOR_HAZARD.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE);

      Draw_List->AddRectFilledMultiColor(ImVec2(Window_Size.x * 0.5f, 0.0f), ImVec2(Window_Size.x * 1.0f, Window_Size.y), 
          BLINKER_INDICATOR_HAZARD.color(sdSysData.PROGRAM_TIME.current_frame_time()), BLACK_QPAQUE, 
          BLACK_QPAQUE, BLINKER_INDICATOR_HAZARD.color(sdSysData.PROGRAM_TIME.current_frame_time()));
    }
  }
}

void SCREEN4::set_screen_default_colors(system_data &sdSysData)
{
  //sdSysData.PANEL_CONTROL.COLOR_SELECT.init(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f);
  
  ImGuiStyle& style = ImGui::GetStyle();

  ImColor background = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_BACKGROUND(RAS_BLUE);
  ImColor dim = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_DIM(RAS_BLUE);
  ImColor standard = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_BLUE);
  ImColor hovered = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_HOVERED(RAS_BLUE);
  ImColor active = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_ACTIVE(RAS_BLUE);

  //style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  style.Colors[ImGuiCol_Text] = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE);

  style.Colors[ImGuiCol_TitleBg] = standard;
  style.Colors[ImGuiCol_TitleBgActive] = active;
  style.Colors[ImGuiCol_TitleBgCollapsed] = background;

  style.Colors[ImGuiCol_FrameBg] = background;
  style.Colors[ImGuiCol_FrameBgHovered] = hovered;
  style.Colors[ImGuiCol_FrameBgActive] = active;

  style.Colors[ImGuiCol_Border] = background;
  style.Colors[ImGuiCol_BorderShadow] = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_BACKGROUND(RAS_YELLOW);

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

  style.Colors[ImGuiCol_Separator] = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_STANDARD(RAS_WHITE);
  style.Colors[ImGuiCol_SeparatorHovered] = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_HOVERED(RAS_WHITE);
  style.Colors[ImGuiCol_SeparatorActive] = sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_ACTIVE(RAS_WHITE);

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
    // Set Blinker Speed
    BLINKER_INDICATOR_LEFT.PROPS.DURATION = ((790.0f * 2.0f) / 4.0f);
    BLINKER_INDICATOR_RIGHT.PROPS.DURATION = ((790.0f * 2.0f) / 4.0f);
    BLINKER_INDICATOR_HAZARD.PROPS.DURATION = ((790.0f * 2.0f) / 4.0f);

    // Set colors

    //sdSysData.PANEL_CONTROL.COLOR_SELECT.init(sdSysData.PROGRAM_TIME.current_frame_time(), 1.0f);

    // Init and prepare DISPLAY_SCREEN
    
    clear_color = ImVec4(0.00f, 0.55f, 0.00f, 0.00f);

    BLACK_QPAQUE = sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_BLACK).STANDARD;
    BLACK_QPAQUE.Value.w = 0.0f;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
      printf("Graphics Set Error\n");
      return 2;
    }

    // Create window with graphics context
    int width = DEF_SCREEN_WIDTH;
    int height = DEF_SCREEN_HEIGHT;

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
    
    //ImGui_ImplOpenGL2_Init();

    const char* glsl_version = "#version 300 es"; // For OpenGL ES 3.0+
    ImGui_ImplOpenGL3_Init(glsl_version);

    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 57.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 28.0f);
    //io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 100.0f);

    sdSysData.PANEL_CONTROL.FONT_18 =       io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 18.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER);
    sdSysData.PANEL_CONTROL.FONT_57 =       io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 57.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER);
    sdSysData.PANEL_CONTROL.FONT_28 =       io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 28.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER);
    sdSysData.PANEL_CONTROL.FONT_100 =      io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 100.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER);

    // Console Font
    
    ImFontConfig config;

    static const ImWchar glyph_ranges[] = 
    {
        0x0020, 0x00FF,       // Basic Latin (ASCII + Latin-1 Supplement)
        0x2000, 0x206F,       // General Punctuation 
        0x2190, 0x21FF,       // Arrows 
        0x2500, 0x257F,       // Box Drawing 
        0x2580, 0x25FF,       // Block Elements + Geometric Shapes 
        0x2800, 0x28FF,       // Braille Patterns 
        0x2B00, 0x2BFF,       // Miscellaneous Symbols and Arrows 
        0,                    // End of list
    };

    sdSysData.PANEL_CONTROL.FONT_CONSOLE =  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",18.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, &config, glyph_ranges);
    //sdSysData.PANEL_CONTROL.FONT_CONSOLE =  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",15.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, &config, glyph_ranges);
    //sdSysData.PANEL_CONTROL.FONT_CONSOLE =  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/unifont/unifont.ttf",15.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, &config, glyph_ranges);
    //sdSysData.PANEL_CONTROL.FONT_CONSOLE =  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf",15.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER, &config, glyph_ranges);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
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
    ADSB.create();

    // Prepare Current Screen Widgets
    LIGHTS.PROPS.COLOR_TRUE = RAS_WHITE;
    LIGHTS.PROPS.COLOR_FALSE = RAS_BLUE;
    LIGHTS.update_text("LED ILLUM", "LED ILLUM");

    CAMERA.PROPS.COLOR_TRUE = RAS_WHITE;
    CAMERA.PROPS.COLOR_FALSE = RAS_BLUE;
    CAMERA.update_text(" CAMERA  ", " CAMERA  ");

    DEBUG.PROPS.COLOR_TRUE = RAS_RED;
    DEBUG.PROPS.COLOR_FALSE = RAS_WHITE;
    DEBUG.update_text("  DEBUG  ", "         ");

    OVERHEAD.PROPS.COLOR_TRUE = RAS_WHITE;
    OVERHEAD.PROPS.COLOR_FALSE = RAS_BLUE;
    OVERHEAD.update_text(" OVERHEAD ", " OVERHEAD ");

    HAZARD.PROPS.COLOR_TRUE = RAS_RED;
    HAZARD.PROPS.COLOR_FALSE = RAS_BLUE;
    HAZARD.update_text("  HAZARD  ", "  HAZARD  ");

    DAY_NIGHT.PROPS.COLOR_TRUE = RAS_WHITE;
    DAY_NIGHT.PROPS.COLOR_FALSE = RAS_BLUE;
    DAY_NIGHT.update_text("  DAY  ", " NIGHT ");

    TIMER.PROPS.COLOR_TRUE = RAS_WHITE;
    TIMER.PROPS.COLOR_FALSE = RAS_BLUE;
    TIMER.update_text(" TIMER ", " TIMER ");

    GPS.PROPS.COLOR_TRUE = RAS_WHITE;
    GPS.PROPS.COLOR_FALSE = RAS_BLUE;
    GPS.update_text("  GPS  ", "  GPS  ");

    COMPASS.PROPS.COLOR_TRUE = RAS_WHITE;
    COMPASS.PROPS.COLOR_FALSE = RAS_BLUE;
    COMPASS.update_text("COMPASS", "COMPASS");

    /*

    DOOR2.PROPS.COLOR_TRUE = RAS_WHITE;
    DOOR2.PROPS.COLOR_FALSE = RAS_BLUE;
    DOOR2.update_text(" DOOR2", " DOOR2");

    DOOR3.PROPS.COLOR_TRUE = RAS_WHITE;
    DOOR3.PROPS.COLOR_FALSE = RAS_BLUE;
    DOOR3.update_text("DOOR3 ", "DOOR3 ");

    DOOR4.PROPS.COLOR_TRUE = RAS_WHITE;
    DOOR4.PROPS.COLOR_FALSE = RAS_BLUE;
    DOOR4.update_text("DOOR4 ", "DOOR4 ");
    */

    AUTO.PROPS.COLOR_TRUE = RAS_WHITE;
    AUTO.PROPS.COLOR_FALSE = RAS_BLUE;
    AUTO.update_text(" AUTO  ", " AUTO  ");

    ADSB_IND.PROPS.COLOR_TRUE = RAS_WHITE;
    ADSB_IND.PROPS.COLOR_FALSE = RAS_BLUE;
    ADSB_IND.update_text(" ADS-B ", 
                          " ADS-B ");

    string version = "v: ";
    version = version + Revision;
    VERSION.update_text(sdSysData, version);
    VERSION.PROPS.COLOR = RAS_WHITE;

    TEMP.update_text(sdSysData, "Temp: NA");
    TEMP.PROPS.COLOR = RAS_WHITE;

    {
      BAR_TIMER.PROPS.LABEL = "Timer";
      BAR_TIMER.PROPS.BAR_HEIGHT = 20.0f * DEF_SCREEN_SIZE_X_MULTIPLIER;
      BAR_TIMER.PROPS.MARKER_SIZE = 15.0f * DEF_SCREEN_SIZE_X_MULTIPLIER;
      BAR_TIMER.PROPS.COLOR_BACKGROUND = RAS_BLUE;
      BAR_TIMER.PROPS.COLOR_MARKER = RAS_YELLOW;
      BAR_TIMER.PROPS.DRAW_MIN_MAX_ON_TOP = false;
      BAR_TIMER.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      BAR_TIMER.PROPS.DRAW_MIN_MAX = false;
      BAR_TIMER.PROPS.MAX = 1.0f;
      BAR_TIMER.PROPS.DRAW_RULER = true;
      BAR_TIMER.PROPS.COLOR_RULER = RAS_WHITE;
      BAR_TIMER.PROPS.MAX_TICK_LEVEL = 3;
      BAR_TIMER.create();
    }

    // Load Advertisements
    ADVERTISEMENTS.create(sdSysData);

    // Prepare Compass
    COMPASS_WINDOW_COMPASS.set_size(32, (15 / 2));
    // set at frame rate for slow and jitter size for fast.
    
    // Set font size on create.
    ImVec2 text_size = ImGui::CalcTextSize("Alerts_List.res_alert_text_line(alert_num).c_str()");
    sdSysData.SCREEN_META_DATA.TYPICAL_PIXEL_LINE_HEIGHT = text_size.y;
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

void SCREEN4::character_enter(system_data &sdSysData, unsigned char Character, 
                bool Shift_Pressed, bool Backspace_Pressed, bool Enter_Pressed)
{
    // Letters
    if (Character >= 65 && Character <= 90)
    {
      // All Upper Case Letters, but with shift not pressed to lower the case.
      if (Shift_Pressed)
      {
        COMMAND_TEXT += Character;
        sdSysData.SCREEN_COMMS.command_text_set(COMMAND_TEXT);
      }
      else
      {
        COMMAND_TEXT += Character + 32;
        sdSysData.SCREEN_COMMS.command_text_set(COMMAND_TEXT);
      }
    }

    // Symbols
    else if ((Character >= 32 && Character <= 64) || 
              (Character >= 91 && Character <= 96) || 
              (Character >= 123 && Character <= 126))
    {
      COMMAND_TEXT += Character;
      sdSysData.SCREEN_COMMS.command_text_set(COMMAND_TEXT);
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
      sdSysData.SCREEN_COMMS.carrage_return_set();
    }
}

// ---------------------------------------------------------------------------------------

void SCREEN4::draw(system_data &sdSysData, ANIMATION_HANDLER &Animations)
{
  if (sdSysData.TTY_ONLY == false)
  {
    // For all colors, set the current frame time
    sdSysData.PANEL_CONTROL.COLOR_SELECT.set_frame_time(sdSysData.PROGRAM_TIME.current_frame_time());

    //Check for changes in style
    if (sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_is_changing())
    {
      set_screen_default_colors(sdSysData);
    }

    // Handle Console Iputs
    if (sdSysData.SCREEN_COMMS.printw_q_avail() == true)
    {
      CONSOLE.add_line(sdSysData.SCREEN_COMMS.printw_q_get());
    }
    
    // Clear
    if (sdSysData.SCREEN_COMMS.command_text_clear_get() == true)
    {
      COMMAND_TEXT = "";
    }

    DISPLAY_TIMER = sdSysData.cdTIMER.is_active();

    // Will need to create a new ADS-B Alert Widget for every ADSB Enty in the list
    while (ADSB_ALERTS.size() < sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size())
    {
      ALERT_WIDGET tmp_adsb_alert_widget;
      ADSB_ALERTS.push_back(tmp_adsb_alert_widget);
    }

    // ---------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------
    // Handle Console Outputs
    ImGuiIO &io = ImGui::GetIO();
    //ImGuiStyle& style = ImGui::GetStyle();

    // reduce call backs.
    unsigned long current_frame_time = sdSysData.PROGRAM_TIME.current_frame_time();
    
    glfwPollEvents();

    // Check for keybaord input
    // Disable if on terminal screen
    if (sdSysData.PANEL_CONTROL.console_enabled(current_frame_time))
    {
      int character_pressed = 0;
      bool shift_pressed = false;
      bool backspace_pressed = false;
      bool enter_pressed = false;

      for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) 
      {
        if (ImGui::IsKeyDown((ImGuiKey)i))
        {  
          
          // Check for backspace
          if ((i == ImGuiKey_Backspace) && !COMMAND_TEXT.empty()) 
          {
              character_pressed = i;
              backspace_pressed = true;
          }

          // Check for enter key
          if (i == ImGuiKey_Enter || i == ImGuiKey_KeypadEnter) 
          {
              character_pressed = i;
              enter_pressed = true;
          }
          // Check for shift key
          if (i == ImGuiKey_LeftShift || i == ImGuiKey_RightShift || i == ImGuiKey_ModShift) 
          {
              shift_pressed = true;
          }

          // Check for 0 through 9
          if (i >= ImGuiKey_0 && i <= ImGuiKey_9)
          {
            //ImGuiKey_0 = 536 
            //ImGuiKey_1 = 537;
            //ImGuiKey_9 = 545
            character_pressed = i - 488;
          }

          // Check for A through Z
          if (i >= ImGuiKey_A && i <= ImGuiKey_Z)
          {
            //ImGuiKey_A = 546
            //ImGuiKey_Y   
            //ImGuiKey_Z = 571
            character_pressed = i - 481;
          }

          // Check for space
          if (i == ImGuiKey_Space)
          {
            character_pressed = 32;
          }

          // Check for special characters
          if (i == ImGuiKey_GraveAccent)
          {
            character_pressed = 96;
          }
        }
      }


      if (character_pressed > 0 && character_pressed != PREV_FRAME_KEY_DOWN)
      {
        PREV_FRAME_KEY_DOWN = character_pressed;

        character_enter(sdSysData, character_pressed, shift_pressed, backspace_pressed, enter_pressed);

      }
      else if (character_pressed == 0)
      {
        PREV_FRAME_KEY_DOWN = 0;
      }
    }

    // ---------------------------------------------------------------------------------------

    // Set the Reorganize all data flag to be true on the next automobile update.
    AUTOMOBILE.set_reorganize_data_flag_on();

    // ---------------------------------------------------------------------------------------

    // Start the Dear ImGui frame
    //ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (sdSysData.DNFWTS.active_update(current_frame_time))
    {
      if (ImGui::Begin("DNFWTS", &show_test_window, sdSysData.SCREEN_DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      {
        ImVec4 working_area = get_working_area();

        if (DISPLAY_DNFWTS_SHOW)
        {
          ImDrawList* draw_list = ImGui::GetWindowDrawList();
          ImVec2 line_start_1;
          ImVec2 line_end_1;
          ImVec2 line_start_2;
          ImVec2 line_end_2;

          line_start_1 = ImVec2(working_area.x + 50.0f, working_area.y + 50.0f);
          line_end_1 = ImVec2(working_area.x + working_area.z - 50.0f, working_area.y + working_area.w - 50.0f);
          
          line_start_2 = ImVec2(working_area.x + working_area.z - 50.0f, working_area.y + 50.0f);
          line_end_2 = ImVec2(working_area.x + 50.0f, working_area.y + working_area.w - 50.0f);

          draw_list->AddLine(line_start_1, line_end_1, sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_RED).STANDARD, 50.0f);
          draw_list->AddLine(line_start_2, line_end_2, sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_RED).STANDARD, 50.0f);

          ImGui::PushFont(sdSysData.PANEL_CONTROL.FONT_28);
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.pure_color(RAS_RED).STANDARD));

          ImGui::SetCursorScreenPos(ImVec2(working_area.x + 50.0f, working_area.y + 150.0f));
          ImGui::Text(sdSysData.DNFWTS.DNFWTS_TEXT_1.value().c_str());
          
          ImGui::SetCursorScreenPos(ImVec2(working_area.x + 200.0f, working_area.y + 50.0f));
          ImGui::Text(sdSysData.DNFWTS.DNFWTS_TEXT_2.value().c_str());
          
          
          ImGui::PopStyleColor();
          ImGui::PopFont();
        }

        if (button_area(working_area))
        {
          DISPLAY_DNFWTS_SHOW = !DISPLAY_DNFWTS_SHOW;
        }

        ImGui::End();
      }
    }
    else
    {
      if (ImGui::Begin("Window", &show_test_window, sdSysData.SCREEN_DEFAULTS.flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      {
        // draw list
        ImDrawList* draw_list_window_background = ImGui::GetWindowDrawList();

        // Auto Signal and Door Lights
        door_lights(draw_list_window_background, sdSysData, viewport->Size);
        signal_lights(draw_list_window_background, sdSysData, viewport->Size);

        ImGui::BeginChild("Main", ImVec2(ImGui::GetContentRegionAvail().x - (85.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
        {
          // ---------------------------------------------------------------------------------------
          // Status Sub Window

          //ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
          ImGui::BeginChild("Status", ImVec2(ImGui::GetContentRegionAvail().x, 60.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER), true, sdSysData.SCREEN_DEFAULTS.flags_c);
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

              ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));
              ImGui::PushFont(sdSysData.PANEL_CONTROL.FONT_28);
              ImGui::Text("CMD:");

              ImGui::SameLine();

              //ImGui::Text(COMMAND_TEXT.c_str());
              simple_wrap_text_box(COMMAND_TEXT, draw_list_status_left, sdSysData);

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

            ImGui::BeginChild("Status Mid", ImVec2((region_div_4 * 2.0f) - (45.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              ImVec4 working_area_status_mid = get_working_area();
              ImDrawList* draw_list_status_mid = ImGui::GetWindowDrawList();

              // Display Lights Off mode toggle.
              ImGui::BeginGroup();
              {
                LIGHTS.update_tf(sdSysData.Lights_On.value());
                LIGHTS.draw(sdSysData);

                if (sdSysData.SCREEN_COMMS.DEBUG_STATUS.DEBUG)
                {
                  DEBUG.update_tf(sdSysData.SCREEN_COMMS.DEBUG_STATUS.DEBUG);
                  DEBUG.draw(sdSysData);
                }
                else
                {
                  CAMERA.update_tf(sdSysData.CAMERA_BACKUP.camera_online());
                  CAMERA.draw(sdSysData);
                }
              }
              ImGui::EndGroup();
              
              ImGui::SameLine();

              // Status Group
              ImGui::BeginGroup();
              {
                OVERHEAD.update_tf(sdSysData.booOverheadRunning);
                OVERHEAD.draw(sdSysData);

                HAZARD.update_tf(sdSysData.PANEL_CONTROL.AUTO_HAZARDS);
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
                if (sdSysData.AIRCRAFT_COORD.DATA.POSITIONED_AIRCRAFT == 0)
                {
                  ADSB_IND.update_text(center_justify(7, "(-)"), 
                                        " ADS-B ");
                }
                else
                {
                  ADSB_IND.update_text(center_justify(7, "(" + to_string(sdSysData.AIRCRAFT_COORD.DATA.POSITIONED_AIRCRAFT) + ")"), 
                                        " ADS-B ");
                }
                ADSB_IND.draw(sdSysData);
              }
              ImGui::EndGroup();

              // Change Screens
              if (COROP_VOID.button_flash_color(draw_list_status_mid, sdSysData, "COROP_VOID_HIDDEN", RAS_RED, 
                                                  ImVec2(working_area_status_mid.x, working_area_status_mid.y), 
                                                  ImVec2(working_area_status_mid.z, working_area_status_mid.w)))
              {
                sdSysData.DNFWTS.turn_on(current_frame_time);
              }
            }
            ImGui::EndChild();

            // Mid Right
            ImGui::SameLine();

            ImGui::BeginChild("Status Right", ImVec2(ImGui::GetContentRegionAvail().x - (90.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              // Assign Draw List
              ImVec4 working_area_status_right = get_working_area();
              ImDrawList* draw_list_status_right = ImGui::GetWindowDrawList();

              VERSION.draw(sdSysData);

              if (sdSysData.hsHardware_Status.enabled() == true)
              {
                TEMP.update_text(sdSysData, ("Temp: " + to_string((int)sdSysData.hsHardware_Status.get_temperature()) + "c").c_str());
                TEMP.draw(sdSysData);
              }
              
              if (RUNNING_COLOR.button_flash_color(draw_list_status_right, sdSysData, "RUNNING_COLOR_HIDDEN", RAS_RED, 
                                                  ImVec2(working_area_status_right.x, working_area_status_right.y), 
                                                  ImVec2(working_area_status_right.z, working_area_status_right.w)))
              {
                sdSysData.PANEL_CONTROL.COLOR_SELECT.toggle_void_color(sdSysData.PROGRAM_TIME.current_frame_time());
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
                              (working_area.z / 6.0f), false, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                              sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, (sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE), 
                              sdSysData.COMMS_COMPASS.connected(), (sdSysData.COMMS_COMPASS.bearing_calibrated()), false, 0.0f);

                // Change Screens
                if (button_area(working_area))
                {
                  sdSysData.PANEL_CONTROL.DISPLAY_COMPASS_WINDOW = !sdSysData.PANEL_CONTROL.DISPLAY_COMPASS_WINDOW;
                }
              }
            }
            ImGui::EndChild();
          }
          ImGui::EndChild();

          // ---------------------------------------------------------------------------------------
          // Console Sub Window

          ImGui::BeginChild("DISPLAY_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - (48 * DEF_SCREEN_SIZE_Y_MULTIPLIER)), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 0)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("CONSOLE_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
                  
                  CONSOLE.display(sdSysData, "CONSOLE", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
                    
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
                CONSOLE.display(sdSysData, "CONSOLE", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              }
            }

            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 1)
            {
              ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
              AUTOMOBILE.display(sdSysData, DISPLAY_CONFIRM);
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

            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 2)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("ADSB_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                  ADSB.display(sdSysData);
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
                ADSB.display(sdSysData);
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

            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 4)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("LOGS_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
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

            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 5)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("...", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                  DOT_DOT_DOT.display(sdSysData, Animations);
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
                DOT_DOT_DOT.display(sdSysData, Animations);
              }
            }

            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 6)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("TERMINAL", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
                  ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

                  TERMINAL.display(sdSysData);
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
                TERMINAL.display(sdSysData);
              }
            }



            else if (sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 7)
            {
              if (sdSysData.CAR_INFO.active())
              {
                ImGui::BeginChild("Backup Camera", ImVec2(ImGui::GetContentRegionAvail().x - (106.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
                {
                  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                  BACKUP_CAMERA.display(sdSysData, degrees_to_radians(sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle() / 12.0f));
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
                BACKUP_CAMERA.display(sdSysData, degrees_to_radians(sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle() / 12.0f));
              }
            }

            else
            {
              ImGui::Begin(" ", NULL, sdSysData.SCREEN_DEFAULTS.flags_w);
              {
                // Show set screen to console.
                sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 0;
              }
              ImGui::End();
            }
          }
          ImGui::EndChild();

          // ---------------------------------------------------------------------------------------
          // Tabs Sub Window

          // Restack windows placed here to complete cycle.
          RESTACK_WINDOWS = false;

          ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x - sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB.x - (15.0f * DEF_SCREEN_SIZE_X_MULTIPLIER), ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (BTC_TAB_CONSOLE.button_toggle_color(sdSysData, "CONSOLE", "CONSOLE", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 0, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 0;
              RESTACK_WINDOWS = true;
            }

            ImGui::SameLine();

            if (BTC_TAB_AUTOMOBILE.button_toggle_color(sdSysData, "AUTOMOBILE", "AUTOMOBILE", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 1, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 1;
            }

            ImGui::SameLine();

            if (BTC_TAB_ADSB.button_toggle_color(sdSysData, "MAP", "MAP", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 2, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 2;
            }

            ImGui::SameLine();

            /*
            if (button_toggle_color(sdSysData, "GPS", "GPS", DISPLAY_SCREEN == 3, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_SCREEN = 3;
            }

            ImGui::SameLine();
            */

            if (BTC_TAB_LOGS.button_toggle_color(sdSysData, "LOGS", "LOGS", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 4, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 4;
              RESTACK_WINDOWS = true;
            }

            ImGui::SameLine();

            if (BTC_TAB_DOTDOTDOT.button_toggle_color(sdSysData, ".", ".", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 5, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 5;
            }

            ImGui::SameLine();

            if (BTC_TAB_TERMINAL.button_toggle_color(sdSysData, "[]", "[]", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 6, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 6;
            }

            ImGui::SameLine();

            if (BTC_TAB_CAMERA.button_toggle_color(sdSysData, "O", "O", sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN == 7, RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 7;
            }

            ImGui::SameLine();

            ImGui::BeginChild("Info Bar", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
            {
              
              ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.PANEL_CONTROL.COLOR_SELECT.neo_color_TEXT(RAS_GREEN)));
              ImGui::PushFont(sdSysData.PANEL_CONTROL.FONT_28);

              // Display the Autononomous State or Street Legal Mode
              if (sdSysData.PANEL_CONTROL.autonomous_state() == 2)
              {
                ImGui::Text(sdSysData.PANEL_CONTROL.EXTRA.c_str());
              }
              else
              {
                /*
                if (sdSysData.CAR_INFO.STREET_LEGAL_MODE == true)
                {
                  //ImGui::Text("%s", mask_string("Street_Legal_Md=ON", 3).c_str());
                  ImGui::Text("%s", "Street_Legal_Md=ON");
                }
                else
                {
                  //ImGui::Text("%s", mask_string("Street_Legal_Md=OFF", 3).c_str());
                  ImGui::Text("%s", "Street_Legal_Md=OFF");
                }
                */
              }

              ImGui::PopFont();
              ImGui::PopStyleColor();
            }
            ImGui::EndChild();

          }
          ImGui::EndChild();

          if (sdSysData.ALL_ALERTS.alert_count() > 0  || sdSysData.AIRCRAFT_COORD.ADSB_ALERT_COUNT > 0)
          {
            ImGui::SameLine();

            if (BT_DISPLAY_ALERTS.button_color(sdSysData, "DISPLAY\nALERTS", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.ALL_ALERTS.display_active_alerts();

              for (int adsb_alert = 0; adsb_alert < (int)sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST.size(); adsb_alert++)
              {
                sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[adsb_alert].ALERTS_ADSB.display_active_alerts();
              }
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
            if (BTC_TIMER.button_toggle_color(sdSysData, "TIMER", "TIMER", sdSysData.cdTIMER.is_active(), RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              if (sdSysData.cdTIMER.is_active() == false)
              {
                sdSysData.SCREEN_COMMS.command_text_set("  ");
              }
              else
              {
                sdSysData.SCREEN_COMMS.command_text_set(" `");
              }
            }

            //ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            // Autononoumous button (3 values on a toggle)
            if (sdSysData.PANEL_CONTROL.autonomous_state() == 0 || sdSysData.PANEL_CONTROL.autonomous_state() == 1)  // On or Off
            {
              if (BTC_AUTONOMOUS.button_toggle_color(sdSysData, "AUTON\n(On)", "AUTON\n(Off)", sdSysData.PANEL_CONTROL.autonomous_state() == 1, 
                                                  RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
              {
                if (sdSysData.PANEL_CONTROL.autonomous_state() == 1)
                {
                  sdSysData.PANEL_CONTROL.autonomous_off();
                }
                else
                {
                  sdSysData.PANEL_CONTROL.autonomous_on();
                }
              }
            }
            else // if (sdSysData.PANEL_CONTROL.autonomous_state() == 2)
            {
              if (BTC_AUTONOMOUS.button_toggle_color(sdSysData, "AUTON\n(Active)", "INVALID\nSTATE", sdSysData.PANEL_CONTROL.autonomous_state() == 2, 
                                    RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
              {
                sdSysData.PANEL_CONTROL.autonomous_off();
              }
            }

            // Overhead Lights
            if (BTC_OVER_HEAD_LIGHTS.button_toggle_color(sdSysData, "OVER\nHEAD\nLIGHTS", "OVER\nHEAD\nLIGHTS", sdSysData.booOverheadRunning, RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              if (sdSysData.booOverheadRunning == true)
              {
                sdSysData.SCREEN_COMMS.command_text_set("o`");
              }
              else
              {
                if (DISPLAY_OVERHEAD_COLOR == true)
                {
                  sdSysData.SCREEN_COMMS.command_text_set("oo");
                  DISPLAY_OVERHEAD_COLOR = false;
                }
                else
                {
                  DISPLAY_OVERHEAD_COLOR = true;
                }
              }
            }

            if (BT_FLASH.button_color(sdSysData, "FLASH", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.SCREEN_COMMS.command_text_set("ff");
            }

            if (BT_CARDS.button_color(sdSysData, "CARDS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_CARDS_WINDOW = !DISPLAY_CARDS_WINDOW;
            } 

            if (BTC_LIGHTS.button_toggle_color(sdSysData, "LIGHTS", "LIGHTS", sdSysData.Lights_On.value(), RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              if (sdSysData.Lights_On.value() == true)
              {
                sdSysData.SCREEN_COMMS.command_text_set(" lightsoff");
              }
              else
              {
                sdSysData.SCREEN_COMMS.command_text_set(" lightson");
              }
            }

            if (BT_DOTDOTDOT.button_color(sdSysData, "...", RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 1;
            } 
          }
          else if (DISPLAY_MENU == 1)
          {
            if (BTC_HAZARD.button_toggle_color(sdSysData, "HAZARD", "HAZARD", sdSysData.PANEL_CONTROL.AUTO_HAZARDS, RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              if (sdSysData.PANEL_CONTROL.AUTO_HAZARDS == true)
              {
                sdSysData.SCREEN_COMMS.command_text_set("h`");
              }
              else
              {
                sdSysData.SCREEN_COMMS.command_text_set("hh");
              }
            }

            if (BTC_RUNNING_COLOR.button_toggle_color(sdSysData, "SET\nRUNNING\nCOLOR", "SET\nRUNNING\nCOLOR", DISPLAY_RUNNING_COLOR, RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_RUNNING_COLOR = !DISPLAY_RUNNING_COLOR;
            }

            if (BTC_CAMERA_START_STOP.button_toggle_color(sdSysData, "CAMERA\nSTOP", "CAMERA\nSTART", sdSysData.CAMERA_BACKUP.camera_online(), RAS_RED, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              if (sdSysData.CAMERA_BACKUP.camera_online())
              {
                sdSysData.SCREEN_COMMS.command_text_set(" camf");
              }
              else
              {
                sdSysData.SCREEN_COMMS.command_text_set(" camo");
              }
            }

            //ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);


            if (sdSysData.Day_On_With_Override.value())
            {
              if (BT_NIGHT_MODE.button_color(sdSysData, "NIGHT\nMODE", RAS_WHITE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
              {
                sdSysData.SCREEN_COMMS.command_text_set("dd");
              }
            }
            else
            {
              if (sdSysData.Day_On)
              {
                if (BT_NIGHT_MODE.button_color(sdSysData, "NIGHT\nMODE", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
                {
                  sdSysData.SCREEN_COMMS.command_text_set("dd");
                }
              }
              else
              {
                if (BT_NIGHT_MODE.button_color(sdSysData, "NIGHT\nMODE", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
                {
                  sdSysData.SCREEN_COMMS.command_text_set("dd");
                }
              }
            }

            if (BT_CLEAR_ANIMS.button_color(sdSysData, "CLEAR\nANIMS", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.SCREEN_COMMS.command_text_set("``");
            }

            if (BT_SYSTEM.button_color(sdSysData, "SYSTEM", RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 2;
            }

            if (BT_BACK_1.button_color(sdSysData, "<-", RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_MENU = 0;
            }
          }
          else if (DISPLAY_MENU == 2)
          {
            if (BT_EXIT.button_color(sdSysData, "EXIT", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.SCREEN_COMMS.command_pending_set("X");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            //ImGui::InvisibleButton("noshow2", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (BT_SYSTEM_SHUT_DOWN.button_color(sdSysData, "SYSTEM\nSHUT\nDOWN", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.SCREEN_COMMS.command_pending_set(" shutdown");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            if (BT_REBOOT.button_color(sdSysData, "SYSTEM\nREBOOT", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.SCREEN_COMMS.command_pending_set(" reboot");
              DISPLAY_CONFIRM = !DISPLAY_CONFIRM;
            }

            ImGui::InvisibleButton("noshow4", sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON);

            if (BT_TERMINAL.button_color(sdSysData, "TERMINAL", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              sdSysData.PANEL_CONTROL.PANELS.MAIN_DISPLAY_SCREEN = 6;
            }

            if (BT_DEBUG.button_color(sdSysData, "DEBUG", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_DEBUG = !DISPLAY_DEBUG;
            }

            if (BT_BACK_2.button_color(sdSysData, "<-", RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
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
      else
      {
        sdSysData.SCREEN_COMMS.WINDOW_CLOSE = true;
      }

      ImGui::End();

      // ---------------------------------------------------------------------------------------
      // Debug Window
      
      if (DISPLAY_DEBUG == true)
      {
        ImGui::SetNextWindowSize(ImVec2(266 * DEF_SCREEN_SIZE_X_MULTIPLIER, 345 * DEF_SCREEN_SIZE_Y_MULTIPLIER));
        if (ImGui::Begin("Debug", &DISPLAY_DEBUG, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          ImGui::Text("%.3f ms/frame  %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
          //ImGui::Text("%.1f FPS", io.Framerate);

          if (BTC_DEBUG.button_toggle_color(sdSysData, "Dug", "Dbug", sdSysData.SCREEN_COMMS.DEBUG_STATUS.DEBUG, RAS_WHITE, RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            sdSysData.SCREEN_COMMS.DEBUG_STATUS.DEBUG = ! sdSysData.SCREEN_COMMS.DEBUG_STATUS.DEBUG;
          }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(1)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          ImGui::SameLine();

          if (BTC_DEBUG_2.button_toggle_color(sdSysData, "2", "2", sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[1], RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[1] = !sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
          }

          ImGui::SameLine();

          if (BTC_DEBUG_4.button_toggle_color(sdSysData, "4", "4", sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[3], RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[3] = !sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[3];
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

          if (BTC_DEBUG_1.button_toggle_color(sdSysData, "1", "1", sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[0], RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
          {
            sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[0] = !sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
          }

          ImGui::SameLine();

            ImGui::TableNextColumn();
            if (BTC_DEBUG_3.button_toggle_color(sdSysData, "3", "3", sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[2], RAS_WHITE, RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL))
            {
              sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[2] = !sdSysData.SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
            }

          ImGui::SameLine();

          button_simple_enabled(sdSysData, to_string(sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(2)).c_str(), false, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_SMALL);

          // Rasfled Stats
          ImGui::Text("Prev Sleep : %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.PREVSLEEPTIME.get_data(), sdSysData.PROGRAM_TIME.PREVSLEEPTIME.start_to_start_time());
          ImGui::Text("   Compute : %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.COMPUTETIME.get_data(), sdSysData.PROGRAM_TIME.COMPUTETIME.start_to_start_time());
          ImGui::Text("     Cycle : %3.0lf %3.0lf ms", sdSysData.PROGRAM_TIME.CYCLETIME.get_data(), sdSysData.PROGRAM_TIME.CYCLETIME.start_to_start_time());
          ImGui::NewLine();
          ImGui::Text("AUTO Comms : %3.0lf %3.0lf ms", sdSysData.dblCOMMS_AUTO_TRANSFER_TIME.get_data(), sdSysData.dblCOMMS_AUTO_TRANSFER_TIME.start_to_start_time());
          ImGui::Text(" GPS Comms : %3.0lf %3.0lf ms", sdSysData.dblCOMMS_GPS_TRANSFER_TIME.get_data(), sdSysData.dblCOMMS_GPS_TRANSFER_TIME.start_to_start_time());
          ImGui::Text("   COMPASS : %3.0lf %3.0lf ms", sdSysData.dblCOMMS_COMPASS_DATA_READ_TIME.get_data(), sdSysData.dblCOMMS_COMPASS_DATA_READ_TIME.start_to_start_time());
          ImGui::Text("    Screen : %3.0lf %3.0lf ms", sdSysData.dblSCREEN_RENDER_TIME.get_data(), sdSysData.dblSCREEN_RENDER_TIME.start_to_start_time());
          ImGui::Text("LED Render : %3.0lf %3.0lf ms", sdSysData.dblCOMMS_LED_RENDER_TIME.get_data(), sdSysData.dblCOMMS_LED_RENDER_TIME.start_to_start_time());
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Timer Window

      if (DISPLAY_TIMER == true && RESTACK_WINDOWS == false)
      {
        ImGui::SetNextWindowSize(ImVec2(250 * DEF_SCREEN_SIZE_X_MULTIPLIER, 90 * DEF_SCREEN_SIZE_Y_MULTIPLIER));
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
        ImGui::SetNextWindowSize(ImVec2(143 * DEF_SCREEN_SIZE_X_MULTIPLIER, 292 * DEF_SCREEN_SIZE_Y_MULTIPLIER));
        if (ImGui::Begin("Overhead Color", &DISPLAY_OVERHEAD_COLOR, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          if (BT_OVER_RED.button_color(sdSysData, "Red", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("or");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_OVER_GREEN.button_color(sdSysData, "Green", RAS_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("og");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          if (BT_OVER_BLUE.button_color(sdSysData, "Blue", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("ob");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_OVER_PURPLE.button_color(sdSysData, "Purple", RAS_PURPLE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("ou");
            DISPLAY_OVERHEAD_COLOR = false;
          }
          
          if (BT_OVER_YELLOW.button_color(sdSysData, "Yellow", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("oy");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_OVER_CYAN.button_color(sdSysData, "Cyan", RAS_CYAN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("oc");
            DISPLAY_OVERHEAD_COLOR = false;
          }
          
          if (BT_OVER_ORANGE.button_color(sdSysData, "Orange", RAS_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("on");
            DISPLAY_OVERHEAD_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_OVER_WHITE.button_color(sdSysData, "White", RAS_WHITE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("ow");
            DISPLAY_OVERHEAD_COLOR = false;
          }
        }
        ImGui::End();
      }

      // ---------------------------------------------------------------------------------------
      // Overhead Color Window
      
      if (DISPLAY_RUNNING_COLOR == true && RESTACK_WINDOWS == false)
      {
        ImGui::SetNextWindowSize(ImVec2(sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.x * 2.5f, 
                                        sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM.y * 7.0f));
        if (ImGui::Begin("Running Color", &DISPLAY_RUNNING_COLOR, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
        {
          if (BT_RUNNING_RED.button_color(sdSysData, "Red", RAS_RED, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rr");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_RUNNING_GREEN.button_color(sdSysData, "Green", RAS_GREEN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rg");
            DISPLAY_RUNNING_COLOR = false;
          }

          if (BT_RUNNING_BLUE.button_color(sdSysData, "Blue", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rb");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_RUNNING_PURPLE.button_color(sdSysData, "Purple", RAS_PURPLE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("ru");
            DISPLAY_RUNNING_COLOR = false;
          }
          
          if (BT_RUNNING_YELLOW.button_color(sdSysData, "Yellow", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("ry");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_RUNNING_CYAN.button_color(sdSysData, "Cyan", RAS_CYAN, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rc");
            DISPLAY_RUNNING_COLOR = false;
          }
          
          if (BT_RUNNING_ORANGE.button_color(sdSysData, "Orange", RAS_ORANGE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rn");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_RUNNING_BLACK.button_color(sdSysData, "Mono", RAS_MONOCHROME, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rx");
            DISPLAY_RUNNING_COLOR = false;
          }
          
          if (BT_RUNNING_GREY.button_color(sdSysData, "Grey", RAS_GREY, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("re");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();
          
          if (BT_RUNNING_WHITE.button_color(sdSysData, "White", RAS_WHITE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rw");
            DISPLAY_RUNNING_COLOR = false;
          }

          if (BT_RUNNING_PINK.button_color(sdSysData, "Pink", RAS_PINK, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rp");
            DISPLAY_RUNNING_COLOR = false;
          }

          ImGui::SameLine();

          if (BT_RUNNING_TEMP.button_color(sdSysData, "Temp", RAS_MONOCHROME, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_MEDIUM))
          {
            sdSysData.SCREEN_COMMS.command_text_set("rt");
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

        if (CONFIRM.confirm_dialog(sdSysData, choice))
        {
          if (choice)
          {
            sdSysData.SCREEN_COMMS.command_pending_send();
            DISPLAY_CONFIRM = false;
          }
          else
          {
            DISPLAY_CONFIRM = false;
          }
        }
      }

      // ---------------------------------------------------------------------------------------
      // Display Advert

      ADVERTISEMENTS.draw(sdSysData);

      // ---------------------------------------------------------------------------------------
      // Alert Windows

      if (RESTACK_WINDOWS == false)
      {
        AUTOMOBILE_ALERTS.draw(sdSysData, sdSysData.ALL_ALERTS);
        for (int alert = 0; alert < (int)ADSB_ALERTS.size(); alert++)
        {
          ADSB_ALERTS[alert].draw(sdSysData, sdSysData.AIRCRAFT_COORD.AIRCRAFTS_MAP.AIRCRAFT_DETAIL_LIST[alert].ALERTS_ADSB);
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
      
      if (sdSysData.PANEL_CONTROL.DISPLAY_COMPASS_WINDOW == true)
      {
        if (sdSysData.COMMS_COMPASS.connected())
        {
          ImGui::SetNextWindowSize(ImVec2(140.0f * DEF_SCREEN_SIZE_X_MULTIPLIER, 210.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER));

          ImGui::Begin("Compass", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
          {
            ImVec4 working_area = get_working_area();

            ImDrawList* draw_list_status_compass_window = ImGui::GetWindowDrawList();

            COMPASS_WINDOW_COMPASS.draw(draw_list_status_compass_window, sdSysData, 2, 
                        ImVec2(working_area.x + (working_area.z / 2.0f), 
                        working_area.y + (working_area.w / 2.0f)), 
                        working_area.z / 2.0f * 0.6f, true, sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_GPS_FIX, 
                        sdSysData.GPS_SYSTEM.CURRENT_POSITION.VALID_TRACK, sdSysData.GPS_SYSTEM.CURRENT_POSITION.TRUE_HEADING.VALUE, 
                        sdSysData.COMMS_COMPASS.connected(), sdSysData.COMMS_COMPASS.bearing_calibrated(), false, 
                        true, sdSysData.COMMS_COMPASS.bearing_jitter_min(), sdSysData.COMMS_COMPASS.bearing_jitter_max(), 0.0f);

            if (button_area(working_area))
            {
              sdSysData.PANEL_CONTROL.DISPLAY_COMPASS_WINDOW = false;
            }
          }
          ImGui::End();
        }
      }

      // ---------------------------------------------------------------------------------------
      
      if (DISPLAY_CARDS_WINDOW == true)
      {
          ImGui::SetNextWindowSize(ImVec2(90.0f * DEF_SCREEN_SIZE_X_MULTIPLIER, 195.0f * DEF_SCREEN_SIZE_Y_MULTIPLIER));

          ImGui::Begin("Cards", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
          {
            if (BT_QR_CODE.button_color(sdSysData, "QR\nCODE", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_QR_CODE = !DISPLAY_QR_CODE;
              DISPLAY_CARDS_WINDOW = false;
            } 

            if (BT_REFER.button_color(sdSysData, "REFER", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
            {
              DISPLAY_REFERENCE_CARD = !DISPLAY_REFERENCE_CARD;
              DISPLAY_CARDS_WINDOW = false;
            } 

            ImGui::End();
          }
      }
    }
    
    // ---------------------------------------------------------------------------------------

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    //ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //glUseProgram(last_program);

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);

  }
  else
  {
    // TTY SCREEN
    // Handle Console Iputs
    if (sdSysData.SCREEN_COMMS.printw_q_avail() == true)
    {
      printf("%s", sdSysData.SCREEN_COMMS.printw_q_get().c_str());
      
      if (COMMAND_TEXT != "")
      {
        COMMAND_TEXT_CHANGED = true;
      }
    }

    // Clear
    if (sdSysData.SCREEN_COMMS.command_text_clear_get() == true)
    {
      COMMAND_TEXT = "";
      COMMAND_TEXT_CHANGED = true;
    }

    if (COMMAND_TEXT_CHANGED)
    {
      string command_display = left_justify((int)TERMINAL_WINDOW.ws_col, "CMD: " + COMMAND_TEXT) + "" + 
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

        character_enter(sdSysData, character_pressed, shift_pressed, backspace_pressed, enter_pressed);
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
    //ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
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

//int SCREEN4::get_current_screen()
//{
//  return DISPLAY_SCREEN;
//}

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
    DAEMON_LOG.add_line_with_indent(Text);
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



// ---------------------------------------------------------------------------------------
#endif