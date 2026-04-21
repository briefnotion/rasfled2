#ifndef RENDER_LEDS_H
#define RENDER_LEDS_H

#include <atomic>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <cstring>
#include <algorithm>
#include <pthread.h>

// RASFled related header files
#include "definitions.h"
#include "system.h"
#include "threading.h"
#include "fled_time.h"
#include "LEDstuff.h"


/**
 * @brief WS2812Spi - Stability Version
 * * Key fixes for "Shifted Colors" and "Incorrect Position":
 * 1. Padding: Adds leading/trailing zero-bytes to ensure a clean Reset signal.
 * 2. Single-Shot: Sends the entire frame in one ioctl call (requires bufsiz=65536).
 * 3. Bit-Accuracy: Unrolled bit packing to prevent timing jitter.
 */
class WS2812Spi {
private:
    int fd;
    int ledCount;
    //uint32_t speed = 2400000; // 4MHz = 250ns per bit
    //uint32_t speed = 3200000; // 4MHz = 250ns per bit
    uint32_t speed = 4000000; // 4MHz = 250ns per bit

    const uint8_t PATTERN_0 = 0x80; // T0H: 1 bit high (~250ns), 7 bits low
    const uint8_t PATTERN_1 = 0xF0; // T1H: 4 bits high (~1000ns), 4 bits low
    
    uint8_t* alignedBuf;
    size_t dataSize;   // Bytes used for LED data
    size_t totalSize;  // Total bytes including padding
    
    const size_t PADDING = 1024; // 1KB of silence (zeros)

    std::atomic<bool> is_busy{false};

    void show_internal();

public:
    WS2812Spi(int count, const char* device = "/dev/spidev0.0") : fd(-1), ledCount(count) {
        fd = open(device, O_RDWR);
        if (fd < 0) {
            perror("Failed to open SPI device");
            return;
        }

        uint8_t mode = SPI_MODE_0;
        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) perror("SPI mode");
        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) perror("SPI speed");

        dataSize = ledCount * 24;
        totalSize = dataSize + (PADDING * 2); 
        
        // Allocate page-aligned memory for DMA efficiency
        if (posix_memalign((void**)&alignedBuf, 4096, totalSize) != 0) {
            std::cerr << "Failed to allocate memory" << std::endl;
            return;
        }
        
        memset(alignedBuf, 0x00, totalSize);
        mlock(alignedBuf, totalSize);
    }

    ~WS2812Spi() { 
        if (fd >= 0) {
            while(is_busy.load());
            memset(alignedBuf, 0x00, totalSize);
            show_internal();
            munlock(alignedBuf, totalSize);
            free(alignedBuf);
            close(fd); 
        }
    }

    void setRealTimePriority();
    void show(const std::vector<uint32_t>& colors);
};

// ----

uint32_t wheel_cycle(uint8_t pos);

// ----

class RENDER_LEDS_CLASS
{
    private:
        // Threading things.
        THREADING_INFO  THREAD_CONTROL;  // Controls: update_frame_thread()
        unsigned long long INTERVAL = 1; // Time in milliseconds between each thread loop iteration.
        bool RUN = false;
        atomic<bool> CLOSED{false};

        // Buffer for the LED colors to be rendered.
        int LED_COUNT = 0;
        CRGB* CRGB_ARRAY_BUFFER = nullptr;
        atomic<bool> CRGB_ARRAY_BUFFER_CHANGED = false;

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