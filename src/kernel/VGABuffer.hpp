#ifndef VGA_BUFFER_HPP
#define VGA_BUFFER_HPP

#include "FrameBuffer.h"
#include "PSF1Font.h"

#define BBP 4
struct Cursor {
    uint32_t x;
    uint32_t y;
    uint32_t color;
    Cursor(uint32_t x = 0, uint32_t y = 0, uint32_t color = 0)
        : x(x), y(y), color(color) {}
};

class VGABuffer {
   public:
    VGABuffer(FrameBuffer* frameBuffer, PSF1Font* fontBuffer, uint32_t x = 0,
              uint32_t y = 0, uint32_t color = 0xffffffff);

    void putChar(uint32_t color, char character, uint32_t xOff, uint32_t yOff);

    void print(const char* str);

    void clearScreen();

   private:
    void clearScanline(uint32_t y);
    FrameBuffer* m_frame;
    PSF1Font* m_font;
    Cursor m_cursor;
};

#endif
