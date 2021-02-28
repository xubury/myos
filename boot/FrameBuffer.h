#ifndef UEFI_FRAME_BUFFER_H
#define UEFI_FRAME_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char *baseAddr;
    size_t bufferSize;
    uint32_t width;
    uint32_t height;
    uint32_t pixelsPerScanline;
} FrameBuffer;

#endif
