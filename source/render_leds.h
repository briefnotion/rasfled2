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

    void setRealTimePriority() {
        struct sched_param param;
        param.sched_priority = 80; 
        if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
            std::cerr << "Note: RT Priority failed. Run with sudo." << std::endl;
        }
    }

    void show(const std::vector<uint32_t>& colors) {
        if (fd < 0 || is_busy.exchange(true)) return;

        // Pointer to the data section (after the leading zeros)
        uint8_t* startPtr = alignedBuf + PADDING;
        size_t bufIdx = 0;
        int count = std::min(ledCount, (int)colors.size());

        for (int i = 0; i < count; ++i) {
            uint32_t color = colors[i];
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            // WS2812 expects GRB
            uint8_t channels[3] = {g, r, b};
            for (int c = 0; c < 3; c++) {
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
        if (bufIdx < dataSize) {
            memset(startPtr + bufIdx, PATTERN_0, dataSize - bufIdx);
        }

        show_internal();
        is_busy.store(false);
    }

private:
    void show_internal() {
        struct spi_ioc_transfer tr;
        std::memset(&tr, 0, sizeof(tr));
        
        tr.tx_buf = (unsigned long)alignedBuf;
        tr.len = totalSize; 
        tr.speed_hz = speed;
        tr.bits_per_word = 8;
        tr.cs_change = 1; // Signal a clear end-of-message to hardware

        if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
            // If this fails, verify cat /sys/module/spidev/parameters/bufsiz
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