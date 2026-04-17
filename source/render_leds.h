#ifndef RENDER_LEDS_H
#define RENDER_LEDS_H

#include <atomic>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <csignal>
#include <sched.h>
#include <sys/mman.h>
#include <malloc.h>
#include <cstring> // Required for memset

// RASFled related header files
#include "definitions.h"
#include "system.h"
#include "threading.h"
#include "fled_time.h"
#include "LEDstuff.h"


/**
 * @brief WS2812Spi Class - Thread-Safe Double-Buffered Version
 * Designed for heavily threaded applications. Uses two separate 
 * DMA-aligned buffers to prevent "tearing" or flickering when 
 * the logic thread updates colors while the SPI hardware is sending.
 */
class WS2812Spi {
private:
    int fd;
    int ledCount;
    uint32_t speed = 4000000; 

    // 4MHz Timing: 1 bit = 250ns
    // '0': 1 high bit (250ns), 3 low (750ns) -> 0x80
    // '1': 2 high bits (500ns), 2 low (500ns) -> 0xC0
    const uint8_t PATTERN_0 = 0x80; 
    const uint8_t PATTERN_1 = 0xC0; 
    
    // Double buffering to prevent thread contention
    uint8_t* bufA;
    uint8_t* bufB;
    uint8_t* activeWriteBuf;
    uint8_t* activeReadBuf;
    
    size_t bufSize;
    const size_t MAX_KERNEL_BUF = 4096;

    // Timing tracking to ensure we don't release the thread too early
    std::chrono::steady_clock::time_point last_write_time;
    double transfer_duration_ms;

    // Static FD to keep DMA latency request alive for the life of the process
    static int pm_latency_fd;

public:
    WS2812Spi(int count, const char* device = "/dev/spidev0.0") : fd(-1), ledCount(count) {
        fd = open(device, O_RDWR | O_DSYNC);
        if (fd < 0) {
            perror("Failed to open SPI device");
            return;
        }

        uint8_t mode = SPI_MODE_0;
        ioctl(fd, SPI_IOC_WR_MODE, &mode);
        ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

        // Calculate size: 24 bytes per LED + 2KB reset padding
        bufSize = (ledCount * 24) + 2048;
        
        // Duration = (Total Bits * 250ns per bit) converted to ms
        transfer_duration_ms = (bufSize * 8.0 * 0.00025);

        // Allocate two page-aligned buffers
        if (posix_memalign((void**)&bufA, 4096, bufSize) != 0) return;
        if (posix_memalign((void**)&bufB, 4096, bufSize) != 0) return;
        
        memset(bufA, 0x00, bufSize);
        memset(bufB, 0x00, bufSize);

        mlock(bufA, bufSize);
        mlock(bufB, bufSize);

        activeWriteBuf = bufA;
        activeReadBuf = bufB;

        std::cout << "SPI Thread-Safe Initialized (" << ledCount << " LEDs)" << std::endl;
    }

    ~WS2812Spi() { 
        if (fd >= 0) {
            memset(activeWriteBuf, 0x00, bufSize);
            show_internal(activeWriteBuf);
            munlock(bufA, bufSize);
            munlock(bufB, bufSize);
            free(bufA);
            free(bufB);
            close(fd); 
        }
    }

    bool isValid() const { return fd >= 0; }

    /**
     * @brief Prepares and sends the color data.
     */
    void show(const std::vector<uint32_t>& colors) {
        if (fd < 0) return;

        memset(activeWriteBuf, 0x00, bufSize);

        size_t bufIdx = 0;
        size_t limit = std::min((size_t)ledCount, colors.size());

        for (size_t i = 0; i < limit; ++i) {
            uint32_t color = colors[i];
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            uint8_t grb[3] = {g, r, b};

            for (int j = 0; j < 3; ++j) {
                uint8_t ch = grb[j];
                activeWriteBuf[bufIdx++] = (ch & 0x80) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x40) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x20) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x10) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x08) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x04) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x02) ? PATTERN_1 : PATTERN_0;
                activeWriteBuf[bufIdx++] = (ch & 0x01) ? PATTERN_1 : PATTERN_0;
            }
        }
        
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_write_time).count();
        if (elapsed < transfer_duration_ms) {
            std::this_thread::sleep_for(std::chrono::microseconds((int)((transfer_duration_ms - elapsed) * 1000)));
        }

        uint8_t* swap = activeReadBuf;
        activeReadBuf = activeWriteBuf;
        activeWriteBuf = swap;

        show_internal(activeReadBuf);
        last_write_time = std::chrono::steady_clock::now();
    }

private:
    void show_internal(uint8_t* data) {
        size_t sent = 0;
        while (sent < bufSize) {
            size_t to_send = std::min(MAX_KERNEL_BUF, bufSize - sent);
            if (write(fd, data + sent, to_send) < 0) break;
            sent += to_send;
        }
    }
};

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