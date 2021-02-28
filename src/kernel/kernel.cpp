#include "VGABuffer.hpp"

extern "C" int _start(FrameBuffer *frameBuffer) {
    uint32_t y = 50;
    uint8_t bbp = 4;
    for (size_t x = 0; x < frameBuffer->width / 2 * bbp; x += bbp) {
        size_t offset = x + (y * frameBuffer->pixelsPerScanline * bbp);
        *(uint32_t *)(frameBuffer->baseAddr + offset) = 0xffffffff;
    }
    return 123;
}
