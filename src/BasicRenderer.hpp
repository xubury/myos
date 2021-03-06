#ifndef BASIC_RENDERER_HPP
#define BASIC_RENDERER_HPP

#include "FrameBuffer.h"
#include "Math.hpp"
#include "PSF1Font.h"

class BasicRenderer {
   public:
    static uint8_t bytesPerPixel;

    void init(FrameBuffer* frameBuffer, PSF1Font* fontBuffer, uint32_t x,
              uint32_t y, RGBA foreground, RGBA background);

    void setPixel(uint32_t x, uint32_t y, RGBA color);
    RGBA getPixel(uint32_t x, uint32_t y);

    void putChar(char character);
    void putChar(char character, uint32_t xOff, uint32_t yOff, RGBA foreground,
                 RGBA background);

    void backSapce(uint32_t cnt);

    void print(const char* str);
    void newLine();
    void scrollDown();

    void clearScreen(RGBA color = RGBA());

    void setColor(RGBA foreground, RGBA background);

    void setCurosr(uint32_t x, uint32_t y);

   private:
    void clearScanline(uint32_t y, RGBA color);
    friend class KernelManager;
    BasicRenderer();

    FrameBuffer* m_frame;
    PSF1Font* m_font;
    Cursor m_cursor;
};

#endif
