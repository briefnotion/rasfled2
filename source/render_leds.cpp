#ifndef RENDER_LEDS_CPP
#define RENDER_LEDS_CPP

#include "render_leds.h"

void WS2812Spi::show_internal() 
{
    struct spi_ioc_transfer tr;
    std::memset(&tr, 0, sizeof(tr));
    
    tr.tx_buf = (unsigned long)alignedBuf;
    tr.len = totalSize; 
    tr.speed_hz = speed;
    tr.bits_per_word = 8;
    tr.cs_change = 1; // Signal a clear end-of-message to hardware

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) 
    {
        // If this fails, verify cat /sys/module/spidev/parameters/bufsiz
    }
}

void WS2812Spi::setRealTimePriority() 
{
    struct sched_param param;
    param.sched_priority = 80; 
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) 
    {
        std::cerr << "Note: RT Priority failed. Run with sudo." << std::endl;
    }
}

void WS2812Spi::show(const std::vector<uint32_t>& colors) 
{
    if (fd < 0 || is_busy.exchange(true)) return;

    // Pointer to the data section (after the leading zeros)
    uint8_t* startPtr = alignedBuf + PADDING;
    size_t bufIdx = 0;
    int count = std::min(ledCount, (int)colors.size());

    for (int i = 0; i < count; ++i) 
    {
        uint32_t color = colors[i];
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        // WS2812 expects GRB
        uint8_t channels[3] = {g, r, b};
        for (int c = 0; c < 3; c++) 
        {
            uint8_t ch = channels[c];
            
            // Explicitly unrolled for maximum timing consistency
            startPtr[bufIdx++] = (ch & 0x80) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x40) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x20) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x10) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x08) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x04) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x02) ? PATTERN_1 : PATTERN_0;
            startPtr[bufIdx++] = (ch & 0x01) ? PATTERN_1 : PATTERN_0;
        }
    }

    // Fill remaining data area with black if necessary
    if (bufIdx < dataSize) 
    {
        memset(startPtr + bufIdx, PATTERN_0, dataSize - bufIdx);
    }

    show_internal();
    is_busy.store(false);
}

// ----

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

// ----

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

            {
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
        //cout << "*" << flush; // Frame skipped.
    }
}

#endif