#ifndef GDT_HPP
#define GDT_HPP

#include <stdint.h>

struct GDTDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t accessByte;
    uint8_t limit1Flag;
    uint8_t base2;
} __attribute__((packed));

struct GDT {
    GDTEntry null;
    GDTEntry kernelCode;
    GDTEntry kernelData;
    GDTEntry userNull;
    GDTEntry userCode;
    GDTEntry userData;
} __attribute__((packed)) __attribute((aligned(0x1000)));

extern GDT defaultGDT;

extern "C" void loadGDT(GDTDescriptor *gdtDescriptor);

#endif
