#include "PSF1Font.h"
#include "VGABuffer.hpp"

void putChar(FrameBuffer *frameBuffer, PSF1Font *fontBuffer, uint32_t color,
             char character, uint32_t xOff, uint32_t yOff) {
    uint32_t *pixPtr = (uint32_t *)frameBuffer->baseAddr;
    char *fontPtr = (char *)fontBuffer->glyphBuffer +
                    (character * fontBuffer->header->characterSize);
    for (uint32_t y = yOff; y < yOff + 16; ++y) {
        for (uint32_t x = xOff; x < xOff + 8; ++x) {
            if ((*fontPtr & (0b10000000) >> (x - xOff)) > 0) {
                *(uint32_t *)(pixPtr + x +
                              (y * frameBuffer->pixelsPerScanline)) = color;
            }
        }
        ++fontPtr;
    }
}
extern "C" int _start(FrameBuffer *frameBuffer, PSF1Font *fontBuffer) {
    putChar(frameBuffer, fontBuffer, 0xffffffff, 'G', 10, 10);
    // uint32_t y = 50;
    // uint8_t bbp = 4;
    // for (size_t x = 0; x < frameBuffer->width / 2 * bbp; x += bbp) {
    //     size_t offset = x + (y * frameBuffer->pixelsPerScanline * bbp);
    //     *(uint32_t *)(frameBuffer->baseAddr + offset) = 0xffffffff;
    // }
    return 123;
}
