#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stdint.h>

#include "EFIMemory.hpp"

uint64_t getMemorySize(EFI_MEMORY_DESCRIPTOR* map, uint64_t mapEntries,
                       uint64_t descriptorSize);

#endif
