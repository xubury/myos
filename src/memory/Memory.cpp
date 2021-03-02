#include "Memory.hpp"

uint64_t getMemorySize(EFIMemoryDescriptor *map, uint64_t mapSize,
                       uint64_t descriptorSize) {
    static uint64_t memoryBytes = 0;
    if (memoryBytes > 0) {
        return memoryBytes;
    }

    uint64_t entries = mapSize / descriptorSize;
    for (uint64_t i = 0; i < entries; ++i) {
        EFIMemoryDescriptor *desc =
            (EFIMemoryDescriptor *)((uint64_t)map + (i * descriptorSize));
        memoryBytes += (double)desc->numberOfPages * 4096;
    }
    return memoryBytes;
}

void memset(void *start, uint8_t value, uint64_t nums) {
    for (size_t i = 0; i < nums; ++i) {
        *(uint8_t *)((uint64_t)start + i) = value;
    }
}
