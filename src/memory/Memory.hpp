#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "EFIMemory.h"

uint64_t getMemorySize(EFIMemoryDescriptor* map, uint64_t mapSize,
                       uint64_t descriptorSize);

void memset(void* start, uint8_t value, uint64_t nums);

#endif
