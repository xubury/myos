#include "VGABuffer.hpp"

VGABuffer::VGABuffer(FrameBuffer *frameBuffer, PSF1Font *fontBuffer, uint32_t x,
                     uint32_t y, uint32_t color)
    : m_frame(frameBuffer), m_font(fontBuffer), m_cursor(x, y, color) {}

void VGABuffer::putChar(uint32_t color, char character, uint32_t xOff,
                        uint32_t yOff) {
    uint8_t charSize = m_font->header->characterSize;
    uint32_t *pixPtr = (uint32_t *)m_frame->baseAddr;
    char *fontPtr = (char *)m_font->glyphBuffer + (character * charSize);
    for (uint32_t y = yOff; y < yOff + charSize; ++y) {
        for (uint32_t x = xOff; x < xOff + 8; ++x) {
            if ((*fontPtr & (0b10000000) >> (x - xOff)) > 0) {
                *(uint32_t *)(pixPtr + x + (y * m_frame->pixelsPerScanline)) =
                    color;
            }
        }
        ++fontPtr;
    }
}

void VGABuffer::print(const char *str) {
    const char *ch = str;
    while (*ch != '\0') {
        putChar(m_cursor.color, *ch, m_cursor.x, m_cursor.y);
        m_cursor.x += 8;
        if (m_cursor.x + 8 > m_frame->width) {
            m_cursor.x = 0;
            m_cursor.y += m_font->header->characterSize;
        }
        ++ch;
    }
}

void VGABuffer::clearScanline(uint32_t y) {
    uint64_t bytePerScanline = m_frame->pixelsPerScanline * BBP;
    uint64_t pixPtrBase = (uint64_t)m_frame->baseAddr + (bytePerScanline * y);
    for (uint32_t *pixPtr = (uint32_t *)pixPtrBase;
         pixPtr < (uint32_t *)(pixPtrBase + bytePerScanline); ++pixPtr) {
        *pixPtr = 0;
    }
}

void VGABuffer::clearScreen() {
    for (uint32_t y = 0; y < m_frame->height; ++y) {
        clearScanline(y);
    }
}
