#ifndef RENDER_LEDS_CPP
#define RENDER_LEDS_CPP

#include "render_leds.h"

uint32_t wheel_cycle(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) return ((255 - pos * 3) << 16) | (0 << 8) | (pos * 3);
    if (pos < 170) {
        pos -= 85;
        return (0 << 16) | ((pos * 3) << 8) | (255 - pos * 3);
    }
    pos -= 170;
    return ((pos * 3) << 16) | ((255 - pos * 3) << 8) | 0;
}

void RENDER_LEDS_CLASS::shutdown()
{
    // Shutdown the LED strip routine.
    //ws2811_fini(&ledstring);
}

int RENDER_LEDS_CLASS::create(system_data &sdSysData)
{
    int return_code = 0;

    // ---------------------------------------------------------------------------------------
    // LED Library Vars and Init

    sdSysData.LED_LIGHTS_ENABLED = DEF_LED_LIGHTS_ENABLED;
    LED_COUNT = sdSysData.CONFIG.LED_MAIN.at(0).led_count();
    CRGB_ARRAY_BUFFER = new CRGB[LED_COUNT];

    if (sdSysData.LED_LIGHTS_ENABLED)
    {
        cout << "WS2811_INIT SUCCESS" << endl;
        sdSysData.SCREEN_COMMS.printw("  LED LIGHTS INIT SUCCESS");
        sdSysData.SCREEN_COMMS.printw("  LED count: " + to_string(LED_COUNT));
        // Signal (DIN): Pin 19 (GPIO 10 / MOSI)
    }

    return return_code;
}

void RENDER_LEDS_CLASS::thread_main()
{
    TIMED_IS_READY  frame_limit;     // Controls sleep time
    FLED_TIME thread_time;           // Thread gets its own Time 
    thread_time.create();


    WS2812Spi strip(LED_COUNT);
    std::vector<uint32_t> colors(LED_COUNT);

    strip.setRealTimePriority();

    CLOSED = false;
    RUN = true;

    while (RUN)
    {
        // prepare thread
        thread_time.setframetime();
        frame_limit.set(thread_time.current_frame_time(), INTERVAL);

        if (CRGB_ARRAY_BUFFER_CHANGED == true)
        {
            PROCESSING = true;
            CRGB_ARRAY_BUFFER_CHANGED = false; // Mark as read immediately after snapshot

            // Method 2
            /*
            {
                // Create a local snapshot of the buffer so the animation thread 
                // can't change 'r', 'g', or 'b' while we are in the middle of packing.
                std::vector<CRGB> snapshot(LED_COUNT);
                std::memcpy(snapshot.data(), CRGB_ARRAY_BUFFER, LED_COUNT * sizeof(CRGB));
                
                for (int lcount = 0; lcount < LED_COUNT; lcount++) {
                    // Use the snapshot, not the shared buffer
                    uint32_t rVal = static_cast<uint8_t>(snapshot[lcount].r);
                    uint32_t gVal = static_cast<uint8_t>(snapshot[lcount].g);
                    uint32_t bVal = static_cast<uint8_t>(snapshot[lcount].b);
                    colors[lcount] = (rVal << 16) | (gVal << 8) | bVal;
                }
            }
            */

            // Method 1
            {
                // Direct packing without snapshot vector
                for (int lcount = 0; lcount < LED_COUNT; lcount++) {
                    uint32_t rVal = static_cast<uint8_t>(CRGB_ARRAY_BUFFER[lcount].r);
                    uint32_t gVal = static_cast<uint8_t>(CRGB_ARRAY_BUFFER[lcount].g);
                    uint32_t bVal = static_cast<uint8_t>(CRGB_ARRAY_BUFFER[lcount].b);
                    colors[lcount] = (rVal << 16) | (gVal << 8) | bVal;
                }
            }

            strip.show(colors);
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
        cout << "*" << flush; // Frame skipped.
    }
}



#endif