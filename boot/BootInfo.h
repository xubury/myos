#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include <efi.h>

#include "FrameBuffer.h"
#include "PSF1Font.h"

typedef struct {
    FrameBuffer* frameBuffer;
    PSF1Font* psf1Font;
    EFI_MEMORY_DESCRIPTOR* map;
    uint64_t mapSize;
    uint64_t mapDescriptorSize;
} BootInfo;

#endif
