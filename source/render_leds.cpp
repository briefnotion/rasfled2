#ifndef RENDER_LEDS_CPP
#define RENDER_LEDS_CPP

#include "render_leds.h"



// ***************************************************************************************
// The Following chunk of code is what handles the imported library's ability to put the
// lights on and off or whatever.  Will not pretend to understand it.  Instead, will
// squish it down as much as possible so that I can pretend its not there.
// ***************************************************************************************


void RENDER_LEDS_CLASS::matrix_render(int led_count)
{
    int x;

    for (x = 0; x < led_count; x++)
    {
        ledstring.channel[0].leds[x] = matrix[x];
    }
}

void RENDER_LEDS_CLASS::ctrl_c_handler(int signum)
{
  (void)(signum);
    //running = 0;
}

void RENDER_LEDS_CLASS::setup_handlers(void)
{
    struct sigaction sa;
      sa.sa_handler = ctrl_c_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void RENDER_LEDS_CLASS::ledprep(ws2811_t *ws2811, int led_count)
{
  ws2811->channel[0].count = led_count;
}

void RENDER_LEDS_CLASS::shutdown()
{
    // Shutdown the LED strip routine.
    ws2811_fini(&ledstring);
}

// ---------------------------------------------------------------------------------------
// Global function for Main Loop Threads
// By passing the global variable, difficult to work with, ledstring to the, just as
//  difficult to work with, ws2811_render routine, all led and values will be transmitted
//  to the lights on a seperate thread.
void RENDER_LEDS_CLASS::proc_render_thread()
{
    //int ret = 0;  // contains fail or pass status of the render routine.
    //ret = ws2811_render(&ledstring);  // Send values of ledstring to hardware.
    ws2811_render(&ledstring);  // Send values of ledstring to hardware.
}

int RENDER_LEDS_CLASS::create(system_data &sdSysData)
{
    int return_code = 0;

    // ---------------------------------------------------------------------------------------
    // LED Library Vars and Init

    sdSysData.WS2811_ENABLED = DEF_WS2811_ENABLED;
    LED_COUNT = sdSysData.CONFIG.LED_MAIN.at(0).led_count();
    CRGB_ARRAY_BUFFER = new CRGB[LED_COUNT];

    if (sdSysData.WS2811_ENABLED)
    {
        sdSysData.SCREEN_COMMS.printw("Initializing LEDS ...");

        ledstring.freq = TARGET_FREQ;
        ledstring.dmanum = DMA;
        ledstring.channel[0].gpionum = GPIO_PIN;
        ledstring.channel[0].count = LED_COUNT;
        ledstring.channel[0].brightness = 255;
        ledstring.channel[0].invert = 0;
        ledstring.channel[0].strip_type = STRIP_TYPE;

        ws2811_return_t ret;
        ledprep(&ledstring, LED_COUNT);
        matrix = (int*)malloc(sizeof(ws2811_led_t) * LED_COUNT);
        setup_handlers();
        if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
        {
            cout << "WS2811_INIT failed: " << ws2811_get_return_t_str(ret) << endl;
        sdSysData.SCREEN_COMMS.printw("");
        sdSysData.SCREEN_COMMS.printw("FAILURE:");
        //std::cerr << "ws2811_init failed: " << ws2811_get_return_t_str(ret) << std::endl;
        string return_description = ws2811_get_return_t_str(ret);
        sdSysData.SCREEN_COMMS.printw("     ws2811_init failed: " + return_description);
        sdSysData.SCREEN_COMMS.printw("");

        return_code = (int)ret;

        sdSysData.WS2811_ENABLED = false;
        }
        else
        {
            cout << "WS2811_INIT SUCCESS" << endl;
        sdSysData.SCREEN_COMMS.printw("  WS2811 INIT SUCCESS");
        sdSysData.SCREEN_COMMS.printw("  LED count: " + to_string(LED_COUNT));
        }
    }

    return return_code;
}

void RENDER_LEDS_CLASS::thread_main()
{
    TIMED_IS_READY  frame_limit;     // Controls sleep time
    FLED_TIME thread_time;           // Thread gets its own Time 
    thread_time.create();

    CLOSED = false;
    RUN = true;
    while (RUN)
    {
        // prepare thread
        thread_time.setframetime();
        frame_limit.set(thread_time.current_frame_time(), INTERVAL);

        // MAIN THREAD ROUTINE GOES HERE
        if (CRGB_ARRAY_BUFFER_CHANGED == true)
        {
            PROCESSING = true;

            // move the values from the buffer to the display matrix and send to the leds.
            for (int lcount = 0; lcount < LED_COUNT; lcount++)
            {
                matrix[lcount]=CRGB_ARRAY_BUFFER[lcount].b + (CRGB_ARRAY_BUFFER[lcount].g << 8) + (CRGB_ARRAY_BUFFER[lcount].r << 16) + (0 << 24);
            }

            matrix_render(LED_COUNT);
            proc_render_thread();

            CRGB_ARRAY_BUFFER_CHANGED = false;
            PROCESSING = false;
        }

        //sleep thread
        thread_time.request_ready_time(frame_limit.get_ready_time());
        thread_time.sleep_till_next_frame();
    }

    CLOSED = true;

    shutdown();
    std::cout << "LED RENDER Thread Ended" << std::endl;
}

void RENDER_LEDS_CLASS::thread_start()
{
    {
        THREAD_CONTROL.create(1000);
        // Start the camera update on a separate thread.
        // This call is non-blocking, so the main loop can continue immediately.
        THREAD_CONTROL.start_render_thread([&]() 
                  {  thread_main();  });
    }
}

void RENDER_LEDS_CLASS::thread_stop()
{
    while (CLOSED == false)
    {
        RUN = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void RENDER_LEDS_CLASS::prepare_matrix(deque<v_profile_strip_main>& LED_Section)
{
    if (PROCESSING == false)
    {
        int mcount = 0;
        // Copy the prepared Matrixes to the display matrix
        //if((cons.keywatch.getnoreset(KEYDEBUG) == 0) || (cons.keywatch.get(KEYLEDDRCYCL) == 0))
        {
            for(int group=0; group < LED_Section.at(0).g_size(); group++)
            {
                for(int strip=0; strip < LED_Section.at(0).s_size(group); strip++)
                {
                    for (int lcount = 0; lcount < LED_Section.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).led_count(); lcount++)
                    {
                        // Normal Display
                        CRGB_ARRAY_BUFFER[mcount] = LED_Section.at(0).vLED_GROUPS.at(group).vLED_STRIPS.at(strip).crgbARRAY[lcount];
                        mcount++;
                    }
                }
            }
        }
        CRGB_ARRAY_BUFFER_CHANGED = true;
    }
    else
    {
        //cout << "*" << flush; // Frame skipped.
    }
}



#endif