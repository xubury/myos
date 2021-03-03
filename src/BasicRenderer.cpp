#include "BasicRenderer.hpp"

uint8_t BasicRenderer::bytesPerPixel = 4;

BasicRenderer::BasicRenderer() : m_frame(nullptr), m_font(nullptr) {}

void BasicRenderer::init(FrameBuffer *frameBuffer, PSF1Font *fontBuffer,
                         uint32_t x, uint32_t y, RGBA foreground,
                         RGBA background) {
    m_frame = frameBuffer;
    m_font = fontBuffer;
    m_cursor.x = x;
    m_cursor.y = y;
    m_cursor.foregoundColor = foreground;
    m_cursor.backgoundColor = background;
}

void BasicRenderer::setPixel(uint32_t x, uint32_t y, RGBA color) {
    *((uint32_t *)(m_frame->baseAddr +
                   bytesPerPixel * m_frame->pixelsPerScanline * y +
                   bytesPerPixel * x)) = (uint32_t)color;
}

RGBA BasicRenderer::getPixel(uint32_t x, uint32_t y) {
    return RGBA(*((uint32_t *)(m_frame->baseAddr +
                               bytesPerPixel * m_frame->pixelsPerScanline * y +
                               bytesPerPixel * x)));
}

void BasicRenderer::putChar(char character, uint32_t xOff, uint32_t yOff,
                            RGBA foreground, RGBA background) {
    uint8_t charSize = m_font->header->characterSize;
    char *fontPtr = (char *)m_font->glyphBuffer + (character * charSize);
    for (uint32_t y = yOff; y < yOff + charSize; ++y) {
        for (uint32_t x = xOff; x < xOff + 8; ++x) {
            if ((*fontPtr & (0b10000000) >> (x - xOff)) > 0) {
                setPixel(x, y, foreground);
            } else {
                setPixel(x, y, background);
            }
        }
        ++fontPtr;
    }
}

void BasicRenderer::print(const char *str) {
    for (const char *ch = str; *ch != '\0'; ++ch) {
        if (*ch == '\n') {
            newLine();
            continue;
        }
        putChar(*ch, m_cursor.x, m_cursor.y, m_cursor.foregoundColor,
                m_cursor.backgoundColor);
        m_cursor.x += 8;
        if (m_cursor.x + 8 > m_frame->width) {
            newLine();
        }
    }
}

void BasicRenderer::newLine() {
    m_cursor.x = 0;
    if (m_cursor.y + m_font->header->characterSize * 2 >= m_frame->height) {
        scrollDown();
    } else {
        m_cursor.y += m_font->header->characterSize;
    }
}

void BasicRenderer::scrollDown() {
    uint8_t charSize = m_font->header->characterSize;
    for (uint32_t y = 0; y < m_frame->height - charSize; ++y) {
        for (uint32_t x = 0; x < m_frame->pixelsPerScanline; ++x) {
            setPixel(x, y, getPixel(x, y + charSize));
        }
    }
    for (uint32_t y = m_frame->height - charSize; y < m_frame->height; ++y) {
        clearScanline(y, m_cursor.backgoundColor);
    }
}

void BasicRenderer::clearScanline(uint32_t y, RGBA color) {
    for (uint32_t x = 0; x < m_frame->pixelsPerScanline; ++x) {
        setPixel(x, y, color);
    }
}

void BasicRenderer::clearScreen(RGBA color) {
    for (uint32_t y = 0; y < m_frame->height; ++y) {
        clearScanline(y, color);
    }
}

void BasicRenderer::setColor(RGBA foreground, RGBA background) {
    m_cursor.foregoundColor = foreground;
    m_cursor.backgoundColor = background;
}
