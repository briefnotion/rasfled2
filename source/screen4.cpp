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
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL2_Init();

  io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18.0f);

  return 0;
}

// ---------------------------------------------------------------------------------------

void SCREEN4::draw(system_data &sdSysData)
{


  // sdSysData.tmeCURRENT_FRAME_TIME


  // Handle Console Iputs
  if (SCREEN_COMMS.printw_q_avail() == true)
  {
    CONSOLE_TEXT = CONSOLE_TEXT + SCREEN_COMMS.printw_q_get();
    CONSOLE_SCROLL_TO_BOTTOM = true;
  }

  // Simple Check Size
  if (CONSOLE_TEXT.size() > 1024 * 5)
  {
    CONSOLE_TEXT.erase(0, CONSOLE_TEXT.find_first_of("\n", 512 ) +1 );
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

  if (ImGui::Begin("Window", &show_test_window, flags)) // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
  {
    ImGui::BeginChild("Main", ImVec2(ImGui::GetContentRegionAvail().x - 100, ImGui::GetContentRegionAvail().y), false, flags_c);
    {
      // ---------------------------------------------------------------------------------------
      // Status Sub Window

      ImGui::BeginChild("Status", ImVec2(ImGui::GetContentRegionAvail().x, 60), true, flags_c);
      {

        float region_div_4 = ImGui::GetContentRegionAvail().x / 4;

        // Divide sub window into 4
        // Left
        ImGui::BeginChild("Status Left", ImVec2(region_div_4, ImGui::GetContentRegionAvail().y), false, flags_c);
        {
          
          if (RESET_KEYBOARD_FOCUS == true)
          {
            ImGui::SetKeyboardFocusHere();
            RESET_KEYBOARD_FOCUS = false;
          }

          ImGui::Text("CMD : ");

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

        ImGui::BeginChild("Status Mid", ImVec2(region_div_4 * 2, ImGui::GetContentRegionAvail().y), false, flags_c);
        {
          // Display Lights Off mode toggle.
          ImGui::BeginGroup();
          {
            text_simple_bool("LIGHTS", sdSysData.Lights_On.value());
            if (SCREEN_COMMS.DEBUG_STATUS.DEBUG == true)
            {
              ImGui::Text("DEBUG");
            }
          }
          ImGui::EndGroup();
          
          ImGui::SameLine();

          // Status Group
          ImGui::BeginGroup();
          {
            text_simple_bool(" OVERHEAD ", sdSysData.booOverheadRunning);
            text_simple_bool("  HAZARD  ", sdSysData.booHazardRunning);
          }
          ImGui::EndGroup();

          ImGui::SameLine();

          // Status Group
          ImGui::BeginGroup();
          {
            // Lights
            if(sdSysData.Day_On_With_Override.value() == true)
            {
              text_simple_bool("  DAY  ", true);
            }
            else
            {
              text_simple_bool(" NIGHT ", false);
            }

            // Timer
            text_simple_bool(" TIMER ", sdSysData.cdTIMER.is_active());
          }
          ImGui::EndGroup();

          ImGui::SameLine();
          
          // Door Group
          ImGui::BeginGroup();
          {
            text_simple_bool(" DOOR2", sdSysData.CONFIG.vSWITCH_PIN_MAP.at(1).value);
            ImGui::SameLine();
            text_simple_bool("DOOR4 ", sdSysData.CONFIG.vSWITCH_PIN_MAP.at(3).value);

            text_simple_bool(" DOOR1", sdSysData.CONFIG.vSWITCH_PIN_MAP.at(0).value);
            ImGui::SameLine();
            text_simple_bool("DOOR3 ", sdSysData.CONFIG.vSWITCH_PIN_MAP.at(2).value);

          }
          ImGui::EndGroup();

          ImGui::SameLine();
          
          // Auto ADSB
          ImGui::BeginGroup();
          {
            text_simple_bool(" AUTO  ", sdSysData.CAR_INFO.active());
            text_simple_bool(" ADS-B ", sdSysData.AIRCRAFT_COORD.is_active());
          }
          ImGui::EndGroup();
          
        }
        ImGui::EndChild();

        // Mid Right
        ImGui::SameLine();

        ImGui::BeginChild("Status Right", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, flags_c);
        {
          ImGui::Text("Version:");
          ImGui::SameLine();
          ImGui::Text(Revision);

          if (sdSysData.hsHardware_Status.enabled() == true)
          {
            ImGui::Text(("Temp: " + to_string((int)sdSysData.hsHardware_Status.get_temperature()) + "c").c_str());
          }
          else
          {
            ImGui::Text("Temp: NA");
          }
        }
        ImGui::EndChild();
       
      }
      ImGui::EndChild();

      // ---------------------------------------------------------------------------------------
      // Console Sub Window

      ImGui::BeginChild("DISPLAY_SCREEN", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 48), false, flags_c);
      {
        // ImGui::Text("Hello from another window!");

        ImGui::SetNextWindowPos(ImGui::GetItemRectMin());
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

        if (DISPLAY_SCREEN == 0)
        {
          ImGui::Begin("Console", NULL, flags_w);
          {
            //static ImGuiWindowFlags flags_console = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoUndoRedo |
            //                                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
            //static ImGuiWindowFlags flags_console = ImGuiInputTextFlags_None;
            
            //CONSOLE_TEXT = CONSOLE_TEXT + to_string(ImGui::GetScrollMaxY()) + "\n";
            ImGui::TextUnformatted(CONSOLE_TEXT.c_str());
            if (CONSOLE_SCROLL_TO_BOTTOM == true && ImGui::GetScrollMaxY() > 0)
            {
              ImGui::SetScrollHereY(1.0f);
              CONSOLE_SCROLL_TO_BOTTOM = false;
            }
          }
          ImGui::End();
        }

        else if (DISPLAY_SCREEN == 3) // Create a window called "Hello, world!" and append into it.
        {
          ImGui::Begin("Hello, world!", NULL, flags_w);
          {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::BeginGroup();

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
              counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::EndGroup();

            ImGui::SameLine();
            if (ImGui::Button("Exit", ImVec2(50, 50)))
            {
              WINDOW_CLOSE = true;
            }
          }
          ImGui::End();
        }
        else
        {
          ImGui::Begin(" ", NULL, flags_w);
          {
            // Show Nothing
          }
          ImGui::End();
        }
      }
      ImGui::EndChild();

      // ---------------------------------------------------------------------------------------
      // Tabs Sub Window

      ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, flags_c);
      {

        if (button_simple_toggle_color("Console", "Console", DISPLAY_SCREEN == 0, -1, SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 0;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("Automobile", "Automobile", DISPLAY_SCREEN == 1, -1, SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 1;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("ADSB", "ADSB", DISPLAY_SCREEN == 2, -1, SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 2;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("Hello\nWorld", "Hello\nWorld", DISPLAY_SCREEN == 3, -1, SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 3;
        }

        ImGui::SameLine();

        if (button_simple_toggle_color("LOGS", "LOGS", DISPLAY_SCREEN == 4, -1, SIZE_BUTTON_TAB))
        {
          DISPLAY_SCREEN = 4;
        }

        /*
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
          if (ImGui::BeginTabItem("Console"))
          {
            DISPLAY_SCREEN = 0;
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Automobile"))
          {
            DISPLAY_SCREEN = 1;
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("ADSB"))
          {
            DISPLAY_SCREEN = 2;
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Hello World"))
          {
            DISPLAY_SCREEN = 3;
            ImGui::EndTabItem();
          }
          ImGui::EndTabBar();
        }
        */

      }
      ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // ---------------------------------------------------------------------------------------
    // Menus Sub Window

    ImGui::BeginChild("System Menu", ImVec2(0, ImGui::GetContentRegionAvail().y), false, flags_c);
    {
      // Main Menu
      if (DISPLAY_MENU == 0)
      {
        if (button_simple_toggle_color("Stop\nTimer", "Start\nTimer", sdSysData.cdTIMER.is_active(), H_GREEN, SIZE_BUTTON))
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

        button_simple_enabled(" ", false, SIZE_BUTTON);

        if (button_simple_toggle_color("Over\nHead\nLights", "Over\nHead\nLights", sdSysData.booOverheadRunning, H_RED, SIZE_BUTTON))
        {
          if (sdSysData.booOverheadRunning == true)
          {
            SCREEN_COMMS.command_text_set("o`");
          }
          else
          {
            SCREEN_COMMS.command_text_set("oo");
          }
        }

        if (button_simple_color("Flash", H_GREEN, SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set("ff");
        }

        button_simple_enabled(" ", false, SIZE_BUTTON);

        if (button_simple_toggle_color("Lights\n(On)", "Lights\n(Off)", sdSysData.Lights_On.value(), -1, SIZE_BUTTON))
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

        if (button_simple_color("...", H_BLUE, SIZE_BUTTON))
        {
          DISPLAY_MENU = 1;
        } 
      }
      else if (DISPLAY_MENU == 1)
      {
        if (button_simple_toggle_color("HAZARD\n(On)", "HAZARD\n(Off)", sdSysData.booHazardRunning, H_RED, SIZE_BUTTON))
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

        button_simple_enabled(" ", false, SIZE_BUTTON);

        button_simple_enabled(" ", false, SIZE_BUTTON);

        button_simple_enabled(" ", false, SIZE_BUTTON);

        if (button_simple_color("Clear\nAnims", H_RED, SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set("``");
        }

        if (button_simple_color("System", H_BLUE, SIZE_BUTTON))
        {
          DISPLAY_MENU = 2;
        }

        if (button_simple_color("<-", H_BLUE, SIZE_BUTTON))
        {
          DISPLAY_MENU = 0;
        }
      }
      else if (DISPLAY_MENU == 2)
      {
        if (button_simple_color("Exit", H_RED, SIZE_BUTTON))
        {
          WINDOW_CLOSE = true;
        }

        button_simple_enabled(" ", false, SIZE_BUTTON);

        if (button_simple_color("SYSTEM\nSHUT\nDOWN", H_RED, SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set(" comshutd");
        }

        if (button_simple_color("SYSTEM\nREBOOT", H_RED, SIZE_BUTTON))
        {
          SCREEN_COMMS.command_text_set(" reboot");
        }

        button_simple_enabled(" ", false, SIZE_BUTTON);

        if (button_simple_color("DEBUG", H_YELLOW, SIZE_BUTTON))
        {
          DISPLAY_DEBUG = !DISPLAY_DEBUG;
        }

        if (button_simple_color("<-", H_BLUE, SIZE_BUTTON))
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

  // Debug Window
  
  if (DISPLAY_DEBUG == true)
  {
    ImGui::SetNextWindowSize(ImVec2(270, 152));
    if (ImGui::Begin("Debug", &DISPLAY_DEBUG, flags_w_pop)) 
    {
      ImGui::Text("%.3f ms/frame  %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
      //ImGui::Text("%.1f FPS", io.Framerate);

      // Debug On Off
      ImGui::BeginGroup();
      {
        if (button_simple_toggle_color("Dug", "Dbug", SCREEN_COMMS.DEBUG_STATUS.DEBUG, H_RED, SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DEBUG = ! SCREEN_COMMS.DEBUG_STATUS.DEBUG;
        }
      }
      ImGui::EndGroup();

      ImGui::SameLine();

      // Doors
      ImGui::BeginGroup();
      {
        // Needs Better Spacing Alignment.

        // Door 2 (1)
        ImGui::SetNextItemWidth(43);
        ImGui::Text("%d", sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(1));
        ImGui::SameLine();
        if (button_simple_toggle_color("2", "2", SCREEN_COMMS.DEBUG_STATUS.DOOR[1], -1, SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[1] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[1];
        }
        ImGui::SameLine();
        
        // Door 4 (3)
        if (button_simple_toggle_color("4", "4", SCREEN_COMMS.DEBUG_STATUS.DOOR[3], -1, SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[3] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[3];
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(43);
        ImGui::Text("%d", sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(3));
        
        // Door 1 (0)
        ImGui::SetNextItemWidth(43);
        ImGui::Text("%d", sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(0));
        ImGui::SameLine();
        if (button_simple_toggle_color("1", "1", SCREEN_COMMS.DEBUG_STATUS.DOOR[0], -1, SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[0] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[0];
        }
        ImGui::SameLine();
        
        // Door 3 (2)
        if (button_simple_toggle_color("3", "3", SCREEN_COMMS.DEBUG_STATUS.DOOR[2], -1, SIZE_BUTTON_SMALL))
        {
          SCREEN_COMMS.DEBUG_STATUS.DOOR[2] = !SCREEN_COMMS.DEBUG_STATUS.DOOR[2];
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(43);
        ImGui::Text("%d", sdSysData.intCHANNEL_GROUP_EVENTS_COUNTS.at(2));
      }
      ImGui::EndGroup();
      
      ImGui::SameLine();
      
      // Close
      ImGui::BeginGroup();
      {
        button_simple_enabled(" ", false, SIZE_BUTTON_SMALL);
         
        if (ImGui::Button("X", SIZE_BUTTON_SMALL))
        {
          DISPLAY_DEBUG = false;
        }
      }
      ImGui::EndGroup();

    }
    ImGui::End();
  }

  if (DISPLAY_TIMER == true)
  {
    ImGui::SetNextWindowSize(ImVec2(80, 60));
    if (ImGui::Begin("Timer", &DISPLAY_TIMER, flags_w_pop)) 
    {

      long elaped_time = 0;
      unsigned long duration_time = 0;
      long remaining_time = 0;

      // Calculate
      duration_time = sdSysData.cdTIMER.duration();
      elaped_time = sdSysData.cdTIMER.elapsed_time(sdSysData.tmeCURRENT_FRAME_TIME);
      remaining_time = duration_time - elaped_time;

      // Display Timer

      //string mins = linemerge_right_justify(2, "00", to_string(millis_to_time_minutes(remaining_time)));
      //string secs = linemerge_right_justify(2, "00", to_string(millis_to_time_seconds(remaining_time)));

      ImGui::Text("%.2i:%.2i", millis_to_time_minutes(remaining_time), millis_to_time_seconds(remaining_time));

      /*
      Countdown_Timer.PROP.LABEL = "Timer: " + mins + ":" + secs + " ";
      Countdown_Timer.PROP.MAX_VALUE = duration_time;
      Countdown_Timer.PROP.POSY = 1;
      Countdown_Timer.PROP.POSX = 2;
      Countdown_Timer.update(duration_time - elaped_time, sdSysData.tmeCURRENT_FRAME_TIME);

      Countdown_Timer.draw(TIMER_PANEL, ScrStat.Needs_Refresh);
      TIMER_PANEL.draw(ScrStat.Needs_Refresh);
      tiTimer.draw(ScrStat.Needs_Refresh);
      */

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
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

// ---------------------------------------------------------------------------------------
#endif