#include "BasicRenderer.hpp"

uint8_t BasicRenderer::bytesPerPixel = 4;

BasicRenderer::BasicRenderer(FrameBuffer *frameBuffer, PSF1Font *fontBuffer,
                             uint32_t x, uint32_t y, RGBA color)
    : m_frame(frameBuffer), m_font(fontBuffer), m_cursor(x, y, color) {}

void BasicRenderer::putChar(char character, uint32_t xOff, uint32_t yOff,
                            RGBA foreground, RGBA background) {
    uint8_t charSize = m_font->header->characterSize;
    uint32_t *pixPtr = (uint32_t *)m_frame->baseAddr;
    char *fontPtr = (char *)m_font->glyphBuffer + (character * charSize);
    for (uint32_t y = yOff; y < yOff + charSize; ++y) {
        for (uint32_t x = xOff; x < xOff + 8; ++x) {
            uint32_t color;
            if ((*fontPtr & (0b10000000) >> (x - xOff)) > 0) {
                color = (uint32_t)foreground;
            } else {
                color = (uint32_t)background;
            }
            *(uint32_t *)(pixPtr + x + (y * m_frame->pixelsPerScanline)) =
                color;
        }
        ++fontPtr;
    }
}

void BasicRenderer::print(const char *str) {
    const char *ch = str;
    while (*ch != '\0') {
        putChar(*ch, m_cursor.x, m_cursor.y, m_cursor.foregoundColor,
                m_cursor.backgoundColor);
        m_cursor.x += 8;
        if (m_cursor.x + 8 > m_frame->width) {
            m_cursor.x = 0;
            m_cursor.y += m_font->header->characterSize;
        }
        ++ch;
    }
}

void BasicRenderer::clearScanline(uint32_t y) {
    uint64_t bytePerScanline = m_frame->pixelsPerScanline * bytesPerPixel;
    char *pixPtrBase = (char *)m_frame->baseAddr + (bytePerScanline * y);
    for (uint32_t x = 0; x < bytePerScanline; x += bytesPerPixel) {
        *(uint32_t *)(pixPtrBase + x) = 0;
    }
}

void BasicRenderer::clearScreen() {
    for (uint32_t y = 0; y < m_frame->height; ++y) {
        clearScanline(y);
    }
}

void BasicRenderer::setColor(RGBA foreground, RGBA background) {
    m_cursor.foregoundColor = foreground;
    m_cursor.backgoundColor = background;
}
