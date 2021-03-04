#ifndef BASIC_RENDERER_HPP
#define BASIC_RENDERER_HPP

#include "FrameBuffer.h"
#include "PSF1Font.h"

struct RGBA {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : a(a), r(r), g(g), b(b) {}
    RGBA(uint32_t rgba) {
        uint8_t* p = (uint8_t*)&rgba;
        a = p[3];
        r = p[2];
        g = p[1];
        b = p[0];
    }

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
    Cursor(uint32_t x = 0, uint32_t y = 0, RGBA foregound = RGBA(255, 255, 255),
           RGBA background = RGBA(0, 0, 0))
        : x(x), y(y), foregoundColor(foregound), backgoundColor(background) {}
};

class BasicRenderer {
   public:
    static uint8_t bytesPerPixel;

    void init(FrameBuffer* frameBuffer, PSF1Font* fontBuffer, uint32_t x,
              uint32_t y, RGBA foreground, RGBA background);

    void setPixel(uint32_t x, uint32_t y, RGBA color);
    RGBA getPixel(uint32_t x, uint32_t y);

    void putChar(char character, uint32_t xOff, uint32_t yOff, RGBA foreground,
                 RGBA background);

    void print(const char* str);
    void newLine();
    void scrollDown();

    void clearScreen(RGBA color = RGBA());

    void setColor(RGBA foreground, RGBA background);

    void clearScanline(uint32_t y, RGBA color);

   private:
    friend class KernelManager;
    BasicRenderer();

    FrameBuffer* m_frame;
    PSF1Font* m_font;
    Cursor m_cursor;
};

#endif
