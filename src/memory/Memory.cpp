#include "Memory.hpp"

uint64_t getMemorySize(EFI_MEMORY_DESCRIPTOR *map, uint64_t mapEntries,
                       uint64_t descriptorSize) {
    static uint64_t memoryBytes = 0;
    if (memoryBytes > 0) {
        return memoryBytes;
    }

    for (uint64_t i = 0; i < mapEntries; ++i) {
        EFI_MEMORY_DESCRIPTOR *desc =
            (EFI_MEMORY_DESCRIPTOR *)((uint64_t)map + (i * descriptorSize));
        memoryBytes += (double)desc->NumberOfPages * 4096;
    }
    return memoryBytes;
}

void memset(void *start, uint8_t value, uint64_t nums) {
    for (size_t i = 0; i < nums; ++i) {
        *(uint8_t *)((uint64_t)start + i) = value;
    }
}
