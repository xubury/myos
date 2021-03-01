#ifndef BASIC_RENDERER_HPP
#define BASIC_RENDERER_HPP

#include "FrameBuffer.h"
#include "PSF1Font.h"

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}

    explicit operator uint32_t() {
        uint32_t rgba;
        uint8_t* p = (uint8_t*)&rgba;
        p[3] = a;
        p[2] = r;
        p[1] = g;
        p[0] = b;
        return rgba;
    }
};

struct Cursor {
    uint32_t x;
    uint32_t y;
    RGBA foregoundColor;
    RGBA backgoundColor;
    Cursor(uint32_t x = 0, uint32_t y = 0, RGBA foregound = RGBA(),
           RGBA background = RGBA(0, 0, 0))
        : x(x), y(y), foregoundColor(foregound), backgoundColor(background) {}
};

class BasicRenderer {
   public:
    static uint8_t bytesPerPixel;

    BasicRenderer(FrameBuffer* frameBuffer, PSF1Font* fontBuffer,
                  uint32_t x = 0, uint32_t y = 0,
                  RGBA color = RGBA(255, 255, 255));

    void putChar(char character, uint32_t xOff, uint32_t yOff, RGBA foreground,
                 RGBA background);

    void print(const char* str);
    void printNewLine();

    void clearScreen();

    void setColor(RGBA foreground, RGBA background);

   private:
    void clearScanline(uint32_t y);
    FrameBuffer* m_frame;
    PSF1Font* m_font;
    Cursor m_cursor;
};

#endif
