#ifndef RENDER_LEDS_H
#define RENDER_LEDS_H

#include <chrono>
#include <atomic>
#include <thread>

// External Library for controlling the WS2811 LEDs.  This is the library that does the actual work of putting the lights on and off.  It is a C library, so it is not object oriented.  It is also not very friendly to work with, so I have wrapped it in a class that makes it easier to use.  
#include "../../rpi_ws281x/ws2811.h"

// RASFled related header files
#include "definitions.h"
#include "system.h"
#include "threading.h"
#include "fled_time.h"
#include "LEDstuff.h"

class RENDER_LEDS_CLASS
{
    private:
        // The ancient functions
        // ***************************************************************************************
        // The Following chunk of code is what handles the imported library's ability to put the 
        // lights on and off or whatever.  Will not pretend to understand it.  Instead, will 
        // squish it down as much as possible so that I can pretend its not there.
        // ***************************************************************************************
        ws2811_t ledstring;
        int *matrix;
        //static char running;
        void matrix_render(int led_count);
        static void ctrl_c_handler(int signum);
        void setup_handlers(void);
        void ledprep(ws2811_t *ws2811, int led_count);

        // Threading things.
        THREADING_INFO  THREAD_CONTROL;  // Controls: update_frame_thread()
        unsigned long long INTERVAL = 1; // Time in milliseconds between each thread loop iteration.
        bool RUN = false;
        atomic<bool> CLOSED{false};

        // Buffer for the LED colors to be rendered.
        int LED_COUNT = 0;
        CRGB* CRGB_ARRAY_BUFFER = nullptr;
        bool CRGB_ARRAY_BUFFER_CHANGED = false;

        void shutdown();
        void proc_render_thread();

    public:
        atomic<bool> PROCESSING{false}; // Atomic flag to indicate if the thread is currently processing its main routine.
        int create(system_data &sdSysData);
        void thread_main();
        void thread_start();
        void thread_stop();
        void prepare_matrix(deque<v_profile_strip_main>& LED_Section);
};




#endif